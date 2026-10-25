from pathlib import Path
import json,hashlib,re,copy,importlib.util
r=Path.cwd();p=Path(__file__).resolve().parent;prev=p.parent/'q7-scm-readback-correction-20261025';rf=p/'Q7-AUDITED-REQUEST.DRAFT.json';req=json.loads(rf.read_text(encoding='utf-8'));h=lambda f:hashlib.sha256(f.read_bytes()).hexdigest();src=r/'tools/qualification';pins=(src/'scm-n0-context-pins.h').read_text();q4pins=pins[pins.index('#if defined(PAPACC_Q4_TOKEN_DIAGNOSTIC)'):pins.index('#elif !defined(PAPACC_Q4_TOKEN_DIAGNOSTIC)')];context=(src/'scm-n0-context.inc').read_text();cycle=(src/'scm-instrumented-cycle-lab.inc').read_text();runtime=(src/'scm-runtime-cycle-lab.inc').read_text();owner=(r/'apps/server/server_service_admission_win32.inc').read_text();pe=Path(req['artifacts']['qualifier']['path']).read_bytes();spec=importlib.util.spec_from_file_location('pincheck',prev/'verify-image-pin-in-binary.py');pincheck=importlib.util.module_from_spec(spec);spec.loader.exec_module(pincheck)
def check(d):
 if d['operation']!='N0TOKEN-20261025-01' or d['attempt']!='10584f02e11f92c92b43cddf0d9d316f':raise ValueError('FIXED_IDENTITY')
 for item in list(d['artifacts'].values())+list(d['dependencies'].values()):
  if h(Path(item['path']))!=item['sha256'] or Path(item['path']).stat().st_size!=item['bytes']:raise ValueError('ARTIFACT_OR_POLICY_PIN')
 if h(Path(d['final_binding']['path']))!=d['final_binding']['sha256']:raise ValueError('FINAL_BINDING')
 pincheck.attest(pe,d['artifacts']['service_image']['sha256'],['2386d63a256affd48c3dfbcd78ff36380400c86f61a0eeb68e209d6ed4520821'])
 observed=json.loads('"'+re.search(r'cycle_previous_image\[\]=L"([^"\n]+)"',runtime).group(1)+'"');expected='"'+observed+'" service host'
 if expected!=d['SCM_precondition']['command']:raise ValueError('SCM_PREDECESSOR')
 if '"'+d['outputs']['image']+'" service host'!=d['SCM_precondition']['candidate_after_rebind']:raise ValueError('SCM_CANDIDATE')
 if observed.encode('utf-16le') not in pe or d['outputs']['image'].encode('utf-16le') not in pe:raise ValueError('COMPILED_IMAGEPATH')
 for item in d['protected_inputs']:
  prefix='DOSSIER' if item['role']=='record' else 'EVIDENCE';values=[]
  for key in ['VOLUME','HIGH','LOW','BYTES']:values.append(int(re.search(r'#define IG_N0_'+prefix+'_'+key+r' (\d+)UL',q4pins).group(1)))
  if values!=list(map(int,item['native_id'].split(':')))+[item['bytes']]:raise ValueError('ORIGINAL_INPUT_ID')
  if re.search(r'#define IG_N0_'+prefix+'_SHA "([0-9a-f]{64})"',q4pins).group(1)!=item['sha256']:raise ValueError('INPUT_HASH')
  if item['RAW1_sha256'] not in context:raise ValueError('RAW1')
 if d['limits']['native_handles']!=128 or d['limits']['shared_administrative_handles']!=1024 or d['limits']['participant_roles']!=4 or d['limits']['HOST_CREDITS']!='NONE_GRANTED':raise ValueError('BUDGET_CONTRACT')
 if d['limits']['global_freshness_s']!=60 or d['limits']['final_freshness_s']!=1:raise ValueError('FRESHNESS_CONTRACT')
 ident=(r/'apps/server/server_service_identity_win32.h').read_text()
 for key,macro in [('account','PAPACC_SCM_ACCOUNT_LITERAL'),('service','PAPACC_SCM_NAME_LITERAL')]:
  literal=re.search(r'#define '+macro+r' L"([^"\n]+)"',ident).group(1)
  if json.loads('"'+literal+'"')!=d['requirements'][key]:raise ValueError('SCM_ACCOUNT_OR_SERVICE')
 if d['requirements']['configuration_sha256'] not in context or d['requirements']['native_configuration_id']!='417614100:393216:1120240' or d['requirements']['native_parent_id']!='417614100:1966080:79426':raise ValueError('CONFIG_BINDING')
 if d['requirements']['host_sha256'] not in context or ('d->boot='+str(d['requirements']['boot'])+'ULL') not in context:raise ValueError('HOST_BOOT_PIN')
 sid=re.search(r'static const DWORD sid\[\]\s*=\s*\{([^}]+)\}',context).group(1)
 if [int(x) for x in re.findall(r'(\d+)UL',sid)]!=[int(x) for x in d['requirements']['specific_sid'].split('-')[3:]]:raise ValueError('SERVICE_SPECIFIC_SID')
 if d['manual_only'] is not True or d['automatic_execution'] is not False:raise ValueError('MANUAL_ONLY')
 if Path(d['approval_record_path']).exists():raise ValueError('UNEXPECTED_APPROVAL')
check(req)
cases=[('current',None),('old_operation',lambda x:x.update(attempt='0b1de869e11493b7cb08fb329578f6ce')),('bad_qualifier',lambda x:x['artifacts']['qualifier'].update(sha256='f'*64)),('bad_image',lambda x:x['artifacts']['service_image'].update(sha256='f'*64)),('old_n7',lambda x:x['dependencies']['N7'].update(sha256='c17aa105580aa7680e8d97042abc9daa32640dcf1e69dcdce49c59b8462109ac')),('generic_scm_path',lambda x:x['SCM_precondition'].update(command='"C:\\generic.exe" service host')),('old_candidate',lambda x:x['outputs'].update(image=x['SCM_precondition']['command'])),('substituted_input',lambda x:x['protected_inputs'][0].update(native_id='417614100:327680:2932527')),('changed_content',lambda x:x['protected_inputs'][0].update(sha256='f'*64)),('wrong_RAW1',lambda x:x['protected_inputs'][0].update(RAW1_sha256='f'*64)),('raised_native_quota',lambda x:x['limits'].update(native_handles=129)),('raised_admin_quota',lambda x:x['limits'].update(shared_administrative_handles=1025)),('fifth_role',lambda x:x['limits'].update(participant_roles=5)),('expired_threshold_relaxed',lambda x:x['limits'].update(final_freshness_s=2)),('automatic',lambda x:x.update(automatic_execution=True)),('wrong_binding',lambda x:x['final_binding'].update(sha256='f'*64))]
cases += [('wrong_account',lambda x:x['requirements'].update(account='LocalSystem')),('wrong_service',lambda x:x['requirements'].update(service='OtherLab')),('bad_config_hash',lambda x:x['requirements'].update(configuration_sha256='f'*64)),('substituted_config_id',lambda x:x['requirements'].update(native_configuration_id='417614100:393216:1120241')),('wrong_boot',lambda x:x['requirements'].update(boot=236442)),('wrong_service_sid',lambda x:x['requirements'].update(specific_sid='S-1-5-80-1-2-3-4-5'))]
results=[]
for name,mutate in cases:
 d=copy.deepcopy(req)
 if mutate:mutate(d)
 error=None
 try:check(d)
 except ValueError as e:error=str(e)
 assert (error is None)==(mutate is None),(name,error);results.append(dict(case=name,passed=True,result='ACCEPTED_STATIC_ONLY' if not error else 'REJECTED',reason=error))
# Real gate order and final owner freshness/CREATE_NEW, not a rewritten model.
labels=['PROTECTED_BINDING','CANONICAL_ADMISSION','SOURCE_IMAGE','SCM_READBACK','N0_PROTECTED_CONTEXT','CONFIG_BYTE_LOCK','PREFLIGHT_CENSUS','EXACT_POSITIVE_CONFIRMATION','PRECONSUME_ABSENCE','PRE_METRIC_REVALIDATION','PREFLIGHT_METRICS','CONFIRM_REVALIDATION','CONSUME_INTENT'];positions=[cycle.index('cycle_stage="'+x+'"') for x in labels];assert positions==sorted(positions)
assert owner.index('psa_n0_metrics_fresh("CONSUME")')<owner.index('PSA.consumed=1;')<owner.index('PSA.intent=CreateFileW')
assert 'CREATE_NEW' in owner and 'ig_absence.pending=1;native.retained=1;ig_gate.retained=1' in cycle
assert 'if(native.retained||ig_gate.retained||ig_absence.used' in cycle
# All current four qualifiers have native graph edges and compiled correct image pin.
compiled=[]
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  b=r/'build/q7-preconsumption-audit-20261025'/profile/cfg;blob=(b/'papacc_scm_instrumented_lab.exe').read_bytes();v=pincheck.attest(blob,req['artifacts']['service_image']['sha256']);rule=next(x for x in (b/'build.ninja').read_text().replace('\\','/').splitlines() if x.startswith('build CMakeFiles/papacc_scm_instrumented_lab.dir/tools/qualification/scm-registration-lab.c.obj:'));assert all(x in rule for x in ['scm-q4-image-pin.h','scm-runtime-cycle-lab.inc','scm-n0-context-pins.h']);v.update(profile=profile,config=cfg,explicit_dependencies=True,qualifier_sha256=h(b/'papacc_scm_instrumented_lab.exe'));compiled.append(v)
(p/'STATIC-CONSISTENCY-NEGATIVES.json').write_text(json.dumps(results,indent=2)+'\n');(p/'COMPILED-PINS-AND-DEPENDENCIES.json').write_text(json.dumps(compiled,indent=2)+'\n');(p/'ACTUAL-PRECONSUMPTION-ORDER.json').write_text(json.dumps(dict(stages=labels,owner_freshness_before_consume=True,exclusive_creation=True,pending_freezes_and_retains=True,scope='SOURCE_AND_BINARY_NOT_RUNTIME_ADMISSION'),indent=2)+'\n')
print('STATIC_CONSISTENCY=PASS cases='+str(len(results))+'; FOUR_COMPILED_PINS_AND_DEPENDENCIES=PASS; ACTUAL_GATE_ORDER=PASS; NO_OPERATION')
