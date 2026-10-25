# Exact predicate review and vector preparation

Date2026-10-21. Documentary review only; no executable crypto proof, build,
key generation, source selection or operational network. Public official
specification/source consultation is distinct from network tests.

| Requirement | Normative clause | Disposition |
|---|---|---|
| Exactly32 bytes, no unsafe short read | P0 | Defined |
| Canonical coordinate and sign | P1 | y<p, parity selection, reject negative zero, exact roundtrip |
| Valid Edwards point | P0/P1 | Equation and successful decoding required |
| Identity/neutral element | P0/P2 | Same O=(0,1), explicitly rejected |
| Small-order and mixed-order exclusions | P2 | P!=O AND [L]P=O; no cofactor clearing |
| Administrative versus signature validity | P3 | Separate checks, no authority |
| TS1/TS7 compatibility | P0-P3 | Explicit specialization of previously underspecified validity |
| RFC10049 compatibility | P5 | Long-term local eligibility only; unchanged protocol verification |
| Lifetime and Model E | P4 | Existing owner, synchronous borrow, factual closure, no timeout free |
| ED08/ED11 | P5 | No runtime implementation; UNKNOWN costs remain UNKNOWN |

The normative predicate is complete without a backend. <B> has prime order L;
therefore the nonidentity solutions of [L]P=O have order exactly L. On the
cofactor torsion component multiplication by odd prime L is invertible, so
any nonzero torsion component fails that equality. This is the review's
mathematical justification; testing [8]P!=O alone is insufficient.

RFC8032 section5.1.5 produces public points as multiples of B. The project
constraint does not replace section5.1.7 verification or add a check to R
or temporary PUBK by inference. No RFC conflict requiring a wire change found.
No claim that subgroup validation alone establishes a secure deployment.

## Backend reference evidence

Consulted the requested official documentation, core source and ChangeLog:

- https://doc.libsodium.org/advanced/point-arithmetic
- https://github.com/jedisct1/libsodium/blob/master/src/libsodium/crypto_core/ed25519/core_ed25519.c
- https://github.com/jedisct1/libsodium/blob/master/ChangeLog
- https://raw.githubusercontent.com/jedisct1/libsodium/1.0.21-RELEASE/ChangeLog

Documentation warns versions<=1.0.20 can accept mixed-order points. Master
ChangeLog describes the fix under1.0.20-stable;1.0.21 release notes include
that security fix. Thus historical stable snapshots require exact revision
analysis; no blanket claim about every snapshot sharing a version label.
The inspected master core calls canonical/decoder/on-curve/small-order/main-
subgroup helpers. Reading call sites does not qualify helper correctness,
compiled behavior, sign edges or target portability. These mutable references
are evidence for candidate review only, not a pinned dependency. No source
code from upstream was compiled or incorporated. Exact source/commit/hash,
license/dependency closure, independent tests, VC6 and ED11 remain gates.

## Vector matrix — planned, not executed

Notation Enc is P1 encoding; B,L,O are P0 constants; Tj is a point of exact
order j. Symbolic cases specify required coverage, not imported byte fixtures.

| Class | Planned input / provenance | Expected | Qualification obligation |
|---|---|---|---|
| Valid | RFC8032 section7.1 TEST1 public key d75a980182b10ab7d54bfed3c964073a0ee172f3daa62325af021a68f707511a | DATA valid | Public independent vector, no private seed copied |
| Identity | Enc(O)=01 followed by31 zero octets | Reject | Mathematical boundary, not source identity |
| Order2 | Enc((0,-1)) | Reject | Exact order independently checked |
| Order4 | Enc((sqrt(-1),0)) and its inverse | Reject | Both sign encodings; choose defined field roots |
| Order8 | Enc(T8), including all exact-order8 cases | Reject | Obtain pinned public vectors or label local derivation |
| Mixed2L | Enc(B+T2) | Reject | Canonical on-curve, not merely invalid bytes |
| Mixed4L | Enc(B+T4) | Reject | Demonstrate exact order4L |
| Mixed8L | Enc(B+T8) | Reject | Demonstrate exact order8L |
| Noncanonical y | Little-endian y=p and y>p with both sign bits | Reject | No reduction modulo p |
| Negative zero | x=0 sign1, y=1 or p-1 | Reject | P1 failure independently of subgroup rejection |
| Off-curve | Canonical y with nonsquare (y^2-1)/(d*y^2+1) | Reject | Pin public bytes and independent nonsquare evidence before tests |
| Truncated | Valid public input with length0..31 | Reject | Length before primitive, no out-of-bounds read |
| Excess | Valid32 bytes plus suffix, length33 or larger | Reject | No truncation |
| Missing | Null span, any length | Reject | No primitive call |

Only TEST1 is identified here as an independent published positive vector.
Symbolic math/boundary cases are not falsely described as external vectors.
No local fixtures generated; exact-byte negative corpus and independent
order/on-curve evidence must be pinned at backend qualification. Candidate's
own tests alone are not an independent oracle. No signature-only success
establishes point validity. No benchmark, compatibility or activation follows.
