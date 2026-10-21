---
decision-id: MODE_C_EVIDENCE_VALIDITY_SEMANTICS
status: accepted
decision-date: 2026-10-18
revision: 1
scope-level: project
scope-target: canonical Mode C evidence semantics only
decision-makers: [Tobias Tromm]
supersedes: []
---
# Mode C evidence validity semantics

## Authority and exact acceptance
The owner explicitly approved the six rules of validity-policy-proposed.md and subsequently confirmed ten conditions in the Mode C Validity Policy decision. This record accepts only those semantics, not numerical target parameters or runtime admission. The unchanged proposal is preserved at ../integration/mode-c-validity-browser-binding-20261018/validity-policy-proposed.md (SHA256 4F7D8D5064F7ABBF20617CDE88D17AE7E57EF6333784587FD76B44E0E8035DA3).

C2 revision 1.0 section 11.3 requires separate evidence-backed target approval of freshness/headroom. ADR-0052 explicitly excludes Site TLS and productive C2; its C8 CONTROL exception and 1000 ms threshold do not apply. ADR-0044/0048 and DECISION-mode-c-site-tls-profile-authority-20261018.md preserve factual committed configuration, CURRENT policy, epoch and separate Site TLS authorization. Model E and ED08 lifecycle are unchanged. No conflicting clause was identified in these applicable authorities. No existing accepted decision is amended or superseded.

## Accepted rules
1. Scope is the declared canonical normal PST065 Mode C pair, one logical operation, separate per-process accounting. Applicable target-policy revision must bind to the existing administrative committed root, CURRENT policy, factual epoch and approved Site TLS profile. Correlation issues no authority.
2. Preserve original observation, validation and recorded expiry with explicit presence, actual local monotonic source and process creation lifetime. Do not compare participant clocks, renew evidence at export, or interpret zero as absence.
3. Require factual process, image, producer, configuration, policy, epoch and operation identities according to actual availability. Missing facts stay absent. Changed process/creation/image/SDK/build/configuration/root/policy/epoch invalidates reuse for current admission; historical evidence and its associations remain preserved. Local Session IDs are not global peer identities.
4. Resource freshness requires an approved target metric, maximum age and collection validity at the required local validation point. Freshness/headroom quantitative values remain PENDING_EVIDENCE_AND_SEPARATE_NORMATIVE_DECISION, never zero, unlimited, arbitrary or inherited from C8. Recorded expiry coherence does not approve its applicability to Mode C. Missing policy parameters, unverifiable expiration or required evidence yields INCOMPLETE and DENY, never positive validity.
5. Regression, ambiguous wrap/domain, measurement failure, missing identity, overflow or loss prevents positive validity. Contradictory identity/integrity is INVALID; absent or inconclusive evidence is INCOMPLETE. Receipt correlation neither authenticates processes automatically nor grants ED11 PASS. Synthetic evidence cannot be promoted to native proof.
6. Stop and revocation prevent new operational authority without erasing evidence. Existing read-only observation requires its durable owner still alive. Retention consumes original local credit until factual destruction and quiescence. Remote FINAL is not local teardown. Do not sample a destroyed owner, invent a terminal measurement, release uncertain dependencies or introduce alternate cleanup.

## Limits and next gates
No new supervisor, collector, owner, credit manager, operational authority, grant, wire field, quota or operational identifier. Provider/kernel remain UNKNOWN. CLIENT_ADMISSION=DISABLED; SITE_TLS_ADMISSION=DENY; ED11_ADMISSION=DENY. No network execution is authorized. No Browser/PST/public API/PACC wire/pin change. Production remains0.6.4; PST0.6.5 remains candidate until integral Phase6 closeout.

This acceptance does not make current freshness evaluation PASS. Existing consumer NOT_ESTABLISHED continues to describe absence of an applicable complete quantitative target policy. Independent receipt/provenance consistency tests may proceed; Browser changes require its separately authorized Codex handoff. Quantitative target approval, real producer binding and operational execution approval remain separate gates.