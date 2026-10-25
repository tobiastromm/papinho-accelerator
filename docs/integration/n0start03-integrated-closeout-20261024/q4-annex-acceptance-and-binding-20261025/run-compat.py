from pathlib import Path
import subprocess,json,re
p=Path(__file__).resolve().parent;old=p.parent/'q0-q9-acceptance-and-integration-20261025';tests=sorted({x['test'] for x in json.loads((old/'regression-results.json').read_text())});builds=[];runs=[]
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  log=p/f'build-compat-{profile}-{cfg}.log'
  with log.open('wb') as out:q=subprocess.run(['cmd','/c',str(p/f'build-compat-{profile}-{cfg}.cmd')],stdout=out,stderr=subprocess.STDOUT)
  warnings=len(re.findall(r'warning C[0-9]+|warning LNK[0-9]+',log.read_text(errors='replace')));builds.append(dict(profile=profile,config=cfg,exit=q.returncode,warnings=warnings));(p/'compat-build-results.json').write_text(json.dumps(builds,indent=2))
  if q.returncode or warnings:print('BUILD_FAIL',profile,cfg,flush=True);raise SystemExit(1)
  for test in tests:
   exe=Path.cwd()/'build/q4-historical-compatibility-20261025'/profile/cfg/('papacc_'+test+'.exe');log=p/f'compat-{profile}-{cfg}-{test}.log'
   with log.open('wb') as out:q=subprocess.run([str(exe)]+(['--model'] if test=='service_n0_sd_test' else []),stdout=out,stderr=subprocess.STDOUT,timeout=60)
   runs.append(dict(profile=profile,config=cfg,test=test,exit=q.returncode,log=log.name));(p/'compat-regression-results.json').write_text(json.dumps(runs,indent=2))
   if q.returncode:print('FAIL',profile,cfg,test,flush=True);raise SystemExit(1)
  print('COMPAT_PASS',profile,cfg,len(tests),flush=True)
