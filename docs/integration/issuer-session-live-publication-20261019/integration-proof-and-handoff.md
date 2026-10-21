# Issuer Session/live publication: implemented proof and handoff

Task PACC-PHASE6-ISSUER-SESSION-LIVE-PUBLICATION-COMPLETION-1. HEAD main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Authority: accepted issuer/C2 epoch relation, accepted Session coexistence, C2/C8/ADR0048, ADR0012, issuer/technical-profile decisions, Model E, ED08/ED11. No normative decision was changed. Initial operation.json/function-level-proof.md are preserved as the pre-application review record; final-source.diff/final-manifest.json cover the actual resulting source and supplementary approved hunks.

## Implemented interface and transaction

The SAME root supplies issuer_prepare_live(image, operational_projection, expected_R, expected_high_water), then issuer_publish_live(root, &cas_result). Prepare validates existing participants/source/leases, image history/integrity, expected R/high-water and full semantic operational equality. Image input must remain immutable for the serialized prepare call; no caller pointer survives into asynchronous work. Root-owned staged bytes become the existing worker's private inputs.

First publish call begins the existing worker once and returns COMMIT_PENDING. Subsequent calls poll zero-time; no synchronous commit, second worker or callback is introduced. Poll remains usable after config Stop to collect the factual result. Failure is latched; no automatic commit restart. Only completed CAS_SUCCESS, no Stop/unresolved/authority loss, unchanged expected R/P, CURRENT participants/leases and revalidated full operational projection permit selection. One root lock covers selection and model acknowledgement with no callbacks. Publication attempt is latched on failure, preventing blind retry after a postcommit failure.

A is emitted by the existing same-root postcommit RNG path, selected atomically with CURRENT; I comes from the validated issuer image and has explicit presence, R identifies actual selected root snapshot, P remains the factual current C2 context only for an identical operational projection. Tags are distinct even if initial A/P are numerically equal. No caller selects an epoch. No counter/generation comparison accepts stale P. BODY formats/codec and PACC wire are unchanged; new descriptors are private in-memory state.

CAS_CONFLICT/UNKNOWN retain the operation and deny dependent effects; never adopt observed file bytes or old in-memory R as a replacement CURRENT. Existing P, Principal, grants and admitted snapshots are not rewritten. Stop/cancellation/destruction do not acquire fresh authority. Unknown native resources remain owned by the previously implemented pending adapter. No retry, forced termination, alternate cleanup or credit refund was added.

## Session and reference proof

Actual production adoption still requires the original real full RESULT/negotiated capabilities and authenticated control/Principal. It acquires a pin on actual CURRENT before Session exposure, captures R/I/A/P and the actual incarnation in the SAME root's pre-existing four reference containers, then attaches to the existing manager. Failed acquisition exposes no Session; failed release retains its pin/metadata. No second owner, registry, additional grant or fabricated Principal is introduced.

Session currency first proves exact owned address/incarnation and its own admitted pin, then independently proves actual CURRENT operational P, complete configuration projection, source/leases, policy and original authenticated negotiation checks. A retained historical reference alone is never authority. Genuine P changes deny new effects under old captured P. Existing snapshots and their upper bounds are immutable.

Listener/site paths preserve their existing security/time/profile/Principal/grant/ED11 gates. The existing listener/attachment may retain an old administrative R only with independently proven CURRENT operational projection/P. Wake uses that full operational proof; detach uses held-reference lifetime solely for cleanup. Actual attachment_cleanup still proves Stop, native quiescence, C2 dependency absence and factual closure before freeing anything. Retained/unproven cleanup remains denied.

The reaper retains every original guard and independently checks exact Session, absence of native Session/Connection, operation/write/denial references (even stale EMPTY refs), channels, processors and matching secure candidates/PST/security/adapter dependencies. Only then does it unpin the exact R/domain/A/P/incarnation/ticket and erase manager/Session metadata. Failure preserves references. A retained slot marker is never cleared or made reusable. No CURRENT requirement is substituted for the destruction-only lifetime proof.

## Exact reconciliation and discovered composition fixes

Historical eighteen hunks were not replayed. The reconciled twelve-file operation was officially approved, followed by exact supplementary hunks:

1. C2 root-owner target compiled without outbound TLS while canonical owner/loop used it. TLS changes IoLoop layout, including c2_owner offset. The first closed-runtime test failed before synthetic Session creation. PUBLIC PAPACC_ENABLE_PST_OUTBOUND_TLS on the canonical C2 root-owner target now enforces the SAME ABI in its objects and consumers, including the census. This profile already requires outbound proof; no production API2 target is changed.
2. Old attachment wake/detach required administrative R equality. After two correct issuer-only publications, teardown refused the retained R1 attachment. Wake now requires CURRENT operational projection/P; detach requires the held pin for cleanup and preserves all factual close/retention checks. The final positive case proves both wake at R2 and normal detach after Stop at R3.
3. Test-only additions cover postcommit Stop and lost operational proof. An optional explicit fixture destination prevents collisions with older PID-based historical paths. A collision was refused before any overwrite; final runs all use new files inside the new canonical build directories. No earlier evidence was deleted/replaced. Initial failures/logs remain preserved.

## Evidence matrix

Final Debug/Release: /W4 /WX, zero warnings, fresh task build directories and final build5 logs. Each configuration has 23 successful invocations in *-final2-test-results.json, 46 total. Polling assertion counts vary and are not distinct coverage counts.

| Case | Factual implementation exercised | Limit |
|---|---|---|
| catalog-live | native initial root and two real TxF commits, CURRENT R2/R3, new nonzero distinct A, unchanged actual P; old/new references, frozen R2 issuer bytes, alias/invalid pin rejection; three pinned slots deny; reaper borrower classes block; actual unpin and normal closed-runtime teardown | REVOKED Session metadata and request/CA metadata are synthetic, nonauthorizing; no mTLS/OPEN/BIND traffic |
| catalog-live-conflict | actual fixture byte conflict; CAS_CONFLICT, no CURRENT/A, P/snapshot retained, no retry | no concurrent production writer qualification |
| catalog-live-unknown | actual native file access denied by exclusive test handle, so pre-CAS result UNKNOWN; no authority fallback | not a native failure after successful CommitTransaction |
| catalog-live-stop | Stop after begin; worker finishes/rolls back, no publication | race can validly preserve confirmed committed_pending |
| catalog-live-late-stop | native thread signaled and commit confirmed before Stop; owner collects result without publishing CURRENT/A | not an OS crash test |
| catalog-live-proof-change | synthetic negative-only operational-proof change after begin; actual commit can finish, but CURRENT/A denied and prior P unchanged | not a governed live operational policy transition |
| catalog / catalog-issuer06 / catalog-fail / abort | initial normative transaction, old dormant ROOT06 still denies publication/signing, real CREATE_NEW collision, precommit abort | no authentication/admission |
| pending adapter cases | native TxF success, real mismatch, controlled pause with Stop and simulated uncertain result retained | pause/uncertainty are explicitly compile-time test hooks; no native close-failure qualification |
| storage/history/snapshots/codec/core/production gate/catalog/layout | unchanged serial, crash/recovery simulation, retention, three slots, historical codec and normal-backend guards plus actual compiled sizes | recovery remains nonauthoritative; synthetic records/DER marker are not certificates |

No authenticated live Session or actual C2 data-flow continuity is claimed from REVOKED lifecycle metadata. New Session acquisition ordering and current-P/authentication guards are implemented/compiled and their failure/retention paths remain enforced; native mTLS/operation qualification remains outside this no-network task. Sessions were not granted permissions for the tests. No CA, real certificate or CNG key operation was executed.

## ED11

Same-profile canonical reservation advances from 1957667 to 1957939 bytes: delta272, consisting of three additional per-slot P/I tags (48), six existing pin descriptors growing by32 (192), and bound operation state/alignment (32). Normal configuration owner=103568; component test owner=103592 (test hooks only). No new worker, stack, slot or journal. The existing 65536-byte worker stack is charged once.

Census with the ACTUAL outbound-enabled ABI: root174616, fixed layout1376224, base arrays319488, secure arrays25088, scheduler4432, bind/listener144, plus existing worker stack65536 => known managed1790912. Reduced no-outbound census values1695352/1695400 from earlier reports/runs are NOT evidence for this full product layout; those historical files remain intact. Current root transaction's own previously compiled reservation1957667 used its full outbound-enabled layout, so delta272 remains the correct comparison.

Canonical pre-attachment combined reservation1957939; existing closed attachment wake allowance4096 yields checked reservation1962035, below2097152. Quotas unchanged. Residual arithmetic is not approved host headroom. Process totals, native/provider/kernel, physical capacity, freshness and operational admission evidence remain separate UNKNOWN/PENDING gates. ED11_TOTAL_STATUS=PARTIAL.

## Next integration gates

- Authoritative recovery: checkpoint/latest high-water, CA generation/history consistency and fencing of old instances remain unproved. No image hash, file lease, timestamp or restored runtime epoch is promoted to proof. Keep DENY.
- Windows CNG: integrate only after exact diagnostic key disposition, issuer serial reservation before signing, uncertain-result reconciliation, key correspondence, KSP/protection and backup/recovery gates. No automatic signing/retry or real CA initialization follows this change.
- Core issuer root port still reports insufficient signing/recovery proof bits. Normal PUBLISHED/REVOKED credential activation guard and active-certificate-revocation POLICY_PENDING remain intact. Live administrative metadata CURRENT is not real credential issuance qualification.
- Preserve actor authorization at the future administrative caller; these private owner methods are not a public administration listener, grant or operational admission mechanism.
- Keep CLIENT_ADMISSION=DISABLED, SITE_TLS_ADMISSION=DENY, ED11_ADMISSION=DENY; no network, Browser/PST/wire or production0.6.4 change. PST0.6.5 remains candidate until Phase6 completion. Box/ISA not qualified.