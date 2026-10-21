# PACC C2 RFC draft supplement0.3.2 — ED08/ED11 proposed guards
Date2026-10-12. Semantic revision3, encoding candidate1 unchanged; PROPOSED.
Preserve [spec0.3](pacc-c2-protocol-specification-draft-revision-0.3.md), [draft0.3.1](pacc-c2-final-gate-addendum-draft-0.3.1.md), twelve layouts/fourteen vectors and all accepted ADRs. [Conditional decision](../adr/DECISION-pacc-c2-conditional-semantic-approval-20261012.md) approves direction only.

Architectural decision traceability: [separate ED08/ED11 ADR candidate](../adr/ADR-CANDIDATE-pacc-c2-ed08-record-end-and-ed11-admission.md), status proposed.

MUST/MUST NOT/SHOULD/MAY retain specification meanings; new guards become mandatory only after exact owner approval. No code/wire change, numeric ID or enum reassignment.

## Proposed closure phase machine
NORMAL_ACTIVE→APPLICATION_DELIVERY_FINISHED→PUBLIC_RECIPROCAL_COMPLETE→APP_AB_FENCE_PUBLISHED→BROWSER_BA_PRODUCER_FROZEN→BA_PREFIX_ACK_WRITTEN→PST_CHILD_RELEASE→AB_EOF_DRAIN→AB_PREFIX_ACK→OWNER_FINALIZATION.
Any lost/failed/expired/revoked predicate leads to truthful NON_CLEAN or RETAINED, not a skipped success transition.

APPLICATION AB END_PREFIX acquires a proposed additional normal-close eligibility guard: reciprocal COMPLETE and no more admissible plaintext output. Browser full receipt can freeze its local RECORD BA admission while preserving RECORD AB native send. This is cross-role coordination by explicit validated observation and one owner, never guessed cross-socket timestamp order. Existing FENCE still acknowledges only its own prefix.

BA must be native accepted by C3 and ACK-written before PST child release; otherwise no remaining downstream is available. AB seal requires factual producer end and ordered C3 drain after confirmed DESTROYED, with U08 guarantee for provider-held output. Manager, C3 consumer peer, role channels and shared leases remain until their independent finalization guards. Foreign/late identities cannot receive/produce payload. No raw EOF/close_notify is invented.

All four prefix/downstream facts remain separate from TLS completion, Site TCP delivery and destructive close. Remote FINAL reports Accelerator-owned outcome only; Browser local cleanup is still required. Ordinary FENCE kind1 is not an HTTP/TLS half-close and receives no new byte layout.

~~~mermaid
sequenceDiagram
    participant B as Browser owner
    participant A as Accelerator owner
    participant P as PST child
    A->>P: Governed reciprocal shutdown, after required plaintext
    P-->>A: Public COMPLETE
    A-->>B: Proposed post-COMPLETE APP AB fence
    B->>B: Freeze new RECORD BA reads, preserve AB sends
    B->>A: Already admitted BA prefix then BA fence
    A->>A: C3 native BA accepted
    A-->>B: Matching BA ACK (full write)
    A->>P: Remove membership and eligible try_release
    alt OK and DESTROYED
        P-->>A: Child destruction confirmed
        A->>A: Keep manager/peer; factual ordered AB drain to EOF
        A-->>B: Remaining AB records then AB fence
        B->>B: C1 native-send accepted prefix
        B-->>A: Matching AB ACK
        A-->>B: FINAL after all owner guards
        B->>B: Separate own native finalization guard
    else retained
        A->>A: Preserve quota/dependencies; no fallback
    end
~~~

This sequence is candidate semantics, not normal runtime evidence. U08 output-guarantee clarification remains prerequisite; generic peer EOF without provider guarantee cannot prove absent provider-held records. Deadline/backpressure/loss leaves non-clean outcome.

## Proposed resource/admission clauses
Owner MUST maintain complete user/kernel/external ledger per lifetime/Session/process. Retained allocations/handles/leases remain quota occupants. Reserve queue/staging/control/notification/callback/metadata and validated provider/kernel allowance before effects. Known controlled subtotal329514 bytes per one-op Session endpoint MUST NOT be called total memory.

Candidate controlled caps512KiB/op,16KiB/Session, Browser2MiB and Accelerator2MiB/process plus candidate full20/2MiB and32/8MiB user/kernel envelopes require owner/target evidence. Opaque-context/shared allowances are hypotheses, not API caps. Unknown hard-cap category denies enablement where hard bounding is required. No destructive eviction or changing approved64KiB lab defaults silently. Actual NT4 qualification remains separate.

Complete clause/evidence mapping:
| Clause | Decision/evidence | Future test plan |
| --- | --- | --- |
| BA before child release | ED08 close-order1-9 | EP05/EP07/EP08 |
| AB producer/EOF/downstream | U08-1/2 and Model E | EP01/EP02/EP06/EP10 |
| Cancel/late/retained | ED09/10 preserved | EP04/EP09/EP11/EP12 |
| Direction closure/fairness | Four-prefix semantics | EP03/EP13 |
| Total ledger/caps/target | ED11 proposed policy | Target P0-P8/PR/PC, queue sweep |
| Spec/codec/test agreement | ED14 preserved | Original CG/EW plus approved future goldens |

See [close contract](../capabilities/pacc-c2-ed08-record-close-order-proposal-20261012.md), [probe matrix](../capabilities/pacc-c2-ed08-probe-evidence-plan-20261012.md), [ledger](../capabilities/pacc-c2-ed11-resource-ledger-admission-proposal-20261012.md), [target plan](../capabilities/pacc-c2-ed08-ed11-target-probe-plan-20261012.md) and [owner/upstream handoff](../integration/pacc-c2-ed08-ed11-upstream-and-owner-handoff-20261012.md).
No runtime test, C2/C3 implementation or E1-E12 execution is authorized by this supplement. WIRE_READY_FOR_APPROVAL=NO.
