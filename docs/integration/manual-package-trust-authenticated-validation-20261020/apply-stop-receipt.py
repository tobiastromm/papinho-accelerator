from pathlib import Path
import json,hashlib
r=Path.cwd();d=r/'docs/integration/manual-package-trust-authenticated-validation-20261020';ops=json.loads((d/'stop-receipt-operations.json').read_text())
for o in ops:
 assert hashlib.sha256((r/o['target']).read_bytes()).hexdigest()==o['preimage'];assert hashlib.sha256((d/'staged'/o['source']).read_bytes()).hexdigest()==o['sha256']
for o in ops:
 (r/o['target']).write_bytes((d/'staged'/o['source']).read_bytes())
print('Exact first parser/Stop receipt preservation and combined Stop/retention regression applied')
