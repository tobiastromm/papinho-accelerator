from pathlib import Path
import subprocess,json,re
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/one-op-correlated-diagnostic-design-20261026';rows=[]
for arch in ['MSVC-x64','VC6-x86']:
 sdk=r/'build/ed08-r4-integrated-20261012/sdk-normal' if arch=='MSVC-x64' else Path('C:/Projetos/PapinhoBrowser/third_party/pst-sdk-0.6.5-normal-4dbd2783')
 vc='C:/Program Files (x86)/Microsoft Visual Studio/18/BuildTools/VC/Auxiliary/Build/vcvars64.bat' if arch=='MSVC-x64' else 'C:/MSVC600-master/VC98/Bin/VCVARS32.BAT'
 for cfg in ['Debug','Release']:
  name=arch+'-'+cfg;out=r/'build/one-op-correlated-diagnostic-20261026'/name;out.mkdir(parents=True,exist_ok=True);exe=out/'provider-query-test.exe'
  cmd='@echo off\ncall "'+vc+'"\nif errorlevel 1 exit /b %errorlevel%\ncl /nologo /MD /WX '+('/W4 ' if arch=='MSVC-x64' else '/W3 ')+('/Od /Z7 ' if cfg=='Debug' else '/O2 ')+'/I"'+str(sdk/'include')+'" /Fo"'+str(out/'test.obj')+'" /Fd"'+str(out/'test.pdb')+'" /Fe"'+str(exe)+'" "'+str(p/'owner-provider-readonly-probe-test.c')+'" /link /incremental:no\nexit /b %errorlevel%\n'
  batch=p/(name+'.cmd');batch.write_text(cmd);q=subprocess.run(['cmd','/c',str(batch)],cwd=out,capture_output=True);log=q.stdout+q.stderr;(p/(name+'.build.log')).write_bytes(log);warnings=len(re.findall(rb'warning [A-Z]+[0-9]+',log));assert q.returncode==0 and warnings==0,log.decode(errors='replace')
  q=subprocess.run([str(exe)],cwd=out,capture_output=True);log=q.stdout+q.stderr;(p/(name+'.test.log')).write_bytes(log);assert q.returncode==0,log.decode(errors='replace');rows.append(dict(target=name,build='PASS',warnings=warnings,test='PASS',output=log.decode().strip()));print(rows[-1],flush=True)
(p/'PROBE-QUALIFICATION.json').write_text(json.dumps(rows,indent=2)+'\n')
