# SCM LAB dependency DLL ACL correction: manual handoff

2026-10-23. TASK=PACC-PHASE6-SCM-LAB-DEPENDENCY-ACL-CORRECTION-1. Branch main; HEAD913c90cf2328f7939d8f5b927640601903ac6e91. Exact plan and launcher were reviewed/applied by the official executor. Preparation, four builds and offline/read-only qualification completed. Actual correction and post-apply qualification remain pending manual elevated owner execution. No UAC launch, security write or StartService was performed by the executor. Do not interpret preparation as corrected DLLs.

The two exact LAB DLL identities/content/owner/DACL still match the previous report. Parent, papacc.conf and papacc_server.exe are pinned and preserved. [Exact plan](exact-plan.json) adds current native metadata and descriptor SHA256 to the original proposed before/after SDDL, preserving the original proposal unchanged. DLL bytes/native IDs/hashes and all dependencies remain unchanged. [Original diagnosis](../windows-virtual-service-native-lab-start-stop-20261023/dependency-acl-report-and-plan.md).

The manual launcher tools/qualification/scm-dependency-acl-uac.ps1 uses native UAC and the existing unchanged scm-file-acl-native.ps1 helper. Only Elevated mode, actual eligible fixed operator/console, full attempt-specific confirmation, current preimage checks and an exclusive intent receipt can reach Apply. Both DLLs receive the approved operator as owner; groups unchanged; six existing inherited ACEs copied explicitly, DACL protected; only ACE0/1/5 Modify become RX. BA/SYSTEM FullControl and Users RX remain. The effect uses SetSecurityInfo on the two retained native handles with OWNER/DACL/PROTECTED_DACL only, not names, SACL, integrity or child propagation. No general ACL API/authority was added.

Source helper, exact plan, native read-only executable and self are SHA256-pinned with read-share-only lifetime pins. The script compares native IDs/size/mtime/hash/SD before each effect, rechecks operator/parent/preserved files, then reads back the exact descriptor semantically. P versus PAI is accepted only with protection, same owner/group, exact ordered ACE types/principals/masks/flags and all remaining controls unchanged. The existing semantic verifier and scm_descriptor predicate were not weakened or modified.

The new isolated papacc_scm_dependency_acl_readonly build target compiles the existing unchanged scm_descriptor against the fixed two-file query. It contains no SetSecurityInfo, StartService or SCM mutation. Before correction it returns2/DENY as expected; after correction it must return0/PASS with checked close/free results. The launcher runs it after semantic post-readback and Verify reruns it independently. It is not a second service/configuration manager. Read-only descriptor conformance does not establish the actual future operational token, effective KSP denial or whole loader closure.

Manual intent is exclusive and denies subsequent Launch/Elevated attempts. Failure after the first effect preserves factual observations, closure results and UNKNOWN; no rollback/delete/retry. The owner must retain the window/result and never rerun remediation after a partial failure. Receipt, file/parent/descriptor closure and independent Verify must be assessed together; zero exit alone is insufficient.

[Manual command and acceptance checklist](manual-handoff.md). [Tests/resources and evidence index](evidence/index.md).

```text
CURRENT_TASK_COMPLETED=NO
LAB_LIBCRYPTO_ACL=MANUAL_CORRECTION_PREPARED_CURRENT_DENY
LAB_LIBSSL_ACL=MANUAL_CORRECTION_PREPARED_CURRENT_DENY
DLL_IDENTITY=EXACT_PREIMAGES_REVALIDATED_PRESERVED
DLL_HASHES=EXACT_MATCH_PRESERVED
SCM_DEPENDENCY_VALIDATOR=NATIVE_DENY_BEFORE_CORRECTION_EXPECTED_POST_MANUAL_PENDING
ACL_MUTATIONS=LAB_ONLY_IF_MANUALLY_APPROVED
EXECUTOR_ACL_MUTATIONS=NONE
SCM_EFFECTS=NONE
BUILDS=SELECTED_LEGACY_DEBUG_RELEASE_PASS
WARNINGS=0_FINAL
NATIVE_TESTS=CHECK_PASS_FOUR_VALIDATOR_DENIALS_SIX_LAUNCHER_BRANCHES_72_CLI_CASES_PASS
OFFLINE_TESTS=20_SEMANTIC_MODEL_CASES_56_SUITE_CONFIGURATION_COMBINATIONS_PASS
NETWORK_TESTS=NOT_RUN
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES_BASELINE7834_HASH_CHECKED_EXCEPT_EXACT_REVIEWED_TASK_CHANGES
FIRST_UNRESOLVED_DIVERGENCE=MANUAL_TWO_DLL_OWNER_DACL_EFFECT_AND_POST_VERIFICATION_PENDING
NEXT=WINDOWS_VIRTUAL_SERVICE_NATIVE_LAB_START_STOP_AFTER_DLL_VERIFICATION
```


## Superseding task outcome after owner manual execution

Owner attempt700c1c10705f4385ae751889e193fc11 completed both exact DLL effects. Receipt exit0 and checked closure; independent executor Mode Verify exit0, exact IDs/hash/protected semantic postimages and unchanged SCM descriptor validator PASS. Parent/exe/conf preserved. The initial pending result above is historical; CURRENT_TASK_COMPLETED=YES for this DLL correction scope. [Verified manual result and current result fields](manual-elevated-result-20261023.md). Do not repeat remediation. No service Start or authority follows; next is separately governed native LAB Start/Stop. ED11 PARTIAL.
