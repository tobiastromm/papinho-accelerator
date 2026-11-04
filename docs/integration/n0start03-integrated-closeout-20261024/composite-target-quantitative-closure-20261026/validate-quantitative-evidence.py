from pathlib import Path
import json,csv,copy
P=Path(__file__).resolve().parent

def validate(rows,states):
 if len(rows)!=25 or len(states)!=12:return False
 pid=rows[0]['pid'];creation=rows[0]['creation'];prev=0
 expected='openssl' if rows[0]['backend']=='OpenSSL-x64' else 'retrozilla-nss'
 binding=(rows[0]['backend'],rows[0]['config'],rows[0]['run'])
 for n,x in enumerate(rows):
  if (x['backend'],x['config'],x['run'])!=binding:return False
  if x['sequence']!=n+1 or x['pid']!=pid or x['creation']!=creation:return False
  if x['begin']>x['end'] or x['begin']<prev:return False
  prev=x['end']
  cycle=(n+1)//2;phase='BASELINE' if n==0 else ('CORE_LIVE_PROVIDER_UNINITIALIZED' if n%2 else 'PUBLIC_RELEASE_RETURNED')
  if x['cycle']!=cycle or x['phase']!=phase:return False
  if min(x[k] for k in ['private','ws','commit','paged','nonpaged','peak_commit','handles'])<0:return False
 for n,x in enumerate(states):
  if (x['backend'],x['config'],x['run'])!=binding:return False
  if x['cycle']!=n+1 or x['index']!=0 or x['initialized']!=0 or x['available']!=1 or x['provider']!=expected:return False
 return True

rows=list(csv.DictReader((P/'RUNTIME-SAMPLES.csv').open()));states=list(csv.DictReader((P/'PROVIDER-STATE.csv').open()))
for x in rows:
 for k in x.keys()-{'backend','config','phase'}:x[k]=int(x[k])
for x in states:
 for k in x.keys()-{'backend','config','provider'}:x[k]=int(x[k])
print('providers',set(x['provider'] for x in states))
results=[]
for backend in ['OpenSSL-x64','NSS-VC6-x86']:
 for cfg in ['Debug','Release']:
  for run in range(1,6):
   a=[x for x in rows if (x['backend'],x['config'],x['run'])==(backend,cfg,run)];b=[x for x in states if (x['backend'],x['config'],x['run'])==(backend,cfg,run)];assert validate(a,b),(backend,cfg,run);results.append(dict(case=f'{backend}/{cfg}/{run}',pass_result=True))
a=rows[:25];b=states[:12]
mutations=[('pid swap',3,'pid',0),('creation swap',3,'creation',0),('sequence replay',3,'sequence',1),('phase missing',3,'phase','UNKNOWN'),('cycle replay',3,'cycle',1),('negative handles',3,'handles',-1),('clock reversed',3,'end',0),('stale local order',3,'begin',0)]
for name,index,key,val in mutations:
 m=copy.deepcopy(a);m[index][key]=val;assert not validate(m,b),name;results.append(dict(case=name,pass_result=True))
for name,index,key,val in [('provider initialized',0,'initialized',1),('provider absent',0,'available',0),('provider mismatch',0,'provider','retrozilla-nss'),('provider replay',1,'cycle',1)]:
 m=copy.deepcopy(b);m[index][key]=val;assert not validate(a,m),name;results.append(dict(case=name,pass_result=True))
assert not validate(a[:-1],b);results.append(dict(case='missing final sample',pass_result=True))
assert not validate(a,b[:-1]);results.append(dict(case='missing provider observation',pass_result=True))
layout=json.loads((P/'COMPILED-CONTROLLED-LAYOUTS.json').read_text());br=layout[-1]['measurements'];charge=list(br.values())[-1];assert charge==1858020 and charge*2>2097152 and charge<2097152
results.append(dict(case='Browser actual charge fits once; twice denied by unchanged 2MiB arithmetic',pass_result=True))
(P/'EVIDENCE-ANALYSIS-TESTS.json').write_text(json.dumps(dict(scope='analysis pipeline, not product admission/freshness authority',tests=results,pass_count=len(results)),indent=2)+'\n');print('PASS',len(results))
