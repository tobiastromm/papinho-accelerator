from pathlib import Path
import json,hashlib,subprocess,re
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/r0-r9-acceptance-and-integration-20261025';old=p.parent/'ed11-final-closure-resolution-20261025';sha=lambda f:hashlib.sha256(Path(f).read_bytes()).hexdigest()
rows=[]
for x in json.loads((old/'ESTABLISHED-AUTHORITY-CORPUS-REVALIDATION.json').read_text(encoding='utf-8')):
 now=sha(x['path']);rows.append(dict(path=x['path'],expected=x['sha256'],actual=now,unchanged=now==x['sha256']))
assert all(x['unchanged'] for x in rows)
(p/'AUTHORITY-PRESERVATION.json').write_text(json.dumps(rows,indent=2)+'\n',encoding='utf-8')
oldpins=json.loads((old/'FINAL-PRODUCT-AND-AUTHORITY-PRESERVATION.json').read_text(encoding='utf-8'));rows=[]
for x in oldpins['operational_artifact_policy_tuples']:
 now=sha(x['path']);rows.append(dict(item=x['item'],path=x['path'],expected=x['sha256'],actual=now,unchanged=now==x['sha256']))
assert all(x['unchanged'] for x in rows)
(p/'HISTORICAL-ARTIFACT-PRESERVATION.json').write_text(json.dumps(rows,indent=2)+'\n',encoding='utf-8')
adr=r/'docs/adr/DECISION-service-virtual-user-identity-and-q4-documentary-disposition-20261025.md';proposal=r/'docs/adr/ADR-CANDIDATE-service-virtual-user-identity-and-q4-documentary-disposition-20261025.md';approved='d19ad5922874569af416baab7e86b196b916eaead057418a97dd4226bfc8d9ef';assert sha(proposal)==approved and adr.read_bytes().endswith(proposal.read_bytes())
policy=sha(adr);tag=('R0_R9_POLICY_'+policy).encode();oldimage=r/'build/q4-final-binding-20261025/Selected/Release/papacc_server_lab_host.exe';assert tag not in oldimage.read_bytes();pin=sha(oldimage);art=[]
for pr in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  b=r/'build/r0-r9-integration-20261025'/pr/cfg;s=b/'papacc_server_lab_host.exe';q=b/'papacc_scm_instrumented_lab.exe';has=tag in s.read_bytes();pincompiled=pin.encode() in q.read_bytes();assert has and pincompiled and sha(s)!=pin
  art.append(dict(profile=pr,config=cfg,service_path=str(s),service_sha256=sha(s),qualifier_path=str(q),qualifier_sha256=sha(q),accepted_policy_sha256=policy,service_policy_tag_present=has,qualifier_compiled_image_pin=pin,qualifier_pin_matches_preserved_history=pincompiled,qualifier_pin_matches_new_candidate=False,admission='DENY_NEW_BINDING_AND_SCOPE_REQUIRED',operational_pins='HISTORICAL_PRESERVED_NOT_ACTIVATED'))
(p/'DERIVED-CANDIDATE-PINS.NOT-ACTIVATED.json').write_text(json.dumps(art,indent=2)+'\n',encoding='utf-8')
files=['CMakeLists.txt','apps/server/server_service_runtime_win32.inc','apps/server/server_service_win32.c','apps/server/server_service_virtual_user_r0r9_win32.inc','tests/service_runtime_component_test.c','tests/service_runtime_file_fixture.inc','tests/service_virtual_user_r0r9_test.c','tests/service_virtual_user_r0r9_file_test.c']
(p/'FINAL-SOURCE-HASHES.json').write_text(json.dumps([dict(path=f,sha256=sha(r/f)) for f in files],indent=2)+'\n',encoding='utf-8')
# This fixture was clean at task start. Recover original Git bytes plus its observed CRLF working form; identify reconstruction explicitly.
f='tests/service_runtime_file_fixture.inc';blob=subprocess.check_output(['git','show','HEAD:'+f]);(p/'preimages/service_runtime_file_fixture.HEAD.inc').write_bytes(blob);(p/'preimages/service_runtime_file_fixture.RECONSTRUCTED-CRLF.inc').write_bytes(blob.replace(b'\r\n',b'\n').replace(b'\n',b'\r\n'))
print('authority',len(json.loads((p/'AUTHORITY-PRESERVATION.json').read_text())), 'preserved pins',len(rows),'policy',policy);print(json.dumps(art[1],indent=2))
