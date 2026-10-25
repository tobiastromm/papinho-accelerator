# Manual elevated result and independent verification, 2026-10-23

Owner attempt700c1c10705f4385ae751889e193fc11: two exact DLL owner/DACL effects completed, exit0. Intent script/plan SHA256 match the approved actual files; effective operator SID S-1-5-21-785699550-1440458426-1682014700-1108, elevated Administrators, interactive console, no impersonation. Manual receipt SHA256 `02d8e02fc20b03018cc7abe1256e1e490a4edd6c43f3fddb605d8908a6f0b2ff`; intent SHA256 `7479a11b2f6f6255a9a7fa29331a3ac04278331c49eb7217d09d6c5170995270`. Original receipts preserved unchanged.

Executor independently invoked the existing launcher Mode Verify read-only: exit0, VERIFY_PASS, BOTH_DLL_POSTIMAGES_AND_NATIVE_VALIDATOR_PASS, PRESERVED_FILES_PASS, ACL_MUTATIONS=NONE. Native validator confirms libcrypto417614100:393216:1120009 and libssl417614100:393216:1119998, original sizes/hash, protected DACL and scm_descriptor PASS for both; checked close errors0. Both have approved owner, original group, exact six explicit ACEs, RX for foreign principals/AU/Users and unchanged BA/SYSTEM FullControl. Observed D:PAI differs from planned D:P only by permitted AUTO_INHERITED; protection, owner/group/ACEs/rights/order remain conformant. This is semantic verification, not acceptance based solely on exit0 or screenshot.

Receipt confirms all four file handles and parent closed; descriptor_retained=false. Parent/exe/config protections, native identities and hashes pass independent Verify. Read-only SCM queries still show DISABLED/STOPPED/PID0 and original registered image; no rebind/Start/Stop/delete. No executor security mutation, no new key/certificate/network/bootstrap/CURRENT. Native runtime token/effective CA isolation and first service lifecycle remain unqualified; ED11 PARTIAL. Do not repeat remediation.

```text
CURRENT_TASK_COMPLETED=YES
LAB_LIBCRYPTO_ACL=PASS_MANUAL_EFFECT_AND_INDEPENDENT_VERIFY
LAB_LIBSSL_ACL=PASS_MANUAL_EFFECT_AND_INDEPENDENT_VERIFY
DLL_IDENTITY=PRESERVED
DLL_HASHES=PRESERVED
SCM_DEPENDENCY_VALIDATOR=PASS_READ_ONLY
ACL_MUTATIONS=OWNER_MANUAL_TWO_EXACT_LAB_DLLS_ONLY
EXECUTOR_ACL_MUTATIONS=NONE
SCM_EFFECTS=NONE
BUILDS=PREVIOUSLY_QUALIFIED_SELECTED_LEGACY_DEBUG_RELEASE_PASS
WARNINGS=0_FINAL
NATIVE_TESTS=MANUAL_RECEIPT_AND_INDEPENDENT_VERIFY_PASS
NETWORK_TESTS=NOT_RUN
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=NONE_IN_THIS_DLL_CORRECTION_SCOPE
NEXT=WINDOWS_VIRTUAL_SERVICE_NATIVE_LAB_START_STOP
```
