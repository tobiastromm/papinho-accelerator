from pathlib import Path
import subprocess,json,hashlib,re,shutil
R=Path.cwd();P=R/"docs/integration/phase6-virtual-scm-integration-20261029"
def sha(f):return hashlib.sha256(f.read_bytes()).hexdigest()
def save(name,v):(P/name).write_text(json.dumps(v,indent=2)+"\n",encoding="utf-8")
assert subprocess.check_output(["git","rev-parse","HEAD"],text=True).strip()=="f341985924d29870d8ff627ed37c8a0874ad6469"
allowed={"apps/server/personal_scq_admin_win32.inc","apps/server/personal_scq_recipient_win32.inc","apps/server/personal_scq_session_win32.inc","apps/server/server_configuration_personal_native_win32.inc","apps/server/server_configuration_commit_win32.h","apps/server/server_service_startup_win32.h","apps/server/server_service_startup_win32.inc","tests/scq_admin_source_test.c","tests/scq_recipient_close_test.c","tests/scq_native_adapter_test.c"}
previous=json.loads((P/"SSO-SOURCE-HASHES.json").read_text());rows=[]
for x in previous:
 h=sha(R/x["path"]);assert h==x["sha256"] or x["path"] in allowed,x["path"]
 rows.append(dict(path=x["path"],before_sha256=x["sha256"],sha256=h))
for n in sorted(allowed-{x["path"] for x in previous}):rows.append(dict(path=n,before_sha256=None,sha256=sha(R/n)))
save("SSB-SOURCE-HASHES.json",rows)
assert sha(P/"STARTUP-SOURCE-AND-CLOSED-READINESS.CANDIDATE.md")=="60cdd50be68fa15c1379c31f287edd697140cfc2bf39ff8b319b86f0e5f08fa3"
for n,h in [("scm-n0-private-stdout-native.c","a2235963f6427cf686b2a4791fa30f71bc8425c9d54039e66b0925c798dd21b8"),("scm-n0-private-stdout-native.h","a60594a081ea2863d409e89d7a95ef504f53dbaef41773b86cf75879182db471")]:assert sha(R/"tools/qualification"/n)==h
assert sha(R/"docs/integration/phase6-persistent-ca-fixture-20261029/ONE-ATTEMPT.receipt.txt")=="7bf799c34a70a5dae672cfeba4d2bb6cae4a1acbb9d6569758e73b7fbe68f667"
pattern=r"^papacc\.(personal\.scq_(admin_source|recipient_close|session_driver|native_adapter)|service\.host_(product|lab)_model)$"
configs=[]
for target in ("Selected","Legacy"):
 for cfg in ("Debug","Release"):
  tag=f"{target}-{cfg}";b=R/f"build/phase6-functional-20261028/{target}/{cfg}"
  build=(P/f"SSB-{tag}-build.log").read_text(errors="replace");assert not re.search(r"warning [A-Z]+[0-9]+|error [A-Z]+[0-9]+|FAILED:",build,re.I),tag
  result=subprocess.run(["ctest","--test-dir",str(b),"-R",pattern,"--output-on-failure"],capture_output=True,text=True)
  (P/f"SSB-{tag}-ctest.log").write_text(result.stdout+result.stderr,encoding="utf-8");assert result.returncode==0,result.stdout+result.stderr
  assert "0 tests failed out of 6" in result.stdout
  shutil.copyfile(b/"Testing/Temporary/LastTest.log",P/f"SSB-{tag}-LastTest.log")
  test=(P/f"SSB-{tag}-LastTest.log").read_text()
  assert "MODEL_RNG NO_REAL_CNG" in test and "REAL_SID_SD_AND_CNG_RANDOM_HELPERS" not in test
  assert "SSO_PRE_RUNTIME_CASES=15 checks=923" in test and "SCQ_RECIPIENT_CLOSE cases=31 checks=757" in test
  result=subprocess.run([str(b/"papacc_canonical_root_layout_test.exe")],capture_output=True,text=True);assert result.returncode==0
  (P/f"SSB-{tag}-layout.log").write_text(result.stdout+result.stderr,encoding="utf-8")
  configs.append(dict(configuration=tag,build="PASS",warnings=0,ctests=6,status="PASS",native_bilateral="NOT_DEMONSTRATED"))
mutants=json.loads((P/"SSB-MUTANTS.json").read_text());assert len(mutants)==3 and all(m["rejected"] for m in mutants)
save("SSB-QUALIFICATION.json",dict(scope="OFFLINE_PARTIAL_COMPONENT_INTEGRATION",configurations=configs,mutants=mutants,normative="ACCEPTED_SSO_B_S0_S7",source="CONTROLLER_LOCAL_ONLY",readiness="RECIPIENT_EVIDENCE_QUERY_ONLY_NOT_PRE_RUNNING",complete_startup_chain="NOT_IMPLEMENTED",original_operational_registrations="EMPTY",native_bilateral="NOT_DEMONSTRATED",preliminary_scope_deviation="INHERITED_CNG_CSPRNG_CALLED_NO_KEY_KSP_OR_ENDPOINT; FIXTURE_CORRECTED_TO_MODEL_AND_FINAL_CAMPAIGN_REPEATED",authority="DENY",E21="PENDING",ED11="PARTIAL",Phase6="PARTIAL"))
files=[f for f in P.glob("SSB-*") if f.is_file() and f.name!="SSB-ARTIFACT-HASHES.json"]+[P/"NEXT-SSB-CLOSED-PREPARATION-AND-ORIGINAL-DELIVERY.md",R/"docs/adr/DECISION-personal-scq-closed-startup-s0-s7-20261030.md",R/"docs/capabilities/canonical-phase6-consolidation.md",R/"docs/integration/n0start03-integrated-closeout-20261024/ED11_GATE_MATRIX.csv"]
for t in ("Selected","Legacy"):
 for c in ("Debug","Release"):
  b=R/f"build/phase6-functional-20261028/{t}/{c}"
  files.extend(b/n for n in ("papacc_server.exe","papacc_scq_admin_source_test.exe","papacc_scq_recipient_close_test.exe","papacc_service_host_product_model.exe","papacc_service_host_lab_model.exe","papacc_scq_session_driver_test.exe","papacc_scq_native_adapter_test.exe"))
save("SSB-ARTIFACT-HASHES.json",[dict(path=str(f.relative_to(R)).replace(chr(92),"/"),sha256=sha(f)) for f in files])
print("SSB: four builds,24 final affected CTests,three killed mutants; full startup chain NOT_IMPLEMENTED; DENY")
