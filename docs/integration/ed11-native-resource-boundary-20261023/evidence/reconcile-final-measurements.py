from pathlib import Path
import re,json,statistics
p=Path(r'C:\Projetos\_tmp\PapinhoAccelerator\ed11-native-resource-boundary-20261023');profiles={}
for profile in ['Selected','Legacy']:
 for config in ['Debug','Release']:
  lines=(p/f'service-{profile}-{config}.asm').read_text(errors='replace').splitlines();functions={};name=None;body=[]
  for line in lines:
   m=re.match(r'^(\S+)\s+PROC(?:\s|$)',line)
   if m:name=m.group(1);body=[]
   elif name and re.match(re.escape(name)+r'\s+ENDP',line):
    instructions=[x.strip().split(';')[0].strip() for x in body if x.startswith('\t') and not x.lstrip().startswith(';')];alloc=0;push=0;rax=None;seen_alloc=False
    for ins in instructions[:30]:
     m=re.match(r'mov\s+eax,\s*(\d+)\b',ins)
     if m:rax=int(m.group(1))
     if re.match(r'push\s+r\w+\b',ins):push+=8
     m=re.match(r'sub\s+rsp,\s*(\d+|rax)\b',ins)
     if m:alloc=rax if m.group(1)=='rax' else int(m.group(1));seen_alloc=True;break
     if ins.startswith('ret'):break
    calls=[]
    for ins in instructions:
     m=re.match(r'call\s+(.+)',ins)
     if m:calls.append(m.group(1))
    functions[name]=dict(stack_sub=alloc,saved_register_push_bytes=push,local_stack_extent_excluding_return=alloc+push,calls=list(dict.fromkeys(calls)),scope='THIS_OBJECT_STATIC_FRAME_NOT_OS_CRT_OR_FULL_TRANSITIVE_BOUND')
    name=None
   elif name:body.append(line)
  chains=[['ps_main','psr_prepare','psrd_bind','psrd_security','psrd_descriptor'],['ps_main','psr_prepare','psr_open','psrt_return','psrd_emit','psrf_finish','psrf_write'],['ps_main','psr_prepare','psrd_prepare_complete','psrt_prepared','psrd_emit','psrf_finish','psrf_write']]
  if config=='Release':
   chains=[chains[0],['ps_main','psr_prepare','psr_open','psrt_return','psrd_emit','psrf_finish'],['ps_main','psr_prepare','psrd_emit','psrf_finish']]
  prefix=[]
  for chain in chains:
   present=[x for x in chain if x in functions];edges=[dict(caller=x,callee=y,explicit_call=any(c==y for c in functions[x]['calls'])) for x,y in zip(present,present[1:])]
   prefix.append(dict(path=chain,present=present,all_edges_explicit=all(x['explicit_call'] for x in edges),edges=edges,known_stack_bytes_with_return_slots=sum(functions[x]['local_stack_extent_excluding_return']+8 for x in present),interpretation='KNOWN_PREFIX_ONLY_EXCLUDES_UNSEEN_NATIVE_AND_CRT_CALLEES_AND_CALLER_THREAD_SETUP'))
  profiles[f'{profile}-{config}']=dict(functions={k:v for k,v in functions.items() if k.startswith(('psrd_','psrf_','psrt_','psr_')) or k=='ps_main'},chains=prefix)
(p/'stack-frames-and-callgraph.json').write_text(json.dumps(profiles,indent=2));out=[]
for log in sorted(p.glob('measurement-final-*.jsonl')):
 rows=[json.loads(x) for x in log.read_text().splitlines()];facts=rows.pop();baseline=next(x for x in rows if x['phase']=='stable_pre_file_baseline');end=rows[-1];fields=['private_bytes','working_set','peak_working_set','peak_commit_charge','handles','process_quota_paged','process_quota_peak_paged','process_quota_nonpaged','process_quota_peak_nonpaged','stack_committed_va','stack_reserved_va','stack_pointer_distance','host_commit_pages','host_kernel_paged_pages','host_kernel_nonpaged_pages','host_cache_pages','host_available_physical']
 out.append(dict(log=log.name,configuration=log.name.split('-')[2],facts=facts,duration_observed_ms=(end['qpc']-baseline['qpc'])*1000/facts['frequency'],fields={k:dict(baseline=baseline[k],observed_max=max(x[k] for x in rows),observed_min=min(x[k] for x in rows),final=end[k],max_minus_baseline=max(x[k] for x in rows)-baseline[k],final_minus_baseline=end[k]-baseline[k]) for k in fields},io=dict(read_operations=end['io_read_ops']-baseline['io_read_ops'],write_operations=end['io_write_ops']-baseline['io_write_ops'],read_bytes=end['io_read_bytes']-baseline['io_read_bytes'],write_bytes=end['io_write_bytes']-baseline['io_write_bytes']),interpretation='EMPIRICAL_BRACKETED_SNAPSHOT_NOT_CONTINUOUS_PEAK_NOT_PER_FILE_KERNEL_ATTRIBUTION'))
(p/'measurements-final-reconciled.json').write_text(json.dumps(out,indent=2));
for cfg in ['Debug','Release']:
 a=[x for x in out if x['configuration']==cfg];print(cfg,'private delta',[x['fields']['private_bytes']['max_minus_baseline'] for x in a],'handles delta',[x['fields']['handles']['max_minus_baseline'] for x in a],'commit stack',[x['fields']['stack_committed_va']['observed_max'] for x in a],'physical min',min(x['fields']['host_available_physical']['observed_min'] for x in a))
for key in ['Selected-Debug','Selected-Release']:
 v=profiles[key];print(key,{x:v['functions'].get(x,{}).get('local_stack_extent_excluding_return') for x in ['ps_main','psr_prepare','psr_open','psrt_return','psrd_emit','psrf_finish','psrf_write']},[(x['all_edges_explicit'],x['known_stack_bytes_with_return_slots']) for x in v['chains']])
