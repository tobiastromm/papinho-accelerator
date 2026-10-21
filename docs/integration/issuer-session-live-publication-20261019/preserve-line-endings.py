from pathlib import Path
p=Path('docs/integration/issuer-session-live-publication-20261019')
for f in (p/'proposed').iterdir():
 old=(p/'preimages'/f.name).read_bytes();s=f.read_text();assert '\r' not in s
 b=s.encode()
 if old.count(b'\r\n')>old.count(b'\n')/2:b=b.replace(b'\n',b'\r\n')
 f.write_bytes(b)
