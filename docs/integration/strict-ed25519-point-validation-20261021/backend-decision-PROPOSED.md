---
status: proposed
task: PACC-PHASE6-STRICT-ED25519-POINT-VALIDATION-1
date: 2026-10-21
---
# Strict Ed25519 backend decision — PROPOSED

Accelerator main HEAD913c90cf2328f7939d8f5b927640601903ac6e91.
Browser HEAD96f6cd2f9ec4f834999828fa13bb7d170db66a98; clean status observed.
Existing Accelerator WIP (61 porcelain entries) was not edited.
No functional changes, builds, tests, library import, keys or operational network.

TS1 literally requires: Mathematical validity/correspondence is checked with an existing qualified primitive backend; no custom crypto.
TS3 requires32 raw bytes, nonzero key and SHA256 correspondence. TS7 rejects malformed keys. Frozen TS clauses do not enumerate subgroup, order, identity, canonical coordinate/sign encoding or small-order rejection. Do not silently extend ACCEPTED TS.
Frozen clauses SHA256 verified0f4d604e732167e2804e05e0188e7d808ca452cddeb6e334d4cfe811763c0038.
Accepted decision SHA256ddd9e17b01944b915e0c7bac3b4ec54dfca09ee1d2ee40916a374891a039a532.

Existing OpenSSL3.5.8 raw import/public_check is insufficient (archived noncanonical all-ff acceptance probe). Inspected BearSSL Curve25519 and NSS headers do not demonstrate a strict Ed25519 point validator. X25519 operations/TLS algorithm identifiers are not substitutes.

Candidate: libsodium crypto_core_ed25519_is_valid_point. Official documentation specifies canonical on-curve main-subgroup non-small-order checks, but warns versions through1.0.20 accepted mixed-order points. Release1.0.21 identifies a security fix. No revision selected: authenticate/pin and audit corrected source before qualification. Do not replace Roughtime Ed25519 with Ristretto or custom curve code.

Official references consulted2026-10-21:
https://doc.libsodium.org/advanced/point-arithmetic
https://github.com/jedisct1/libsodium/releases
https://github.com/jedisct1/libsodium/blob/master/src/libsodium/crypto_core/ed25519/core_ed25519.c
https://doc.libsodium.org/installation
https://raw.githubusercontent.com/jedisct1/libsodium/master/LICENSE

ISC permits redistribution with notices retained; exact dependency notices remain subject to review. Modern Visual Studio build documentation does not qualify VC6/NT4, box or ISA. C89 caller ABI does not prove backend compatibility. Sources above are mutable review references, not pinned dependency evidence.

Focal proposal: explicitly define exactly32-byte canonical compressed Edwards25519 encoding, valid curve point in prime-order main subgroup, excluding identity and small-order points, including canonical sign handling. Null/length/backend/error failures deny. This proposed clarification is not ACCEPTED and does not change RFC signature semantics.

After predicate approval, evaluate an authenticated pinned corrected libsodium build as an additional narrow validator, preserving existing backends. Before import justify linked closure, license/provenance, initialization, CRT/OS/CPU dependencies and full resources; avoid unreviewed extraction. Submit exact source hunks/build/test commands for official approval. Tests need independently justified valid/noncanonical/identity/small-order/mixed-order/off-curve/sign-bit/null/31/33-byte vectors. Signature tests alone are insufficient. No commands executed to qualify proofs in this task.

Adapter must be synchronous, borrow immutable bytes only for the call, retain no pointer or authority. Component9 remains denied until qualified; DATA validation never authorizes a TIME_SOURCE.

ED11 previous fixed Selected1952624, Legacy1790872; analytical coexistence2172856 is not an operational peak. New backend code/data, scratch, initialization, full stack, provider/CRT, retention and concurrency UNKNOWN. No reuse discount or quota increase. Windows3145728 explicit only, Legacy2097152 unchanged. No operational capacity qualification.

CURRENT_TASK_COMPLETED=NO
STRICT_ED25519_PRIMITIVE=NOT_QUALIFIED
BACKEND_SELECTED=NONE_LIBSODIUM_CORRECTED_REVISION_CANDIDATE
CANONICAL_ENCODING=PROPOSED_NOT_IMPLEMENTED
SUBGROUP_VALIDATION=PROPOSED_NOT_IMPLEMENTED
VC6_COMPATIBILITY=NOT_PROVEN
ED11_TOTAL_STATUS=PARTIAL
TIME_SOURCE_KEY_VALIDATION=DENY_UNTIL_QUALIFIED
UTC_SOURCE=NOT_QUALIFIED
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
FIRST_UNRESOLVED_DIVERGENCE=ACCEPTED_STRICT_PREDICATE_NOT_ENUMERATED
NEXT=EXACT_STRICT_PREDICATE_AND_PINNED_BACKEND_REVIEW
