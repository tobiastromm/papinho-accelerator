from pathlib import Path
import subprocess,sys
r=Path.cwd();d=r/"docs/integration/package-delegation-persistence-implementation-20261020"
k=sys.argv[1]
with (d/(k+"-build-tests.log")).open("wb") as f:q=subprocess.run(["cmd.exe","/d","/c",str(d/("build-"+k+".cmd"))],stdout=f,stderr=subprocess.STDOUT)
print(k,"exit",q.returncode)
print((d/(k+"-build-tests.log")).read_text(errors="replace")[-2800:])
raise SystemExit(q.returncode)
