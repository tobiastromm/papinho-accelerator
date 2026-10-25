from pathlib import Path
import subprocess,json,sys,hashlib
r=Path(r'C:\Projetos\PapinhoAccelerator'); t=Path(__file__).parent
results=[]
for cfg in ('Debug','Release'):
 exe=r/'build/local-bootstrap-config-cli-20261019'/cfg/'papacc_server.exe'
 base=['config','first-install','--declare','NEW_INSTALL','--source','NOT_CREATED_INITIAL_CONFIG','--parent','C:\\NOT_CREATED_INSTALL_PARENT','--container','new-install']
 cases=[('declared-input-only',base,2),('declaration-absent',base[:2]+base[4:],1),('restore-not-new',[('RESTORE' if v=='NEW_INSTALL' else v) for v in base],1),('update-not-new',[('UPDATE' if v=='NEW_INSTALL' else v) for v in base],1),('caller-sid-not-authority',base+['--operator','S-1-5-32-544'],1),('flag-not-authorization',base+['--authorized'],1),('source-missing',[('' if v=='NOT_CREATED_INITIAL_CONFIG' else v) for v in base],1),('parent-missing',[('' if v=='C:\\NOT_CREATED_INSTALL_PARENT' else v) for v in base],1),('container-too-long',[('a'*65 if v=='new-install' else v) for v in base],1),('source-too-long',[('a'*260 if v=='NOT_CREATED_INITIAL_CONFIG' else v) for v in base],1),('control-in-target',[('bad\nvalue' if v=='new-install' else v) for v in base],1),('target-different',[('different-install' if v=='new-install' else v) for v in base],2),('replay-same-input',base,2),('exact-span-limits',[('a'*259 if v=='NOT_CREATED_INITIAL_CONFIG' or v=='C:\\NOT_CREATED_INSTALL_PARENT' else 'b'*64 if v=='new-install' else v) for v in base],2),('old-apply-denied',['config','apply','--source','NOT_CREATED_INITIAL_CONFIG'],2)]
 for name,args,expected in cases:
  p=subprocess.run([str(exe),*args],capture_output=True,text=True,timeout=15)
  output=p.stdout+p.stderr
  assert p.returncode==expected,(cfg,name,p.returncode,output)
  if args[1]=='first-install':
   assert ('BOOTSTRAP=DENY' in output),(name,output)
   if expected==2: assert 'OPERATOR=UNBOUND; AUTHORIZATION=ABSENT' in output and 'TARGET=NATIVE_UNBOUND' in output and 'CUSTODY=NOT_ACQUIRED' in output
  assert 'NOT_CREATED' not in output and 'S-1-5-32' not in output,(name,'argument disclosed')
  results.append(dict(configuration=cfg,case=name,exit=p.returncode,output=output))
 assert not (r/'NOT_CREATED_INITIAL_CONFIG').exists()
 assert not Path(r'C:\NOT_CREATED_INSTALL_PARENT').exists()
 results.append(dict(configuration=cfg,exe_sha256=hashlib.sha256(exe.read_bytes()).hexdigest()))
(t/'cli-results.json').write_text(json.dumps(results,indent=2))
print('NATIVE_CLI_NEGATIVE_ONLY_PASS',len(results)-2)
