from pathlib import Path
import json,hashlib,csv,subprocess
r=Path.cwd();p=Path(__file__).resolve().parent;h=lambda f:hashlib.sha256(f.read_bytes()).hexdigest();a=json.loads((p/'FINAL-ARTIFACTS.json').read_text());request=p/'Q7-OPERATION-REQUEST.DRAFT.json';binding=p/'Q4-FINAL-BINDING.ANNEX.PROPOSED.json';req=json.loads(request.read_text(encoding='utf-8'))
assert h(request)==a['request_sha256'] and h(binding)==a['binding_sha256'];assert req['final_binding']['sha256']==h(binding)
for item in a['artifacts'].values():assert h(Path(item['path']))==item['sha256']
for item in req['dependencies'].values():assert h(Path(item['path']))==item['sha256']
builds=json.loads((p/'build-results.json').read_text());assert len(builds)==4 and all(x['exit']==0 and x['warnings']==0 for x in builds)
reg=json.loads((p/'compat-regression-results.json').read_text());assert len(reg)==76 and all(x['exit']==0 for x in reg)
assert len(json.loads((p/'focused-test-results.json').read_text()))==8
assert len(json.loads((p/'powershell-test-results.json').read_text()))==3
assert not (p/'EXACT-Q7-APPROVED.json').exists()
assert a['artifacts']['service_image']['sha256'] in (r/'tools/qualification/scm-q4-image-pin.h').read_text()
for row in a['builds']:
 for file,key in [('papacc_scm_instrumented_lab.exe','qualifier_sha256'),('papacc_server_lab_host.exe','image_sha256'),('papacc_n0_private_stdout.dll','native_resource_sha256')]:assert h(r/'build/q4-final-binding-20261025'/row['profile']/row['config']/file)==row[key]
with (p.parent/'ED11_GATE_MATRIX.csv').open(encoding='utf-8',newline='') as f:
 rows=[x for x in csv.reader(f) if x];assert all(len(x)==9 for x in rows),[x for x in rows if len(x)!=9];assert rows[-1][0]=='Q4_FINAL_PINS_Q7_READINESS'
head=subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip();branch=subprocess.check_output(['git','branch','--show-current'],text=True).strip();assert head=='913c90cf2328f7939d8f5b927640601903ac6e91' and branch=='main'
check=dict(status='PASS',HEAD=head,branch=branch,request_sha256=h(request),final_annex_sha256=h(binding),artifact_hashes_current=True,dependency_hashes_current=True,frozen_image_pin_current=True,builds=4,warnings=0,focused_C_outcomes=164,actual_plan_configurations=4,historical_regressions=76,powershell_gates=63,invalid_bootstrap_probes=4,approval_record='ABSENT',Q4='NOT_ACTIVATED',budget='NOT_GRANTED',ED11='PARTIAL',Phase6='PARTIAL',operational_execution='NONE')
(p/'FINAL-VALIDATION.json').write_text(json.dumps(check,indent=2)+'\n')
files=[x for x in p.rglob('*') if x.is_file() and x.name!='FINAL-HASHES.json'];files+=[r/key for key in json.loads((p/'source-preimages.json').read_text())];files+=[r/'docs/capabilities/canonical-phase6-consolidation.md',p.parent/'INTEGRATED-CLOSEOUT.md',p.parent/'ED11_GATE_MATRIX.csv']
(p/'FINAL-HASHES.json').write_text(json.dumps({str(f.relative_to(r)):h(f) for f in sorted(set(files))},indent=2)+'\n');print(json.dumps(check,indent=2))
