# SCM file ACL and virtual SID focused correction — implementation and manual handoff

TASK=PACC-PHASE6-SCM-FILE-ACL-VIRTUAL-SID-CORRECTION-1, 2026-10-23. Owner attachment ae11aa5c-a1c8-4188-bfd4-074cbd2cc668 authorizes scoped source correction/tests and preparation of a manually confirmed two-file ACL operation, not executor ACL mutations. Branch main, HEAD913c90cf2328f7939d8f5b927640601903ac6e91 unchanged. Accepted V0–V8/Model E/ED08/ED11 and prior independent diagnostics reviewed. Original WIP preserved by exact pre/postimage review; no accepted ADR edits.

## SID correction implemented

server_service_virtual_win32.inc retains original API and CreateService -> configure UNRESTRICTED -> configured-account/command/SID-type readback -> LookupAccountNameW order. It no longer equates caller buffer capacity with actual SID length. A successful SID now requires reported capacity between8 and the fixed buffer size, structural IsValidSid, actual GetLengthSid <= reported capacity, six subauthorities and NT authority/service namespace80. Store actual GetLengthSid (32 for this shape), not caller capacity68. No SID truncation accepted, no provider/backend/fallback identity change or authority grant. Allocation sizes and owner lifetimes unchanged.

Eight new native-API MODEL modes cover valid32-byte SID while capacity68 is preserved, exact reported size, truncated31, invalid revision, insufficient-buffer error, zero/oversized reported capacity, wrong shape, and a pre-registration trust-error then post-registration success model. Existing namespace mismatch, Stop, partial creation, cleanup uncertainty/retention and no-retry tests remain. Model registration is only intercepted fake handles; no real service created. Binding grants remain denied.

The read-only UAC qualification wrapper now separately records read_only_data_pass, sid_name_resolution and pending post-registration state, prints the actual SID error, and preserves full qualification/exit DENY when SID unresolved. It does not infer factual registration from a computed SID. The prior target absent/1789 and EventLog success diagnostics are preserved, not treated as a demonstrated general domain failure. No service was created to resolve the name. Product's existing post-create ordering is correct and unchanged; post-registration actual name/token proof remains pending separate authorization.

## Builds/tests

Fresh build directories build/scm-file-acl-sid-correction-20261023/{Selected,Legacy}/{Debug,Release}; same MSVC19.51 x64 Ninja/toolchain and /W4 /WX. All four builds passed with zero warnings. Existing read-only LAB executable407552bytes SHA2563737e83be95613a2616b9fb764fe95d72796912746e9440e37b3ed2a77e01abb and configuration SHA256a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44 remain unchanged. New builds do NOT replace the original LAB executable or its native identity/pin. Future effectful SCM qualification must explicitly select the corrected build and qualify its own protected binding; do not silently repin or claim this old LAB binary contains the SID fix.

40 regression combinations pass in regressions.json: 10 suites across four profiles/configurations, including lifecycle DATA, native/virtual/query/ACL models, console, personal effect-plan/native read-only, memory census and issuer denial. The target-creation component was built but NOT run. No network, certificate/key/bootstrap or CURRENT test. Legacy here is managed-memory profile, not VC6/NT4 qualification.

ACL model: two files each P and PAI accepted, plus ten unsafe owner/writer/unprotected/extra-ACE/flag cases rejected. ModelTest does not open native target files. Initial PowerShell5.1 JSON array wrapping produced PLAN_COUNT and denied before file effects; exact reviewed correction removed the wrapper. Final ModelTest0 and Check0 pass. Current-unmodified ACL correctly rejected as postimage (Verify2); wrong Elevated script hash denied before effect2. Read-only wrapper Check2 confirms executor still lacks eligible elevated/interactive context; no automatic UAC attempted.

## Manual ACL operation, NOT EXECUTED

Launcher tools/qualification/scm-file-acl-uac.ps1 SHA256 b9a290e935f33937c305f9ccf33427a6798c633b00a05d668e80619c54ef64c9.
Native helper tools/qualification/scm-file-acl-native.ps1 SHA256 f52242fe74932d1539e83a742430ea523eda746ce39d8815b1b977cf6bb6ac75.
Exact scripts approved before writing; final postimage hashes in verified-postimages.json. The prepared operation imports and hash-pins the exact previous acl-correction-PROPOSED.json and original parent intent. It changes ONLY the two recorded objects under the same exact parent, never the parent itself:

- papacc.conf native417614100:393216:1120240, existing operator owner preserved; groups preserved; ACE0/1/5 Modify0x1301BF -> read/execute0x1200A9.
- papacc_server.exe native417614100:262144:1119989, owner -> approved operator ...1108; group preserved; same three mask reductions.
- Protect each file DACL and preserve existing entries explicitly; retain all other rights, no generic FullControl grant or other-file/ancestor changes. No content writes, binary replacement, SACL/integrity, parent or SCM ACL effects.

Both exact files and parent are held through the operation; native IDs, expected sizes/write times, no-reparse, content SHA256 and native descriptor preimage hash checked before confirmation and again immediately before effects. Both files are prevalidated before first effect. Paths/identities derive from fixed plan, not arbitrary CLI arguments. Parent expected owner/group/seven ACEs/PROTECTED is independently checked with only AUTO_INHERITED variation allowed.

Elevated mode requires exact actual operator ...1108, factual elevated/nonlimited token, enabled Administrators, no impersonation and real input/output console. Uses native UAC only through manual Launch; no password parameters/logs or policy change. The full exact proposed descriptors are displayed, and only the whole displayed CORRECT_SCM_FILES <new-attempt-id> line confirms this invocation. Enter alone denies. Old parent/F10/read-only confirmation is not reused.

Exclusive durable manual-intent.json is created/flushed before first effect; existing intent refuses retry. Apply uses SetSecurityInfo on held FILE handles: DACL+PROTECTED only for conf; owner+DACL+PROTECTED for exe. These are non-directory objects, no descendant propagation. Pre/post group is preserved. Readback compares native object/hash plus structural owner/group, ordered six ACE binaries, ACL revision/control with only AUTO_INHERITED ignored when protected. No SDDL text equality. The parent is checked after each stage. Any partial effect, interruption or uncertainty is STOP, not rollback; second-file failure does not undo first file or permit reusing authorization.

Finally records available observed descriptors/hash, factual per-file and parent closure, descriptor retention, attempted effect and exit in a separate exclusive manual-result.json. Each handle closes at most once; failed closure retains its reference in the invocation, no retry/timeout release. At most one uncertain native descriptor allocation is retained by helper and blocks further effects. This is temporary qualification-tool bookkeeping, not a new product resource owner/manager. No automatic deletion/retry. F10 closes the preserved result window only.

Manual command (OWNER ONLY; executor did not invoke Launch or Elevated positive):

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-file-acl-uac.ps1" -Mode Launch
```

After manual execution and receipt inspection, separate read-only verification:

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-file-acl-uac.ps1" -Mode Verify
```

Do not run the parent remediation launcher. Do not repeat this file launcher after error/UNKNOWN or delete its intent. The existing protected-binding read-only producer will subsequently exercise unchanged native scm_descriptor on actual file handles; full SID resolution remains a separate post-registration gate, so exit2 with DATA_PASS and SID_PENDING is not permission to create a service. No post-manual PASS has been fabricated.

## ED11 and preservation

SID fix adds no buffers, allocations, handle, thread, owner or pin table. Four memory-census tests pass; existing PS_NATIVE/host/root sizes unchanged. No quota increase: Legacy2097152, explicitly Selected3145728. Provider/kernel/CRT/full transitive stack and operational resource proof remain UNKNOWN/PARTIAL, not zero.

Manual tool controlled resources: four pinned script/helper/plan/parent-intent streams; one parent and two file handles held; one8192-byte streaming hash buffer per sequential operation; one <=65536-byte native descriptor per query, checked LocalFree; possible single retained descriptor denies subsequent effects. Separate intent/result stream sequentially. UAC parent/child pinned streams coexist, with native target handles only in elevated child. RawSecurityDescriptor copies, GC, SHA/tool runtime/compiler/CLR/PowerShell/UAC/kernel/stack costs not fully bounded, and are not used to claim product admission or transfer costs between profiles. Original process ownership/Stop/retention/CAS unchanged.

No ACL mutation was executed in this task; no account/service/KSP/key/certificate/CURRENT/root mutation; no PST/Browser/wire or production0.6.4 change; no clones/worktrees/commit/push/tag/release. Original two file identities/hashes/native descriptors and parent PASS revalidated by final Check, original LABs preserved. Only exact approved functional hunks and new qualification scripts plus live evidence changed.

CURRENT_TASK_COMPLETED=YES
CONFIG_FILE_ACL=MANUALLY_APPLIED_INDEPENDENT_READONLY_VERIFY_PASS
EXECUTABLE_FILE_ACL=MANUALLY_APPLIED_INDEPENDENT_READONLY_VERIFY_PASS
SID_LENGTH_CORRECTION=IMPLEMENTED_4_BUILDS_40_REGRESSIONS_PASS
VIRTUAL_SID_RESOLUTION=PRE_REGISTRATION_PENDING_NO_FALLBACK; POST_CREATE_ORDER_PRESERVED
PARENT_ACL=PASS_PRESERVED
ACL_MUTATIONS=MANUAL_LAB_ONLY_TWO_FILES_CONFIRMED; NONE_EXECUTOR
SCM_EFFECTS=NONE
BUILDS=SELECTED_LEGACY_DEBUG_RELEASE_PASS
WARNINGS=0
OFFLINE_TESTS=40_REGRESSION_COMBINATIONS_PASS_PLUS_ACL_MODELS
NATIVE_TESTS=EXACT_TWO_FILE_READONLY_PREFLIGHT_PASS_NEGATIVES_DENY; NO_POSITIVE_MUTATION
NETWORK_TESTS=NOT_RUN
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=EXACT_PRE_POSTIMAGES_AND_ORIGINAL_LAB_PIN_VERIFIED
FIRST_UNRESOLVED_DIVERGENCE=FULL_SCM_READONLY_REQUALIFICATION_AND_LATER_FACTUAL_SID_REGISTRATION_PENDING
NEXT=SCM_PROTECTED_BINDING_READ_ONLY_REQUALIFICATION


## Manual owner execution and independent post-verification

Owner executed attempt ae238f04c9964527a12d943d99bf4c90 and returned SCM_FILE_ACL_EXIT=0. Original manual-intent.json/manual-result.json preserved. Both file postimages and hashes match the approved plan; owner operator ...1108, protected DACLs and six ordered ACEs; only permitted AUTO_INHERITED variation. Each file handle and parent handle factually closed, descriptor_retained=false, no SCM effect. Executor subsequently ran ONLY Mode Verify: exact native identities, sizes/write times, hashes, descriptors and unchanged parent passed, exit0. See manual-result-independent-verification.json. No ACL reapply, no remediation launcher repetition. Full canonical binding read-only qualification remains the next task; actual virtual-name mapping after SCM registration/token still unqualified.
