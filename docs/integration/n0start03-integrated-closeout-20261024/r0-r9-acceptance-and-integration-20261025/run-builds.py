from pathlib import Path
import subprocess,json,re,hashlib
r=Path.cwd();p=Path(__file__).resolve().parent;rows=[]
for pr in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  log=p/f'build-{pr}-{cfg}.log'
  with log.open('wb') as f:q=subprocess.run(['cmd','/c',str(p/f'build-{pr}-{cfg}.cmd')],stdout=f,stderr=subprocess.STDOUT)
  text=log.read_text(errors='replace');warnings=len(re.findall(r'warning (?:C|LNK)[0-9]+',text));rows.append(dict(profile=pr,config=cfg,exit=q.returncode,warnings=warnings));(p/'BUILD-RESULTS.json').write_text(json.dumps(rows,indent=2)+'\n');print(pr,cfg,q.returncode,warnings,flush=True)
  if q.returncode or warnings:print(text[-5000:],flush=True);raise SystemExit(1)
