from pathlib import Path
import subprocess,json,re
p=Path(__file__).resolve().parent;rows=[]
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  f=p/f'build-{profile}-{cfg}.log'
  with f.open('wb') as log:q=subprocess.run(['cmd','/c',str(p/f'build-{profile}-{cfg}.cmd')],stdout=log,stderr=subprocess.STDOUT)
  text=f.read_text(errors='replace');warnings=len(re.findall(r'warning (?:C|LNK)[0-9]+',text));row=dict(profile=profile,config=cfg,exit=q.returncode,warnings=warnings);rows.append(row);(p/'ACCELERATOR-BUILDS.json').write_text(json.dumps(rows,indent=2)+'\n');print(row,flush=True)
  if q.returncode or warnings:print(text[-6000:]);raise SystemExit(1)
