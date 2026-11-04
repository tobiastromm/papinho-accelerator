from pathlib import Path
import subprocess,json
p=Path('docs/integration/n0start03-integrated-closeout-20261024/r0-r9-acceptance-and-integration-20261025');rows=[]
targets=['papacc_service_virtual_user_r0r9_test','papacc_service_virtual_user_r0r9_file_test','papacc_service_runtime_model_test','papacc_service_runtime_file_model_test','papacc_scm_registration_lab_model','papacc_service_host_lab_model','papacc_service_host_product_model','papacc_service_n0_census_content_test']
for pr in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  for t in targets:
   exe=Path('build/r0-r9-integration-20261025')/pr/cfg/(t+'.exe');q=subprocess.run([str(exe.resolve())],capture_output=True);out=q.stdout.decode(errors='replace')+q.stderr.decode(errors='replace');(p/f'test-{pr}-{cfg}-{t}.log').write_text(out,encoding='utf-8');rows.append(dict(profile=pr,config=cfg,test=t,exit=q.returncode,output=out));print(pr,cfg,t,q.returncode,flush=True)
(p/'CALLER-AND-REGRESSION-RESULTS.json').write_text(json.dumps(rows,indent=2)+'\n',encoding='utf-8')
raise SystemExit(any(x['exit'] for x in rows))
