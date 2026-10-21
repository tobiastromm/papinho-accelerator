from pathlib import Path
import json,hashlib
r=Path.cwd();d=r/'docs/integration/automatic-enrollment-listener-pairing-20261020';ops=json.loads((d/'source-operations.json').read_text(encoding='utf-8'))
for o in ops:
 p=r/o['target'];b=(d/'staged'/o['source']).read_bytes()
 assert (not p.exists()) if o['preimage']=='ABSENT' else hashlib.sha256(p.read_bytes()).hexdigest()==o['preimage']
 assert hashlib.sha256(b).hexdigest()==o['after']
for o in ops:(r/o['target']).write_bytes((d/'staged'/o['source']).read_bytes())
print('Exact offline DATA enrollment slice applied; profile is candidate metadata, runtime/root/authority stays denied; no source historical format changed')
