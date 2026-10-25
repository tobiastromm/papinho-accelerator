from pathlib import Path
import hashlib,json,subprocess,re
r=Path.cwd();p=Path(__file__).resolve().parent;image=r/'build/q4-binding-20261025/Selected/Release/papacc_server_lab_host.exe';h=lambda f:hashlib.sha256(f.read_bytes()).hexdigest();sha=h(image);f=r/'tools/qualification/scm-q4-image-pin.h';s=f.read_text();f.write_text(re.sub(r'[0-9a-f]{64}',sha,s));rows=[]
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  b=r/'build/q4-binding-20261025'/profile/cfg
  recipe='@echo off\nset VSLANG=1033\ncall "C:\\Program Files (x86)\\Microsoft Visual Studio\\18\\BuildTools\\VC\\Auxiliary\\Build\\vcvars64.bat"\nif errorlevel 1 exit /b %errorlevel%\ncmake --build '+b.as_posix()+' --parallel 2 --target papacc_scm_instrumented_lab\nexit /b %errorlevel%\n'
  script=p/f'final-qualifier-{profile}-{cfg}.cmd';script.write_text(recipe);log=p/f'final-qualifier-{profile}-{cfg}.log'
  with log.open('wb') as out:q=subprocess.run(['cmd','/c',str(script)],stdout=out,stderr=subprocess.STDOUT)
  warnings=len(re.findall(r'warning C[0-9]+|warning LNK[0-9]+',log.read_text(errors='replace')))
  assert q.returncode==0 and warnings==0,(profile,cfg,log)
  rows.append(dict(profile=profile,config=cfg,exit=q.returncode,warnings=warnings,image_sha256=h(b/'papacc_server_lab_host.exe'),qualifier_sha256=h(b/'papacc_scm_instrumented_lab.exe')));print('FINAL_PASS',profile,cfg,flush=True)
assert h(image)==sha,'frozen image changed during qualifier-only rebuild'
(p/'final-artifacts.json').write_text(json.dumps(dict(image_pin=sha,launcher_sha256=h(r/'tools/qualification/scm-n0-protected-single-powershell.ps1'),builds=rows,status='OFFLINE_BINDING_ONLY_NOT_ACTIVATED'),indent=2))
