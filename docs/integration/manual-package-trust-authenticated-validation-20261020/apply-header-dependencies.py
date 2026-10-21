from pathlib import Path
import json,hashlib
r=Path.cwd();d=r/'docs/integration/manual-package-trust-authenticated-validation-20261020';o=json.loads((d/'header-dependencies-operation.json').read_text());p=r/o['target'];b=(d/'staged'/o['source']).read_bytes();assert hashlib.sha256(p.read_bytes()).hexdigest()==o['preimage'];assert hashlib.sha256(b).hexdigest()==o['sha256'];p.write_bytes(b);print('Exact build dependency correction applied; compiler/toolchain/backend unchanged')
