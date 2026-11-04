from pathlib import Path
import subprocess,json,concurrent.futures
pkg=Path(__file__).resolve().parent
jobs=[(p,c) for p in ['Selected','Legacy'] for c in ['Debug','Release']]
def run(j):
 p,c=j;r=subprocess.run([str(Path('build/v3-backings-20261021')/p/c/'papacc_config_root8_v5_test.exe')],capture_output=True,text=True,timeout=180)
 (pkg/('final-'+p+'-'+c+'-v5.log')).write_text(r.stdout+r.stderr,encoding='utf-8')
 return dict(profile=p,config=c,exit=r.returncode,output=r.stdout+r.stderr)
with concurrent.futures.ThreadPoolExecutor(max_workers=4) as ex:rs=list(ex.map(run,jobs))
(pkg/'FINAL-DATA-RESULTS.json').write_text(json.dumps(rs,indent=2),encoding='utf-8')
for r in rs:print(r)
raise SystemExit(0 if all(r['exit']==0 for r in rs) else 1)
