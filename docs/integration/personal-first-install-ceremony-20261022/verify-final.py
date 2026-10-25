from pathlib import Path
import json,hashlib,subprocess
r=Path(r'C:\Projetos\PapinhoAccelerator');d=r/'docs/integration/personal-first-install-ceremony-20261022'
a=json.loads((d/'operations.json').read_text());b=json.loads((d/'stop-operations.json').read_text());expected={x['path']:x['post'] for x in a+b}
for p,h in expected.items():assert hashlib.sha256((r/p).read_bytes()).hexdigest()==h,p
baseline=(d/'status-before.txt').read_text().splitlines();now=subprocess.check_output(['git','status','--porcelain=v1','-uall'],cwd=r,text=True).splitlines()
def unrelated(lines):
 return set(s for s in lines if not any(p in s for p in expected) and 'personal-first-install-ceremony-20261022' not in s and 'docs/capabilities/canonical-phase6-consolidation.md' not in s)
assert unrelated(baseline)==unrelated(now),sorted(unrelated(baseline)^unrelated(now))
assert hashlib.sha256((r/'docs/adr/DECISION-personal-first-install-authorization-f0-f9-20261022.md').read_bytes()).hexdigest()=='c44b41aca319d06c06d6024c0b6d9a8e6946e5cef947d6c4d2bb1b4f144f497c'
(d/'source-verification.json').write_text(json.dumps(dict(final_hashes=expected,accepted_contract_unchanged=True,unrelated_wip_status_preserved=True),indent=2))
for cfg in ('Debug','Release'):
 text=(d/(cfg+'-final.log')).read_text(errors='replace');assert '100% tests passed' in text
(d/'SHA256SUMS.json').write_text(json.dumps({p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(d.iterdir()) if p.is_file() and p.name!='SHA256SUMS.json'},indent=2))
print('EXACT_FINAL_SOURCE_HASHES_AND_WIP_SCOPE_PASS')
