from pathlib import Path
import subprocess,json,sys
r=Path.cwd();cfg=sys.argv[1];base=r/'build/bootstrap-boundary-execution-20261019'/cfg;dest=base/'native-budget-final';dest.mkdir(exist_ok=True)
results=[]
for mode in ['created','stop','pending-stop','namespace','identity','unknown','closefail','reparse']:
 folder=dest/mode;folder.mkdir();parent=folder
 if mode=='reparse':
  alias=dest/'reparse-alias'
  p=subprocess.run(['cmd','/c','mklink','/J',str(alias),str(folder)],capture_output=True,text=True)
  assert p.returncode==0,(p.returncode,p.stdout,p.stderr)
  parent=alias
 p=subprocess.run([str(base/'papacc_bootstrap_namespace_test.exe'),str(folder/'root-absent.bin'),str(parent),mode],capture_output=True,text=True,timeout=25)
 print(mode,p.returncode,p.stdout.strip())
 results.append({'case':mode,'exit':p.returncode,'stdout':p.stdout,'stderr':p.stderr})
 assert not (folder/'root-absent.bin').exists()
(r/'docs/integration/bootstrap-boundary-execution-20261019'/(cfg+'-namespace-results.json')).write_text(json.dumps(results,indent=2),encoding='utf-8')
assert all(x['exit']==0 for x in results)
