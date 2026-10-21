from pathlib import Path
import os,sys,subprocess,hashlib,base64,json
root=Path.cwd();cfg=sys.argv[1]
env=os.environ.copy()
env['PAPACC_ALLOWED_CLIENT_CERT_SHA256']=hashlib.sha256(base64.b64decode((root/'tests/fixtures/security/client-a.der.b64').read_text())).hexdigest()
exe=root/'build/config-d3-envelope-implementation-20261019'/cfg/'papacc_canonical_root_transaction_test.exe'
results=[]
for mode in ['snapshot-copy','snapshot-stop','snapshot-retain','snapshot-consume','snapshot-consume-stop','snapshot-consume-retain']:
 r=subprocess.run([str(exe),mode],env=env,cwd=root,capture_output=True,text=True)
 print('CASE='+mode+' EXIT='+str(r.returncode));print(r.stdout);print(r.stderr)
 results.append({'case':mode,'exit':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
(root/'docs/integration/config-d3-envelope-implementation-20261019'/(cfg+'-root-results.json')).write_text(json.dumps(results,indent=2),encoding='utf-8')
sys.exit(0 if all(r['exit']==0 for r in results) else 1)
