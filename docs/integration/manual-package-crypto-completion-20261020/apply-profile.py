import pathlib,hashlib,json
r=pathlib.Path.cwd();d=r/'docs/integration/manual-package-crypto-completion-20261020';ops=json.loads((d/'profile-operations.json').read_text())
for o in ops:
 p=r/o['target'];b=(d/'staged'/p.name).read_bytes();assert hashlib.sha256(p.read_bytes()).hexdigest()==o['preimage'];assert hashlib.sha256(b).hexdigest()==o['postimage']
for o in ops:
 p=r/o['target'];p.write_bytes((d/'staged'/p.name).read_bytes())
print('Exact public-profile and DER checks applied')
