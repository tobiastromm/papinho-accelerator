from pathlib import Path
import hashlib,json,re,subprocess
R=Path.cwd();P=R/"docs/integration/phase6-virtual-scm-integration-20261029"
def sha(f):return hashlib.sha256(f.read_bytes()).hexdigest()
def save(n,x):(P/n).write_text(json.dumps(x,indent=2)+"\n",encoding="utf-8")
assert subprocess.check_output(["git","branch","--show-current"],text=True).strip()=="main"
assert subprocess.check_output(["git","rev-parse","HEAD"],text=True).strip()=="f341985924d29870d8ff627ed37c8a0874ad6469"
allowed={"CMakeLists.txt","apps/server/personal_cng_probe_win32.inc","apps/server/personal_runtime_identity_win32.h","apps/server/personal_runtime_identity_win32.inc","apps/server/personal_scq_profile_win32.h","apps/server/personal_scq_profile_win32.inc","apps/server/server_configuration_commit_win32.h","apps/server/server_configuration_personal_native_win32.inc","apps/server/server_service_win32.c","apps/server/personal_scq_session_win32.inc","apps/server/personal_scq_recipient_win32.inc","apps/server/server_service_win32.h","apps/server/server_service_startup_win32.h","tests/personal_runtime_identity_test.c","tests/scq_session_driver_test.c","tests/scq_recipient_close_test.c","tests/service_host_model_test.c"}
old=json.loads((P/"SSB-SOURCE-HASHES.json").read_text());rows=[]
for x in old:
 h=sha(R/x["path"]);assert h==x["sha256"] or x["path"] in allowed,x["path"]
 rows.append(dict(path=x["path"],before_sha256=x["sha256"],sha256=h))
extra={"apps/server/server_launcher_owner_win32.c","apps/server/server_launcher_owner_win32.h","apps/server/main.c.api3.inc","tests/scq_pre_readiness_chain_test.c"}
for n in sorted(extra-{x["path"] for x in old}):rows.append(dict(path=n,before_sha256=None,baseline="NOT_IN_PREVIOUS_MANIFEST",sha256=sha(R/n)))
save("SPC-SOURCE-HASHES.json",rows)
assert sha(P/"STARTUP-SOURCE-AND-CLOSED-READINESS.CANDIDATE.md")=="60cdd50be68fa15c1379c31f287edd697140cfc2bf39ff8b319b86f0e5f08fa3"
accepted=R/"docs/adr/DECISION-personal-scq-closed-startup-s0-s7-20261030.md"
previous=[x for x in json.loads((P/"SSB-ARTIFACT-HASHES.json").read_text()) if x["path"]==str(accepted.relative_to(R)).replace(chr(92),"/")]
assert len(previous)==1 and sha(accepted)==previous[0]["sha256"]
for n,h in [("scm-n0-private-stdout-native.c","a2235963f6427cf686b2a4791fa30f71bc8425c9d54039e66b0925c798dd21b8"),("scm-n0-private-stdout-native.h","a60594a081ea2863d409e89d7a95ef504f53dbaef41773b86cf75879182db471")]:assert sha(R/"tools/qualification"/n)==h
assert sha(R/"docs/integration/phase6-persistent-ca-fixture-20261029/ONE-ATTEMPT.receipt.txt")=="7bf799c34a70a5dae672cfeba4d2bb6cae4a1acbb9d6569758e73b7fbe68f667"
native=(R/"apps/server/server_configuration_personal_native_win32.inc").read_text()
for method in ("personal_scq_registered_owner_decision","personal_scq_registered_endpoint_decision","personal_scq_registered_cng_decision"):
 assert "return NULL" in native[native.index(method+"(void)"):native.index(method+"(void)")+160]
profile=(R/"apps/server/personal_scq_profile_win32.inc").read_text();assert "#define PAPACC_SCQ_RECORD_BYTES" in (R/"apps/server/personal_scq_profile_win32.h").read_text()
assert "return scqp_plan_state(p,SERVICE_RUNNING);" in profile
configs=[]
for target in ("Selected","Legacy"):
 for cfg in ("Debug","Release"):
  tag=f"{target}-{cfg}"
  build=(P/f"SPC-{tag}-build.log").read_text(errors="replace")
  assert not re.search(r"warning [A-Z]+[0-9]+|error [A-Z]+[0-9]+|FAILED:",build,re.I),tag
  result=(P/f"SPC-{tag}-ctest.log").read_text();assert "0 tests failed out of 12" in result,tag
  test=(P/f"SPC-{tag}-LastTest.log").read_text();assert "FAIL line=" not in test
  assert "cases=16 checks=548 ready=4 retained=12" in test
  assert "NO_PREMATURE_OR_PRODUCTIVE_RUNNING" in test and "CANONICAL_CLOSED_EMPTY_OWNER_ACQUIRE_FINISH PASS" in test
  assert "REAL_SID_SD_AND_CNG_RANDOM_HELPERS" not in test and "MODEL_RNG NO_REAL_CNG" in test
  assert "native=12816 host=104 atomic=4" in test
  layout=(P/f"SPC-{tag}-layout.log").read_text();managed=1979776 if target=="Selected" else 1811208;limit=3145728 if target=="Selected" else 2097152
  assert f"KNOWN_MANAGED={managed} LIMIT={limit}" in layout
  configs.append(dict(configuration=tag,build="PASS",warnings=0,affected_ctests=12,status="PASS",c8_managed_bytes=managed,scm_separate_bytes=12924,static_sum_components=managed+12924,limit=limit))
mutants=json.loads((P/"SPC-MUTANTS.json").read_text());assert len(mutants)==4 and all(x["rejected"] for x in mutants)
save("SPC-QUALIFICATION.json",dict(task="PACC-PHASE6-SSO-B-PRE-READINESS-COMPLETION",chain="PASS_SOURCE_LINKED_OFFLINE_MODEL",normative="EXISTING_SSO_B_ACCEPTED_UNCHANGED",configurations=configs,new_chain=dict(cases=16,checks_per_configuration=548,ready_before_later_close_failures=4,retained_host_models=12,pre_pending_required=True,premature_or_productive_running_reports=0),mutants=mutants,real_evidence="SID_SD_HELPERS_THREADS_AND_CANONICAL_EMPTY_METADATA_OWNER_ONLY",native_bilateral="NOT_DEMONSTRATED",operational_request="NOT_READY",operational_registrations="EMPTY",operational_authorization="NONE",E21="PENDING",ED11="PARTIAL",Phase6="PARTIAL",authority="DENY",opaque_native_costs="UNKNOWN",new_persistent_format=False,new_owner=False,new_endpoint=False))
files=[f for f in P.glob("SPC-*") if f.is_file() and f.name!="SPC-ARTIFACT-HASHES.json"]+[P/"NEXT-SPC-ONE-NATIVE-ORIGINAL-BINDING.md",accepted,R/"docs/capabilities/canonical-phase6-consolidation.md",R/"docs/integration/n0start03-integrated-closeout-20261024/ED11_GATE_MATRIX.csv"]
for t in ("Selected","Legacy"):
 for c in ("Debug","Release"):
  b=R/f"build/phase6-functional-20261028/{t}/{c}"
  files.extend(b/n for n in ("papacc_server.exe","papacc_scq_pre_readiness_chain_test.exe","papacc_service_closed_empty_model.exe","papacc_service_host_product_model.exe","papacc_service_host_lab_model.exe","papacc_scq_recipient_close_test.exe","papacc_scq_session_driver_test.exe","papacc_scq_native_adapter_test.exe","papacc_scq_admin_source_test.exe","papacc_scq_private_profile_test.exe","papacc_personal_runtime_identity_test.exe","papacc_personal_cng_probe_test.exe","papacc_personal_scq_dispatch_test.exe"))
save("SPC-ARTIFACT-HASHES.json",[dict(path=str(f.relative_to(R)).replace(chr(92),"/"),sha256=sha(f)) for f in files])
print("SPC VERIFIED: 4 builds zero warnings,48 affected CTests,4 rejected mutants; native authority DENY")
