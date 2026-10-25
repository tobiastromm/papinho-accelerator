# RAW1 revision 1 — integral normative review and decision package

Date: 2026-10-24. Status: REVIEW CONFORMING; OWNER APPROVAL PENDING. This is the executing agent's documentary review, not an independent external review or factual runtime qualification.

Review target: [original candidate](../../adr/ADR-CANDIDATE-n0-n9-security-descriptor-digest-profile-20261024.md), complete file SHA256 `203704246d5435f55485d698452554ea8ef432b9d0584457689dedb55758e673`. [Byte-exact revision1 archive](reviewed-revision1-original.md) has the same hash. The candidate remains PROPOSED; no accepted record is created or edited.

## Integral clause assessment

| Clause | Assessment and boundary |
|---|---|
| Problem/scope | Actual native/.NET ordering mismatch is preserved; no permission change or universal semantic-equivalence assertion. Exact N0–N9 LAB specialization only. |
| S0 | Same host/boot/attempt/operator/service/config/candidate and canonical authority. No new owner or exception to Stop, consumption, four roles, 128 handles, quotas or NONE_GRANTED. Historical target pins are not fresh host evidence. |
| S1 | Original noninheritable file handle; GetSecurityInfo(SE_FILE_OBJECT,mask7), success, valid revision1 self-relative SD, exact GetSecurityDescriptorLength bounded 1..65536. Existing SHA256 covers the ORIGINAL returned bytes including ordered data and layout, not SDDL/.NET reconstruction. Validity checks remain mandatory: the numeric lower bound alone does not validate a descriptor. No sorting, zeroing, canonicalization or semantic fallback. |
| S2 | Scope is owner/group/DACL. SACL, auditing and mandatory labels remain unqueried UNKNOWN. Any independently mandatory integrity/SACL fact still requires its own evidence or STOP. Returned offset0 proves no absence. No privilege or ACL mutation. |
| S3 | Profile/decision hash enters the same evidence/dossier/plan/pins; original native identity, protected acquisition and content/provenance checks remain mandatory. A textual hash or ordinary docs file cannot authorize admission. Only the opened file is evidenced; no mapped-image/console authentication claim. Live token/process/freshness/confirmation facts must come from the existing owner, not invented static entries. |
| S4 | Empty pins/DENY and all historical evidence remain. Normative acceptance authorizes neither implementation nor placement/preflight/Start/Stop/rebind/ACL changes. |
| Acceptance paragraph | Explicit exact-byte owner approval is still required before designated ACCEPTED registration. The current request is a review instruction, not that approval. |

## Comparison, lifetime and security findings

The observed descriptors are both248 bytes, with matching owner/group/control/offsets/ACE multiset and different ACE order. Native digest `7852b616c03c57528c4f85f843fd2f671d49dbd66a965f227c2ffbf663a177e0`; reconstructed .NET digest `b643263616bb2e6cfd85743fbd21f78e4becc259896a0faa46a247477c80ace0`. Reuse the [preserved reconciliation](../n0-n9-protected-dossier-completion-20261024/reconciliation.md) and its original evidence; no new file observation was performed. This does not establish universal semantic equivalence or make order differences automatically unsafe.

RAW1 is appropriate as an additional exact-byte integrity check, not a replacement for owner/DACL authorization, provenance or original-object binding. Compare only the same producer/profile/mask/length/algorithm and held native object. Byte-different equivalent descriptors deliberately deny. No Windows-wide canonical byte guarantee or continuous-race exclusion follows. GetSecurityInfo does not solve concurrent descriptor mutation; the accepted N4 protected-object/lifetime and required-generation revalidation remain controlling, with invalidation on mismatch/failure/staleness. Two samples are not continuous proof. Approved administrator/OS trust assumptions remain; no new waiver is created.

N0–N9 N4–N6 and B0–B6 B2/B4 retain original process/file/token references, freshness60s/final1s, suppression of effects, factual Stop/quiescence distinctions and UNKNOWN retention/charges. Descriptor buffers and handle closure must remain reconciled by the existing owner during a later implementation review. A hash cannot prove release, authorize reacquisition by PID, or turn uncertain close into success. Model E/ADR0030–0032 and ED08/ED11 are unchanged. No new resource measurements or builds are needed for this normative-only review; previous test evidence is historical, not rerun.

## Primary documentation checked

[Microsoft GetSecurityInfo](https://learn.microsoft.com/en-us/windows/win32/api/aclapi/nf-aclapi-getsecurityinfo) specifies handle-scoped copied security information, READ_CONTROL for owner/group/DACL, separate SACL rights and race limitations. [Microsoft self-relative descriptors](https://learn.microsoft.com/en-us/windows/win32/secauthz/absolute-and-self-relative-security-descriptors) allows components in different orders. These support RAW1's limited exact-byte interpretation, not a canonical serialization claim.

## Recommendation and exact approval requested

No normative correction is required to revision1 S0–S4. Recommend approval of the exact candidate file hash above, solely as profile `N2-SD-RAW1-OWNER-GROUP-DACL`. Explicitly preserve conservative false denials, unqueried UNKNOWN, existing authority/quotas/Stop/retention and independent operational gates. Approval of the profile is NOT approval of observed native digest as an operational input, the dossier, actual file placement, changed source/pins or any native operation.

After explicit approval, register the exact approved body through a designated acceptance record while keeping this original/archive and accepted N0–N9/B+D/ED11 bytes unchanged. Only then prepare the separately authorized protected-dossier placement/source/pin slice. Ordinary docs inheritance is not qualified protection. No runnable preflight is offered.
