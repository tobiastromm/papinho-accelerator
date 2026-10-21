from pathlib import Path
import hashlib,json
r=Path.cwd();d=r/'docs/integration/pairing-enrollment-transport-acceptance-20261020';ops=json.loads((d/'document-operations.json').read_text(encoding='utf-8'));info=json.loads((d/'source-intervals.json').read_text(encoding='utf-8'))
for k,m in info.items():
 b=(r/m['source']).read_bytes();f=(d/'staged'/(k+'-frozen-clause-bytes.txt')).read_bytes()
 assert hashlib.sha256(b).hexdigest()==m['source_sha256']
 assert b[m['start_inclusive']:m['end_exclusive']]==f
 assert hashlib.sha256(f).hexdigest()==m['clause_sha256']
record=(d/'staged/DECISION-pairing-enrollment-transport-20261020.md').read_bytes()
for k in info:assert (d/'staged'/(k+'-frozen-clause-bytes.txt')).read_bytes() in record
for o in ops:
 p=r/o['target'];b=(d/'staged'/o['source']).read_bytes()
 assert (not p.exists()) if o['preimage']=='ABSENT' else hashlib.sha256(p.read_bytes()).hexdigest()==o['preimage']
 assert hashlib.sha256(b).hexdigest()==o['after']
for o in ops:(r/o['target']).write_bytes((d/'staged'/o['source']).read_bytes())
print('P0-P9 and T0-T7 Revision2 exact normative clauses registered ACCEPTED with explicit precedence; no functional sources or authority activation')
