# Manual client package and issuer contract — PROPOSED revision 1

Date: 2026-10-18. TASK=PACC-PHASE6-MANUAL-CLIENT-PACKAGE-AND-ISSUER-CONTRACT-1.
This is an exact proposal for owner review, not an accepted ADR, implementation authorization, issuance authorization or product capability. Concrete choices below require acceptance before dependent code. No certificates, credentials or requests were created.

## Authority and factual state

Accepted constraints: [provisioning decision](../../adr/DECISION-production-client-provisioning-20261018.md) items 1–8; [ADR-0006](../../adr/ADR-0006-perfil-de-transport-security-e-credenciais-do-secure-principal.md) Trust model, endpoint key ownership, credential != Principal, certificate clock, first-use trust and secrets; [ADR-0028](../../adr/ADR-0028-modelo-ortogonal-de-admissao-autenticacao-e-policies.md); ADR-0012 immutable Session snapshot. [Earlier reconciliation](../production-client-provisioning-consolidation-20261018/RECONCILIATION.md) A1–A12 remains the authority/evidence map. Browser ADR-0007 sections Minimum Secure Provisioning R1 and Production Secure Provisioning: two official modes require convergence on existing CredentialGeneration/Reference/store/leases. Model E and C2 specification §11.2/§11.3 remain unchanged.

Current issuer: NOT_ESTABLISHED. The source `apps/server/server_security_source_win32.h` explicitly identifies a lab-only configuration source and single enrolled leaf fingerprint. It is neither a CA nor a productive multi-client issuer/registry. Existing root transaction only supports first dormant commit, not live client addition/revocation. Neither fixture CA, server leaf certificate nor Site TLS SYSTEM trust supplies issuance authority.

Manual generation/import cannot be declared implementation-ready merely because this proposed format is specified. Operational issuer selection and live publication remain blockers; client-side request/key preparation is also missing in the Browser. All automatic bootstrap/code-ceremony decisions stay deferred.

## D1–D7 disposition

The exact unresolved content originates in the earlier reconciliation, table “Somente decisões concretas ainda faltantes”. No accepted invariant is reopened.

| Decision | Exact open content / existing constraint | Manual impact and smallest proposed resolution | Automatic impact / blocks generator? |
|---|---|---|---|
| D1 | Who confirms code; request/key/server transcript binding, relay/replay/cancel/re-pair and pending recovery; contextual single-use code already required | No automatic code ceremony in manual mode. Administrator independently verifies recipient CSR/public-key digest and expected server/signer through trusted transfer | Ceremony remains open for automatic. Does not block manual serializer; recipient verification does |
| D2 | Verifiable precredential bootstrap; no silent TOFU or weakened operational mTLS | Manual first trust is explicit independent verification of package signer SPKI digest, expected server name and selected channel anchor before any trust installation | Network bootstrap remains open. Manual trusted-channel verification must be accepted; automatic bootstrap does not block manual |
| D3 | Encoding, authenticity/verifier, recipient/server/request identity, validity, duplicates/replay, bounds and secret handling | Public-only PCP1 envelope below; locally held key matched to CSR/SPKI, signed exact bytes, no key export | Direct blocker. Same public issuance facts may be reused by automatic later; its wire is not selected |
| D4 | Approved issuer, CSR/PoP, subject/SAN/EKU/key algorithms, custody, operators and references; endpoint key ownership already required | PKCS#10 DER signed CSR, externally administered existing approved issuer, explicit issuer/signer bindings, no CA key in runtime; approval and certificate mapping controlled by same admin root | Direct blocker: actual issuer/signer and policy selection still absent. Same issuer/registry required for automatic; no separate manual CA |
| D5 | CLI/GUI callers, permissions, durable live publication and factual status | Proposed CLI below; authenticated local admin frontend to same root, compare-and-commit revision, status/recovery by request. No restart and no dormant-precondition bypass | Direct blocker to functional generator/admission, not pure serializer. Automatic approval UI/API remains deferred |
| D6 | Concrete expiry, attempts, pending/concurrency/replay bounds and ED11 costs | Structural bounds below are proposed parser limits only. Explicit signed package deadline supplied per authorized request, capped by approved issuer/admin policy and certificate validity; missing limit/clock denies. No universal duration invented | Parser bounds need acceptance; deployment validity/record capacity/credit need selection before issuance. Automatic rate/code/pending defaults not resolved here |
| D7 | Persistent revoke, checkpoints/ack and disposition of active Sessions/ops, recovery, leases | Package cancellation cannot revoke a committed credential. Revoke new authentication through same persistent registry; active-session effect explicitly remains unapproved and command returns policy-required before pretending completion | Does not block public serialization; blocks claims of complete revoke/product lifecycle. Never infer active kill/cleanup or choose it silently |

## Recommended custody model

A — preferred and proposed for PCP1: generate/store private key on client through approved backend, emit PKCS#10 CSR, verify its signature as possession evidence for the included public key, independently approve recipient and request. Signature possession does not prove device/user identity or freshness alone. Administrator's durable approval binds exact CSR digest, server, recipient, issuer and policy revision; prevent CSR substitution and duplicate issuance with request journal. Client import verifies possession/correspondence again with its locally held key.

B — server-generated private key in protected package: OUT_OF_SCOPE / NOT_AUTHORIZED. Encryption alone does not authorize server custody or export. It needs a separate explicit endpoint-key exception, authenticated encryption/KDF/recipient-key contract, custody/deletion/recovery and legacy-target proof. No password container, PKCS12, server-generated key or permissive fallback is included in PCP1. Existing granular import remains available, without asserting local CSR generation already exists.

## PCP1 exact envelope proposal

Purpose: one public manual response package for one approved client CSR and one server endpoint. No archive, compression, nested file paths, executable content, network references to fetch, passwords, bearer tokens, grants or private keys. File extension `.papacc-client`; extension alone is not detection.

All integers unsigned big endian, no native structs/padding. Header: magic eight bytes ASCII `PACCMP01`; u16 version=1; u16 flags=0; u32 total file length including signature; u32 field count=21. Then exactly 21 TLVs in ascending tag order, each u16 tag + u32 byte length + value. No duplicate/optional/unknown tags, no trailing bytes. Final 256-byte RSA signature. Reject malformed sizes/unsupported version/flags/overflow before allocation. Signature suite fixed by version: RSASSA-PKCS1-v1_5 SHA-256 using RSA2048 package-signing key. This crypto choice is PROPOSED, not an inferred existing PST or issuer API; no SHA1/algorithm negotiation. Reuse vetted existing crypto provider primitives, never handwritten RSA/padding or new PST APIs.

| Tag | Value and exact meaning |
|---|---|
| 1 | SHA256 of exact original CSR DER, 32 bytes; request content reference, not a new operational ID or authorization |
| 2 | SHA256 of issuer's canonical approved public binding document, 32; requires existing administrative verification, not self-authentication |
| 3 | SHA256 of package signer SPKI DER, 32; compare to independently approved signer; signer public key is field21 |
| 4 | ExpectedServerName ASCII DNS name, 1..253 bytes; lowercase LDH labels1..63, no wildcard/URI/NUL/trailing dot/empty label; same exact name used for channel verification |
| 5 | Endpoint port u16, nonzero; destination metadata, not trust |
| 6 | Channel trust anchor one X509 DER certificate, 1..16384 bytes; bounded to current single-anchor material interface; not Site TLS trust |
| 7 | Channel server leaf certificate X509 DER, 1..16384; identity evidence to verify name/chain/validity, not signer identity or permanent no-rotation leaf pin |
| 8 | Client leaf certificate X509 DER, 1..16384; returned SPKI must match approved CSR and local key |
| 9 | SHA256 of recipient SPKI DER, 32; exact key binding, not Principal/user identity |
| 10 | Recipient display label UTF8, 1..128 bytes, no controls/NUL; presentation only, never sufficient recipient check |
| 11 | Existing configuration domain identity, 32 bytes, exact committed provenance |
| 12 | Configuration revision u64, nonzero; actual committed revision |
| 13 | Existing policy identity, 32 bytes; administrative assignment reference, never policy executable content |
| 14 | Policy revision u64, nonzero; actual committed snapshot revision |
| 15 | issued_at UTC seconds u64 |
| 16 | import_not_before UTC seconds u64 |
| 17 | import_expires UTC seconds u64; issued_at <= not_before < expires; all inside required certificate/signer validity intervals |
| 18 | ALPN exact eight ASCII bytes `papacc/1`; no NUL or length prefix inside TLV |
| 19 | transport profile u16=1 meaning TLS1.3 mTLS, no fallback |
| 20 | custody model u16=1 meaning CLIENT_LOCAL_KEY; reject any other value |
| 21 | Signer SubjectPublicKeyInfo DER, 1..1024 bytes; RSA2048 exponent65537 only; externally approved SPKI digest must match tag3 |

Signature input is ASCII domain separator `PAPACC-MANUAL-CLIENT-PACKAGE-V1` followed by one zero byte, then the exact header and all TLVs as serialized. The signature excludes itself; total length still includes its fixed256bytes. Whole-file SHA256 is a transport/audit digest, NOT a substitute for signature or independent trust. Signature certificate/CA selection is in approved issuer binding, not delegated by an arbitrary file-supplied key. Verify signature before displaying untrusted content as approved or using embedded trust. Unknown signer/invalid clock => failure, not trust installation.

Proposed total cap65536bytes; each certificate16384; signerSPKI1024; CSR16384 on issuer input. These are restrictive new format bounds for acceptance, NOT measured ED11 capacity or existing CS_LIMIT (currently1MiB/material). They exclude some deployment chains: PCP1 deliberately supports the current one-anchor/one-client-certificate interface; unsupported chain requirements fail, never strip intermediates or silently substitute trust. Subject/SAN names and issuance policy must be selected by issuer policy, not copied from unchecked CSR extensions. No new Principal field is needed in package: stable Principal mapping remains authoritative server-side, not a Browser declaration. No Session, grant or operation epoch is exported as a future authority. Configuration provenance cannot make a stale package CURRENT.

## Validity, import and recovery

Request states proposed: PREPARED -> APPROVED -> ISSUANCE_PENDING -> ISSUED -> PUBLISHED -> EXPORTED; CANCELLED/REJECTED are prepublication terminals. Uncertain issuer or commit result becomes RECONCILIATION_REQUIRED, no second issuance. State journal is part of same administrative configuration authority; no shadow allowlist. Issuer correlation and CSR hash identify retry; exact bytes of an already published valid package may be re-exported, never silently reissued/extended/rebound. Expiry/revocation prevents new positive export.

Import verifies bounded encoding/signature/independent signer/server confirmation, plausible local wall clock, certificate and package validity, issuer/recipient correspondence, local CSR and key, trust purpose and policy provenance. Current R1 material validator checks framing/CA/key correspondence, NOT all of these assertions: a passing R1 validator is insufficient package authentication/chain/revocation proof. No external clock comparison is used. An offline importer cannot know later remote revocation: label that fact UNKNOWN and rely on live server authorization to deny revoked credentials; never advertise offline import as fresh remote approval.

Duplicate key: committed server identity + CSR digest + SHA256 of exact package bytes. Exact duplicate of active import is ALREADY_IMPORTED without another generation; same request with different bytes requires explicit reviewed replacement, never overwrite. Preserve replay tombstone/provenance in existing configuration-generation metadata under its gate; bounds and retirement must be costed. One successful local commit consumes the pending request; lost/uncertain commit requires readback of ActiveConfiguration and metadata, not reissue. Copying public package does not transfer the local private key. Reimport after local removal requires a new explicit workflow; cannot erase replay history to bypass it. Cross-device global single-use cannot be proved offline; server enrollment/registry governs credential authorization.

Cancel before ActiveConfiguration flip preserves previous committed generation, clears pending secret buffers and retires only own unconsumed candidates by existing API. Commit flip is the publication point; after-flip cancellation is a new explicit configuration transition, not rollback of history. Crash recovery uses existing generation metadata, lease and retirement gates. Never delete credential material with surviving borrowers or use timeout/process exit/remote FINAL as destruction proof. Stop and all Model E rules remain unchanged.

Administrative cancel before issue stops pending request. After possible issue, cancellation reports uncertain/issued state and requires issuer reconciliation or revocation; deleting an export is not revocation. After commit, persistent revoke is a separate authorized operation. No automatic retry that generates a new cert/serial/package until PASS.

## Issuer contract proposal

Select an existing approved external/enterprise CA or existing managed issuer operated by authorized custodian. No existing concrete operational binding was found. Managed means an adapter to that selected issuer under the same admin root, not a new CA or CA key copied into service credentials. Server leaf, client leaf, package signer, inbound channel anchor and origin SYSTEM trust have separate purposes and approval; none automatically delegates another.

Approved issuer binding must supply authority/trust-domain identity, public CA certificate and allowed chain model, administrator/issuer operators and permissions, CSR verification algorithm allowlist, client subject/SAN and EKU clientAuth policy, keyUsage constraints, serial uniqueness responsibility, validity ceiling, clock authority, revocation/renewal/status interfaces, idempotency lookup and protected signer/key reference. CA key remains outside normal Accelerator runtime. Package signer may be an existing administratively delegated RSA2048 signing credential of the selected authority with appropriate digitalSignature permission; it is not automatically the CA or server TLS key. If no such signer exists, issuance/credential creation needs its own specific approval; no signer creation is authorized here.

Private adapter contract, proposed semantic operations: validate_request(exact_CSR, approved_context); issue_or_lookup(request_digest, approved_context); query_status(issuer_correlation); revoke(certificate_reference, reason); sign_package(exact_bounded_bytes, authorized_snapshot). Returns factual success/failure/UNKNOWN plus public certificate/correlation/status; UNKNOWN preserves issued resources and denies publication. PKCS10 signature proves key possession, not administrator approval. Validate returned certificate SPKI, issuer path, clientAuth, names, serial, times and approved policy before committing credential mapping. Issuer may not change key/subject/validity silently. For v1 propose RSA2048/e65537/SHA256 CSR to reuse R1's qualified RSA correspondence path; no new key-generation qualification is claimed. Other algorithms require explicit future compatible profile.

Issue only after admin approval of exact request; live registry publish only after real root transaction. Publish failure after issuance records issued-but-unpublished, denies authentication, and reconciles/revokes via issuer; never claims issuance rolled back by memory cleanup. Root publishes mapping credential/device -> stable Principal and policy with factual revision/epoch, not fingerprint-as-Principal. No approval implicit grants. Manual and future automatic callers share this adapter and registry.

## Proposed administrative CLI (not implemented)

Local authenticated/authorized admin frontend; no unauthenticated new endpoint/listener. Every mutation requires exact expected current revision, scoped permission, durable operation audit and rejection on stale writer/Stop. Package issuance/signing keys are referenced through protected issuer interfaces, never command-line raw keys/passwords. Audit records public digests/revisions/outcomes, not CSR proofs, tokens, private material or sensitive payloads.

```text
papacc-admin client create --csr client.csr.der --recipient-label DEVICE --issuer-ref REF --policy-ref REF --expected-revision REV --not-before UTC --expires UTC
papacc-admin client export --request-sha256 HEX --out new-client.papacc-client --expected-revision REV
papacc-admin client list [--request-sha256 HEX]
papacc-admin client cancel --request-sha256 HEX --expected-revision REV
papacc-admin client revoke --client-ref REF --reason CODE --expected-revision REV
```

create validates and journals before invoking issuer; only returns PUBLISHED when factual durable live publication is proved. Pending/unknown is explicit. export uses existing issued/published request, CREATE_NEW destination, checked full write/flush/close; partial/uncertain export never reports success or causes new issuance. Re-export is exact-byte idempotent. list redacts secrets and distinguishes pending/issued/unpublished/current/revoked/uncertain. cancel and revoke follow the distinct semantics above. Revoke command must report ACTIVE_SESSION_POLICY_REQUIRED until D7 is approved where needed; cannot return success for an unspecified active-session disposition. Proposed stable outcomes: SUCCESS, DENIED, INVALID_INPUT, STALE_REVISION, PENDING, RECONCILIATION_REQUIRED, UNSUPPORTED, RESOURCE_DENIED; numeric exit mappings deferred to CLI implementation contract before code.

The root's current first-commit APIs cannot implement these live commands. Implementing same-root bounded live preparation/publication and recovery is a real prerequisite; no service restart, direct store edits or second owner is a substitute. Does not require deciding automatic code ceremony/auto-approval UX now.

## ED11 and acceptance boundary

No runtime storage added by these documents. Future package cap64KiB is not permission to allocate it outside quotas. Prefer bounded streaming over the same immutable held input; if content must be reread, reject identity/length changes and verify exact bytes finally consumed to prevent verify/use races. Account streaming scratch, crypto contexts, parsed public certificates, local key material, R1 material staging, store copies, both config generations, issuer journal/replay records and all retained failures/concurrent writers. Avoid stack-sized package buffers. Existing CS_LIMIT1MiB per material is a validation limit, not a reservation. Quantitative scratch/journal capacity and compiled coexistence census remain required implementation gates; no quotas increased or C8 resource exception imported. Provider/kernel UNKNOWN, ED11 total PARTIAL.

Owner acceptance needed: D3 PCP1 fields/bounds/signature/trust ceremony; D4 selected deployment issuer/signer and CSR policy; D5 same-root local CLI/live transaction behavior; manual-only D6 limits/validity policy. D7 active lifecycle remains separately pending; full production provisioning cannot close without it. Accepting a proposed interface does not supply an issuer or authorize key generation, runtime admission or implementation.

IMPLEMENTATION_AUTHORIZED=NO
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION