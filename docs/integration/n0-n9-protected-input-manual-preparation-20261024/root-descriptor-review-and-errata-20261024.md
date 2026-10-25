# Protected-root descriptor — official native in-memory review, creator STOP

Owner steering suspended creator implementation and directed exact descriptor review/errata preparation only. No creator C/CMake/launcher was written, compiled or run. No directories were created, including no new review directory: this report and evidence are documentary files under an EXISTING integration directory. Accepted decisions, prepared input files and all previous evidence remain unchanged.

## Findings

| Source | Valid | Control | Bytes | SHA256 |
|---|---|---|---:|---|
| Original prepared creation input |yes|0x9004|104|688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579|
| Native ConvertString of D:P |yes|0x9004|104|5e279428c3af64613a74133c6d27952cd5de12fcc99d8126361277ded130e6a1|
| Native ConvertString of D:PAI |yes|0x9404|104|d8dcb5d58531887a8c7ba2b0912675ec78c5f71ee9bcdcba4175ff80c97019a0|
| Task literal without valid D: component |no|not produced|not produced|Win3287, no descriptor|

Original bytes were reproduced byte-exactly from the prior prepared candidate and verified against its frozen104-byte hash; they are NOT an observed existing directory's RAW descriptor. IsValidSecurityDescriptor/GetSecurityDescriptorLength/GetSecurityDescriptorControl/GetSecurityDescriptorOwner/Group/Dacl/IsValidAcl/GetAce/ConvertSidToStringSidW verify all fields, not hash alone. Original/native-D:P owner/group/control and ordered ACE bytes match. Original offsets20/36/52 differ from native72/88/20. Native to-string on original returns D:P; from-string of that output does not reproduce the original binary. No component/ACE reordering was applied to any source or authorization input. D:PAI adds0x0400 and changes native SHA; never collapse it to D:P or the original.

Original meets exact requested owner/group Administrators, protected DACL, SYSTEM then Administrators Full Access/OI/CI, no other ACE. This is a memory-structure/policy-field proof, not successful CreateDirectory, effective AccessCheck for every token, newly assigned owner, inheritance behavior after creation or protection against privileged administrators/compromised Windows. Existing-object SACL/mandatory labels remain unobserved.29 allocated native descriptor/SID/string buffers were LocalFree'd successfully; no object handles or security-mutating APIs were used.

[Proposed errata E0–E5](../../adr/ADR-CANDIDATE-n0-protected-root-descriptor-text-errata-20261024.md), complete file SHA256 `40bafdb030436cbf1004c2d72d1aa6859aa0547546a5669d83ca8481d59dc583`. Correct only the displayed invalid SDDL to the native-produced D:P string while preserving the exact original104 bytes/hash. Do NOT replace them with native regenerated D:P or D:PAI bytes. The AI request would be a separate material control change. Normative acceptance is still pending; no silent correction/accepted decision update.

## Tests and audit

Four native in-memory conversion cases plus original-descriptor and native-round-trip validation are recorded with raw bytes, SHA, control, offsets, ACEs and native errors in [JSON](root-descriptor-native-review-20261024.json). APIs are official Advapi32; invoked from temporary in-memory Python standard-library ctypes, -I -B, without script/compiler/cache creation, privilege enabling, new recorder, service, directory or ACL writes. Prior fsutil/preflight/creation attempts were not repeated. First malformed87 is preserved rather than replaced by success. There is no current lost handle/ref or UNKNOWN cleanup claim; this says nothing about historical orphan lifetime.

Selected/Legacy Debug/Release builds NOT_RUN because owner explicitly kept STOP and suspended the source implementation slice. No positive workflow/native creation is claimed or needed to validate this descriptor's memory structure. User task SDDL wording and the native comparison are preserved verbatim in the proposed errata/evidence; source/helper/launcher/admission pins remain unchanged.

## Current gate

DESCRIPTOR_REVIEW=CONFORMING_ORIGINAL104_CONTROL9004
ERRATA=PROPOSED_PENDING_EXACT_OWNER_APPROVAL
CREATOR_IMPLEMENTATION=STOP_NOT_STARTED
DIRECTORIES_CREATED=NO
ACL_MUTATIONS=NONE
SCM_MUTATIONS=NONE
PREFLIGHT_EXECUTED=NO
START_EXECUTED=NO
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=EXACT_TEXTUAL_ERRATA_DECISION_BEFORE_CREATOR_RESUMPTION
