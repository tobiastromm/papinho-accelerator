# CNG native permission diagnosis and exact-key reconciliation

TASK=PACC-PHASE6-CNG-NATIVE-PERMISSION-AND-TEST-KEY-RECONCILIATION-1.
Date2026-10-19. Read-only native diagnosis completed to the limit of existing evidence. Exact historical failure origin remains unresolved; it cannot honestly be reconstructed by assigning an API name to aggregate status. No generation repeated, no source/ACL/security changes, no key deletion or private export.

## Identity and evidence

Current query context: TCORP-TOBIAS\CodexSandboxOffline, SID S-1-5-21-896877160-3262526087-2538948522-1003, NTLM, medium integrity S-1-16-8192. No impersonation/thread impersonation; Administrator role false; only reported privilege SeChangeNotifyPrivilege. These are current factual query observations, not a captured token/integrity receipt from the previous fixture. Native fixture source uses its actual TokenUser as operator and real LocalService SID S-1-5-19 as proposed service identity, not an impersonated service test.

Baseline main HEADc50b8fb9a7e3293b5e6355fc08459bf85b56857a, initial WIP509 entries. Source .c SHA256a8a03fbc6a8eb18607cd6c7629deb0d0253bdb218763584f0055f2cedf7afac3, header9b65157460f08639dcdaeb60b4d1efe355e307c1f732b23579a24254cee7a2f5, test0773407282bba41ee535e4501b65fd5b532fea45ee053207ba19f92aa36f08c2. Selected current Debug executable3ff9b19e288596f80ffb2acad0918b10e4a3382e0324e70669e154e564dc5ecf. Existing build logs/source are consistent artifacts; no new runtime execution/binary provenance assertion is fabricated for the historical process.

Historical fixture returned status0x80090010/state3/created1 then exit20. It printed no per-API receipt. Native original API error and locally synthesized policy failure both use NTE_PERM. The statement “NCrypt returned NTE_PERM” is therefore NOT established. The precise valid statement is “the backend operation returned NTE_PERM after successful NCryptCreatePersistedKey”. No automatic sandbox, filesystem ACCESS_DENIED, elevation or KSP incompatibility diagnosis.

## Historical path reconstruction

| Stage / code location | Exact parameters or condition | What the previous evidence permits |
|---|---|---|
| ca_identity before provider, .c12–23/92 | actual TokenUser must equal configured admin; distinct valid service SID | Passed on the path reaching created1, source-derived; historical token details not logged |
| NCryptOpenStorageProvider, .c97 | MS_KEY_STORAGE_PROVIDER, flags0 | Success on reached path, source-derived |
| NCryptCreatePersistedKey, .c101–102 | NCRYPT_RSA_ALGORITHM, exact name below, legacyKeySpec0, NCRYPT_MACHINE_KEY_FLAG0x20, no OVERWRITE | Returned success because created1 set only afterward; NOT proof of finalization/persistence |
| NCryptSetProperty length, .c103 | NCRYPT_LENGTH_PROPERTY, DWORD2048, length4, flags0 | Reached unless this is first failure; exact return unknown |
| NCryptSetProperty export, .c104 | NCRYPT_EXPORT_POLICY_PROPERTY, DWORD0, length4, flags0 | May not have been reached; return unknown |
| NCryptSetProperty usage, .c105 | NCRYPT_ALLOW_SIGNING_FLAG0x2, length4, flags0 | May not have been reached; return unknown |
| SID/SD conversion, .c106–109 | SDDL D:P deny GA LocalService; allow GA operator and SYSTEM; bounded1024byte SD | Failures map to NTE_FAIL, not final NTE_PERM; they cannot alone explain final status |
| NCryptSetProperty security descriptor, .c110 | NCRYPT_SECURITY_DESCR_PROPERTY, generated SD and n bytes, DACL_SECURITY_INFORMATION0x4 OR NCRYPT_PERSIST_FLAG0x80000000 | Candidate native failure, not proved; actual n/descriptor bytes not recorded |
| NCryptFinalizeKey, .c111 | NCRYPT_SILENT_FLAG0x40 | Candidate native failure, not proved |
| Validation ca_identity, .c64 | Requery effective thread/process token and compare operator SID | Can synthesize NTE_PERM locally after creation; no evidence excludes this branch |
| NCryptGetProperty, .c34/65–67 | length, export-policy, usage with flags0; exact DWORD values | Native candidate errors; successful values not logged |
| NCryptExportKey, .c68 | BCRYPT_RSAPUBLIC_BLOB only,512byte buffer, flags0; checks RSA2048/e65537 | Public-only native candidate, not private export; result unrecorded |
| NCryptGetProperty DACL, .c41 | Security Descr,1024byte buffer, DACL_SECURITY_INFORMATION | Native candidate error, not proved |
| Local DACL checks, .c53/57 | Reject unexpected allowed SID or absence of literal GENERIC_ALL bits | Synthesizes NTE_PERM even if every native operation succeeded; masks/ACEs not logged |
| Native/local cleanup, .c80–82/116–119 | checked FreeObject, LocalFree, CloseHandle | state3 means backend CLOSED according to code, not persisted-key deletion; no key-delete step was reached |

The local DACL code compares literal GENERIC_ALL bits rather than recording effective mapped rights. Whether the selected KSP canonicalizes generic masks is a hypothesis requiring observed readback; current evidence does not prove that was the failed branch. Do not “fix” by accepting wider ACLs, ignoring rights or changing the intended policy. Likewise moving DACL assignment across FinalizeKey or elevating creation cannot be justified by this aggregate error alone.

Permission categories: token query needs TOKEN_QUERY; machine key creation/finalization obey KSP machine-store and effective-token policy; security-descriptor modification needs the provider's applicable security-descriptor modification rights (DACL changes normally require WRITE_DAC/owner authority), not inferred admin membership from a display name. Readback needs applicable read-security/property rights. No permission grant/elevation is applied here. Current medium nonadmin context is relevant evidence, not proof of a particular rejected operation or that elevation is the correct remedy.

## Exact-key read-only inventory

Provider: Microsoft Software Key Storage Provider. Scope: MACHINE, not a user-key fallback.
Name: PapinhoAccelerator.CA.e6a180fbd0341839ab9d28f0aa6008c0.20c434ce21eec7524eb33c9268e67b2a.

1. Local certutil -key help was read. Query used only -silent -csp exact provider -key exact name, without -privatekey/-user/-split/-pin. Output reported LoadKeys NTE_BAD_KEYSET0x80090016; process exit0. Thus exit0 is not a successful key-load receipt.
2. Existing .NET CngKey.Open with same exact provider/name, MachineKey|Silent options failed with CryptographicException HRESULT0x80090016. No CngKey.Create, signing, export or key property mutation called. No new compiler/Add-Type/helper binary was used.

Result: no accessible persisted key found under current executor at this machine provider/name. This is NOT proof of global absence/deletion under all ACL/identities; classification remains KEY_STATE_UNKNOWN rather than KEY_ABSENT_CONFIRMED. No alternative scope/provider/token was tried. No owner/DACL, algorithm, key usage, export policy, SPKI or finalization could be read because opens failed. Requested values in source are not observed properties. Native key finalization/absence cannot be inferred from closing previous handles. No key handle was acquired by these two failing opens, and no pending operation/global handle inventory is established. Process exit is not proof of persistent-key cleanup.

KEY_CLEANUP_REQUIRED=NOT_ESTABLISHED. KEY_CLEANUP_AUTHORIZED=NO. Never delete an inaccessible/unconfirmed object by guessing its KSP file.

## Minimum next correction — proposal only, source unchanged

The proved implementation defect for diagnosis is loss of first-failure step/origin and underlying Win32 status. Minimum next authorized source change is a bounded diagnostic receipt in existing owner: last-attempted API/local guard, last-successful step, first-failure origin (NATIVE_NCRYPT/WIN32/LOCAL_IDENTITY/LOCAL_DACL/LOCAL_PROPERTY), raw status, underlying GetLastError only for failed Win32 calls, property identifier/flags/length, and separate cleanup receipt. Preserve first cause across cleanup, omit keys/passwords/proofs/SD blobs. Add markers around the candidate calls/guards listed above, without modifying order, values, ACL policy or control results. Account new receipt bytes in ED11 before further tests. No new collector/supervisor, no native generation is authorized by this diagnostic handoff.

No parameter-remediation patch is claimed: incorrect parameter or rights mapping has NOT been established, so producing a “fix” that moves/weakens guards or privileges would be speculative. A future exact instrumentation patch/review can be prepared separately. Read-only inventory cannot retroactively prove the historical API; a new isolated creation run would require its own explicit authorization and diagnostic receipts, not automatic replay. New key/ACL/admin context changes must not be used just to force PASS.

## Proposed cleanup procedure — NOT authorized for execution

- First obtain a successful read-only exact-object inventory under an authorized identity without weakening ACLs. Record provider/machine scope/full name, key public identity, owner/DACL and observable properties, and match the preserved test creation evidence. Current failure to open is not enough.
- If absent can be conclusively proved in the intended scope, record that fact and do no delete/create. If still unknown, preserve unknown; no filesystem probing/deletion of KSP private files.
- Only if presence and exclusively diagnostic identity are proved, submit a distinct owner authorization and official exact operation for one key retirement using existing provider API, with observed identity revalidation and checked native disposition. No wildcard, prefix-based cleanup, key overwrite, private export, ACL change or “delete as test”. On failure retain receipt and stop, no destructive retry.
- Cleanup of this RSA test object does not create/qualify CA authority, fix original error, grant signing access or resume issuer/root work.

## Constraints and outcome

Read-only queries used current executor, no CNG elevation or other-user impersonation. Official escalation requests in this task are documentary writes only; they do not rerun CNG in a different token. No source/Binary/ACL/KSP policy/ownership change, generation, deletion, private export, builds, product tests, network, serial/live implementation, Browser/PST/PACC/pin change, clone/worktree/commit/push/tag/release. Model E and ED11 retained: missing/unknown state never frees credits or grants authority. Pin0.6.4, candidate065 unpublished, gates DISABLED/DENY/DENY.

CURRENT_TASK_COMPLETED=NO
READ_ONLY_DIAGNOSIS=COMPLETED_TO_AVAILABLE_EVIDENCE_LIMIT
NTE_PERM_EXACT_API=UNDETERMINED_NATIVE_API_OR_LOCAL_GUARD
NTE_PERM_CAUSE=FIRST_FAILURE_ORIGIN_NOT_RECORDED_CANNOT_RECONSTRUCT_EXACT_CAUSE
EXECUTOR_IDENTITY=TCORP-TOBIAS\CodexSandboxOffline_MEDIUM_NONADMIN_CURRENT_CONTEXT
KSP_PROVIDER=Microsoft Software Key Storage Provider
KEY_NAME=PapinhoAccelerator.CA.e6a180fbd0341839ab9d28f0aa6008c0.20c434ce21eec7524eb33c9268e67b2a
KEY_STATE=KEY_STATE_UNKNOWN
KEY_OWNERSHIP=NOT_OBSERVABLE
KEY_CLEANUP_REQUIRED=NOT_ESTABLISHED
KEY_CLEANUP_AUTHORIZED=NO
SOURCE_CHANGED=NO
ACL_CHANGED=NO
KEY_CREATED=NO
KEY_DELETED=NO
BUILDS=NOT_RUN
NETWORK_TESTS=NOT_RUN
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
WIP_PRESERVED=YES_SCOPED_HASH_AND_WRITE_SCOPE_VERIFICATION
FIRST_UNRESOLVED_DIVERGENCE=NO_PER_STAGE_HISTORICAL_FAILURE_RECEIPT_AND_NO_READABLE_KEY_OBJECT
NEXT=OWNER_DECISION_ON_EXACT_CNG_REMEDIATION