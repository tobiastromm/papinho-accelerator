from pathlib import Path
import subprocess,json,re
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/explicit-inheritance';results=[]
for cfg in ['Debug','Release']:
 for strategy in ['legacy','limited']:
  for repeat in range(2):
   name=f'{cfg}-{strategy}-{repeat}'
   cmd=['powershell.exe','-NoProfile','-ExecutionPolicy','Bypass','-File',str(p/'isolated-supervisor.ps1'),'-Configuration',cfg,'-Strategy',strategy]
   v=subprocess.run(cmd,input=b'',capture_output=True,timeout=40);raw=v.stdout+v.stderr;(p/(name+'.log')).write_bytes(raw);t=raw.decode(errors='replace')
   assert v.returncode==0,(name,t[-1500:])
   expected=8 if strategy=='legacy' else 0
   assert f'matches={expected} expected={expected}' in t
   pre=int(re.search(r'TABLE_BEGIN phase=CRT_ENTRY.*?pre=(\d+)',t)[1]);results.append(dict(case=name,exit=v.returncode,child_entry=pre,synthetic_matches=expected))
 for fault in [0,1,2,3,4,5,6,8,9]:
  child='delay-child' if fault==0 else 'clean';name=f'{cfg}-fault-{fault}'
  cmd=['powershell.exe','-NoProfile','-ExecutionPolicy','Bypass','-File',str(p/'isolated-supervisor.ps1'),'-Configuration',cfg,'-Fault',str(fault),'-Child',child]
  v=subprocess.run(cmd,input=b'',capture_output=True,timeout=40);raw=v.stdout+v.stderr;(p/(name+'.log')).write_bytes(raw);t=raw.decode(errors='replace')
  assert v.returncode==(2 if fault in [3,9] else 0),(name,t[-1800:])
  row=json.loads(re.search(r'NX_RESULT (.*)',t)[1]);assert row['start']=={0:0,1:5,2:2,3:5,4:50,5:995,6:6,8:1460,9:5}[fault]
  assert row['owned']==({3:6,9:2}.get(fault,0));assert row['creates']==(1 if fault in [0,3] else 0)
  results.append(dict(case=name,exit=v.returncode,**row))
 for strategy in ['legacy','limited']:
  name=f'{cfg}-{strategy}-stdio';cmd=['powershell.exe','-NoProfile','-ExecutionPolicy','Bypass','-File',str(p/'isolated-supervisor.ps1'),'-Configuration',cfg,'-Strategy',strategy,'-Child','stdio-child']
  v=subprocess.run(cmd,input=b'CONFIRM_FIXTURE\nSTOP_FIXTURE\n',capture_output=True,timeout=40);raw=v.stdout+v.stderr;(p/(name+'.log')).write_bytes(raw);t=raw.decode(errors='replace');assert v.returncode==0,(name,t[-1500:]);assert 'FIXTURE_CONFIRM_ACCEPTED_SYNTHETIC' in t and 'FIXTURE_INPUT_STOP_RECEIVED' in t and 'FIXTURE_STDERR_FUNCTIONAL' in t;results.append(dict(case=name,exit=0))
(p/'inheritance-results.json').write_text(json.dumps(results,indent=2),encoding='utf-8');print(len(results),'inheritance/protocol/fault cases PASS')
