# Issuer checkpoint/recovery: independent data validation

Task PACC-PHASE6-ISSUER-AUTHORITATIVE-CHECKPOINT-FENCING-1. Branch main, HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Owner confirms no approved concrete backup/restore/fencing mechanism; independent components authorized, authoritative recovery remains DENY. No accepted ADR modified or new authority created.

## Exact implementation

Four-file operation.json and proposed.patch approved/applied through official executor, pre/postimages verified. src/issuer/issuer_codec.h/.c provides a private version1 IN-MEMORY report over the existing version1 durable issuer image inside same-root ROOT06. No new serialized format/file/journal/registry/worker or root. CMake adds Windows native SHA256 tests; common code remains free of Win32/CNG.

Assessment validates entire image/history and compares immutable root/authority/generation/prefix/policy/key/origin, content revision, high-water and complete old event prefix. Greater/equal counter alone cannot hide replaced history. Explicit states INVALID, INCOMPLETE, ROLLBACK, DIVERGENT, STOPPED and DATA_VALID_RECOVERY_DENIED. Valid data returns DENIED, never OK/proof bits/epoch. Retained baseline is a comparison floor, NOT independently proven latest state. An intact stale backup without a newer floor remains undetectable as stale and cannot authorize recovery.

Caller owns immutable complete input leases, disjoint output and serialized Stop/access. Routine does not allocate, mutate input, transfer ownership, invoke signing or change serial/grants/policy/epoch/admission. No call was inserted in the active owner: native hash latency/Stop safety is not newly qualified. Existing publication/worker/pins/reaper unchanged.

## Evidence

Fresh Debug/Release canonical normal API3 builds /WX pass zero warnings. Each has24 successful invocations: previous23 root/storage/pending/core/production-gate/catalog/layout cases plus checkpoint,48/48total. New checkpoint executable performs41 assertions per configuration, not41 independent scenarios. Tests cover intact/missing floor, short inputs, hash failure/corruption, relative rollback/high-water, immutable identity mismatch, divergent valid branches, uncertain reservation retained and revoked history preserved, Stop and absent proof bits.

New records/authority/DER marker and numeric record epochs are SYNTHETIC, nonauthorizing. SHA256 is native BCrypt; no keys/sign/certificates. Prior root suite freshly rerun proves real local TxF/CAS/runtime/publication facts with synthetic Session metadata/faults, NOT OS crash recovery, authenticated Sessions, concurrent restored installations or native signing. No network or real CA. Source manifest31 entries verified initially;30 unaffected prior entries reverified after approved changes. Initial documentation attempt stopped before writes due reading nonexistent manifest sha256 field instead of after; no real hash divergence or executor denial. Later documentation preparation syntax error also performed no writes.

## ED11

Fresh census both configurations: root174616, fixed1376224, arrays319488, secure25088, scheduler4432, bind144 and existing worker stack65536 => known managed1790912, limit2097152. Prior checked combined reservation1962035 unchanged. No added product slots/worker/storage/runtime call. Report sizeof192; two decoded views plus nested existing validation use stack only when called. Future adapter hookup MUST reserve report/stack/input copies/retention before invocation; no second8192byte pair is implicitly reserved. Test-only three8192byte buffers are not product allocations. Provider/kernel UNKNOWN, ED11 PARTIAL, no approved physical headroom/freshness inferred.

## Unfinished scope

Complete durable checkpoint linkage of R/predecessor/I/CURRENT/A/P/publication acknowledgement is NOT implemented by this data-only report. Existing live CURRENT/A remain in their earlier scope. Historical A/P cannot resurrect Sessions or confer runtime authority. No recovery adapter/signing-root port activation. Full checkpoint layout/update/crash semantics must be reviewed within the same root and historical formats preserved; no free-byte metadata or shadow journal.

Accepted technical profile2.4/3.5-3.6 and issuer/C2 P6.1-P6.4 require independent latest checkpoint and fencing of ALL old/restored instances. Owner confirms no mechanism selected. Local CAS/mutex/file lock/PID/timestamp/random ID/hash/config flag cannot supply it. Keep signing/recovery DENY, no automatic CA rotation, no re-sign/retry or serial reuse.

CURRENT_TASK_COMPLETED=NO
INDEPENDENT_DATA_VALIDATION_SLICE=IMPLEMENTED_OFFLINE_VALIDATED
AUTHORITATIVE_CHECKPOINT=PARTIAL_COMPLETE_DURABLE_ROOT_LINKAGE_PENDING
ISSUER_FENCING=NOT_ESTABLISHED
SERIAL_HIGH_WATER_RECOVERY=RELATIVE_ROLLBACK_DETECTION_NOT_LATEST_AUTHORITY
REVOCATION_RECOVERY=DATA_HISTORY_ONLY
CURRENT_A_RECOVERY=DENY
C2_P_PRESERVATION=EXISTING_LIVE_RULE_REGRESSION_PASS
STOP_RETENTION=EXISTING_PATHS_REGRESSION_PASS
BUILDS=DEBUG_RELEASE_PASS
WARNINGS=0
OFFLINE_TESTS=48_OF_48_INVOCATIONS_PASS
NETWORK_TESTS=NOT_RUN
AUTHORITATIVE_RECOVERY=DENY
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
WIP_PRESERVED=YES_OUTSIDE_EXACT_APPROVED_FILES
FIRST_UNRESOLVED_DIVERGENCE=COMPLETE_ROOT_CHECKPOINT_LINKAGE_AND_APPROVED_LATEST_STATE_FENCING_ABSENT
NEXT=COMPLETE_SAME_ROOT_CHECKPOINT_LINKAGE_AND_SELECT_APPROVED_BACKUP_FENCING
