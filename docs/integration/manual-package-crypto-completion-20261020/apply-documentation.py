import pathlib,hashlib,json
r=pathlib.Path.cwd();d=r/'docs/integration/manual-package-crypto-completion-20261020';ops=json.loads((d/'documentation-operations.json').read_text())
for o in ops:
 p=r/o['target'];assert (hashlib.sha256(p.read_bytes()).hexdigest() if p.exists() else 'ABSENT')==o['preimage'];assert hashlib.sha256((d/'staged'/o['source']).read_bytes()).hexdigest()==o['postimage']
for o in ops:
 p=r/o['target'];p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((d/'staged'/o['source']).read_bytes())
print('Exact closeout, proof, attribution and additive capability update applied')
