from pathlib import Path
import subprocess,json,hashlib,base64,os,uuid,concurrent.futures
r=Path.cwd();d=r/'docs/integration/issuer-same-root-checkpoint-linkage-20261019'
spec=json.loads((r/'docs/integration/issuer-authoritative-checkpoint-fencing-20261019/Debug-test-results.json').read_text())
spec += [dict(target='canonical-root',case='catalog-live-checkpoint-'+c) for c in ['conflict','unknown','stop','late-stop']]
env=os.environ.copy();env['PAPACC_ALLOWED_CLIENT_CERT_SHA256']=hashlib.sha256(base64.b64decode((r/'tests/fixtures/security/client-a.der.b64').read_text())).hexdigest()
def suite(cfg):
 b=r/'build/issuer-same-root-checkpoint-linkage-20261019'/cfg;rows=[]
 for item in spec:
  target=item['target'];case=item['case'];args=[];fixture=b/('final-'+uuid.uuid4().hex+'.fixture')
  if target=='canonical-root':exe='papacc_canonical_root_transaction_test';args=[case,str(fixture)]
  elif target=='root-store':exe='papacc_issuer_root_store_test';args=[case,str(fixture)]
  elif target=='normal-guard':exe='papacc_issuer_root_guard_test';args=['--native',str(fixture)]
  else:exe=target
  p=subprocess.run([str(b/(exe+'.exe'))]+args,env=env,capture_output=True,timeout=60)
  log=d/(cfg+'-'+target+'-'+case.lstrip('-')+'-final.log');log.write_bytes(p.stdout+p.stderr)
  rows.append(dict(target=target,case=case,exit=p.returncode,log=log.relative_to(r).as_posix(),fixture=str(fixture)))
  if p.returncode:print(cfg,target,case,'FAILED',p.returncode,p.stdout.decode(errors='replace')[-400:])
 (d/(cfg+'-final-test-results.json')).write_text(json.dumps(rows,indent=2));print(cfg,str(sum(x['exit']==0 for x in rows))+'/'+str(len(rows)))
 return all(x['exit']==0 for x in rows)
with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool:results=list(pool.map(suite,['Debug','Release']))
raise SystemExit(0 if all(results) else 1)
