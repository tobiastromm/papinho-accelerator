from pathlib import Path
import hashlib,json
d=Path("docs/integration/manual-package-factual-trust-integration-20261020")
ops=json.loads((d/"documentation-operations.json").read_text())
for o in ops:
 assert not (d/o["target"]).exists()
 assert hashlib.sha256((d/"staged"/o["target"]).read_bytes()).hexdigest()==o["sha256"]
for o in ops:(d/o["target"]).write_bytes((d/"staged"/o["target"]).read_bytes())
print("Four exact PROPOSED/review/closeout documents created; no accepted norms or sources changed")
