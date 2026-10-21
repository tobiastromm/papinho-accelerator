# Closeout — exact internal CA technical profile preparation

2026-10-18. Owner selected preparation of the three technical decisions after the implementation preflight identified their explicit exclusion from prior acceptance. This completes that DOCUMENTARY continuation only. Original PACC-PHASE6-INTERNAL-CLIENT-ISSUER-IMPLEMENTATION-1 is NOT completed; no functional source or build/test was performed. Dependent implementation remains pending exact normative acceptance.

- [DECISION-TECHNICAL-PROPOSED.md](DECISION-TECHNICAL-PROPOSED.md): exact proposed X509/serial/key-custody text with concrete values and preservation constraints.
- [COMPATIBILITY-AND-IMPLEMENTATION-HANDOFF.md](COMPATIBILITY-AND-IMPLEMENTATION-HANDOFF.md): local factual sources and limitations, target tests, independent/dependent implementation split.
- BASELINE.json and SHA256SUMS.txt: selected source/authority hashes, working-tree status, deliverable integrity. No earlier accepted or proposed document rewritten.

## Selected proposal and rationale

X509 RSA2048/e65537/SHA256 direct root-to-leaf profile uses the bounded certificate family already reported in historical PST NT4 interoperability evidence and existing Browser R1 RSA correspondence code. Separate CA, serverAuth, clientAuth and delegated codeSigning keys/purposes avoid role confusion. CA5*365days permits practical private-installation continuity; leaf90days reduces stale compromised-credential lifetime at the cost of explicit renewals. Rotation preparation90days before CA expiry covers the proposed maximum leaf span, without automatic trust change. These are concrete proposed administrative tradeoffs, not empirically measured security guarantees or public-Web-PKI policy claims.

Serial16bytes with persisted generation prefix+u64 counter is bounded and representable by existing target integer/DER capabilities. Durable reservation before signing, counter burn, idempotent query and restore fencing address concurrency/crash/uncertainty; a valid local old backup hash is explicitly insufficient anti-rollback evidence. All records stay in the same root authority, requiring new reviewed versioned issuer persistence instead of pretending old BODY05 holds a registry.

Modern CNG Software KSP persisted nonexportable signing key limits normal API exposure, avoids a custom encrypted CA file and separates administrative purpose from normal TLS handlers. Distinct actual tokens/key ACLs must be proved, not assumed from helper functions. This is software OS protection, not HSM isolation. Encrypted full-system recovery was selected as initial backup contract because a nonexportable machine key cannot honestly promise portable PKCS8 restore. Concrete OS backup tool/procedure and actual restore remain deployment qualification inputs; lack of proof keeps issuer activation denied. Future TrustStore migration can preserve exact public cert/status/history while old protected provider services retained work and new issuance uses explicit rotation; portable private-key migration is not claimed.

No package v2 KDF/AEAD/signature/size/format change or acceptance; no new active-Session revocation policy. Accepted manual server custody and automatic client-local key preserved. Internal CA normal design and future TrustStore nondependency unchanged. No new collector/supervisor/registry/listener or PACC message.

## Implementation disposition

Independent of proposed values: read-only integrity checks, existing root/Stop/retention negative regressions under their current contracts, purpose-neutral owned receipt/borrower reuse and fail-closed unavailability paths, census preparation and API/test documentation. These do not grant ability to sign/publish. Avoid unused fake providers/parallel registry just to show partial progress.

Dependent: actual certificate generation/role validator policy; serial allocator/private persistence/recovery; CNG key creation/protection/backup context; same-root live issuer registration and real issue/query/revoke/renew/sign implementation. Must accept exact technical candidate first. Package suite and active-session policy have separate remaining gates. No automatic implementation resumes from a documentary approval.

## Verification

Accelerator main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a.28/28 selected preexisting hashes unchanged; normal SDKs Accelerator46/46 and Browser55/55 hash matched. No SDK modifications; no historical outputs overwritten. git diff --check passed. Scoped preservation is not a full fresh hash inventory of ignored outputs.

Windows SDK ncrypt.h establishes API declarations, not qualified runtime behavior. PST phase7 legacy report is historical API1.2/library0.3 evidence; new normal065/VC6/NT4 chain remains NOT_RUN/NOT_QUALIFIED. No actual NCrypt key create/open/export/sign/backup/restore or certificate/credential generation. No builds, product tests, network, Browser/PST/wire/pin change, Git config/ACL change, clones/worktrees or commit/push/tag/release. Official documentary write approvals granted. Runtime ED11 delta0 for documentation; future provider/key/crypto/journal/retention/coexistence costs unmeasured, provider/kernel UNKNOWN, quota unchanged.

CURRENT_TASK_COMPLETED=YES_DOCUMENTARY_TECHNICAL_PROFILE_SLICE
INTERNAL_ISSUER_IMPLEMENTATION_COMPLETED=NO
NORMATIVE_STATUS=PROPOSED
X509_PROFILE=RSA2048_SHA256_DIRECT_CHAIN_DISTINCT_PURPOSES_PROPOSED
CERTIFICATE_VALIDITY=CA1825D_LEAF_MAX90D_PROPOSED
PERSISTENT_SERIAL=PREFIX8_COUNTER8_RESERVE_BEFORE_SIGN_PROPOSED
CA_KEY_PROTECTION=CNG_SOFTWARE_KSP_NONEXPORTABLE_ADMIN_CONTEXT_PROPOSED
BACKUP_RECOVERY=ENCRYPTED_OS_RECOVERY_WITH_HIGHWATER_FENCING_PROPOSED_NOT_QUALIFIED
AUTOMATIC_LOCAL_KEY=PRESERVED
MANUAL_CUSTODY_EXCEPTION=PRESERVED
ACTIVE_SESSION_REVOCATION=POLICY_PENDING
IMPLEMENTATION_RESUMED=NO_DEPENDENT_WORK_AWAITS_ACCEPTANCE
REAL_CA_CREATED=NO
CERTIFICATES_GENERATED=NO
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PROVIDER_KERNEL=UNKNOWN
BUILDS=NOT_RUN
OFFLINE_TESTS=NOT_RUN
NETWORK_TESTS=NOT_RUN
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_SCOPED_HASH_AND_WRITE_SCOPE_CHECKS
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=THREE_TECHNICAL_PARAMETERS_PROPOSED_AWAIT_EXACT_ACCEPTANCE
NEXT=OWNER_ACCEPTANCE_OF_EXACT_INTERNAL_CA_TECHNICAL_PROFILE