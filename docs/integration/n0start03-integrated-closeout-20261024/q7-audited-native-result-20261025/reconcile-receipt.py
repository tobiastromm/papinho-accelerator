from pathlib import Path
import json,re,copy
p=Path(__file__).resolve().parent;t=(p/'OWNER-MANUAL-Q7-RESULT-AS-REPORTED.txt').read_text(encoding='utf-8-sig')
def vals(line):
 return {k:v for k,v in re.findall(r'([A-Za-z_][A-Za-z_0-9]*)=([^ ;\r\n]+)',line)}
final=vals(next(x for x in t.splitlines() if x.startswith('IG_FINAL ')))
meta=vals(next(x for x in t.splitlines() if x.startswith('PACC_PSR_FIRST_FAILURE ')))
role=vals(next(x for x in t.splitlines() if x.startswith('IG_CENSUS_ORIGINAL ')))
mask=vals(next(x for x in t.splitlines() if x.startswith('IG_CENSUS_ROLE_MASK ')))
assert final['first']=='13' and final['starts']=='1' and final['stops']=='0' and final['close']=='0' and final['quiescent']=='1' and final['intent_consumed']=='1' and final['effect']=='2' and final['diagnostic_created']=='1'
assert meta['pid']=='22620' and meta['predicate']=='token.specific_group.enabled' and meta['returned_win32']=='5' and meta['api_success']=='1' and meta['raw_win32']=='0' and meta['nt_valid']=='0'
def classify(m):
 if m['q_error']!='0' or m['q_known']!='15' or m['q_complete']!='1':return 'UNKNOWN'
 n=int(m['sid_count']);a=int(m['sid_attrs'],16)
 if n>1:return 'AMBIGUOUS'
 if n==0:return 'ABSENT_FROM_GROUPS' if a==0 else 'INCONSISTENT'
 if a&16:return 'DENY_ONLY'
 if a&4:return 'ENABLED'
 if a&2:return 'DEFAULT_WITHOUT_ENABLED'
 return 'DISABLED'
cases=[('observed',{},'ABSENT_FROM_GROUPS'),('query-failed',{'q_error':'5'},'UNKNOWN'),('partial',{'q_complete':'0'},'UNKNOWN'),('missing-query',{'q_known':'3'},'UNKNOWN'),('enabled',{'sid_count':'1','sid_attrs':'00000004'},'ENABLED'),('default-owner',{'sid_count':'1','sid_attrs':'0000000a'},'DEFAULT_WITHOUT_ENABLED'),('deny-only',{'sid_count':'1','sid_attrs':'00000010'},'DENY_ONLY'),('enabled-deny',{'sid_count':'1','sid_attrs':'00000014'},'DENY_ONLY'),('disabled',{'sid_count':'1','sid_attrs':'00000000'},'DISABLED'),('duplicate',{'sid_count':'2'},'AMBIGUOUS'),('contradictory',{'sid_attrs':'00000004'},'INCONSISTENT')]
results=[]
for name,delta,expected in cases:
 m=dict(meta);m.update(delta);v=classify(m);assert v==expected;results.append(dict(case=name,result=v,status='PASS'))
assert int(mask['expected'])^int(mask['seen'])==int(mask['missing'])==4
assert role['expected_pid']==role['observed_pid']=='22620' and role['expected_creation']==role['observed_creation']=='134374440955188877'
assert role['result']=='2' and role['query_error']=='0' and role['wait']=='0'
assert int(role['begin'])<=int(role['end'])<=int(role['observed'])
samples=[]
for x in t.splitlines():
 if x.startswith('IG_PROCESS '):samples.append({k:int(v) if k!='KERNEL_TRANSITIVE' else v for k,v in vals(x).items()})
admin=[samples[0]['handles']+samples[2]['handles'],samples[3]['handles']+samples[5]['handles']]
qual=[x['handles'] for x in samples if x['role']==1]
assert admin==[997,999] and qual==[118,120,121];assert max(qual)<=128 and max(admin)<=1024
(p/'RECEIPT-FACTS.json').write_text(json.dumps({'final':final,'token_observation':meta,'token_classification':classify(meta),'census_masks':mask,'original_process_correlation':role,'fully_paired_administrative_samples':admin,'qualifier_samples':qual,'max_qualifier_sample':max(qual),'unknown_unsampled_peak':True,'service_resource_metrics':'NOT_PRINTED','last_admin_sample':'INCOMPLETE_AUXILIARY_SAMPLE_NOT_REPRINTED','global_quiescence':'UNPROVEN','approval':'USED_ONCE_NONREPLAYABLE'},indent=2)+'\n')
(p/'RECONCILIATION-CHECKS.json').write_text(json.dumps({'token_classification_cases':results,'receipt_flags':'PASS','identity_and_mask_correlation':'PASS','sample_accounting_no_cross_process_doublecount':'PASS','scope':'OFFLINE_TRANSCRIPT_AND_DECODER_ONLY_NOT_NATIVE_TOKEN_REQUERY_OR_PRODUCT_QUALIFICATION'},indent=2)+'\n')
print('RECEIPT_RECONCILIATION=PASS; TOKEN_DECODER=11_PASS; ORIGINAL_PROCESS_SIGNALED_AFTER_SNAPSHOT; NO_OPERATION')
