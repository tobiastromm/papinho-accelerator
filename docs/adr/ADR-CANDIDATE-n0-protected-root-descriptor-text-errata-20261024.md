---
status: proposed
revision: 1
scope-level: project
scope-target: exact N0-N9 protected-root creation-descriptor candidate only
supersedes: []
---
# Candidate — protected-root descriptor textual errata E0–E5

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

## Evidence

[Native API review](../integration/n0-n9-protected-input-manual-preparation-20261024/root-descriptor-native-review-20261024.json), [review and decision package](../integration/n0-n9-protected-input-manual-preparation-20261024/root-descriptor-review-and-errata-20261024.md). Microsoft [SDDL flags](https://learn.microsoft.com/en-us/windows/win32/secauthz/security-descriptor-string-format), [native conversion](https://learn.microsoft.com/en-us/windows/win32/api/sddl/nf-sddl-convertstringsecuritydescriptortosecuritydescriptorw), [control bits](https://learn.microsoft.com/en-us/windows/win32/secauthz/security-descriptor-control). These APIs support validation/representation, not stable canonical byte serialization or actual-object access qualification.
