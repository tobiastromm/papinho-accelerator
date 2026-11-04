from pathlib import Path
import json,subprocess,hashlib
p=Path('docs/integration/n0start03-integrated-closeout-20261024/q4-target-evidence-closure-20261025');rows=[]
for pr in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  for name in ['papacc_target_typed_evidence_test','papacc_process_provenance_offline_test','papacc_pair_receipt_offline_test']:
   exe=Path('build/q4-target-evidence-20261025')/pr/cfg/(name+'.exe');q=subprocess.run([str(exe.resolve())],capture_output=True);out=q.stdout.decode(errors='replace')+q.stderr.decode(errors='replace');(p/f'test-{pr}-{cfg}-{name}.log').write_text(out,encoding='utf-8');rows.append(dict(profile=pr,config=cfg,name=name,exit=q.returncode,sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),output=out));print(pr,cfg,name,q.returncode,out.splitlines()[-1][:180] if out else '',flush=True)
(p/'ACCELERATOR-TEST-RESULTS.json').write_text(json.dumps(rows,indent=2)+'\n',encoding='utf-8');raise SystemExit(any(x['exit'] for x in rows))
