from pathlib import Path
import json,hashlib
r=Path.cwd();d=r/'docs/integration/enrollment-listener-specialization-20261020';ops=json.loads((d/'document-operations.json').read_text(encoding='utf-8'))
for o in ops:
 p=r/o['target'];q=d/'staged'/o['source']
 assert (not p.exists()) if o['preimage']=='ABSENT' else hashlib.sha256(p.read_bytes()).hexdigest()==o['preimage']
 assert hashlib.sha256(q.read_bytes()).hexdigest()==o['after']
for o in ops:(r/o['target']).write_bytes((d/'staged'/o['source']).read_bytes())
print('Only exact owner-approved listener architecture L0-L5 registered ACCEPTED; new ALPN/framing/limits remain PROPOSED; no functional sources changed')
