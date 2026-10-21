# C2 conditional semantics — unresolved exact-wire owner decisions
2026-10-12. Current authoritative review is [final gate matrix](../capabilities/pacc-c2-final-encoding-gate-review-20261012.md) with companion audit. [Recorded owner decision](../adr/DECISION-pacc-c2-conditional-semantic-approval-20261012.md) accepts conditional direction only. Spec0.3, twelve messages, fourteen vectors and original ED matrix remain unchanged.

## Decision ED08 — closure partial order
Public PST shuts down terminally, not application directional half-close; plaintext reads after C_SHUTTING are invalid. COMPLETE does not report record-output byte quiescence on C3.

Recommended next design decision: explicitly define completion of required plaintext BEFORE shutdown; retain operation manager/C3/DATA/CONTROL while eligible PST child release confirms DESTROYED, then bounded factual bridge EOF/drain and prefix ACK; exact Browser BA seal coordination without fabricated native EOF. State whether this can be expressed by constrained phases of existing12 messages or needs a revised proposal. No new message or changed meaning is accepted here. Retained child closes remain non-clean/no fallback.

Accepting this order requires later exact-package proof of final-record preservation, four-fence progress, both closure initiators, EOF-less site, partial/stalled output and Model E retention. Do not accept a WOULD_BLOCK/sleep or forced close as drain evidence. If owner requires PST release only after all four fences, an alternative public-interface producer-quiescence proof must be supplied; do not claim it exists.

## Decision ED11 — enforceable total/target cap policy
Known controlled allocation component329514 bytes for one operation/Session endpoint excludes metadata/PST/kernel/baseline. Full ledger is in review. Proposed metadata envelope16KiB/op and16 callbacks/op remains candidate, not accepted limits.

Owner must define supported target RAM/headroom and whether measured/capped provider/kernel peak plus fail-closed admission is sufficient, or a hard library/kernel bound is mandatory. Current public PST has no allocator budget API. If that required guarantee cannot be achieved with normal package, keep gate open for upstream disposition; do not change PST autonomously. Authorize a separate bounded measurement task only if desired; E1-E12 is still unauthorized. Smaller queues cannot silently replace ADR29 lab configuration.

## Other recommendations
ED01-07/09-10/12-14 have explicit approve-with-proof-gate recommendations in review. ED06 proposes modern server OS CSPRNG, reviewed/pinned portable comparison and target-specific non-elidable wipe; no RNG requirement is introduced on NT4 Browser. TLS_READY must use exact positive public CLIENT facts, never ACK/log/unsupported truthiness. Old peer documentary compatibility remains, runtime verification pending. Legacy production remains denied.

ADR_READY_FOR_ACCEPTANCE=NO
WIRE_READY_FOR_APPROVAL=NO
C2_IMPLEMENTATION_AUTHORIZED=NO
C3_IMPLEMENTATION_AUTHORIZED=NO
E1_E12=NOT_AUTHORIZED_NOT_EXECUTED
NEXT=OWNER_RESOLVE_ED08_CLOSE_PARTIAL_ORDER_AND_ED11_TOTAL_CAP_POLICY_BEFORE_EXACT_WIRE_GATE


## Current disposition 2026-10-12

Historical review/handoff consumed by PapinhoAccelerator/ADR-0034. Current normative contract: docs/protocol/pacc-c2-specification-revision-1.0.md; live ED01-ED14 and independent implementation/target gates: docs/capabilities/pacc-c2-normative-approval-and-gates-20261012.md. Earlier HOLD/approval-required statuses retain their as-of-date historical meaning. No runtime gate becomes PASS, implementation is NOT authorized, and numeric total envelopes remain candidates.
