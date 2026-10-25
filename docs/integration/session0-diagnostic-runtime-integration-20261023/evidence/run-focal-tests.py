from pathlib import Path
import subprocess,json,hashlib,re,ctypes,time
from ctypes import wintypes as W
r=Path(r'C:\Projetos\PapinhoAccelerator'); t=Path(r'C:\Projetos\_tmp\PapinhoAccelerator\session0-diagnostic-runtime-integration-20261023')
names='papacc_service_host_product_model papacc_service_host_lab_model papacc_service_scm_query_model_test papacc_service_scm_virtual_model_test papacc_service_scm_acl_model_test papacc_service_lifecycle_data_test papacc_service_scm_native_test papacc_server_console_win32_test papacc_personal_effect_plan_test papacc_phase6_memory_census_test papacc_issuer_production_gate_test papacc_service_runtime_model_test papacc_service_runtime_native_test papacc_service_runtime_closed_test papacc_scm_cycle_model psrf-model psrf-native papacc_service_runtime_file_model_test'.split()
class PMC(ctypes.Structure):
 _fields_=[('cb',W.DWORD),('PageFaultCount',W.DWORD)]+[(x,ctypes.c_size_t) for x in ['PeakWorkingSetSize','WorkingSetSize','QuotaPeakPagedPoolUsage','QuotaPagedPoolUsage','QuotaPeakNonPagedPoolUsage','QuotaNonPagedPoolUsage','PagefileUsage','PeakPagefileUsage','PrivateUsage']]
k=ctypes.WinDLL('kernel32',use_last_error=True); p=ctypes.WinDLL('psapi',use_last_error=True)
p.GetProcessMemoryInfo.argtypes=[W.HANDLE,ctypes.POINTER(PMC),W.DWORD]; p.GetProcessMemoryInfo.restype=W.BOOL
k.GetProcessHandleCount.argtypes=[W.HANDLE,ctypes.POINTER(W.DWORD)]; k.GetProcessHandleCount.restype=W.BOOL
out=[]; builds=[]
for profile in ['Selected','Legacy']:
 for config in ['Debug','Release']:
  log=(t/f'build-{profile}-{config}-final.log').read_text(errors='replace')
  issues=[s for s in log.splitlines() if re.search(r'\b(warning [A-Z]+\d+|error [A-Z]+\d+|CMake Warning|FAILED:)\b',s,re.I)]
  builds.append(dict(profile=profile,config=config,issues=issues,sha256=hashlib.sha256(log.encode()).hexdigest()))
  d=r/'build/session0-diagnostic-runtime-integration-20261023'/profile/config
  for name in names:
   exe=d/(name+'.exe'); digest=hashlib.sha256(exe.read_bytes()).hexdigest()
   dest=t/f'{profile}-{config}-{name}.log'; samples=[]; errors=set(); start=time.monotonic()
   args=[str(exe)]
   if name=='psrf-native':
    fixture=Path(r'C:\Projetos\_tmp\PapinhoAccelerator\session0-diagnostic-capture-20261023')/'fixtures'/'runtime-integration-final'/profile/config/'record.bin';fixture.parent.mkdir(parents=True,exist_ok=True);assert not fixture.exists(),str(fixture);args.append(str(fixture))
   with dest.open('wb') as f:
    child=subprocess.Popen(args,stdout=f,stderr=subprocess.STDOUT,creationflags=subprocess.CREATE_NO_WINDOW)
    # Only newly-created, allowlisted no-effect test instances; original process handle.
    while True:
     m=PMC();m.cb=ctypes.sizeof(m);h=W.DWORD(); ok=p.GetProcessMemoryInfo(W.HANDLE(int(child._handle)),ctypes.byref(m),ctypes.sizeof(m)); err=ctypes.get_last_error() if not ok else None
     hok=k.GetProcessHandleCount(W.HANDLE(int(child._handle)),ctypes.byref(h)); herr=ctypes.get_last_error() if not hok else None
     if ok: samples.append(dict(peak_working_set=m.PeakWorkingSetSize,peak_pagefile=m.PeakPagefileUsage,private_usage=m.PrivateUsage,handles=h.value if hok else None))
     if err: errors.add(('GetProcessMemoryInfo',err))
     if herr: errors.add(('GetProcessHandleCount',herr))
     code=child.poll()
     if code is not None: break
     if time.monotonic()-start>40: raise RuntimeError('TIMEOUT_PRESERVE_CHILD_NO_KILL '+str(exe))
     time.sleep(.001)
   out.append(dict(profile=profile,config=config,test=name,sha256=digest,exit=code,seconds=time.monotonic()-start,sample_count=len(samples),observed_maxima={key:max([s[key] for s in samples if s[key] is not None],default=None) for key in ['peak_working_set','peak_pagefile','private_usage','handles']},native_query_errors=sorted(errors),measurement_status='EMPIRICAL_NOT_BOUND_NOT_SERVICE_OR_LAUNCHER_ADMISSION'))
(t/'tests-and-component-measurements.json').write_text(json.dumps(out,indent=2)); (t/'build-review.json').write_text(json.dumps(builds,indent=2))
art=[]
for profile in ['Selected','Legacy']:
 for config in ['Debug','Release']:
  exe=r/'build/session0-diagnostic-runtime-integration-20261023'/profile/config/'papacc_server_lab_host.exe'
  art.append(dict(profile=profile,config=config,path=str(exe),bytes=exe.stat().st_size,sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),status='INTEGRATED_FILE_LAB_BUILT_NOT_INSTALLED_NATIVE_ADMISSION_AND_MANUAL_REBIND_REQUIRED'))
(t/'instrumented-artifacts.json').write_text(json.dumps(art,indent=2))
print(json.dumps(dict(tests=len(out),failed=[x['test'] for x in out if x['exit']],build_issues=sum(len(x['issues']) for x in builds),measurement_errors=sum(len(x['native_query_errors']) for x in out))))
