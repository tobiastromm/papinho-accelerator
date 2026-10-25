# Exact imported LAB dependency rejection and proposed correction

No ACL, owner, service or executable bytes changed. Evidence is read-only. The accepted Windows virtual-account decision V3/V4 requires protected installed code and least privilege; V5/V8 preserve gates and partial-effect retention. Section9 of the current owner task explicitly requires STOP for unsafe ACL. Existing parent remediation and two-file executable/configuration correction remain preserved, not repeated. No normative conflict or validator weakening is required.

## Observed objects

Both files are under C:\Projetos\PapinhoAccelerator\build\scm-protected-installation-binding-20261022\Selected\Release. They are direct candidate imports according to dumpbin /dependents, not speculative unused neighboring files. This inspection does not load the libraries or run their entrypoints.

| Object | Native identity | Bytes | SHA256 |
|---|---|---:|---|
| libcrypto-3-x64.dll |417614100:393216:1120009|7217664|09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044|
| libssl-3-x64.dll |417614100:393216:1119998|1321472|3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed|

Exact before/proposed SDDL: [dependency-acl-plan-PROPOSED.json](evidence/dependency-acl-plan-PROPOSED.json). Current owner for both: S-1-5-21-896877160-3262526087-2538948522-1003; current group: S-1-5-21-896877160-3262526087-2538948522-513. SE_DACL_PROTECTED is absent. These facts were read through the same handles used to identify/hash each file; no path-only ownership claim.

## Predicate comparison, both files

| Predicate | Observed | Result / purpose |
|---|---|---|
| Native ID / non-directory / non-reparse | IDs above, regular non-reparse files | PASS observation; binds exact objects |
| Valid descriptor and owner SID | Valid native descriptors and SID | PASS |
| scm_descriptor owner = approved operator, SYSTEM or Builtin Administrators | Foreign owner ending1003 | FIRST FAIL; foreign owner can affect protected ownership/DACL |
| scm_descriptor valid present non-null DACL | Present valid six-ACE DACL | PASS |
| No ALLOW modifying rights for unapproved principal | ACE0 foreign SID ending1002, ACE1 foreign SID ending941509878, ACE5 AU have0x1301bf | FAIL independently; permits modification/deletion of loaded code |
| Runtime protected-DACL requirement | D:AI, protected flag absent | FAIL independently; inheritance not frozen |
| SYSTEM and Administrators legitimate access | ACE2 BA and ACE3 SY FullControl | Existing permitted rights; preserve |
| Users reading/executing | ACE4 BU0x1200a9 | Permitted RX; preserve |
| Inherited ACE alone unsafe | All six ACEs ID | Not a rejection by itself; principal and mask determine unsafe grant |

0x1301bf is Modify. Reducing it to0x1200a9 removes DELETE, WRITE_DATA, APPEND_DATA, WRITE_EA and WRITE_ATTRIBUTES while preserving read/execute/synchronize and READ_CONTROL. It does not introduce WRITE_DAC/WRITE_OWNER or DELETE_CHILD. Parent rights, actual service token and all effective access remain separately governed; this is not a generic permission recipe.

## Minimum correction PROPOSED, not executed

Only the two exact native file objects above: owner becomes already-approved administrative operator S-1-5-21-785699550-1440458426-1682014700-1108; group unchanged. Convert current six inherited ACEs to explicit equivalents, protect each file DACL, reduce only ACE0/1/5 Modify to RX, preserve BA/SYSTEM FullControl and BU RX. Preserve principals, masks otherwise, order and bytes. No parent/child propagation, no CA/service ACL change, no deletion or binary replacement. Protected DACL AI flag may differ only under the already-reviewed semantic verifier rule; all other owner/group/ACE/control/identity differences must reject.

This exact proposed descriptor has NOT been applied or authorized as an effect. Future remediation requires official exact operation approval and manual elevated owner confirmation. Before any effect, reopen protected native handles and compare current ID/hash/descriptor with this evidence; divergence stops. After any effect, read back semantically, run the unchanged descriptor validator, check hashes and legitimate actual operational RX/no-write access. Partial failure is retained/UNKNOWN, no retry/rollback presumed.

Do not repeat old parent or exe/conf ACL launchers. The current task ends at STOP. After focused DLL correction, requalify the dependency closure (including actual resolved runtime DLL paths and system runtime loading) before arming the existing-service rebind/Start/Stop plan. Library imports are not proof of provider initialization, zero provider cost, or no loader effects. ED11 PARTIAL; actual process/thread/stack/token/kernel/provider costs and retained coexistence remain UNKNOWN.
