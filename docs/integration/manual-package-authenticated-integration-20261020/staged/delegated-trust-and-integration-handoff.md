# Factual delegated-trust and authenticated integration gate

Task PACC-PHASE6-MANUAL-PACKAGE-AUTHENTICATED-INTEGRATION-1. Review at main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a. No functional patch applied. All577 baseline source/build/ADR/capability preimages and the previous61-entry manifest must remain byte-identical. Existing WIP is not restored, normalized or discarded.

## Exact ordering divergence

GOVERNANCE CONFLICT: task section3 lists derivation and AEAD before independent signature and signer trust. The exact accepted M1 third paragraph states:

> Authenticate independent approved signer before KDF; verify AEAD tag before parsing/using plaintext.

M5 independently orders approved-signer authentication, bounded KDF/AEAD quarantine, archive/material validation and existing transactional import. The task also requires the contract's order and creates no accepted normative amendment. Do not infer a deliberate supersession from the conflicting list. No dependent functional integration is applied. This handoff preserves M1; it is not a new accepted rule. If the owner intended a different order, a delimited normative review is necessary before dependent code.

Compatible implementation sequence, subject to resolving the task discrepancy and trust/budget gates:

1. Hold immutable complete input through existing operation ownership; validate envelope bounds/version/suite/count/SPKI before expensive crypto.
2. Acquire the independent approved signer context and its factual owner/lifetime. Validate fingerprint/purpose/delegation/validity/revocation; verify the mathematical package signature over the exact M2 domain. Both trust AND signature must succeed before KDF. No included CA or embedded SPKI authenticates itself.
3. Obtain owned normalized password and exact salt/nonce/AAD; derive exactly600000 PBKDF2 iterations. No fallback or iteration tuning.
4. Authenticate/decrypt into exclusive quarantine. No externally accessible archive/plaintext before final tag success. Authentication success alone still does not release a package result.
5. Inspect exact ZIP STORE/TLV; validate actual CA/client/key/identity/time/serial/issuer/server metadata and signer purpose against applicable independent authorities, not signed strings alone.
6. Only after all checks and final Stop/lifetime/revision checks may existing candidate/material ownership accept the result. Existing transactional generation/store/leases remain the only import path. No CURRENT, grant or admission follows from validation.

No boolean trusted flag, arbitrary callback returning success, fixture SPKI, native token membership, digest or purpose enumeration is introduced as authority.

## Authority inventory

|Existing source|Factual capability|Why not delegated package trust|
|---|---|---|
|M0-M8 accepted M1/M4/M5|Requires independent signer authentication, exact administrative delegation, full SPKI reference, codeSigning and separate purposes|Normative obligations do not instantiate a trusted signer/context or approve a concrete record as delegation|
|Internal CA technical profile1.3/3.4|Defines dedicated package certificate purpose/key and administrative separation|A certificate chain or codeSigning EKU alone explicitly does not authorize packages|
|src/issuer/issuer_core.h PAPACC_ISSUER_PACKAGE|Represents issuer request purpose and records serial/certificate/state/policy/epochs|Issuing a purpose certificate is not evidence of independent package acceptance delegation|
|PAPACC_ISSUER_ROOT_PORT read/cas/copy_certificate|Same-root record/certificate access and factual issuer view, subject to proofs|No implemented package-validator acquisition of approved signer delegation, its scope/expiry/revocation or trust-reference lease|
|PAPACC_ISSUER_BACKEND authorize|Authorizes explicit issuer prepare/reserve/sign/query/revoke/recover operations against a supplied root view|Must not be reinterpreted as package-import signer approval or called with invented action/context|
|server_issuer_cng_backend_win32.c binding/auth|Compares actual issuer record/root proofs and delegates administrative issuer authorization|Private CA signing/issuance guard, not a public package trust source; PACKAGE does not permit reusing CA key for package bytes|
|manual_package_crypto.h mp_crypto_verify_package|RSA/hash/exact SPKI/domain verification against caller-supplied independent approved bytes|Caller trust acquisition is a documented prerequisite, not implemented by passing bytes; MP_CRYPTO_ONLY is not trust|
|server_security_source_win32.h environment_resolve|Explicit LAB channel/client material source and enrolled-client fingerprint|TLS/client trust is a different purpose. No package signer delegation, no automatic reuse of channel/client/CA credentials|
|Browser existing credential store/candidates/leases (read only)|Existing material import, validation and generation/lifetime convergence|No named delegated package signer authority found in inspected core/client/store paths; advanced import must not be weakened or treated as package trust|

Searches covered src, apps/server, applicable accepted ADRs and relevant capability records, plus read-only Browser client/store paths. This is a bounded factual inventory, not a claim that no possible future source exists anywhere. None of the inspected sources supplies the required live delegated context. The owner's earlier CURRENT-positive environment and administrative qualification gates remain pending; no store/authority is manufactured to fill them.

## Minimum focal handoff

Use the SAME administrative root and its existing issuer/policy/snapshot ownership. Identify the exact approved administrative action and record that delegates package authentication to a dedicated signer, additional to certificate issuance. The applicable record must bind exact full SPKI/certificate, issuer/generation, administrative scope, validity and revocation/status to factual revision and protected lifetime. The existing issuer PACKAGE record is a candidate source of public certificate/history, not automatically the delegation. No new fields, codec bytes, counters, registry or journal are selected here.

Before source integration, resolve whether an existing approved record/action actually supplies those facts. If existing representations are insufficient, prepare the smallest exact same-root normative/persistence extension for owner and executor approval; do not implement a hypothetical trust store/provider or silently reinterpret purpose/state. An independently authenticated first-Browser fingerprint ceremony is allowed by M1 but is not implemented merely by providing a string. Its confirmation/purpose/delegation provenance and lifetime need their actual governed binding.

Consumer contract must reject missing/expired/revoked/ambiguous/mismatched delegation, obsolete applicable context, lost pin/lease, Stop or uncertainty before KDF. A historical certificate or pin does not create CURRENT authority. Certificate status and delegation revocation are distinct checks. Never infer an active-operation revocation/Stop policy where the accepted policy is pending.

## Lifecycle and release proof obligations

Current canonical mp_package_validate performs envelope inspection and returns MP_CRYPTO_UNAVAILABLE for otherwise valid framing. It does not accept a password or allocate/decrypt confidential content. It remains unchanged. The low-level adapter is isolated, serialized and synchronous: its caller must keep all inputs/context/key/quarantine alive until factual return, observe Stop without closing live provider state, and wipe only after quiescence. PBKDF2 is not internally cancellable; no claim of owner Stop responsiveness follows from pre/post checks.

Future whole-result quarantine must span AEAD, ZIP/TLV, material/profile, authority and final checks. mp_crypto_unseal copying plaintext after tag success is a low-level crypto output, NOT permission to expose it as validated package data. Hold it as operation-private staging through remaining checks. Borrowed ZIP/TLV slices remain under that same owner. Retained/unknown completion keeps all buffers, handles and credits; timeout/Stop/process exit cannot prove destruction. No new worker, owner, retry or retirement system is allowed. These integration obligations are unimplemented, not declared proven by primitive tests.

## ED11 reconciliation

No source/storage/runtime linkage change: new product fixed/transient/retained graph bytes0. Known selected base2103847 and legacy2067655 remain accounting facts, not complete activation proofs. Only explicit Windows profile allows3145728; default/legacy2097152 and individual quotas unchanged.

Prior real-test CRT peak1290354 includes the PUBLIC TEST ONLY global signature fixture context. The private adapter/material phase reached569962 before that context. These observations are not a complete operational provider graph. Shared allocations must be attributed once; no shared cache or mutual exclusion may be presumed. Conservative base+complete-test peak3394201 exceeds3MiB by248473 before caller buffers. Do not call this a measured production peak and do not discard it to claim available credit.

Still account immutable outer/ciphertext, signature/key/cert/provider state, password/KDF outputs, exclusive quarantine, ZIP/TLV borrowed views, owned validated-result material copies, old/new generations, callbacks/stack, Stop and retention. Per-array caps (outer51042, archive49654, material49152; generic ciphertext/quarantine65536 when instantiated) are not separate reservations or automatic simultaneous sums. Full actual coexistence, provider/global fixture separation, same-root live composition and credit admission must be proved. Provider/kernel and full stack remain UNKNOWN. Full integration budget DENY, ED11 total PARTIAL. No reservation is reduced, omitted or moved outside accounting.

## Validation scope

Debug/Release existing component build/check and tests rerun successfully; build system reported no work to do, no new source compiled.1155 real-crypto assertions each remain scoped to primitives/public fixtures. No new functional patch was submitted/applied because the task's trust/order/credit gates are unresolved. No network, operational package, key/certificate generation or productive private signature occurred. The existing test performs only its already approved in-memory private signature with a published upstream TEST ONLY key, never a delegated production signer.

|Requested case|Evidence now|Whole authenticated package conclusion|
|---|---|---|
|Valid package with delegated trust|No factual approved delegation/context; no full positive test|NOT_RUN, DENY; no synthetic authority substituted|
|Package without trust|Canonical validator remains CRYPTO_UNAVAILABLE on structural fixture; component test confirms it|DENY; no claim of completed trust-aware orchestrator|
|Invalid signature/unknown signer|Existing mathematical-domain and expected-SPKI negatives pass|Primitive evidence only|
|Wrong password/tag/cipher corruption|Existing real KDF/AEAD/byte mutation tests pass|Primitive evidence only; not full material approval|
|Certificate/key incompatible|Existing correspondence/truncation and negative new-profile tests pass|No positive complete profile1 validation qualified|
|Approved identity mismatch|No integrated independently approved metadata/source binding|NOT_RUN for full pipeline|
|Corruption/version/truncation|Existing framing/crypto negatives pass|No success inferred from partial validation|
|Stop/retention|Existing synchronous pre/post Stop/quarantine clearing checks pass|Owner-thread Stop, pending/unknown retention and complete-result release NOT_QUALIFIED|
|ED11|Existing measured counters/bounds reviewed; unchanged product graph|Activation DENY; no reservation or total qualification|

No normative authority or operational status is changed by this handoff. This document is factual review and pending implementation guidance only.
