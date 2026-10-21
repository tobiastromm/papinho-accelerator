# C2 exact encoding/security approval handoff
2026-10-11. READY_FOR_OWNER_REVIEW; NOT_ACCEPTED, NOT_IMPLEMENTED.

Review [spec0.3](../protocol/pacc-c2-protocol-specification-draft-revision-0.3.md), [exact layouts](../protocol/pacc-c2-exact-wire-layouts.json), [candidate byte vectors](../protocol/pacc-c2-exact-wire-candidate-vectors.json), [ED01-ED14/security/conformance](../capabilities/pacc-c2-exact-wire-security-review-20261011.md) and [encoding ADR candidate](../adr/ADR-CANDIDATE-pacc-c2-exact-encoding-and-security.md) together with preserved semantic revision3.

Proposed capability3/major1 and messages0x0010..0x001B are collision-free against captured local registry only; not globally reserved. Existing IDs1..15, negotiation count2+two6-byte keys=result14, Envelope/CONTROL1.0 and old peer behavior remain unchanged.

Owner must accept/amend each ED row, scopes and residual risks, allocate canonical ADR/registry, approve server entropy/comparison source and total resource caps, and decide public API/package metadata. Security and compatibility review must include Legacy on-path exposure, independent Session binding, local revocation/escaped bytes, bounded shutdown fences and server FINAL versus Browser-owned native closure. Required public PST verification facts must be demonstrated before dependent code; no PST change is authorized.

No code implementation, protocol activation, E1-E12, production Legacy, PST promotion, commit/push/tag/release authorized. C0/C1 preserved only for prior exact scopes, not complete all-candidate policy or actual NT4.

NEXT=OWNER_APPROVAL_OF_EXACT_C2_ENCODING_AND_SECURITY_CONTRACT
