---
status: accepted
revision: 1
decision-date: 2026-10-22
scope-level: project
scope-target: PapinhoAccelerator personal Windows CA preinitialization
supersedes: []
---
# Decision — personal CA preinitialization procedure and operational identity

## Owner authority and exact scope

The owner explicitly approved the proposed normative direction and authorized official normative acceptance subject to integral review of technical profile3.3 and3.5–3.7. This registration becomes effective only after official executor approval of its exact write and postimage verification. It authorizes no implementation or effects. The review is [recorded here](../integration/personal-ca-preinitialization-acceptance-20261022/conformance-review.md).

Sources remain immutable historical proposals:
- ADR-CANDIDATE-personal-ca-preinitialization.md SHA256 efadd9c80ccf71948701589741e88697bf3926c71630bd36db69e967fe3d7c05.
- PROCEDURE-PROPOSED.md revision1 SHA256 284f1fd1b246e1abdff246e5260667e75bfd2aea0362b35566cdc4f6842fd72d.

The exact P0–P6 source segment below has SHA256 d0f0a91d972cff6c6a126693fa6998568ac08e0d63c037bd7c072deed5228460. Sections A–I of the procedure are accepted as the delimited operational procedure, reproduced byte-exact in [accepted procedure](../integration/personal-ca-preinitialization-acceptance-20261022/PROCEDURE-ACCEPTED.md); that segment has SHA256 600d81ba1579da7909a73b14c87298640f7c57396b278fc592ed44c5fe0f502f.

Historical words PROPOSED/candidate and the original instruction not to record ACCEPTED in the prior review are superseded ONLY as to normative status of the explicitly incorporated clauses/procedure by this subsequent owner-authorized record. They remain valid as history in the original sources. References to unqualified tools, KSP restore, operational isolation, unperformed operations, pending machine-specific inputs and implementation/resource gates are NOT superseded. Documentation links and source headings do not promote their external contents to normative authority.

## Identity interpretation and limits under3.3

For this personal console-host direction, the distinct operational identity is a dedicated LOCAL STANDARD ACCOUNT. Its SAM account SID must be selected explicitly and later observed as TokenUser of the actual operational process, with separate factual administrative operator identity/context. Account creation, name resolution alone or a configured account string cannot qualify that binding.

This is NOT a claim that an account SID is an SCM per-service SID (S-1-5-80), a service-token group SID or a virtual-account qualification. No SCM hosting is implemented/required by this decision. Future SCM hosting must independently establish its selected logon account and any per-service SID and actual token; it may not substitute a derived name/SID for factual provenance. The service identity requirement of3.3 remains a distinct actual operational security principal, explicit denial of private-key use/open/export and real token isolation/access testing. No LocalService/SYSTEM/Administrators default, new service/supervisor or shared-token issuer activation follows. Same-root transactional authority remains mandatory.

## Exact accepted clauses

P0. Limit this procedure to personal/local Windows installation using the existing Microsoft Software KSP MACHINE CA owner. Preserve portable contracts and all accepted authority/ED11 gates. No global Windows/backend requirement.

P1. Adopt the three distinct gates A/B/C in [procedure revision1](../integration/personal-ca-preinitialization-20261022/PROCEDURE-PROPOSED.md): approved concrete procedure before productive initialization; actual initial protected backup after creation; verified isolated restore and remaining authority/resource proofs before issuance. No backup of a nonexistent key or previous CURRENT is demanded for initial bootstrap. All pre-effect applicable gates remain mandatory.

P2. Use wbadmin full-volume/bare-metal image plus offline BitLocker-protected independent media as the primary QUALIFICATION CANDIDATE. Official documentation demonstrates relevant tool capabilities, not recoverability of this installation's KSP state. Require target-specific image/restore evidence before calling the mechanism qualified. No portable private-key export, manual KSP file store, automatic fallback or change to export policy0.

P3. Prefer a local standard account dedicated to this installation as the distinct operational identity for the existing console host. Owner selects and provisions it separately through Windows; actual SAM SID and process TokenUser must be established and bound before productive use. No implicit LocalService/SYSTEM/Administrators selection. No SCM supervisor/service framework added merely to acquire a service SID. SCM-specific hosting remains a separately reviewed implementation choice.

P4. Preserve administrative operator versus runtime token isolation, existing key owner/borrows/Stop/quiescence and failure retention. Actual CA private-key denial with operational token and approved administrative use must be qualified; the current GENERIC_ALL operator ACL is not asserted to prove minimal rights. No effective source is fabricated from textual account/key names.

P5. Backup includes complete OS protection plus consistent same-root issuer/history/status/counters, two protected recovery generations and separately retained latest checkpoint evidence. Recoverable reporting depends on durable archive; recovery requires independently established latest state and fencing of all prior emitters. Unknown clones/high-water keep old-generation signing denied; no automatic new CA, rollback or regeneration. Rotation/retirement retain history and active/uncertain resources.

P6. Treat procedure commands as templates for separately authorized exact operations. No key/account/service/ACL/media configuration, backup/restore, certificate, bootstrap, CURRENT, private export or signature is authorized by this record. ED11 stays PARTIAL. No quotas/formats/wire/PST/Browser/production0.6.4 change.


## Normative compatibility and unclosed qualification

Preserve DECISION-internal-ca-technical-profile-20261018 sections2.4/3.1–3.7 and Owner conditions; issuer/custody clause6 and11; accepted personal F0–F9/P1; Model E/ED08 and ED11. None is edited or superseded. LAB G1–G5 remains LAB-only, no implicit productive non-overwrite-gate waiver.

Procedure approval before key creation (A), actual initial backup after authorized creation (B) and verified isolated restore before issuance (C) are separate. wbadmin is an image-backup candidate; two independent BitLocker-protected offline media are the approved procedural direction. Actual tools/media/OS/KSP restore and operational isolation remain NOT_QUALIFIED; real backup/restore remain NOT_RUN. Image recovery is protected OS-state recovery, not NCrypt private-key export or promised portability to another machine.

Rotation/retirement preserve same-authority history, revocations, burned serials, quiescence/borrows and uncertain retention. Independent latest-state and fencing remain required; an old valid image cannot resume signing. Failure cannot create automatic replacement CA or overwrite UNKNOWN.

Implementation, machine-specific source qualification, effective CA initialization, certificate generation, signing, bootstrap, CURRENT/publication and authoritative recovery are not authorized. No account/service/ACL/key/media alteration, backup/restore or operational test is authorized by this acceptance. Opaque provider/kernel and full process/host capacity remain unqualified; no quota increase.

PERSONAL_CA_PREINITIALIZATION_PROCEDURE=ACCEPTED_NORMATIVE_ONLY
BACKUP_MECHANISM=WBADMIN_CANDIDATE_NOT_QUALIFIED
OFFLINE_MEDIA=PROCEDURAL_TWO_BITLOCKER_PROTECTED_NOT_PROVISIONED
OPERATIONAL_IDENTITY=DEDICATED_LOCAL_STANDARD_ACCOUNT_DIRECTION_NOT_PROVISIONED
SCM_SERVICE_SID=NOT_SELECTED_NOT_QUALIFIED
KSP_RECOVERY=NOT_PROVEN
OPERATIONAL_ISOLATION=NOT_PROVEN
REAL_BACKUP=NOT_RUN
REAL_RESTORE=NOT_RUN
IMPLEMENTATION_AUTHORIZED=NO
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
PRODUCTION_PIN=0.6.4

## History

Revision1,2026-10-22: owner-approved direction, exact compatibility review and official write gate. Original proposals and previous STOP records preserved as history; qualification remains open.
