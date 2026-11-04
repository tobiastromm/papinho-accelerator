from pathlib import Path
import json,subprocess,re
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/r0-r9-acceptance-and-integration-20261025'
src=p/'layout-only.c';src.write_text('#define main unused_model_main\n#include "../../../../tests/service_runtime_component_test.c"\n#undef main\nint main(void){printf("FULL_FILE_RUNTIME=%zu NATIVE=%zu TOTAL=%zu\\n",sizeof(PS_RUNTIME),sizeof(PS_NATIVE),papacc_server_service_storage_bytes());return 0;}\n',encoding='utf-8')
rows=[]
for cfg in ['Debug','Release']:
 b=r/'build/r0-r9-integration-20261025/Selected'/cfg
 cmds=json.loads((b/'compile_commands.json').read_text())
 original=next(x['command'] for x in cmds if x['file'].endswith('service_virtual_user_r0r9_test.c'))
 link=subprocess.check_output(['ninja','-C',str(b),'-t','commands','papacc_service_virtual_user_r0r9_test'],text=True).splitlines()[-1]
 libs=link.split('/subsystem:console ',1)[1].split(' && cd .',1)[0]
 compiler=original.split(' ',1)[0]
 for variant in ['default','R0R9']:
  out=(p/f'layout-{cfg}-{variant}').as_posix()
  cmd=re.sub(r'/Fo\S+', '/Fo'+out+'.obj',original);cmd=re.sub(r'/Fd\S+', '/Fd'+out+'.pdb',cmd);cmd=cmd.rsplit(' -c ',1)[0]+' -c '+str(src)
  if variant=='default':cmd=cmd.replace('-DPAPACC_R0R9_VIRTUAL_USER_LAB=1','')
  batch=p/f'layout-{cfg}-{variant}.cmd';batch.write_text('@echo off\ncall "C:\\Program Files (x86)\\Microsoft Visual Studio\\18\\BuildTools\\VC\\Auxiliary\\Build\\vcvars64.bat"\n'+cmd+'\nif errorlevel 1 exit /b %errorlevel%\n'+compiler.rsplit('\\',1)[0]+'\\link.exe /nologo /subsystem:console /out:'+out+'.exe '+out+'.obj '+libs+'\n',encoding='utf-8')
  q=subprocess.run(['cmd','/c',str(batch)],cwd=b,capture_output=True);log=q.stdout.decode(errors='replace')+q.stderr.decode(errors='replace');(p/f'layout-{cfg}-{variant}.log').write_text(log,encoding='utf-8')
  if q.returncode:print(log);raise SystemExit(1)
  q=subprocess.run([out+'.exe'],capture_output=True);rows.append(dict(config=cfg,variant=variant,exit=q.returncode,output=q.stdout.decode()));print(rows[-1])
(p/'COMPILED-STORAGE.json').write_text(json.dumps(rows,indent=2)+'\n',encoding='utf-8')
