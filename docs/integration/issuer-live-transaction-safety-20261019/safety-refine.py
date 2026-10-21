from pathlib import Path
import json,hashlib,difflib
r=Path.cwd();d=r/'docs/integration/issuer-live-transaction-safety-20261019';m=json.loads((d/'operation.json').read_text())
def fix(p,a,b):
 f=d/'proposed'/p.replace('/','__');s=f.read_text();assert a in s,(p,a);f.write_bytes(s.replace(a,b,1).encode())
fix('apps/server/server_configuration_pending_win32.inc','int ok=0,conflict=0,rolled=0;','int ok=0,conflict=0,rolled=0,verified=0;')
fix('apps/server/server_configuration_pending_win32.inc',' if(InterlockedCompareExchange(&p->stop,0,0))goto cleanup;\n if(SetFilePointer',' verified=1;\n if(InterlockedCompareExchange(&p->stop,0,0))goto cleanup;\n if(SetFilePointer')
fix('apps/server/server_configuration_pending_win32.inc','p->result=conflict?PAPACC_CAS_CONFLICT:PAPACC_COMMIT_FAILED;','p->result=conflict?PAPACC_CAS_CONFLICT:(verified?PAPACC_COMMIT_FAILED:PAPACC_CAS_UNKNOWN);')
fix('apps/server/server_configuration_root_transaction_win32.inc','if(o->configuration_commit.pending_commit.active||o->configuration_commit.authority_lost||!preparation_owned(o)||!p||p->invocation','if(!preparation_owned(o)||o->configuration_commit.pending_commit.active||o->configuration_commit.authority_lost||!p||p->invocation')
fix('apps/server/server_configuration_commit_win32.c','if(!owner(o)||o->unresolved||!path_ok(path))','if(!owner(o)||o->unresolved||pending_blocks(o)||!path_ok(path))')
for e in m:e['after']=hashlib.sha256((d/'proposed'/e['path'].replace('/','__')).read_bytes()).hexdigest()
(d/'operation.json').write_text(json.dumps(m,indent=2));out=[]
for e in m:
 p=e['path'];out.extend(difflib.unified_diff((r/p).read_text().splitlines(True) if e['before'] else [],(d/'proposed'/p.replace('/','__')).read_text().splitlines(True),fromfile='a/'+p,tofile='b/'+p))
(d/'proposed.patch').write_text(''.join(out))
