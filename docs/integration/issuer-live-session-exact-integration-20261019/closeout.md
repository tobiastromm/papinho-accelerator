# Issuer live Session exact integration — incomplete safety proof

Task: PACC-PHASE6-ISSUER-LIVE-SESSION-EXACT-INTEGRATION-1.
HEAD: main / c50b8fb9a7e3293b5e6355fc08459bf85b56857a.

## State and preservation

The accepted I/R/A/P relation was reconciled against current source. Candidate function-level changes are preserved in candidate-hunks.json and proposed/. They are DRAFT, NOT APPROVED, NOT APPLIED and NOT READY FOR APPLICATION. The historical eighteen hunks were not replayed. All 17 files captured in baseline.json still match their SHA-256 preimages. No functional source was modified in this task. This is not a new executor rejection.

## Concrete safety gaps before submission

1. The proposed issuer_publish_live calls papacc_configuration_commit on the existing owner thread while holding the same root critical section. preparation_owned in server_launcher_owner_win32.c requires that thread. The existing native transaction in server_configuration_commit_win32.c:150 performs synchronous CreateFileTransactedA, ReadFile, WriteFile, FlushFileBuffers, CommitTransaction, rollback and handle closure. There is no implemented pending-transaction cancellation/handoff boundary in this route. The console handler only records an atomic Stop flag; server_run_root_win32.inc:265 processes it on the owner thread after the call returns. The draft proves serialized data access, but does NOT prove that live publication preserves timely Stop while native persistence is pending. No claim is made that a native call necessarily hangs. Reusing a startup/dormant storage operation is insufficient proof for the live Session case. A pre/post check or a successful synthetic test cannot resolve an in-flight native call.

2. After compare-and-commit failure, the draft wrapper retains the preparation only when c->unresolved is set. The native commit may reject a changed on-disk preimage and successfully roll back its own transaction without setting unresolved. That does not prove the old in-memory revision remains authoritative CURRENT. Accepted relation P6.4 requires positive authority proof rather than fallback to old CURRENT. The candidate needs a distinct conservative authority-loss disposition preserving snapshots/pins and Stop, without fabricating retained native handles. It must not erase serial/history or retry automatically.

3. The new full semantic operational-projection comparison, typed A/P pin read, Session acquisition and final reaper unpin remain uncompiled/unexecuted candidates. They cannot be claimed safe by inspection alone. Defensive manager initialization and complete fault/alias/capacity regressions remain necessary before source submission. Normal prepare still rejects PUBLISHED/REVOKED issuer record states: this candidate does not qualify real credential publication or active certificate revocation.

No new lock/thread/supervisor/registry was introduced to hide these gaps. No general native cancellation or administrative transaction architecture is authorized by this draft. Existing root recovery proofs remain insufficient for checkpoint/high-water/fencing; AUTHORITATIVE_RECOVERY=DENY.

## Minimum continuation

First establish an implementable same-root live-persistence boundary that allows functional Stop independently of a pending native commit and preserves uncertain transaction ownership. Reuse an existing governed mechanism if one is established; do not invent a second owner or change authority from a timeout. Specify failed-CAS/uncertain-commit authority disposition against accepted P6.4. Reconcile these with the draft as one complete operation, add real tests for pin and reaper lifetime and no stale CURRENT, then submit exact source/preimage operations officially. Only after approval apply and run Debug/Release, offline tests and compiled ED11 census. Session admission and transport authentication remain separate native qualification gates.

## Evidence limitations and result

No new builds or tests were run; prior PASS is not attributed to these candidates. ED11 delta is zero in applied functional source; candidate layout, stack and coexistence have not been compiled. Provider/kernel UNKNOWN. Source diff whitespace check passed (Git emitted existing line-ending notices). No network, CNG key operation, certificate, publication, Browser/PST/wire change or Git publication occurred.

CURRENT_TASK_COMPLETED=NO
ISSUER_SESSION_BINDING=DRAFT_NOT_APPLIED
ISSUER_LIVE_PUBLICATION=DENY_PENDING_SAFETY_PROOF
ISSUER_ONLY_EPOCH_PRESERVATION=DRAFT_SEPARATE_A_P_NOT_VALIDATED
CURRENT_AUTHORITY_GUARDS=EXISTING_SOURCE_PRESERVED
OLD_REVISION_RETENTION=EXISTING_WIP_PRESERVED_NEW_BINDING_UNVALIDATED
REAPER_MODEL_E=DRAFT_NOT_APPLIED
AUTHORITATIVE_RECOVERY=DENY
OFFICIAL_EXECUTOR_SOURCE_REVIEW=NOT_SUBMITTED
PATCH_APPLIED=NO
BUILDS=NOT_RUN
WARNINGS=NOT_ASSESSED
OFFLINE_TESTS=NOT_RUN
NETWORK_TESTS=NOT_RUN
CNG_NATIVE_STATUS=UNRESOLVED
ED11_TOTAL_STATUS=PARTIAL
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_17_OF_17_CAPTURED_PREIMAGES_VERIFIED
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=LIVE_NATIVE_COMMIT_STOP_AND_AUTHORITY_FAILURE_DISPOSITION_UNPROVEN
NEXT=COMPLETE_LIVE_TRANSACTION_SAFETY_PROOF_BEFORE_EXACT_SOURCE_SUBMISSION