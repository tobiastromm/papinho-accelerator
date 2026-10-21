"""Focused offline synthetic oracles and native observation of this Python host.
No Browser/Accelerator/HTTPS execution. Block/close faults are synthetic records.
"""
import copy, ctypes as C, hashlib, importlib.util, json, os, subprocess, sys, unittest
from ctypes import wintypes as W
from pathlib import Path
sys.dont_write_bytecode=True
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('pair',ROOT/'tools/offline/mode_c_pair.py')
m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
def fixture():
    sample=dict(event='EXTERNAL_C1_SAMPLE',seq=1,pid=2,creation=3,observed_ms=10,completed_ms=11,phase_association='INCOMPLETE',provider=None,kernel_max=None)
    for k in ('private_usage','working_set','peak_working_set','pagefile_usage','peak_pagefile_usage','paged_pool_quota_usage','nonpaged_pool_quota_usage','handles','physical_available','commit_available'):sample[k]=1
    rows=[dict(event='OBSERVER_BEGIN',run='fixture'),dict(event='PROCESS_IDENTITY',pid=2,creation=3,observed_file_sha256='a'*64),dict(event='QUERY_BEGIN',seq=1,observer_ms=10),sample]
    rows += [dict(event='OBSERVER_CLOSE',resource=n,closed=True,retained=False) for n in ('observed_image_file','observer_process_handle')]
    rows += [dict(event='OBSERVER_END',capture_complete=True,lifecycle='HANDLE_CLOSURES_CONFIRMED')]
    return rows,dict(run='fixture',process=2,creation=3,image='a'*64)
def run(rows,receipt=None,digest=None):
    if receipt is None:receipt=fixture()[1]
    raw=b'\n'.join(m.canonical(r) for r in rows)+b'\n'
    return m.correlate_external_resources(raw,digest or hashlib.sha256(raw).hexdigest(),receipt)
class Tests(unittest.TestCase):
    def test_valid_resource_join_without_phase(self):
        r=run(fixture()[0]);self.assertEqual(r['process_correlation'],'CONSISTENT_REPORTED_HANDLE_FILE_OBSERVATION');self.assertEqual(r['status'],'INCOMPLETE');self.assertFalse(r['ED11_PASS'])
    def test_pid_reuse(self):
        rows,r=fixture();r['creation']=4;self.assertEqual(run(rows,r)['status'],'INVALID')
    def test_image(self):
        rows,r=fixture();r['image']='b'*64;self.assertEqual(run(rows,r)['status'],'INVALID')
    def test_integrity(self):self.assertEqual(run(fixture()[0],digest='0'*64)['status'],'INVALID')
    def test_blocked_query(self):
        r=run(fixture()[0][:3]);self.assertEqual(r['observer_lifecycle'],'PENDING');self.assertEqual(r['reason'],'QUERY_OR_OBSERVER_PENDING')
    def test_measurement_failure(self):
        rows,r=fixture();rows[3]=dict(event='QUERY_INCOMPLETE',seq=1,errors=[5,0,0,0],observer_ms=11);self.assertEqual(run(rows,r)['reason'],'OBSERVATION_FAILED')
    def test_retained_handle(self):
        rows,r=fixture();rows[4].update(closed=False,retained=True);self.assertEqual(run(rows,r)['observer_lifecycle'],'RETAINED')
    def test_double_close(self):
        rows,r=fixture();rows.insert(-1,copy.deepcopy(rows[4]));self.assertEqual(run(rows,r)['status'],'INVALID')
    def test_no_identity(self):self.assertEqual(run([fixture()[0][0]])['reason'],'OBSERVER_IDENTITY_ABSENT')
    def test_clock(self):
        rows,r=fixture();rows[3]['completed_ms']=9;self.assertEqual(run(rows,r)['status'],'INVALID')
    def test_no_phase_promotion(self):
        rows,r=fixture();rows[3]['phase_association']='F';self.assertEqual(run(rows,r)['status'],'INVALID')
    def test_no_opaque_promotion(self):
        rows,r=fixture();rows[3]['kernel_max']=0;self.assertEqual(run(rows,r)['status'],'INVALID')
    def test_missing_completion(self):
        rows,r=fixture();del rows[3];self.assertEqual(run(rows,r)['observer_lifecycle'],'PENDING')
    def test_nondict_record(self):
        rows,r=fixture();rows[2]=[];self.assertEqual(run(rows,r)['status'],'INVALID')
    def test_unknown_record(self):
        rows,r=fixture();rows[2]['event']='APPROVE';self.assertEqual(run(rows,r)['status'],'INVALID')
    def test_duplicate_end(self):
        rows,r=fixture();rows.append(copy.deepcopy(rows[-1]));self.assertEqual(run(rows,r)['status'],'INVALID')
    def test_no_lifecycle_authority_mutation(self):
        rows,r=fixture();r.update(current=True,epoch=9,credit=123,stopping=True)
        before=copy.deepcopy(r);result=run(rows[:3],r)
        self.assertEqual(r,before);self.assertEqual(result['observer_lifecycle'],'PENDING');self.assertEqual(result['admission'],'DENY')
    def test_local_teardown_not_inferred(self):
        rows,r=fixture();rows.insert(-1,dict(event='PROCESS_EXIT_OBSERVED',local_teardown=None));result=run(rows,r);self.assertFalse(result['native_qualified']);self.assertEqual(result['admission'],'DENY')

def native(executable,directory):
    k=C.WinDLL('kernel32',use_last_error=True)
    k.GetCurrentProcess.restype=W.HANDLE
    k.DuplicateHandle.argtypes=[W.HANDLE,W.HANDLE,W.HANDLE,C.POINTER(W.HANDLE),W.DWORD,W.BOOL,W.DWORD];k.DuplicateHandle.restype=W.BOOL
    k.GetProcessTimes.argtypes=[W.HANDLE]+[C.POINTER(W.FILETIME)]*4;k.GetProcessTimes.restype=W.BOOL
    k.CloseHandle.argtypes=[W.HANDLE];k.CloseHandle.restype=W.BOOL
    selfh=k.GetCurrentProcess();times=[W.FILETIME() for _ in range(4)]
    assert k.GetProcessTimes(selfh,*[C.byref(v) for v in times])
    creation=(times[0].dwHighDateTime<<32)|times[0].dwLowDateTime
    directory=Path(directory).resolve();assert directory.is_relative_to(ROOT/'build');directory.mkdir(parents=True,exist_ok=False)
    results=[]
    for name,rights,expected_birth,expected_code in [('native',0x00100410,creation,0),('stale',0x00100410,creation+1,1),('access-failure',0x00001000,creation,1)]:
        duplicate=W.HANDLE();assert k.DuplicateHandle(selfh,selfh,selfh,C.byref(duplicate),rights,True,0)
        log=directory/(name+'.jsonl');trace=directory/(name+'.stdio')
        try:
            with trace.open('xb') as f:
                process=subprocess.Popen([str(Path(executable).resolve()),'--handle',str(duplicate.value),str(os.getpid()),str(expected_birth),'offline-'+name,str(log),'2','1'],stdin=subprocess.DEVNULL,stdout=f,stderr=f,close_fds=False)
                # On unresolved wait, never kill observer or target to conclude test.
                try:code=process.wait(timeout=10)
                except subprocess.TimeoutExpired:raise AssertionError('OBSERVER_LIFECYCLE_PENDING_NO_TERMINATION')
            assert code==expected_code,(name,code,trace.read_text())
        finally:assert k.CloseHandle(duplicate)
        raw=log.read_bytes();rows=[json.loads(l) for l in raw.splitlines()]
        identity=next((r for r in rows if r['event']=='PROCESS_IDENTITY'),None)
        receipt=dict(run='offline-'+name,process=os.getpid(),creation=creation,image=identity['observed_file_sha256'] if identity else None)
        result=m.correlate_external_resources(raw,hashlib.sha256(raw).hexdigest(),receipt)
        if name=='native':
            assert result['samples']==2 and result['process_correlation'].startswith('CONSISTENT'),result
            assert hashlib.sha256(Path(sys.executable).read_bytes()).hexdigest()==receipt['image']
            assert 'PROCESS_EXIT_OBSERVED' not in [r['event'] for r in rows]
        if name=='stale':assert result['status']=='INVALID'
        if name=='access-failure':assert result['reason']=='OBSERVER_IDENTITY_ABSENT',result
        assert result['admission']=='DENY' and not result['ED11_PASS']
        results.append(dict(case=name,code=code,result=result))
    # Real exited process object remains identified through its held handle.
    child=subprocess.Popen([sys.executable,'-c','pass'],stdin=subprocess.DEVNULL,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
    assert child.wait(timeout=10)==0
    values=[W.FILETIME() for _ in range(4)]
    assert k.GetProcessTimes(W.HANDLE(int(child._handle)),*[C.byref(v) for v in values])
    ended_birth=(values[0].dwHighDateTime<<32)|values[0].dwLowDateTime
    duplicate=W.HANDLE();assert k.DuplicateHandle(selfh,W.HANDLE(int(child._handle)),selfh,C.byref(duplicate),0x00100410,True,0)
    try:
        log=directory/'exited.jsonl'
        with (directory/'exited.stdio').open('xb') as f:
            observer=subprocess.Popen([str(Path(executable).resolve()),'--handle',str(duplicate.value),str(child.pid),str(ended_birth),'offline-exited',str(log),'2','1'],stdin=subprocess.DEVNULL,stdout=f,stderr=f,close_fds=False)
            assert observer.wait(timeout=10)==1
        rows=[json.loads(l) for l in log.read_bytes().splitlines()]
        assert any(r['event']=='PROCESS_EXIT_OBSERVED' and r['local_teardown'] is None for r in rows)
        assert not any(r['event']=='EXTERNAL_C1_SAMPLE' for r in rows)
        results.append(dict(case='exited',local_teardown=None,native_qualified=False))
    finally:assert k.CloseHandle(duplicate)
    (directory/'results.json').write_text(json.dumps(results,indent=2))
    print('NATIVE_SELF_HOST_CASES=4_PASS_TARGET_IS_PYTHON_NOT_PRODUCT')
if __name__=='__main__':
    if len(sys.argv)>1 and sys.argv[1]=='--native':native(sys.argv[2],sys.argv[3])
    else:unittest.main()