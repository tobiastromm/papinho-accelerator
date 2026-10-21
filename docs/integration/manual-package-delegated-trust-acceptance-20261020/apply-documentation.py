from pathlib import Path
import hashlib,json
r=Path.cwd();d=r/'docs/integration/manual-package-delegated-trust-acceptance-20261020';ops=json.loads((d/'documentation-operations.json').read_text())
for o in ops:
 assert not (r/o['target']).exists();assert hashlib.sha256((d/'staged'/o['source']).read_bytes()).hexdigest()==o['sha256']
for o in ops:
 (r/o['target']).write_bytes((d/'staged'/o['source']).read_bytes())
print('Exact review, pending-acceptance closeout and unapplied PROPOSED correction recorded. No ACCEPTED record.')
