---
status: proposed
revision: 1
decision-date: 2026-10-22
scope-level: project
scope-target: PapinhoAccelerator personal Windows CA preinitialization
supersedes: []
---
# ADR candidate — personal CA procedure and dedicated operational identity

Task PACC-PHASE6-PERSONAL-CA-PREINITIALIZATION-1. For exact owner/official review; no normative acceptance or execution authorization inferred.

## Decision proposed

P0. Limit this procedure to personal/local Windows installation using the existing Microsoft Software KSP MACHINE CA owner. Preserve portable contracts and all accepted authority/ED11 gates. No global Windows/backend requirement.

P1. Adopt the three distinct gates A/B/C in [procedure revision1](../integration/personal-ca-preinitialization-20261022/PROCEDURE-PROPOSED.md): approved concrete procedure before productive initialization; actual initial protected backup after creation; verified isolated restore and remaining authority/resource proofs before issuance. No backup of a nonexistent key or previous CURRENT is demanded for initial bootstrap. All pre-effect applicable gates remain mandatory.

P2. Use wbadmin full-volume/bare-metal image plus offline BitLocker-protected independent media as the primary QUALIFICATION CANDIDATE. Official documentation demonstrates relevant tool capabilities, not recoverability of this installation's KSP state. Require target-specific image/restore evidence before calling the mechanism qualified. No portable private-key export, manual KSP file store, automatic fallback or change to export policy0.

P3. Prefer a local standard account dedicated to this installation as the distinct operational identity for the existing console host. Owner selects and provisions it separately through Windows; actual SAM SID and process TokenUser must be established and bound before productive use. No implicit LocalService/SYSTEM/Administrators selection. No SCM supervisor/service framework added merely to acquire a service SID. SCM-specific hosting remains a separately reviewed implementation choice.

P4. Preserve administrative operator versus runtime token isolation, existing key owner/borrows/Stop/quiescence and failure retention. Actual CA private-key denial with operational token and approved administrative use must be qualified; the current GENERIC_ALL operator ACL is not asserted to prove minimal rights. No effective source is fabricated from textual account/key names.

P5. Backup includes complete OS protection plus consistent same-root issuer/history/status/counters, two protected recovery generations and separately retained latest checkpoint evidence. Recoverable reporting depends on durable archive; recovery requires independently established latest state and fencing of all prior emitters. Unknown clones/high-water keep old-generation signing denied; no automatic new CA, rollback or regeneration. Rotation/retirement retain history and active/uncertain resources.

P6. Treat procedure commands as templates for separately authorized exact operations. No key/account/service/ACL/media configuration, backup/restore, certificate, bootstrap, CURRENT, private export or signature is authorized by this record. ED11 stays PARTIAL. No quotas/formats/wire/PST/Browser/production0.6.4 change.

## Frozen proposal for review

Procedure revision1 SHA256: 284f1fd1b246e1abdff246e5260667e75bfd2aea0362b35566cdc4f6842fd72d

Accepted authorities: DECISION-internal-ca-technical-profile-20261018 sections2.4/3.1–3.7 and Owner conditions; DECISION-internal-client-issuer-and-manual-custody-20261018 clauses6/11; DECISION-personal-first-install-authorization-f0-f9-20261022; P1/bootstrap; Model E/ED08; ED11 decisions. None amended or superseded.

## Compatibility and remaining gates

Nonexportability through NCrypt and OS-state image recovery are not logically contradictory. Success on another hardware/OS/KSP combination is not promised. If protected restore fails, the candidate cannot satisfy3.6; preserve DENY and request governed alternative without export weakening.

No material normative contradiction identified in this proposal. Concrete host/media/custodian choices, empirical recovery capability, operational account binding, actual access isolation, latest-state/fencing and full ED11 remain unqualified. Approval of policy is separate from completion of those proofs; technical approval does not approve unknown machine-specific facts.

## Acceptance state

PROPOSED. This task requests review, not automatic acceptance of candidate tools/identity sources. Exact procedure hash must be rechecked for any eventual acceptance record. Preserve original proposal. Implementation and effects require separate authorization.

IMPLEMENTATION_AUTHORIZED=NO
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
