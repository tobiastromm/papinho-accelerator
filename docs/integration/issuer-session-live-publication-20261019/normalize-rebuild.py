from pathlib import Path
import json
r=Path.cwd();d=r/'docs/integration/issuer-session-live-publication-20261019';h=json.loads((r/'docs/integration/issuer-live-session-exact-integration-20261019/candidate-hunks.json').read_text());s={}
for i,e in enumerate(h):
 p=e['file']
 if p not in s:s[p]=(r/p).read_text()
 a=e['before'].replace('\r\n','\n');b=e['after'].replace('\r\n','\n')
 if a=='EOF':s[p]+=b;continue
 if i==15:
  start=s[p].index('static int root_snapshot_held(');end=s[p].index('PAPACC_RESULT papacc_server_launcher_configuration_current_pin(',start);s[p]=s[p][:start]+b+s[p][end:];continue
 assert s[p].count(a)==1,(i,p,s[p].count(a));s[p]=s[p].replace(a,b,1)
for p,v in s.items():(d/'proposed'/p.replace('/','__')).write_bytes(v.encode())
# Remove previous extra test/CMake candidates; scripts below rebuild from current.
for p in ['tests/canonical_root_transaction_test.c','CMakeLists.txt']:
 f=d/'proposed'/p.replace('/','__')
 if f.exists():f.unlink()
a=(d/'adapt.py').read_text();a=a.replace("assert s.count(a)==1;save(e['file'],s.replace(a,b,1))","assert s.count(a)==1 or b in s;save(e['file'],s.replace(a,b,1) if a in s else s)")
(d/'adapt-normalized.py').write_bytes(a.encode())
