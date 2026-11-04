from pathlib import Path
import hashlib,json,re,subprocess,csv,io
P=Path(__file__).resolve().parent
R=P.parents[2]
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def save(name,data):(P/name).write_text(json.dumps(data,indent=2)+"\n",encoding="utf-8")
assert subprocess.check_output(["git","rev-parse","HEAD"],cwd=R,text=True).strip()=="f341985924d29870d8ff627ed37c8a0874ad6469"
old=json.loads((P/"RCE-SOURCE-HASHES.json").read_text(encoding="utf-8"))
allowed={"CMakeLists.txt","apps/server/server_service_win32.c","apps/server/server_service_win32.h","tests/service_host_model_test.c"}
rows=[]
for x in old:
 h=digest(R/x["path"])
 assert h==x["sha256"] or x["path"] in allowed,x["path"]
 rows.append(dict(path=x["path"],before_sha256=x["sha256"],sha256=h))
for name in ("apps/server/server_service_startup_win32.h","apps/server/server_service_startup_win32.inc"):
 rows.append(dict(path=name,before_sha256=None,sha256=digest(R/name)))
save("SSO-SOURCE-HASHES.json",rows)
configs=[]
for t in ("Selected","Legacy"):
 for c in ("Debug","Release"):
  tag=f"{t}-{c}"
  log=(P/f"SSO-{tag}-build.log").read_text(errors="replace")
  assert not re.search(r"warning [A-Z]+[0-9]+|error [A-Z]+[0-9]+|FAILED:",log,re.I),tag
  tests=(P/f"SSO-{tag}-ctest.log").read_text()
  assert "100% tests passed, 0 tests failed out of 2" in tests,tag
  last=(P/f"SSO-{tag}-LastTest.log").read_text()
  assert last.count("startup-source MODEL PASS")==2,tag
  assert "native=12816 host=96 atomic=4" in last
  configs.append(dict(configuration=tag,build="PASS",warnings=0,affected_ctests=2,tests="PASS",native=12816,host=96,host_growth=0))
assert digest(P/"ONA-SCQ-CROSS-PRINCIPAL-DELIVERY.CANDIDATE.md")=="e31b4e5feb78a96baca47494df8d26ffb019d49518c72efd84b08ffc97b559d1"
assert digest(P/"OSR-SCQ-ORIGINAL-HANDOFF.CANDIDATE.md")=="34c68d97250f89634e2fff78ed48b1dbc4a02b4cc6c87bb91bb00a487fb2e561"
receipt=R/"docs/integration/phase6-persistent-ca-fixture-20261029/ONE-ATTEMPT.receipt.txt"
assert digest(receipt)=="7bf799c34a70a5dae672cfeba4d2bb6cae4a1acbb9d6569758e73b7fbe68f667"
m=R/"docs/integration/n0start03-integrated-closeout-20261024/ED11_GATE_MATRIX.csv"
with m.open(encoding="utf-8-sig",newline="") as f:
 reader=csv.DictReader(f);fields=reader.fieldnames;matrix=list(reader)
for row in matrix:
 if row["gate"]=="E17" and "SSO-CONSOLIDATED" not in row["current_evidence"]:
  row["current_evidence"]+="; SSO: negative startup source resolver qualified offline; no positive source/readiness designated; ../phase6-virtual-scm-integration-20261029/SSO-CONSOLIDATED-CLOSEOUT.md"
  row["factual_gap"]+="; original pre-callback startup source and closed readiness contract not designated; candidate PROPOSED"
with m.open("w",encoding="utf-8",newline="") as f:
 writer=csv.DictWriter(f,fieldnames=fields);writer.writeheader();writer.writerows(matrix)
save("SSO-QUALIFICATION.json",dict(scope="OFFLINE_NEGATIVE_SOURCE_RESOLUTION",configurations=configs,positive_startup_source="NOT_IMPLEMENTED_NO_DESIGNATED_CONTRACT",endpoint="NOT_CREATED",operational_effects="NONE",authority="DENY",E21="PENDING",phase6="PARTIAL",candidate_sha256=digest(P/"STARTUP-SOURCE-AND-CLOSED-READINESS.CANDIDATE.md")))
files=list(P.glob("SSO-*"))+[P/"STARTUP-SOURCE-AND-CLOSED-READINESS.CANDIDATE.md",P/"NEXT-SSO-OWNER-SOURCE-DECISION.md",R/"docs/capabilities/canonical-phase6-consolidation.md",m]
for t in ("Selected","Legacy"):
 for c in ("Debug","Release"):
  b=R/f"build/phase6-functional-20261028/{t}/{c}"
  files.extend(b/x for x in ("papacc_server.exe","papacc_service_host_product_model.exe","papacc_service_host_lab_model.exe"))
save("SSO-ARTIFACT-HASHES.json",[dict(path=str(f.relative_to(R)).replace(chr(92),"/"),sha256=digest(f)) for f in files if f.is_file() and f.name!="SSO-ARTIFACT-HASHES.json"])
print("SSO evidence verified; 4 builds / 8 affected CTests PASS; unchanged source pins preserved")
