from pathlib import Path
import subprocess,json,sys
r=Path.cwd();cfg=sys.argv[1];b=r/'build/bootstrap-d3-commit-authorization-20261019'/cfg;dest=b/'native-final';dest.mkdir()
results=[]
for case in ['success','stop','unknown','conflict','identity','missing','revoked']:
 p=subprocess.run([str(b/'papacc_bootstrap_d3_commit_test.exe'),str(dest/(case+'-absent.bin')),str(dest),case],capture_output=True,text=True,timeout=25)
 entry={'case':case,'exit':p.returncode,'stdout':p.stdout,'stderr':p.stderr,'file_exists':(dest/case/'root.bin').exists()}
 results.append(entry);print(case,p.returncode,p.stdout.strip())
 assert p.returncode==0,entry
 if case in ['stop','identity','missing','revoked']:assert not entry['file_exists']
 if case=='success':assert entry['file_exists']
 if case=='conflict':assert (dest/case/'root.bin').read_bytes()==b'existing'
(r/'docs/integration/bootstrap-d3-commit-authorization-20261019'/(cfg+'-native-commit-results.json')).write_text(json.dumps(results,indent=2),encoding='utf-8')
