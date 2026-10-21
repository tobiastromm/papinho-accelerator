# PACC C2 RFC supplement0.3.4 — total resource accounting and admission

2026-10-12. PROPOSED protocol/admission supplement. Preserves specification0.3, supplements0.3.2/0.3.3, twelve proposed messages and fourteen vectors. No ID/layout/encoding change. [Owner's incremental interpretation](../adr/DECISION-ed11-preserve-incremental-resource-envelopes-20261012.md) is ACCEPTED; numerical caps and this implementation policy are not automatically accepted.

MUST/MUST NOT/SHOULD/MAY retain draft0.3 meanings. Accepted owner instructions apply now; newly proposed product clauses require approval.

## Resource plane and scope

An operation, CONTROL Session, provider/runtime and physical process have distinct lifetimes. Count owned allocations once, record aliases/borrowers, and preserve original charges on retention. Session CONTROL storage MUST NOT be freed merely because its last operation ends. Session callback/lease borrowers and retained children delay reclamation. A retired operation/Session generation cannot retarget a later owner.

Candidate envelopes: Browser managed2MiB/incremental user20MiB/incremental kernel2MiB; Accelerator managed2MiB/incremental user32MiB/incremental kernel8MiB. These are neither process-total ceilings nor minimum RAM. Total process, baseline, increment, peak, shared attribution, retained resources and target margin are separate evidence.

Private commit, working set, classified virtual mappings, reserved address space and kernel quota charges MUST NOT be summed as disjoint allocations. Unmeasurable provider/kernel categories remain unknown, not zero.

## Proposed reserve/commit/rollback machine

~~~mermaid
stateDiagram-v2
    [*] --> DENIED: Unknown required bound or missing provider/measurement/kernel/host margin
    [*] --> RESERVED: Checked sizes, IDs, global/Session slots and host credits
    RESERVED --> ROLLED_BACK: Partial allocation or cancel before effects
    ROLLED_BACK --> [*]: Only unconsumed allocation and credits refunded
    RESERVED --> ACTIVE: All owned storage established, authority still valid
    ACTIVE --> QUIESCING: Stop or close intent
    QUIESCING --> RETAINED: Lifetime unresolved
    QUIESCING --> OP_RECLAIMED: Destruction confirmed, callbacks quiescent
    OP_RECLAIMED --> SESSION_ALIVE: CONTROL and Session borrowers independent
    SESSION_ALIVE --> SESSION_RECLAIMED: Control destroyed, no children or borrowers
    RETAINED --> RETAINED: Query only; no eviction, retry or credit recovery
~~~

Before external effects, owner MUST check required envelopes, checked arithmetic, exact immutable identity, finite slots and incremental credits AND available commit/physical/process margin. Deduct reserved host credit so multiple admissions cannot spend the same capacity snapshot twice. Actual allocator failure is still possible; rollback only newly acquired unconsumed storage. No consumer native fallback is introduced.

Retained charges survive Stop/timeouts and initiating-scope return. Session and operation credits are reclaimed independently only with factual destruction and borrower quiescence. Session generation exhaustion denies fresh incarnation; no wrap/reuse.

## Proposed bounded fairness and control reserves

The experimental64KiB four queues/high48/low24KiB and16KiB flow service budget remain unchanged. Fixed CONTROL803/819, CANCEL56/FINAL92 and bounded role-ACK staging cannot borrow saturated payload storage. One bounded CONTROL opportunity before additional DATA, at most8 opportunities/wake and rotating flow cursor prevent continuous CONTROL or DATA from monopolizing one owner. Blocked progress yields future readiness/deadline handling, not unbounded drain or fake completion.

The isolated policy tests fill four DATA spans and CONTROL staging, verify opportunity/count bounds and release guards. This is not production PACC dispatch, TLS CONTROL liveness or actual serialized Session traffic.

## Conformance traceability

| Clause | Evidence / status |
| --- | --- |
| Incremental meaning / host exhaustion | Accepted owner decision; policy negative host commit/physical/process tests |
| Partial allocation/cancel rollback | Deterministic allocator gate tests; actual slabs freed without external effects |
| Exact limits/overflow/finite count | C89 policy negative matrix;50 checks in each of3 builds |
| Retained quota / no eviction | Synthetic controlled-storage state; normal PRECONDITION observed separately; R4 external |
| Session independent lifetime | Zero-operation CONTROL reserve, old-incarnation and callback negatives |
| Control/data fairness | Saturated controlled-storage scheduler model, not integrated wire |
| Total resources | Native loops, private/working-set/commit/quota/handles/threads, VirtualQuery classification, external C1 measurements |
| Legacy/NT4 | No production Legacy or NT4 execution claim; C12 pending |
| Codec/spec consistency | Original schemas/vectors unchanged; future accepted wire/codecs/goldens gate remains |

[Ledger](../capabilities/ed11-total-resource-ledger-reconciled-20261012.md), [candidate ADR](../adr/ADR-CANDIDATE-ed11-total-resource-admission.md), [current closeout](../capabilities/ed11-total-resource-closeout-20261012.md), [owner handoff](../integration/ed11-total-resource-final-owner-handoff-20261012.md).

Encoding remains proposed; ED11 controlled policy qualification does not qualify whole-runtime resource ceilings or approve C2/C3 implementation/E1-E12.
