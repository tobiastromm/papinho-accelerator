---
adr: ADR-0034
title: PACC C2 Revision 3 exact contract and separated implementation gates
status: accepted
decision-date: 2026-10-12
last-revised: 2026-10-12
revision: 1
scope-level: project
scope-target: PapinhoAccelerator
decision-makers: [Tobias Tromm]
supersedes: []
superseded-by: []
---

# ADR-0034 — C2 contract approval and separate implementation gates

## Authority and context

Owner TASK=PAPINHOACCELERATOR-PACC-C2-REVISION3-NORMATIVE-APPROVAL-AND-IMPLEMENTATION-GATE-SEPARATION explicitly authorizes acceptance and exact encoding allocation if sufficiently determined, while withholding C2/C3 implementation. Documentary ED01–ED14 review found no essential unresolved wire/security choice; actual future runtime proofs are separate gates. OrganizationEngineering/ADR-0002/0003 govern this record and independent RFC specification. ADR-0033 is already assigned to isolated Model E qualification; its scope is not broadened or silently transplanted into main. ADR-0034 is the next unused local identity after that governed record.

The earlier gate incorrectly coupled contract acceptance with execution of a not-yet-implemented C2 owner. Conditional-semantic approval is preserved historically; this subsequent owner-authorized decision approves the exact contract. Existing accepted ADR-0022/0029/0032 and isolated ADR-0033 remain unchanged. No implementation/production fact follows from this decision.

## Decision

Accept semantic Revision3 and exact encoding1 as normative specification revision1.0, including resolved ED08 partial order and controlled ED11 policies. Allocate MODE_C_OPERATION capability3/major1 requiring TLS_OFFLOAD2/major1, message IDs0x0010–0x001B. Existing envelope/CONTROL negotiation, IDs1–15, keys1/1,1/2,2/1 and old-peer grammar remain unchanged. Normative registry allocation is separate from functional registration/codecs; no functional wire/header change occurs in this task.

Contract approval, implementation authorization, implementation qualification and production enablement are independent. Real-owner ED08, codecs, security negatives and full ED11 target measurements remain mandatory after separately authorized implementation; their current absence does not block this fully specified contract. Full ED08/ED11 runtime qualification is NOT granted. C2/C3 implementation, E1-E12, Browser work, public API/package revision/publication and production enablement remain unauthorized.

The normative specification fixes Session/grants, TWO_ROLES_ONE_OPERATION, identity/generation/token scopes, bounded CONTROL, exact ACK/fence facts, owner-local partial revocation, FINAL versus local cleanup, old-peer negotiation, Secure Principal and Legacy lower assurance. Legacy production remains denied. Site TLS remains independent from Network Egress; Browser owns Internet egress in Mode C.

ED06 requires modern OS CSPRNG with no fallback, full32-byte vetted comparison and non-elidable erasure of all quiescent secret copies. Exact utility source/hash/license and compiler-output proof are implementation artifact gates before secret-code integration; selection of an unreviewed utility is prohibited. ED07 requires exact positive public PST CLIENT facts before shutdown; unknown or missing facts fail closed. Nothing delegates essential security semantics to implementation discretion.

ED08 preserves the exact audited order in specification11.2. BA prefix acceptance/full ACK precede child release; COMPLETE is not output drain. Only DESTROYED plus factual ordered external C3 EOF and exact AB downstream prefix permit AB seal. Retention is query-only/non-clean with no retry/fallback or fabricated EOF. Manager/lease finalization is independent from child destruction and Browser's native owner.

ED11 approves managed2MiB controlled cap and evidence-backed checked reservation/rollback/identity/Session/CONTROL/host-credit/retained-quota/fairness policies, subject to implementation conformance. Numeric incremental totals20/32MiB user and2/8MiB kernel remain candidates. Provider/kernel/full Browser/NT4 and exact host margin/freshness remain explicit target/admission gates, not RAM minima. Unknown required bounds deny operational enablement before effects; implementation may exercise that denial without claiming target qualification.

## Frozen artifacts

The following hashes freeze complete UTF-8 artifacts; schemas/example bytes remain byte-identical to encoding candidate1 apart from approval metadata and explicit assigned-ID provenance. Specification contains all formats, examples, sequences and machines; this ADR does not duplicate it.

- docs/protocol/pacc-c2-specification-revision-1.0.md SHA256=EB3738CE8BB7A76678131BD8349FD3B0EDE33D502509377744AFE93256B5BB42
- docs/protocol/pacc-c2-wire-layouts-revision-1.0.json SHA256=C969F34D154C85114BB6DDD233EDA91DC96F2AA196654D38FCB36BA3AECBB877
- docs/protocol/pacc-c2-wire-vectors-revision-1.0.json SHA256=C90C7164773C7038E29C34F6A51CF3DF9C3CE9CA156C6F30010B0BB8BCD0B77E
- docs/protocol/pacc-c2-registry-allocation-20261012.json SHA256=91929E845AEA7AFB9417D4F2419185013D76C0A6C72355E8D6D0D76E9B5F05F3

## Rationale, rejected alternatives and consequences

Reject requiring absent owner execution before authorizing its implementation: that is a dependency cycle. Reject approving wire while inventing TLS/drain/security facts, relabeling synthetic/native evidence, weakening oracles, silently migrating Session grants or accepting empirical totals as hard bounds. Choose a frozen interoperable contract plus independently enforceable implementation/target gates. Failure of a later proof blocks that implementation/target and never becomes automatic production permission.

This absorbs the two preserved C2 semantic/encoding candidates locally without deleting history. Global ecosystem human-protocol governance remains proposed; only the existing local ADR↔RFC↔encoding↔test consistency obligation is accepted here. Future/docs/handoffs get explicit current-state references rather than competing approval statuses.

## Implementation verification and operational limits

Implementation authorization readiness means the contract is determined, not that authorization has been issued. Gate matrix ED01–ED14 and independent IG01–IG08 are in docs/capabilities/pacc-c2-normative-approval-and-gates-20261012.md. No gate is waived by this ADR. API3/SPI4/OpenSSL package-only targets remain distinct from normal/test-only/native/NT4 evidence and from production pin0.6.4. No PST/Browser/wire implementation, commit/push/tag/release or promotion is authorized.

## Relations and history

PapinhoAccelerator ADR-0022/0029/0032; isolated ADR-0033; PapinhoSecureTransport ADR-0008/0009; PapinhoEngineering ADR-0013; OrganizationEngineering ADR-0002/0003. Preserved semantic/encoding ADR candidates, drafts0.3/0.3.2/0.3.3/0.3.4 and ED01–ED14 evidence retain provenance.

| Revision | Date | Decision |
| --- | --- | --- |
| 1 | 2026-10-12 | Owner-authorized exact contract approval, registry allocation and separation of implementation/qualification/production gates. |
