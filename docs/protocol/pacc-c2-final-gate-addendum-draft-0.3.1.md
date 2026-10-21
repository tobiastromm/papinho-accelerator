# PACC C2 progressive specification — final gate addendum draft0.3.1
2026-10-12. Semantic revision3, encoding candidate1 preserved. PROPOSED supplement, NOT owner-approved encoding or implementation.

[Spec0.3](pacc-c2-protocol-specification-draft-revision-0.3.md), [layouts](pacc-c2-exact-wire-layouts.json), [fourteen vectors](pacc-c2-exact-wire-candidate-vectors.json) and original ED matrix remain byte-exact. [Conditional semantic decision](../adr/DECISION-pacc-c2-conditional-semantic-approval-20261012.md) accepts direction only. [Final review](../capabilities/pacc-c2-final-encoding-gate-review-20261012.md) is the current evidence/recommendation matrix; [owner decisions](../integration/pacc-c2-final-gate-owner-decisions-20261012.md) names open gates. This version adds proposed guards, not new types/fields/enums.

MUST/MUST NOT are obligations only if this supplement and the exact encoding receive explicit approval. No paragraph here upgrades a proposed C2 decision to accepted.

## TLS_READY fact and publication guard
Before Site PST leaves C_ESTABLISHED, owner MUST obtain public CLIENT peer summary with API3 size/version and exact TRUE certificate/chain/name/authentication plus TLS version13. Public summary constants TRUE2, UNSUPPORTED3 and NOT_APPLICABLE4 MUST NOT be treated as C boolean equivalents. Proposed wire version=0x0304; verification-mask bit0=TLS1.3, bit1=chain, bit2=expected DNS name, all three required, other bits0. Authentication is an additional admission predicate, not a fabricated fourth wire bit.

ALPN success contains exact offered/policy-selected bytes; absence requires documented UNAVAILABLE/public FALSE and optional/disabled policy. Unsupported is not empty success. Snapshot generation and current policy MUST match at publication. Complete TLS_READY write and validated peer receipt remain separate authority barriers. The verified fact snapshot is not authority after revoke/release or for a later generation.

## Token lifetime guard
Modern Accelerator generates independent32-byte role tokens via reviewed OS CSPRNG. Browser need not generate these tokens. Comparison utility and non-elidable erase recipe MUST be pinned/reviewed per target before code; no PST-private crypto access. Reject absent entropy and finite-candidate saturation before new effects. Logical invalidation occurs at revoke/consume/expiry; physical erase/free waits for borrowed writer/callback quiescence.

~~~mermaid
stateDiagram-v2
    [*] --> LIVE_UNUSED: Bounded reservation and entropy success
    LIVE_UNUSED --> CLAIMED: Atomic valid role commit
    LIVE_UNUSED --> INVALID_QUIESCING: Expiry or revocation
    CLAIMED --> INVALID_QUIESCING: Claim secret no longer usable
    INVALID_QUIESCING --> WIPED: All borrowers quiescent; target-safe erase
    WIPED --> [*]
~~~

No transition restores a consumed token after ACK loss. Retained ownership keeps resource identity/dependencies, not token admission permission. Secure transport/Principal/Session gates remain additional; Legacy on-path disclosure is unresolved by entropy.

## Closure and budget gates still unresolved
APPLICATION END_PREFIX is not TLS directional half-close and MUST NOT authorize plaintext reads after shutdown begins. Required plaintext consumption/delivery must finish while the public PST read state permits it, or closure must remain non-clean.

ED08 needs an accepted producer-quiescence/last-RECORD-fence partial order and Browser BA seal authority. The proposed release-DESTROYED→factual C3 drain/EOF plan keeps manager/bridge/DATA alive; it is not an approved lifecycle and is not proven to preserve all records. No idle/readiness/sleep substitutes for drain proof; no extra message or phase meaning is silently assigned. Model E retained branches never become clean.

ED11 accounts payload, staging, CONTROL, metadata/callbacks, candidate admission, public-PST/provider allocations, lazy shared dependencies, retained resources, heap overhead and actual kernel state. Known wire-storage subtotal329514 bytes/one-operation Session endpoint does not bound total memory. Numerical normal-package/OS peaks and owner-approved old-target headroom/cap policy remain absent; no claim of NT4 fit.

## Conformance and acceptance progression
Future implementation must keep spec/schema/vector/codecs/tests byte-exact at the same approved revision. Required new proof gates supplement original P/N/R2/CG/EW matrices:
- TLS predicate exact-state truth, collected before CLOSED/shutdown, wrong name/trust/version and optional ALPN absence.
- Full token-copy lifetime and wipe, failed RNG, fixed comparison and finite pre-auth plus post-bind work.
- Both closure initiators, four directional prefixes/ACKs, final plaintext/record drainage, EOF-less site, retained close and late callback.
- Full per-owner allocation/inventory and target memory high-water, retained capacity, actual kernel margins and real NT4.
- Both old/new peer directions, missing grants, exact minimum/maximum schemas, arithmetic and fragmented/coalesced goldens.

All are NOT_EXECUTED here. Owner must resolve ED08/ED11, decide registry/API/package metadata and accept exact hashes. C2/C3 implementation and C7 E1-E12 remain separately unauthorized. Server FINAL never attests Browser-owned native destruction; local finalization remains required.

C2_SEMANTIC_DIRECTION=CONDITIONALLY_APPROVED
C2_WIRE_ENCODING=PROPOSED_PENDING_FINAL_GATE
ADR_READY_FOR_ACCEPTANCE=NO
WIRE_READY_FOR_APPROVAL=NO
