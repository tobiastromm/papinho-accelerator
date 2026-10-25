from pathlib import Path,PureWindowsPath
import copy,json,re,hashlib
p=Path(__file__).resolve().parent
c=json.loads((p/'candidate-binding.INERT.json').read_text())
def validate(x):
 a=x['attempt'];assert re.fullmatch('[0-9a-f]{32}',a) and a not in x['historical_attempts_forbidden']
 assert x['status']=='INERT_CANDIDATE_ONLY_NOT_OWNER_SELECTED_NOT_ACTIVATED' and not x['activated'] and not x['objects_created'] and not x['SCM_ACL_mutations']
 assert x['operation']=='N0TOKEN-20261025-01' and x['generation']['bits']==128 and x['generation']['calls']==1 and not x['generation']['derived_from_history']
 assert x['target_service']=='PapinhoAcceleratorLab20261023' and x['account']=='NT SERVICE'+chr(92)+x['target_service']
 assert x['mutex']=='Global'+chr(92)+'PapinhoAccelerator.ED11.SingleLabAdmission' and x['owner']=='PS_ADMISSION existing canonical owner'
 assert x['host_sha256']=='2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff' and x['boot_record']==236441
 leaves={'intent':'papacc-psr-'+a+'.intent','image':'papacc_server_lab_host_ntstatus_'+a+'.exe','sink':'papacc-psr-'+a+'.diag','evidence':'papacc-n2-'+a+'.evidence','dossier':'papacc-n2-'+a+'.approved','future_exact_approval':'EXACT-TOKEN-OBSERVATION-'+a+'-APPROVED.json'}
 folded=[]
 for k,v in x['paths'].items():
  path=PureWindowsPath(v);assert path.name==leaves[k] and path.is_absolute() and '..' not in path.parts
  assert all(h not in v for h in x['historical_attempts_forbidden']);folded.append(str(path).casefold())
 assert len(set(folded))==len(folded)
 assert PureWindowsPath(x['paths']['intent']).parent==PureWindowsPath(x['paths']['image']).parent==PureWindowsPath(x['paths']['sink']).parent
 assert str(PureWindowsPath(x['paths']['intent']).parent).casefold()==r'C:\Projetos\PapinhoAccelerator\build\scm-protected-installation-binding-20261022\Selected\Release'.casefold()
 assert x['arm']==dict(container='sink',offset=0,bytes=1024,kind=0,separate_file=False)
 for key in ['future_native_ids','future_service_pid_creation','future_token_statistics','future_metrics','new_artifact_hashes','operational_approval']:assert x[key] is None
 assert x['default_profile']=='EMPTY_DENY'
 assert x['limits']=={'native_handles': 128, 'admin_shared_handles': 1024, 'actual_roles': 4, 'preflight_ms': 60000, 'final_ms': 1000, 'admission_physical_bytes': 5368709120, 'admission_commit_available_bytes': 9663676416, 'monitor_physical_floor_bytes': 4294967296, 'monitor_commit_floor_bytes': 8589934592, 'selected_controlled_bytes': 3145728, 'legacy_controlled_bytes': 2097152, 'diagnostic_frame_bytes': 1024, 'diagnostic_file_bytes': 2048, 'diagnostic_owned_refs': 2, 'starts': 1, 'safe_stops_at_most': 1}
 assert x['HOST_CREDITS']=='NONE_GRANTED' and x['N7']=='REQUIRED_NOT_GRANTED_FOR_NEW_IDENTITY' and x['independent_budget']=='REQUIRED_NOT_GRANTED'
 assert hashlib.sha256(Path(x['Q2_record']['path']).read_bytes()).hexdigest()==x['Q2_record']['sha256']=='5794e17265af6e34d335591b0fed42e5d9048aec81e751578fd9f4132cf524e1'
validate(c)
cases=[]
def deny(name,change):
 x=copy.deepcopy(c);change(x)
 try:validate(x)
 except (AssertionError,KeyError,ValueError):cases.append(dict(case=name,result='DENY_AS_EXPECTED'));return
 raise AssertionError('MUTANT_SURVIVED '+name)
deny('old03',lambda x:x.update(attempt='0b1de869e11493b7cb08fb329578f6ce'))
deny('old02',lambda x:x.update(attempt='75330d11cc7e4123ae62eadc9a59c2f5'))
deny('upper_identity',lambda x:x.update(attempt=x['attempt'].upper()))
deny('wrong_length',lambda x:x.update(attempt=x['attempt']+'0'))
for key in c['paths']:
 deny('old_file_'+key,lambda x,k=key:x['paths'].update({k:x['paths'][k].replace(c['attempt'],'0b1de869e11493b7cb08fb329578f6ce')}))
deny('docs_intent_authority',lambda x:x['paths'].update(intent=r'C:\Projetos\PapinhoAccelerator\docs\manual-native-intent.txt'))
deny('sink_image_alias',lambda x:x['paths'].update(sink=x['paths']['image']))
deny('separate_arm_object',lambda x:x['arm'].update(separate_file=True))
deny('activated',lambda x:x.update(activated=True))
deny('objects_created',lambda x:x.update(objects_created=True))
deny('SCM_ACL_mutation',lambda x:x.update(SCM_ACL_mutations=True))
deny('host_divergence',lambda x:x.update(host_sha256='0'*64))
deny('boot_divergence',lambda x:x.update(boot_record=236442))
deny('owner_replacement',lambda x:x.update(owner='new owner'))
deny('mutex_replacement',lambda x:x.update(mutex='another mutex'))
deny('account_divergence',lambda x:x.update(account='LocalSystem'))
deny('service_replacement',lambda x:x.update(target_service='new service'))
for key in ['future_native_ids','future_service_pid_creation','future_token_statistics','future_metrics','new_artifact_hashes','operational_approval']:
 deny('invented_'+key,lambda x,k=key:x.update({k:'invented'}))
deny('credit_grant',lambda x:x.update(HOST_CREDITS='GRANTED'))
deny('old_N7_inherited',lambda x:x.update(N7='ACCEPTED_FOR_03'))
deny('budget_granted',lambda x:x.update(independent_budget='GRANTED'))
for key in c['limits']:
 deny('changed_limit_'+key,lambda x,k=key:x['limits'].update({k:x['limits'][k]+1}))
deny('default_allow',lambda x:x.update(default_profile='ALLOW'))
(p/'candidate-negative-results.json').write_text(json.dumps({'valid_inert_candidate':'PASS','negative_cases':cases,'scope':'candidate data only; not canonical owner/admission implementation'},indent=2))
print('CANDIDATE_DATA_QUALIFICATION=PASS negative_cases='+str(len(cases))+' NO_ACTIVATION_NO_EFFECT')
