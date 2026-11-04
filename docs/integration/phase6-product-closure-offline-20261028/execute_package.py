"""Passive source/document audit; no product invocation, network or native API."""
import csv
import hashlib
import json
import re
import subprocess
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
BROWSER = ROOT.parent / 'PapinhoBrowser'
def location(name):
    return BROWSER / name[2:] if name.startswith('B:') else ROOT / name
def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()
def save(name, value):
    (HERE/name).write_text(json.dumps(value,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')

baseline=ROOT/'docs/integration/phase6-original-scope-recovery-20261028/SCOPE-VERIFICATION.json'
expected='7588bfcd2f0d395fc841c6a71ff0838b839fa631c6910a84db2d221a0f5197c3'
if not (HERE/'BASELINE.json').exists():
    if digest(baseline)!=expected: raise SystemExit('BASELINE_DIVERGENCE_STOP')
    save('BASELINE.json',json.loads(baseline.read_text(encoding='utf-8')))
    p=HERE/'preimages';p.mkdir(exist_ok=True)
    for name in ('build_matrix.py','REQUIREMENTS.csv'):
        (p/name).write_bytes((baseline.parent/name).read_bytes())

spec='''area|source|symbol|field_or_guard|state|missing_producer_or_proof
root|apps/server/server_config_cli.c|first_install_entry|build-profile branch|PREFLIGHT_NOT_PUBLICATION|Concrete effect and authorized act binding
root|apps/server/server_launcher_personal_win32.inc|papacc_server_launcher_personal_first_install|personal_data / same owner|READ_ONLY_NATIVE_DATA|No CA/commit/publication here
root|apps/server/server_configuration_root_transaction_win32.inc|papacc_server_launcher_configuration_transaction_prepare_store|expected zero/dormant/absent target|FIRST_COMMIT_ONLY|Actual administrative caller and preparation
root|apps/server/server_configuration_root_transaction_win32.inc|papacc_server_launcher_configuration_transaction_publish|administrative_published|COMMIT_BEFORE_CURRENT|Actual authorized publication and original object
current|apps/server/server_configuration_root_transaction_win32.inc|papacc_server_launcher_configuration_current_pin|context_epoch/operational_epoch/ticket|OWNED_PIN_NOT_CALLER_EPOCH|Live original publication
current|apps/server/server_configuration_root_transaction_win32.inc|root_operational_reference_current|source/issuer/config projections|REVALIDATION_REQUIRED|Metadata cannot replace live source
sitetls|apps/server/server_launcher_owner_win32.c|papacc_server_launcher_c2_site_template|current catalog/profile|CATALOG_AUTHORITY_ONLY|Approved SiteTLS/trust/destination
c2|apps/server/server_c2_root_binding_win32.inc|papacc_server_launcher_c2_adopt|Principal/session/current pin|IDENTITY_AND_BORROW_REQUIRED|Authenticated session and grants
quantitative|apps/server/server_c8_target_evidence_win32.c|papacc_c8_target_collect|timestamps/sequence|LOCAL_RAW_NOT_TARGET_POLICY|Applicable quantitative policy
quantitative|apps/server/server_c8_target_evidence_win32.c|papacc_c8_target_view_validate|policy ABSENT|RESOURCE_UNKNOWN_AFTER_VALID_RAW|No target authority installer
quantitative|apps/server/server_c8_target_evidence_win32.c|papacc_c8_target_passive_copy|original history/overflow|COPY_NOT_RESAMPLE|Phase coverage and completeness
provider|src/network_egress/c3_bridge_win32.c|papacc_c3_bridge_provider_snapshot|operation/generation/lifetime|METADATA_NOT_COST|Actual active provider/kernel attribution
provider|B:src/papinho_pst_owner.c|papinho_pst_owner_provider_snapshot|thread/registry/frozen/lease|METADATA_NOT_COST|Mapped provider and active connection
browser|B:src/papinho_c2_s6.inc|candidate_c2_execute|job/Session/parent runtime|CANONICAL_OFFLINE_IMPLEMENTED|Real authorized pair
browser|B:src/papinho_c2_s6.inc|c2_accept_open_result|tuple/request/incarnation/epoch|PROTOCOL_NOT_NATIVE_CLOCK|Actual authenticated peer receipt
browser|B:src/papinho_c2_s6.inc|c2_close_roles|fences/ACK/remote FINAL|NOT_LOCAL_DESTROYED|Native owner/group/producer close
browser|B:src/papinho_c2_s6.inc|papinho_candidate_resource_observe|presence/source/errors|OPTIONAL_API_UNKNOWN_PRESERVED|NT4 and same-phase real evidence
browser|B:src/papinho_c2_budget.inc|c2_capacity|provider/kernel/host/freshness|ZERO_INITIALIZED_DENY|Authenticated quantitative source
manual|src/provisioning/manual_package_validation.c|mp_package_validate_authenticated|M1 port/budget/quarantine|PIPELINE_NOT_TRUST_GRANT|Factual trust before KDF
manual|src/provisioning/manual_package_pdg.c|mp_auth_pdg_candidate|PDG/digest/SPKI|SOURCE_ABSENT_EVEN_DATA_MATCH|Live policy/pin and independent confirmation
trust|src/provisioning/shared_structure/trust_unavailable.c|sp_trust_acquire|existing-owner opaque borrow|FIXED_DENY_NO_FACTORY|E8 actual producers/protected graph
browser-import|B:src/papinho_papacc_m1.inc|pb_m1_step|TRUST stage/KDF zero|SIGNATURE_THEN_DENY|Independent trust/confirmation
browser-import|B:src/papinho_credential_import.inc|papinho_credential_material_import|granular DER/PKCS8|NOT_PACKAGE_JOURNEY|Validated package into same store
auto|src/provisioning/enrollment.c|ep_frame_decode|PAE1 bounded grammar|OFFLINE_DATA|Canonical listener/socket owner
auto|src/provisioning/enrollment.c|ep_trace_observe|order/phase/owned CSR|DATA_NOT_APPROVAL|Policy/admin/issuer binding
auto|src/provisioning/enrollment.c|ep_pair_claim_data|connection/generation/deadline|CLAIM_NOT_ISSUANCE|Original same TLS and exact request
auto|src/provisioning/enrollment.c|ep_limiter_admit|origin/held tickets|DATA_NOT_NATIVE_RESERVATION|Native endpoint and factual close
auto|src/provisioning/enrollment_pst.c|ep_tls_profile_prepare|papacc-enroll/1/borrowed credentials|NOT_LISTENER_REGISTRATION|Versioned purpose/runtime collection
issuer|src/issuer/issuer_core.c|view_ok|ROOT_PROOFS|RECOVERY_PROOFS_STILL_REQUIRED|M3 purpose-separated NORMAL implementation
issuer|src/issuer/issuer_core.c|papacc_issuer_transition|state/target/request|NOT_M7_SESSION_FANOUT|Durable revoke to actual session denial/Stop
issuer|src/issuer/issuer_core.c|papacc_issuer_recover|recover operation/root authority|NOT_NORMAL_RESTART|Recovery proofs remain mandatory
issuer-root|apps/server/server_configuration_root_transaction_win32.inc|issuer_root_port_read|epoch/proofs absent|INIT_DENY|Actual key/freshness/prefix/history/current
issuer-root|apps/server/server_configuration_root_transaction_win32.inc|issuer_root_port_cas|publish unsupported/commit uncertain|NOT_LIVE_PUBLICATION|End-to-end CAS/CURRENT acknowledgement
issuer-root|apps/server/server_configuration_root_transaction_win32.inc|papacc_server_launcher_configuration_issuer_publish_live|postcommit projection|LOW_LEVEL_LIVE_HOOK|Connect issuer port under complete authority
restart|apps/server/server_configuration_restart_win32.inc|papacc_configuration_recovery_guard03|volume/FileID/content witness|NOT_ISSUER_M3|Current witness source and purpose proof
listeners|apps/server/server_listener_composition_win32.inc|papacc_server_launcher_listener_ready|current/reference/resources|NOT_NETWORK_AUTHORITY|Enrollment representation/runtime multiplicity
'''
rows=list(csv.DictReader(spec.strip().splitlines(),delimiter='|'))
errors=[];pins={};evidence=[]
for row in rows:
    path=location(row['source']);text=path.read_text(encoding='utf-8-sig',errors='replace')
    matches=[i+1 for i,line in enumerate(text.splitlines()) if re.search(r'\b'+re.escape(row['symbol'])+r'\s*\(',line)]
    if not matches: errors.append('missing symbol '+row['symbol'])
    row['lines']=';'.join(map(str,matches));pins[row['source']]=digest(path)
    evidence.append(dict(source=row['source'],symbol=row['symbol'],lines=matches,
                         scope='LEXICAL_LOCATION_NOT_RUNTIME_OR_COMPLETE_CALLGRAPH'))
with (HERE/'CALLER-MAP.csv').open('w',encoding='utf-8-sig',newline='') as f:
    w=csv.DictWriter(f,fieldnames=list(rows[0]));w.writeheader();w.writerows(rows)
save('CALLER-EVIDENCE.json',evidence)

names=['DECISION-manual-package-m0-m8-20261020.md',
 'DECISION-package-signer-delegation-persistence-20261020.md',
 'DECISION-pairing-enrollment-transport-20261020.md',
 'DECISION-enrollment-listener-adr0009-specialization-20261020.md',
 'DECISION-factual-evidence-e0-e8-lifecycle-20261021.md',
 'DECISION-phase6-personal-mvp-20261022.md',
 'DECISION-mode-c-d2-one-operation-preparation-exception-20261026.md']
decisions=[]
for name in names:
    rel='docs/adr/'+name;p=ROOT/rel;t=p.read_text(encoding='utf-8-sig')
    accepted=bool(re.search(r'^status:\s*accepted\s*$',t,re.M))
    if not accepted: errors.append('decision not accepted '+name)
    pins[rel]=digest(p);decisions.append(dict(source=rel,status='ACCEPTED' if accepted else 'UNKNOWN',operations_granted=False))
save('DECISION-STATE.json',decisions)

old='docs/integration/n0start03-integrated-closeout-20261024/d0-d9-bounded-diagnostic-policy-20261026/HYBRID-POLICY-CANDIDATE.REVISION2.json'
historical=json.loads((ROOT/old).read_text(encoding='utf-8-sig'));pins[old]=digest(ROOT/old)
parameters=[]
for item in historical['parameters']:
    p=dict(item);p['source_annex']=old;p['current_authority']='INGREDIENT_ONLY_NOT_TARGET_ADMISSION'
    p['multiplicity']='As scoped; embedded subsets/shared resources must not be charged twice'
    p['invalidation']='Artifact/backend/owner/creation/root-policy-epoch/lifetime/phase changed; error/Stop/retained => DENY'
    if p['classification'].startswith('COMPILED'): p['reuse_limit']='HISTORICAL_LAYOUT_NOT_REBUILT_OR_NEW_FULL_TARGET_CENSUS'
    if p['value'] is None: p['required_evidence']='Justified target policy/attribution; no value inferable from this audit'
    parameters.append(p)
save('D4-D5-ANNEX.json',dict(format='OFFLINE_D4_D5_SOURCE_RECONCILIATION',status='INCOMPLETE_DENY',
 d2='ACCEPTED_PREPARATION_ONLY',parameters=parameters,
 arithmetic=dict(browser_one_historical_managed=1858020,browser_cap=2097152,
   residual_arithmetic_only=2097152-1858020,two_historical_charges=3716040,second_operation_fits=False),
 constraints=[
 'Effective interval is contained in original expiry AND applicable target validity; copied history never renews it',
 'C8 100ms completion/1000ms expiry is not Mode C policy or Browser clock conversion',
 'Progress-reset 30000ms is not whole-duration; summing local timeouts is not a bound',
 'Physical/commit host availability and per-process private/WS/pool overlap; do not sum',
 'Provider initialized/version/id is not memory/kernel bound or mapped-image attestation',
 'Controlled UNKNOWN and Q4 retained costs cannot be refunded or reused'],
 bindings={k:None for k in ('host_boot','operator_token','browser_pid_creation','accelerator_pid_creation',
   'mapped_modules','root_current_epoch','site_tls_profile','destination','resource_approval')},
 operational_request_ready=False,command=None,approval=None,authority='DENY'))
save('EXECUTION-VERIFICATION.json',dict(status='PASS_SOURCE_AND_DOCUMENT_AUDIT_ONLY' if not errors else 'FAIL',
 errors=errors,source_symbols=len(rows),accepted_records=len(decisions),parameters=len(parameters),
 accelerator_head=subprocess.check_output(['git','-C',str(ROOT),'rev-parse','HEAD'],text=True).strip(),
 browser_head=subprocess.check_output(['git','-C',str(BROWSER),'rev-parse','HEAD'],text=True).strip(),
 baseline_sha256=expected,pins=pins,product_code_changes=False,product_tests='NOT_RUN_NO_FUNCTIONAL_CHANGE',
 builds='NOT_RUN_NO_FUNCTIONAL_CHANGE',effects='NONE',ed11='PARTIAL',phase6='PARTIAL',authority='DENY'))
print(json.dumps(dict(errors=errors,source_symbols=len(rows),parameters=len(parameters),accepted_records=len(decisions))))
raise SystemExit(bool(errors))
