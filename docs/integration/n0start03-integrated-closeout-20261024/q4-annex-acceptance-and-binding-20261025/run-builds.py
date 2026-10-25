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
