# Service identity predicate review — PROPOSED

The real Q4 token has no matching specific-SID TokenGroups entry (complete/error-free original-token observation). The pinned primary TokenUser equality predicate passed before rejection. This establishes this token only, not historical03 attributes or a universal Windows token-construction rule.

V2 of DECISION-windows-virtual-service-identity-20261022 distinguishes configured logon/SID type from actual TokenUser/groups and permits identical SID bytes. Q0–Q9 Q6 and Q4 annex A3 explicitly forbid relaxing token.specific_group.enabled in this observation. Neither instruction authorizes silently changing the guard.

Alternatives for an explicit focal review:

A. Retain the separately enabled, non-deny-only group requirement and remain DENY for the observed shape. Do not change provisioning/account/groups or repeat Start to force PASS.

B. Review an explicit virtual-account rule distinguishing original primary TokenUser equality from a separate service-SID group grant. Before any change, justify equivalent security with original TokenId/AuthId/ModifiedId continuity, no unexpected impersonation, generic SERVICE group, no enabled Administrators, actual groups/privileges/restrictions/effective access, protected identity, Stop/UNKNOWN and resource limits. Define matching-user versus distinct-user profiles and negative cases for absence, deny-only, ambiguous duplicates, SID mismatch, query failure and concurrent changes. Never equate DEFAULT with ENABLED. This option is neither selected, accepted nor implemented.

C. STOP if no equivalent rule is justified. A blind repeat is not a discriminating experiment.

GOVERNANCE CONFLICT would arise if B were implemented under the present Q6/A3 prohibition without explicit resolution. Current code remains unchanged. Any alternative requires focal normative review followed by actual-caller/effective-access qualification. No new identity, intent, approval or operation is prepared. The consumed Q4 effects also need their own disposition scope; historical03 Q2 and pre-Start P3 do not automatically apply.
