from pathlib import Path
import json,subprocess,hashlib,re
r=Path.cwd();p=Path(__file__).resolve().parent;rows=[]
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  log=p/f'build-parser-{profile}-{cfg}.log'
  with log.open('wb') as out:q=subprocess.run(['cmd','/c',str(p/f'build-parser-{profile}-{cfg}.cmd')],stdout=out,stderr=subprocess.STDOUT)
  warnings=len(re.findall(r'warning C[0-9]+|warning LNK[0-9]+',log.read_text(errors='replace')));assert q.returncode==0 and warnings==0,log
  exe=r/'build/q4-input-candidate-20261025'/profile/cfg/'candidate-parser.exe';q=subprocess.run([str(exe),str(p/'OPENED-WINDOWS-FILE.CANDIDATE.txt')],capture_output=True,text=True,timeout=15);assert q.returncode==0,q.stdout+q.stderr
  rows.append(dict(profile=profile,config=cfg,build_exit=0,warnings=warnings,test_exit=q.returncode,output=q.stdout,exe_sha256=hashlib.sha256(exe.read_bytes()).hexdigest()));p.joinpath('parser-build-test-results.json').write_text(json.dumps(rows,indent=2));print(profile,cfg,q.stdout.strip(),flush=True)
