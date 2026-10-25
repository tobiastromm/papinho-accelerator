from pathlib import Path
import subprocess,json,re
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/t0-t9-normative-resolution';results=[]
for cfg in ['Debug','Release']:
 exe=r/'build/ed11-t0t9-20261025/Selected'/cfg/'papacc_noninherited_transport_test.exe'
 for mode in ['clean','wrong','bad-creation','token','bad-server','early','close-unknown','descendant','collision','create-fail','server-close','console','bad-id-text','bad-local-name','replay','peer-exit','ack-write-fail','impersonated']:
  log=p/(cfg+'-'+mode+'-final-qualified.log');si=subprocess.STARTUPINFO();si.dwFlags=subprocess.STARTF_USESHOWWINDOW;si.wShowWindow=0
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
  if mode not in ['wrong','replay','collision','create-fail']:assert 'private_object_matches=0' in text
  results.append({'config':cfg,'mode':mode,'exit':code,'log':log.name})
(p/'native-results.json').write_text(json.dumps(results,indent=2),encoding='utf-8');print(len(results),'noninherited native expected outcomes PASS')

guards=[]
for cfg in ['Debug','Release']:
 exe=r/'build/ed11-t0t9-20261025/Selected'/cfg/'papacc_noninherited_transport_test.exe'
 v=subprocess.run([str(exe),'--guard-model'],capture_output=True,timeout=10)
 (p/(cfg+'-guard-model.log')).write_bytes(v.stdout+v.stderr)
 assert v.returncode==0 and b'checks=20' in v.stdout and b'client_endpoint_opens=0' in v.stdout,(cfg,v.stdout,v.stderr)
 guards.append({'config':cfg,'checks':20,'endpoint_opens':0,'exit':v.returncode})
(p/'guard-results.json').write_text(json.dumps(guards,indent=2),encoding='utf-8')
print('40 additional pure argument/namespace checks PASS; no network attempted')
