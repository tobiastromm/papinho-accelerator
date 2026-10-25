"""One read-only local host query; no SCM, account, ACL, network or service start."""
import ctypes,json,time,datetime,hashlib,subprocess
from ctypes import wintypes as W
from pathlib import Path
class MEMORY(ctypes.Structure):
 _fields_=[('length',W.DWORD),('load',W.DWORD)]+[(s,ctypes.c_ulonglong) for s in ['total_phys','avail_phys','total_pagefile','avail_pagefile','total_virtual','avail_virtual','avail_extended_virtual']]
class PERFORMANCE(ctypes.Structure):
 _fields_=[('cb',W.DWORD)]+[(s,ctypes.c_size_t) for s in ['commit_total','commit_limit','commit_peak','physical_total','physical_available','system_cache','kernel_total','kernel_paged','kernel_nonpaged','page_size']]+[(s,W.DWORD) for s in ['handles','processes','threads']]
def main():
 start=time.perf_counter_ns();utc=datetime.datetime.now(datetime.timezone.utc).isoformat()
 k=ctypes.WinDLL('kernel32',use_last_error=True);ps=ctypes.WinDLL('psapi',use_last_error=True)
 k.GlobalMemoryStatusEx.argtypes=[ctypes.POINTER(MEMORY)];k.GlobalMemoryStatusEx.restype=W.BOOL
 ps.GetPerformanceInfo.argtypes=[ctypes.POINTER(PERFORMANCE),W.DWORD];ps.GetPerformanceInfo.restype=W.BOOL
 k.GetTickCount64.restype=ctypes.c_ulonglong
 m=MEMORY();m.length=ctypes.sizeof(m);perf=PERFORMANCE();perf.cb=ctypes.sizeof(perf)
 okm=k.GlobalMemoryStatusEx(ctypes.byref(m));em=0 if okm else ctypes.get_last_error()
 okp=ps.GetPerformanceInfo(ctypes.byref(perf),ctypes.sizeof(perf));ep=0 if okp else ctypes.get_last_error()
 tick=k.GetTickCount64();osv=__import__('sys').getwindowsversion()
 command="$ErrorActionPreference='Stop'; $e=Get-WinEvent -FilterHashtable @{LogName='System';ProviderName='Microsoft-Windows-Kernel-General';Id=12} -MaxEvents 1; $x=[xml]$e.ToXml(); [ordered]@{record=$e.RecordId;computer=[string]$x.Event.System.Computer;time_utc=$e.TimeCreated.ToUniversalTime().ToString('o');data=@($x.Event.EventData.Data | ForEach-Object { [ordered]@{name=$_.Name;value=$_.InnerText} })} | ConvertTo-Json -Depth 5 -Compress"
 proc=subprocess.run([r'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe','-NoProfile','-NonInteractive','-Command',command],capture_output=True,text=True,encoding='utf-8',errors='strict')
 boot=None;boot_error=None
 if proc.returncode:boot_error={'exit':proc.returncode,'stderr':proc.stderr}
 else:
  boot=json.loads(proc.stdout);computer=boot.pop('computer');boot['computer_sha256']=hashlib.sha256(computer.encode('utf-8')).hexdigest()
 pins={}
 for name in ['build/session0-diagnostic-runtime-integration-20261023/Selected/Release/papacc_server_lab_host.exe','build/scm-protected-installation-binding-20261022/Selected/Release/papacc.conf']:
  try:pins[name]={'sha256':hashlib.sha256(Path(name).read_bytes()).hexdigest()}
  except OSError as ex:pins[name]={'error':ex.winerror}
 end=time.perf_counter_ns()
 data={'task':'PACC-PHASE6-ED11-B4-FIRST-NATIVE-DIAGNOSTIC-POLICY-1','scope':'ONE_LOCAL_READ_ONLY_QUERY_NOT_ADMISSION','utc_begin':utc,'utc_end':datetime.datetime.now(datetime.timezone.utc).isoformat(),'monotonic_begin_ns':start,'monotonic_end_ns':end,'collection_elapsed_ns':end-start,'os':{'major':osv.major,'minor':osv.minor,'build':osv.build},'pointer_bits':ctypes.sizeof(ctypes.c_void_p)*8,'tick_ms':tick,'memory_error':em,'performance_error':ep,'memory':{s:getattr(m,s) for s,_ in MEMORY._fields_} if okm else None,'host':{s:getattr(perf,s) for s,_ in PERFORMANCE._fields_} if okp else None,'boot':boot,'boot_error':boot_error,'pins':pins,'start_executed':False,'scm_mutations':'NONE','acl_mutations':'NONE'}
 if okp:data['host_commit_available_bytes']=(perf.commit_limit-perf.commit_total)*perf.page_size
 print(json.dumps(data,indent=2));return 0 if okm and okp and boot is not None and all('sha256' in x for x in pins.values()) else 2
if __name__=='__main__':raise SystemExit(main())
