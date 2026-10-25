from pathlib import Path
import subprocess,json,hashlib,time,sys
p=Path(r'C:\Projetos\_tmp\PapinhoAccelerator\ed11-native-resource-boundary-20261023');out=[]
for config in ['Debug','Release']:
 exe=p/f'probe-{config}.exe'
 for run in [1,2,3]:
  parent=p/'fixtures'/f'{config}-{run:02d}';file=parent/'record.bin';log=p/f'measurement-{config}-{run:02d}.jsonl'
  existing=log.exists();start=None if existing else time.time_ns()
  if not existing:
   parent.mkdir(parents=True,exist_ok=False)
   with log.open('wb') as f:
    child=subprocess.Popen([str(exe),str(parent),str(file)],stdout=f,stderr=subprocess.STDOUT,creationflags=subprocess.CREATE_NO_WINDOW)
    try:code=child.wait(timeout=30)
    except subprocess.TimeoutExpired:
     print('STOP_RETAINED_NO_KILL_NO_RETRY',config,run,child.pid,flush=True);raise
  else:code=0
  rows=[json.loads(x) for x in log.read_text(encoding='utf-8').splitlines() if x.strip()];final=rows[-1]
  assert code==0 and final['result']==0 and final['preparer_closed'] and final['writer_closed'] and final['parent_closed'],(config,run,code,final)
  assert all(not row[k] for row in rows[:-1] for k in ['memory_error','host_error','available_error','handles_error','io_error','stack_error'])
  assert file.stat().st_size==2048
  out.append(dict(config=config,run=run,exit=code,start_unix_ns=start,end_unix_ns=None if existing else time.time_ns(),existing_success_receipt_reconciled=existing,exe_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),file=str(file),file_sha256=hashlib.sha256(file.read_bytes()).hexdigest(),logical_bytes=file.stat().st_size,samples=len(rows)-1,result=final))
(p/'measurement-runs.json').write_text(json.dumps(dict(runs=out,windows_version=list(sys.getwindowsversion()),python=sys.version,scope='NO_SERVICE_OWN_PROCESS_MAIN_THREAD_ONLY_EMPIRICAL_NOT_BOUND',clock='OBSERVATION_ONLY_NO_CLOCK_CHANGE'),indent=2),encoding='utf-8')
print(json.dumps(dict(runs=len(out),samples=sum(x['samples'] for x in out),all_pass=True,files_retained=len(out))))
