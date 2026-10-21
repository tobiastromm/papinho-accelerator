---
adr: null
candidate-id: pacc-c2-ed08-record-end-and-ed11-admission
title: C2 RECORD producer-end ordering and total-resource admission
status: proposed
revision: 1
proposal-date: 2026-10-12
last-revised: 2026-10-12
scope-level: project
scope-target: PapinhoAccelerator
decision-makers: []
supersedes: []
superseded-by: []
---

# ED08/ED11 closeout decision candidate
The owner's [conditional semantic decision](DECISION-pacc-c2-conditional-semantic-approval-20261012.md) is not acceptance of this new closure guard or candidate target cap. Preserve accepted ADRs and all previous exact C2 candidate artifacts.

## Proposed decision and rationale
Adopt the [ED08 partial order](../capabilities/pacc-c2-ed08-record-close-order-proposal-20261012.md), conditional on U08 upstream guarantees and explicit phase approval: normal APP AB fence eligible after reciprocal COMPLETE freezes Browser-local BA producer admission; BA admitted tail/downstream native acceptance and ACK precede PST child release; confirmed DESTROYED then external ordered C3 EOF/drain delimits AB before its last fence/ACK. Retain manager/consumer peer/PACC/shared dependencies separately. No new wire type/field/enum is proposed.

Rationale: the previous release-first sketch cannot ACK a pending BA tail after its downstream socket is destroyed. COMPLETE alone is not output drain, native accepted bytes are not TLS processing, and FINAL is not remote native destruction. Retained release freezes affected I/O and original quota, never retry/fallback.

Adopt the [ED11 ledger/admission design](../capabilities/pacc-c2-ed11-resource-ledger-admission-proposal-20261012.md) with user/kernel/external separation and concrete candidate measurement envelopes, not measured runtime limits. Count every live/closing/retained allocation/lifetime, reserve CONTROL/CANCEL/FINAL/role-control/callback capacity before effects, deny unknown required hard caps and never evict retained owners. Proposed managed caps512KiB/op,16KiB/Session, Browser2MiB and Accelerator2MiB/process; full user/kernel thresholds20/2MiB and32/8MiB. No statement of old-target fit or opaque allocator enforcement.

## Alternatives and remaining dependencies
Reject idle/WOULD_BLOCK as producer-end, release-before-BA-ACK, C1 cancel disabling pending AB send, native consumer close of PST socket, fabricated close_notify and silent thirteenth message.
A different public producer-completion observer or explicit new coordination proposal may be needed if owner rejects the phase guard/U08 contract. No upstream code/API change authorized.

Reject payload-only total budgets, sizeof(opaque pointer) resource estimates, average provider memory as worst-case bound, socket-option hint as kernel ceiling, reconnect around retention and process exit as leak-free close.
Hard opaque/provider/kernel total guarantees require supported upstream/target evidence; empirical lab envelope acceptance must state its residual limitations.

## Owner approval and proof
[Prepared upstream/owner request](../integration/pacc-c2-ed08-ed11-upstream-and-owner-handoff-20261012.md) carries U08-1/2, hard versus empirical cap decision and target requirements. [Probe matrix](../capabilities/pacc-c2-ed08-probe-evidence-plan-20261012.md) and [measurement plan](../capabilities/pacc-c2-ed08-ed11-target-probe-plan-20261012.md) are prepared, not executed. [RFC supplement0.3.2](../protocol/pacc-c2-ed08-ed11-rfc-supplement-draft-0.3.2.md) is synchronized proposal text; exact spec0.3/layouts/vectors remain unchanged.

No accepted ADR revision, implementation, C2/C3 activation, E1-E12, Legacy production, PST rebuild/change/promotion, pin change or commit/push/tag/release follows from this candidate. ED01-07/09-10/12-14 recommendations stay preserved, not promoted to accepted.
