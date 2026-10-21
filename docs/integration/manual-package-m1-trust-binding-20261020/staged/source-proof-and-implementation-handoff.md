# Source review, implementation boundary and regression evidence

Branch main; HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Previous61+23 manifest entries and577 source/build/ADR/capability preimages verified. No preexisting WIP modified. No functional patch submitted or applied.

The owner correction in this task closes the previous prompt-order discrepancy. M1 stays unchanged. The first remaining gate is NOT ordering: it is the absent factual package signer delegation source/binding. The focal proposal is prepared under task section4 and is PROPOSED only.

|Source|What exists|Missing fact|
|---|---|---|
|issuer_core.h PAPACC_ISSUER_PACKAGE|Certificate request purpose and existing record/history|Explicit delegation to accept manual package signatures|
|PAPACC_ISSUER_RECORD|Existing request, purpose, policy ID, issuer generation, certificate digest/size, serial, approved/published epochs, state|An accepted mapping from explicit policy approval to signer trust, independent of issuance|
|PAPACC_ISSUER_ROOT_PORT read/copy_certificate|Same-root public record/certificate retrieval contract|Concrete applicable delegated approval context, pin and trust lifetime exposed to package validation|
|PAPACC_ISSUER_BACKEND authorize / CNG binding|Issuer-operation administrative authorization against factual root/record|Package acceptance authorization and consumer trust; no issuer action may be repurposed silently|
|server_issuer_x509_win32.c|Public client/package purpose preparation|codeSigning cert preparation is not delegation or trust approval|
|mp_crypto_verify_package|Exact independent-SPKI/domain RSA public verification; CRYPTO_ONLY result|Independent source of approved bytes, profile/delegation/status/scope and held reference|
|mp_package_validate|Envelope inspection followed by CRYPTO_UNAVAILABLE|No authenticated orchestrator exists; no password/KDF/decryption is invoked on this path|
|Browser candidate/material/store/lease model|Existing import/lifetime convergence; M1 allows independent first-fingerprint/server confirmation|No implemented package-specific provenance/purpose/delegation/status binding was identified; Browser remains read-only|

The source inventory and accepted authorities from the prior handoff were rechecked, including issuer purpose/record/public certificate access, administrative auth and package verifier. No source supplying the missing delegation was found in that inspected boundary. No claim is made that a concrete signer exists merely because the norm requires one. No prior trust store, arbitrary callback or self-signed/file-supplied CA is introduced.

## Execution boundary

Per section4, preserve authenticated validation DENY and prepare the smallest focal decision. Dependent implementation is stopped before any presumed trust. DECISION-DELEGATED-SIGNER-TRUST-PROPOSED.md proposes the explicit same-root policy/issuer source and consumer-binding semantics for an exact owner decision. It deliberately does not assign identities, choose trust transport/freshness parameters, write policy bytes or reuse a purpose enum as authority.

An accepted explicit source binding is prerequisite to exact functional hunks. If chosen, read existing policy owner/codec and produce only the necessary source/persistence/lease changes, with separately accepted byte-exact storage extension if needed. No new registry/worker is required or authorized. External positive CURRENT and administrative proof stay pending as previously directed by the owner.

## Lifecycle and ED11

No new fixed, caller, transient, heap/stack or retained production storage was added: delta0. No runtime linkage, buffers, owner, pins or cleanup behavior changed. Existing primitive quarantine/cleansing remains intact; complete-result quarantine and owner Stop/uncertain-retention integration are unimplemented and are not declared proven.

Known selected base2103847 /3145728 and legacy2067655 /2097152 do not reserve activation credit. Prior measured full-test provider peak1290354 includes global state exclusive to the published signing fixture; earlier private-adapter/material peak569962 is not a complete positive signature/material runtime graph. Conservative selected base+full-test peak3394201 exceeds3MiB by248473 before caller buffers, but is not a measured deployed peak. Keep ED11 DENY until actual shared/owned provider/caller/quarantine/material/stack/retained coexistence is measured and credited. No omission, automatic sharing, lower reservation or new quota; provider/kernel UNKNOWN.

## Tests and their precise limits

Existing modern Windows Debug/Release component configuration/build checks passed with no work to do and no warnings; no new compilation/source patch. The unchanged component suite reran1155 real-crypto assertions per build, including native BCrypt KDF comparison, RFC AEAD, mathematical RSA domain/byte corruption, wrong password/tag/key/material truncation, primitive Stop and quarantine wiping. Its approved published upstream TEST ONLY private key signs only an in-memory public synthetic envelope; no productive signature, new key/certificate or operational package.

These are NOT tests of the unimplemented delegated-trust orchestrator. Requested valid trusted signature, valid-but-untrusted, revoked trust, integrated KDF call-count, whole-result release/retention and full ED11 admission are NOT_RUN, not PASS. Static inspection proves only that the unchanged canonical deny entry has no KDF path, not a positive integrated M1 pipeline. Trust fixtures must be isolated and rejected in product selection before future positive tests can be meaningful.

No network, Browser/PST/wire, CURRENT, signature authority, production0.6.4 or admission change. Historical reports remain untouched. Proposed acceptance is the next dependency; it does not itself enable use.
