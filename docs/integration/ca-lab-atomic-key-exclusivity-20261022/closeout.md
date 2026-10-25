# CA LAB atomic exclusivity review — 2026-10-22

Task PACC-PHASE6-CA-LAB-ATOMIC-KEY-EXCLUSIVITY-1. Review/preparation only; no KSP query, native creation/finalization, UAC, build/test, certificate, bootstrap or operational network use. Official documentation browsing was explicitly requested.

## Official evidence

Without overwrite, a named-key collision produces NTE_EXISTS. MACHINE selects computer scope. The API returns a handle requiring later completion before use; releasing that handle is distinct from deletion. It does not specify a prior absence query as a prerequisite. [NCryptCreatePersistedKey](https://learn.microsoft.com/en-us/windows/win32/api/ncrypt/nf-ncrypt-ncryptcreatepersistedkey).

Finalization is separate. Its status list is non-exhaustive; SILENT requests no provider UI. No general rollback guarantee follows from failure. [NCryptFinalizeKey](https://learn.microsoft.com/en-us/windows/win32/api/ncrypt/nf-ncrypt-ncryptfinalizekey).

Microsoft describes software KSP isolation and storage resources, but supplies neither a resource byte bound nor a linearization proof for concurrent pending creations. [Microsoft KSP architecture](https://learn.microsoft.com/en-us/windows/win32/seccng/key-storage-and-retrieval).

These references establish a provider-side non-overwrite contract, not an indivisible create/set/finalize transaction, global absence/history proof, fencing, or namespace reservation throughout concurrent finalization. The exact persistence point and artifacts after failed calls are not proven here. Stronger concurrency guarantees remain unqualified.

## Existing code

server_ca_key_win32.c:147–162 already calls CreatePersistedKey with MS_KEY_STORAGE_PROVIDER, RSA, exact owner name, legacy spec0, and exactly NCRYPT_MACHINE_KEY_FLAG. No overwrite. Failure exits before setters/finalization; created=1 follows success. Length2048/export0/signing usage/DACL precede Finalize(SILENT). Attempt and confirmation are separate diagnostics; public validation precedes READY. Close is not deletion; uncertain cleanup retains ownership. No source, binary or flags changed. NTE_EXISTS and every other native error are STOP; error lists are not exhaustive. No signature/private export or artificial success test is used.

## Contract reconciliation

Accepted bootstrap P3/P4 permits observed exclusive creation plus custody as local evidence without global history; its subject is the store container, so it cannot prove key-provider atomicity by analogy. Accepted D2 explicitly says prior absence checking does not replace exclusive creation at commit. Neither establishes an absolute absence oracle for each native call.

Issuer/custody clause6 and bootstrap P8 prohibit replacement/retry around uncertain initialization. They remain binding. The old LAB key stays UNKNOWN and is not reused, deleted or replaced. A separately scoped disposable LAB experiment is proposed, not a productive recovery. No unrestricted new-CA permission is inferred from P3. [G1–G5 PROPOSED](LAB-NONOVERWRITE-GATE-PROPOSED.md) delimits the interpretation for official review; if clause6/P8 is judged to prohibit even that independent experiment, STOP requires an explicit designated exception.

The immediate requirement for prior absence by query was section3 of the preceding LAB task, not a discovered NCrypt requirement. Its replacement is therefore proposed explicitly, without changing accepted ADRs. Previous reports/plans/launcher remain byte-exact and their STOP remains effective until review/adoption and exact launcher correction. No functional patch is applied automatically.

## Next operation and resources

Proposal: fixed prepared new name; manually elevated eligible expected operator and real console; pinned exact plan/binary; one consumed attempt; existing no-overwrite create; collision/any failure STOP; setters/finalization only after create success on the same owned handle; separate factual results; uncertainty retained, no automatic retry/rename/provider fallback. Future exact launcher hunks and manual execution are still required. No raw effectful command is authorized now.

No resource was acquired in this task. Old UNKNOWN persisted debt remains charged independently. Future provider/key, stack, buffers, runtime and retained resources have their real multiplicities; provider/kernel byte costs remain UNKNOWN, not zero. No complete ED11 margin or quota increase. Model E/ED08 factual quiescence/closure and ED11 W4/issuer11 remain unchanged.

Branch main, HEAD913c90cf2328f7939d8f5b927640601903ac6e91. Existing WIP, four historical LABs, sources, binaries, accepted contracts and receipts preserved; only new documents and appended capability note. PST, Browser, wire, production0.6.4 unchanged. No clones/worktrees/commit/push/tag/release.

## Result

CURRENT_TASK_COMPLETED=YES
ATOMIC_EXCLUSIVE_CREATION=NON_OVERWRITE_DOCUMENTED_FULL_SEQUENCE_ATOMICITY_NOT_PROVEN
KSP_COLLISION_SEMANTICS=NTE_EXISTS_WITHOUT_OVERWRITE_STOP
PRIOR_ABSENCE_PROOF_REQUIRED=NOT_BY_API_LAB_REPLACEMENT_PROPOSED
CONTRACT_COMPATIBILITY=LOCAL_CREATION_COMPATIBLE_INDEPENDENT_LAB_SCOPE_PENDING_REVIEW
LAB_GATE_CORRECTION=G1_G5_PROPOSED_NOT_APPLIED
NEW_LAB_EXECUTION=NOT_RUN
PREVIOUS_LAB_KEY_DISPOSITION=UNKNOWN
CA_CUSTODY_INITIALIZATION=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=FOCAL_LAB_GATE_ADOPTION_AND_NATIVE_QUALIFICATION_PENDING
NEXT=CA_CUSTODY_ELEVATED_LAB_FINALIZATION

NEXT is conditional on exact review/adoption and manual execution, not permission to run now.
