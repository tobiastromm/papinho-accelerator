from pathlib import Path
import subprocess,json
p=Path(__file__).resolve().parent;rows=[]
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  log=p/f'build-{profile}-{cfg}.log'
  with log.open('wb') as f:q=subprocess.run(['cmd','/c',str(p/f'build-{profile}-{cfg}.cmd')],stdout=f,stderr=subprocess.STDOUT)
  rows.append({'profile':profile,'config':cfg,'exit':q.returncode,'log':log.name});(p/'build-results.json').write_text(json.dumps(rows,indent=2));print(profile,cfg,q.returncode,flush=True)
  if q.returncode:print(log.read_text(errors='replace')[-2500:]);raise SystemExit(1)
