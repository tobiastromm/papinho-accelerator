---
status: proposed
revision: 1
scope: manual package delegated signer source and consumer binding
---
# Proposed focal decision - same-root package signer delegation

Task PACC-PHASE6-MANUAL-PACKAGE-M1-TRUST-BINDING-1 expressly permits preparing the smallest focal decision when no governed factual source exists. This is that proposal, NOT acceptance, a functional patch or an instantiated trust authority. M0-M8 and all accepted formats remain unchanged. No key/certificate, root, registry, authority, listener, codec or operational trust is created.

## D0 - accepted sequence, no normative amendment

The owner has corrected the preceding prompt. Preserve exactly: bounded outer structure/parameters -> independent package signature verification -> established signer delegation/trust -> KDF -> authenticated AEAD -> private archive/material/identity checks -> controlled whole-result release. Both independent signature and trusted delegation must pass before KDF. Signature verification may use embedded public SPKI as UNTRUSTED mathematical input; it must match an independently approved complete SPKI before KDF. Do not call the existing approved-SPKI verifier with file-supplied bytes and then label its result trusted. Existing verifier may instead obtain independently pinned public bytes from the applicable context before performing signature verification; no ordering requirement authorizes deriving trust from those bytes alone.

This restates M1 and the current owner correction only. It does not modify M0-M8. No signature/KDF ordering conflict remains open.

## D1 - proposed source selection requiring explicit acceptance

Select the existing administrative root's policy approval, with the existing issuer PACKAGE record as the public certificate/history reference, as the sole producer-side source of package signer delegation. A separate explicit administrator-authorized policy approval SHALL delegate acceptance of manual package signatures to that exact signer. Preparing, issuing or publishing a PACKAGE-purpose certificate SHALL NOT itself grant this delegation.

The approval SHALL bind the existing issuer request/record identity, issuer authority/generation, exact certificate digest and complete DER SPKI/fingerprint, manual PACCMP02/version2/suite1/domain purpose, applicable root/server scope, validity and delegation revocation/status to a factual committed/published administrative revision. Identifiers retain their existing meanings; no new global signer ID or counter is introduced. Certificate validity/revocation and delegation validity/revocation SHALL be checked independently. Missing or ambiguous facts deny.

This selects authority SEMANTICS, not new serialized fields or a concrete installed signer. Implement through the existing policy participant/issuer/root transaction and lifetime only. Existing PAPACC_ISSUER_RECORD.policy identifies policy; its bytes are NOT themselves a delegation record or proof. If the actual policy representation cannot carry an explicit approval, a smallest versioned same-root extension must be reviewed/accepted before dependent persistence. No reserved bytes, inferred enum meaning, additional journal or shadow allowlist is permitted. Do not designate a codec/layout by inference in this decision.

## D2 - acquisition and public validation

The existing root/policy owner SHALL provide a read-only immutable view of the exact applicable approval and issuer certificate/history while retaining its existing pin/lease. The view is borrowed or caller-owned copied data under that owner, not an authority-bearing boolean, arbitrary successful callback or second trust store.

Validate dedicated signer RSA2048/e65537, sha256WithRSAEncryption certificate profile, critical digitalSignature KeyUsage and CA=FALSE BasicConstraints, noncritical codeSigning-only EKU, accepted Papinho Package signer identity, issuer/direct-chain association, validity and applicable certificate/delegation revocation evidence. CA/channel/client keys must not be repurposed. codeSigning or certificate-chain validity alone is insufficient. Exact full SPKI and scope must match independent approval.

The producer/local administrative consumer requires factual applicable root revision/CURRENT and protected lifetime where those contracts require them. A historical pin or decoded record does not manufacture CURRENT. Do not require Browser offline import to fabricate or remotely query the Accelerator's live CURRENT; its independent consumer reference is addressed separately in D3.

## D3 - proposed consumer binding requiring explicit acceptance

Bind the existing independently authenticated M1 full-fingerprint/server confirmation to the existing Browser candidate/generation/reference/store/lease pipeline, without a second persistent trust store. Its source SHALL establish the exact signer SPKI fingerprint, package purpose/delegation, expected server/root scope, validity and the applicable certificate/delegation status reference INDEPENDENTLY of the package being imported. Explicit confirmation of a value read only from that package SHALL NOT satisfy this requirement and SHALL NOT be TOFU.

For a first Browser, reuse M1's already allowed independent administrative confirmation ceremony; no mandatory second material file, prior CSR, new wire protocol or network service is selected here. The administrative source of that confirmation must be the independently authenticated administrator acting under the D1 approval, not generic TLS trust, a candidate CA or a plain UI label. The existing candidate/generation owner must retain the reference and confirmation facts through validation/commit; no raw textual fingerprint becomes authority without that provenance.

Before implementation, its exact existing confirmation action, owner and source transport must be identified and demonstrated. If not available, the consumer remains DENY; do not invent a source to implement this proposal. This decision does not claim any global/current revocation freshness from an offline package. An explicit applicable status/freshness policy is required for the presented evidence; absent an approved sufficient policy/reference, deny. No numeric TTL, online dependency or alternative revocation protocol is inferred here. Producer CURRENT and the consumer's independently established reference are distinct facts.

## D4 - denial and lifetime

Before KDF, reject absent/unapproved/unknown/expired/revoked/ambiguous signer or delegation, wrong certificate purpose/SPKI/issuer/server scope, lost applicable pin/reference, Stop or uncertain authority. Certificate issuance authorization, an enum, an admin group membership, successful RSA and password knowledge cannot substitute for delegated acceptance.

All confidential buffers remain private until AEAD, archive/TLV, key/certificate, issuer/serial/time/server identity and final authority/lifetime checks finish. Tag success alone does not release a package. Hold inputs, password/key/provider/quarantine and borrowed material through factual synchronous completion or retained pending work. No early cleanup, timeout destruction, destructive retry or fabricated rollback after uncertainty. Active-operation revocation semantics not already accepted remain a separate gate; do not introduce a Stop policy for old Sessions.

## D5 - accounting and tests

No new quota or credit is granted. Charge actual owned storage/copies and all simultaneous/retained provider and caller resources once under existing ED11. Default2MiB and explicitly selected Windows3MiB remain unchanged, provider/kernel UNKNOWN, full ED11 PARTIAL. Fixture/global provider contexts must be separated from actual runtime without omitting real shared costs or assuming exclusivity. Qualification failure denies integration.

Required tests after the approved binding exists: valid delegated signature (fixture authority clearly isolated from production), valid signature without delegation, revoked/expired/wrong-scope delegation, bad signature, zero KDF invocations for every pre-trust failure, exact one authorized derivation, password/tag/cipher/material negatives, Stop at each phase, no externally visible plaintext until complete success, retained resources/credits on uncertainty, stale references and complete ED11 coexistence. Synthetic trust must never be selectable in production.

## D6 - acceptance and implementation boundary

Only D1 source-selection semantics and D3 independent-consumer binding are additional proposed decisions. D0/D2/D4/D5 preserve accepted obligations and do not expand authority. Owner acceptance plus official executor approval is required before recording a designated accepted decision or implementing dependent policy/persistence/consumer semantics. Preserve this proposal and all original accepted records. No automatic acceptance or functional patch approval by reference.

Even after acceptance, actual signer identity/delegation, positive CURRENT/admin environment, concrete policy/persistence representation, lifetime, status/freshness evidence and ED11 must be supplied/qualified before positive use. Missing proof continues DENY. No production signing, recovery, CURRENT, admission or Phase6 closure is authorized. Browser/PST/wire and production0.6.4 unchanged.
