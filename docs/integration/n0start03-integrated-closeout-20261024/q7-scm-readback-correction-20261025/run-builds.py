from pathlib import Path
import subprocess,json,re
p=Path(__file__).resolve().parent
rows=[]
for profile,cfg in [('Selected','Release'),('Selected','Debug'),('Legacy','Release'),('Legacy','Debug')]:
 log=p/f'build-{profile}-{cfg}.log'
 with log.open('wb') as out:q=subprocess.run(['cmd','/c',str(p/f'build-{profile}-{cfg}.cmd')],stdout=out,stderr=subprocess.STDOUT)
 text=log.read_text(errors='replace');warnings=len(re.findall(r'warning C[0-9]+|warning LNK[0-9]+',text))
 rows.append(dict(profile=profile,config=cfg,exit=q.returncode,warnings=warnings,log=log.name));(p/'build-results.json').write_text(json.dumps(rows,indent=2))
 print(profile,cfg,q.returncode,warnings,flush=True)
 if q.returncode or warnings:raise SystemExit(1)
 subprocess.run(['python',str(p/'verify-image-pin-in-binary.py'),'--qualifier',str(Path.cwd()/'build/q7-scm-readback-correction-20261025'/profile/cfg/'papacc_scm_instrumented_lab.exe'),'--image',str(Path.cwd()/'build/q4-final-binding-20261025/Selected/Release/papacc_server_lab_host.exe'),'--pin-header',str(Path.cwd()/'tools/qualification/scm-q4-image-pin.h'),'--forbid','2386d63a256affd48c3dfbcd78ff36380400c86f61a0eeb68e209d6ed4520821'],check=True)

