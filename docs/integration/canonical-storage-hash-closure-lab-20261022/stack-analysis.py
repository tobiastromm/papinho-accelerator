import pathlib,os,re,json,bisect
p=pathlib.Path(os.environ['TEMP'])/'pacc-storage-hash-lab-20261022';results={}
def number(s):
 s=s.strip()
 return int(s[:-1],16) if s.lower().endswith('h') else int(s,16) if re.fullmatch('[0-9A-Fa-f]{8,16}',s) else int(s)
for cfg in ['msvc-Debug','msvc-Release','vc6-Debug','vc6-Release']:
 directory=p/cfg;word=8 if cfg.startswith('msvc') else 4;inst={}
 for line in (directory/'closure-disasm.txt').read_text(errors='replace').splitlines():
  m=re.match(r'\s*([0-9A-Fa-f]{8,16}):\s+(?:(?:[0-9A-Fa-f]{2})\s+)+([a-z][a-z0-9]*)\s*(.*)',line)
  if m:inst[int(m[1],16)]=(m[2],m[3].strip())
 names={}
 for line in (directory/'closure.map').read_text(errors='replace').splitlines():
  m=re.match(r'\s+0001:[0-9a-fA-F]+\s+(\S+)\s+([0-9a-fA-F]{8,16})\s+f\b',line)
  if m:names[int(m[2],16)]=m[1]
 starts=sorted(names);addresses=sorted(inst);following={a:addresses[i+1] if i+1<len(addresses) else None for i,a in enumerate(addresses)}
 local={}
 for index,start in enumerate(starts):
  end=starts[index+1] if index+1<len(starts) else max(inst)+16;todo=[(start,0,None)];seen={};peak=0;edges=[]
  while todo:
   address,depth,bp=todo.pop()
   if address in seen:
    if seen[address]!=(depth,bp):raise Exception((cfg,names[start],'unbalanced CFG join',hex(address),seen[address],(depth,bp)))
    continue
   seen[address]=(depth,bp)
   if address not in inst:raise Exception((cfg,names[start],'missing instruction',hex(address)))
   op,arg=inst[address];nxt=following[address];peak=max(peak,depth)
   if op=='push':depth+=word
   elif op=='pop':depth-=word
   elif op in ['add','sub'] and re.match(r'(e|r)sp,',arg):
    imm=number(arg.split(',',1)[1]);depth+=imm if op=='sub' else -imm
   elif op=='mov' and arg.replace(' ','') in ['ebp,esp','rbp,rsp']:bp=depth
   elif op=='mov' and arg.replace(' ','') in ['esp,ebp','rsp,rbp']:
    if bp is None:raise Exception('unknown frame pointer')
    depth=bp
   elif op=='leave':depth=bp-word
   elif op=='call':
    target=number(arg)
    if target not in names:raise Exception((cfg,'unresolved call',hex(target)))
    edges.append({'target':target,'depth':depth,'tail':False})
   elif op.startswith('ret'):
    if depth!=0:raise Exception((cfg,names[start],'nonzero return',depth))
    continue
   elif op in ['int','int3','ud2']:continue
   elif re.match(r'(?:e|r)sp(?:,|$)',arg) and op not in ['cmp','test']:raise Exception(('unhandled stack instruction',op,arg))
   peak=max(peak,depth)
   if depth<0:raise Exception('negative frame')
   if op.startswith('j'):
    target=number(arg)
    if target<start or target>=end:
     if op!='jmp' or target not in names or depth!=0:raise Exception((cfg,names[start],'external branch',hex(target),depth))
     edges.append({'target':target,'depth':depth,'tail':True})
    else:todo.append((target,depth,bp))
    if op=='jmp':continue
   if nxt is None or nxt>=end:raise Exception((cfg,names[start],'falls out',hex(address)))
   todo.append((nxt,depth,bp))
  local[start]={'name':names[start],'local_stack_peak':peak,'calls':edges,'reachable_instruction_count':len(seen)}
 memo={};visiting=set()
 def total(addr):
  if addr in memo:return memo[addr]
  if addr in visiting:raise Exception((cfg,'recursive call graph'))
  visiting.add(addr);v=local[addr]['local_stack_peak']
  for e in local[addr]['calls']:v=max(v,e['depth']+(0 if e['tail'] else word)+total(e['target']))
  visiting.remove(addr);memo[addr]=v;return v
 for a in starts:total(a)
 report={names[a]:{'local_peak_excluding_return':local[a]['local_stack_peak'],'nested_including_entry_return':word+memo[a],'calls':sorted(set(names[e['target']] for e in local[a]['calls']))} for a in starts}
 maxall=max(word+v for v in memo.values());cb=next(a for a in starts if names[a].lstrip('_')=='pacc_storage_hash_callback')
 results[cfg]={'word_bytes':word,'maximum_over_all_probe_and_capsule_entries':maxall,'callback_nested_bound':word+memo[cb],'frames':report,'validation':'CFG stack depth equal at all joins including loops; direct acyclic calls; tailcalls reuse caller frame; no unresolved/indirect call; excludes caller/OS startup stack'}
print(json.dumps({n:{k:v for k,v in r.items() if k!='frames'} for n,r in results.items()},indent=2))
(p/'stack-bound.json').write_text(json.dumps(results,indent=2)+'\n')
