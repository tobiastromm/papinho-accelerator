# Internal issuer contract and implementation handoff — PROPOSED

Read DECISION-PROPOSED.md and OWNER-REQUEST.txt first. No implementation or key generation authorized. Exact proposed package v2 algorithms/bounds/encoding remain unchanged; future implementation requires normative acceptance and provider/target evidence. Operational internal CA does not exist merely because source supports crypto or root commits.

## Minimum component and ownership

One private administrative issuer participant under existing application/root authority, not another registry service, TLS listener or runtime owner. Existing root persists authority/client/certificate/revocation metadata through bounded administrative generations. A purpose-restricted signer adapter holds the dedicated CA key in protected administrative storage; operational authentication consumes read-only public CA/status/config snapshots, never CA private key access. Private key references/leases remain tied to durable owning context and quiescence, not a borrowed thread stack. Do not invoke keygen/storage/signing synchronously in the active I/O owner as an unbounded operation.

Reuse modern Accelerator crypto provider integration for CSPRNG, keygen, certificate encoding/signing, hash and protection after exact artifact/API qualification; do not assume PST exposes a certificate issuer/keygen API or modify PST. Existing `server_security_source_win32` is lab credential resolution and lease plumbing, not productive CA implementation. Existing `server_configuration_root_transaction_win32.inc` and participants supply first-commit/pinning patterns, not live reload or an issuer. Extend the same authority deliberately; do not copy fixture cert-generation scripts into startup.

Authority public record: stable administrative identity; key-generation identifier; CA certificate DER/fingerprint and SPKI; approved issuer policy revision; key-provider/protected-object reference (no raw key); next serial/high-water state; valid-from/to and rotation state; journal/checkpoint/revocation revisions; backup/restore generation; factual dispositions. Administrative identity is an issuer record, not a Principal or certificate digest pretending to be a Principal. Certificate record: issuer identity+key generation+serial, exact public cert DER/hash, public-key digest, stable client/device reference, Principal mapping, policy assignment, issuance request correlation, validity, predecessor/successor, revocation reason/time/revision and publication/reconciliation state. No private client key/password in public record.

## Certificate profile and serial contract

Dedicated CA key and certificate; CA BasicConstraints critical CA=TRUE, pathLen0 for initial direct-client profile; CA KeyUsage critical keyCertSign+cRLSign; not a serverAuth/clientAuth leaf or TLS credential. Client leaf CA=FALSE, clientAuth EKU, digitalSignature KeyUsage, subject/device naming controlled by administrative policy (CSR extensions are requests, not authorization), appropriate authority/subject key identifiers. Stable Principal lives in verified administrative mapping; CN/SAN/serial/fingerprint never becomes the Principal by convention. Returned key and certificate must match; automatic CSR signature/proof checked, manual public request originates from protected generated client key.

For manual leaf/key and package signing use the unchanged package contract candidate choices. CA key/signature algorithm, provider object protection and certificate validity ceilings require explicit approved issuer policy before initialization; no fallback to backend defaults or invented CA lifetime. They need not equal the package encryption/signature algorithms. Missing policy => NOT_READY, not implicit generation. Wall clock must be trustworthy for certificate validity, monotonic local durations for operations; no process-clock comparison or disabled time validation.

Proposed exact serial allocation: positive X509 serial, <=20octets, encoded canonically; issuer-generation prefix8bytes with high bit clear and nonzero, generated once at explicit initialization, followed by u64 nonzero durable counter, total16bytes before DER INTEGER framing. Allocate/reserve counter under same admin transaction BEFORE signing; burn reservations on failed/uncertain issue, never recycle. Exhaustion denies issuance and requires planned rotation. Pair issuer key-generation+serial is unique across records; certificate issuer identity disambiguates CA generations. Initialization rejects identity/prefix collision in its persistent authority history. Restore must reconcile the durable serial high-water and all later-issued evidence; if rollback freshness cannot be established, disable signing/rotate explicitly rather than risk reuse. No random serial per attempt used as a substitute for durable recovery.

Issue request journal is keyed by durable authorized administrative request/correlation and exact public-key+policy digest. Signing outcome is recorded before successful client registration; issuer success and root CURRENT are separate facts. Repetition queries journal and returns the same certificate, pending or UNKNOWN; no new serial until explicit new request. Revocation is a monotonic persistent certificate status, not deleting the record; an old snapshot/export cannot resurrect it. Issuer-only revoked, deny-currently-published and active-session disposition are distinct receipts.

## Explicit initialization procedure — proposed, do not execute

1. Authorized installation administrator selects protected authority storage/object backend, existing root identity, issuer policy, actors allowed initialize/issue/revoke/export/backup/restore/rotate, validity and retention limits, audit destination and backup custodians. No install default creates a CA silently. Canonical concrete storage path and provider must be selected rather than invented from fixture locations.
2. Proposed `papacc-admin issuer init --policy-ref REF --expected-revision REV` displays plan and requires explicit admin authorization through protected local frontend. Durable intent and uniqueness claim precede key generation. Existing authority => report it, not generate another. Unknown/corrupt state => reconciliation required. No CA secret on CLI/ENV/log.
3. Acquire accounted administrative key reference; generate dedicated key through approved CSPRNG/provider, create own self-signed CA certificate, validate signature/key match/CA extensions/identity/clock; persist protected key and exact public record with verified write/flush/close receipts. Failed close retains ownership; no success receipt before resolution. No real action performed by this document.
4. Commit authority binding in same root transaction and publish CURRENT/epoch factually; denied admission persists. Key created but commit uncertain => orphaned-owned/PENDING reconciliation; never regenerate on service restart. Service start opens only selected existing identity, verifies protected-object/certificate correspondence and committed state; otherwise issuance disabled.
5. Separate explicit `issuer backup`, `issuer restore`, `issuer rotate` operations require protected operator authorization and audited receipts. Backup includes protected key, authority/certificates, serial high-water/reservations, issuance/revocation journal and consistent root reference; never public client package. A machine-bound encrypted blob alone is not a portable recovery plan. Select a vetted OS/provider protected-backup mechanism and custody policy; no custom encryption/KDF or plaintext backup is accepted by inference. Verify restore under controlled authorization before claiming recoverability.
6. Rotation creates a new explicit key-generation record in same authority/history; new issuance switches only at committed revision. Keep old public CA/records/status and any required old signer references through approved validity/revocation/lease retention. Distribution of replacement client trust requires independent approved trust workflow, never automatic trust just because a root changed. Root compromise requires incident/revocation/trust replacement, not silent renewal.

## Purpose-specific replaceable interfaces

Private semantic contracts, not new public APIs or an implemented PapinhoTrustStore. Common inputs: authorized actor/context, existing root/current pin, expected revision, immutable request correlation, approved issuer policy, deadline/cancel and charged owner. All return status+public factual receipt+owned references as applicable. UNKNOWN or retained disposition never authorizes publication/release.

| Boundary | Contract and separation |
|---|---|
| Key protection | create/open/borrow/return/finalize protected administrative key, approved backup/restore; no raw private material passed to TLS/runtime or client |
| Issuance | prepare/reserve serial, sign-or-query exact request, validate returned cert, reconcile; supports manual-generated public key and automatic CSR without two registries |
| Signing | Purpose-checked certificate/CRL signing by CA; package signing by explicitly delegated existing/new approved leaf reference; CA key never automatically signs arbitrary package bytes |
| Validation | Validate CA/client chain/extensions/key correspondence/time against explicit issuer binding; cannot approve policy, assign Principal or trust new CA alone |
| Revocation/status | Persistent idempotent revoke; query issued/revoked/expired/pending/unknown at a particular revision; CRL publication if policy selects it is separate from local authorization snapshot, no new OCSP/network service assumed |
| Trust | Public anchors, expected names and independently approved trust transitions; channel server trust, client issuer trust and Site TLS SYSTEM trust stay distinct |
| Rotation | Prepare new generation, publish explicitly, retain old facts/references and reconcile; never auto-remove still-valid issued records or ignore revocation |

Future PapinhoTrustStore may replace the trust/validation/rotation implementation or coordinate other providers, but it does not implicitly inherit CA private-key custody or become a Phase6 dependency. Migration imports exact issued DER/serial/authority/history and explicit trust decisions, not reissuance by reinterpretation. Different provider object handles need controlled rebind/recovery without changing public identity or losing revoked records.

## Package and channel binding reconciliation

The complete-manual v2 proposal requires its channel `ca.der` and client certificate to fit the existing supported direct-chain/single-anchor profile; internal client issuer does not automatically become the channel server CA. Two valid deployment choices must be distinguished: explicitly approve channel server leaf under the same internal CA (different server key, proper serverAuth/name policy, no CA-key reuse), or retain existing distinct server trust. The second may require a separate approved package/validator chain extension because v2 does not carry arbitrary client chains. Do not silently alter fields, trust semantics or certificate validation to make it fit. Record unsupported binding as DENY and present that exact delta for approval. No Site TLS origin trust alteration.

Package signer remains a purpose-authorized signing credential referenced by the admin authority, distinct from CA and channel keys. If none exists, explicitly provision a delegated signing leaf/key under approved policy after authorization; no real signing credentials are created here. Package v2 suite1 RSA signer, KDF, caps and restricted ZIP remain PROPOSED and unchanged. Internal CA selection does not itself accept those choices.

## Live publication and Session safety

Administrative create/renew/revoke and issuer status changes prepare against actual CURRENT, reserve bounded next generation, validate participant/key/status facts, durable commit, publish new CURRENT and factual epoch through the same root. Old immutable views and their leases persist until consumer quiescence. An issued-but-unpublished cert remains denied, not automatically enrolled by CA chain validity. Caller-selected epoch, direct persistent edits, forcing dormant guards true or second allowlist/registry are forbidden.

New authentication reads current verified credential/device mapping and revocation, with Principal resolved by existing authority and policy/grants independently enforced. Renewal does not add capabilities to existing Session snapshots. Existing Sessions/operations are NOT changed by this task; active revocation disposition/checkpoints/ack/bounds need a separate accepted policy. Report issuer revoke, publication deny and active-session handling separately. If requested scope requires unspecified active handling, return POLICY_REQUIRED/PENDING rather than false global success. Retention/no double release/no resurrection/no early reuse and exact close dispositions survive failure/recovery.

## Security and ED11 matrix

| Failure/risk | Required behavior / future proof |
|---|---|
| CA-key theft/operator misuse | Restricted separate signer/protected backend and scoped admin permissions, audit every purpose; no claim of hardware protection until selected/qualified |
| Startup re-creates authority | Missing/corrupt/denied open => issuer unavailable; offline regression proves no init side effect |
| Duplicate issue/serial after crash/restore | Durable intent+serial burn+idempotent query/high-water reconciliation; uncertain restore disables signing |
| Manual key compromise/copy | Explicit custody risk, encrypted package only, transient wipe, controlled retention, revoke/renew; no single-device guarantee |
| Wrong CSR/key/identity | Verify public key/CSR, policy-authorized naming, returned cert; mapping independent of fingerprint/label |
| Issue succeeds / publication fails | Preserve issued-unpublished receipt, deny runtime, reconcile/revoke under same authority; no automatic rollback fiction |
| Revoke with held Sessions | Persistent deny for applicable new authentication; active-policy pending; retain views/leases and original credits |
| Backup/rotation rollback | Verify full history/serial floor; keep historical CA/status and trust transition; no silent new identity or revoked resurrection |
| Retained signing/provider/storage | Durable owner/leases/callback quiescence and factual close result; no destructive retry or fabricated DESTROYED |
| ED11 | Price CA/client/signing key handles/crypto scratch, protected blobs/staging, certs, registry/journal/status/recovery, lease and generation coexistence; no quota increase; provider/kernel UNKNOWN |

## Minimum implementation handoff after exact acceptance

1. Register accepted scoped decision only after approval of exact text; reconcile explicit precedence with ADR-0006/provisioning decision, preserving original historical text. No edits now.
2. Select protected provider/key policy/backup backend, CA algorithm/validity, serial/history/retention caps and channel/signer bindings. No invented defaults; this proposal fixes responsibilities, not unapproved deployment credentials.
3. Implement same-root administrative issuer participant and durable status/serial/recovery; explicit init/backup/restore/rotate commands, then issue/renew/revoke/query and live publication. Existing first dormant commit is a factual limitation to address, not reuse as already-live code.
4. Integrate protected manual package generation and Browser import under their separate authorized slices. Browser-only edits go to its Codex with exact package handoff; preserve advanced import and all automatic local-key requirements.
5. After separate authorization, run offline init/no-reinit, key-match/CSR/profile, serial uniqueness/crash/high-water, duplicate request/uncertain signing, revoke/current stale-read, renewal/rotation/restore, Stop/retention/callback/Model E and resource tests. Separate synthetic failure fixtures from native storage/provider proof. No real CA/key/network or new tests executed here. Compile appropriate targets and census before PASS.

All six Phase6 provisioning gates OPEN. No new roadmap, collector, supervisor or PapinhoTrustStore implementation.