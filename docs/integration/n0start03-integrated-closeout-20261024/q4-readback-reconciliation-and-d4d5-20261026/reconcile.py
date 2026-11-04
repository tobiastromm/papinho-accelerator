from pathlib import Path
import hashlib,json,csv,io,subprocess
base=Path('docs/integration/n0start03-integrated-closeout-20261024')
out=base/'q4-readback-reconciliation-and-d4d5-20261026'
if (out/'Q4-RECONCILIATION.json').exists(): raise SystemExit('Archived reconciliation already exists; do not rerun mutations')
old=base/'ed11-final-closure-resolution-20261025'
def digest(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def save(name,data): (out/name).write_text(json.dumps(data,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
request=json.loads((old/'Q4-EFFECTS-READBACK-REQUEST.DRAFT.json').read_text(encoding='utf-8-sig'))
items=[request['script']]+request['dependencies']
checks=[{'path':x['path'],'expected':x['sha256'],'actual':digest(Path(x['path'])),'match':digest(Path(x['path']))==x['sha256']} for x in items]
checks.append({'path':str(old/'Q4-EFFECTS-READBACK-REQUEST.DRAFT.json'),'expected':'2aa9e0b81352b0efe28e23135fa4644b70d17241565e1fb6e6a6d191312c663b','actual':digest(old/'Q4-EFFECTS-READBACK-REQUEST.DRAFT.json'),'match':digest(old/'Q4-EFFECTS-READBACK-REQUEST.DRAFT.json')=='2aa9e0b81352b0efe28e23135fa4644b70d17241565e1fb6e6a6d191312c663b'})
assert len(checks)==22 and all(x['match'] for x in checks)
transcript='''Owner-reported excerpt, normalized from chat. Not an independent native capture; terminal markup omitted. No execution timestamp inferred.
PS C:\\WINDOWS\\system32> & "C:\\Projetos\\PapinhoAccelerator\\docs\\integration\\n0start03-integrated-closeout-20261024\\ed11-final-closure-resolution-20261025\\q4-effects-readback.CANDIDATE.ps1" -Mode Verify -ExpectedScriptSha256 aa9b398dfe29443be1a51faa48eaf4c4418720b531c21505a744e5843daff5e4 -Confirmation "READ_ONLY_Q4_EFFECTS N0TOKEN-20261025-01 10584f02e11f92c92b43cddf0d9d316f"
HOST_BOOT_DENY
q4-effects-readback.CANDIDATE.ps1:48 caractere:232
CategoryInfo: OperationStopped: (HOST_BOOT_DENY:String) [], RuntimeException
FullyQualifiedErrorId: HOST_BOOT_DENY
PS C:\\WINDOWS\\system32>
'''
(out/'OWNER-REPORTED-RESULT.txt').write_text(transcript,encoding='utf-8')
src=(old/'q4-effects-readback.CANDIDATE.ps1').read_text(encoding='utf-8-sig')
assert src.index("throw 'HOST_BOOT_DENY'") < src.index('$result=[PaccQ4EffectsRead]::Run()') < src.index('$dossier=[PaccQ4InputRead]::Run()') < src.index("[Console]::Out.WriteLine('Q4_EFFECTS_READBACK_FIRST_ERROR=")
assert '$hostHash-cne' in src and '$boot.RecordId-ne236441' in src and "2026-10-23T18:36:34.5000000Z" in src
save('Q4-RECONCILIATION.json',{'task':'PACC-PHASE6-ED11-Q4-READBACK-RECONCILIATION-AND-D4D5','operation':request['operation'],'attempt':request['attempt'],'source':'OWNER_REPORTED_CONSOLE_EXCERPT','first_failure':'HOST_BOOT_DENY','failed_operand':'NOT_DISTINGUISHED_HOST_HASH_OR_BOOT_RECORD_OR_BOOT_START','status':'MANUAL_ATTEMPT_REPORTED_DENIED_BEFORE_PROTECTED_READERS','protected_readback':'NOT_REACHED_BY_SOURCE_ORDER_CONDITIONAL_ON_REPORTED_EXACT_SCRIPT','native_final_summary':'ABSENT_NOT_REACHED','protected_reader_closure':'NO_NEW_PROTECTED_READER_ACQUISITION_BY_CONTROL_FLOW_NOT_NATIVE_CLOSE_RECEIPT','global_quiescence':'UNPROVEN','historical_unknown':'PRESERVED','retry_authorized':False,'agent_verify_executions':0,'disposition_applied':False})
save('Q4-DISPOSITION-READINESS.json',{'status':'BLOCKED_PRECONDITIONS_NOT_SATISFIED','approvable':False,'operation':request['operation'],'attempt':request['attempt'],'candidate_disposition':'CLOSED_ORIGINAL_LOCAL_REFERENCES_WITH_PERSISTENT_EFFECTS_AND_UNKNOWN_RETAINED','readback_request_sha256':checks[-1]['actual'],'reported_result_sha256':digest(out/'OWNER-REPORTED-RESULT.txt'),'first_failure':'HOST_BOOT_DENY','normative_evaluation_authority':'DECISION-service-virtual-user-identity-and-q4-documentary-disposition-20261025 R6-R8 accepted','missing':['R7 original-handle protected object identity, RAW1, content and same-handle revalidation','Native readback closure/retention outcome','Separate exact documentary application approval'],'preserved':['Start1 Stop0 consumed intent','Historical first failures','Historical unprinted intent/image creation IDs UNKNOWN','Opaque residue and global quiescence UNPROVEN','Persistent effects and unavailable real reserves'],'next_owner_decision':'Resolve failed immutable host/boot binding and permissible evidence path; no automatic re-pin/retry or R7 waiver','application':'DENY','physical_release_refund_cleanup':'NOT_AUTHORIZED','new_operational_identity':'NOT_CREATED'})
# Preserve exact original live files before narrow documentary updates.
matrix=base/'ED11_GATE_MATRIX.csv'; live=Path('docs/capabilities/canonical-phase6-consolidation.md')
(out/'ED11_GATE_MATRIX.csv.preimage').write_bytes(matrix.read_bytes()); (out/'CAPABILITY.preimage.bin').write_bytes(live.read_bytes())
raw=matrix.read_bytes(); text=raw.decode('utf-8-sig'); lines=text.splitlines(keepends=True); rows=list(csv.reader(io.StringIO(text))); before={r[0]:r for r in rows[1:] if r}
changed=0
for i,line in enumerate(lines):
    if line.startswith('E21,'):
        row=next(csv.reader([line])); assert len(row)==len(rows[0])
        row[3]='Owner manual Verify reported HOST_BOOT_DENY at script line48 before protected readers; 22 repository dependency hashes PASS; q4-readback-reconciliation-and-d4d5-20261026/CONSOLIDATED-REPORT.md'
        row[4]='Original object IDs RAW1 content and native readback closure not observed; failed host or boot operand unknown; historical UNKNOWN and real reservations preserved'
        row[5]='Owner decision on failed binding/evidence path; no retry or pin change; complete R7 before exact separately approved documentary application; no refund'
        buf=io.StringIO(newline=''); csv.writer(buf,lineterminator='\r\n' if line.endswith('\r\n') else '\n').writerow(row); lines[i]=buf.getvalue(); changed+=1
assert changed==1
new=''.join(lines); matrix.write_bytes((b'\xef\xbb\xbf' if raw.startswith(b'\xef\xbb\xbf') else b'')+new.encode('utf-8'))
after={r[0]:r for r in list(csv.reader(io.StringIO(new)))[1:] if r}
assert before.keys()==after.keys() and all(before[k]==after[k] for k in before if k!='E21')
assert before['E21'][2]==after['E21'][2] and before['E21'][-1]==after['E21'][-1]
addition='''\n\n## 2026-10-26 — Q4 manual readback denied before protected collection\n\nOwner reports the exact manual Verify attempt failed HOST_BOOT_DENY at line48, before either protected reader and before native closure/final summaries. The combined guard does not disclose which host/boot operand differed. No fresh original-object identity/RAW1/content or protected closure receipt was obtained. No retry, pin change, new identity, disposition, cleanup or refund. E21 remains pending; historical UNKNOWN/real reservations/global quiescence UNPROVEN preserved. D4/D5 quantitative constraints, absolute Site-pump guard scope and missing target policy/bindings are consolidated in [Q4/D4D5 report](../integration/n0start03-integrated-closeout-20261024/q4-readback-reconciliation-and-d4d5-20261026/CONSOLIDATED-REPORT.md). D2 stays preparation-only accepted; numeric age/collection validity/overall duration/physical and commit floors are not fabricated or inherited from LAB/C8. ED11 4demonstrated/12limited/6pending; Phase6 PARTIAL; productive authorities DENY. Earlier unexecuted descriptions remain historical.\n'''
with live.open('ab') as f: f.write(addition.encode('utf-8'))
# Source evidence only: no query of host/boot, runtime modules or protected effect paths.
paths=[old/'q4-effects-readback.CANDIDATE.ps1',old/'q4-effects-readback.CANDIDATE.cs',old/'q4-inputs-readonly.CANDIDATE.cs',Path('apps/server/server_c8_target_evidence_win32.c'),Path('apps/server/server_c8_resource_root_win32.inc'),Path('src/network_egress/c3_bridge_win32.c'),Path('src/core/ed11_process_profile.h'),Path('tools/offline/mode_c_pair.py'),Path('C:/Projetos/PapinhoBrowser/src/papinho_c2_s6.inc'),Path('C:/Projetos/PapinhoBrowser/src/papinho_c2_budget.inc'),Path('C:/Projetos/PapinhoBrowser/platform/win32/papinho_site_pump_win32.c')]
assert 2097152-1858020==239132 and 2*1858020-2097152==1618888 and 3145728-1965128==1180600 and 2097152-1799632==297520
samples=list(csv.DictReader((base/'composite-target-quantitative-closure-20261026/RUNTIME-SAMPLES.csv').open(encoding='utf-8-sig')))
assert len(samples)==500
save('VERIFICATION.json',{'result':'PASS_WITH_NATIVE_PROOF_LIMITS','repository_dependency_checks':checks,'source_control_flow_assertions':'PASS','script_executed':False,'protected_objects_opened':False,'source_hashes':{str(p):digest(p) for p in paths},'controlled_arithmetic':'PASS','prior_runtime_samples_reparsed':len(samples),'unique_prior_process_lifetimes':len({(r['pid'],r['creation']) for r in samples}),'gate_ids_count':sum(k.startswith('E') and k[1:].isdigit() for k in after),'non_E21_rows_unchanged':True,'E21_classification_unchanged':True,'builds_tests':'NO_FUNCTIONAL_CHANGE_NO_PRODUCT_REBUILD_OR_DUPLICATE_CAMPAIGN','limits':'Static consistency/control flow and historical data only; no fresh active pair/headroom/protected readback proof'})
save('MANIFEST.json',{str(p.relative_to(out)):digest(p) for p in out.iterdir() if p.is_file() and p.name not in ('MANIFEST.json','WRITE-CHECK.txt','reconcile.py')})
print('PASS: 22 dependency hashes; pre-reader boundary; controlled arithmetic; 500 historical samples; live E21-only row update; native proof still absent')
