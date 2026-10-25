from pathlib import Path
import subprocess,json
p=Path(__file__).resolve().parent
root=Path.cwd()
rows=[]
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  log=p/f'isolated-build-{profile}-{cfg}.log'
  with log.open('wb') as out:
   q=subprocess.run(['cmd','/c',str(p/f'build-isolated-{profile}-{cfg}.cmd')],stdout=out,stderr=subprocess.STDOUT)
  rows.append(dict(kind='build',profile=profile,config=cfg,exit=q.returncode,log=log.name))
  if q.returncode:
   (p/'integrated-observation-results.json').write_text(json.dumps(rows,indent=2));print(log.read_text(errors='replace')[-3000:]);raise SystemExit(1)
  for name in ['poststart-identity-observation','q-census']:
   log=p/f'{name}-{profile}-{cfg}.log'
   with log.open('wb') as out:
    q=subprocess.Popen([str(root/'build/ed11-positive-private-20261025'/profile/cfg/(name+'.exe'))],stdout=out,stderr=subprocess.STDOUT)
    code=q.wait(timeout=30)
   rows.append(dict(kind=name,profile=profile,config=cfg,exit=code,log=log.name))
  print(profile,cfg,rows[-1]['exit'],flush=True)
  (p/'integrated-observation-results.json').write_text(json.dumps(rows,indent=2))

if any(x["exit"] for x in rows):raise SystemExit(1)
