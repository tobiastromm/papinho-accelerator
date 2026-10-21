# Compatibility, decisions and implementation handoff

Status PROPOSED. Date2026-10-18. Read [exact candidate](DECISION-TECHNICAL-PROPOSED.md). User authorized document preparation/review only for this continuation. No code/build/test/key/network action. Existing accepted CA/manual decision remains unchanged; these technical parameters need owner acceptance before dependent source.

## Evidence and compatibility matrix

| Selection | Evidence actually inspected | What it establishes | Required new proof |
|---|---|---|---|
| RSA2048/SHA256 root+leaves, direct chain | PST `docs/codex/phase-history/phase7-closure.md` Tested scope: RSA2048/SHA256, direct/intermediate chains, real NT4 SP6/NSS legacy evidence | Historical bounded certificate-family interoperability; report is API1.2/library0.3 history, NOT exact new PST065/API3 certificate qualification | Normal065 Browser x86/VC6/NSS + Accelerator x64/MSVC/OpenSSL, exact CA/server/client profiles, all negative purpose/name/time tests |
| Browser RSA key correspondence | `src/papinho_credential_import.inc`: BearSSL local SHA256 RSA sign/verify, CS_LIMIT1MiB per material, signature scratch1024 | Local framing/CA/key match support; NOT full trust/EKU/time/revocation or new issuance qualification | Exact DER/PKCS8 from chosen provider; malformed/tampered/mismatch/unsupported chain and expiry; no parser-acceptance-as-trust |
| serverAuth/clientAuth EKU separation | PST `docs/backend-nss.md` SS5 host server/client EKU negatives; existing interop/security matrices | Role-usage enforcement evidence in documented tested scopes, not blanket NT4/API3 proof | Crossed/missing/multiple/anyEKU rejection, client name not used as Principal, exact API3 peer facts |
| Package codeSigning leaf | Separate purpose is a new candidate; existing RSA verify mechanism only | No existing approved signer or package-import implementation | Exact delegation, independent signer fingerprint, TLS role rejection and codeSigning policy; do not reuse CA/TLS key |
| CNG persisted RSA CA | Local Windows SDK26100 ncrypt.h declares MS_KEY_STORAGE_PROVIDER, persisted-key, property/sign/free APIs and export flags | Compile-time native API availability on modern toolchain; no provider qualification or key operation executed | Exact OS/provider/version, returned exponent/key size, signing-only/nonexportable readback, ACL/SID actual token deny, persistence/restart/cancel/close results |
| Browser NT4 | Browser is consumer of DER and package, never CNG key API | No CNG deployment dependency is added to NT4; C89/VC6 importer remains same frontend/store/leases | Actual full build/runtime on NT4, package crypto and storage performance; do not infer current-host results |
| Serial16 + durable reservations | Existing root transaction demonstrates committed views/pins/epoch but first dormant commit only | Reusable authority/lifetime patterns, not issuer ledger/live-update implementation | New same-root versioned persistence/rollback-safe recovery/CAS, concurrent writer/crash/sign-unknown/exhaustion/restore clone fence |
| Nonexportable key backup | Proposed Windows encrypted full-system recovery baseline; no backup product executed/qualified | Explicit recovery scope and fail-closed dependency, no portable PKCS8 export claim | Select exact OS tool/version/media/procedure, isolated restore, actual key/cert/ACL/history/serial continuity and restore fencing |

No new crypto primitives. No direct OpenSSL symbol presumed merely because PST has libcrypto runtime: normal SDK exposes PST consumer APIs, not an issuer API. Candidate CNG/native-X509 adapter requires its own qualified binding, factual failures and reviewed DER/signature interoperability. Do not modify PST or handwrite missing crypto functions.

## Implementation responsibility and interfaces

Modern private issuer provider: purpose-authorized admin key create/open/readback/sign/close; receives public X509/CSR-approved inputs and emits public certificate/signature/status, not CA private bytes. Existing application root owns issuer participant and public credential/status mapping; operational handlers only see immutable public snapshot/Principal/policy. Local admin frontend context must enforce distinct actual token from operational service; if service runs as SYSTEM or a token that can use the key, a same-process function check is not isolation. Deny activation until identity/key ACL architecture passes. Native key name/security descriptor are not caller-provided arbitrary escape paths.

Certificate-building boundary uses vetted native/provider routines for TBSCertificate DER, extensions, signed output and verify; RSA CNG signing input must be exact SHA256 digest with PKCS1 padding, not undocumented cert-generation fallback. Key leases and native close receipts remain separate from PST Connection dispositions. Do not claim NCryptFreeObject return as PST DESTROYED. Errors retain owners/credits until valid closure path.

Root live-update work must bind durable serial request to current config and issuer content, preserve historical BODY versions, pin read-only operational status, and publish epoch only after durable commit. Never use a boolean test hook as commit authority or create separate root/allowlist/registry. Real registry/private-format integration must have exact bounded codec/capacity/ownership review; the existing8KiB store/three slots cannot be advertised as general persistent issuer history.

## Parts independent of these three parameters

These parts can proceed under separately applicable source/test authorization without assuming proposed technical values; none is implemented by this documentary task:

- Inventory/read-only preservation and scope checks; no issuance/key/store changes.
- Regressions of existing root denial paths, stale current pins, dormant/live exclusion, Stop and retention using current contracts and isolated existing tests; no new normative serial/profile/provider expectations.
- Purpose-neutral durable owner/borrower/close-receipt helpers and fail-closed issuer-unavailable result handling, if reused in current owner rather than a second issuer framework. They must not pretend to create/issue/publish/revoke or accept arbitrary callback authority.
- Compile-time ED11 census infrastructure using existing build recipes, and cost inventory for proposed structures; no allocation/reservation quota changes, no PASS for provider/kernel UNKNOWN.
- Proposed API/error/lifetime documentation and future negative-case matrix.

Existing implementation authorization is not proof that missing technical values are accepted. Avoid adding unused shadow registry or fake provider just to report partial source progress. It is legitimate for the productive issue/sign path to stay unavailable until dependent gates resolve.

## Parts that depend on exact acceptance

- X509 generation/validation policy, CA/server/client/signer signing profile and deployed chain binding.
- Serial encoding and allocator, persisted reservation/sign-result recovery, restart/high-water/clone-fencing rules.
- CNG named-key creation, key usage/export/ACL/security context and OS backup/restore strategy; no default credentials/accounts/paths generated.
- Same-root versioned issuer participant and durable publication/recovery with actual metadata schema; never reserved-byte re-interpretation of BODY05.
- Actual issue_or_lookup/query_status/revoke/renew/sign_package crypto/persistence implementation, tested outcomes and live administrative publication.

Package codec/KDF/limits and active Session-revocation disposition have their own pending acceptance; this three-point decision does not unblock them automatically. Subject labels/device records never manufacture Principal. No real CA/cert creation or operational admission is authorized even by later code/test acceptance without its own authorization.

## Exact future qualification gates

1. Owner accepts exact three-section candidate; register designated accepted normative text without editing earlier history. Pin evidence/artifact/protection procedure and test-only output destinations before build.
2. X509 fixtures only: root/leaf positive, invalid signature/key/purpose/EKU/basicConstraints/pathLen/hostname/clock/validity, self CA separate from leaf keys, full DER lengths and PKCS8 matching; normal SDK targets, real VC6/NT4 scope explicit.
3. Serial model and native persistence: competing threads/processes/stale expected revision, crash before reservation flush/after flush/before sign/unknown sign/before result commit/after publication; exhaustion, duplicate request, revoke/renew linkage, corrupted/truncated history, rolled-back restore and cloned issuers. No serial reuse or automatic retry while uncertain.
4. Protected keys: explicit admin init/no startup generation, token ACL denied for service, exact nonexportability/sign-only property checks, cannot use CA as TLS/package key, restart open/mismatch, protected consistent backup+restore, authority rotation and old-view retention. Only isolated synthetic/test authority, never productive store.
5. Lifecycle/ED11: Stop and cancellation at each owned step, native key/file close failures reported separately from synthetic injected failures, no double release/early reuse, retained credit and coexistence. Actual size/stack/crypto-provider/staging/journal peaks before quota claims; UNKNOWN remains denial.

No network/TLS session required for documentary preparation. Later mTLS target qualification is separate authorized execution. Keep production064, candidate065 normal and R2 TEST_ONLY isolated. Six provisioning gates OPEN; no new roadmap or PapinhoTrustStore dependency.