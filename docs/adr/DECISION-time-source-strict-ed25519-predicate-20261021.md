---
status: accepted
revision: 1
decision-date: 2026-10-21
scope-level: project
scope-target: TIME_SOURCE strict public Ed25519 key DATA predicate only
decision-makers: [Tobias Tromm]
supersedes: []
---
# TIME_SOURCE: exact strict Ed25519 public-key predicate

## Authority and bounded precedence

This decision takes effect only after official executor approval of its exact
bytes and verified post-hashes, under task
PACC-PHASE6-ED25519-STRICT-PREDICATE-ACCEPTANCE-1 and its conditional owner
authorization. Accept only P0-P5 below. The preserved proposal, investigation,
review, vectors and upstream source are not accepted implementation by reference.

P0-P5 specialize TS1 mathematical validity/correspondence and TS7 malformed-key
rejection for the raw long-term TIME_SOURCE public key at TSR1 bytes[88,120).
No TS clause or historical serialized bytes are edited. This is a local
administrative key eligibility constraint, not an assertion that RFC signature
verification alone imposes this predicate on every point in a signature.

## P0 Domain and constants

Input is a readable immutable octet span s with explicit length n. Null,
unreadable or n != 32 is invalid; length must be checked before primitive access.
Define p=2^255-19, Fp, d=-121665/121666 in Fp, curve
E: -x^2+y^2=1+d*x^2*y^2 over Fp. Let B be the Ed25519 base point,
L=2^252+27742317777372353535851937790883648493 and O=(0,1).
O is both identity and neutral element, not a second exceptional point.
[L] denotes integer group multiplication without reducing L modulo L.

## P1 Canonical decoding

Read y from bits0..254 little-endian and sign b from bit255. Require
0 <= y < p. Recover x in Fp satisfying x^2=(y^2-1)/(d*y^2+1).
Reject failed inversion, absent square root or failed curve equation.
Select canonical representative 0 <= x < p with x mod2=b; reject x=0,b=1.
Re-encoding (x,y) must reproduce all32 input bytes exactly. No input reduction,
masking away the sign, repair, truncation, extension or permissive normalization.
Field arithmetic inside a qualified backend is not input normalization.

## P2 Exact group predicate

For the uniquely decoded P require P != O AND [L]P=O.
These are necessary and sufficient after P0/P1: P has exactly prime order L
and lies in the main subgroup <B>. The identity satisfies [L]O=O but is
explicitly rejected. All other points of orders2,4,8 are rejected. All
mixed-order points of orders2L,4L,8L are rejected. Equivalently, writing
P=Q+T with Q in <B> and T in the cofactor torsion subgroup, require T=O
and Q!=O. Merely rejecting [8]P=O does not reject mixed-order points;
cofactor-clearing P before checking is forbidden. Explicit small-order checks
may supplement but never replace the full predicate. Do not scalar-reduce
L to zero as a purported subgroup test.

## P3 Result and authority boundary

Only the conjunction P0,P1,P2 permits mathematical DATA validity. Unsupported
primitive, error, unknown result or incomplete check denies usable validation.
This is distinct from Ed25519 signature verification over the RFC domain and
message; neither substitutes for the other. Key/fingerprint correspondence,
canonical administrative identity/purpose/authorization/status/revocation,
CURRENT/revision/pin/lifetime, source correctness and Q0-Q7 remain independent.
No valid point, signature, decoded DATA or LAB key grants TIME_SOURCE authority.

## P4 Backend and lifecycle gates

No backend selected or implementation authorized. libsodium1.0.21 or a later
corrected revision is a candidate only. Before selection require exact version,
commit and artifact hashes, authenticated provenance, license/dependency
review, independent vectors, audited predicate equivalence including sign-bit
edges, C89 interface plus VC6/target build evidence and full ED11 costs.
No whole-library import, private primitive, Ristretto substitution or provider
replacement follows. Preserve existing OpenSSL/BearSSL/NSS/backend roles.

A future validator borrows already protected immutable bytes synchronously;
no stored pointer, new pin/owner/worker or source authority. Stop prevents
new effects; uncertain operation resources remain retained until factual
quiescence/closure under Model E. No timeout release or fallback. This decision
creates no API, storage, allocation, lease or lifecycle implementation.

## P5 Compatibility and capacity

RFC8032 encoding/decoding and generated Ed25519 public points support this
local eligible-key subset. RFC10049 long-term keys remain32 bytes, with its
existing signature, delegation and wire rules. Temporary PUBK, signature R/S,
issuer/package keys and historical codecs are not redefined by this decision.
No wire, root format, CP4, TSC1/TSR1 offset or hash domain changes. Historic
DATA never restores operational authority. New implementation must enforce
this predicate without altering historical bytes or silently normalizing them.

No backend/target is qualified by normative acceptance. No runtime bytes added
by this document; backend code/data/init/stack/scratch/retention/transitive
costs remain unmeasured UNKNOWN. ED11 total PARTIAL; no reuse discount or
quota increase. Windows3145728 only explicit selection; Legacy2097152 and
individual quotas unchanged. ED08/Model E gates persist.

## References and status

[RFC8032 sections5.1-5.1.3,5.1.5,5.1.7](https://www.rfc-editor.org/rfc/rfc8032),
[RFC10049 sections5,9.3](https://www.rfc-editor.org/rfc/rfc10049).
The subgroup exclusion is this explicit project specialization, not a new
requirement attributed to RFC8032's cofactored verification equation.

[Review and vector matrix](../integration/ed25519-strict-predicate-acceptance-20261021/conformance-and-vectors.md)
and [input hashes](../integration/ed25519-strict-predicate-acceptance-20261021/input-hashes.json)
record evidence separately from normative clauses.

STRICT_ED25519_PREDICATE=ACCEPTED
BACKEND_REFERENCE=LIBSODIUM_1_0_21_OR_LATER_CORRECTED_CANDIDATE
BACKEND_SELECTED=NONE
BACKEND_IMPLEMENTATION_AUTHORIZED=NO
TIME_SOURCE_KEY_VALIDATION=DENY
UTC_SOURCE=NOT_QUALIFIED
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
