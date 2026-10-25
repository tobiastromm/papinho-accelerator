# First native Windows virtual-service LAB cycle: STOP before rebind

2026-10-23. TASK=PACC-PHASE6-WINDOWS-VIRTUAL-SERVICE-NATIVE-LAB-START-STOP-1. Branch main; HEAD913c90cf2328f7939d8f5b927640601903ac6e91. Requested first START→RUNNING→STATUS→STOP→STOPPED is NOT qualified. Preparation found an unsafe dependency ACL under the exact existing protected LAB parent. Section9 requires STOP. No rebind, candidate placement, service Start, runtime or permission mutation was executed. Existing owner/parent/config/image protections were preserved; they do not protect already-existing DLL contents from independently granted writes.

The existing PapinhoAcceleratorLab20261023 remains registered under NT SERVICE\PapinhoAcceleratorLab20261023, SID type UNRESTRICTED, DISABLED/STOPPED/PID0. Original papacc_server.exe remains registered with quoted absolute path plus service host. Current observed file hash3737e83be95613a2616b9fb764fe95d72796912746e9440e37b3ed2a77e01abb differs from the separate approved candidate62087dbe13712c468dc92c5848de684cbc7cba6f921d15259f895fa8664d2716; this is the known pending rebind, not a silent replacement.

PE inspection of that candidate confirms direct imports of libssl-3-x64.dll and libcrypto-3-x64.dll. Both existing DLLs in the exact intended protected installation are owned by foreign SID S-1-5-21-896877160-3262526087-2538948522-1003. Their unprotected DACLs allow Modify0x1301bf to two foreign principals and Authenticated Users. The existing scm_descriptor, compiled unchanged into a read-only diagnostic, returns DENY for both. The first actual rejection is its owner predicate. DACL protection and unsafe modification grants are additional independent failures. Imported library code cannot be presumed protected because listener initialization is disabled. No alternate backend, static-link change or directory substitution was made.

Held-handle observations confirm parent417614100:1966080:79426, config417614100:393216:1120240, original image417614100:262144:1119989, exact hashes and protected descriptors. DLL IDs, SHA256 and exact descriptors were captured through held read-only handles, with before/after identity checks and checked frees/closes. Candidate-directory DLL bytes match these same hashes; this does not qualify their ownership or private provider costs. Read-only SCM status is not a token or readiness proof.

Selected/Legacy Debug/Release existing runtime builds repeated successfully, zero final warnings. Each configuration passes14 existing suites (56 combinations);72 CLI negative/help cases pass. These include Stop-before/during/after readiness and retention models, real token denial and real closed-manager components. They did not start the service. The focused native descriptor probe compiled warning-free after adding the canonical already-used capsule header include paths; its initial missing-include compile attempt executed no probe. No toolchain/backend change. No automatic approval rejection occurred.

[Exact two-file diagnosis and minimum proposed correction](dependency-acl-report-and-plan.md). [Unarmed future manual lifecycle plan](manual-cycle-plan-STOP.md). [Evidence index](evidence/index.md). Existing functional code remains unchanged by this task; the exploratory cycle source remained unapplied in canonical external scratch, and no executable mutation launcher was built or armed.

```text
CURRENT_TASK_COMPLETED=NO
LAB_SERVICE_START=NOT_RUN_STOP_UNSAFE_DEPENDENCY_ACL
LAB_SERVICE_RUNNING=NOT_QUALIFIED
LAB_SERVICE_STOP=NOT_RUN
LAB_SERVICE_STOPPED=OBSERVED_EXISTING_STOPPED_PID0_NOT_CYCLE_PROOF
SERVICE_TOKEN=NOT_CAPTURED_POSITIVE_SERVICE_PENDING
SERVICE_SID=PRIOR_POST_REGISTRATION_SID_PRESERVED_RUNNING_TOKEN_UNQUALIFIED
ACL_ISOLATION=DENY_TWO_IMPORTED_DLLS_UNSAFE
RUNTIME_READINESS=OFFLINE_QUALIFIED_NATIVE_SERVICE_NOT_RUN
STOP_QUIESCENCE=MODEL_COMPONENT_PASS_NATIVE_CYCLE_NOT_RUN
LISTENER=DISABLED
ED11_TOTAL_STATUS=PARTIAL
BUILDS=SELECTED_LEGACY_DEBUG_RELEASE_PASS
WARNINGS=0_FINAL
OFFLINE_TESTS=56_SUITE_CONFIGURATION_COMBINATIONS_PASS
NATIVE_TESTS=72_CLI_CASES_PASS_AND_READONLY_DEPENDENCY_DIAGNOSIS_CONFIRMED
NETWORK_TESTS=NOT_RUN
LAB_IMAGE_PLACEMENT=NOT_RUN
LAB_SERVICE_REBIND=NOT_RUN
PRODUCT_SERVICE_INSTALL=NOT_RUN
BOOTSTRAP_EXECUTION=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES_BASELINE7803_HASH_CHECKED_EXCEPT_REVIEWED_DOCUMENTATION
FIRST_UNRESOLVED_DIVERGENCE=IMPORTED_LAB_DLL_OWNER_DACL_UNSAFE
NEXT=SCM_LAB_DEPENDENCY_ACL_FOCUSED_CORRECTION
PLANNED_AFTER_NATIVE_CYCLE=PERSONAL_LISTENER_AND_LIVE_CONFIGURATION_RECONCILIATION
```
