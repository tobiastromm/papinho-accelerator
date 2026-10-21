# Active-root lifetime guard review — proposed only, 2026-10-18
TASK=PACC-PHASE6-ACTIVE-ROOT-LIFETIME-GUARD-REVIEW-1
HEAD main c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Working tree has existing tracked and untracked WIP; it was not reset. No source, test, build, network, Browser/PST or accepted ADR change. proposed.patch is NOT APPLIED and is NOT an executor submission for source modification. Approval of artifact creation does not approve its application.

## Rejected operation and difference
The previous rejected operation expanded all durable phases, removed the explicit active-network/dispatch rejection, enlarged the result structure and changed tests. It never ran, so no on-disk rejected patch/preimages were created. Its proposed text and exact denial are preserved in the task tool transcript; the byte-exact current source preimages here correspond to that unchanged baseline. Do not claim the rejected candidate was recovered from disk or approved.
This proposal is different and smaller: two hunks, no struct/layout change, no tests applied. The existing inactive branch and its explicit network/dispatch guard remain verbatim. Only a distinct ACTIVE/RUNNING branch may export a previously captured native process snapshot, without refresh, dynamic graph traversal or live CURRENT assertion. Stopping/retained phases are not enabled. It is not full completion of the active-root campaign task.

## Objects actually touched and lifetime proof boundary
|Object/read|Owner, protection and acquisition|Release/Stop/retention/callback boundary|
|---|---|---|
|root singleton, scalar network/secure flags, preparation and resource scalar fields|Static process-lifetime root in server_launcher_owner_win32.c:12; same pointer, owner thread, nonzero invocation and ACTIVE state required before active reads. acquire CAS and invocation change precede publication to caller|finish accepts only owner thread, tests borrowers/resources and retains on uncertain quiescence; FINISHED permits later reinitialization only after confirmed finish. Proposed branch rejects RETAINED/FINISHED, stopping, dispatch and closed/rolled-back preparation. Not a license to dereference root child pointers|
|root.target_producer and embedded binding/evidence/path output|Embedded by-value storage; producer collects only on binding owner thread. Active proposal copies existing binding after explicit root checks. No sampling, callback registration, wait or allocation in new branch|Native origin/sequence/history overflow/full binding/process/thread/creation/cadence-health and recorded interval checks stay in helper. No new lease is acquired or released. A retained underlying Connection does not authorize reading it; proposal reads none|
|caller output and helper local f/process_path|Caller-owned output must remain live for synchronous call; root alias and helper producer/binding overlap checks preserved; output committed only on success|Caller must not alias any separately protected owner storage or call from callbacks. Existing owner-thread safe-point contract is mandatory; arbitrary same-thread reentrancy is not proved by static storage. This proposal installs no callback call site|
|OS process image and process times|Borrowed own-process pseudo-handle and main-module handle; no owned file/handle or module lease acquired. GetProcessTimes identity must equal native sample; mapped main module/path checked by existing helper|No FreeLibrary/CloseHandle, no ownership transfer. Main executable lives for process lifetime; mapping/path association is not page attestation. No other process consulted|
|collection_bank health|Existing process-static bank; nonwaiting CAS in papacc_c8_collection_health binds invocation/owner and sticky incomplete state|Busy or poisoned health denies. No reset, second bank or admission authority introduced; no lock held across an owner callback|
|listener/config CURRENT pin and pointed arrays, leases, PST objects, Session/op/Connection|NOT traversed by new active branch. Existing inactive observation path remains unchanged and retains its prior contracts|No new read is claimed safe for these objects. The prior root_target_binding -> root_listener_check_mode -> root_cohort_preflight path traverses active graph and configuration_current uses a critical section; it is intentionally not invoked in new active export. f.current is explicitly cleared for the historical snapshot|

Same-thread serialization plus no callbacks invoked by this proposed export protects the restricted synchronous read window; it does not prove every caller is a safe point. Secure scheduler sets dispatch_active around dispatch_ready (server_secure_io_win32.c.api3.inc:1828–1831), but that flag alone is not a universal callback-depth lock. No call site inside dispatch, callbacks, finish, teardown or provider thread is authorized. A future implementation must retain this callable-boundary restriction and prove its actual invocation point before native active coverage. External asynchronous mutation of these root-owned scalar fields is not accepted as safe; any discovered writer outside the owner thread blocks application.

The helper still checks recorded C8 sample interval, not an approved Mode C freshness policy. Missing/stale/regressive data denies; numerical Mode C freshness/headroom stay PENDING. A successful output is observation only, cannot install CURRENT, epoch, grants, tickets, refunds or admission. Historical revision/domain/epoch are sample identities, not current authorization. Session and operation are deliberately absent, not inferred from counts.

## Phase review
|Campaign phase|Classification|Exact boundary|
|---|---|---|
|A baseline|SAFE_TO_OBSERVE|Existing inactive path, live owner, non-dispatch valid native sample; previously native offline observed|
|B preparation|SAFE_TO_OBSERVE for existing prepared process snapshot only|No claim of every internal preparation callback or committed transaction; no newly acquired pin|
|C listener/mTLS|REQUIRES_LIFETIME_PROOF at actual call site|Proposed ACTIVE/RUNNING export is snapshot-only between owner operations; no handshake-internal read, native execution pending|
|D OPEN/BIND|REQUIRES_LIFETIME_PROOF|Same restricted snapshot can accompany existing public receipts; Session/op traversal and causal placement not added|
|E Site TLS|REQUIRES_LIFETIME_PROOF|No child TLS/lease/provider access authorized; snapshot is not profile approval|
|F transfer/peak|REQUIRES_LIFETIME_PROOF|No automatic phase hook or resampling; peak and per-operation load not added/proved by this patch|
|G Stop/draining|NOT_OBSERVABLE_WITH_CURRENT_CONTRACT for expanded provenance export|stopping guard remains; existing resource refresh is a distinct read-only mechanism, not automatically qualified full provenance|
|H teardown/retention|NOT_OBSERVABLE_WITH_CURRENT_CONTRACT for expanded provenance export|RETAINED/FINISHED and b.stopping still deny; no object resurrection; existing copied history may be inspected only under its own contract|

No A-H run occurred. C-F are not native-observed; there are no newly executed synthetic tests either. This proposal is a narrow prerequisite, not campaign readiness. Whole active graph safety and safe hooks remain unproved. When quiescence is uncertain, keep references/credits and refuse extension rather than infer destruction from process exit or FINAL.

## Exact hunks and regressions required before future application
H1 apps/server/server_c8_resource_root_win32.inc / papacc_server_launcher_target_process_provenance (baseline lines84–95): preimage SHA256 96DE6E459DBB70AAF3214D720C088EC101A2439B2F950FD1CC26C8C3C2302ABD. Insert explicit root/owner/dispatch guard and restrictive active snapshot branch BEFORE existing inactive branch. Require ACTIVE owner, RUNNING, no stopping/retained/closed/rollback, valid native producer binding with historical committed identity/epoch and matching local reservation scalars. No child pointers. Return historical current=0 after success. Guard for all other states unchanged. Additional local PAPACC_C8_TARGET_BINDING already exists; no persistent delta. No active refresh, no CURRENT query/lock, no operational validation altered.
H2 apps/server/server_c8_target_evidence_win32.c / papacc_c8_target_process_provenance (baseline lines172–175): preimage SHA256 132D75D7C3F9C00A2DF93AA2D66CE8A4F786E7395A638C862205C6654C1EA1EC. Add only ACTIVE to existing BASELINE/PREPARED phase allow-list. Keep b.stopping, native origin, binding equality, identity, health, clock and expiry guards. Private low-level helper requires its established owner-thread/native-sample preconditions; production caller remains the guarded root wrapper. No universal numeric-range acceptance.

Required offline cases, NOT RUN: inactive native regression; explicit wrong owner/root; finished/retained/stopping/closed/rollback; dispatch-active; phase ACTIVE with valid synthetic graph labels reported SYNTHETIC_TESTED; each invocation/profile/epoch/reservation mismatch; native-origin substitution, process creation/PID/thread divergence; overflow and health busy/clock sticky; absent/regressive/stale timestamp; alias; failure preserves output; success current=0; no ledger/grants/credits/config/policy/epoch mutation; callback invocation refusal at intended call site; no sampling after finish. Resource metric export/full Session/op binding are absent from this patch and must not be claimed covered. Future Debug/Release zero-warning builds and compiled census required under separate application approval.

## Review decision
ACTIVE_ROOT_LIFETIME_PROOF=RESTRICTED_BY_VALUE_SNAPSHOT_ONLY_CALLSITE_PROOF_PENDING
GUARD_PRESERVATION=EXPLICIT_ACTIVE_ALLOWLIST_EXISTING_INACTIVE_AND_STOP_DENIALS_PRESERVED
SAFE_PHASES=A_AND_EXISTING_B_PROCESS_SNAPSHOT
UNPROVEN_PHASES=C_D_E_F; G_H_EXPANDED_EXPORT_DENIED
EXACT_PATCH_PREPARED=YES_TWO_HUNKS_PROPOSED_ONLY
OFFICIAL_EXECUTOR_REVIEW=NOT_SUBMITTED_FOR_SOURCE_PATCH
SOURCE_PATCH_APPLIED=NO
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
BUILDS=NOT_RUN
OFFLINE_TESTS=NOT_RUN
NETWORK_TESTS=NOT_RUN
BROWSER_CHANGED=NO
PST_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
FIRST_UNRESOLVED_DIVERGENCE=ACTUAL_ACTIVE_OWNER_SAFE_POINT_AND_NONREENTRANCY_NOT_PROVEN
NEXT=OWNER_REVIEW_OF_EXACT_LIFETIME_SAFE_PATCH