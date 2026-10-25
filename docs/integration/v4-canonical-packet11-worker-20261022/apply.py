from pathlib import Path
import sys,json,hashlib
r=Path(r"C:\Projetos\PapinhoAccelerator")
a=json.loads(Path(sys.argv[1]).read_text())
for x in a:
 p=(r/x['path']).resolve(); assert p.is_relative_to(r)
 assert (hashlib.sha256(p.read_bytes()).hexdigest() if p.exists() else None)==x['pre'],x['path']
 assert hashlib.sha256(Path(x['stage']).read_bytes()).hexdigest()==x['post']
for x in a:
 p=r/x['path'];p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(Path(x['stage']).read_bytes())
print('APPLIED_EXACT_FILES',len(a))
