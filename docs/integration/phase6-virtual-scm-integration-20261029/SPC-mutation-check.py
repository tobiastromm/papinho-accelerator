from pathlib import Path
import subprocess,json
R=Path.cwd();P=R/"docs/integration/phase6-virtual-scm-integration-20261029"
variants=[
 ("closed_accepts_running_identity","apps/server/personal_runtime_identity_win32.inc","(r->closed_preparation==1U?(DWORD)SERVICE_START_PENDING:(DWORD)SERVICE_RUNNING)","((DWORD)SERVICE_RUNNING)"),
 ("closed_can_report_productive_ready","apps/server/server_service_win32.c"," if(host.closed_mode)return 0; /* closed evidence never becomes productive readiness */", " /* MUTANT removes closed/productive separation */"),
 ("premature_running_report","apps/server/server_service_win32.c","int papacc_server_service_scq_closed_ready(void)\n{", "int papacc_server_service_scq_closed_ready(void)\n{\n (void)ps_running(&host.life);(void)ps_report(SERVICE_RUNNING,ERROR_SUCCESS); /* model-only mutant */"),
 ("closed_can_reach_ksp","apps/server/personal_cng_probe_win32.inc","if(r->closed_preparation||r->profile!=PAPACC_PERSONAL_RUNTIME_VIRTUAL", "if(r->profile!=PAPACC_PERSONAL_RUNTIME_VIRTUAL")]
recipe=P/"SPC-mutant-build.cmd"
# Reuse the exact qualified vcvars invocation; build ONLY the effect-model executable.
base=(P/"SPC-Selected-Debug-build.cmd").read_text()
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
  (P/f"SPC-mutant-{name}.log").write_text(b.stdout+b.stderr+t.stdout+t.stderr,encoding="utf-8")
  assert t.returncode!=0,name
  results.append(dict(mutant=name,compile="PASS",rejected=True,scope="MODEL_EXECUTABLE_ONLY"))
 finally:f.write_bytes(original)
b=subprocess.run(["cmd","/c",str(recipe)],capture_output=True,text=True)
(P/"SPC-model-restored-build.log").write_text(b.stdout+b.stderr,encoding="utf-8");assert b.returncode==0
(P/"SPC-MUTANTS.json").write_text(json.dumps(results,indent=2)+"\n",encoding="utf-8")
print("Four mutants compiled and rejected; original bytes restored; model rebuilt")
