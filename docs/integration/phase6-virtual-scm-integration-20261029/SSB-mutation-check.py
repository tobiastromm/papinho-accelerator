from pathlib import Path
import subprocess,json
R=Path.cwd();P=R/"docs/integration/phase6-virtual-scm-integration-20261029"
variants=[("admin_profile_taken","apps/server/personal_scq_admin_win32.inc","c->plan->phase!=2U||!c->plan->taken||","", "papacc.personal.scq_admin_source"),
("admin_receiver_consumed","apps/server/personal_scq_admin_win32.inc","!c->value.controller_consumed||c->value.consumed||c->value.borrowed||","!c->value.controller_consumed||c->value.borrowed||","papacc.personal.scq_admin_source"),
("readiness_query_only","apps/server/personal_scq_recipient_win32.inc","c->transport.operation||c->transport.query_only||!(c->transport.legs&2U)","c->transport.operation||!(c->transport.legs&2U)","papacc.personal.scq_recipient_close")]
results=[]
for name,file,before,after,test in variants:
 f=R/file;original=f.read_bytes();s=original.decode("utf-8").replace("\r\n","\n");assert before in s
 try:
  f.write_text(s.replace(before,after,1),encoding="utf-8")
  b=subprocess.run(["cmd","/c",str(P/"SSB-Selected-Debug-build.cmd")],capture_output=True,text=True)
  assert b.returncode==0,b.stdout+b.stderr
  t=subprocess.run(["ctest","--test-dir","build/phase6-functional-20261028/Selected/Debug","-R","^"+test.replace(".",r"\.")+"$","--output-on-failure"],capture_output=True,text=True)
  (P/f"SSB-mutant-{name}.log").write_text(b.stdout+b.stderr+t.stdout+t.stderr,encoding="utf-8")
  results.append(dict(mutant=name,compile="PASS",rejected=t.returncode!=0));assert t.returncode!=0,name
 finally:f.write_bytes(original)
b=subprocess.run(["cmd","/c",str(P/"SSB-Selected-Debug-build.cmd")],capture_output=True,text=True)
(P/"SSB-Selected-Debug-build.log").write_text(b.stdout+b.stderr,encoding="utf-8");assert b.returncode==0
(P/"SSB-MUTANTS.json").write_text(json.dumps(results,indent=2)+"\n",encoding="utf-8")
print("Three relevant mutants compiled and rejected; originals restored and rebuilt")
