from pathlib import Path
import subprocess,json
R=Path.cwd();P=R/"docs/integration/phase6-virtual-scm-integration-20261029"
variants=[
 ("missing_original_close_producer","apps/server/personal_scq_recipient_win32.inc","||c->transport.close_produced!=3U",""),
 ("missing_original_close_delivery","apps/server/personal_scq_recipient_win32.inc","!(c->transport.legs&4U)||","" )]
recipe=P/"ONB-mutant-build.cmd"
# Reuse the exact qualified vcvars invocation; build ONLY the effect-model executable.
base=(P/"ONB-Selected-Debug-build.cmd").read_text()
lines=base.splitlines();lines=[line[:line.index(" --target")]+" --target papacc_scq_pre_readiness_chain_test" if line.startswith("cmake --build") else line for line in lines]
recipe.write_text("\n".join(lines)+"\n",encoding="utf-8")
results=[]
for name,file,before,after in variants:
 f=R/file;original=f.read_bytes();s=original.decode("utf-8").replace("\r\n","\n");assert before in s,name
 try:
  f.write_text(s.replace(before,after,1),encoding="utf-8")
  b=subprocess.run(["cmd","/c",str(recipe)],capture_output=True,text=True)
  assert b.returncode==0,b.stdout+b.stderr
  t=subprocess.run(["ctest","--test-dir","build/phase6-functional-20261028/Selected/Debug","-R",r"^papacc\.personal\.scq_pre_readiness_chain$","--output-on-failure"],capture_output=True,text=True)
  (P/f"ONB-mutant-{name}.log").write_text(b.stdout+b.stderr+t.stdout+t.stderr,encoding="utf-8")
  assert t.returncode!=0,name
  results.append(dict(mutant=name,compile="PASS",rejected=True,scope="MODEL_EXECUTABLE_ONLY"))
 finally:f.write_bytes(original)
b=subprocess.run(["cmd","/c",str(recipe)],capture_output=True,text=True)
(P/"ONB-model-restored-build.log").write_text(b.stdout+b.stderr,encoding="utf-8");assert b.returncode==0
(P/"ONB-MUTANTS.json").write_text(json.dumps(results,indent=2)+"\n",encoding="utf-8")
print("Two mutants compiled and rejected; original bytes restored; model rebuilt")
