from pathlib import Path
import json,hashlib
r=Path.cwd();d=r/'docs/integration/manual-package-authenticated-integration-20261020';ops=json.loads((d/'documentation-operations.json').read_text())
for o in ops:
 assert not (r/o['target']).exists();assert hashlib.sha256((d/'staged'/o['source']).read_bytes()).hexdigest()==o['sha256']
for o in ops:
 (r/o['target']).write_bytes((d/'staged'/o['source']).read_bytes())
print('Exact gate closeout and focal trust/lifetime/ED11 handoff recorded; no source or normative change')
