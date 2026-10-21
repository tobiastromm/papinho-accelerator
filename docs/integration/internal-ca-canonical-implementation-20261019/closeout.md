# Internal CA implementation slice — interrupted at native key protection gate

TASK=PACC-PHASE6-INTERNAL-CA-CANONICAL-IMPLEMENTATION-1. Date2026-10-19.
CURRENT TASK NOT COMPLETED. Source added in canonical Accelerator, not another tree. No productive CA, certificate, issuance, admission or network. The code is a partial administrative key backend, NOT a complete issuer or product-integrated/live registry.

## Source delivered and exact scope

- apps/server/server_ca_key_win32.h/.c: explicit Microsoft Software KSP machine-key creation/open without overwrite/startup generation; authority/generation-derived name, actual caller SID versus configured distinct operator/service SID,2048/e65537 public-key check, export-policy0 and signing-only readback, restricted DACL readback, owner thread and checked close/RETAINED disposition. No signing entry point exists without future factual root reservation; no handle is attached to TLS handlers. No callbacks/threads/alternate crypto added.
- tests/ca_key_offline_test.c: six invalid-input checks and opt-in --native-key fixture. Native fixture uses real current user as operator and well-known LocalService SID as service identity; it does NOT impersonate LocalService, so DACL inspection could not prove actual service-token isolation even if successful.
- build-Debug.cmd/build-Release.cmd: real existing MSVC x64 toolchain, /W4 /WX, isolated new build/internal-ca-canonical-implementation-20261019 outputs. CMake/product startup/SDKs were not modified. Canonical server target is NOT linked to new backend yet.

First Debug build failed due missing stdio.h declaration of _snwprintf_s; corrected by exact reviewed include. Additional reviewed hunks retain failed LocalFree allocations and failed token-close owner. Initial attempted PowerShell submission of native-test text failed parsing before any write; corrected and officially resubmitted, not a denial bypass. Prior logs preserved. Debug/Release final build3 logs compile/link current backend+test with zero warnings; these are FOCAL component builds, not complete product builds.

## Native STOP — factual result

The official executor APPROVED one isolated native RSA key test. The native API path then returned NTE_PERM 0x80090010, fixture exit20. Output:

```text
native_create_status=0x80090010; state=3; created=1
fixture_key_name=PapinhoAccelerator.CA.e6a180fbd0341839ab9d28f0aa6008c0.20c434ce21eec7524eb33c9268e67b2a
```

created=1 means NCryptCreatePersistedKey returned success. The backend operation failed afterward; the first fixture has no per-call stage receipt, so the exact later rejecting API is UNDETERMINED. Do not relabel this as a verified OS privilege diagnosis, successful finalize, successful property/ACL proof, protected stored key or actual service-token test. state3 is backend CLOSED handle disposition, NOT certificate/root destruction or persisted-key deletion. Native key persistence/finalization and deletion remain NOT_PROVEN; fixture's checked NCryptDeleteKey path was NOT reached. Public fixture name retained above for future exact reconciliation. No CA certificate was generated.

Per task STOP on absence of proven key protection/unsafe lifecycle, native creation path was interrupted. No second native attempt, changed token, broadened ACL, provider fallback, manual key-file deletion, destructive retry or alternate cleanup was attempted. This was NOT auto-review denial: executor approval was granted and native operation returned an error. Productive CA state was never initialized.

Six invalid-argument negatives then passed in both configurations without --native-key:6/6 Debug,6/6 Release. These are deterministic validation checks, not twelve independent native/security cases or issuance proof. No private export test, native reopen, service-token access test, signing, certificate issuance, serial, persistent recovery or live publication succeeded/executed after the native stop. Tests return process exit; that does not prove persistent key removal.

## Remaining original task

Private issuer codec, same-root durable serial reservation/history/revocation/renewal and live publication are NOT implemented. initialize_authority/issue_or_lookup/query_status/revoke/renew/sign_package are NOT delivered by this key component. No shadow registry/fake callback/current/epoch created to replace those facts. Full API/issuer tests and product Debug/Release builds remain pending. Active Session revocation policy remains pending and no existing Session behavior changed.

Required next bounded action: inspect/review exact native failure stage and reconcile ONLY the named test object under an officially authorized query/recovery procedure; establish whether it exists/finalized and its ownership/protection. Do not infer deletion from closed handles or retry initialization. If a correct implementation fix or execution identity/permission change is required, prepare the exact operation for official review; no automatic ACL/security escalation. Then requalify backend with isolated tests before extending root persistence or signing. Do not skip to manual generator/CLI as though CA task passed.

## ED11

sizeof(PAPACC_CA_KEY)=408 bytes in both compiled focal configurations. Native fixture owns two such owners816bytes. Backend declares local arrays: ca_begin descriptor1024+text1024bytes, validation public blob512, ACL descriptor1024, identity token query buffer sizeof(TOKEN_USER)+SECURITY_MAX_SID_SIZE. Nested stack/call frames and allocated SID/security descriptors require compiled peak accounting, not summing source arrays as a full census. Native KSP/provider/kernel and persistent storage remain UNKNOWN. New key backend is not instantiated in product/root, so no new runtime root managed reservation was claimed. No quota increase. Full issuer ED11 census NOT_RUN/PARTIAL.

## Preservation

Baseline main HEADc50b8fb9a7e3293b5e6355fc08459bf85b56857a and WIP recorded before writes.6/6 selected original files unchanged, including CMake/root/accepted decisions/pin.46/46 normal SDK hashes matched. New sources/test and task evidence only; no historical outputs/maps overwritten. git diff --check passed. Scope checks are not a complete fresh hash inventory of all ignored historic outputs. Browser/PST/wire unchanged, no Git config/ACL changes initiated by shell, no clone/worktree/commit/push/tag/release. Native test attempted a new isolated KSP security descriptor as explicitly reviewed, not existing-key or filesystem ACL replacement. No secret key material logged/exported, no CA/private client certificate produced.

CURRENT_TASK_COMPLETED=NO
INTERNAL_CA_SOURCE=PARTIAL_ADMIN_KEY_COMPONENT_ONLY
CNG_BACKEND=COMPILED_NATIVE_PREPARATION_NTE_PERM
ADMIN_KEY_ISOLATION=NOT_PROVEN
SERIAL_PERSISTENCE=NOT_IMPLEMENTED
ISSUER_API=NOT_IMPLEMENTED
CERTIFICATE_ISSUANCE=NOT_IMPLEMENTED
REVOCATION=NOT_IMPLEMENTED
RENEWAL=NOT_IMPLEMENTED
LIVE_PUBLICATION=NOT_IMPLEMENTED
BUILDS=FOCAL_DEBUG_RELEASE_PASS_PRODUCT_NOT_RUN
WARNINGS=0_FINAL_FOCAL_BUILDS
OFFLINE_TESTS=6_ARGUMENT_NEGATIVES_PASS_EACH_CONFIG_NATIVE_FIXTURE_FAIL_EXIT20
NETWORK_TESTS=NOT_RUN
ED11_TOTAL_STATUS=PARTIAL_UNTIL_QUALIFIED
REAL_CA_CREATED=NO
TEST_RSA_KEY_CREATION_STARTED=YES
TEST_RSA_KEY_FINALIZATION=NOT_PROVEN
TEST_RSA_KEY_DELETION=NOT_PROVEN
PRODUCTIVE_ISSUANCE=DISABLED
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_SCOPED_BASELINE_AND_WRITE_SCOPE_CHECKS
EXECUTOR_APPROVALS=GRANTED_NATIVE_API_RETURNED_NTE_PERM_NO_BYPASS
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=NATIVE_CNG_KEY_PREPARATION_NTE_PERM_STAGE_UNDETERMINED_PROTECTION_NOT_PROVEN
NEXT=EXACT_NATIVE_FAILURE_AND_TEST_KEY_RECONCILIATION_BEFORE_ISSUER_CONTINUATION