from pathlib import Path
import json,hashlib,subprocess,difflib
r=Path.cwd();p=Path(__file__).resolve().parent;b=r/'build/q4-final-binding-20261025/Selected/Release';src=r/'tools/qualification';base=p.parent/'q4-n7-protected-inputs-and-activation-20261025';h=lambda f:hashlib.sha256(f.read_bytes()).hexdigest()
A=lambda f:dict(path=str(f),sha256=h(f),bytes=f.stat().st_size)
artifacts=dict(launcher=A(src/'scm-n0-protected-single-powershell.ps1'),qualifier=A(b/'papacc_scm_instrumented_lab.exe'),native_resource=A(b/'papacc_n0_private_stdout.dll'),framework_binding=A(r/'build/ed11-private-stdout-20261025/Selected/Release/Pacc.N0PrivateStdout.Binding.dll'),service_image=A(b/'papacc_server_lab_host.exe'))
factual=json.loads((base/'Q4P-FACTUAL-PINS.PREPARED.json').read_text());inputs=factual['inputs']
deps={
 'Q0_Q9':r/'docs/adr/DECISION-n0start03-poststart-disposition-and-one-token-observation-20261025.md',
 'Q4_annex':r/'docs/adr/DECISION-q4-independent-service-token-observation-annex-20261025.md',
 'Q2_record':p.parent/'q2-protected-readback-native-result-20261025/Q2-EXACT-DISPOSITION-RECORDED.md',
 'N7':r/'docs/adr/DECISION-q4-specific-n7-and-independent-evaluation-20261025.md',
 'evaluation_plan':base/'INDEPENDENT-ED11-EVALUATION-PLAN.md',
 'T10_T14':r/'docs/adr/DECISION-n0start03-positive-private-transport-composition-20261025.md',
 'T0_T9':r/'docs/adr/DECISION-n0start03-noninherited-local-transport-20261025.md',
 'N0_N9':r/'docs/adr/DECISION-first-native-lab-diagnostic-boundary-20261024.md',
 'RAW1':r/'docs/adr/DECISION-n0-n9-security-descriptor-digest-profile-20261024.md',
 'B0_B6':r/'docs/adr/DECISION-ed11-single-lab-diagnostic-native-boundary-20261023.md',
 'B_D':r/'docs/adr/DECISION-canonical-lab-console-topology-bd-20261023.md'}
requirements=dict(host_sha256='2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff',boot=236441,operator_sid='S-1-5-21-785699550-1440458426-1682014700-1108',service='PapinhoAcceleratorLab20261023',account='NT SERVICE\\PapinhoAcceleratorLab20261023',specific_sid='S-1-5-80-3567404667-26456103-3018599297-139793421-3962028451',owner='existing PS_ADMISSION',mutex='Global\\PapinhoAccelerator.ED11.SingleLabAdmission',configuration_sha256='a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44',native_configuration_id='417614100:393216:1120240',native_parent_id='417614100:1966080:79426')
parent=r/'build/scm-protected-installation-binding-20261022/Selected/Release';attempt='10584f02e11f92c92b43cddf0d9d316f';operation='N0TOKEN-20261025-01'
limits=dict(native_handles=128,shared_administrative_handles=1024,participant_roles=4,global_freshness_s=60,final_freshness_s=1,admission_physical_GiB=5,admission_commit_GiB=9,monitor_physical_GiB=4,monitor_commit_GiB=8,selected_controlled_MiB=3,legacy_controlled_MiB=2,other_existing_caps='UNCHANGED_REQUIRED',HOST_CREDITS='NONE_GRANTED',budget='REQUIRED_FRESH_NOT_GRANTED')
outputs=dict(intent=str(parent/f'papacc-psr-{attempt}.intent'),image=str(parent/f'papacc_server_lab_host_ntstatus_{attempt}.exe'),sink=str(parent/f'papacc-psr-{attempt}.diag'),arm='kind0 offset0 1024 bytes inside same diagnostic sink; no separate file',native_ids=None)
binding=dict(status='PROPOSED_FINAL_ARTIFACT_ANNEX_NOT_ACTIVATION',operation=operation,attempt=attempt,requirements=requirements,protected_inputs=inputs,RAW1=factual['RAW1'],artifacts=artifacts,dependencies={key:A(f) for key,f in deps.items()},outputs=outputs,limits=limits,runtime_plan='actual ig_n0_plan hashes fresh original PID/creation, token statistics, control thread, challenge, original config/image/aux IDs, final exact approval digest, protected IDs, Q0/Q4/Q2/N7; cannot precompute live digest',historical='Q2 documentary only; global quiescence UNPROVEN; UNKNOWN and real retained reservations unavailable; no refund',activation='NOT_ACTIVATED; final annex acceptance and separate exact manual Q7 authority still required')
(p/'Q4-FINAL-BINDING.ANNEX.PROPOSED.json').write_text(json.dumps(binding,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
request=dict(status='DRAFT_NOT_OWNER_APPROVAL',operation=operation,attempt=attempt,mode='Cycle',profile='Q4TokenDiagnostic',manual_only=True,automatic_execution=False,scope='Owner final Q4 artifact annex acceptance and one separately bounded Q7 independent token/census diagnostic; no implicit admission',final_binding=A(p/'Q4-FINAL-BINDING.ANNEX.PROPOSED.json'),artifacts=artifacts,dependencies={key:A(f) for key,f in deps.items()},protected_inputs=inputs,requirements=requirements,limits=limits,maximum_future_effects=['one canonical intent consume under existing owner','exclusive new image','exclusive new sink with embedded arm','explicit rebind of existing LAB service','at most one Start','at most one safe accepted Stop under actual state'],outputs=outputs,prior_to_effects=['exact owner annex/operation decision registered; no inferred approval','original-handle protected ancestry, native identity, RAW1 and content revalidation','original operator/token/session and identified auxiliary; specific N7 not broad authentication','fresh independent full ED11 budget and compatible obligations, canonical exclusivity','exact runtime confirmations, original pending Stop/freeze checks, exclusive absence and no adoption'],stop_conditions=['missing or changed identity/hash/descriptor/host/boot/token','absent proof or unreadable object','freshness expiry or ED11 excess','controlled UNKNOWN, incompatible real reservation, fifth role','Stop/freeze, partial creation, uncertain close or token query','START_PENDING is not accepted Stop'],on_failure='preserve first original error, partial effects/UNKNOWN/owned references; no automatic retry/delete/overwrite/rollback/kill/rearm',diagnostic_success='new original token SID entry/count/raw attrs ENABLED 0x4 versus ENABLED_BY_DEFAULT 0x2 versus DENY_ONLY 0x10, restrictions/query status; PID/creation and census masks/order; early predicate rejection valid consumed diagnostic, not obligation to reach RUNNING',proof_limits=['new token only; cannot reconstruct historical03 token or masks','no universal global quiescence or physical refund','picos sampled only; unobserved kernel/cache/stack costs UNKNOWN','no ED11/Phase6 closure solely from diagnostic'],forbidden=['Place replay','old intent/approval/effect reuse','new identity/service/owner/mutex','unapproved SCM/ACL changes or cleanup','network/listener/CA/certificates/bootstrap/CURRENT/signature/recovery'],approval_record_path=str(p/'EXACT-Q7-APPROVED.json'),approval_record_state='ABSENT_DO_NOT_CREATE_WITHOUT_OWNER_DECISION')
(p/'Q7-OPERATION-REQUEST.DRAFT.json').write_text(json.dumps(request,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
# Inert schema template: deliberately fails owner status, exact hash and native prefix gates.
template=dict(status='DRAFT_NOT_OWNER_APPROVAL',operation=operation,attempt=attempt,mode='Cycle',manual_only=True,automatic_execution=False,request_sha256=h(p/'Q7-OPERATION-REQUEST.DRAFT.json'),T10_T14_sha256=h(deps['T10_T14']),**{key+'_sha256':h(deps[key]) for key in ['Q0_Q9','Q4_annex','Q2_record','N7','evaluation_plan']},q7_activation_authorized=False,final_binding_sha256=h(p/'Q4-FINAL-BINDING.ANNEX.PROPOSED.json'),artifacts={key:dict(path=v['path'],sha256=v['sha256']) for key,v in artifacts.items() if key!='service_image'})
(p/'Q7-APPROVAL-RECORD.INERT-TEMPLATE.json').write_text(json.dumps(template,indent=2)+'\n')
assert not (p/'EXACT-Q7-APPROVED.json').exists()
# Check all literal hashes used by launcher ordinary normative pins against actual files.
s=(src/'scm-n0-protected-single-powershell.ps1').read_text();import re
checks=[]
for path,sha in re.findall(r"Pin \(\$Root\+'([^']+)'\) '([0-9a-f]{64})'",s):
 f=r/path.lstrip('\\').replace('\\','/');assert h(f)==sha,(f,sha,h(f));checks.append(dict(path=str(f),sha256=sha))
# Preserve old approved packages and old actual image/qualifier binaries, not just their filenames.
old=p.parent/'q4-annex-acceptance-and-binding-20261025';manifest=json.loads((old/'FINAL-HASHES.json').read_text());preserved=[]
for path,sha in manifest.items():
 f=r/path
 if f.is_relative_to(old):assert h(f)==sha,(f,'historical package changed');preserved.append(path)
for row in json.loads((old/'final-artifacts.json').read_text())['builds']:
 for fn,key in [('papacc_server_lab_host.exe','image_sha256'),('papacc_scm_instrumented_lab.exe','qualifier_sha256')]:
  f=r/'build/q4-binding-20261025'/row['profile']/row['config']/fn;assert h(f)==row[key];preserved.append(str(f.relative_to(r)))
assert h(base/'manual-Q4P-20261025-01.txt')=='3024b2b9470de08df983140758feeaa49df124759ea72503b0d9bcf461f47c0c'
(p/'preservation-and-literal-pins.json').write_text(json.dumps(dict(historical_files_checked=len(preserved),files=preserved,literal_policy_pins=checks,placement_receipt_unchanged=True,exact_q7_approval_absent=True),indent=2))
diffs=[]
for name in [x.name for x in (p/'preimages').iterdir() if x.is_file()]:
 f=src/name
 if f.exists():diffs.extend(difflib.unified_diff((p/'preimages'/name).read_text().splitlines(True),f.read_text().splitlines(True),fromfile='before/'+name,tofile='after/'+name))
(p/'APPLIED-SOURCE.patch').write_text(''.join(diffs))
(p/'FINAL-ARTIFACTS.json').write_text(json.dumps(dict(artifacts=artifacts,request_sha256=h(p/'Q7-OPERATION-REQUEST.DRAFT.json'),binding_sha256=h(p/'Q4-FINAL-BINDING.ANNEX.PROPOSED.json'),source_hashes={str(f.relative_to(r)):h(f) for f in src.iterdir() if f.name in json.loads((p/'source-preimages.json').read_text()) or f.name in [x.name for x in (p/'preimages').iterdir()]}),indent=2))
print(json.dumps(dict(request_sha256=h(p/'Q7-OPERATION-REQUEST.DRAFT.json'),binding_sha256=h(p/'Q4-FINAL-BINDING.ANNEX.PROPOSED.json'),artifacts=artifacts,preserved=len(preserved)),indent=2))
