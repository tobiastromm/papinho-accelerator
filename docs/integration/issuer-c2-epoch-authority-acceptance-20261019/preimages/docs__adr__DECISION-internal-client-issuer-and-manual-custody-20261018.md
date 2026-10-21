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

# Internal client certificate issuer and scoped manual key custody

## Authority and scope

Owner explicitly authorized exact normative acceptance in task PACC-PHASE6-INTERNAL-ISSUER-MANUAL-CUSTODY-NORMATIVE-ACCEPTANCE-1, subject to official executor approval of this registration. This file is the designated normative record. Original proposal and historical accepted ADRs remain unchanged.

Source: [DECISION-PROPOSED.md](../integration/internal-issuer-manual-custody-decision-20261018/DECISION-PROPOSED.md), revision1, SHA256 5e3564b92bf713d742f2edb9a5b12592999851d26c36e282e1343a1871b3b26e. The eleven numbered clauses below are reproduced exactly from its decision section; surrounding proposal-status prose is not promoted.

## Accepted decision

1. PapinhoAccelerator SHALL provide an administratively initialized private certificate authority for normal client provisioning. An external CA SHALL NOT be required for the normal product flow. Its dedicated private key, CA certificate and persistent authority identity SHALL be separate from the operational channel server credential. No TEST_ONLY fixture or fresh CA per service restart is allowed. The CA private key SHALL NOT be a TLS runtime credential, an exported client material or an implicit startup side effect.
2. The authority SHALL issue and sign individual client credentials, keep persistent serial/issuance/revocation/renewal history, expose status and recovery, and preserve bounded audit without secrets. Manual and automatic provisioning SHALL use the same administrative client registry and issuance authority, with credential-to-stable-Principal and policy mapping separate from certificate/fingerprint/public key identity. Issuance SHALL NOT implicitly grant capabilities or admission.
3. A narrowly scoped exception to ADR-0006 “Private keys permanecem no endpoint proprietário” SHALL apply only to the complete MANUAL package flow: an explicitly authorized Accelerator administrative operation MAY generate one individual client private key and deliver it solely within the authenticated encrypted `.papacc` package. It SHALL keep no persistent plaintext copy or recovery password; transient copies SHALL have owned lifetimes, bounded credit and reviewed wiping at quiescence. The package and unlock secret SHALL have distinct approved delivery. Runtime plaintext exports, fixture keys and CA-key inclusion remain forbidden.
4. This exception accepts the additional exposure of the client key during server-side generation, packaging and delivery. A compromised provisioning host/operator or copied package plus unlock secret may expose/clone that client credential. Encryption, labels and import status do not prove single-device possession or physical erasure. The normal ADR-0006 assertion that server compromise does not expose client private keys does not apply to the manual issuance window/protected retained exports. This exception does not weaken transport identity, trust validation, mTLS or authorization.
5. AUTOMATIC SHALL retain client-local key generation and private-key custody. Browser sends only the public CSR/proof of possession after verified authorized pairing. Address/server identity verification, contextual code, OPEN/CLOSED enrollment, CLOSED password and AUTO/MANUAL approval remain required by their accepted contracts. No automatic implementation or bootstrap protocol is approved here.
6. CA initialization SHALL be a distinct authenticated administrative action. Missing/corrupt/unrecoverable authority on service startup SHALL deny issuance and report the state, never generate a replacement CA. Initialization, backup, restore and rotation SHALL preserve identity/history and verify key/certificate correspondence. Duplicate/uncertain initialization SHALL reconcile, not overwrite a store or try another key until success.
7. Issued clients SHALL be registered through the existing administrative root/configuration/policy and factual PREPARE -> VALIDATE -> durable COMMIT -> CURRENT -> EPOCH sequence, including bounded live updates without restart. No second root, registry/listener or direct store edit is authorized. Existing dormant transaction preconditions SHALL NOT be bypassed. Current implementation of first dormant commit alone is insufficient live publication.
8. Revocation SHALL persist and deny new revoked authentication/admission after applicable publication; renewal SHALL be explicit and may preserve stable Principal through approved mapping. Neither operation SHALL silently alter existing Sessions/operations without an accepted active-revocation policy. Surviving dependencies/leases/retained work remain governed by Model E and ED08. If active disposition is unspecified, report that scope as pending; do not report complete active revocation or free dependencies.
9. The proposed complete-manual `.papacc` v2 encrypted authenticated envelope and restricted ZIP content SHALL remain the implementation candidate. This issuer decision alone SHALL NOT accept or modify its proposed algorithms, KDF count, bounds or encoding. Simplified single-operation Browser import and granular advanced import SHALL coexist on the same CredentialGeneration/Reference/store/commit/leases pipeline. No prior CSR is required for manual import.
10. Cryptographic mechanisms SHALL use reviewed providers, not custom primitives. The CA/signing/key-protection and trust implementations SHALL be replaceable behind purpose-specific private contracts, preserving issued certificate bytes, identifiers, chains, validity and revocation during migration. Future PapinhoTrustStore SHALL NOT be a Phase6 prerequisite, an implementation now, or an assumption that a trust component automatically owns issuance/private keys.
11. No quotas increase is authorized. Key protection, signing, storage/journal/recovery, generations, leases and retained failures SHALL be included in ED11 before effects. Opaque provider/kernel resources remain UNKNOWN. All six provisioning gates remain OPEN until implementation/qualification. Production PST0.6.4 and PACC wire are unchanged; PST0.6.5 remains candidate until full Phase6 completion. Client/Site TLS/ED11 admission remain disabled/denied.

## Delimited precedence and unchanged authorities

Clauses3–4 prevail only for complete MANUAL client-package generation and protected transfer over ADR-0006's client-local generation/custody rule and its general server-compromise protection claim, and over item2 of DECISION-production-client-provisioning-20261018 insofar as that earlier item did not authorize server generation/export. They grant no plaintext/runtime/general export exception. AUTOMATIC and all other contexts retain client-local custody.

Clause1 selects the Papinho private-CA option allowed by ADR-0006 for the normal product flow, under explicit administrative initialization. CA private-key custody is restricted to the separate administrative issuer purpose; it is not distributed as a normal operational server TLS credential or to clients. No CA/TLS key reuse, fixture authority or startup regeneration is permitted. This does not establish an operational CA or authorize creation now.

ADR-0028's transport/admission/AuthN/AuthZ and channel/client/Site TLS trust boundaries remain unchanged. Principal remains distinct from credentials/fingerprints. Existing root/policy/current/epoch, Model E, ED08 and ED11 retain their authority. No second registry, root or listener, early epoch, grant or resource approval is created.

Active Session/operation revocation policy remains PENDING. Clause8 accepts persistent revocation/new-authentication denial and explicit renewal as obligations, not the earlier proposed Stop-on-revoke disposition for live Sessions. No new Session behavior or cleanup authority is accepted.

Clause9 preserves the complete-manual package as a PROPOSED implementation candidate. Package algorithms/KDF count/bounds/encoding are NOT accepted by this registration. The accompanying ISSUER-CONTRACT-AND-HANDOFF.md remains PROPOSED: its exact serial encoding, certificate profile, backend/storage/backup choices and interfaces are not promoted wholesale. Persistent serial/history, protection/recovery/rotation and purpose-specific replaceability are obligations from the eleven clauses; concrete unapproved technical selections remain pending.

PapinhoTrustStore is a future replaceable boundary and not a Phase6 prerequisite. No Future Direction or handoff becomes an accepted implementation or extra authority by reference. Earlier ADR texts and proposal history are preserved; no whole ADR is superseded.

## Limits and implementation state

IMPLEMENTATION_AUTHORIZED=NO
REAL_CA_CREATED=NO
CERTIFICATES_GENERATED=NO
ACTIVE_SESSION_REVOCATION=POLICY_PENDING
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PHASE6_PROVISIONING_GATES=OPEN
PST_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION

This normative registration does not authorize code changes, real CA/key/certificate generation, credential issuance, network, admission, PST publication or Phase6 closure. Future scoped implementation requires its own authorization and relevant accepted concrete contracts/target qualification.

## History

Revision1, 2026-10-18: exact owner-approved internal issuer/manual custody decision registered; original eleven proposal clauses unchanged, precedence delimited; operational and implementation gates remain closed.