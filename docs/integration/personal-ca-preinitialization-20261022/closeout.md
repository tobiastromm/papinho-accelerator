# Personal CA preinitialization — closeout, 2026-10-22

Task PACC-PHASE6-PERSONAL-CA-PREINITIALIZATION-1. Prepared a concrete procedure and focal ADR candidate; normative acceptance remains pending. main HEAD913c90cf2328f7939d8f5b927640601903ac6e91. No functional implementation or operational action.

Review [PROCEDURE-PROPOSED.md](PROCEDURE-PROPOSED.md) and [ADR candidate](../../adr/ADR-CANDIDATE-personal-ca-preinitialization.md). Candidate fixes the procedure SHA256 for exact review. Existing accepted authorities and earlier STOP remain unchanged.

## Findings and proposal

Official Microsoft documentation supports volume/bare-metal backup with wbadmin, WinRE system recovery, BitLocker protection of destination volumes and local account/service identity mechanisms. Source URLs, consultation date and deliberately limited claims are in sources.json. Their capability is documented; recovery of this installation's nonexportable MACHINE KSP key is not empirically established. A candidate qualification run on a separately authorized environment must prove that. No new LAB key or repeat of old LAB work is authorized.

Read-only local inventory: Windows Professional22H2 build19045.6456; wbadmin.exe, manage-bde.exe and reagentc.exe present under System32. Tool presence is not proof of usable WinRE, encryption enabled, recoverable media, supported backup path or KSP restoration. No backup/BitLocker command was invoked and no media state/secrets were queried.

Code searches in apps/src/tools found no SCM service installation/hosting calls (CreateService/OpenSCManager/StartServiceCtrlDispatcher/ServiceMain/ChangeServiceConfig). Existing personal frontend/CA owner are console-based. Dedicated local standard account is proposed as minimal distinct runtime identity, with future factual SID/token binding; no named account is selected/created. Service SID/virtual-account alternative is documented but would require unimplemented SCM hosting. No new supervisor is justified by this task.

Procedure supplies explicit precreation approval, postcreation backup and preissuance restore gates; consistent all-volume capture; two independently protected offline media; recovery-key custody; manual command templates; verification of private-access denial; separately retained checkpoint/high-water and fencing; UNKNOWN handling; rotation/retirement and resource obligations. It does not claim copy-of-files, image timestamp, key name or local receipt provides authority/latest-state. No conflict identified between NCrypt nonexportability and protected OS-image recovery, but no independent-equipment portability guarantee exists.

## Review disposition

PROPOSED; no accepted decision written. Section9 of the request requires approval before ACCEPTED. Requested policy choices for review: procedure revision1 and dedicated local operational account direction, with wbadmin/BitLocker retained as target-qualified candidates. This review cannot approve empirical facts not yet observed. Exact source/target operations and functional patches remain separate gates.

First factual qualification gap: no isolated image/WinRE/KSP recovery proof for the proposed target mechanism. Deployment-specific media/custodians/account and hosting/access isolation are also unselected/unqualified. The proposal distinguishes these from approval of the procedure itself. No extra generic readiness matrix created.

ED11 unchanged: prior Legacy1799624/2097152 and Selected1965120/3145728 are layout subtotals only, not operational peaks. CA caller owner480 bytes charged once if additional. Token/SAM resolution/ACL, full nested stack, runtime, KSP/kernel, retained work, backup/VSS/BitLocker process coexistence, physical storage and host admission remain to be accounted. No cost moved between profiles or treated as zero. No capacity PASS.

Only new proposal/evidence documents added. All existing versioned/untracked nonignored files were hash-compared against the prewrite snapshot; see evidence.json. No functional change or new build/test required for these drafts. Existing LAB receipts/keys and four historical LAB directories untouched; keys not queried.

```text
CURRENT_TASK_COMPLETED=NO
PERSONAL_CA_PREINITIALIZATION_PROCEDURE=PROPOSED_REVISION_1_AWAITING_APPROVAL
CA_KEY_PROTECTION=ACCEPTED_BASELINE_PRESERVED_TARGET_PROOF_PENDING
BACKUP_MECHANISM=WBADMIN_BITLOCKER_CANDIDATE_DOCUMENTED_NOT_QUALIFIED
RECOVERY_PROCEDURE=PROPOSED_WINRE_ISOLATED_BMR_WITH_KSP_PROOF
ROTATION_POLICY=PROPOSED_UNDER_EXISTING_1_4_AND_3_7
RETIREMENT_POLICY=PROPOSED_QUIESCENCE_HISTORY_RETENTION_NO_AUTOMATIC_DELETE
OPERATOR_IDENTITY=EXISTING_NATIVE_TOKEN_SOURCE_REVALIDATION_REQUIRED_PER_ATTEMPT
OPERATIONAL_SERVICE_IDENTITY=DEDICATED_LOCAL_STANDARD_ACCOUNT_PROPOSED_NOT_PROVISIONED
SERVICE_SID_SELECTION=FACTUAL_SAM_TOKENUSER_BINDING_PENDING_NOT_SCM_SERVICE_GROUP
CONTRACT_COMPATIBILITY=NO_NORMATIVE_CONFLICT_IDENTIFIED_QUALIFICATION_PENDING
IMPLEMENTATION_AUTHORIZED=NO
NEW_PRODUCTIVE_KEY_GENERATION=NOT_RUN
BOOTSTRAP_EXECUTION=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
BUILDS=NOT_RUN_DOCUMENTARY_ONLY
TESTS=NOT_RUN_DOCUMENTARY_ONLY
NETWORK_OPERATIONAL=NOT_RUN
OFFICIAL_REFERENCE_CONSULTATION=YES
PREVIOUS_LAB_KEY_DISPOSITION=UNKNOWN
LAB_KEY_PRESERVED=YES_NO_KEY_OPERATION
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES_EXISTING_FILES_HASH_VERIFIED
FIRST_UNRESOLVED_DIVERGENCE=PROCEDURE_APPROVAL_AND_TARGET_SPECIFIC_KSP_IMAGE_RESTORE_CAPABILITY_PENDING
NEXT=PERSONAL_CA_PREINITIALIZATION_EXACT_REVIEW_AND_QUALIFICATION
DOWNSTREAM_NEXT=PERSONAL_FIRST_INSTALL_CA_INITIALIZE_PRODUCER
```
