from pathlib import Path
import json,hashlib,subprocess
r=Path.cwd();d=r/'docs/integration/issuer-session-live-publication-20261019';h=json.loads((r/'docs/integration/issuer-live-session-exact-integration-20261019/candidate-hunks.json').read_text());sources={};conflicts=[]
for i,e in enumerate(h):
 p=e['file']
 if p not in sources:
  b=(r/p).read_bytes();(d/'preimages'/p.replace('/','__')).write_bytes(b);sources[p]=b.decode().replace('\r\n','\n')
 before=e['before'];after=e['after'];s=sources[p]
 if before=='EOF':sources[p]=s+after;continue
 if s.count(before)!=1:
  conflicts.append(dict(index=i,file=p,reason=e['reason'],count=s.count(before),before=before[:180]));continue
 sources[p]=s.replace(before,after,1)
for p,s in sources.items():(d/'proposed'/p.replace('/','__')).write_text(s)
(d/'reconciliation-conflicts.json').write_text(json.dumps(conflicts,indent=2));print(json.dumps(conflicts,indent=2))
(d/'baseline.json').write_text(json.dumps(dict(head=subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),wip=subprocess.check_output(['git','status','--porcelain=v1','--untracked-files=all'],text=True).splitlines(),files=[dict(path=p,sha256=hashlib.sha256((r/p).read_bytes()).hexdigest()) for p in sources]),indent=2))
