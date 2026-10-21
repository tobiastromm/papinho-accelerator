from pathlib import Path
import hashlib,json
r=Path.cwd();d=r/'docs/integration/manual-package-d3-trust-reconciliation-20261020';ops=json.loads((d/'normative-registration-operations.json').read_text());base=json.loads((d/'baseline.json').read_text());auth=json.loads((d/'authority-preimages.json').read_text())
for f,h in {**base,**auth}.items():
 assert hashlib.sha256((r/f).read_bytes()).hexdigest()==h,f
for o in ops:
 assert not (r/o['target']).exists();assert hashlib.sha256((d/'staged'/o['source']).read_bytes()).hexdigest()==o['sha256']
m=json.loads((d/'incorporation-and-preimages.json').read_text());src=(d/'staged/D3-RECONCILED-PROPOSED.md').read_bytes();frozen=(d/'staged/D3-reconciled-clause-bytes.txt').read_bytes();a,b=m['normative_source_clause_interval'];assert src[a:b]==frozen;assert hashlib.sha256(frozen).hexdigest()==m['clause_sha256']
for o in ops:
 p=r/o['target'];p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((d/'staged'/o['source']).read_bytes());assert hashlib.sha256(p.read_bytes()).hexdigest()==o['sha256']
print('Exact D3-only normative acceptance registered and all5 postimages verified. No source/old-authority/proposal changes.')
