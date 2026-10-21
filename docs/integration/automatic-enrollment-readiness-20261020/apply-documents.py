from pathlib import Path
import hashlib,json
r=Path.cwd();d=r/'docs/integration/automatic-enrollment-readiness-20261020'
ops=json.loads((d/'document-operations.json').read_text())
for o in ops:
 p=r/o['target'];b=d/'staged'/o['source']
 assert (not p.exists()) if o['before']=='ABSENT' else hashlib.sha256(p.read_bytes()).hexdigest()==o['before']
 assert hashlib.sha256(b.read_bytes()).hexdigest()==o['after']
for o in ops:(r/o['target']).write_bytes((d/'staged'/o['source']).read_bytes())
print('Only PROPOSED pairing contract, readiness/test handoffs, closeout and capability append applied; no functional or accepted source changed')
