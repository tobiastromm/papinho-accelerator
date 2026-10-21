import pathlib,hashlib,json
r=pathlib.Path.cwd();d=r/'docs/integration/manual-package-crypto-completion-20261020';ops=json.loads((d/'differential-operations.json').read_text())
for o in ops:
 assert hashlib.sha256((r/o['target']).read_bytes()).hexdigest()==o['preimage'];assert hashlib.sha256((d/'staged'/o['source']).read_bytes()).hexdigest()==o['postimage']
for o in ops:
 (r/o['target']).write_bytes((d/'staged'/o['source']).read_bytes())
print('Exact provider differential/accounting patch applied')
