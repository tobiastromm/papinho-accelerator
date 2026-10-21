# PACC C2 human-readable specification and consistency requirement

Status: **accepted owner requirement for documentation and qualification only**.
Date: 2026-10-11.
Authority: explicit section 10 of
PAPINHOACCELERATOR-PAPINHOBROWSER-C2-PACC-VERSIONED-CONTRACT-JOINT-REVIEW.
This records the requirement already supplied by the owner. It does not accept
the proposed C2 service, allocate IDs, approve an encoding or authorize code,
C3, Legacy production, E1–E12, PST publication or a pin change.
No existing accepted ADR is amended. ADR-0025 retains protocol ownership;
ADR-0029 retains Mode C and experiment/production gates.

A human-readable, implementation-independent PACC C2 specification MUST exist
before protocol qualification. An official IETF publication is not required.

The specification MUST define its vocabulary and the meanings of MUST,
MUST NOT, SHOULD and MAY; distinguish accepted requirements, proposed clauses,
illustrative examples and unresolved owner decisions; and identify exact
normative revisions and applicable compatibility profiles.

It MUST contain responsibilities and trust boundaries; Session establishment,
version/capability negotiation; exact approved field layouts/types/sizes/order/
endianness/limits/validation; normal and error sequences; Session/operation/
channel state machines; authorization/role/generation/endpoint binding;
security and Secure/Legacy distinctions; cancel/revocation/deadlines/
backpressure/EOF/half-close/terminality/Model E ownership; and positive/negative
conformance criteria. Sequence diagrams MUST use versionable text or Mermaid
and live with the specification.

After encoding approval, exact C2 wire vectors and factual diagrams MUST be
derived from that approved encoding, with source revision and test identities.
Unapproved IDs, role values, fields or bytes MUST NOT appear as definitive wire.
Pending decisions MUST be explicit; an incomplete draft MUST NOT be presented
as sufficient for implementing an interoperable peer.

Codecs, vectors, state/validation tests and specification MUST identify the
same approved contract revision. Later changes MUST update those artifacts
together and preserve backward-compatibility vectors.

The qualification gate is:
**NO_C2_PROTOCOL_QUALIFICATION_WITHOUT_SPECIFICATION_ENCODING_AND_TEST_CONSISTENCY**.

Evidence for this gate MUST include an approved complete grammar/registry,
specification-to-codec/test mapping, exact valid/invalid vectors, old/new peer
behavior in both directions, portable consumer checks and an independent-peer
implementation/read-through demonstrating that the specification contains all
interoperability requirements. Current-host results MUST NOT imply NT4 execution.

Current implementation: [specification draft](protocol/pacc-c2-protocol-specification-draft.md).
Current review and unresolved encoding decisions:
[revision 1 joint review](capabilities/pacc-c2-joint-review-20261011.md).
