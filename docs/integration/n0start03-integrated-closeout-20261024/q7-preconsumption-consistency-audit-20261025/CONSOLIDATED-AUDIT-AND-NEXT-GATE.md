# Q7 preventive consistency audit and final decision package

Task PACC-PHASE6-ED11-Q7-PRECONSUMPTION-CONSISTENCY-AND-EXECUTION. Preparation completed; no operational authorization recorded or exercised.

## Result

Twenty preconsumption gates reviewed against actual code, compiled pins, exact artifacts, prior protected placement receipts and necessary read-only SCM/file observations. See PRECONSUMPTION-GATE-MATRIX.csv. SOURCE_IMAGE compiled constant matches frozen image; Q4 SCM predecessor remains the preserved historical03 ImagePath. Future post-rebind candidate is distinct. No SCM or protected-input repair occurred.

A further preventive defect was demonstrated: the MSVC Ninja showIncludes prefix in CMakeLists.txt contained corrupted text. Actual old configuration recorded zero automatic dependencies and failed to rebuild a synthetic consumer after its header changed. The corrected UTF8 prefix, using the existing compiler launcher and toolchain, records the dependency and rebuilds correctly. Explicit critical pin dependencies remain. All four new qualifier configurations now record 36 automatic dependencies, including original protected/context/image-pin headers. See ACTUAL-PREFIX-INCREMENTAL-REGRESSION.json and FINAL-BUILD-AUTODEP-RESULTS.json. Only CMake prefix handling and derived Q4 launcher qualifier path/hash changed in this task; native security logic is unchanged. Preexisting WIP is preserved via preimages; git HEAD remains 913c90cf2328f7939d8f5b927640601903ac6e91.

## Qualification

Selected/Legacy Debug/Release qualifier builds: PASS, zero compiler/linker warnings. Frozen service image was not rebuilt. Four compiled image-pin attestations PASS. Twenty-two static consistency/negative cases PASS; actual gate ordering and owner freshness-before-consume checks PASS. Six existing compiled offline models rerun PASS: policy36, BD94, census441 assertions, A1 89 cases/271453 exploration nodes, self-token217 checks, RAW1 13. Three actual artifact/approval-memory gates and 31 AST-extracted profile/approval gates PASS, without invoking launcher/native admission. Previous 136 real-readback simulated outcomes and prior applicable regression evidence remain reused; no new operational proof claimed.

Disk exhaustion interrupted an unnecessarily broad rebuild of unchanged model targets. Failure logs are preserved and are not reported PASS. Final four builds target only the affected qualifier with unchanged Debug/Release flags. Agent-generated build intermediates were NTFS-compressed without deletion; before/after content hashes for 941 existing intermediates agree. No exe/dll/protected input/receipt was compressed by that preservation procedure. See BUILD-INTERMEDIATES-COMPRESSION-PRESERVATION.json. No quota or toolchain relaxation.

## Observations and proof limits

Read-only SCM snapshots show historical ImagePath, expected account/type/start/SID configuration, STOPPED/PID0 and no configured recovery actions/triggers; qfailureflag returned exit0 and FALSE. These are observations, not future freshness or token proof. Original placement receipt RAW1 and native IDs remain historical evidence; no fresh protected read was substituted by a hash. Identity, operator primary token/no impersonation, boot, original file handles/RAW1, configuration, auxiliary identity, census, exclusivity, current budgets, final60s/1s freshness and confirmations must all be proved by the approved native operation. Unknown costs and retained reservations remain unavailable; NONE_GRANTED credits. Token SID attributes have still not been observed.

Both previous manual approvals are used and not reusable. Last receipt remains SCM_READBACK first13 close0 starts0 stops0 intent_consumed0 effect0 diagnostic_created0; transport locally closed. Historical N0START03 remains consumed/post-Start, Q2 documentary only, global quiescence UNPROVEN; historical IDs never printed remain UNKNOWN. This preparation applies no further disposition and grants no refund or recovery authority.

## Exact final artifacts

- Qualifier Selected Release: 76a3d760f3ee468be41e7e98064edbdcaa3209fac108a76810635a22dafd2e04.
- Launcher: 531a749789892e8356de00870085c2d110b2f80a5caa4a4c9918e2424e7a6bb1.
- Frozen service image: 128834681905d861eb3252c1e0079e1d06a02b46c2f6d34ddb3612bcc85b3dc8.
- Native transport: e66735a59cf1fc3c63776e4adb97f67e1e95cdb7f1963790659b0a19637756b5.
- Framework binding: 513107355c46b8282ac17eb9763a1278bd201c0d4f9f9a925950676fba14d9f4.
- Audited binding: f53b8c22dd023a020e42a9baa7d5c909c13a072fdc003fe720a726be9247abcc.
- Final request Q7-AUDITED-REQUEST.DRAFT.json: **d9279e012bb30db2053098248377591adee3f1c37bfdb8fdf8c917303d95774c**.

Original request 4c97a80a99543737a2b21596c32931c78b5229b9987cc9e1b786c668e35721b2 remains immutable. Final request changes because qualifier and launcher artifacts changed; its bytes require a new explicit owner decision. APPROVAL.INERT-TEMPLATE.json is inert, not an approval. The pinned future approval path remains absent. No executable approved command is supplied before that decision.

## Next real gate

Owner approval of one manual corrective Q7 invocation bound exactly to the final request above, conditional on fresh mandatory gates. After approval, record the exact decision at the designated unused approval path, hash actual record bytes, and fill the final manual command. Do not automatically execute or retry. Candidate procedure is MANUAL-PROCEDURE.NOT-AUTHORIZED.md.

CURRENT_TASK_COMPLETED=YES
PREVENTIVE_AUDIT=QUALIFIED_OFFLINE
OPERATIONAL_APPROVAL=NOT_GRANTED
START_EXECUTED=NO
PREFLIGHT_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
POSITIVE_CAPACITY=NOT_DEMONSTRATED_OPERATIONALLY
WIP_PRESERVED=YES
UNKNOWN=RETAINED
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
FIRST_UNRESOLVED_DIVERGENCE=FRESH_NATIVE_GATES_AND_SERVICE_TOKEN_NOT_OBSERVED
NEXT=OWNER_EXACT_Q7_AUDITED_REQUEST_DECISION
