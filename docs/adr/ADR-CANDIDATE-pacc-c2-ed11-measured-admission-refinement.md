---
candidate-id: pacc-c2-ed11-measured-admission-refinement
status: proposed
revision: 1
proposal-date: 2026-10-12
scope-level: project
scope-target: PapinhoAccelerator
decision-makers: []
---

# ED11 measured admission: owner decision candidate

Supplements the [existing ED08/ED11 candidate](ADR-CANDIDATE-pacc-c2-ed08-record-end-and-ed11-admission.md); does not modify accepted ADRs, numerical caps, exact layouts or grant authority.

The compiled isolated candidate policy reserves512KiB core/op,16KiB/Session and Browser-model1MiB app/op within2MiB managed cap. Tests deny unknown envelope, forged/overflow size, occupied Session, stale generation, callbacks not quiescent, attempted destruction without confirmation, and reuse/eviction of a terminal-retained slot. Reserved CANCEL/FINAL spans remain outside full DATA queues. Confirmed eligible normal destruction releases credits once. Retention tests here are synthetic quota states, not PST R4.

Normal OpenSSL resource observations establish empirical scoped envelope only. Full production topology, hostile certificate/input maxima and actual Browser/NT4/kernel totals are unqualified. The public normal SDK has no allocator cap/query. An observer cannot stop opaque allocations before they exceed an assumed reservation.

Owner decision required: maintain hard-bound fail-closed gating until all categories are enforceable, or authorize an explicitly restricted empirical lab-envelope policy with specified inputs, target baseline/headroom, finite counts and stated residual provider/kernel uncertainty. This task recommends hard-bound denial for current unqualified full C2 enablement; it does not silently accept empirical risk.

R4 is a separate [PST owner return dependency](../integration/pst-openssl-model-e-r4-governed-mechanism-request-20261012.md), not a reason to duplicate NSS work. Retained resources never become zero-cost, even under an accepted empirical policy.

[Live measured closeout](../capabilities/phase6-c2-ed08-r4-ed11-governed-closeout-20261012.md), [RFC supplement0.3.3](../protocol/pacc-c2-ed08-r4-ed11-rfc-supplement-draft-0.3.3.md), [measurement evidence](../capabilities/phase6-c2-ed08-r4-ed11-measurement-evidence-20261012.json). No C2/C3 production implementation, wire change, E1-E12, Legacy production, PST rebuild/promotion or pin change authorized.
