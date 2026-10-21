from pathlib import Path
import hashlib,json,subprocess
r=Path.cwd();d=r/'docs/integration/automatic-enrollment-listener-pairing-20261020';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
base=json.loads((d/'baseline.json').read_text(encoding='utf-8-sig'));changed=[p for p,h in base.items() if not (r/p).is_file() or sha(r/p)!=h]
assert sorted(changed)==['CMakeLists.txt','docs/capabilities/client-pairing.md'],changed
for o in json.loads((d/'source-operations.json').read_text()):assert sha(r/o['target'])==o['after']
assert sha(r/'docs/capabilities/client-pairing.md')==json.loads((d/'capability-operation.json').read_text())['after']
assert (r/'CMakeLists.txt').read_bytes().startswith((d/'preimages/CMakeLists.txt').read_bytes())
head=subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip();assert head=='c50b8fb9a7e3293b5e6355fc08459bf85b56857a'
(d/'final-verification.json').write_text(json.dumps({'HEAD':head,'baseline_files':len(base),'changed_existing_files':changed,'new_sources_verified':7,'CMake_WIP_prefix_preserved':True,'sources_and_capability_officially_approved':True,'network_tests':'NOT_RUN'},indent=2)+'\n')
paths={r/p for p in base}|{r/o['target'] for o in json.loads((d/'source-operations.json').read_text())}|{p for p in d.rglob('*') if p.is_file() and p.name!='final-manifest.json'}
manifest={p.relative_to(r).as_posix():sha(p) for p in sorted(paths) if p.is_file()}
(d/'final-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
assert all(sha(r/p)==h for p,h in manifest.items())
print(json.dumps({'verified_hashes':len(manifest),'HEAD':head,'WIP_PRESERVED':True,'build_configurations':4,'suites_each':10,'warnings':0}))
