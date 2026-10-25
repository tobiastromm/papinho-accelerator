from pathlib import Path
import subprocess,json,time,re,hashlib,csv
root=Path(__file__).resolve().parents[3];pkg=Path(__file__).resolve().parent
logs=pkg/('campaign-'+str(time.time_ns()));logs.mkdir(exist_ok=False)
results=[]
for profile in ['Selected','Legacy']:
 for config in ['Debug','Release']:
  exe=root/'build/native-c-premonitor-handle-pressure-20261024'/profile/config/'fixture.exe'
  cases=[('bench',m,n,'clean',r) for m in ['current','proposed'] for n in [32,256,1024] for r in range(3)]
  cases += [('bench',m,32,c,0) for m in ['current','proposed'] for c in ['transient','competing','persistent','token','identity','descriptor','generation','stale60','stale1','stop','unknown','child','project']]
  cases += [('anchor',m,32,c,0) for m in ['current','proposed'] for c in ['clean','token','modified','stop']]
  cases += [('anchor','proposed',32,'close-unknown',0)]
  for mode,method,n,case,rep in cases:
   name=f'{profile}-{config}-{mode}-{method}-{n}-{case}-{rep}';cmd=[str(exe),mode,method,str(n),case]
   child=subprocess.Popen(cmd,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,creationflags=0x08000000)
   output=child.communicate()[0];code=child.wait(timeout=5) # no timeout kill or automatic retry
   (logs/(name+'.log')).write_bytes(output)
   text=output.decode('ascii');line=next((l for l in text.splitlines() if l.startswith('BENCH ') or l.startswith('ANCHOR ') or l.startswith('RETAINED ')),None)
   f=dict(re.findall(r'(\w+)=([^\s]+)',line or ''))
   expected=2 if case in ['unknown','close-unknown'] else 0
   passed=code==expected and bool(f)
   if mode=='bench' and case=='clean':passed=passed and int(f['callbacks'])==(n+2 if method=='current' else 4) and f['result']=='0'
   if mode=='bench' and case=='transient':passed=passed and f['result']==('5' if method=='current' else '0')
   if mode=='bench' and case=='competing':passed=passed and f['result']==('5' if method=='current' else '170')
   if mode=='anchor' and case=='clean':passed=passed and int(f['before'])-int(f['stable'])==(1 if method=='proposed' else 0) and f['comparisons']=='64'
   if case=='close-unknown':passed=passed and f.get('anchor') not in ['0000000000000000',None] and f.get('freeze')=='1'
   row=dict(run=name,profile=profile,config=config,mode=mode,method=method,entries=n,case=case,replica=rep,exit=code,expected=expected,passed=passed,values=f,exe_sha256=hashlib.sha256(exe.read_bytes()).hexdigest());results.append(row)
   (logs/'results.json').write_text(json.dumps(results,indent=2),encoding='utf-8')
   if not passed:raise RuntimeError(('STOP',name,code,text))
print('PASS',len(results),'COUNTEREXAMPLES_EXPECTED_NOT_SECURITY_PASS')
(pkg/'qualified-campaign.json').write_text(json.dumps({'directory':logs.name,'runs':len(results)},indent=2),encoding='utf-8')
