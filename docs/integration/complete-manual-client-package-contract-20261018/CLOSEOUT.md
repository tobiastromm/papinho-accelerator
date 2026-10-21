# Closeout — complete server-generated manual package contract

2026-10-18. Documentary task completed; contract remains PROPOSED. Exact owner request preserved in OWNER-REQUEST.txt. No code, real key, certificate, password or package generated. No normative acceptance or mutation of prior accepted ADRs.

[CONTRACT-PROPOSED.md](CONTRACT-PROPOSED.md) selects an authenticated `.papacc` envelope with restricted ZIP STORE payload rather than promising generic password-ZIP compatibility. Contains exact layout, five archive members, format/suite bounds, generated unlock secret, PBKDF2/AEAD/signature/trust, server-generated client-key custody, issuer uncertainty, live administration, revoke/renew and commit/rollback. All new technical selections including600000 KDF iterations and size limits are proposals, not runtime measurements or existing approvals.

[BROWSER-HANDOFF-AND-VALIDATION.md](BROWSER-HANDOFF-AND-VALIDATION.md) supplies the minimum Browser delta, preserved advanced import, source/API evidence, security/compatibility matrices and ordered implementation gates. No preconfigured Browser or client CSR needed for this manual model. Automatic remains separate with client-local key, same eventual issuer/registry; six provisioning gates remain OPEN. Prior PCP1 public/local-CSR proposal preserved unchanged and not treated as accepted.

## Normative boundary

GOVERNANCE CONFLICT identified before implementation: ADR-0006 “Private keys permanecem no endpoint proprietário” and its normal client-local generation rule versus newly authorized manual server generation; earlier provisioning accepted item2 explicitly did not authorize server export. Owner now approves requirements but requires this revision PROPOSED and no accepted ADR without specific authority. Exact scoped exception must be registered upon later acceptance before dependent code. No need to reopen automatic key custody or weaken other identity/trust/lifecycle contracts. This blocks implementation, not completion of requested proposal/handoff.

No operational issuer or package signer was established. Existing lab source is not that authority. Required minimal integration is same-root live registry transaction plus selected approved issuer/signing adapter and durable reconciliation; first dormant transaction is insufficient. No new CA, listener or parallel registry proposed. Existing server TLS key is not silently repurposed as CA/signer. Actual issuer/material creation needs separate authorization.

## Verification

Both main HEADs remain Accelerator c50b8fb9a7e3293b5e6355fc08459bf85b56857a, Browser1eb028554f622672377953225e162ed9b6564a40. Baseline records actual WIP and21 selected authority/source files. All21 hashes unchanged; Browser Git status unchanged. SDK checksums matched46 normal Accelerator,47 production Browser,55 normal Browser,34 R2 Browser separately. No SDK edits/builds/interchange. git diff --check passed. New files only in this task folder; no old proposal, functional source or output overwritten. Scope verification does not claim a fresh full hash inventory of ignored historical outputs.

Static evidence only: BearSSL includes pure32bit ChaCha20/Poly1305, HMAC/SHA256 and RSA APIs, existing VC6 integration recipe. No qualified PBKDF2 adapter located; exact vetted implementation artifact remains a gate. New suite linked compatibility/performance/cancellation on real VC6/NT4 is NOT_PROVEN. No build, product test, cryptographic vector, native measurement, network, PKI execution or admission change. No secrets logged/persisted. No clone/worktree, commit/push/tag/release. Official escalated documentary writes approved. Historical map limitations remain unchanged.

ED11: documents add no runtime storage. Future keygen/issuer/KDF/crypto/ZIP/parser/store/journal/retention/generation coexistence require exact compiled and peak accounting before effects. No quota increase or opaque provider/kernel approval; UNKNOWN remains UNKNOWN.

CURRENT_TASK_COMPLETED=YES
MANUAL_PACKAGE=SERVER_GENERATED_COMPLETE_PROPOSED_CONTRACT
PACKAGE_FORMAT=PAPACC_V2_AUTHENTICATED_ENVELOPE_WITH_RESTRICTED_ZIP_STORE
PACKAGE_ENCRYPTION=CHACHA20_POLY1305_SUITE1_PROPOSED
PASSWORD_KDF=PBKDF2_HMAC_SHA256_600000_PROPOSED_NOT_QUALIFIED
PACKAGE_AUTHENTICITY=INDEPENDENTLY_VERIFIED_ADMIN_SIGNER_RSA_SHA256_PROPOSED
PRIVATE_KEY_CUSTODY=SERVER_GENERATION_PROTECTED_TRANSFER_SCOPED_NORMATIVE_EXCEPTION_PENDING
ISSUER_CONTRACT=PROPOSED_OPERATIONAL_BINDING_NOT_ESTABLISHED
ADMIN_CLI=CREATE_EXPORT_LIST_REVOKE_RENEW_PROPOSED
HOT_UPDATE=REQUIRED_NOT_IMPLEMENTED_BY_THIS_TASK
BROWSER_SIMPLE_IMPORT=PROPOSED
BROWSER_ADVANCED_IMPORT=PRESERVED
BROWSER_HANDOFF=BROWSER-HANDOFF-AND-VALIDATION.md
VC6_NT4_NEW_PACKAGE_COMPATIBILITY=STATIC_CANDIDATE_RUNTIME_NOT_PROVEN
PHASE6_PROVISIONING_GATES=OPEN
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
WIP_PRESERVED=YES_SCOPED_HASH_AND_STATUS_CHECKS
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=ACCEPTED_CLIENT_LOCAL_CUSTODY_REQUIRES_SCOPED_MANUAL_EXCEPTION_RECORD
NEXT=OWNER_ACCEPTANCE_OF_COMPLETE_MANUAL_PACKAGE_CONTRACT