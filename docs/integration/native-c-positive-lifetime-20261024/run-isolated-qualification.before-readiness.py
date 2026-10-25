"""External test observer only. No SCM, LAB paths, retries, kill or product entry."""
from pathlib import Path
import ctypes as c
from ctypes import wintypes as w
import subprocess,os,json,hashlib,time,re,sys
root=Path(__file__).resolve().parents[3]
pkg=Path(__file__).resolve().parent
out=root/'build/native-c-positive-lifetime-20261024/runs'
# One immutable test campaign; refuses existing names, never adopts prior objects.
campaign='qualification-'+str(time.time_ns())
base=out/campaign;base.mkdir(parents=True,exist_ok=False)
k=c.WinDLL('kernel32',use_last_error=True)
k.OpenProcess.argtypes=[w.DWORD,w.BOOL,w.DWORD];k.OpenProcess.restype=w.HANDLE
k.GetProcessHandleCount.argtypes=[w.HANDLE,c.POINTER(w.DWORD)];k.GetProcessHandleCount.restype=w.BOOL
k.CloseHandle.argtypes=[w.HANDLE];k.CloseHandle.restype=w.BOOL
records=[];observations=[];manifest={}
for profile in ['Selected','Legacy']:
 for config in ['Debug','Release']:
  exe=root/'build/native-c-positive-lifetime-20261024'/profile/config/'fixture.exe'
  manifest[str(exe.relative_to(root))]=hashlib.sha256(exe.read_bytes()).hexdigest()
  cases=[('clean',pss,rep) for pss in [0,1] for rep in range(3)]
  cases += [(case,1,0) for case in ['acquire-fail','close-unknown','saturation','stop','stale','terminated','alive']]
  for case,pss,rep in cases:
   label=f'{profile}-{config}-{case}-pss{pss}-r{rep}'
   run=base/label;run.mkdir(exist_ok=False)
   env=os.environ.copy();env['NATIVE_FIXTURE_ACK']='1'
   command=[str(exe),str(run),case,str(pss)]
   process=subprocess.Popen(command,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,env=env,creationflags=0x08000000)
   query=k.OpenProcess(0x1000|0x100000,False,process.pid)
   if not query:raise RuntimeError(('observer OpenProcess',process.pid,c.get_last_error(),'NO_KILL_NO_RETRY'))
   lines=[];seen=0
   try:
    for line in iter(process.stdout.readline,b''):
     lines.append(line)
     if line.startswith(b'SAMPLE '):
      fields=dict(re.findall(r'(\w+)=([^\s]+)',line.decode('ascii',errors='strict')))
      count=w.DWORD()
      if not k.GetProcessHandleCount(query,c.byref(count)):raise RuntimeError(('observer query',c.get_last_error(),'NO_KILL_NO_RETRY'))
      observations.append(dict(run=label,phase=fields['phase'],internal=int(fields['handles']),external=count.value,seq=int(fields['seq'])))
      process.stdin.write(b'\n');process.stdin.flush();seen+=1
    process.stdout.close();process.stdin.close()
    code=process.wait(timeout=5) # Timeout raises; NEVER kills or retries.
   finally:
    if not k.CloseHandle(query):raise RuntimeError(('observer CloseHandle',c.get_last_error(),'UNKNOWN_NO_RETRY'))
   output=b''.join(lines);log=pkg/(label+'.log');log.write_bytes(output)
   text=output.decode('ascii',errors='strict')
   result=next((dict(re.findall(r'(\w+)=([^\s]+)',l)) for l in text.splitlines() if l.startswith('RESULT ')),{})
   expected=2 if case=='close-unknown' else 0
   good=(code==expected and bool(result) and seen>0)
   if case=='close-unknown':good=good and result.get('retained')=='1' and result.get('first')=='1816' and 'A1_UNKNOWN injected_close_not_called=1' in text
   else:good=good and result.get('own_references_closed')=='1' and result.get('retained')=='0'
   if case in ['clean','alive']:good=good and result.get('first')=='0' and result.get('child_quiescent')=='1'
   records.append(dict(run=label,profile=profile,config=config,case=case,pss=pss,replica=rep,pid=process.pid,command=command,exit=code,expected=expected,passed=good,result=result,log=log.name))
   (pkg/'run-results.json').write_text(json.dumps(records,indent=2),encoding='utf-8')
   (pkg/'external-observations.json').write_text(json.dumps(observations,indent=2),encoding='utf-8')
   print(label,'PASS' if good else 'FAIL',result,flush=True)
   if not good:raise RuntimeError(('fixture failed',label,'STOP_NO_RETRY'))
(pkg/'campaign-manifest.json').write_text(json.dumps(dict(campaign=campaign,tool=sys.executable,python=sys.version,binaries=manifest,runs=len(records),external_query_handles='ONE_PER_RUN_IN_OBSERVER_TABLE_CLOSED',no_kill=True),indent=2),encoding='utf-8')
assert len(records)==52 and all(r['passed'] for r in records)
print('ALL_52_NATIVE_ISOLATED_CASES_PASS; NO_OPERATIONAL_ADMISSION')
