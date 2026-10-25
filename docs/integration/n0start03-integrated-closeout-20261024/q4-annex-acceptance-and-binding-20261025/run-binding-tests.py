from pathlib import Path
import subprocess,json,hashlib
p=Path(__file__).resolve().parent;rows=[]
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  log=p/f'build-binding-test-{profile}-{cfg}.log'
  with log.open('wb') as out:q=subprocess.run(['cmd','/c',str(p/f'build-binding-test-{profile}-{cfg}.cmd')],stdout=out,stderr=subprocess.STDOUT)
  if q.returncode:print(log.read_text(errors='replace'));raise SystemExit(q.returncode)
  exe=Path.cwd()/'build/q4-binding-20261025'/profile/cfg/'q4-binding-test.exe'
  q=subprocess.run([str(exe)],capture_output=True,text=True,timeout=20);(p/f'binding-{profile}-{cfg}.log').write_text(q.stdout+q.stderr);rows.append(dict(profile=profile,config=cfg,exit=q.returncode,cases=21,sha256=hashlib.sha256(exe.read_bytes()).hexdigest()));(p/'binding-test-results.json').write_text(json.dumps(rows,indent=2))
  if q.returncode:print(q.stdout,q.stderr);raise SystemExit(1)
print('BINDING_PASS',len(rows),flush=True)
