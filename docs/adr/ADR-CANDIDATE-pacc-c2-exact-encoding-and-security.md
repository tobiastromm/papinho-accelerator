---
adr: null
candidate-id: pacc-c2-exact-encoding-and-security
title: C2 exact wire encoding and security boundaries
status: proposed
revision: 1
proposal-date: 2026-10-11
last-revised: 2026-10-11
scope-level: project
scope-target: PapinhoAccelerator
decision-makers: []
supersedes: []
superseded-by: []
---

# C2 exact encoding/security decision candidate
Joint PACC, Accelerator, Browser and security owner approval required. No canonical ADR number or registry allocation is inferred.

## Context
ADR-0022/0029 accept Mode C placement and experimental ownership constraints but explicitly leave versioned wire to a later decision. The preserved [semantic ADR candidate](ADR-CANDIDATE-pacc-c2-operation-and-progressive-specification.md) and semantic revision3 resolve proposed lifecycle choices. This separate candidate freezes nothing; it presents exact encoding/security choices for approval without retroactively editing accepted ADRs.

## Proposed decision
Approve or amend ED01-ED14 in the [owner matrix](../capabilities/pacc-c2-exact-wire-security-review-20261011.md), the exact [spec0.3](../protocol/pacc-c2-protocol-specification-draft-revision-0.3.md), layouts and vectors together:
- Capability3/major1 and types0x0010..0x001B, unchanged Envelope/CONTROL1.0 and old messages.
- Dedicated two-key C2 Session, independent existing Session under one owner, explicit grant gates.
- Portable exact mandatory fields, bounded DNS/IPv4 first-major inputs, protocol-only identities and checked exhaustion.
- Two32-byte independent role secrets from reviewed CSPRNG, constant-time comparison and finite negative-candidate work.
- Factual role/TLS-ready barriers, per-direction offset/fence/ACK and explicit downstream meaning.
- Local revocation and escaped prefix accounting, first cause/action/retention separate, server FINAL plus Browser local cleanup guard.
- Fixed wire staging/control reserves and approved experimental queue limits; measured total dependency caps still required before qualification.
- Secure authenticated first-hop admission distinct from plaintext Legacy lab-only assurance; Site TLS independent of Network Egress.

## Rationale and consequences
The exact layouts permit independent candidate-peer analysis while old wire remains compatible. Explicit role secrets and Session context prevent cross-operation/Principal attachment in Secure. Per-role ordering avoids imaginary cross-socket fences. Model E retention consumes capacity without fallback. Distributed observation, downstream ACK limitations and untrusted client physical-socket claims remain residual risks, not hidden guarantees.

Rejected alternatives and threat negatives are recorded in the review matrix. Restricting initial encoding to DNS/IPv4 is a proposed coverage limit, not a change to accepted all-candidate local policy. Missing public SDK verification facts or measured resource caps blocks dependent implementation, not permission to fabricate metadata or change PST.

## Approval and implementation gates
Status stays proposed. Owner acceptance must name exact revisions/hashes, canonical ADR identity, registry allocations, security/risk disposition and API/package metadata. No implementation/experiment/Legacy production permission follows automatically. Accepted ADRs unchanged; global protocol-specification governance remains separately proposed. C2/C3/E1-E12 not authorized; PST/wire/Browser functional code unchanged; pin0.6.4.


## Lifecycle checkpoint 2026-10-12

CONSUMED as a preserved historical candidate by [ADR-0034](ADR-0034-pacc-c2-revision3-contract-and-separated-gates.md). Exact encoding1 is accepted normatively with registry allocation and frozen revision1.0; code/codec/target/security utility/runtime proof and production gates remain independent. Original proposal bytes are preserved.
