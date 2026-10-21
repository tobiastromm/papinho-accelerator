---
id: PAPINHO-PROTOCOL-SPECIFICATION-PRINCIPLE-CANDIDATE
status: proposed
revision: 1
date: 2026-10-11
scope-level: product-family
scope-target: papinho-ecosystem
review-owner: PapinhoEngineering governance authority
---

# Proposed ecosystem principle — human-readable protocol specifications

This is an encaminhável proposal, not a global accepted ADR. It is prepared
locally; no central repository was edited and no message was sent externally.
Canonical ADR identity/acceptance belongs to PapinhoEngineering through
OrganizationEngineering governance. The existing local C2 documentation
requirement does not imply global approval.

## Proposed principle

HUMAN_READABLE_PROTOCOL_SPECIFICATION_REQUIRED=YES
PROTOCOL_SPECIFICATION_INDEPENDENT_OF_IMPLEMENTATION=YES
ADR_DECISION_TRACEABILITY=REQUIRED
SPECIFICATION_ENCODING_TEST_CONSISTENCY=REQUIRED

Apply progressively to proprietary protocols owned by the ecosystem, including
their framing, negotiation, operations, security binding and terminal semantics.
Imported third-party standards retain their own authority; document local
profiles/extensions and governed dependency identity without rewriting them.
API-only/private helpers are not automatically wire protocols.

## Proposed lifecycle

At each contractual decision: record an appropriately scoped proposed/accepted
ADR under normal authority; update a separate versioned human specification;
update sequences/state machines and conformance/test mapping; explicitly label
proposed, experimental, deferred and accepted scope. Do not embed a complete
specification in an ADR or retroactively alter accepted decisions to add rules.
A new normative extension/decision records new authority and compatibility.

The specification must allow an independent peer implementation after encoding
approval. It must define vocabulary and normative terms, roles/trust boundaries,
versions/capabilities, exact byte grammar/endianness/limits/validation, state and
sequence rules, compatibility, security, errors/deadlines/backpressure/ownership,
positive/negative vectors and immutable source/test/package identities.
Documented protocol evolution must precede qualification, not be inferred from code.

Before qualified interoperability, accepted decision revisions, specification,
encoding/codecs, vectors and tests must be consistent and independently reviewed.
No guessed IDs/layouts, fake execution or absence-of-crash conformance.
Keep documentation tests distinct from portable/native/integrated/target tests.

## Progressive adoption and evidence

Existing protocols may enter a documented inventory with precise coverage/gaps,
not retroactive global disqualification or invented historical compliance.
Central authority must decide rollout, exception/time-bound dispositions,
repository ownership, conformance thresholds and release impact. Any exception
needs explicit scope/owner/expiry and must not weaken accepted security contracts.
A protocol's own accepted approval/execution gates remain in force.

Initial illustrative adoption is PACC C2: decision candidate, draft0.2, versioned
Mermaid, revision3 conformance matrix and encoding handoff. It is documentation-
complete for current review scope but not approved/implemented C2 wire.

## Forwardable decision request

PapinhoEngineering owner should review applicability, wording, progressive
migration and evidence/release gates, assign a family ADR identity if appropriate
and obtain governance approval before marking accepted. Proposed trial applies
only through separately authorized project tasks; no silent ecosystem-wide
policy or release gate is asserted by this preparation.

Related authority: OrganizationEngineering ADR-0002 (decision governance) and
ADR-0003 revision3 (taxonomy/lifecycle); PapinhoEngineering ADR governance/
project applicability; Accelerator ADR-0025/0029 and local C2 specification
requirement. Normative decisions, live status, future proposals, handoffs and
Code/Tests remain different categories.
