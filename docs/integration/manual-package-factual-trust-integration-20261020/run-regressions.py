from pathlib import Path
import subprocess,json
r=Path.cwd();d=r/"docs/integration/manual-package-factual-trust-integration-20261020"
results={}
for p in ["Selected","Legacy"]:
 for c in ["Debug","Release"]:
  k=p+"-"+c
  with (d/(k+"-build-tests.log")).open("wb") as f:
   result=subprocess.run(["cmd.exe","/d","/c",str(r/"docs/integration/manual-package-trust-authenticated-validation-20261020"/("build-"+k+".cmd"))],stdout=f,stderr=subprocess.STDOUT)
  results[k]={"exit_code":result.returncode}
  (d/"regression-exits.json").write_text(json.dumps(results,indent=2))
  if result.returncode:raise SystemExit(result.returncode)
print("Four existing offline component configurations passed; no source changes")
