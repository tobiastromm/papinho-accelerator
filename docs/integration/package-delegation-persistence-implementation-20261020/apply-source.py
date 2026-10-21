from pathlib import Path
import hashlib,json
r=Path.cwd();d=r/"docs/integration/package-delegation-persistence-implementation-20261020"
ops=json.loads((d/"source-operations.json").read_text())
for o in ops:
 p=r/o["target"]
 assert (not p.exists()) if o["preimage"]=="ABSENT" else hashlib.sha256(p.read_bytes()).hexdigest()==o["preimage"]
 assert hashlib.sha256((d/"staged"/o["source"]).read_bytes()).hexdigest()==o["sha256"]
for o in ops:(r/o["target"]).write_bytes((d/"staged"/o["source"]).read_bytes())
print("Exact six-file offline codec/shared routines/regression patch applied; worker/current/trust remain denied")
