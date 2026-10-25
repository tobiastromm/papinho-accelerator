# Personal CA preinitialization normative acceptance — 2026-10-22

Owner explicitly approved the direction and authorized exact official normative acceptance subject to compatibility review. Review found no material conflict with3.3/3.5–3.7. This record is effective only after the exact write approval and postimage verification documented in verification.json. See conformance-review.md, the new designated ADR and PROCEDURE-ACCEPTED.md. Original proposals and earlier STOPs remain unchanged history.

The accepted choice is procedural: wbadmin candidate, two offline BitLocker-protected media and dedicated local standard operational account, separated from the administrator. The account SID is an actual SAM/TokenUser identity to be established, not an asserted SCM per-service SID. No service hosting, account or source is qualified. Recovery KSP/isolation remain unproved; backup and restore not run. Approval closes only the normative direction/procedure-approval question, not any factual implementation, source or capacity gate.

No functional patch, build, test, account/service/ACL/key/media configuration, native key query, backup/restore, bootstrap, certificate, network or CURRENT operation. No PST/Browser/wire/quota/production change, clone/worktree or commit/push/tag/release. Original WIP hashes are verified after the documentary write; only the live-doc append is an intended modification of an existing file. Historical LAB files/receipts preserved; no key operation.

CURRENT_TASK_COMPLETED=YES_NORMATIVE_SCOPE_ONLY
PERSONAL_CA_PREINITIALIZATION_PROCEDURE=ACCEPTED_NORMATIVE_ONLY
BACKUP_MECHANISM=WBADMIN_CANDIDATE_NOT_QUALIFIED
OFFLINE_MEDIA=TWO_BITLOCKER_PROTECTED_DIRECTION_ONLY
OPERATIONAL_SERVICE_IDENTITY=DEDICATED_LOCAL_STANDARD_ACCOUNT_NOT_CREATED
SERVICE_SID_SELECTION=SAM_ACCOUNT_TOKENUSER_DISTINCT_FROM_SCM_SERVICE_SID
KSP_RECOVERY=NOT_PROVEN
OPERATIONAL_ISOLATION=NOT_PROVEN
BACKUP_EXECUTION=NOT_RUN
RESTORE_EXECUTION=NOT_RUN
IMPLEMENTATION_AUTHORIZED=NO
NEW_PRODUCTIVE_KEY_GENERATION=NOT_RUN
BOOTSTRAP_EXECUTION=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
PREVIOUS_LAB_KEY_DISPOSITION=UNKNOWN
LAB_KEY_PRESERVED=YES_NO_KEY_OPERATION
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=VERIFIED_IN_VERIFICATION_JSON
FIRST_UNRESOLVED_DIVERGENCE=FACTUAL_OPERATIONAL_ACCOUNT_BINDING_KSP_RECOVERY_ISOLATION_AND_FULL_ED11_UNQUALIFIED
NEXT=PERSONAL_FIRST_INSTALL_CA_INITIALIZE_PRODUCER_UNDER_SEPARATE_IMPLEMENTATION_AUTHORIZATION
