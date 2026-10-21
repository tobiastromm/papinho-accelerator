# Manual package generator — authority reconciliation and implementation handoff

Task PACC-PHASE6-MANUAL-CLIENT-PACKAGE-GENERATOR-1,2026-10-20. main HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Baseline4687 files; prior manifest219/219 hashes match. No source, Browser, PST, wire or accepted decision changes. This task reaches the explicitly requested normative format gate before functional implementation.

## Exact blocking authority

DECISION-internal-client-issuer-and-manual-custody-20261018 clause9: “This issuer decision alone SHALL NOT accept or modify its proposed algorithms, KDF count, bounds or encoding.” Its Delimited precedence explicitly preserves package crypto/KDF/bounds/encoding as PROPOSED. DECISION-internal-ca-technical-profile-20261018 repeats: “Package v2 crypto/KDF/encoding/bounds are not accepted by this decision.” The normal product provisioning decision also leaves package format open. No later accepted package-suite/layout record was found in docs/adr.

The complete-manual v2 CONTRACT-PROPOSED.md remains the intended candidate. Its old custody GOVERNANCE CONFLICT has since been resolved ONLY by accepted manual exception clauses3–4; that does not accept its suite. Its older external-issuer and active Stop-on-revoke proposals cannot override the accepted internal-CA choice or POLICY_PENDING for active Sessions. Therefore no envelope writer/parser/KDF/password command or synthetic protected package was implemented. No build/test was run or claimed PASS for this task. Official approval of these PROPOSED documents will not confer technical acceptance.

## Conformance matrix

|Requirement|Authority/state|Disposition in M0–M8|
|---|---|---|
|Server generated client key, manual only|Accepted custody clauses3–4|Preserved; transient protected custody, no CA/server key export; automatic remains local|
|Internal issuer and dedicated purposes|Accepted issuer1/2/10, technical1.1–1.6|No external/fixture CA substitution; distinct package signer, codeSigning delegation and independent trust|
|Package AEAD/KDF/signature/version/ZIP bounds|Explicitly PROPOSED/excluded by clause9|Existing v2 suite reproduced for exact owner review; no crypto invented/implemented|
|Issuer binding digest|Older proposal says approved issuer binding SHA25632 without input domain|C1 proposes reuse of existing ca_spki_digest; new exact clarification needs acceptance, not inference|
|Serial encoding|Accepted persisted16 bytes; proposed metadata1..20 unsigned magnitude|C2 separates DER sign octet from metadata magnitude and checks the actual accepted profile|
|Expected server identity|Accepted S1–S5/root channel identity, serverAuth/SAN|C3/C4 require actual source; signed digest/name is provenance and never remote CURRENT or origin trust|
|UTF8 labels and import times|Prior proposal bounds but some exact predicates unspecified|C3 makes UTF8/no-controls/UTC units explicit; deployment TTL still requires explicit policy, not90-day default|
|Archive endian/offsets/authentication ordering|Proposed92-byte header and five STORE entries|M2/M3 and offsets/metadata tables; signature before KDF, AEAD before parsing; no extraction/compression|
|Browser simple/advanced convergence|Accepted issuer9 and Browser existing store contracts|Single-package reader is new work; advanced import remains; one existing generation/commit/lease pipeline|
|Revocation and delivery|Accepted persistent server revoke, active policy pending|No Stop-on-revoke accepted; delivered is an administrative event, not import/possession/grants|
|CURRENT/CAS/pins/Stop/retention|Accepted root/C8, issuer/C2, Model E, ED08|No decode-to-RESOLVED promotion, stale/unknown fallback, early wipe/free or new owner|
|ED11 Windows3MiB|Explicit selected profile accepted; default2MiB|No new credit or quotas; static file bounds are not process headroom; total remains PARTIAL|

## Browser source evidence, read only

papinho_credential_import.inc currently imports three separate CA/certificate/private-key files, DER/raw Base64 and PKCS8v0. Its local material validator decodes CA/cert/key and checks public/private correspondence; it does not read PACCMP02/ZIP, authenticate package signer, perform the proposed KDF/AEAD, or establish full chain/time/EKU/replay semantics. A key-correspondence proof is not package authenticity or complete X509 policy validation. No single-package support is assumed.

Reuse papinho_credential_material_import/material_validate/material_release, papinho_credential_store_prepare, gate/candidate/Reference, papinho_credential_commit_metadata, ActiveConfiguration generation selector, candidate_committed and existing lease/finalization. Proposed package input may feed already-decoded material into that same pipeline; do not route to a second store or raw PST path. Historical advanced UI in credential_options stays available. A single new UI operation must provide independent signer/server confirmation, masked unlock, validation and explicit commit/cancel without exposing secret buffers.

Actual delta after acceptance: bounded portable envelope/ZIP/TLV data reader; purpose-specific vetted crypto adapters; quarantine/wipe ownership; independent signer-purpose trust reference; full certificate/profile/time/key/issuer validation; bounded duplicate/replay metadata in the SAME generation; one package-selection UI entry. Preserve VC6/C89/no mandatory CNG, existing reference generations/leases/atomic selector/stale writer/safe retirement. No NT4 or backend qualification follows from BearSSL headers. No Browser write was performed.

## Existing Accelerator readiness and CLI proposal

server_issuer_x509_win32 exposes public client/server TBS preparation and validation and a denied-until-qualified CNG signer adapter. It is not a package encryptor, package-purpose signer, client private-key exporter or actual issuer authority. The config CLI currently offers config validate/diff/show-effective/apply with authority gates; no client package commands exist. No caller boolean or command spelling can supply admin/issuer/CURRENT.

Proposed extension of the existing papacc_server.exe administrative dispatcher, NOT implemented:

- client package prepare --client-ref REF --recipient LABEL --expected-revision REV --not-before UTC --expires UTC: explicit admin generation/issuance/protection request; no password flag/ENV and no side effect without factual authority.
- client package status --request-ref REF: read existing public correlation/results only; distinguish pending/unknown/retained, never report package success without complete facts.
- client package validate --package FILE --signer-ref REF: offline bounded authenticity/decryption/material checks after the exact contract/backend is accepted; signer-ref must be governed, never invented from filename.
- client package delivered --package-ref REF --delivery-ref REF --expected-revision REV: record authorized delivery through the same root transaction; no implicit Browser import, device binding or active client grant.
- client revoke --client-ref REF --reason CODE --expected-revision REV: existing issuer/root transaction and persistent future-auth denial; active Session policy remains pending.

Names are interfaces for exact later implementation review, not operational commands. Audit only public actor/request/revision/reason/disposition. If existing records/codecs cannot persist delivery/package provenance, stop and submit a versioned SAME-root extension first; do not write reserved bytes, create a shadow registry or silently invent record states. Protected retention/output paths and secret input require explicit deployment policy and owned checked staging.

## Test handoff after exact acceptance

No source/crypto tests run in this documentation-only gate. Subsequent tests must include fixed synthetic public vectors (not authority), known-answer/cross-provider PBKDF2/AEAD/signature, signed-header/nonce/tag/SPKI corruption, wrong unlock, unknown suite/version/flags, malformed lengths and arithmetic, strict ZIP/TLV ordering/CRC/overlap/trailing bytes, divergent names/purposes/CA/key/serial/time, duplicate/replay, Stop during KDF/borrow/export, uncertain write/close, old/new generations, wipe at quiescence, no plaintext files/logs, denied backend/CURRENT and complete shared ED11 accounting. Real-key generation/private signing is outside this task, even if future codec tests use existing synthetic fixtures.

Do not claim wrong-password/valid-protected-package PASS by a fake hash or mock encryption. Distinguish standard crypto vectors, synthetic lifecycle tests, native APIs and actual operational issuance. All implementation hunks need exact preimages and official executor approval after owner acceptance of the concrete contract.
