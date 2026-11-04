from pathlib import Path
import subprocess,json,re,shutil
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/composite-target-quantitative-closure-20261026';src=p/'runtime-core-observation.c';rows=[]
for backend in ['OpenSSL-x64','NSS-VC6-x86']:
 sdk=(r/'build/ed08-r4-integrated-20261012/sdk-normal') if backend=='OpenSSL-x64' else Path('C:/Projetos/PapinhoBrowser/third_party/pst-sdk-0.6.5-normal-4dbd2783')
 target='win32-x64-msvc-19.51-openssl3' if backend=='OpenSSL-x64' else 'win32-x86-vc6-retrozilla-nss-md'
 for cfg in ['Debug','Release']:
  out=r/'build/composite-target-quantitative-20261026'/backend/cfg;out.mkdir(parents=True,exist_ok=True)
  for f in (sdk/'runtime'/target).iterdir():
   if f.is_file():shutil.copyfile(f,out/f.name)
  vc='C:\\Program Files (x86)\\Microsoft Visual Studio\\18\\BuildTools\\VC\\Auxiliary\\Build\\vcvars64.bat' if backend=='OpenSSL-x64' else 'C:\\MSVC600-master\\VC98\\Bin\\VCVARS32.BAT'
  libs='"'+str(sdk/'lib'/target/'papinho_secure_transport.lib')+'" '
  libs+=('"'+str(sdk/'lib'/target/'libssl.lib')+'" "'+str(sdk/'lib'/target/'libcrypto.lib')+'" ws2_32.lib crypt32.lib advapi32.lib' if backend=='OpenSSL-x64' else 'wsock32.lib')
  cmd='@echo off\nset VSLANG=1033\ncall "'+vc+'"\nif errorlevel 1 exit /b %errorlevel%\ncl /nologo /MD /WX '+('/W4 ' if backend=='OpenSSL-x64' else '/W3 /DQUANT_NSS=1 ')+('/Od /Z7 ' if cfg=='Debug' else '/O2 ')+'/I"'+str(sdk/'include')+'" /Fo"'+str(out/'fixture.obj')+'" /Fd"'+str(out/'fixture.pdb')+'" /Fe"'+str(out/'runtime-core-observation.exe')+'" "'+str(src)+'" '+libs+' /link /subsystem:console /incremental:no\nexit /b %errorlevel%\n';batch=p/(backend+'-'+cfg+'.cmd');batch.write_text(cmd,encoding='utf-8');q=subprocess.run(['cmd','/c',str(batch)],cwd=out,capture_output=True);log=q.stdout+q.stderr;(p/(backend+'-'+cfg+'.build.log')).write_bytes(log);warnings=len(re.findall(rb'warning [A-Z]+[0-9]+',log));rows.append(dict(backend=backend,config=cfg,exit=q.returncode,warnings=warnings));print(rows[-1],flush=True)
  if q.returncode or warnings:print(log.decode(errors='replace'));raise SystemExit(1)
(p/'BUILD-RESULTS.json').write_text(json.dumps(rows,indent=2)+'\n',encoding='utf-8')
