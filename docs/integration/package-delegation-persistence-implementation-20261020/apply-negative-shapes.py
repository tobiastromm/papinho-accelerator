from pathlib import Path
import json,hashlib
r=Path.cwd();d=r/"docs/integration/package-delegation-persistence-implementation-20261020"
for o in json.loads((d/"negative-shapes-operations.json").read_text()):
 p=r/o["target"];s=d/"staged"/o["source"]
 assert hashlib.sha256(p.read_bytes()).hexdigest()==o["preimage"]
 assert hashlib.sha256(s.read_bytes()).hexdigest()==o["sha256"]
 p.write_bytes(s.read_bytes())
print("Exact test-only header/CP3 semantic negatives applied")
