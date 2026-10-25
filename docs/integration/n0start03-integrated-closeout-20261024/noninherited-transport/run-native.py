from pathlib import Path
import subprocess,json,re
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/noninherited-transport';results=[]
for cfg in ['Debug','Release']:
 exe=r/'build/ed11-noninherit-20261025/Selected'/cfg/'papacc_noninherited_transport_test.exe'
 for mode in ['clean','wrong','bad-creation','token','bad-server','early','close-unknown','descendant','collision','create-fail','server-close','console']:
  log=p/(cfg+'-'+mode+'-verified.log');si=subprocess.STARTUPINFO();si.dwFlags=subprocess.STARTF_USESHOWWINDOW;si.wShowWindow=0
  with log.open('xb') as f:
   q=subprocess.Popen([str(exe),mode],stdin=subprocess.PIPE,stdout=f,stderr=subprocess.STDOUT,startupinfo=si,creationflags=subprocess.CREATE_NEW_CONSOLE if mode=='console' else 0)
   try:code=q.wait(timeout=25)
   except subprocess.TimeoutExpired:
    (p/'RETAINED.json').write_text(json.dumps({'pid':q.pid,'mode':mode,'NO_KILL':True}),encoding='utf-8');raise RuntimeError('UNKNOWN retained no retry')
   q.stdin.close()
  text=log.read_text(encoding='utf-8',errors='replace');assert code==0,(cfg,mode,code,text)
  assert ('NT_FINAL' in text or mode in ['collision','create-fail'])
  if mode not in ['collision','create-fail']:assert text.count('parent_owned_slots=0')==5
  assert text.count('NT_ITERATION=')==5
  if mode not in ['wrong','collision','create-fail']:assert 'private_object_matches=0' in text
  results.append({'config':cfg,'mode':mode,'exit':code,'log':log.name})
(p/'native-results.json').write_text(json.dumps(results,indent=2),encoding='utf-8');print(len(results),'noninherited native expected outcomes PASS')
