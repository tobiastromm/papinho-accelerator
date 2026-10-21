# PACC C2 progressive RFC supplement 0.3.3

2026-10-12. PROPOSED. Spec0.3, supplements0.3.1/0.3.2, twelve layouts/fourteen vectors and encoding IDs remain unchanged. This document adds evidence links and candidate resource-policy conformance, not wire approval. MUST/MUST NOT/SHOULD/MAY retain draft0.3 definitions and become product obligations only after owner acceptance.

## Close states and factual scope

The normal OpenSSL x64/Combined OpenSSL harness observes the following order, preserving the original28 runs:

APPLICATION delivery guards → public reciprocal COMPLETE → proposed APP AB coordination fence → BA producer freeze → exact BA prefix/native C3 acceptance/FENCE_ACK → successful membership removal → try_release result/disposition → DESTROYED → independent C3 ordered EOF and AB drain → exact AB prefix/native site-emulation acceptance/FENCE_ACK → owner guards.

COMPLETE MUST NOT be interpreted as OUTPUT_DRAINED. AB MUST NOT seal without DESTROYED, ordered EOF and exact prefix accounting. A late byte after seal or callback for a retired generation MUST NOT restore authority. Candidate fixture FENCE/FENCE_ACK delivery is distinct from production PACC Session/grant authorization and Browser C1 execution. Clean/non-clean classification remains original draft0.3; reset/truncated EOF/cancel never qualify as clean.

~~~mermaid
sequenceDiagram
    participant A as Isolated consumer owner
    participant P as Normal OpenSSL PST child
    participant C as Owned C3 peer
    participant B as Site-emulation fixture
    A->>P: Public reciprocal shutdown
    P-->>A: COMPLETE (not OUTPUT_DRAINED)
    A->>A: Candidate normal-phase guard; freeze BA admission
    A->>C: Finish admitted BA prefix
    C-->>A: Native send count matches BA prefix
    A->>A: Candidate BA FENCE/ACK complete
    A->>P: Remove wait-set membership; try_release
    alt OK plus DESTROYED
        P-->>A: Pointer invalidated once
        C-->>A: Ordered EOF after exact AB bytes
        A->>B: Send exact AB prefix
        B-->>A: Native accepted count
        A->>A: Candidate AB FENCE/ACK; separate final guards
    else retained
        P-->>A: Result and retained disposition
        A->>A: Preserve owner, leases and original quota
    end
~~~

The retained branch in this diagram is a contract branch. PRECONDITION_RETAINED is observed normally; CLOSE_UNRESOLVED_RETAINED OpenSSL is not observed. NSS historical native evidence and synthetic admission retention MUST NOT replace it. [R4 handoff](../integration/pst-openssl-model-e-r4-governed-mechanism-request-20261012.md) identifies the governed upstream dependency. NSS correction stays separate.

## Resource state machine and candidate admission

~~~mermaid
stateDiagram-v2
    [*] --> DENIED: Required envelope unknown or reservation unavailable
    [*] --> RESERVED: Identity and all required budgets validated
    RESERVED --> ACTIVE: External effects only after reservation
    ACTIVE --> QUIESCING: Stop, cancel or close intent
    QUIESCING --> TERMINAL_RETAINED: Unresolved lifetime
    QUIESCING --> RECLAIMABLE: Confirmed destruction and borrowers quiescent
    RECLAIMABLE --> [*]: Credits released once
    TERMINAL_RETAINED --> TERMINAL_RETAINED: Safe query, duplicate intent rejected
~~~

Retained slots, Session budgets, callback storage, provider/credential/runtime leases and kernel resources MUST remain charged. Stop, timeout, release attempt or remote FINAL MUST NOT reclaim them. Query-only retention MUST NOT cause eviction, new I/O, native retry or reconnect bypass. CONTROL/CANCEL/FINAL reserves MUST remain available when DATA fills. Payload and metadata/staging/allocator charges are separate; aliases are counted once.

Candidate numerical caps remain Browser managed2MiB/user20MiB/kernel2MiB and Accelerator managed2MiB/user32MiB/kernel8MiB, not hard PST/kernel guarantees. Browser app-consumer1MiB/op remains included. Unknown mandatory hard-bound category denies enablement before effects. Empirical-envelope admission requires an explicit owner decision accepting residual uncertainty; no observation after allocation becomes a hard fail-before-effects proof.

## Evidence and conformance

[Measurement matrix](../capabilities/phase6-c2-ed08-r4-ed11-measurement-evidence-20261012.json) records23 normal-package observer executions plus4 separately labeled admission-model executions, with Debug/Release and8/16/32/64KiB explicit variants. Only64KiB is the unchanged approved experiment default. No E1-E12 were run.

| Contract | Evidence | Remaining proof |
| --- | --- | --- |
| BA before release; AB after EOF | Original28 plus instrumented scoped probe order | Real C2/PACC/Browser owner integration and owner phase approval |
| Nonclean EOF/reset/cancel; late identity | Scoped native observer and candidate guard negatives | Production queue/Session implementation |
| Retained query-only owner | Actual PRECONDITION; candidate quota model | Governed OpenSSL R4/O1/O2 integration |
| Bounded controlled reservations | Compiled512KiB core/16KiB Session allocator; byte-size ledger | Actual Browser x86 and C2 structures/callback scheduler |
| Total user/kernel budget | Process private/commit, working set, pool quota and handle measurements | Full context/candidate topology, adversarial inputs, kernel attribution, real NT4 |
| ADR ↔ RFC ↔ tests | Linked proposed candidate and hash manifests | Accepted encoding then codecs/goldens byte consistency |

Measurements MUST identify target, package, source, run/process and operation/generation. PrivateUsage, working set, pagefile commitment and pool quotas MUST NOT be summed as nonoverlapping memory. Pool quota charges and observed SO_SNDBUF/SO_RCVBUF MUST NOT be reported as total socket-kernel memory. Real native partial-send counts were zero in these executions; escaped partial prefix is a separate cancellation fact.

Encoding remains PROPOSED_PENDING_FINAL_GATE. No runtime/global protocol qualification follows from this supplement.
