# Canonical C2/root transaction wiring — factual lifetime stop

Date: 2026-10-18. Task: PAPINHOACCELERATOR-PHASE6-CANONICAL-C2-ROOT-TRANSACTION-WIRING-1.
HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a, main.
CURRENT_TASK_COMPLETED=NO
SLICE_COMPLETED=NO

## Finding before dependent source changes

The prior participant component exists and its final manifest verifies byte-exactly. Its READY state proves only actual empty manager initialization, copied canonical configuration and component storage accounting. Its root pointer is explicitly opaque correlation, not an application transaction authority. The prior closeout explicitly left root registration, all other participants, commit, CURRENT and epoch pending. Its 43 assertions and four tests per configuration were NOT rerun and are not new evidence for this task.

There is no canonical factual root transaction to wire into as a standalone existing authority. The actual application composition remains papacc_server_run_win32. Its security configuration and composition are automatic objects at apps/server/server_run_win32.c:94 and :98. The current secure loop borrows these at :211. Cleanup contains an early return at :343 when close/shutdown cannot be confirmed; the dependency objects then cease to have automatic lifetime. This is a pre-existing lifetime hazard for retained references, not a newly executed failure or newly introduced defect.

Historical apps/server/server_c2_configuration_win32.c:101 borrows a real published security-source transaction. Its successful apply stores source, secure_lease and source_ticket by reference at :112. It does not copy the transitive runtime/provider/credential graph. Historical server_configuration_root_transaction_win32.inc:5–18 requires the existing durable launcher root, real provider success, security composition READY and listener/capabilities/C2 participants. Its commit at :59–78 additionally validates the complete prepared model and actual source lease. Publication at :80–104 issues an epoch only after normative commit and associates that epoch with the actual configuration owner.

The canonical server_security_source_win32.h only declares the environment resolver. It does not provide the historical transactional security source, borrow tickets or publication lifetime. The canonical configuration has no administrative policy model/transactional CURRENT issuer. A copied committed store alone cannot replace these facts.

ADR-0048 lifecycle section explicitly forbids claiming CONFIG_COMMITTED or CURRENT from a component-store commit, synthetic callback or copied reference. It permits epoch only after valid normative commit and requires durable owner/leases/retention. These obligations also follow ADR-0044 D1/D2 and Model E. There is no newly discovered normative contradiction, executor denial or wire incompatibility. The obstacle is missing canonical application-root lifetime/transaction composition.

## Transition matrix

| Boundary | Current factual status |
|---|---|
| PREPARE -> C2_PARTICIPANT_READY | Component implemented/offline validated previously; not root preparation |
| All application participants -> COMMIT_FACTUAL | Not integrated; durable security/provider and administrative policy transaction missing |
| COMMIT_FACTUAL -> POLICY_CURRENT | No canonical publication authority integrated |
| POLICY_CURRENT -> EPOCH_ISSUED | No factual issuance integrated; remains absent |
| Stop/retained graph | Component conservatively retains; full application dependency lifetime not yet corrected |

Directly attaching historical source/secure leases to the current automatic objects would be unsafe. Marking those participants prepared without actual ownership would fabricate preparation. Publishing the component store as CURRENT would bypass ADR-0048. No such patch was written.

## Minimum implementation prerequisite

Refactor the EXISTING application composition to durable root-owned storage for its transitive security/configuration/provider dependencies and one existing dormant C2 participant. Preserve closed admission and do not add another owner/listener. Bring over only the historical security-source transaction/lease and complete administrative-policy/configuration participants needed by that same root. Retained dependencies must outlive the initiating function and stay charged; cleanup must not discard them on the current early return. This includes the real policy model/source, not boolean readiness flags. Only then wire native atomic persistence and policy-owned postcommit epoch issuance. No empirical C8 harness or operational C2/C3 dispatch is needed for this prerequisite.

This report does not claim that such refactoring requires a new architecture decision. It records required engineering work not implemented in this task. Stop was taken before dependent wiring because the direct existing target graph has unsafe lifetime, as prohibited by the owner's STOP rule. The requested positive commit/failure/publication tests cannot truthfully run against an absent transaction. A new synthetic transaction wrapper was not substituted.

## Preservation / evidence limits

Only this new documentary directory was written after scoped official executor approval. Existing functional sources, CMake, SDKs and historical artifacts were not modified. Previous participant final-manifest entries all match. git diff --check returned0. No builds, tests, binaries, network, listeners or authentication were executed. SDK/six-binary identity was verified in the preceding participant task, not independently rehashed in this task; preservation must not be confused with fresh verification. Two historical map preimages remain unrecovered.

C2_ROOT_WIRING=NOT_IMPLEMENTED_LIFETIME_DEPENDENCY
C2_PARTICIPANT_READY=PREVIOUS_COMPONENT_EVIDENCE_PRESERVED
CONFIG_COMMITTED=NOT_ESTABLISHED
POLICY_CURRENT=NOT_ESTABLISHED
EPOCH_FACTUAL=NOT_ISSUED
ROLLBACK_RETENTION=COMPONENT_ONLY_ROOT_GRAPH_PENDING
CLIENT_ADMISSION=DISABLED
ED11_ADMISSION=DENY
ED11_ADDITIONAL_STORAGE_THIS_TASK=0
ED11_ROOT_AGGREGATE=NOT_COMPILED
PROVIDER_KERNEL=UNKNOWN
BUILDS=NOT_RUN
WARNINGS=NOT_ASSESSED_NO_BUILD
OFFLINE_TESTS=NOT_RUN
NETWORK_TESTS=NOT_RUN
C2_OPERATIONAL_DISPATCH=NOT_INTEGRATED
C3_SITE_TLS=NOT_INTEGRATED
C8_INTEGRATED_QUALIFIED=NO
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_NO_EXISTING_FILES_EDITED
EXECUTOR_APPROVALS=1_SCOPED_DOCUMENTATION_APPROVED
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=CANONICAL_APPLICATION_ROOT_TRANSITIVE_LIFETIME_NOT_DURABLE
NEXT=DURABLE_EXISTING_APPLICATION_ROOT_AND_REAL_SECURITY_POLICY_PARTICIPANTS_BEFORE_TRANSACTION_WIRING