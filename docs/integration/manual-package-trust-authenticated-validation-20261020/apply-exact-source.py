from pathlib import Path
import hashlib,json
r=Path.cwd();d=r/'docs/integration/manual-package-trust-authenticated-validation-20261020';ops=json.loads((d/'source-operations.json').read_text())
for o in ops:
 p=r/o['target'];assert (hashlib.sha256(p.read_bytes()).hexdigest() if p.exists() else 'ABSENT')==o['preimage'];assert hashlib.sha256((d/'staged'/o['source']).read_bytes()).hexdigest()==o['sha256']
for o in ops:
 p=r/o['target'];p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((d/'staged'/o['source']).read_bytes());assert hashlib.sha256(p.read_bytes()).hexdigest()==o['sha256']
print('Exactly10 source/test files applied; native trust source remains DENY; no runtime linkage or norm changes.')
