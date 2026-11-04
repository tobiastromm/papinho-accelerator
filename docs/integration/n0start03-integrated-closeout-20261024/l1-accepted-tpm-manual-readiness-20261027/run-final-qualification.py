from pathlib import Path
import subprocess,json,concurrent.futures
root=Path.cwd();pkg=Path(__file__).resolve().parent
jobs=[]
for profile in ['Selected','Legacy']:
 for config in ['Debug','Release']:
  for target in ['papacc_config_root8_v5_test']:
   jobs.append(('final2-'+profile+'-'+config+'-'+target,[str(root/'build/v3-backings-20261021'/profile/config/(target+'.exe'))]))
for config in ['Debug','Release']:
 for version in [4,5]:
  for mode in ['clean','stop','unknown','conflict','identity']:
   label='final-native-v5-'+config+'-base'+str(version)+'-'+mode
   path=pkg/'synthetic-worker-files'/(label+'.dat')
   if path.exists():raise RuntimeError('NO_RETRY_EXISTING '+str(path))
   jobs.append((label,[str(root/'build/v3-backings-20261021/Selected'/config/'papacc_v5_transaction_worker_test.exe'),mode,str(path),str(version)]))
def run(job):
 label,cmd=job
 r=subprocess.run(cmd,capture_output=True,text=True,timeout=180)
 (pkg/(label+'.log')).write_text(r.stdout+r.stderr,encoding='utf-8')
 return dict(label=label,exit=r.returncode,output=(r.stdout+r.stderr).strip())
with concurrent.futures.ThreadPoolExecutor(max_workers=4) as ex:
 results=list(ex.map(run,jobs))
(pkg/'FINAL-QUALIFICATION-RESULTS.json').write_text(json.dumps(results,indent=2),encoding='utf-8')
for r in results:print(r['label'],r['exit'],r['output'])
raise SystemExit(0 if all(x['exit']==0 for x in results) else 1)
