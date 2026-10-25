from pathlib import Path
import subprocess,json,re
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/explicit-inheritance-integration';results=[]
for cfg in ['Debug','Release']:
 b=r/'build/ed11-inheritance-integration-20261025/Selected'/cfg
 for mode in ['global','none','console']:
  cmd=[str(b/'papacc_explicit_inheritance_boundary_test.exe'),str(b/'papacc_service_observer_stack_va_isolated_test.exe'),mode]
  si=subprocess.STARTUPINFO();si.dwFlags=subprocess.STARTF_USESHOWWINDOW;si.wShowWindow=0
  logfile=p/(cfg+'-'+mode+'-qualified.log')
  with logfile.open('xb') as log:
   process=subprocess.Popen(cmd,stdin=subprocess.PIPE,stdout=log,stderr=subprocess.STDOUT,startupinfo=si,creationflags=subprocess.CREATE_NEW_CONSOLE if mode=='console' else 0)
   try:code=process.wait(timeout=45)
   except subprocess.TimeoutExpired:
    (p/(cfg+'-'+mode+'-RETAINED.json')).write_text(json.dumps({'pid':process.pid,'UNKNOWN':True,'no_kill':True}),encoding='utf-8');raise RuntimeError('UNKNOWN process retained; NO_KILL_NO_RETRY')
   process.stdin.close()
  text=logfile.read_text(encoding='utf-8',errors='replace');assert code==0,(cfg,mode,code,text[-1500:])
  if mode=='console':
   assert re.search(r'PARENT_CONSOLE_WINDOW value=(\d+)',text)[1]==re.search(r'CHILD_CONSOLE_WINDOW value=(\d+)',text)[1]
  results.append({'config':cfg,'mode':mode,'exit':code,'evidence':logfile.name})
(p/'boundary-results.json').write_text(json.dumps(results,indent=2),encoding='utf-8');print('6 native concurrency/console results PASS; global case EXPECTS exposure and BLOCKS integration')
