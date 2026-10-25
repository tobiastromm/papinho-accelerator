# CA NCryptFinalizeKey NTE_PERM — focused diagnosis and STOP

Task PACC-PHASE6-CA-NCRYPT-FINALIZE-NTE-PERM-1, 2026-10-22.
main/913c90cf2328f7939d8f5b927640601903ac6e91 unchanged.
CURRENT_TASK_COMPLETED=NO. The exact failing API is proven; a current environmental access barrier is proven. Unique historical root cause and previous key disposition remain unresolved. No new key or production-source correction was justified/applied.

## Previous attempt and inspected call sequence

Existing immutable evidence: [previous closeout](../personal-first-install-bootstrap-completion-20261022/closeout.md), lab-ca-result.json and evidence/ca-lab-result.log. This is the October22 object, NOT the unrelated historical October19 UNKNOWN key. The older key and diagnostic artifacts were not queried or modified.

Exact object: Microsoft Software Key Storage Provider; MACHINE; PapinhoAccelerator.CA.cb87632463bb48dd8a34fa8ea6f3911b.030fa34dd44f41e8b143b9da09532cf2. Authoritycb87632463bb48dd8a34fa8ea6f3911b, generation030fa34dd44f41e8b143b9da09532cf2. LocalService S-1-5-19 is the explicitly chosen old LAB exclusion, never assumed to be the real product service identity.

Source apps/server/server_ca_key_win32.c was inspected unchanged, SHA256fa00439757c18ea1ba993d24c718e72a00a039a45a96bffc4cba33ca6fe7be10. Header19868a000fa1778452b13958b93bda0c145bae06ddeaa4e7d5d796ab638dd5a3. The sticky first-failure record proves FIRST failure BEGIN_CREATE/function1, FINALIZE/stage20, NATIVE_NCRYPT/API7, original0x80090010=NTE_PERM. It is not a synthesized local validation error. All preceding native operations on that reached path returned success; this does not prove completed persistent initialization.

| Operation | Exact source arguments/invariant | Observed/reconstructed scope |
|---|---|---|
| Identity | Actual thread/process TokenUser, configured operator match, distinct service SID | Passed before provider; historical SID/elevation/LUID were not logged |
| OpenStorageProvider | MS_KEY_STORAGE_PROVIDER, flags0 | Success before the first failure |
| CreatePersistedKey | RSA, exact name, legacy spec0, MACHINE0x20; no OVERWRITE | Success, created1; not a finalized/exclusive-persistent-absence proof |
| Set length/export/usage | DWORD2048, DWORD0, ALLOW_SIGNING0x2; sizes4, flags0 | Success before first failure |
| DACL | D:P deny GA exact LAB service; allow GA operator and SYSTEM; bounded1024byte self-relative descriptor | Conversion and SetProperty returned success, not validated readback |
| Set security descriptor | DACL_SECURITY_INFORMATION0x4 OR NCRYPT_PERSIST_FLAG0x80000000 | Success before first failure |
| FinalizeKey | NCRYPT_SILENT_FLAG0x40 | NTE_PERM0x80090010, finalization1 attempted/unconfirmed |
| Validation/export/borrow | Existing owner validation, public export; planned private negative/borrow test | NOT_REACHED |
| Failure cleanup | Existing checked FreeObject key then provider, LocalFree SD/SID references | Receipt stateCLOSED3, key/provider0; not persistent deletion |

No source evidence establishes a wrong parameter, unsupported sequence, wrong KSP, collision, invalid ACL or provider restriction as the unique cause. Moving DACL across finalization, using USER scope, changing masks/provider, dropping SILENT or elevating creation would be experiments, not a proven correction. None was done. There is no fallback or retry, and the previous name remains frozen for reconciliation only.

## Current contexts, separated from historical facts

Native token probes compare three distinct observations:

- Default sandbox: SID S-1-5-21-896877160-3262526087-2538948522-1003, not elevated, elevation type1, no thread token. It is not the owner F10 operator.
- Official executor mode used for the new read-only query: SID S-1-5-21-785699550-1440458426-1682014700-1108, TokenElevation=false, TokenElevationTypeLimited3, session1, no thread token. AuthenticationId0:430209 and token statistics were recorded; token close confirmed.
- Preserved F10 receipt48091b1698c841e6a63bb11ee5b0ab30: same Tobias SID as the official executor, eligible=true, interactive_console=true. Its actual native ceremony requires enabled Administrators and elevated token. It proves that prior read-only invocation only; it is not this process/token or a CA authorization.

The failed October22 LAB source used actual process TokenUser, but did not log SID/elevation/LUID. The same official execution mechanism now yields filtered Tobias. This supports an environmental hypothesis, not retrospective proof of every bit of the historical token. Do not identify the default sandbox SID as the failed LAB actor, or claim F10's elevated token was inherited.

## Exact-key read-only query

The existing readonly diagnostic source was rebuilt in a new directory with PAPACC_CA_READ_ONLY_DIAGNOSTIC applied to BOTH translation units, MSVC19.51 x64 /W4 /WX /O2 /MD, no warnings. No product source/build artifact was replaced. Binary build/ca-finalize-readonly-diagnosis-20261022/readonly_diagnostic.exe SHA2568cf1e4a6057033594bc761e115b0e8d27344f1ac5f3814a337eb609d325c401b.

Imports are only NCryptOpenStorageProvider, NCryptOpenKey, NCryptGetProperty, NCryptExportKey and NCryptFreeObject. Source's sole export selector is BCRYPT_RSAPUBLIC_BLOB. No Create/Set/Finalize/Delete/RNG or private export path is reachable in this compiled variant. The officially approved exact query pinned this binary, the full name above, Tobias SID and old LAB service SID. It did not enumerate keys or open private key files.

Observed: OpenStorageProvider succeeded; NCryptOpenKey(MACHINE|SILENT) failed with0x80090016=NTE_BAD_KEYSET, first failure function2/stage12/API3. Provider was observed owned in the first receipt and CLOSED with close_confirmed1 in the final receipt; no key handle was acquired. Diagnostic exit4 is a query failure, not successful absence proof. PREVIOUS_LAB_KEY_DISPOSITION=UNKNOWN. No alternate user scope, guessed name, delete, reopen retry, namespace overwrite or new key followed.

## Read-only access evidence

The directory security descriptor at C:\ProgramData\Microsoft\Crypto\Keys is O:SYG:SYD:PAI(A;OICI;FR;;;WD)(A;OICI;FA;;;SY)(A;OICI;FA;;;BA). A read-only AccessCheck of FILE_ADD_FILE0x2 using a nonactivated duplicate of the official process token returned allowed=false/granted0. No thread impersonation was set. SD LocalFree and duplicate/process-token closes were confirmed in the successful observation.

The first AccessCheck probe requested only DACL and returned ERROR_INVALID_SECURITY_DESCR1338. It was not treated as a KSP error or permission verdict. The probe was corrected to request OWNER|GROUP|DACL and then succeeded with access denied. This changed only the read request, not ACL/policy/token rights. No file was created in the store, and no KSP private file was read. Initial probe cleanup was attempted in finally; its return values were not preserved, so no additional factual cleanup claim is made for that failed probe.

This proves a current direct machine-store write-access barrier for the filtered context. It does not prove that the provider's internal execution path or the earlier process failed solely because of that directory DACL. KSP policy, internal persistence checks and the unresolved object's namespace remain unqualified. Cause therefore remains UNRESOLVED, with insufficient current context as a demonstrated precondition gap. There is no evidence justifying a product patch.

## Prepared manual next observation, not new generation

A new focal launcher [ca-finalize-readonly-uac.ps1](../../../tools/qualification/ca-finalize-readonly-uac.ps1) is approved for manual use. It follows the existing launcher pattern: fixed canonical plain paths, held read-only script/executable streams, SHA256 verification, native UAC RunAs only in explicit Launch, effective Tobias SID/elevated enabled-Administrator/no-impersonation/interactive-console checks, controlled arguments, no passwords and no fallback. Other elevated-user credentials deny before key access. UTC in receipts is an unqualified log annotation only.

Manual command:

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\tools\qualification\ca-finalize-readonly-uac.ps1' -Mode Launch
```

This command queries ONLY the previous exact object and writes a nonsecret receipt under manual-results. No key generation/deletion/SetProperty, certificate, bootstrap, CURRENT or network. F10 closes the result window. Outer0 means a complete observed query/closed-handles receipt, never custody PASS or absence. Partial/retained/interrupted output or missing close confirmation returnsDENY2. A successful key open can record PRESENT; BAD_KEYSET remains UNKNOWN, never automatically authorizes reuse/generation. Cancellation/UAC launch failure denies. No Launch, native elevation, credential prompt or F10 repeat was executed by the agent.

PowerShell5.1 final parser errors0; Check returns0 without key query; missing script hash and ineligible/no-console Elevated calls deny2 before native key access. These are real negative context tests, not mocked elevated eligibility. Positive elevated query/cancellation/window/receipt path remains manual/unexecuted. The first launcher staging command had an outer PowerShell here-string parse error before any file write; corrected staging was separately reviewed, and final script parses successfully.

Only this nonmutating prerequisite is currently prepared. A new positive elevated CA initialization would require resolved previous disposition, a new separately approved name/IDs, absence/collision proof, actual target/operator/service/custody/resource binding and exact cleanup. None is authorized/executed by this launcher. Elevation has not been proven sufficient to fix finalization. No new normative proposal or architecture was introduced.

## Ownership, ED11 and preservation

Existing CA owner/first-error/cleanup API and all production sources remain byte-exact. Read-only harness owns at most one KSP and one opened key reference plus its existing token/validation/temporary SID resources. In this query only a provider was acquired and its close was confirmed; no key was borrowed, no native work remains reported active in that receipt. Public validation scratch is unreachable on this failed-open observation, but remains part of the diagnostic path's potential cost. Provider/kernel memory, complete nested stack/CRT/PowerShell runtime, initial failed probe cleanup and previous persistent disposition remain UNKNOWN. No unknown is charged as zero and no complete operational bound/margin is claimed. Production root layout/quotas unchanged, ED11 PARTIAL.

HEAD and preimages: baseline1301 old WIP files unchanged before this documentation/live append. Functional write scope is only the new manual read-only launcher. Baseline includes product CA source/header, old F10 UI/launcher/executable/receipt and previous LAB evidence, all preserved. No PST, Browser, wire, production0.6.4, four historical LABs, clones/worktrees, commits/push/tags/releases, key creation/deletion/ACL changes, bootstrap, certificates or network. Official executor approvals covered token/SD probes, exact key query and launcher hunks; no automatic review rejection was bypassed.

## Result

CURRENT_TASK_COMPLETED=NO
NCRYPT_FAILURE_CAUSE=UNRESOLVED_CURRENT_FILTERED_MACHINE_WRITE_ACCESS_DENIED
PREVIOUS_LAB_KEY_DISPOSITION=UNKNOWN
KSP_SCOPE=MICROSOFT_SOFTWARE_KSP_MACHINE
TOKEN_CONTEXT=CURRENT_OFFICIAL_TOBIAS_FILTERED_NOT_F10_ELEVATED_HISTORICAL_DETAILS_UNRECORDED
MINIMUM_CORRECTION=NO_PRODUCT_PATCH_PROVEN_MANUAL_ELEVATED_READ_ONLY_QUERY_PREPARED
ELEVATED_LAB_TEST_REQUIRED=YES
CA_CUSTODY_INITIALIZATION=DENY_UNTIL_QUALIFIED
INITIAL_ROOT_PREPARATION=NOT_RUN
BOOTSTRAP_EXECUTION=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=PREVIOUS_LAB_KEY_NAMESPACE_AND_KSP_FINALIZE_CAUSE_UNPROVEN
NEXT=CA_CUSTODY_NATIVE_LAB_QUALIFICATION


## Subsequent owner observation

Owner elevated read-only execution is now verified: full elevated expected SID, Administrators enabled and interactive console; wrapper0, child query4/NCryptOpenKey BAD_KEYSET0x80090016, confirmed closed provider. Disposition and original finalization cause remain UNKNOWN; no new generation, deletion or authority. Earlier unexecuted statements above describe the pre-manual state. See [verified manual result](manual-elevated-observation-20261022.md).
