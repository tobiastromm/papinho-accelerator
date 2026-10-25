---
status: proposed
revision: 1
scope-level: project
scope-target: exact accepted N0-N9 single Windows LAB attempt only
supersedes: []
---
# Candidate - N0-N9 security descriptor evidence profile RAW1

## Problem and minimal decision

Accepted [N0-N9 N2.2/N2.6/N4](DECISION-first-native-lab-diagnostic-boundary-20261024.md) requires pinned opened-file security evidence but does not define cross-producer SD byte serialization. Existing adapter hashes raw GetSecurityInfo bytes; .NET CommonAcl can sort/compact ACEs in memory. Factual file has native SHA2567852b616c03c57528c4f85f843fd2f671d49dbd66a965f227c2ffbf663a177e0 versus .NETb643263616bb2e6cfd85743fbd21f78e4becc259896a0faa46a247477c80ace0. Owner/group/control/offsets/length and exact ACE multiset match; ordering differs. No ACL was changed. Windows self-relative alone is not a canonical byte format.

Propose ONLY named profile `N2-SD-RAW1-OWNER-GROUP-DACL`, retaining exact native-byte rejection instead of creating a canonicalizer. This is NOT accepted, not an admission architecture or general ED11 change. No implementation/activation/ACL/SCM/file-placement/Start/preflight permission follows.

## S0 - unchanged scope and authority

Only existing N0-N9 attempt75330d11cc7e4123ae62eadc9a59c2f5/host/boot236441/service/config/candidate/operator/policies. Existing owner, confirmation/challenge, four roles,128 handles,shared budget,NONE_GRANTED,freshness,Stop/quiescence/UNKNOWN and one-shot intent unchanged. No console-authentication/mapped-image claim. Independent evidence never grants authority.

## S1 - exact digest input

Input comes from GetSecurityInfo on the ORIGINAL noninheritable FILE handle, SE_FILE_OBJECT, exactly OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION (mask7). Require successful API, IsValidSecurityDescriptor, revision1, SE_SELF_RELATIVE and size1..65536 from GetSecurityDescriptorLength. Hash exactly that many ORIGINAL bytes with existing Accelerator-owned SHA256. Bind returned control bits, owner/group, DACL presence/null/empty distinctions and ordered ACE data; no sorting, compacting, merging, SID/mask expansion, zeroing of fields/padding, SDDL/.NET round-trip or MakeSelfRelativeSD conversion before hashing. Unknown or malformed required input denies. Original raw bytes and producer/mask/length/control/identity stay in evidence for review.

This is a source-specific EXACT-BYTE profile, not a claim that Windows offers one canonical serialization. Equivalent permission fields with different layout/order intentionally DENY; no semantic-equivalence bypass. This conservative false-denial behavior is explicit. A different profile or algorithm requires a separately reviewed decision; no automatic format migration or adoption of current bytes.

## S2 - SACL boundary

Mask7 does NOT query SACL/audit/mandatory labels. Returned Sacl offset0 does NOT prove the object's SACL absent or unchanged. No privilege enablement, SACL access/change or silent widening/waiver. Existing N2 owner/group/DACL scope remains; if a required check depends on SACL/integrity evidence, STOP and identify it separately. All returned bytes/control flags remain exact; unqueried information is UNKNOWN, never zero cost/security success.

## S3 - protected evidence and binding

RAW1/profile decision hash must be explicit in the SAME existing Windows-origin evidence referenced by the existing immutable dossier digest/plan/pins. Keep current record/owner; no second authority or registry. Only after owner acceptance and source/pin review may a dossier use the native digest. Do NOT overwrite historical .NET digest or label it raw; keep both original representations and comparison. Evidence must identify native file volume/id/size/type/reparse/links, protected owner/DACL, content SHA and external Windows catalog evidence for identical file content, plus tool/source identities and limitations.

Documentary candidate files in ordinary docs are NOT protected inputs. Creation/placement of exact approved inputs needs the existing approved operator/owner and verified native protection; no unsafe inherited DACL, current unauthorized collector token, automatic ACL remediation or overwrite. Bind admitted live tuple to original process creation/token, actual host/boot, attempt/plan/config/image/policies, exact confirmation, complete fresh metrics and lifetime through existing owner. Static file evidence is not a reusable admission receipt. Unknown future PID/token/metrics are captured only during separately approved qualification, never invented now.

## S4 - unchanged operational gates

No accepted ADR is edited by this proposal. Pins remain EMPTY/DENY. Preserve original metadata/query failure evidence and all historical UNKNOWN. Normative acceptance is distinct from protected input placement, implementation qualification, manual read-only approval and positive operation approval. No Start/Stop/rebind/ACL,CA,bootstrap,certificate,CURRENT,production or quota authorization. Production0.6.4 unchanged.

## Evidence and acceptance

[Reconciliation/tests](../integration/n0-n9-protected-dossier-completion-20261024/reconciliation.md), [proposed dossier](../integration/n0-n9-protected-dossier-completion-20261024/dossier-PROPOSED.json), [protected-placement preparation](../integration/n0-n9-protected-dossier-completion-20261024/protected-placement-and-next-gate.md). Require explicit owner approval of exact S0-S4 bytes and official integral review before designated ACCEPTED registration. No activation or file protection changes in this candidate. No new generic instrumentation or production component.
