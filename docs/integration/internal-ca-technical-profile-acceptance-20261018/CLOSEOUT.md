# Exact technical-profile acceptance closeout

TASK=PACC-PHASE6-INTERNAL-CA-TECHNICAL-PROFILE-ACCEPTANCE-1.
2026-10-18. Conditional owner acceptance reviewed against exact proposal and accepted issuer/manual, ADR0006/0028, provisioning, Model E and ED08/ED11 constraints. No material conflict identified within this acceptance scope. No code implementation or runtime qualification.

## Official approval and provenance

[Accepted record](../../adr/DECISION-internal-ca-technical-profile-20261018.md) SHA2569800d5d8d1271a5b616e810502c91fb91cb2bcdac990e5f8208d3e9799c3dab2 was staged with exact bytes, submitted through require_escalated official executor review and approved. CreateNew prevented overwrite; persisted hash matched. Proposal SHA256cb5f4c480221eeec8c238192ba939a49c19c226b92a49832dc7ef5610aeb8b10 preserved unchanged. Three technical sections/eighteen numbered clauses compared case-sensitively identical after persistence. Source headings retained explicitly for exact tracing, with accepted status defined by new record.

The record adds acceptance metadata, owner conditions and remaining gates, not new technical parameters. Prior proposal/handoff/closeout and all earlier accepted records stay unchanged. Historical reports remain historical, not rewritten as runtime PASS. Implementation handoff is not accepted wholesale by reference.

## Review matrix

| Owner condition | Exact proposal / accepted scope | Result |
|---|---|---|
| Distinct CA/server/client/package purposes | 1.1–1.3, separate keys, CA pathLen0/keyCertSign+cRLSign, serverAuth/clientAuth/codeSigning | CONFORMANT |
| RSA2048/SHA256, no silent algorithm downgrade | 1.1, fixed initial profile and explicit migration | CONFORMANT |
| Browser no CNG dependency, NT4 real gate | 1.6,3.1; CNG administrative provider only modern Accelerator | CONFORMANT_NOT_QUALIFIED |
| Unique persistent serial/reserve before sign | 2.1–2.2, prefix8/counter8, durable request/root CAS before signature | CONFORMANT |
| Crash/uncertain/idempotent history | 2.3–2.5, burn/pending, exact-correlated query, high-water and restore fencing | CONFORMANT |
| No serial reuse to compensate failure | No reallocation/rebinding of burned serial. 2.3 concerns only original reservation and conclusive no-prior-signature recovery, not a new issue retry while uncertain | CONFORMANT_SCOPED_RECONCILIATION |
| Concrete CNG provider qualification | 3.1 MS Software KSP is initial candidate, must select/read back/test actual OS/provider before use | CONFORMANT_QUALIFICATION_PENDING |
| Nonexportable administrative key isolation | 3.1–3.4, export policy0/sign-only, distinct actual SID/token/context, key binding/ACL readback, no handler access | CONFORMANT_NOT_ABSOLUTE_HOST_PROTECTION |
| Backup before productive initialization/use | Approved procedure first; 3.5–3.6 baseline, protected restore/correspondence/history/high-water and actual recovery proof | CONFORMANT_PRODUCTIVE_INIT_BLOCKED_UNTIL_COMPLETE |
| Inadequate recovery alternative governed | 3.6 plus explicit owner condition: deny then submit alternative, no weaker silent export | CONFORMANT |
| Future replaceability | 3.7 and existing purpose boundaries; public history preserved, no TrustStore implementation/dependency | CONFORMANT |
| No real CA/code/network/admission/Phase6 closure | Explicit limits; package technical choices and active-Session policy remain independent | PRESERVED |

## Remaining gates and precedence

This technical decision supplies the concrete three-point choices left pending in prior accepted issuer/manual decision. It supplements, not globally supersedes ADR0006/0028 or provisioning obligations. Accepted manual custody exception and local automatic key rule remain unchanged. No change to operational mTLS, Principal/grants, root factual commit/CURRENT/epoch, ED08 closure or Model E ownership.

CNG API availability is not backend qualification. Key export restriction is software OS protection, not absolute protection against a privileged compromised host. Browser/VC6/NT4 full exact-profile qualification remains NOT_QUALIFIED. Backup baseline is accepted, actual KSP/tool/procedure/restore is not proved. Productive initialization must wait for approved complete recovery contract/procedure, and no real CA is authorized by this task in any event. No arbitrary new backup crypto or silent fallback if recovery cannot be established.

Same-root issuer persistence/live publication needs reviewed private codec/capacity and source implementation; existing first-dormant BODY05 is not sufficient registry history. Active Session revocation remains POLICY_PENDING. Package v2 algorithms/KDF/encoding/bounds remain PROPOSED separately. Provider/kernel UNKNOWN stays UNKNOWN; no quotas or admission changed. All six Phase6 provisioning gates remain OPEN and PST065 publication deferred.

## Preservation and verification

Main HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Initial WIP503 status entries captured in BASELINE.json.31/31 selected preexisting source/authority hashes unchanged, including original proposal, handoffs, earlier accepted decisions, root sources and pin. New accepted destination had no preimage; original proposal remains byte-exact. New files limited to accepted record and this documentary evidence folder; no historical outputs overwritten. git diff --check passed. Scoped inventory does not claim full fresh hashing of ignored binaries/outputs.

No source edits, builds, product tests, keys/certificates/credentials, CA init, native crypto operations, backup/restore execution, network, Browser/PST/PACC changes, Git config/ACL changes, clones/worktrees, commit/push/tag/release. Documentary operation only; no runtime ED11 delta. Official accepted-record write approved, no denial bypass. Closeout/baseline/manifest writes separately submitted through same executor approval flow.

CURRENT_TASK_COMPLETED=YES
INTERNAL_CA_TECHNICAL_PROFILE=ACCEPTED
X509_PROFILE=RSA2048_SHA256_DISTINCT_PURPOSES
SERIAL_PERSISTENCE=PREFIX8_COUNTER8_DURABLE_RESERVATION_BEFORE_SIGNATURE
CNG_KSP_BACKEND=MS_SOFTWARE_KSP_INITIAL_CANDIDATE_CONCRETE_VALIDATION_REQUIRED
BACKUP_RECOVERY=BASELINE_ACCEPTED_APPROVED_PROCEDURE_AND_RESTORE_QUALIFICATION_PENDING
NT4_COMPATIBILITY=NOT_QUALIFIED
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
WIP_PRESERVED=YES_SCOPED_HASH_AND_WRITE_SCOPE_VERIFICATION
EXECUTOR_APPROVALS=OFFICIAL_ACCEPTED_RECORD_APPROVED
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=NONE_WITHIN_EXACT_ACCEPTANCE_SCOPE
NEXT=SCOPED_INTERNAL_CA_SOURCE_IMPLEMENTATION