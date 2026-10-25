# Owner manual read-only success — 2026-10-22

The owner executed the prepared launcher manually on an eligible elevated interactive Windows console. The supplied screen shows visible declaration entry and READ_ONLY_WORKER_VERIFIED_FINAL_DENY; the owner subsequently corrected the outer launcher result to ELEVATED_QUALIFICATION_EXIT=-1073741510. The earlier agent reading of0 was incorrect.

Verified saved receipt: [023060eb825043248664b625f85bec6c.json](manual-results/023060eb825043248664b625f85bec6c.json).
Receipt SHA-256: be68a3660c53fb673dd940f99eb3f9311a8cbe17ebedb089d80c6696277a3dfe.
Current Release and receipt SHA-256 agree: bb618deffcd975060b2f860833f14cbbc50d61e49b595ad1f57181556961925a.

The receipt establishes eligible operator=true, interactive console=true, confirmation_result=READ_ONLY_WORKER_VERIFIED_FINAL_DENY, CLI exit2 and target_child_absent=true. The pinned code emits this success only after exact confirmation, one-shot DATA consumption, successful same-packet worker revalidation, thread quiescence/closure and CLOSED cleanup status. CLI2 is its mandatory final effect denial. Clean outer-launcher completion is NOT established: reported -1073741510 is0xC000013A, STATUS_CONTROL_C_EXIT, verified in the local Windows SDK ntstatus.h. The CLI receipt was saved before the outer interruption; it does not prove normal launcher teardown. Native code/receipt provenance and current path absence were checked; this does not constitute an independent trace of every native call or a lifetime failure-injection proof.

POSITIVE_NATIVE_CEREMONY=PASS_SCOPED_MANUAL_READ_ONLY
PROTECTED_PARENT_HANDOFF=PASS_SCOPED_READ_ONLY
VISIBLE_CONFIRMATION=OBSERVED
QUALIFICATION_LAUNCHER_EXIT=STATUS_CONTROL_C_EXIT_NOT_CLEAN
TARGET_CHILD_CREATION=NOT_RUN
BOOTSTRAP_EXECUTION=NOT_RUN
CA_CUSTODY=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL

This single successful manual path does not qualify all negative/concurrent/failure/suspend/restart paths, actual exclusive creation, CA binding, operational authority or complete resource capacity. Source contracts and production0.6.4 remain unchanged. Prior failed/manual receipts and PREPARED/NOT_QUALIFIED records remain historical; this observation supersedes the missing successful read-only path only. The agent performed read-only verification and documentation, not elevation or bootstrap.

## Subsequent normal F10 completion — 2026-10-22

New distinct owner execution: receipt [48091b1698c841e6a63bb11ee5b0ab30.json](manual-results/48091b1698c841e6a63bb11ee5b0ab30.json), SHA-256 be68a3660c53fb673dd940f99eb3f9311a8cbe17ebedb089d80c6696277a3dfe. Verified eligible operator, interactive console, matching current binary, READ_ONLY_WORKER_VERIFIED_FINAL_DENY, CLI2 and child absent. Owner explicitly reports F10 closure; supplied outer-console screenshot clearly shows ELEVATED_QUALIFICATION_EXIT=0 and returned prompt. This is a separate normal-completion observation, not a reinterpretation of the earlier STATUS_CONTROL_C_EXIT.

POSITIVE_NATIVE_CEREMONY=PASS_SCOPED_MANUAL_READ_ONLY
QUALIFICATION_LAUNCHER_EXIT=0_NORMAL_AFTER_F10
BOOTSTRAP_EXECUTION=NOT_RUN
TARGET_CHILD_CREATION=NOT_RUN
CA_CUSTODY=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL

This establishes the single successful manual read-only path and normal launcher return, not full negative/failure/operational resource qualification. Previous interruption evidence remains preserved. No code, binary or original receipt changed.
