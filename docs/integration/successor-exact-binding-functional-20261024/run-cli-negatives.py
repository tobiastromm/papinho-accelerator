from pathlib import Path
import subprocess,json
root=Path(__file__).resolve().parents[3]
pkg=Path(__file__).resolve().parent
new='0b1de869e11493b7cb08fb329578f6ce'
old='75330d11cc7e4123ae62eadc9a59c2f5'
results=[]
for profile in ['Selected','Legacy']:
 for config in ['Debug','Release']:
  exe=root/'build'/'successor-exact-binding-20261024'/profile/config/'papacc_scm_instrumented_lab.exe'
  cases=[('empty',[]),('old_prepare',['--prepare-n0',old,'1','2','3','4','5']),('old_cycle',['--cycle-n0',old,'1','2','3','4','5','0'*64]),('new_prepare_missing_N2',['--prepare-n0',new,'1','2','3','4','5']),('new_cycle_missing_N2',['--cycle-n0',new,'1','2','3','4','5','0'*64]),('unsupported_observer',['--observe-self'])]
  for name,args in cases:
   r=subprocess.run([str(exe)]+args,capture_output=True,text=True,timeout=10,creationflags=subprocess.CREATE_NO_WINDOW)
   results.append({'profile':profile,'config':config,'case':name,'exit':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
   (pkg/'cli-negative-results.json').write_text(json.dumps(results,indent=2))
   assert r.returncode==2,(name,r.returncode,r.stdout)
   if name.startswith('new_'):assert 'DENY_BEFORE_ACQUISITION_NO_PREFLIGHT_NO_EFFECT' in r.stdout
print('PASS24 CLI negatives; no preflight acquisition, SCM or effect')
