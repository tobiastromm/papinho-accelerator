from pathlib import Path
import hashlib,json
r=Path.cwd();d=r/'docs/integration/manual-package-m1-trust-binding-20261020';ops=json.loads((d/'documentation-operations.json').read_text())
for o in ops:
 assert not (r/o['target']).exists();assert hashlib.sha256((d/'staged'/o['source']).read_bytes()).hexdigest()==o['sha256']
for o in ops:
 (r/o['target']).write_bytes((d/'staged'/o['source']).read_bytes())
print('Three exact proposed/factual documents recorded; no source or accepted normative change')
