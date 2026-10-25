from pathlib import Path
import subprocess,ctypes,json,time,hashlib
from ctypes import wintypes as W
r=Path(r'C:\Projetos\PapinhoAccelerator');p=Path(r'C:\Projetos\_tmp\PapinhoAccelerator\ed11-native-resource-boundary-20261023')
class PMC(ctypes.Structure):
 _fields_=[('cb',W.DWORD),('PageFaultCount',W.DWORD)]+[(x,ctypes.c_size_t) for x in ['PeakWorkingSetSize','WorkingSetSize','QuotaPeakPagedPoolUsage','QuotaPagedPoolUsage','QuotaPeakNonPagedPoolUsage','QuotaNonPagedPoolUsage','PagefileUsage','PeakPagefileUsage','PrivateUsage']]
k=ctypes.WinDLL('kernel32',use_last_error=True);ps=ctypes.WinDLL('psapi',use_last_error=True);ps.GetProcessMemoryInfo.argtypes=[W.HANDLE,ctypes.POINTER(PMC),W.DWORD];ps.GetProcessMemoryInfo.restype=W.BOOL;k.GetProcessHandleCount.argtypes=[W.HANDLE,ctypes.POINTER(W.DWORD)];out=[]
for profile in ['Selected','Legacy']:
 for config in ['Debug','Release']:
  exe=r/f'build/session0-diagnostic-runtime-integration-20261023/{profile}/{config}/papacc_server_lab_host.exe';log=p/f'host-help-{profile}-{config}.log';samples=[];errors=[]
  with log.open('wb') as f:
   child=subprocess.Popen([str(exe),'--help'],stdout=f,stderr=subprocess.STDOUT,creationflags=subprocess.CREATE_NO_WINDOW)
   begin=time.monotonic()
   while True:
    m=PMC();m.cb=ctypes.sizeof(m);hc=W.DWORD();ok=ps.GetProcessMemoryInfo(W.HANDLE(int(child._handle)),ctypes.byref(m),ctypes.sizeof(m));hok=k.GetProcessHandleCount(W.HANDLE(int(child._handle)),ctypes.byref(hc))
    if ok:samples.append(dict(private=m.PrivateUsage,commit=m.PagefileUsage,peak_commit=m.PeakPagefileUsage,working_set=m.WorkingSetSize,peak_working_set=m.PeakWorkingSetSize,quota_peak_paged=m.QuotaPeakPagedPoolUsage,quota_peak_nonpaged=m.QuotaPeakNonPagedPoolUsage,handles=hc.value if hok else None))
    else:errors.append(ctypes.get_last_error())
    code=child.poll()
    if code is not None:break
    if time.monotonic()-begin>10:raise RuntimeError('STOP_NO_KILL_NO_RETRY '+str(exe))
    time.sleep(.001)
  assert code==0,(profile,config,code)
  out.append(dict(profile=profile,config=config,exit=code,sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),samples=len(samples),query_errors=errors,observed_max={x:max([s[x] for s in samples if s[x] is not None],default=None) for x in samples[0]} if samples else None,scope='EXACT_CANDIDATE_HELP_ONLY_LOADER_CRT_BASELINE_NOT_SERVICE_RUNTIME_OR_START',args=['--help']))
(p/'host-help-process-baselines.json').write_text(json.dumps(out,indent=2));print(json.dumps(out))
