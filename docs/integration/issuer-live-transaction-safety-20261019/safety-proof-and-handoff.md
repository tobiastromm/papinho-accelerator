# Pending issuer commit: safety proof and exact integration handoff

Task PACC-PHASE6-ISSUER-LIVE-TRANSACTION-SAFETY-1. This implements a persistence adapter for the existing root, not live Session/publication integration. The prior draft hunks were not applied. Accepted authority remains DECISION-issuer-c2-epoch-authority-relation-20261019, especially P3/P6.4/P7, C8/ADR0048, Model E, ED08/ED11. No new normative exception is recorded.

## Boundary and ownership

No reusable configuration worker was found in canonical apps/server or src. The old owner-only synchronous commit cannot service Stop while inside persistence. A single bounded native worker is therefore used only for an issuer ROOT06 commit, with no queue or operational authority. Windows scheduling/TxF/handles remain in apps/server/server_configuration_pending_win32.*; src/issuer/issuer_transaction.h contains only private portable outcome names. It creates no root, registry, journal, listener, CNG dependency or public protocol.

The durable configuration owner embeds one packet: immutable previous/next images, scratch comparison bytes and path, plus result/first native error/close error/native handles. There is no pointer from worker to root, Session, grants, policy, epoch or callback. The root/packet outlives the worker. Begin/poll use the existing root lock, but worker never acquires that lock. Inputs are frozen before CreateThread. Output reads occur only after WaitForSingleObject(thread,0) reports signaled, providing completion synchronization. Pending poll never reads partially written results. Begin rejects an active job, retained handle, stopped/unresolved/recovery context, invalid stage and pinned pending slot.

Preparation still uses existing validation/hash/path checks on the owner; this change specifically moves pending native persistence, not all native queries or cryptographic validation. It does not establish universal OS/API latency bounds or qualify active observation. The existing privileged-path-mutation exclusion of the diagnostic storage contract is unchanged.

## Stop and disposal

Stop records a cooperative atomic request and changes the existing stopped state without waiting for worker I/O or completion. The worker checks before persistence, before writing and before commit; it rolls back only its own uncommitted native transaction. A racing Stop can arrive after the final check: commit may still succeed and must be recorded, never treated as cancelled. Poll then installs durable bytes and COMMITTED_PENDING only; stopped blocks publication. Timeout does not imply cancellation. No TerminateThread/process kill, destructive retry or wait-for-worker is introduced.

Abort, synchronous commit, new prepare, pin/CURRENT/publication and finish reject the active packet. Unpin remains available for factual lifetime release; active/uncertain ownership does not refund resources. The launcher already denies finalization while configuration owner remains initialized. Unknown native file/transaction close or commit retains exact handles and buffers. Thread-handle close is attempted at most once; failure latches the handle/active packet and denies reuse, not a retry. Poll may query a pending/signaled job but does not repeat commit. No DESTROYED is inferred.

## Outcomes and authority

| Result | Evidence | Allowed owner disposition |
|---|---|---|
| COMMIT_PENDING | thread not yet signaled | Stop remains available; no publication, retry, abort, reuse or freeing |
| CAS_SUCCESS | full previous-image/file-identity comparison, Write/Flush, CommitTransaction confirmed | install durable image/snapshot; mark committed_pending; no CURRENT/epoch emitted here |
| CAS_CONFLICT | file identity/length/bytes differ; own transaction rolled back and closed | no publication or new epoch; authority_lost denies using old in-memory CURRENT as reconciliation result |
| CAS_UNKNOWN | cannot establish comparison/result, failed commit, rollback or closure | retain available observations and handles; no automatic retry; deny dependent effects |
| COMMIT_FAILED | no operation started, or verified comparison followed by confirmed rollback/closure | no new authority; staged proposal may be explicitly abandoned; no automatic retry |

Commit success and close disposition are separate: confirmed commit plus failed transaction close preserves the durable result but unresolved denies dependent use. First native error and close error remain separate. A failed comparison never selects observed bytes as CURRENT. Actual reconciliation requires approved root continuity/checkpoint/fencing and remains denied here. It is not safe to parse an external file and declare it latest from a hash alone.

Neither conflict nor UNKNOWN rewrites existing C2 epoch, grants, Principal, Session snapshots, pins or dependencies, or automatically Stops independent work. New effects requiring the lost CURRENT proof deny; independent cleanup/cancellation/finalization follows unchanged existing contracts. This does not claim that any operation whose authority depends on the disputed root revision can continue. Retention is not historical authorization.

## Integrating the Session draft next

1. Reconcile every historical hunk against actual source and the accepted I/R/A/P relation. Keep strict operational CURRENT/P checks and immutable admitted snapshots.
2. After factual administrative PREPARE/VALIDATE on the same root, call issuer_begin06 exactly once. Preserve pending revision/policy proof and slot/operation ownership. Do not call old synchronous commit in a live owner.
3. On later owner turns call issuer_poll06 (zero-time wait). Handle Stop independently. Do not schedule any second job while active or use timeout as rollback evidence.
4. Only CAS_SUCCESS with no unresolved/authority loss/stopped state and a newly revalidated identical operational projection can enter the future same-root CURRENT/A publication path. These publication hunks are NOT applied/qualified by this task. Do not substitute A for P.
5. Conflict/UNKNOWN must retain context and report reconciliation pending, not publish old R as the new result or fabricate a replacement P. Active certificate revocation and authoritative recovery remain separate gates.
6. The historical candidate preimages now differ: commit/header/root helpers/launcher/test/CMake changed. Rebuild proposed hunks and hashes, rather than replaying operation files from the prior task.

## Validation matrix

Both Debug/Release compile canonical server and eight scoped targets with /W4 /WX, zero warnings. Per configuration: 12 logged core/storage/layout cases, normal-profile ROOT06 publication guard, and application-root regression = 14 successful invocations. Counts of individual CHECKs vary with polling and are not coverage counts.

- pending-success: native local TxF compare/write/flush/commit, repeated begin/sync-commit/abort/pin/publication denied during active job; Stop after confirmed persistence leaves committed_pending and denies finish/publication.
- pending-stop: native transaction and flush paused by compile-time test-only events before commit; owner Stop returns while worker remains pending; finish denies; release leads to confirmed rollback and factual handle closure. The 1000ms regression assertion is a helper-test responsiveness bound, NOT Mode C freshness/headroom or an OS completion guarantee.
- pending-conflict: real fixture file modified before worker comparison; conflict returned, CURRENT/publication/retry/finish deny. Not a real competing production writer qualification.
- pending-unknown: explicit test-only fault after native Write/Flush, before commit, leaves native file/transaction handles and root retained; not an actual native CommitTransaction failure or OS crash. Child process exit is not reported as Model E destruction.
- native-stop, snapshots, history, codec: existing before-commit Stop, three-slot/pin exhaustion/retention, persisted synthetic issuer states and denial of authoritative recovery.
- issuer core/production gate/catalog: synthetic serial/concurrency/recovery/error tests, test backend rejected by normal library, catalog private codec.
- normal guard and application-root: normal code still denies ROOT06 publication/signing; actual initial root transaction uses existing test material, without network/authentication/CA issuance.
- common outcome header compiles with MSVC C /Za /W4 /WX without Windows/CNG headers. Initial smoke include path was incorrect; portable-check.log retains failure, portable-check2.log is final PASS. Box/ISA NOT_QUALIFIED.

No native OS crash, power loss, failed CloseHandle, real authenticated Session, network, signing or CA key operation was tested. Slow/failing OS storage can keep the packet retained indefinitely; it no longer holds the owner lock while doing that I/O. Product-level live Session integration and its safety tests remain next-task work.

## ED11

Normal compiled configuration owner sizeof=103520; component-test owner=103544 (test hooks only). Relative to immediate prior normal owner 54048, packet addition=49472. One worker stack reserves 65536; at most one thread, one transaction and one file handle, retained if uncertain. No additional heap queue or journal. No per-Session packet copy.

Compiled fixed managed census (including embedded packet once plus one stack)=1695352. Canonical fixture combined reservation=1957667, limit=2097152, difference=139485 (arithmetic residual, NOT approved host headroom). The initial 2023203 fixture count included stack twice; exact second approved hunk corrects launcher_fixed deduplication. Earlier logs remain preserved. Both final root tests confirm 1957667. Opaque thread/TxF/provider/kernel resources remain UNKNOWN and require separate target evidence; no quota or admission exception is granted.