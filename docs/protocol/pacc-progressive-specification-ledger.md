# PapinhoPACC progressive human specification ledger

Revision1 / 2026-10-11. Live ledger, not an accepted protocol extension.
Existing normative specifications remain authoritative; this index neither
relabels historical documents nor silently accepts new C2 rules.

| Protocol area | Decision authority | Human specification | Current consistency scope |
| --- | --- | --- | --- |
| Envelope1.0 | ADR-0002/0004 and normative framing | [Framing](../protocol-framing.md) | Approved header/parser rules; existing codecs/goldens |
| CONTROL1.0 | ADR-0003/0006 and CONTROL specification | [CONTROL](../control-establishment-protocol.md) | Existing bytes; security before publication |
| DATA association | ADR-0003/0006 | [DATA](../data-association-protocol.md) | Existing ticket/accept grammar, Principal/Session gates |
| Capability negotiation | ADR-0012/0016/0018 | [Protocol overview](../protocol-overview.md), public codec contracts | Preserve canonical sets/old28 bound; overview generation summaries partly historical |
| Network Offload | ADR-0017/0022/0023, family ADR-0013 | [PACC package live contract](../integration/papinhopacc.md), capability documents | Cumulative IDs1-15 and negotiated v2; no C2 authority |
| C2 operation extension | [Proposed ADR](../adr/ADR-CANDIDATE-pacc-c2-operation-and-progressive-specification.md), accepted ADR-0029 bounds | [Draft0.2](pacc-c2-protocol-specification-draft-revision-0.2.md) | Semantic candidate + planned conformity only; encoding unapproved |

Progressive workflow: appropriately scoped ADR decision → separate human
specification clauses → matching sequences/state tables → field/codec/vector/
conformance tests → exact version/hash ledger and qualification gates.
Existing local [C2 requirement](../pacc-c2-specification-requirements.md) is
preserved. [Global principle](../future/papinho-ecosystem-protocol-specification-governance-proposal.md)
is proposed, not ecosystem accepted.

Draft0.1 and revision2 review remain historical input, unmodified. Draft0.2 is
the current C2 candidate, not an approved/implemented wire. C2 examples remain
empty until encoding approval; existing examples stay explicitly existing-only.
Future decisions must update each linked artifact or record scoped pending/
experimental status, not infer conformity from source alone.
