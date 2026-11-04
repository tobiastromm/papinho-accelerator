from pathlib import Path
import json,subprocess
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/d0-d9-bounded-diagnostic-policy-20261026';rows=[]
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  b=r/'build/d0-d9-bounded-diagnostic-20261026'/profile/cfg;cmds=(b/'compile_commands.json').read_text();assert 'PAPACC_SINGLE_LAB_ADMISSION' not in cmds
  for name in ['papacc_target_typed_evidence_test','papacc_process_provenance_offline_test','papacc_pair_receipt_offline_test']:
   exe=b/(name+'.exe');data=exe.read_bytes();marker='Global\\PapinhoAccelerator.ED11.SingleLabAdmission';assert marker.encode() not in data and marker.encode('utf-16-le') not in data
   q=subprocess.run([str(exe)],cwd=b,capture_output=True);out=q.stdout+q.stderr;(p/f'test-{profile}-{cfg}-{name}.log').write_bytes(out);rows.append(dict(profile=profile,config=cfg,test=name,exit=q.returncode,scope='normal isolated owner / no listener or LAB activation'));print(rows[-1],out.decode(errors='replace')[-700:],flush=True)
   if q.returncode:raise SystemExit(1)
(p/'ACCELERATOR-TESTS.json').write_text(json.dumps(rows,indent=2)+'\n')
