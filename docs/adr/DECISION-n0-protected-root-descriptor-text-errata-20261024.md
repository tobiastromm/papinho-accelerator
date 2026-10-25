---
status: accepted
revision: 1
decision-date: 2026-10-24
scope-level: project
scope-target: exact N0-N9 protected-root creation-descriptor textual errata only
supersedes: []
---
# Decision — N0 protected-root descriptor textual errata E0–E5

## Exact owner approval and integral official review

Owner explicitly issued OWNER APPROVAL — N0 PROTECTED ROOT DESCRIPTOR TEXT ERRATA E0–E5, approving only the exact candidate SHA256 `40bafdb030436cbf1004c2d72d1aa6859aa0547546a5669d83ca8481d59dc583` for documentary ACCEPTED registration after integral official review and byte-exact verification. IMPLEMENTATION_AUTHORIZED=NO. All owner preservation conditions were checked against the complete E0–E5 text and unchanged original native-API review evidence; no material divergence was found. This is the executing agent's official documentary review/registration, not a claimed external independent reviewer or new runtime qualification.

The approved [candidate](ADR-CANDIDATE-n0-protected-root-descriptor-text-errata-20261024.md) and [byte-exact approved archive](../integration/n0-n9-protected-input-manual-preparation-20261024/root-descriptor-errata-approved-revision1-original-20261024.md) remain historically PROPOSED and unchanged. The exact incorporated E0–E5 body SHA256 is `20c22d827c758129c84cc9665cf8d68d46111811149d243e53393a0f542ac7fd` (original bytes from E0 through E5, excluding trailing newlines). [Acceptance verification](../integration/n0-n9-protected-input-manual-preparation-20261024/root-descriptor-errata-acceptance-verification-20261024.json) records source/archive/body hashes and preservation.

## Owner-condition reconciliation and scope

Original104 bytes remain frozen under SHA256688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579: owner/group Administrators, revision1/control0x9004/protected DACL, SYSTEM then Administrators ACCESS_ALLOWED Full Access0x001f01ff with OI/CI0x03 in original order. This is the original prepared creation-input binary, validated by Win32 APIs, NOT an observed existing-directory GetSecurityInfo descriptor. Its exact hex remains in [unchanged native review](../integration/n0-n9-protected-input-manual-preparation-20261024/root-descriptor-native-review-20261024.json). No bytes are regenerated, reserialized, normalized or reordered by this registration.

The corrected D:P string is descriptive only. Native conversion from that string produces different layout/hash; matching reviewed security fields is not binary identity or a general access-equivalence proof. D:PAI adds0x0400, has a different hash and is NOT accepted as a substitute. No future creator may replace original bytes through a string round-trip or semantic comparison. Unqueried existing-object SACL/audit/mandatory labels remain unproved.

E0–E5's prospective acceptance language resolves ONLY to normative acceptance through THIS designated record. This is a textual errata for the same candidate, not acceptance/creation of a ProgramData root, not a new storage/admission authority or a change to accepted RAW1/N0–N9/B+D/ED11. Original owner/confirmation/exclusivity/intent/Stop/quiescence/UNKNOWN, four roles/native128/shared budgets/quotas/NONE_GRANTED remain controlling. No accepted prior decision or active pin changes.

Owner explicitly withholds implementation: creator source/launcher/build/native creation remain STOP; acceptance does not resume the earlier implementation task. Separate scoped authorization is required to implement the exact-byte creator and qualify it offline; a separate exact manual operation approval is still required before directory creation. No ACL/SCM/preflight/Start/Stop/CA/cert/bootstrap/CURRENT or production effect is authorized or executed. Production0.6.4/PST/Browser/wire and WIP remain preserved.

## Accepted E0–E5 — exact approved source body

## E0 — source discrepancy and narrow scope

Owner selected STOP rather than automatic SDDL substitution. Creator task supplied invalid text `O:BAG:BAD(A;OICI;FA;;;SY)(A;OICI;FA;;;BA)` alongside104-byte SHA256 `688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579`. Win32 ConvertStringSecurityDescriptorToSecurityDescriptorW rejects that text with87. The frozen104 bytes were a prior .NET-prepared CREATION INPUT, never a GetSecurityInfo observation of an existing directory. Official APIs now validate those exact bytes and the intended security fields. This errata affects only the candidate's textual representation/provenance, not accepted RAW1/N0–N9/B+D/ED11 or any object ACL.

## E1 — preserve exact original creation input

Retain the original104 bytes and exact hash above. Native IsValidSecurityDescriptor TRUE, GetSecurityDescriptorLength104, revision1, control0x9004 = SELF_RELATIVE0x8000 | DACL_PROTECTED0x1000 | DACL_PRESENT0x4. Owner and group both S-1-5-32-544. Offsets owner20/group36/DACL52/SACL0. Valid ACL revision2,size52,exactly two ACCESS_ALLOWED ACEs: index0 SYSTEM S-1-5-18,size20; index1 Administrators S-1-5-32-544,size24. Both mask0x001f01ff and flags0x03 (OBJECT_INHERIT|CONTAINER_INHERIT); neither INHERITED nor INHERIT_ONLY. No untrusted ACE, no null/empty DACL or extra access grant. SACL0 describes this in-memory input only; no existing-object audit/mandatory-label state is observed or waived.

## E2 — proposed textual correction, not byte conversion

Correct the invalid display text to the EXACT string returned by ConvertSecurityDescriptorToStringSecurityDescriptorW(original104,revision1,OWNER|GROUP|DACL):

```text
O:BAG:BAD:P(A;OICI;FA;;;SY)(A;OICI;FA;;;BA)
```

This change is textual only. It does NOT permit replacing/regenerating original bytes, changing control/ACEs, sorting, canonicalization or adopting a new hash. Expected input must remain the exact pinned binary; a future creator must validate/use that binary, not assume a parser produces it. No generic semantic-equality bypass is accepted.

## E3 — separate native serializations and PAI comparison

Native ConvertStringSecurityDescriptorToSecurityDescriptorW of corrected D:P produces104 valid bytes, SAME owner/group/control/ordered ACE bytes, but layout DACL20/owner72/group88 and SHA256 `5e279428c3af64613a74133c6d27952cd5de12fcc99d8126361277ded130e6a1`, DIFFERENT from original. Native string round-trip is therefore NOT byte-equivalent. It is diagnostic comparison only, NOT an alternative approved input.

Native conversion of `O:BAG:BAD:PAI(A;OICI;FA;;;SY)(A;OICI;FA;;;BA)` separately produces104 bytes/control0x9404/SHA256 `d8dcb5d58531887a8c7ba2b0912675ec78c5f71ee9bcdcba4175ff80c97019a0`. Compared to native D:P, only byte offset3 differs (0x90 -> 0x94), setting SE_DACL_AUTO_INHERITED0x0400. The original104 converts back to D:P, NOT D:PAI. Adding AI is a control-field change, not this textual errata, and is NOT accepted here. Both native variants satisfy the listed owner/group/protection/two-ACE field predicate, but that does not make their byte hashes interchangeable or authorize either substitution. No universal semantic equivalence/access-check claim.

## E4 — authority, lifecycle and no effects

Keep exact attempt75330d11cc7e4123ae62eadc9a59c2f5/host/boot/operator/candidate/config/policies and same canonical admission owner, explicit confirmation, exclusivity, one-shot intent, Stop, quiescence and UNKNOWN retention. Four roles,128 handles,shared budget,quotas/freshness/headroom and HOST_CREDITS=NONE_GRANTED unchanged. This directory-input review is distinct from RAW1's opened Windows-file evidence; it does not change the5358/326-byte inputs or their hashes.

No creator implementation, launcher activation, ProgramData directories/files, ACL/owner change, positive native test, preflight, SCM, Start/Stop, CA/cert/bootstrap/CURRENT or production permission follows. Creation protection must later be factually checked through original objects. Existing protected-root design remains proposed; durable new-object security/ancestor/exclusive-creation/close/retention tests remain future gates. Creator implementation stays STOP until separately resolved by the owner; acceptance of this errata alone does not authorize execution or silently change a pinned launcher.

## E5 — acceptance gate

PROPOSED only. Require explicit owner approval of this exact E0–E5 record and integral byte-exact review before any designated normative acceptance. Preserve invalid original task text, original104 bytes/hash, separate native conversions, first87 error and historical evidence. Accepted ADRs are not edited. If owner requires AI rather than original0x9004, retain STOP and prepare a separately reviewed material descriptor/hash change; do not call it a spelling fix.

## Registered status

CURRENT_TASK_COMPLETED=YES_DOCUMENTARY_ACCEPTANCE_ONLY
DESCRIPTOR_TEXT_ERRATA=ACCEPTED
DESCRIPTOR_BINARY=UNCHANGED_104_BYTES_SHA256_688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579
DESCRIPTOR_CONTROL=0x9004
TEXTUAL_DESCRIPTION=D:P_ONLY_NO_REGENERATION
D_PAI_SUBSTITUTION=DENY
IMPLEMENTATION_AUTHORIZED=NO
DIRECTORIES_CREATED=NO
ACL_MUTATIONS=NONE
SCM_MUTATIONS=NONE
PREFLIGHT_EXECUTED=NO
START_EXECUTED=NO
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=N0_N9_PROTECTED_ROOT_CREATOR_IMPLEMENTATION
