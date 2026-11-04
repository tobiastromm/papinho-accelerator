from pathlib import Path
import subprocess,json,re
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/d0-d9-bounded-diagnostic-policy-20261026';br=Path('C:/Projetos/PapinhoBrowser');rows=[]
commands=json.loads((r/'build/q4-target-evidence-20261025/Selected/Release/compile_commands.json').read_text());c=next(x['command'] for x in commands if x['file'].endswith('target_typed_evidence_test.c'));incs=' '.join(re.findall(r'-I[^ ]+',c))
for target in ['C3-MSVC-x64','Browser-VC6-x86']:
 for cfg in ['Debug','Release']:
  out=r/'build/d0-d9-bounded-diagnostic-20261026/getter-fixtures'/target/cfg;out.mkdir(parents=True,exist_ok=True);exe=out/'actual-getter-test.exe'
  vc='C:/Program Files (x86)/Microsoft Visual Studio/18/BuildTools/VC/Auxiliary/Build/vcvars64.bat' if target.startswith('C3') else 'C:/MSVC600-master/VC98/Bin/VCVARS32.BAT'
  include=incs if target.startswith('C3') else '/I"'+str(br/'src')+'" /I"'+str(br/'third_party/pst-sdk-0.6.5-normal-4dbd2783/include')+'"'
  source=p/('c3-actual-getter-test.c' if target.startswith('C3') else 'browser-actual-getter-test.c')
  cmd='@echo off\ncall "'+vc+'"\nif errorlevel 1 exit /b %errorlevel%\ncl /nologo /MD /WX '+('/W4 ' if target.startswith('C3') else '/W3 ')+('/Od /Z7 ' if cfg=='Debug' else '/O2 ')+include+' /Fo"'+str(out/'test.obj')+'" /Fd"'+str(out/'test.pdb')+'" /Fe"'+str(exe)+'" "'+str(source)+'" /link /incremental:no\nexit /b %errorlevel%\n'
  batch=p/(target+'-'+cfg+'.cmd');batch.write_text(cmd);q=subprocess.run(['cmd','/c',str(batch)],cwd=out,capture_output=True);log=q.stdout+q.stderr;(p/(target+'-'+cfg+'.build.log')).write_bytes(log);warnings=len(re.findall(rb'warning (?:C|LNK)[0-9]+',log))
  if q.returncode or warnings:print(log.decode(errors='replace'));raise SystemExit(1)
  q=subprocess.run([str(exe)],cwd=out,capture_output=True);(p/(target+'-'+cfg+'.test.log')).write_bytes(q.stdout+q.stderr);assert q.returncode==0,(q.stdout+q.stderr).decode(errors='replace');rows.append(dict(target=target,config=cfg,exit=0,warnings=warnings,test_output=q.stdout.decode().strip()));print(rows[-1],flush=True)
(p/'ACTUAL-GETTER-TESTS.json').write_text(json.dumps(rows,indent=2)+'\n')
