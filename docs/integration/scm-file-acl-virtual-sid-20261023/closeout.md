# SCM file ACL and virtual SID qualification — closeout

TASK=PACC-PHASE6-SCM-FILE-ACL-VIRTUAL-SID-QUALIFICATION-1, 2026-10-23. Branch main; HEAD913c90cf2328f7939d8f5b927640601903ac6e91. Existing WIP, accepted contracts and historical LABs preserved. All source/preimage hashes verified unchanged; only this evidence/proposal directory and a live-document append are introduced. No product/launcher patch applied.

Two separate diagnoses: [file ACLs](acl-identity-diagnosis.md) and [virtual SID/API ordering](virtual-sid-diagnosis.md). Native manifest/native-readonly.json includes exact held-parent relative file identities, hashes, descriptors and ordered ACEs; acl-correction-PROPOSED.json specifies per-file pre/post descriptors. Historical failed-binding receipt lacks leaf IDs, so historical no-substitution is not overstated. Current native observations remained stable through hashing and handles closed. Proposed SID diagnostic patch remains unapplied.

Model E/ED11: diagnostics use one held parent and one leaf sequentially; one native SD plus sequential SDDL/SID conversion allocation, one8192-byte hashing buffer; native API fixed68-byte SID and256-WCHAR domain buffers. LSA one policy handle and returned domain/SID buffers closed/freed. Native C harness uses unchanged predicate and checked resource release. Python/CRT/PowerShell/LSA/kernel/full-stack and account-lookup internals remain UNKNOWN, not zero. No positive operational ED11 inference or quota increase. Source remains C89 portable/backend boundaries intact.

No full SCM binding run, no positive ceremony repeat, no parent remediation, ACL/SACL/integrity/account/service/key mutation, certificate/bootstrap/CURRENT, clock change, operational network test, PST/Browser/wire change, clones/worktrees or commit/push/tag/release. Local supported read-only SCM/LSA/name queries and official documentation research only. No explicit remote host was selected; NULL account lookup can internally consult Windows domain mechanisms, so no claim of zero OS network activity is made.

CURRENT_TASK_COMPLETED=YES
PARENT_ACL=PASS_PRESERVED
CONFIG_FILE_ACL=FAIL_FOREIGN_WRITER_ACE_0_OWNER_PASS
EXECUTABLE_FILE_ACL=FAIL_OWNER_AND_ADDITIONAL_FOREIGN_WRITERS
ACL_IDENTITY_ROOT_CAUSE=CONFIG_CHECK_FAILS_FIRST_WRITER_THEN_EXECUTABLE_INDEPENDENT_OWNER_FAILURE
ACL_MINIMUM_CORRECTION=EXACT_PER_FILE_PROPOSED_MANIFEST_NOT_APPLIED
VIRTUAL_SID_RESOLUTION=UNRESOLVED_PRE_REGISTRATION_EXISTING_CONTROL_RESOLVES
SID_API=LAUNCHER_DOTNET_LSA_LOOKUP_NAMES2_PRODUCT_LOOKUP_ACCOUNT_NAME_W_POST_CREATE
SID_ERROR=REPRO_NTSTATUS_C000018D_MAPPED_WIN32_1789; HISTORICAL_RECEIPT_NUMERIC_CODE_NOT_CAPTURED
SID_ROOT_CAUSE=PRE_REGISTRATION_MAPPING_UNRESOLVED_PLUS_PROVED_NATIVE_CAPACITY_VS_LENGTH_CHECK_DEFECT; LOWER_LEVEL_1789_REASON_UNPROVEN
SID_MINIMUM_CORRECTION=NATIVE_BOUNDED_SID_LENGTH_PATCH_AND_DIAGNOSTIC_PHASE_SPLIT_PROPOSED_NOT_APPLIED
ACL_MUTATIONS=NONE
SCM_EFFECTS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_PREIMAGE_HASHES_VERIFIED
FIRST_UNRESOLVED_DIVERGENCE=APPROVAL_AND_MANUAL_EXACT_LEAF_ACL_CORRECTION_PENDING; SID_POST_REGISTRATION_QUALIFICATION_PENDING
NEXT=SCM_FILE_ACL_AND_VIRTUAL_SID_FOCUSED_CORRECTION

Additional discriminating evidence sid-buffer-contract.json proves the product equality check rejects a valid service SID with a larger caller buffer. sid-native-buffer-length-PROPOSED.patch is exact and unapplied. This is separate from both file ACL rejection and the pre-registration trust error.
