---
status: accepted
decision-date: 2026-10-18
last-revised: 2026-10-18
revision: 1
scope-level: project
scope-target: PapinhoAccelerator
decision-makers: [Tobias Tromm]
supersedes: []
superseded-by: []
---

# Internal CA technical profile 1 — X509, serial and protected key

## Authority and exact acceptance scope

Owner conditional acceptance: PACC-PHASE6-INTERNAL-CA-TECHNICAL-PROFILE-ACCEPTANCE-1, subject to exact review and official executor approval. This is the designated normative record, supplementing DECISION-internal-client-issuer-and-manual-custody-20261018.md. No older accepted file or original proposal is edited.

Source: [DECISION-TECHNICAL-PROPOSED.md](../integration/internal-ca-technical-profile-20261018/DECISION-TECHNICAL-PROPOSED.md), revision1, SHA256 cb5f4c480221eeec8c238192ba939a49c19c226b92a49832dc7ef5610aeb8b10. The three technical sections and eighteen numbered clauses below are reproduced exactly. Their original source headings/wording are retained for exact comparison; in this record they are ACCEPTED requirements, not proposal status. Source introduction, closeout and implementation handoff are not promoted wholesale.

## Accepted technical sections

## 1. X509 profile 1 — exact proposed text

1.1 The initial interoperable profile SHALL use X509v3 DER, RSA2048 modulus with exponent65537 for CA, channel server, client and package signing keys; certificate signatures SHALL use sha256WithRSAEncryption (PKCS1-v1_5 SHA256). No SHA1, smaller RSA, implicit ECDSA/PSS certificate fallback or shared purpose key is allowed. RSA certificate signatures do not replace TLS1.3 ephemeral key exchange. Modern Accelerator signing uses approved native/provider APIs; Browser uses its governed PST normal TLS backend and existing crypto validator. Selection rests on existing bounded RSA2048/SHA256 legacy evidence, not a claim of universal modern cryptographic adequacy. Algorithm migration requires explicit profile/version review.

1.2 There SHALL be one administratively initialized self-signed CA generation for profile1, no intermediate CA in this initial chain. Root BasicConstraints critical CA=TRUE/pathLen0; KeyUsage critical keyCertSign+cRLSign; no EKU. The CA key SHALL NOT authenticate a TLS connection or sign arbitrary package bytes. Root SKI and leaf AKI SHALL be generated consistently by vetted X509 provider routines; no application-invented hash/crypto construction. CA public record owns stable authority ID16bytes from CSPRNG and generation ID16bytes, distinct from root domain/Principal. Subject CN is ASCII `Papinho CA ` followed by lowercase32hex authority ID; renewal/rotation records distinguish generation/public key explicitly, not by CN alone.

1.3 Purpose profiles SHALL be distinct:

| Certificate | BasicConstraints | Critical KeyUsage | EKU (only listed purpose) | Name/binding |
|---|---|---|---|---|
| Channel server | CA=FALSE, critical | digitalSignature | serverAuth, noncritical | SAN dNSName exact approved ExpectedServerName; no CN-only fallback; SNI/hostname verification mandatory |
| Client | CA=FALSE, critical | digitalSignature | clientAuth, noncritical | CN `Papinho Client ` +32hex persistent device-record ID; no serverAuth/SAN-based authorization; stable Principal mapping separate |
| Package signer | CA=FALSE, critical | digitalSignature | codeSigning, noncritical | CN `Papinho Package ` +32hex signer-record ID; exact admin delegation for package purpose, not generic TLS/client authorization |

All leaves contain AKI/SKI, unique serial and explicit validity. No keyEncipherment required for this TLS1.3-only profile; no anyExtendedKeyUsage, mixed client/server EKUs or ignored purpose mismatch. Runtime role validation SHALL enforce purpose even if backend defaults are more permissive. Package signer delegation/independent full-SPKI verification is additional to certificate chain; codeSigning does not by itself authorize an admin package. Distinct signer key means no silent reuse of CA/server/client keys. CSR names/extensions are requests and SHALL be replaced/checked against approved administrative identity policy.

1.4 CA validity SHALL be exactly1825*86400seconds from explicit authorized creation time, subject to provider/DER date validity; leaf maximum90*86400seconds. No automatic backdating or clock-skew bypass. Leaf notBefore is factual authorized issue time or explicitly approved future start; notAfter is min(requested end, notBefore+90days, CA notAfter), must exceed notBefore and never extend issuer validity. Requests exceeding approved max SHALL return the selected interval for explicit admin confirmation, never silently issue a different requested lifetime. Begin planned CA rotation no later than90days before CA expiry; this is an administrative trigger, not permission to automatically initialize or replace trust. No leaf renewal happens automatically. Trustworthy UTC and checked arithmetic mandatory; unavailable/invalid clock denies issuance. Durations/Stop remain monotonic local facts. These certificate lifetimes do not establish package import TTL or Mode C evidence freshness/headroom.

1.5 To fit the unchanged proposed manual package v2 direct-chain/single-anchor path, a new profile1 installation SHALL explicitly bind its channel server certificate under this internal CA, with separate server key and serverAuth profile. Issuing that server certificate is a separately authorized administrative action, not client issuance/startup side effect; no real certificates now. If an installation preserves a different channel CA, profile1 manual package use SHALL be denied until an explicitly accepted chain/validator extension exists; do not substitute trust or alter v2 silently. This deployment binding does not merge channel/server/client/package trust purposes and SHALL NOT change origin HTTPS SYSTEM trust.

1.6 CA, server/client/signer leaf creation requires explicit authorized action and approved target/backend; this candidate does not authorize real material. Direct-chain RSA2048 is chosen to minimize legacy uncertainty. Existing historical PST NT4 RSA evidence is supporting compatibility evidence only; exact generated profile under normal PST0.6.5/Browser must still be tested before qualification.

## 2. Persistent serial and transaction — exact proposed text

2.1 Serial SHALL be16bytes: persisted eight-byte authority-generation prefix from CSPRNG with top bit clear and not all zero, followed by unsigned64bit monotonically increasing big-endian counter. DER INTEGER is positive/minimal; leading zeros removed only for canonical DER representation, not when reconstructing the persisted16byte value. Counter starts1 (CA self-certificate reserves1); client/server/signer issuance shares the same generation counter, next2. Maximum2^64-1; exhaustion denies, no wrap/recycle. Issuer generation+serial is the public uniqueness key. Prefix collision against retained authority history SHALL be rejected at explicit generation preparation; no global cross-CA identity claim.

2.2 Before ANY signature, existing administrative authority SHALL commit a reservation binding authority+generation, counter, exact public-key/CSR digest, purpose, approved policy and immutable admin request correlation. Single writer/CAS against actual root CURRENT revision and expected counter guards all processes, not just a thread mutex. Durable reservation success is required before signer invocation. Competing stale writer rereads/reconciles, never signs with a guessed serial. Root commit/epoch are factual; no caller-selected epoch or raw persistent edits. New registry/journal authority forbidden: reservation/issuance records are issuer data in the same root-owned configuration transaction and recovery chain.

2.3 Proposed request states: PREPARED, SERIAL_RESERVED, SIGN_REQUESTED, ISSUED_UNPUBLISHED, PUBLISHED, REVOKED; terminal ABANDONED before publication and RECONCILIATION_REQUIRED for uncertainty. A reserved counter is burned even if cancelled before signature. After SIGN_REQUESTED, failed/unknown return never releases counter or creates a new request automatically. Query exact durable correlation/public cert hash; return same cert if known, pending/unknown if not. No signing again unless recovery conclusively establishes no prior signature and retains same authorized record/serial under explicit recovery; default is abandon/burn and separately authorized new request. Persist and validate exact certificate DER/hash before publication. Revoke is persistent status, never record removal or serial reuse. Renew is a new key/request and new serial linked to predecessor, preserving stable Principal only through approved mapping.

2.4 Crash/restart SHALL recover only from verified same-authority root committed history, reservations, issuance results and high-water. Missing/unresolved/truncated journal, stale writer, mismatch or corrupt key/cert denies signing; do not regenerate CA or infer no issue from missing response. A software-only local backup does not prove freshness against rollback/cloned installations. Restore SHALL fence all old issuing instances and reconcile later exported/public certificates/reservations with a separately retained backup/administrative recovery checkpoint; if latest high-water cannot be proved, old generation signing SHALL remain disabled. Recovery may preserve validation/status of prior certificates and require explicit new generation/trust rotation; never resume from an older counter just because its file hash is valid. Concurrent restored clones may not sign under same generation. No wall clock/timeout proves uniqueness.

2.5 Existing BODY0.5/8192byte first-dormant store does not already persist issuer records or general live updates. Implementation SHALL extend the same authority with explicit versioned private issuer persistence/participant and bounded generations, not insert metadata into unused old bytes or create a shadow allowlist. Historical BODY versions remain unchanged; root snapshot references/protects exact issuer record revision/content and publication. Serial-reservation commit and certificate-current commit are distinct transactions with retained recovery linkage. Exact private codec/layout and compiled capacity must be reviewed against these invariants before write integration; they cannot be implemented by bypassing dormant preconditions. Issuance status may be public before runtime authorization, but PUBLISHED requires its factual root publication.

## 3. Protected CA key — exact proposed text

3.1 Initial modern Accelerator backend candidate SHALL be Windows CNG Microsoft Software Key Storage Provider (`MS_KEY_STORAGE_PROVIDER`), named persisted machine key, RSA2048/e65537, export policy0 (nonexportable through normal NCrypt API), signing-only usage. CNG headers/API availability are NOT qualification; approved target integration must test creation/property/readback/sign/close and actual denied export. No homegrown encrypted key file, raw CA DER key in BODY0.5, DPAPI blob mistaken for portable backup, TLS key repurposing or silent fallback KSP. OS key storage is software protection, not HSM/hardware isolation or defense against a fully privileged compromised OS.

3.2 Key name SHALL be `PapinhoAccelerator.CA.` +32hex authority ID +`.`+32hex generation ID. Provider key object and public root metadata SHALL be bound by authority/generation/SPKI digest. Explicit init uses CREATE_NEW semantics and checks key existence/correspondence; startup opens selected named key only in authorized admin context, never generates it. All NCrypt operations must check SECURITY_STATUS; failed finalize/close retains durable owner/error receipt, no blind deletion/retry. Key ownership/callback lifetime separate from public cert lifetime and PST Connection lifecycle; do not fabricate PST DESTROYED for native key close.

3.3 Administrator SHALL select explicit issuer-operator SID and distinct operational service SID. Protected key security descriptor grants minimum use to issuer-operator and required OS provider/system authorities; operational service SID receives no private-key use/export/open permission. Administrative ownership/ACL and provider export/key-usage properties must be read back and tested with actual tokens. CA private handle exists only in an explicitly authorized administrative execution context, with purpose-checked request and lease; operational handlers cannot acquire it, cache an admin impersonation token, elevate themselves or load a fallback key. Same-process function separation with the same access token does not prove this isolation. If existing admin command hosting cannot enforce distinct token/context and same-root transactional publication, issuer activation remains denied until that binding is implemented and tested. No second operational listener/supervisor/identity registry is authorized.

3.4 Initial public metadata/journal destination SHALL be explicit admin-selected absolute installation path under the canonical installation's administrative storage, validated for owner/DACL/reparse/file identity and attached to same root. KSP manages its native protected key location; application SHALL NOT manually read/copy its key files as a custom store. No secrets in config/manifests/CLI/ENV/audit. Audit records actor/request/purpose/serial/revision/public hash/disposition only. No CA plaintext export. Client key generated for manual package remains transient under accepted manual exception; automatic key remains local to Browser. Package signer protected key is separate and purpose-authorized; public issuer authority does not create arbitrary signing capability.

3.5 Backup baseline SHALL be an administrator-authorized encrypted offline full-system/bare-metal recovery image using an approved Windows backup/recovery mechanism that includes provider-protected machine key state, OS cryptographic protection/identity and consistent root+serial/status history. The CA key remains nonexportable through NCrypt; backup is protected OS recovery, not plaintext/portable PKCS8 or part of client ZIP. Require initial verified backup before enabling issuance; new committed signing/serial/status state requires durable archival/checkpoint protection before reporting recoverable. Baseline retention: two independently protected recovery generations, preserve newer until replacement verified; never evict sole recoverable generation. Exact filesystem/media capacity/reservations still must fit approved deployment storage policy; full images are not issuer heap buffers or ED11 admission credit.

3.6 Restoring an encrypted machine image is not automatically portable recovery or safe resumption of issuance. Test restore under authorized isolated target with key/cert/SPKI correspondence, SID/DACL/property readback, serial/revocation history, anti-rollback checkpoint and fencing. Missing decryptability/backup key/consistent high-water => issuer unavailable, no CA regeneration. Recovery media secrets belong to separate backup custodians, never runtime or package. If selected Windows backup mechanism cannot satisfy protected consistent restore, deny activation rather than add plaintext export. No arbitrary new backup cryptography/KDF is authorized; choose concrete OS backup artifact/procedure in deployment qualification.

3.7 Rotation SHALL explicitly prepare a new named key/generation and publish under same authority. Preserve old public certs/revocations/issued DER/history and key handles still required by retained work; no key deletion until lease/quiescence and approved retirement. Nonexportable key need not be migrated to another provider: future PapinhoTrustStore can consume old public validation/status/history and route retained old-key work to the original protected provider, while new issuance uses an explicitly rotated generation. No reissuance/reinterpretation of old certificates or loss of revoked status. Portable CA private-key migration is NOT claimed by this baseline and would require a separate approved protected migration policy.

## Owner conditions, scope and remaining qualification gates

Acceptance is normative, not proof of backend/target/backup qualification. RSA2048/SHA256 roles, KeyUsage/EKU/BasicConstraints, separate CA/TLS/client/package keys, persistent serial reservation and CNG protected admin key follow the exact sections above. No additional algorithm, threshold, certificate lifetime, format or automatic downgrade is introduced by this registration.

Serial reconciliation in2.3 can only concern the same original authorized reservation with conclusive evidence of no prior signature; it cannot reallocate a burned/cancelled serial, rebind it to another request or reuse serials to compensate failure. Failed/uncertain issuance without that proof stays pending/denied or burns the reservation; no blind retry or decreasing high-water. Records/history and issuer uniqueness persist.

CNG Software KSP is the selected initial backend candidate, conditional on concrete provider/OS selection and validation before use. Naming/API presence and export-policy0 are not proof of token isolation, absolute host-compromise protection or recoverability. Browser/VC6/NT4 receives certificates and uses its own governed provider; it has no CNG dependency. Exact new-profile compatibility remains NOT_QUALIFIED until real target execution.

Before PRODUCTIVE CA initialization, an approved backup/recovery/protection/rotation procedure must exist, covering key/certificate correspondence and issuance/serial/revocation/root-state consistency. Before issuance, the required initial backup/restore evidence must be verified as specified in3.5–3.6. No real CA may be created while that contract/procedure remains incomplete. Encrypted OS recovery images and two protected generations are an accepted baseline obligation, not a claim that any available Windows tool has been qualified or that a machine-bound key is portable. If selected KSP/backup mechanism fails adequate recovery, keep initialization/activation denied and submit a governed alternative; never silently weaken key protection/export restrictions.

Future PapinhoTrustStore interfaces remain replaceable for issuance/signing/validation/revocation/status/rotation without becoming a Phase6 dependency or implementation now. Nonexportable old-key routing/rotation cannot erase issued/revoked certificate history. Package v2 crypto/KDF/encoding/bounds are not accepted by this decision. Active Session revocation remains POLICY_PENDING. Accepted manual custody exception and automatic client-local key rule are unchanged. Same root/registry/listener, factual commit/CURRENT/epoch, Principal separation, Model E, ED08/ED11 and provider/kernel UNKNOWN remain mandatory.

Concrete private issuer codec/layout/capacity review and actual same-root live integration remain implementation gates; existing first-dormant BODY05 does not become an issuer registry by this acceptance. No unspecified operational authority or resource approval is inferred from documents. Earlier proposal/ADR texts remain intact; no whole ADR is superseded.

## Limits and current result

INTERNAL_CA_TECHNICAL_PROFILE=ACCEPTED
NT4_COMPATIBILITY=NOT_QUALIFIED
CNG_KSP_BACKEND=INITIAL_CANDIDATE_REQUIRES_CONCRETE_PROVIDER_VALIDATION
BACKUP_RECOVERY=APPROVED_PROCEDURE_AND_RESTORE_PROOF_REQUIRED_BEFORE_PRODUCTIVE_USE
ACTIVE_SESSION_REVOCATION=POLICY_PENDING
IMPLEMENTATION_AUTHORIZED=NO
REAL_CA_CREATED=NO
CERTIFICATES_GENERATED=NO
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
PHASE6_PROVISIONING_GATES=OPEN

No code, real CA/certificate/credential, admission, network, PST/PACC change, PST065 publication or Phase6 closure is authorized by this acceptance. Future scoped source implementation and test executions require their applicable separate authorization.

## History

Revision1,2026-10-18: exact technical sections accepted under owner conditions and official executor review; proposal preserved; backend/backup/NT4 and operational gates remain unqualified.