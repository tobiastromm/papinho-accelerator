# Same-root issuer checkpoint linkage: completed scoped implementation

Task PACC-PHASE6-ISSUER-SAME-ROOT-CHECKPOINT-LINKAGE-1. Canonical main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Authority: accepted issuer/C2 P3/P6, CA issuer/custody and technical profile, root/C8, Model E and ED08/ED11. No normative decision changed. Backup/fencing, CNG signing, real credentials and admission are outside this slice.

## Result

The existing administrative root now explicitly records a version1 publication checkpoint after actual native issuer commit and CURRENT/A confirmation. R/predecessor/snapshot revision, actual A and independently current P, issuer identity/generation/prefix/high-water, whole ROOT06/issuer digests and the actual preceding CAS input digest are bound in a512byte trailer of the SAME file. Existing complete issuer history retains reservations, issuance/revocation states and request correlations; no duplicate registry or journal. See format-and-safety-review.md for exact offsets and invariants.

The root administrative begin wrapper revalidates CURRENT, P and complete unchanged operational projection. Lower begin requires the real completed native commit packet, exact committed bytes, published slot and immutable issuer snapshot. It cannot run before publication, accept caller epoch, replay completed checkpoint, or stage while another writer is active. Current P is preserved only through the existing issuer-only full projection gate. Checkpoint records facts; it does not create another CURRENT, epoch or grant.

One existing worker/packet is reused, with immutable inputs and no root/Session pointer in the worker. A second native TxF CAS checks file identity, size and every predecessor byte, writes the extension and confirms commit/closure. No additional worker/thread simultaneously exists. Poll is zero-wait, Stop remains independent; committed-before-Stop remains a factual historical record, not new authority. CAS conflict/UNKNOWN latches authority loss, no old-CURRENT fallback/retry. Failed close/uncertainty preserves existing ownership and buffers/credit.

A subsequent issuer update compares the complete linked16896byte predecessor and writes/truncates to16384 inside its native transaction. The old checkpoint therefore cannot be read as a new revision's acknowledgement. After new actual CURRENT/A, the same explicit begin/poll records a new trailer. Historical8192/16384 formats remain intact and are never silently interpreted as having checkpoints. Optional extension is explicitly version/length/hash/zero-reserved validated. No old free bytes are used.

## Recovery boundary

Native data recovery validates the same file/history/identity and extension and can return historical R/A/P through the explicit data query, whose result remains NOT_SUPPORTED for recovery authority. It does not install A/P in live slots, publish CURRENT, restore Sessions, set signing proof bits or reuse serial. Missing/truncated/corrupt/mismatched linkage rejects output. Relative rollback is detected against retained newer issuer history; an intact old whole installation cannot prove itself latest.

AUTHORITATIVE_RECOVERY=DENY and ISSUER_FENCING=NOT_ESTABLISHED. Hashes/local CAS/exclusive read lease are integrity/correlation, not anti-rollback/exclusivity between restored installations. The native signer root port remains insufficiently proven, with no CNG/key/cert operation executed.

## Validation

Final Debug/Release build5: /WX, zero warnings and full canonical normal API3 product linkage. Final2 suites28invocations each,56/56PASS. Initial C4701 is preserved in build1; one separately approved initializer fixed it. Initial round-trip fixture incorrectly used CREATE_NEW init on an existing file and correctly failed before source recovery; separately approved test-only hunk switched to existing recovery_init02. Later exact test supplements cover real predecessor digest, contextual A/P mismatch and commit-before-Stop. One final diagnostic-output hunk clears unavailable output; final builds/tests were rerun afterward. No executor denial occurred.

| Evidence | Factual scope | Limit |
|---|---|---|
| catalog-live | Real native initial transaction and issuer R2/R3/R4 commits, checkpoints at each R, actual A/P and full same-file readback, predecessor/successor mismatch, relative rollback, truncation/corruption/zero-P and contextual A/P rejection; real serial high-water2 record; old pins/reaper/slot reuse and orderly empty runtime teardown | CA/request/REVOKED Session metadata are synthetic, not authenticated Sessions or certificates |
| checkpoint-conflict / unknown | Actual different file byte / exclusive native handle produces CAS_CONFLICT/UNKNOWN; old pins retained, no retry or recovered authority | UNKNOWN is precommit access failure, not simulated success after native CommitTransaction |
| checkpoint-stop / late-stop | Stop before worker completion and factual native committed-before-Stop; zero-wait poll collects disposition, preserves old pins | Not OS crash/forced-kill/close-failure qualification |
| previous23 cases plus checkpoint assessment | Fresh native storage/CAS/root/publication checks and synthetic fault/history cases, production signing gate, historical formats, catalog and layout | No cross-installation fencing, real certificates, network or native authenticated pair qualification |

Checkpoint decoding uses approved SHA256 adapter. Native hash success never grants freshness. Tests with mutated slot A/P are explicitly negative-only fixture injection, restored immediately; no real epoch invented. Unit DER marker is not a certificate. New file paths/build directories are unique; no clean or historical overwrite. SDK normal46/46 and archive6/6historical binaries rehashed. WIP preserved outside exact approved hunks and factual live-document appends; diff --check passes.

## ED11 / portability

Compiled full outbound profile: root176680 (previous174616), fixed1378288, arrays319488, secure25088, scheduler4432, bind144; existing worker stack65536 once and explicit new owner-side scratch8192 once => known managed1801168. Previous1790912, delta10256 = four buffers growing512 (2048), size/mode fields/alignment16, owner scratch8192. Normal configuration owner105632, component-test105656. No new slots, retained history copies or worker stack. Combined pre-attachment1968195; existing4096 attachment credit gives1972291 <2097152 unchanged quota. This residual is NOT approved physical headroom/freshness. Provider/kernel UNKNOWN; ED11 PARTIAL. Existing reservation guards deny overflow.

Common encoding/validation has no CNG/Win32/filesystem types. Windows adapter performs actual file/transaction/handle facts. Box/ISA remain architectural targets NOT_QUALIFIED. Future adapters need their own durable storage/fencing qualification.

CURRENT_TASK_COMPLETED=YES
CHECKPOINT_CURRENT_A_P=IMPLEMENTED_PERSISTED_NATIVE_OFFLINE_VALIDATED
ROOT_HISTORY_BINDING=ROOT_ISSUER_AND_ACTUAL_CAS_PREDECESSOR_DIGEST
SERIAL_HIGH_WATER=PRESERVED_VALIDATED_NO_REUSE_OR_RECOVERY_AUTHORITY
ROLLBACK_DETECTION=RELATIVE_TO_RETAINED_HISTORY
AUTHORITATIVE_RECOVERY=DENY
ISSUER_FENCING=NOT_ESTABLISHED
BUILDS=DEBUG_RELEASE_PASS
WARNINGS=0
OFFLINE_TESTS=56_OF_56_INVOCATIONS_PASS
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
WIP_PRESERVED=YES_OUTSIDE_EXACT_APPROVED_HUNKS
EXECUTOR_APPROVALS=APPROVED_ALL_EXACT_OPERATIONS_NO_DENIAL
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=APPROVED_LATEST_CHECKPOINT_BACKUP_AND_CROSS_INSTALLATION_FENCING_ABSENT
NEXT=WINDOWS_CNG_AND_ADMIN_CLI_IMPLEMENTATION_WITH_RECOVERY_GATE_PRESERVED
