from pathlib import Path
import json,hashlib,re,subprocess,csv
R=Path.cwd();P=R/"docs/integration/phase6-virtual-scm-integration-20261029"
def sha(f):return hashlib.sha256(f.read_bytes()).hexdigest()
def save(n,x):(P/n).write_text(json.dumps(x,indent=2)+"\n",encoding="utf-8")
assert subprocess.check_output(["git","branch","--show-current"],text=True).strip()=="main"
assert subprocess.check_output(["git","rev-parse","HEAD"],text=True).strip()=="f341985924d29870d8ff627ed37c8a0874ad6469"
allowed={"apps/server/personal_scq_recipient_win32.inc","tests/scq_pre_readiness_chain_test.c"}
previous=json.loads((P/"SPC-SOURCE-HASHES.json").read_text());sources=[]
for x in previous:
 h=sha(R/x["path"]);assert h==x["sha256"] or x["path"] in allowed,x["path"]
 sources.append(dict(path=x["path"],before_sha256=x["sha256"],sha256=h))
save("ONB-SOURCE-HASHES.json",sources)
assert sha(P/"STARTUP-SOURCE-AND-CLOSED-READINESS.CANDIDATE.md")=="60cdd50be68fa15c1379c31f287edd697140cfc2bf39ff8b319b86f0e5f08fa3"
accepted=R/"docs/adr/DECISION-personal-scq-closed-startup-s0-s7-20261030.md"
assert any(x["sha256"]==sha(accepted) and x["path"]==str(accepted.relative_to(R)).replace(chr(92),"/") for x in json.loads((P/"SPC-ARTIFACT-HASHES.json").read_text()))
for name,h in [("scm-n0-private-stdout-native.c","a2235963f6427cf686b2a4791fa30f71bc8425c9d54039e66b0925c798dd21b8"),("scm-n0-private-stdout-native.h","a60594a081ea2863d409e89d7a95ef504f53dbaef41773b86cf75879182db471")]:assert sha(R/"tools/qualification"/name)==h
assert sha(R/"docs/integration/phase6-persistent-ca-fixture-20261029/ONE-ATTEMPT.receipt.txt")=="7bf799c34a70a5dae672cfeba4d2bb6cae4a1acbb9d6569758e73b7fbe68f667"
with (P/"ONB-ORIGINAL-BINDINGS.csv").open(encoding="utf-8",newline="") as f:bindings=list(csv.DictReader(f))
assert len(bindings)==16
configs=[]
for t in ("Selected","Legacy"):
 for c in ("Debug","Release"):
  tag=f"{t}-{c}";build=(P/f"ONB-{tag}-build.log").read_text(errors="replace")
  assert not re.search(r"warning [A-Z]+[0-9]+|error [A-Z]+[0-9]+|FAILED:",build,re.I),tag
  result=(P/f"ONB-{tag}-ctest.log").read_text();assert "0 tests failed out of 5" in result
  test=(P/f"ONB-{tag}-LastTest.log").read_text();assert "FAIL line=" not in test
  assert "cases=21 checks=916 ready=4 retained=17" in test and "NATIVE_CLOSE_BEFORE_READY" in test
  assert "CANONICAL_CLOSED_EMPTY_OWNER_ACQUIRE_FINISH PASS" in test
  configs.append(dict(configuration=tag,build="PASS",warnings=0,affected_ctests=5,result="PASS",native_bilateral="NOT_DEMONSTRATED"))
mutants=json.loads((P/"ONB-MUTANTS.json").read_text());assert len(mutants)==2 and all(x["rejected"] for x in mutants)
native=(R/"apps/server/server_configuration_personal_native_win32.inc").read_text()
for name in ("personal_scq_registered_owner_decision","personal_scq_registered_endpoint_decision","personal_scq_registered_cng_decision"):
 assert "return NULL" in native[native.index(name+"(void)"):native.index(name+"(void)")+160]
save("ONB-QUALIFICATION.json",dict(task="PACC-PHASE6-SPC-ONE-NATIVE-ORIGINAL-BINDING",configurations=configs,mutants=mutants,binding_rules=16,close_order="ORIGINAL_SQR1_ACK_SQC1_PRODUCED_DELIVERED_VALIDATED_BEFORE_CLOSED_RUNNING",new_chain=dict(cases=21,checks_per_configuration=916,ready_before_final_close_failures=4,retained_host_models=17),layout_growth=0,request="NOT_READY",first_gap="EXACT_ORIGINAL_OPERATION_PLAN_DECISION_AND_JUSTIFIED_POLICY_VALUES",future_pid_token="FRESH_GATES_NOT_PRESESSION_VALUES",native_bilateral="NOT_DEMONSTRATED",native_operations="NONE",authority="DENY",E21="PENDING",ED11="PARTIAL",Phase6="PARTIAL"))
files=[f for f in P.glob("ONB-*") if f.is_file() and f.name!="ONB-ARTIFACT-HASHES.json"]+[P/"NEXT-ONB-EXACT-SESSION-PLAN.md",R/"docs/capabilities/canonical-phase6-consolidation.md",R/"docs/integration/n0start03-integrated-closeout-20261024/ED11_GATE_MATRIX.csv"]
for t in ("Selected","Legacy"):
 for c in ("Debug","Release"):
  b=R/f"build/phase6-functional-20261028/{t}/{c}"
  files.extend(b/n for n in ("papacc_server.exe","papacc_scq_pre_readiness_chain_test.exe","papacc_scq_recipient_close_test.exe","papacc_service_host_product_model.exe","papacc_service_host_lab_model.exe","papacc_service_closed_empty_model.exe"))
save("ONB-ARTIFACT-HASHES.json",[dict(path=str(f.relative_to(R)).replace(chr(92),"/"),sha256=sha(f)) for f in files])
print("ONB VERIFIED:4 builds/20 affected CTests PASS;2 mutants rejected;16 binding rules; actual request NOT_READY")
