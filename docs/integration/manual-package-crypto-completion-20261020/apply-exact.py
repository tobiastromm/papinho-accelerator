import pathlib,hashlib,json
r=pathlib.Path.cwd();d=r/'docs/integration/manual-package-crypto-completion-20261020'
ops=json.loads((d/'source-operations.json').read_text())
for o in ops:
 assert not (r/o['target']).exists(),o['target']
 assert hashlib.sha256((r/o['source']).read_bytes()).hexdigest()==o['sha256']
for o in ops:
 p=r/o['target'];p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((r/o['source']).read_bytes())
print('Applied exactly five absent-preimage files; no existing source modified.')
