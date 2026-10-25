from pathlib import Path
import subprocess,json,re
p=Path('docs/integration/n0start03-integrated-closeout-20261024/explicit-inheritance-integration');results=[]
for cfg in ['Debug','Release']:
 for probe in ['duplicate','badlease','write-error']:
  v=subprocess.run(['powershell.exe','-NoProfile','-ExecutionPolicy','Bypass','-File',str(p/'isolated-supervisor.ps1'),'-Configuration',cfg,'-Probe',probe,'-Child','delay-child'],input=b'',capture_output=True,timeout=40)
  raw=v.stdout+v.stderr;(p/('final-'+cfg+'-'+probe+'.log')).write_bytes(raw);row=json.loads(re.search(r'NX_RESULT (.*)',raw.decode(errors='replace'))[1]);assert v.returncode==(0 if probe=='duplicate' else 2),(probe,raw[-900:]);assert row['creates']==1
  assert row['first']=={'duplicate':183,'badlease':5023,'write-error':29}[probe]
  assert row['owned']==(0 if probe=='duplicate' else 2)
  results.append(dict(config=cfg,probe=probe,**row))
(p/'final-lease-negative-results.json').write_text(json.dumps(results,indent=2),encoding='utf-8');print('6 lease/duplicate/write-error cases PASS')
