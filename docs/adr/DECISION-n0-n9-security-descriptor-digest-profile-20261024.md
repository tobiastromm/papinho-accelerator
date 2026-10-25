---
status: accepted
revision: 1
decision-date: 2026-10-24
scope-level: project
scope-target: exact accepted N0-N9 single Windows LAB attempt only
supersedes: []
---
# Decision — N0–N9 security descriptor evidence profile RAW1

## Owner approval, integral review and exact registration

Owner explicitly issued OWNER APPROVAL — RAW1 REVISION 1, NORMATIVE ONLY, approving only S0–S4 of `N2-SD-RAW1-OWNER-GROUP-DACL`, full candidate SHA256 `203704246d5435f55485d698452554ea8ef432b9d0584457689dedb55758e673`. Owner authorizes its normative ACCEPTED registration after integral review and byte-exact verification, and explicitly withholds implementation, placement, pins and operational effects.

[Integral review](../integration/n0-n9-raw1-digest-acceptance-20261024/review-and-decision-package.md) assessed all clauses and applicable N0–N9, B+D revision2, ED11 B0–B6 revision4, Model E and ED08 constraints. Re-reading the full approved candidate and comparing its complete bytes against the reviewed archive found no divergence from owner conditions. This is the executing agent's official documentary review and registration; no independent external reviewer or runtime qualification is claimed.

Register exact revision1 S0–S4 as ACCEPTED through THIS designated record. The [candidate](ADR-CANDIDATE-n0-n9-security-descriptor-digest-profile-20261024.md), [reviewed archive](../integration/n0-n9-raw1-digest-acceptance-20261024/reviewed-revision1-original.md) and [approved archive](../integration/n0-n9-raw1-digest-acceptance-20261024/approved-revision1-original.md) remain byte-exact and historically PROPOSED. Incorporated S0–S4 body SHA256 `f752d0272c0cdab671318571ca9e47413916c2137236ee75277b0c691bd0c6aa` (original bytes from S0 heading through S4, excluding trailing newlines). [Acceptance verification](../integration/n0-n9-raw1-digest-acceptance-20261024/acceptance-verification.json) records preservation and post-write checks.

## Composition and limits

This specifies only the security-descriptor digest profile within [N0–N9 N2.2/N2.6/N4](DECISION-first-native-lab-diagnostic-boundary-20261024.md). Existing [B+D revision2](DECISION-canonical-lab-console-topology-bd-20261023.md), [ED11 B0–B6 revision4](DECISION-ed11-single-lab-diagnostic-native-boundary-20261023.md), canonical authority, original native identities, provenance, lifetime/revalidation, Stop, quiescence and UNKNOWN retention remain mandatory. Four roles, individual native128-handle limits, shared administrative budget, all quotas/freshness/headroom and HOST_CREDITS=NONE_GRANTED remain unchanged.

RAW1 uses original noninheritable file handle, GetSecurityInfo(SE_FILE_OBJECT,mask7), a valid revision1 self-relative descriptor and SHA256 of exactly the original returned descriptor length. Same-profile comparison only; differing bytes, errors, missing or stale evidence deny. No ACE sorting, SDDL reconstruction, semantic equivalence bypass or automatic adoption of currently observed bytes. SACL/auditing/mandatory labels not queried remain UNKNOWN; an independently required fact is not waived. No authenticated console association or mapped-image equivalence is asserted.

Prospective approval wording in the incorporated body resolves solely to normative acceptance through THIS record. S4's unchanged-pins/operational prohibitions remain in force. Profile acceptance does not approve a concrete dossier/native digest as operational input or prove current boot/token/process/metrics. Protected input placement and reviewed implementation/pins remain separately unauthorized. No runtime readiness or ED11 operational PASS follows.

## Accepted S0–S4 — exact approved source body

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

## Registered status and next gate

CURRENT_TASK_COMPLETED=YES
RAW1_PROFILE=ACCEPTED
DESCRIPTOR_SOURCE=ORIGINAL_FILE_HANDLE_GETSECURITYINFO_MASK7_SELF_RELATIVE
RAW_DIGEST_COMPARISON=EXACT_SAME_PROFILE_BYTES_NO_NORMALIZATION
ACE_ORDER=UNCHANGED
ACL_MUTATIONS=NONE
IMPLEMENTATION_AUTHORIZED=NO
PREFLIGHT_EXECUTED=NO
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=N0_N9_PROTECTED_DOSSIER_PLACEMENT

Next step is separately authorized preparation/review of exact protected inputs using the existing owner and approved operator, with verified native protection. No file placement, pin/source change, preflight, StartService/StopService, rebind SCM, ACL, CA, certificate, bootstrap, CURRENT or quota effect is authorized or executed. Production0.6.4, PST, Browser, wire, original proposals/decisions and historical UNKNOWN remain preserved.
