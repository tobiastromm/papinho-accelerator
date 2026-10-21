# Closeout — internal issuer and scoped manual custody proposal

2026-10-18. Requested documentary decision/handoff complete. All new normative material remains PROPOSED; no accepted ADR changed, no implementation or key/certificate created. Exact source authorization preserved in OWNER-REQUEST.txt.

- [DECISION-PROPOSED.md](DECISION-PROPOSED.md): exact acceptance candidate, manual-only custody/risk exception, internal normal issuer without external CA dependency, automatic local custody preserved, clause-level precedence matrix.
- [ISSUER-CONTRACT-AND-HANDOFF.md](ISSUER-CONTRACT-AND-HANDOFF.md): smallest same-root issuer participant, explicit initialization, protected key/backup/recovery/rotation, persistent serial/status history, dynamic client publication, replaceable purpose interfaces, security/ED11 matrix and ordered implementation gates.
- [BASELINE.json](BASELINE.json): active main HEADs/WIP and24 selected preexisting hashes; SHA256SUMS.txt pins deliverables and source request.

The earlier complete-manual package format/crypto/bounds are preserved without edits or implicit acceptance. Earlier external-issuer selection is no longer the intended normal design: owner chooses the internal issuer, which is still unimplemented. The new proposal does not create a CA or operational trust merely by naming it. Internal client CA, channel trust, package signer and origin SYSTEM trust remain purpose-separated; the single-anchor package deployment binding is an explicit unresolved integration input, not silently changed encoding.

Normative divergence remains scoped and explicit until accepted: ADR-0006 normally keeps client key generated/stored locally and the earlier accepted provisioning decision item2 did not authorize server export. The new candidate supplies exact manual exception/risk text while retaining automatic local key and all other security rules. Owner requested PROPOSED, so neither previous accepted text nor decision status was autonomously changed. Existing Sessions are not assigned a new revocation disposition; issuer status/new-auth deny and active Session handling are separate facts.

Operational gaps: CA/provider/protected storage and policy are not initialized/selected; backup artifact/method and validity/retention/capacity limits need explicit approved binding; current root supports first dormant commit, not complete live registry update. Proposed status/serial/issuer interfaces are not implemented API symbols. PapinhoTrustStore is future and not a Phase6 dependency. No independent identity store, registry, root or listener is introduced.

Verification: Accelerator main HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a; Browser main HEAD1eb028554f622672377953225e162ed9b6564a40.24/24 selected preexisting files unchanged, Browser status unchanged. SDK inventories matched Accelerator normal46, Browser production47, normal55, R2 34 independently; no outputs rebuilt or SDK mixed. git diff --check passed. New files only in this folder; existing source/proposals/roadmap preserved. Scoped hashes/status do not claim a complete new inventory of ignored historical output files. Historical map limitations unchanged.

No build, product test, native measurement, real keys/certificates, credentials, CA init, backup/restore, network, Browser/PST/PACC modification, admission, Git config/ownership change, clone/worktree, commit/push/tag/release. Official escalated documentary writes approved. No runtime ED11 delta from documents; future issuer/provider/storage/journal/generation/retention costs require accounting. Provider/kernel UNKNOWN, no quota increase. Six provisioning gates remain OPEN; Phase6/PST065 publication not qualified.

CURRENT_TASK_COMPLETED=YES
INTERNAL_CA=PROPOSED
EXTERNAL_CA_REQUIRED=NO_FOR_NORMAL_DESIGN
MANUAL_SERVER_KEY_CUSTODY=PROPOSED_SCOPED_EXCEPTION
AUTOMATIC_CLIENT_KEY_CUSTODY=PRESERVED
CERTIFICATE_ISSUANCE=CONTRACT_PROPOSED_NOT_IMPLEMENTED
CERTIFICATE_REVOCATION=PERSISTENT_STATUS_CONTRACT_PROPOSED_ACTIVE_SESSION_POLICY_PENDING
LIVE_CLIENT_PUBLICATION=SAME_ROOT_CONTRACT_PROPOSED_IMPLEMENTATION_GAP
FUTURE_TRUSTSTORE_BOUNDARY=PURPOSE_SPECIFIC_REPLACEABLE_NOT_PHASE6_DEPENDENCY
NORMATIVE_STATUS=PROPOSED
IMPLEMENTATION_AUTHORIZED=NO
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
BUILDS=NOT_RUN
OFFLINE_TESTS=NOT_RUN
NETWORK_TESTS=NOT_RUN
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_SCOPED_HASH_AND_STATUS_VERIFIED
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=MANUAL_CUSTODY_EXCEPTION_AND_INTERNAL_ISSUER_TEXT_AWAIT_NORMATIVE_ACCEPTANCE
NEXT=OWNER_ACCEPTANCE_OF_INTERNAL_ISSUER_AND_MANUAL_CUSTODY_CONTRACT