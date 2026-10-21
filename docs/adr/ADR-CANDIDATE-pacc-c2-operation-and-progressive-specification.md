---
adr: null
candidate-id: ADR-CANDIDATE-PACC-C2
title: C2 operation boundaries and progressive protocol specification
status: proposed
revision: 1
proposal-date: 2026-10-11
last-revised: 2026-10-11
scope-level: project
scope-target: PapinhoAccelerator
decision-makers: []
review-authority: PapinhoAccelerator/PapinhoPACC owner, coordinated with Browser and security owners
supersedes: []
superseded-by: []
---

# ADR candidate — C2 operation boundaries and progressive specification

No canonical numeric ADR number or wire allocation is claimed. This is a
proposed separate decision; existing accepted ADRs are untouched.
It complements ADR-0022/0029, ADR-0012/0016/0018/0025 and the existing local
human-readable protocol requirement. It does not authorize implementation.

## Problem

Revision2 leaves Session composition, CONTROL bounds/progress, partial-send
revocation, ACK meaning and role/direction fences undecided. A codec alone
cannot settle ownership/security/compatibility or document interoperable peers.

## Proposed decision

Adopt [revision3 semantic contract](../future/pacc-c2-versioned-contract-proposal-revision-3.md)
R3-I01-I10 as a coherent candidate: dedicated two-key C2 Session and preserved
existing-service Session under one bounded integration owner; state/type CONTROL
limits and reserved notification capacity; local revocation with explicit
irrevocable prefixes; distinct ACK/activation facts; ordered per-role/direction
prefix fences and downstream ACKs; scoped CONTROL final outcome and Model E
retention. Site TLS grants remain independent of remote Network Egress.

Adopt R3-I11-I12 progression locally: decision record first, separate protocol
specification update, matching versioned diagrams/state machines, conformance/
test mapping and explicit proposed/experimental/pending dispositions.
No whole specification is embedded in this ADR.

## Alternatives and rationale

A three-key offer to unknown peers exceeds current negotiated assumptions.
Silent same-Session capability growth violates ADR-0012. Unbounded Session
pooling or parallel production runtimes violates bounded owner composition.
The proposed two explicitly scoped Sessions preserve old services without
claiming C2 grants authorize Mode B. Additional connections/leases consume caps.

Finishing cancelled DATA frames with new unauthorized payload contradicts
revocation; splicing violates framing. Abandonment preserves escaped prefixes,
at cost of non-clean termination. Instant global revocation across independent
streams is impossible to infer; owner-local guards and loss/deadlines are explicit.

CONTROL-only closure ordering cannot prove DATA prefixes; ordered role fences
supply scoped receipt/downstream evidence, at cost of state/accounting.
ACKs do not attest remote physical endpoint, TLS internals or HTTP completion.

## Consequences and limits

New Session/dual-role state, deterministic shutdown fences and accounting are
implementation work, not present support. Unknown/missing grants fail before
effects. Retained resources deny new admission, never authorize eviction.
Legacy lower assurance and D9B production gate remain. Existing wire1-15,
PST/API/SPI/Browser behavior and production pin0.6.4 remain unchanged.

Semantic acceptance does not approve encoding or authorize C2/C3/C7.
PACC owner must separately settle every exact field/ID/layout/bound and security
parameter, review both-direction compatibility and publish approved vectors.
A human independent-peer read-through remains required before qualification.

## Decision-to-specification/test traceability

[Protocol draft0.2](../protocol/pacc-c2-protocol-specification-draft-revision-0.2.md)
references this candidate and maps R3-I01-I12 to clauses/diagrams.
[Conformance matrix and encoding plan](../capabilities/pacc-c2-revision3-conformance.md)
maps positive/negative oracles and required evidence; all new tests are planned.
[Approval handoff](../integration/pacc-c2-revision3-wire-approval-handoff.md)
contains the exact owner decisions and remaining encoding gates.

Acceptance must record approving authority/date/revision/hashes and the agreed
trade-offs, then assign a canonical ADR identity through normal governance.
No status accepted is inferred from this review task.


## Lifecycle checkpoint 2026-10-12

CONSUMED as a preserved historical candidate by [ADR-0034](ADR-0034-pacc-c2-revision3-contract-and-separated-gates.md) and normative specification1.0. Original candidate text/status records proposal history; it is not current contract approval authority. No implementation/qualification/production permission follows.
