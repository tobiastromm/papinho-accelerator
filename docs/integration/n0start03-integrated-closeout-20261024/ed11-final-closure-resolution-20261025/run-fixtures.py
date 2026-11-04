from pathlib import Path
import subprocess,json,re,hashlib
r=Path.cwd();p=Path(__file__).resolve().parent;rows=[]
for pr in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  log=p/f'build-fixture-{pr}-{cfg}.log'
  with log.open('wb') as f:q=subprocess.run(['cmd','/c',str(p/f'build-fixture-{pr}-{cfg}.cmd')],stdout=f,stderr=subprocess.STDOUT)
  t=log.read_text(errors='replace');w=len(re.findall(r'warning (?:C|LNK)[0-9]+',t));print(pr,cfg,q.returncode,w,flush=True)
  row=dict(profile=pr,config=cfg,build_exit=q.returncode,warnings=w);rows.append(row)
  (p/'FIXTURE-RESULTS.json').write_text(json.dumps(rows,indent=2)+'\n')
  if q.returncode or w:raise SystemExit(1)
  exe=r/'build/ed11-final-closure-fixture-20261025'/pr/cfg/'identity-candidate.exe'
  test=subprocess.run([str(exe)],capture_output=True);(p/f'fixture-{pr}-{cfg}.log').write_bytes(test.stdout+test.stderr);row.update(test_exit=test.returncode,sha256=hashlib.sha256(exe.read_bytes()).hexdigest());print(test.stdout.decode(errors='replace')[-300:],flush=True)
  (p/'FIXTURE-RESULTS.json').write_text(json.dumps(rows,indent=2)+'\n')
  if test.returncode:raise SystemExit(2)
