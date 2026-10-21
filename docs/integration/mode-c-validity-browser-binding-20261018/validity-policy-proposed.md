# Mode C evidence validity — PROPOSED
Date: 2026-10-18. No accepted ADR is amended. Not operational approval.

Authority: C2 revision 1.0 section 11.3 requires separate evidence-backed target approval of freshness/headroom before admission. ADR-0052 is scoped to isolated C8 CONTROL; its 1000 ms validity is not a Mode C rule. The factual timestamp closeout preserves that distinction.

Proposed minimum decision for owner review:
1. Policy is scoped to the declared canonical normal-PST065 Mode C pair, one logical operation, and separate local resource accounting. Bind the approved target policy revision to the existing administrative committed root, CURRENT policy, factual epoch and approved Site TLS profile. Correlation cannot issue authority.
2. Each producer preserves original observation, validation and recorded expiry plus explicit presence and its actual local clock source and process creation lifetime. Never compare participant clocks, renew an observation at export, or interpret zero as absence.
3. Identity evidence remains historical evidence of its recorded lifetime; it cannot authorize a replacement process. Changed process/creation/image/SDK/build/configuration/root/policy/epoch invalidates reuse for current admission. Preserve the old evidence and its original associations.
4. Resource freshness is separately evaluated at the required local validation point using an approved target metric, maximum age and collection validity. These numerical parameters remain OPEN, not zero, unlimited or inherited from C8. Internally coherent recorded expiry is not itself Mode C approval. Missing policy or required time/reference yields INCOMPLETE and DENY.
5. Regression, ambiguous wrap/domain, failed measurement, missing identity, overflow or loss prevents positive validity. Contradictory identity/integrity is INVALID; absent required evidence is INCOMPLETE. No elapsed-time rule is installed by this proposal.
6. Stop revokes new operational authority without erasing evidence. Existing read-only observation may continue only while its required durable owner survives. Retention consumes local credit until factual destruction; FINAL from the other process is not local teardown. Do not sample a destroyed owner or invent a terminal measurement.

Owner decision requested: approve or revise these semantics for a subsequent normative record. Numerical target freshness/headroom and provider/kernel disposition require a separate evidence-backed decision; approving this proposal alone cannot enable admission or freshness PASS. Implementation dependent on a new accepted policy is paused.

No new collector, supervisor, credit manager, admission mechanism, wire field, grant, quota or operational ID is proposed. ED11 incremental runtime storage in this documentary task: zero. Future Browser adapter/caller scratch must be compiled and reconciled before claiming a delta. Provider/kernel UNKNOWN; CLIENT_ADMISSION=DISABLED; SITE_TLS_ADMISSION=DENY; ED11_ADMISSION=DENY.