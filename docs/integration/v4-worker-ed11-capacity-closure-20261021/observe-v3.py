import ctypes as C,ctypes.wintypes as W,pathlib,subprocess,time,json,hashlib,os
s=pathlib.Path(__file__).parent;r=pathlib.Path(r"C:\Projetos\PapinhoAccelerator")
class PMC(C.Structure):
 _fields_=[('cb',W.DWORD),('PageFaultCount',W.DWORD)]+[(n,C.c_size_t) for n in ['PeakWorkingSetSize','WorkingSetSize','QuotaPeakPagedPoolUsage','QuotaPagedPoolUsage','QuotaPeakNonPagedPoolUsage','QuotaNonPagedPoolUsage','PagefileUsage','PeakPagefileUsage','PrivateUsage']]
ps=C.WinDLL('psapi',use_last_error=True);k=C.WinDLL('kernel32',use_last_error=True)
ps.GetProcessMemoryInfo.argtypes=[W.HANDLE,C.POINTER(PMC),W.DWORD];ps.GetProcessMemoryInfo.restype=W.BOOL
k.GetProcessHandleCount.argtypes=[W.HANDLE,C.POINTER(W.DWORD)];k.GetProcessHandleCount.restype=W.BOOL
results=[]
for mode in ['Debug','Release']:
 exe=r/'build/v3-current-e0-20261021/Selected'/mode/'papacc_v3_transaction_worker_test.exe'
 for case in ['success','conflict','unknown','stop']:
  dest=s/(mode+'-'+case+'-fixture.bin');assert not dest.exists()
  log=s/(mode+'-'+case+'-native.log')
  with log.open('wb') as out:
   p=subprocess.Popen([str(exe),case,str(dest)],stdout=out,stderr=subprocess.STDOUT,creationflags=subprocess.CREATE_NO_WINDOW)
   maxima={};samples=0;queries_failed=0;start=time.monotonic()
   while True:
    m=PMC();m.cb=C.sizeof(m)
    if ps.GetProcessMemoryInfo(W.HANDLE(int(p._handle)),C.byref(m),m.cb):
     samples+=1
     for n,_ in PMC._fields_[1:]:maxima[n]=max(maxima.get(n,0),int(getattr(m,n)))
    else:queries_failed+=1
    handles=W.DWORD()
    if k.GetProcessHandleCount(W.HANDLE(int(p._handle)),C.byref(handles)):maxima['sampled_handle_count']=max(maxima.get('sampled_handle_count',0),handles.value)
    rc=p.poll()
    if rc is not None:break
    if time.monotonic()-start>30:raise RuntimeError('fixture still active; retain without termination')
    time.sleep(.001)
   result=dict(configuration=mode,case=case,returncode=rc,executable=str(exe),executable_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),elapsed_seconds=time.monotonic()-start,samples=samples,failed_queries=queries_failed,maxima=maxima,output=log.read_text(errors='replace'),scope='Observed existing v3 synthetic-admin fixture process only; no v4 worker or factual CURRENT. Peak counters include fixture/harness/CRT/provider/runtime; samples are not hard upper bounds, managed credit or incremental attribution.')
   results.append(result);print(mode,case,rc,samples,maxima.get('PeakWorkingSetSize'),flush=True)
(s/'native-process-observations.json').write_text(json.dumps(results,indent=2))
