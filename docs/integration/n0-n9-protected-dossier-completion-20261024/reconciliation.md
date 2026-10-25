# SD reconciliation - factual distinction, no normalization

Accepted N2 requires native security evidence, not a .NET reconstruction. Existing adapter uses raw GetSecurityInfo(mask7)/GetSecurityDescriptorLength/SHA256. Accepted text does not define a byte-level cross-producer canonical profile. Per this task, propose minimal [RAW1](../../adr/ADR-CANDIDATE-n0-n9-security-descriptor-digest-profile-20261024.md) and preserve DENY; no accepted ADR, adapter or pin is changed.

## Actual file result

Final fixed-file fixture held original System32/conhost.exe handle through hash, native identity and two SD observations; separate existing FileStream held the file across signature/.NET/native collection. Identical content SHA256b02ee54fb2ec69673386d41119ee8ed083a6eab3bfca6aa2155d20ce68ef8963 before/after and native. Native417614100:327680:1591236,size867840,links2. External signature again Valid/Catalog/IsOSBinary=True, Microsoft Windows. [Pinned coherent evidence](final-pinned-file-sd-evidence.json) records fixture/source SHA, actual collector SID1003 (NOT approved LAB operator1108), no SCM/preflight. Read-only file/provenance observation is not admission. No console association or mapped-image equivalence claim. Existing signature-cmdlet cache/URL behavior not asserted offline; no operational network test.

| Input | SHA256 |
|---|---|
| Native original GetSecurityInfo bytes |7852b616c03c57528c4f85f843fd2f671d49dbd66a965f227c2ffbf663a177e0|
| .NET GetSecurityDescriptorBinaryForm |b643263616bb2e6cfd85743fbd21f78e4becc259896a0faa46a247477c80ace0|
| Native SDDL UTF16LE, excluded terminator (diagnostic only) |85bdb5e58485de39cbab5b5df91cfd7d7078203488fbc6bc16c310da82a21217|

Both binary representations248 bytes, revision1, control0x9404 = SELF_RELATIVE|DACL_PROTECTED|DACL_AUTO_INHERITED|DACL_PRESENT. Owner offset20/group52/DACL84,queried SACL offset0; owner/group TrustedInstaller. DACL revision2,count6,size164. All ACEs ACCESS_ALLOWED_TYPE,flags0. TrustedInstaller0x1f01ff; BA,SY,BU,AC,S-1-15-2-2 each0x1200a9. Native order TrustedInstaller/BA/SY/BU/AC/S-1-15-2-2; .NET order SY/BA/BU/TrustedInstaller/AC/S-1-15-2-2. Complete ACE blobs/masks/SIDs match as a multiset, ORDER does not. [Offline byte comparison](native-sd-comparison.json) permuting only the analysis copy exactly reproduces .NET bytes/digest. Real mismatch is ACE ordering, not offset/size/owner/group change in these samples. This is not a general AccessCheck equivalence proof; order can matter, so authorization MUST NOT sort to PASS.

.NET source CommonAcl constructor can Canonicalize/sort/compact, explaining why reconstructed .NET bytes are not the raw API observation. No SetSecurityInfo/SetFileSecurity/ACL write occurred; in-memory construction/sorting does not modify the file. SACL/audit/mandatory label not requested under mask7; actual SACL state remains UNKNOWN. No new privilege was enabled. Never infer actual SACL absent from returned offset0.

## Tests and limits

Four Selected/Legacy Debug/Release builds zero warnings;32 executable test runs PASS. New13-case SD suite: exact digest stability, equivalent ordered security fields with shifted valid self-relative offsets but DIFFERENT bytes => raw gate DENY; changed mask/owner/group/control/ACE order/inheritance/SACL, absent/empty/null DACL distinguishable, no actual object permission mutation. Existing73-case canonical BD model covers original process identity/reuse/exit/token/confirmation/replay/exclusivity/budgets/Stop/retention/UNKNOWN;6 source-gate and29 adapter/busy cases plus runtime/policy/closed/sink regressions PASS. No actual qualifier/service/preflight executed. Prior cross-process qualification preserved, not repeated as unrelated evidence.

First focused native read returned13 and closed refs; its exact failed stage was not recorded and remains UNKNOWN. Fixture then separated read-side LastAccessTime from native identity and added focal stages. This correction is not retrospective proof of the first13 cause. Follow-up fixed-file queries returned0, native refs_closed1 and stable two-sample raw SD, including final query with held executable/file pins. Preserve [first evidence](coherent-file-sd-evidence.json); no service retry/cleanup or universal stability claim. No further equivalent file sampling is required.

## Primary sources

[Microsoft self-relative representation](https://learn.microsoft.com/en-us/windows/win32/secauthz/absolute-and-self-relative-security-descriptors): components may follow in any order; self-relative is not canonical ordering. [GetSecurityInfo](https://learn.microsoft.com/en-us/windows/win32/api/aclapi/nf-aclapi-getsecurityinfo): handle-scoped copied SD, READ_CONTROL for owner/group/DACL, SACL rights distinct and concurrent modification is not solved by API. [Control/revision](https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-getsecuritydescriptorcontrol). [Microsoft Framework CommonAcl source](https://github.com/microsoft/referencesource/blob/main/mscorlib/system/security/accesscontrol/acl.cs): Canonicalize constructor, sorting/compaction. No implementation behavior is elevated to accepted policy automatically.
