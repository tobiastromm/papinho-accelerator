from pathlib import Path
import subprocess,json,hashlib,csv
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/composite-target-quantitative-closure-20261026';runs=[];samples=[];providers=[]
for backend in ['OpenSSL-x64','NSS-VC6-x86']:
 for cfg in ['Debug','Release']:
  out=r/'build/composite-target-quantitative-20261026'/backend/cfg;exe=out/'runtime-core-observation.exe'
  for rep in range(1,6):
   q=subprocess.run([str(exe)],cwd=out,capture_output=True);data=q.stdout+q.stderr;(p/f'{backend}-{cfg}-run{rep}.log').write_bytes(data);text=data.decode(errors='replace');row=dict(backend=backend,config=cfg,run=rep,exit=q.returncode,exe_sha256=hashlib.sha256(exe.read_bytes()).hexdigest());runs.append(row)
   if q.returncode:print(text);raise SystemExit(1)
   assert 'FINAL first=0 runtime_ref=0 module_ref=0 retained=0' in text
   for line in text.splitlines():
    if line.startswith('S,'):
     a=line.split(',');samples.append(dict(backend=backend,config=cfg,run=rep,sequence=int(a[1]),cycle=int(a[2]),phase=a[3],pid=int(a[4]),creation=int(a[5]),begin=int(a[6]),end=int(a[7]),private=int(a[8]),ws=int(a[9]),commit=int(a[10]),paged=int(a[11]),nonpaged=int(a[12]),peak_commit=int(a[13]),handles=int(a[14])))
    if line.startswith('P,'):
     a=line.split(',');assert int(a[5])==0;providers.append(dict(backend=backend,config=cfg,run=rep,cycle=int(a[1]),index=int(a[2]),provider=a[3],available=int(a[4]),initialized=int(a[5])))
  print(backend,cfg,'5 independent processes PASS',flush=True)
for name,data in [('RUNTIME-SAMPLES.csv',samples),('PROVIDER-STATE.csv',providers)]:
 with (p/name).open('w',encoding='utf-8',newline='') as f:w=csv.DictWriter(f,list(data[0]));w.writeheader();w.writerows(data)
(p/'RUNTIME-RUNS.json').write_text(json.dumps(runs,indent=2)+'\n',encoding='utf-8');summary=[]
for backend in ['OpenSSL-x64','NSS-VC6-x86']:
 for cfg in ['Debug','Release']:
  group=[x for x in samples if x['backend']==backend and x['config']==cfg];deltas=[]
  for rep in range(1,6):
   g=[x for x in group if x['run']==rep];base=g[0];live=[x for x in g if x['phase']=='CORE_LIVE_PROVIDER_UNINITIALIZED'];final=g[-1];deltas.append(dict(run=rep,private_sampled_peak_above_baseline=max(x['private'] for x in g)-base['private'],handle_sampled_peak_above_baseline=max(x['handles'] for x in g)-base['handles'],final_private_minus_baseline=final['private']-base['private'],final_handles_minus_baseline=final['handles']-base['handles']))
  summary.append(dict(backend=backend,config=cfg,samples=len(group),cycles=60,deltas=deltas,scope='runtime core only, public provider initialized=0; no perConnection/provider/kernel envelope'))
(p/'RUNTIME-QUANTITATIVE-SUMMARY.json').write_text(json.dumps(summary,indent=2)+'\n',encoding='utf-8');print(json.dumps(summary,indent=2))
