from pathlib import Path
import json,hashlib,difflib
r=Path.cwd();d=r/'docs/integration/issuer-live-transaction-safety-20261019';m=json.loads((d/'operation.json').read_text());p='apps/server/server_configuration_pending_win32.inc';f=d/'proposed'/p.replace('/','__');s=f.read_text();s=s.replace('/* Only native persistence','static int pending_overlap(const void *a,PAPACC_SIZE na,const void *b,PAPACC_SIZE nb)\n{ULONG_PTR x=(ULONG_PTR)a,y=(ULONG_PTR)b;return na&&nb&&(x<=y?y-x<na:x-y<nb);}\n/* Only native persistence',1).replace('issuer06_overlap(o,sizeof(*o),out,sizeof(*out))','pending_overlap(o,sizeof(*o),out,sizeof(*out))');f.write_bytes(s.encode())
for e in m:e['after']=hashlib.sha256((d/'proposed'/e['path'].replace('/','__')).read_bytes()).hexdigest()
(d/'operation.json').write_text(json.dumps(m,indent=2))
out=[]
for e in m:
 p=e['path'];before=(r/p).read_text().splitlines(True) if e['before'] else [];after=(d/'proposed'/p.replace('/','__')).read_text().splitlines(True);out.extend(difflib.unified_diff(before,after,fromfile='a/'+p,tofile='b/'+p))
(d/'proposed.patch').write_text(''.join(out));print('Patch lines',len(out))
