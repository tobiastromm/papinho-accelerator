from pathlib import Path
import json,hashlib
d=Path("docs/integration/manual-package-f1-f5-acceptance-20261020")
ops=json.loads((d/"documentation-operations.json").read_text())
for o in ops:
 assert not (d/o["target"]).exists()
 assert hashlib.sha256((d/"staged"/o["target"]).read_bytes()).hexdigest()==o["sha256"]
for o in ops:(d/o["target"]).write_bytes((d/"staged"/o["target"]).read_bytes())
print("Exact PROPOSED layout, offsets, review, handoff and closeout created; no ACCEPTED registration")
