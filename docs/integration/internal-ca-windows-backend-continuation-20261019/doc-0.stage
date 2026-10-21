# Windows issuer continuation — scoped proof and implementation handoff

## Authority and scope

Accepted internal issuer/custody clauses 1–11 and technical profile 1.1–1.6, 2.1–2.5, 3.1–3.7 remain unchanged. Same-root I/R/A/P and checkpoint contracts remain intact. This implementation adds only a read-only public validation component in the Windows adapter directory. It does not register a productive backend or root authority. No KSP, ACL, key export policy, historical diagnostic key, service token, root codec or common issuer rule is changed.

Source preimages and exact postimages are in source-operations.json; the separately reviewed test-fopen-operation.json corrects only a CRT warning without suppression. The initial failed /WX logs are retained. No denied action was bypassed.

## Implemented boundary

apps/server/server_issuer_windows_validation.h/.c provide a function matching the existing PAPACC_ISSUER_BACKEND.verify_request signature. The Windows provider computes SHA256 of the exact CSR and canonical full DER SubjectPublicKeyInfo; both must match the immutable request. Decode/encode round-trip rejects noncanonical/trailing data. RSA2048/e65537, sha256WithRSAEncryption, explicit NULL parameters, zero unused bits, version zero and no attributes are required in this bounded implementation. Unsupported attributes/algorithms are rejected; CSR subject/extensions never supply administrative identity or grants. Native proof of possession is checked with CryptVerifyCertificateSignatureEx and the CSR public key, not a private key. No custom crypto primitive, signature or key generation exists.

The native function is callback-compatible, not a complete backend implementation. It is not installed into an operational backend; the test assigns only that existing port and calls it directly. Actual authorization, UTC, protected acquire/sign, complete X509 verification and release still need the same-root adapter. There is no pretend vtable that returns authorization or signer success.

A separate conservative result classifier maps NTE_PERM to DENIED, unsupported to UNSUPPORTED, retained native ownership to RETAINED and other errors (including BAD_KEYSET) to UNCERTAIN. It does not collapse original codes or mutate PAPACC_CA_KEY/first_failure. Raw SUCCESS/READY still returns DENIED because it proves neither root reservation nor signing authority. It does not touch NCrypt, close a handle, retry, export or infer global key absence.

## Lifetime / Stop

The caller owns and serializes the workspace descriptor, immutable record, CSR and scratch. Input/record/context/scratch overlaps are rejected before output mutation. Scratch is aligned, bounded by the supplied capacity and split into decoded structures and reusable synchronous tail; no pointer escapes the call. No allocation flag or owned handle is introduced. Native providers can use opaque resources; those costs remain UNKNOWN. Native_error preserves the failing Windows code, without logging input bytes. Stop is checked on entry and before/after native signature verification at the serialized owner boundary. This is NOT a cross-thread cancellation API and does not prove preemption/latency of a blocked provider call. Do not attach this synchronous helper to an active owner until its latency/lifecycle and cancellation composition are qualified.

The existing CNG owner, namespace worker, Stop, retained handles/buffers and root snapshots/pins are untouched. No actual retained resource is released by the classifier or tests. Native queries do not prove Model E DESTROYED. No new worker, registry, journal, listener, root or operational authority exists.

## Current root and issuance facts

Existing portable serial/reserve/state/revoke/renew code and canonical ROOT06 issuer persistence are preserved byte-for-byte. Same-root checkpoint R/A/P/high-water linkage remains implemented, but checkpoint freshness/backup/fencing and authoritative recovery remain DENY. D3+issuer carrier remains unsupported. No sign or issuance retry is added. Existing SIGN_REQUESTED uncertainty retains reconciliation semantics and consumes the reservation; no serial can be reclaimed by this component.

CSR_PREPARATION is only cryptographic request validation here. Constructing/signing a complete CSR is NOT implemented by this slice. X509_PREPARATION is still pending: no TBS/operational certificate builder or complete certificate profile verifier is claimed. The new validator must not be mistaken for CA chain validation, approved names/Principal mapping, signing authority or complete manual package generation. The accepted certificate profile remains the authority for that future implementation; no new profile is inferred.

## Validation evidence

Final build2 Debug/Release CMake targets: issuer common library, public Windows validation library/test, issuer offline, production gate and checkpoint; all pass with /W4 /WX, zero warnings. Focal existing CNG production and read-only variants also recompile Debug/Release with /W4 /WX and zero warnings. These are affected component builds, not a newly linked full server.

Four issuer CTests pass in each configuration. Detailed executable receipts show 117 common model assertions, 4 production gate assertions, 41 checkpoint assertions and 33 public-validation assertions per configuration. Existing CNG suites pass 39 read-only, 6 invalid-argument and 598 instrumentation/differential assertions per configuration; 19 preimage differential cases are included in that last suite. Replayed receipts do not count as distinct scenarios.

The new test consumes ONLY an existing public fixture certificate to build an ASN.1 CSR with a zero signature. Native Crypt32 must reject its proof of possession; digest mismatch, malformed CSR, trailing byte, Stop, overlap, wrong state and key-error/retention classifications are checked. No fixture private key is read. The test does not generate a key, certificate or valid signed CSR. Positive valid-CSR native qualification remains NOT_RUN. Core signature/certificate/Stop/revocation/renewal/uncertainty scenarios are synthetic; checkpoint hashing is native over synthetic metadata. They do not prove a native CA, actual CNG NTE_PERM reproduction, real certificates or recovery fencing. New test imports only public Crypt32 routines, no NCrypt/private-signing API (imports-and-frames.log).

## ED11 and activation gate

The operational root composition remains 2,093,415 / 2,097,152, margin 3,737; no operational instance, static data buffer or root field is added. The standalone public test has a caller descriptor of 24 bytes and scratch of 4,096 bytes, plus test-only public fixture buffers. Debug helper frame allocation is 400 bytes; Release 144 bytes, plus saved registers/callers/provider costs. These frame allocations are NOT full peak bounds. Native key owner remains 472 bytes, unchanged. Common core216/record288/view176/backend88 are unchanged, not duplicate root storage.

If a new descriptor+4096 scratch were simply added to the current ALL scenario, even without stack/provider costs it would total 2,097,535 (>2,097,152 by383). Therefore runtime attachment is DENY; this library is NOT linked or instantiated by the server. Any future reuse of existing scratch needs an explicit non-overlap/coexistence/retention proof, not subtraction of a legitimate reservation. No available margin approves the new cost. Provider/kernel UNKNOWN, ED11 total PARTIAL.

## Next exact implementation boundaries

1. Preserve the existing UNKNOWN diagnostic key and unresolved native CNG failure. No creation/open/sign attempt is authorized by this slice. Future native work needs exact key identity/context, ownership disposition and its own official approval; no fallback KSP/exportable key or permission change.
2. Complete public CSR construction and X509 TBS/profile verification under the accepted roles/names/SKI/AKI/validity using reviewed provider routines and bounded caller storage; test positive public requests using explicitly identified non-operational evidence. Do not treat the zero-signature fixture as positive.
3. Add the real Windows backend through the existing authorize/UTC/verify/acquire/sign/verify/release ports only when the same root proves actual reserved request, generation/SPKI, CURRENT/history and required backup/fencing authority. No fabricated proof bits, authority, epoch or return-OK stub. Keep serial uncertain states and output/key leases retained.
4. Before runtime attachment prove ED11 space, callback latency/Stop and protected administrative isolation. Admin positive qualification remains BLOCKED_BY_ENVIRONMENT independently; no token modification or escalation was attempted here.
5. The manual client package generator may consume the completed issuer only after its own exact package contract and these signing/profile/resource gates. This task does not declare it ready, publish trust, emit credentials or finish Phase6.
