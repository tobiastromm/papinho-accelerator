from pathlib import Path
import subprocess,json
p=Path(__file__).resolve().parent
old=p.parent/'pre-metric-timeout-resolution-20261025'
rows=[]
for row in json.loads((old/'regression-results.json').read_text()):
 profile,cfg,test=row['profile'],row['config'],row['test']
 exe=Path.cwd()/'build/ed11-positive-private-20261025'/profile/cfg/('papacc_'+test+'.exe')
 log=p/('regression-'+profile+'-'+cfg+'-'+test+'.log')
 args=[str(exe)]+(['--model'] if test=='service_n0_sd_test' else [])
 with log.open('wb') as out:
  q=subprocess.Popen(args,stdout=out,stderr=subprocess.STDOUT)
  code=q.wait(timeout=60)
 rows.append(dict(profile=profile,config=cfg,test=test,exit=code,log=log.name))
 if code:print(test,profile,cfg,code,log.read_text(errors='replace'),flush=True)
 (p/'regression-results.json').write_text(json.dumps(rows,indent=2))
print('REGRESSIONS',len(rows),'FAILED',sum(x['exit']!=0 for x in rows),flush=True)
if any(x['exit'] for x in rows):raise SystemExit(1)
