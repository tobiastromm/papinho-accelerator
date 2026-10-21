from pathlib import Path
import json,hashlib
r=Path.cwd();d=r/"docs/integration/pdg1-m1-trust-consumption-20261020"
ops=json.loads((d/"source-operations.json").read_text())
for o in ops:
 p=r/o["target"]
 assert (not p.exists()) if o["preimage"]=="ABSENT" else hashlib.sha256(p.read_bytes()).hexdigest()==o["preimage"]
 assert hashlib.sha256((d/"staged"/o["source"]).read_bytes()).hexdigest()==o["sha256"]
for o in ops:(r/o["target"]).write_bytes((d/"staged"/o["source"]).read_bytes())
print("Exact PDG DATA consumption / native fixed-DENY M1 / bounded retention and isolated regressions applied")
