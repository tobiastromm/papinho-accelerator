from pathlib import Path
import hashlib,json
r=Path.cwd();d=r/'docs/integration/automatic-enrollment-listener-pairing-20261020';o=json.loads((d/'capability-operation.json').read_text());p=r/'docs/capabilities/client-pairing.md';b=(d/'staged/client-pairing.md').read_bytes()
assert hashlib.sha256(p.read_bytes()).hexdigest()==o['preimage']
assert hashlib.sha256(b).hexdigest()==o['after']
p.write_bytes(b)
print('Approved exact capability appendix applied; no functional changes')
