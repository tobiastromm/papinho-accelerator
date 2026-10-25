# SCM LAB parent ACL root-cause qualification — 2026-10-23

Scope: read-only diagnosis; no ceremony repetition, SCM effects, security descriptor changes or functional patches. Branch main; HEAD 913c90cf2328f7939d8f5b927640601903ac6e91. Existing dirty tree preserved. Source/binary/config/ADR preimages are in preimages.json; observed descriptor and native identity in security-descriptor.json.

## Object and evidence provenance

Exact target comes from the existing pinned qualification script, corroborating the owner-provided elevated screenshot (PARENT_ACL, exit2, resources CLOSED). The screenshot itself does not contain a parent file ID or full descriptor; no historical descriptor is fabricated. Current native OPEN_EXISTING directory observation: volume417614100, indexHigh1966080, indexLow79426, attributes0x10, not a reparse point; handle closed successfully. Current descriptor is corroborating evidence, not proof that no ACL change occurred between the screenshot and this inspection. Executable SHA256 3737e83be95613a2616b9fb764fe95d72796912746e9440e37b3ed2a77e01abb and configuration SHA256 a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44 match pinned values.

A separately approved read-only query outside the sandbox returned the SAME descriptor and actual querying operator SID S-1-5-21-785699550-1440458426-1682014700-1108, matching the owner's earlier native operator evidence. No operator eligibility ceremony was repeated. Owner SID is S-1-5-21-896877160-3262526087-2538948522-1003 (CodexSandboxOffline). Group is S-1-5-21-896877160-3262526087-2538948522-513. Group is not evaluated by this predicate. DACL protected=False, auto-inherited=True; seven ALLOW ACEs, no DENY ACEs. No SACL/integrity label query requested: this predicate does not inspect them and owner mismatch already discriminates the failure; integrity/whole effective-token access is not qualified by this audit.

## Exact predicate comparison

Function: apps/server/server_launcher_scm_prepare_win32.inc, scm_descriptor; caller sets PARENT_ACL immediately before invoking it. The subsequent WIN32=5 is the adapter denial, not evidence that GetSecurityInfo itself returned access denied.

| Ordered condition | Observed | Result |
|---|---|---|
| Descriptor valid, owner present/valid | Windows descriptor parsed, owner SID above | PASS in current observation |
| System and Administrators SIDs constructible | S-1-5-18 and S-1-5-32-544 | No evidence of failure; ordinary supported constants |
| Owner equals actual operator OR SYSTEM OR Administrators | Owner ...1003 differs from operator ...1108 and both well-known SIDs | **FIRST FAIL** |
| DACL present, non-NULL, valid | Seven parsed ACEs | Structurally present; not reached by original short-circuit after owner failure |
| ACE bounds/type/SID valid | Ordinary ALLOW ACEs | No malformed/unsupported ACE observed |
| Applicable ALLOW write trustee belongs to allowed three | Multiple trustees outside allowed three | Additional independent failures below |

This is an ordered source/descriptor reconstruction, not instrumentation of the earlier elevated invocation. It identifies the first failing condition for the observed descriptor and factual operator. Exact historical bytes were not logged; historical immutability remains unproven.

## ACE-by-ACE evaluation

All seven ACEs are inherited. Inherited status alone is not rejected. OI/CI govern child inheritance; IO is skipped for the parent by this implementation.

| Index | Trustee SID | Mask | Parent applicability | Predicate result |
|---|---|---|---|---|
| 0 | S-1-5-21-896877160-3262526087-2538948522-1002 | 0x001301BF | Applies; OI/CI | FAIL: foreign writer |
| 1 | S-1-5-21-1212450354-1333653649-3686394325-941509878 | 0x001301BF | Applies; OI/CI | FAIL: foreign writer |
| 2 | S-1-5-32-544 | 0x001F01FF | Applies; OI/CI | PASS for this predicate |
| 3 | S-1-5-18 | 0x001F01FF | Applies; OI/CI | PASS for this predicate |
| 4 | S-1-5-32-545 | 0x001200A9 | Applies; OI/CI | PASS: read/execute, no tested write bits |
| 5 | S-1-5-11 | 0x001301BF | Applies | FAIL: Authenticated Users writer |
| 6 | S-1-5-11 | 0xE0010000 | INHERIT_ONLY | SKIP for parent; descendant policy still requires separate review |

0x001301BF grants read/execute/synchronize plus write-data/add-file0x2, append/add-subdirectory0x4, write-EA0x10, write-attributes0x100, DELETE0x10000. It does NOT grant WRITE_DAC, WRITE_OWNER or FILE_DELETE_CHILD0x40. Broad modify can permit directory deletion subject to Windows parent/delete semantics, content addition/modification and substitution opportunities; no actual destructive access test was performed. 0x001200A9 is read/execute/synchronize. No DENY counteraction exists in this descriptor. This is mask/predicate analysis, not an AccessCheck proof for every possible token. SYSTEM/Administrators full access happens to pass this implementation; the accepted contract does not require granting either unlimited rights.

## Normative mapping and classification

Accepted Windows virtual identity V3 requires protected native installation/config/image identity against substitution through effect. V4 requires per-resource minimum privilege, no operational writes to binaries, and real-token access qualification; V7 preserves ED11 UNKNOWN; V8 forbids bypassing gates. Optional SCM decision preserves shared console runtime. Model E requires checked closure/retention rather than presumed timeout destruction. None specifies this exact three-owner SID allowlist as a universal normative theorem: it is the implemented conservative adapter policy supporting V3/V4. This audit does not promote it into a new accepted contract or prove all alternate safe descriptors invalid.

Root cause for the observed object: **B, divergent owner**, with **A, additional broad writer ACL** independently demonstrated. A development/build artifact owned by a sandbox principal and writable by additional principals does not establish protected installation ownership. No descriptor parsing defect or excessive-policy defect is demonstrated by this case. Passing only the owner check would still fail ACE0 (and ACE1/5). No justification exists to weaken the validator merely to accept this directory.

## Minimum correction proposal — NOT APPLIED

For this exact LAB object only, the minimum parent-predicate changes would be: assign owner to the verified administrative operator (or another already allowed administrative owner with separately approved rationale), and remove the parent-applicable write/delete portions of ACE0, ACE1 and ACE5 for unauthorized writers, retaining justified read/execute access. Their current mask0x001301BF would become0x001200A9 if read/execute is the approved need. Applicable inherited ACEs require a deliberate inheritance decision; do not broadly reset ACLs or silently propagate changes. ACE6 is skipped on the parent but can grant broad rights to descendants; config/image descriptors and ancestor substitution protection must be assessed before approving an effectful binding. Merely changing parent ownership does not qualify those resources.

No executable ACL-changing command is authorized/prepared for blind execution. A later exact operation must pin this file identity, snapshot descriptor/hash, explicitly enumerate resulting ACEs/inheritance and affected descendants, preserve executor access where needed, and obtain owner approval before mutation. Removing sandbox writer access could impair builds/tooling; changing inherited permissions can affect descendants. An isolated secure installation staging design would require its own approved scope and must not silently replace this target during diagnosis.

Next discriminating test: after an approved focused correction, read back the SAME object identity and complete descriptor, evaluate owner then each applicable ACE, and only then rerun the pinned read-only binding once. Configuration/image ACL failures may become visible later. No prior operator probe, UAC cycle, service installation or new key operation is needed to establish this cause.

## Resource and preservation record

One read-only native directory handle opened/closed; supported Get-Acl descriptor queries only, including approved external-context corroboration. No service/account/ACL/SACL/inheritance/CA/CURRENT/clock/network mutations, no creation of production artifacts. No build needed or performed: functional code unchanged. ED11 total remains PARTIAL; inspection allocations are not an operational resource bound. Existing WIP and historical LABs untouched; only this new documentary evidence directory is added.

CURRENT_TASK_COMPLETED=YES
PARENT_ACL_ROOT_CAUSE=OWNER_DIVERGENCE_AND_ADDITIONAL_FOREIGN_WRITERS_OBSERVED
PARENT_OWNER=S-1-5-21-896877160-3262526087-2538948522-1003
DACL_CONFORMANCE=FAIL_APPLICABLE_FOREIGN_WRITER_ACES_0_1_5
FIRST_FAILED_PREDICATE=OWNER_EQUALS_OPERATOR_OR_SYSTEM_OR_ADMINISTRATORS
MINIMUM_CORRECTION=FOCUSED_OWNER_AND_APPLICABLE_WRITER_ACE_PROPOSAL_NOT_APPLIED
SCM_READONLY_BINDING=STOP_PARENT_ACL
SCM_EFFECTS=NONE
ACL_CHANGES=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_FUNCTIONAL_AND_PREEXISTING_FILES_UNCHANGED
FIRST_UNRESOLVED_DIVERGENCE=EXACT_LAB_OWNER_AND_WRITER_ACL_CORRECTION_REQUIRES_APPROVAL
NEXT=SCM_LAB_PARENT_ACL_FOCUSED_CORRECTION
