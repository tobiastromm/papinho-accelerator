from pathlib import Path
import json,hashlib
r=Path(r'C:\Projetos\PapinhoAccelerator');d=r/'docs/integration/personal-first-install-authorization-closure-20261022'
e=json.loads((d/'incorporation.json').read_text());src=Path(e['source']).read_bytes();out=(r/e['target']).read_bytes()
b=src[src.index(b'## F0 '):];found=out[out.index(b'## F0 '):]
assert b==found
assert hashlib.sha256(src).hexdigest()==e['source_sha256']
assert hashlib.sha256(found).hexdigest()==e['body_sha256']
assert hashlib.sha256(out).hexdigest()==e['target_sha256']
(d/'byte-exact-verification.json').write_text(json.dumps(dict(body_byte_exact=True,source_sha256=e['source_sha256'],body_sha256=e['body_sha256'],accepted_sha256=e['target_sha256'],official_executor='exact manifest apply approved and returned APPLIED_EXACT_FILES 1',implementation_authorized=False),indent=2))
print('BYTE_EXACT_AND_SHA256_PASS')
