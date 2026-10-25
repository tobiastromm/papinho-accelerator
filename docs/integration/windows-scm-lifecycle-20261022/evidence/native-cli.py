from pathlib import Path
import subprocess,json,hashlib
r=Path(r'C:\Projetos\PapinhoAccelerator');t=Path(__file__).parent
cases=[(['service','status'],{0,2}),(['service','install','--account','.\\ModelOnlyUnqualified'],{2}),(['service','start'],{2}),(['service','stop'],{2}),(['service','startup','automatic'],{2}),(['service','startup','manual'],{2}),(['service','remove'],{2}),(['service','host'],{2}),(['service','unknown'],{1})]
results=[]
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  exe=r/f'build/windows-scm-lifecycle-20261022/{profile}/{cfg}/papacc_server.exe'
  for args,expected in cases:
   p=subprocess.run([str(exe),*args],capture_output=True,text=True)
   results.append(dict(profile=profile,configuration=cfg,args=args,exit=p.returncode,expected=sorted(expected),stdout=p.stdout,stderr=p.stderr,pass_case=p.returncode in expected))
(t/'native-cli-results.json').write_text(json.dumps(results,indent=2))
assert all(x['pass_case'] for x in results),[x for x in results if not x['pass_case']]
print(json.dumps(dict(cases=len(results),passed=sum(x['pass_case'] for x in results),status_observations=[x for x in results if x['args']==['service','status']]),indent=2))
