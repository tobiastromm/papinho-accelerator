from pathlib import Path
import hashlib,json
r=Path.cwd();d=r/'docs/integration/pairing-code-transport-review-20261020'
ops=json.loads((d/'document-operations.json').read_text(encoding='utf-8'))
for o in ops:
 p=r/o['target'];b=d/'staged'/o['source']
 assert (not p.exists()) if o['preimage']=='ABSENT' else hashlib.sha256(p.read_bytes()).hexdigest()==o['preimage']
 assert hashlib.sha256(b.read_bytes()).hexdigest()==o['after']
for o in ops:(r/o['target']).write_bytes((d/'staged'/o['source']).read_bytes())
print('Exact PROPOSED review/complement/closeout and capability append applied; original P0-P9 and accepted decisions unchanged')
