from pathlib import Path
import json,hashlib
r=Path.cwd();d=r/"docs/integration/package-delegation-persistence-acceptance-20261020"
ops=json.loads((d/"official-write-operations.json").read_text())
for o in ops:
 assert not (r/o["target"]).exists(),o["target"]
 assert hashlib.sha256((d/"staged"/o["source"]).read_bytes()).hexdigest()==o["sha256"]
for o in ops:(r/o["target"]).write_bytes((d/"staged"/o["source"]).read_bytes())
print("Exact owner-conditional F1-F5/G0-G7 normative registration, frozen annexes and documentary closeout created")
