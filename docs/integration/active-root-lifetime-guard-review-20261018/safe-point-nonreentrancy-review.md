# Safe-point and nonreentrancy review — 2026-10-18
Task PACC-PHASE6-ACTIVE-ROOT-SAFE-POINT-AND-NONREENTRANCY-1.
Read-only source proof. No proposed patch applied, no functional/test instrumentation changed, no build/test/network execution. The two-hunk patch remains SHA256 7481F9393C6D95F820AD5A5453868DF6F897987DEB585A7FAB7F45623A77CA57. Main HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a; existing WIP preserved.

## Finding
A candidate synchronous boundary exists after the COMPLETE successful return from papacc_server_secure_io_win32_poll_once in server_run_root_win32.inc:269–280, before another poll and before cleanup:284. No actual call to target_refresh or target_process_provenance is present there. Thus there is NO implemented active snapshot-completion/export call point in the normal canonical loop. The existing two-hunk patch alone is insufficient and should not be applied as proof of safe integration.

Do not choose the point immediately after dispatch_active becomes false at server_secure_io_win32.c.api3.inc:1831: close reconciliation, drive_pending_protocol, maintenance_budget, C2 progress, stream close, outbound registration, stop_update and retention checking still follow until the poll returns at1866. These operations can mutate the facts being proposed for export. A flag saying nondispatch is not a snapshot boundary.

## Ten questions
1. Ownership: launcher acquire stores GetCurrentThreadId and invocation in the single static root. Secure init stores the owner thread; poll_once and request_stop reject other threads. A synchronous new hook at the complete-poll return would run on that same launcher thread, not a provider worker or console callback. This is source ownership evidence, not executed active proof.
2. Completeness: collect_diagnostic calls collection_begin, reads native metrics, calls reduce, then collection_end before returning (target_evidence_win32.c:87–124). The gate is held until completion bookkeeping. Only successful return with reason OK, unchanged identity/sequence and healthy gate establishes a completed observation; RATE/BUSY/CLOCK/failure is not a new sample. No normal canonical loop call currently produces that active sample. Do not export an idle/prepared sample as active or regard status alone as phase coverage.
3. Callbacks: scheduler dispatch is synchronous within poll, but later poll maintenance may also trigger callbacks before return. The full-return candidate is outside those call stacks. Arbitrary same-thread callbacks calling the public helper are not excluded by its current dispatch flag alone. The helper invokes QPC/QPF, GetProcessTimes, GetModuleFileNameW, GetMappedFileNameW, GetProcessImageFileNameW and lstrcmpiW. It invokes no application callback explicitly, but these OS calls do not provide a documented bounded nonblocking latency guarantee. No universal no-reentrancy proof for the public entry point is claimed.
4. Stop: console handler (server_console_win32.c:10–34) only sets a process-local Interlocked flag. It does not mutate root/secure state. Owner-thread request_stop (secure_io:1869 onward) checks thread and changes resource/Stop state synchronously. A console event may arrive after any pre-check or during observation; it must remain pending and be checked AFTER preparation/copy, before publishing a successful active observation. This is evidence discard on a pending Stop, never authority for new work. Stop cannot be made impossible with a single RUNNING check. The proposed two hunks do not check this console flag or supply a call-site protocol.
5. Storage: static root persists, but FINISHED can be reacquired and memset. A same-thread synchronous hook must require ACTIVE, exact invocation/owner, no dispatch and no Stop/retention, and complete before owner_finish. Proposed by-value read accesses no Session/PST pointers. Caller output and its references remain caller-owned until consumed. No inference of child lifetime from root lifetime is allowed.
6. Partial state: collection gate prevents overlapping native collectors, not arbitrary reentrant root mutation or lifetime transfer. Producer reduce writes the evidence in stages; copying during it is unsafe. Collect-return followed synchronously by copy is the required ordering, not a generic asynchronous reader of s->status. No public safe-point token or completion/copy contract currently binds such a hook. No new lock/thread is justified by this finding.
7. Side effects: proposed export is NOT a pure memcpy: it re-queries process identity, image paths and local time, and uses the existing nonwaiting health CAS. No explicit heap/file I/O/user callback is present, but absence of blocking, internal OS allocation and latency impact is not established. Adding it in a running loop changes elapsed time and requires accounting/qualification; do not claim identical scheduling timing. The current two-hunk proposal cannot satisfy a strict copy-only/no-new-query safe-point requirement.
8. Failure: current helper constructs local f and writes out only on success. A future hook must keep failures separate from the functional poll result and lifecycle: absent/INCOMPLETE observation, no altered grant/credit/Stop return. Invalid identity/integrity remains invalid evidence. Do not retry until PASS, refresh the clock at export, turn failure into cleanup, or overwrite prior phase facts. Current code has no integrated error channel/call site for this hook.
9. Forbidden phases: existing b.stopping and phase guards reject Stop/draining/teardown/retained in provenance helper; proposed patch still rejects those in its restricted active branch. FINISHED and wrong owner must remain denied. The after-poll point must recheck because poll can itself move to Stop/retention. No G/H enablement follows from safe-point reasoning; retained dependencies/credits remain untouched.
10. Authority: native sample binding is historical observation. No live CURRENT query, commit, epoch issuance, grant, ticket/refund or admission follows from copying it. Existing Mode C parameters remain PENDING, provider/kernel UNKNOWN, all gates DENY. Clearing current in the proposal avoids asserting live currency but does not itself prove the phase, identity completeness or scheduling safety.

## Minimal necessary adaptation, NOT implemented or approved for application
Use one explicit caller-owned observation boundary at the complete-poll success return, outside all callback/dispatch/cleanup paths. Establish a local sequence: owner/invocation/Stop checks -> existing bounded collector finishes successfully -> synchronous by-value snapshot copy -> owner/invocation/Stop recheck -> evidence-only publication. Failure discards this attempted observation without changing functional lifecycle. If console Stop arrives during the window, do not publish positive active coverage; return to existing Stop handling. This is not a promise of instantaneous Stop and must not delay its request behind serialization.

Prefer a copy of already captured identity/resources rather than repeating Win32 identity/image queries at export. The existing collector currently does not capture main-module image paths: a genuinely copy-only export would therefore require an explicitly scoped identity-capture/cache-lifetime design in the existing producer, or separate preserved identity references with a proved continuity contract. Do not invent a cache or silently broaden this task to implement one. Caller scratch/storage coexistence must be reconciled under ED11 before application.

A hook in the loop and an exact completion/copy contract are missing from the existing two-hunk patch. Its broad helper accessibility lacks a safe-point-only enforcement mechanism. Therefore neither these adaptations nor the old proposal should be applied solely on the strength of this report. No new lock, thread, supervisor or collector is proposed. A subsequent focused implementation authorization would need precise hunks and offline interception tests, with official executor approval.

## Validation status and required tests
SOURCE_INSPECTION=PERFORMED. BUILDS/OFFLINE_TESTS/NETWORK_TESTS=NOT_RUN. No synthetic scheduling model is presented as native proof. Tests on the unmodified idle helper cannot prove a missing active call site; writing a surrogate implementation solely to produce PASS would duplicate the unapproved change.
Future tests must intercept the real integration boundary and exercise Stop before collection, during native query completion and before publication; nested export/callback attempts; mismatched invocation/thread; failed/partial native measurement; busy/rate/sticky CLOCK gate; untouched prior snapshot on failure; finished/retained denial; unchanged functional result/credits/policy; and exact copy integrity. Instrumentation would require separate official approval. Active network/native workload coverage remains NOT_OBSERVED.

## Evidence identities
server_run_root_win32.inc SHA256 6BD826CDBF96A7921A51BC0EE752D366603935AC613D3764EC4B3ADDB4C68A27.
server_secure_io_win32.c.api3.inc SHA256 63756CAB85C2DD525711B6B1FAB4DBDEBAE58F1BAF6354B2DB8119A1C53A11D3.
server_console_win32.c SHA256 09751563EF0ADBF42EF7ECF059615C4D1F72AB59C78B7BA0ED5F31065967C7CD.
Target producer/root preimages remain those in preimages/ and review-and-hunk-matrix.md. Rejected operation was not reapplied. Model E, ED08/ED11, accepted C8/Mode C semantics and campaign non-executable state remain intact.

CURRENT_TASK_COMPLETED=YES
SAFE_CALL_POINT=CANDIDATE_AFTER_COMPLETE_POLL_RETURN_NOT_WIRED
OWNER_THREAD=CANONICAL_LAUNCHER_OWNER
SNAPSHOT_COMPLETENESS=COLLECTOR_RETURN_CONTRACT_ONLY_ACTIVE_CALL_ABSENT
NONREENTRANCY=NOT_PROVEN_FOR_PUBLIC_EXPORT_ENTRY
STOP_RACE=ATOMIC_REQUEST_IDENTIFIED_PRE_POST_PROTOCOL_MISSING
STORAGE_LIFETIME=SYNCHRONOUS_OWNER_WINDOW_ONLY
EXPORT_SIDE_EFFECTS=EXISTING_NATIVE_QUERIES_NO_BOUNDED_LATENCY_PROOF
PATCH_APPLICATION_AUTHORIZED=NO
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
FIRST_UNRESOLVED_DIVERGENCE=NO_IMPLEMENTED_ACTIVE_COMPLETION_COPY_SAFE_POINT
NEXT=OWNER_DECISION_ON_EXISTING_TWO_HUNK_PATCH
Recommendation: do not apply the existing two-hunk patch unchanged; authorize only a separately reviewed minimal completion/copy-boundary design if desired.