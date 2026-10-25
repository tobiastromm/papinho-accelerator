---
status: accepted
revision: 1
decision-date: 2026-10-23
scope-level: project
scope-target: PapinhoAccelerator personal MVP listener-selection direction
supersedes: []
---
# Owner-approved direction: canonical interfaces and live configuration

Explicit owner addendum of2026-10-23 approves the direction below, conditioned on existing accepted contracts. This record takes effect after exact official documentary approval/hash verification. No implementation, grammar, topology qualification, CURRENT exception or productive listener is accepted by this directional record. Historical accepted ADRs are unchanged. Technical specializations listed as pending below are NOT accepted by reference.

## Accepted direction

Preserve existing opaque machine-local PersistentID model, snapshot resolution, listener collection and canonical configuration owner. Friendly name/IP/transient index are presentation/runtime facts, never replacement identity. Reuse frontend/backend/core boundaries and existing multiple-listener contract and conflict policy; support/capacity still requires implementation evidence.

`defaultLan` is a logical automatic selector resolved to ONE existing canonical ID, not a new interface, persistent ID, ALL selection or listener type. Exactly one policy-eligible LAN selects its ID; none denies activation with reason; several deny ambiguity and present canonical IDs. Preserve selected binding on unavailability; never silently migrate. IP changes re-evaluate addresses under the SAME identity; topology changes require eligibility/binding revalidation before effects. A single active adapter is not proof of LAN safety or Internet-listener permission. Do not duplicate listeners if resolved and explicit selections collide.

Allow explicit canonical IDs and CLI inspection showing name/ID/type/addresses/state/eligibility/reason. `papacc network interfaces` is a conceptual command equivalent, not frozen syntax. Reuse existing .conf directives/blocks and preserve explicit/history values. `defaultLan` is only a candidate default when no explicit selection exists AND grammar/contracts permit. Do not accept it as decimal interface_ref or migrate existing IDs automatically.

Personal MVP includes the requirement for equivalent config show/validate/apply/status and applying supported changes without service restart. Reuse existing candidate capture/validation, canonical resolver, authority/resource preparation, Stop/capacity, transactional activation, effective-state readback and old-generation drain/retention. Partial effects/UNKNOWN never mean rollback. Frontends including future GUI share mechanisms and owner. Listener exposure and client Internet egress remain independent policies. No ports/protocol/security rules, CA or administrative identity change follows from a reload request.

## Exact pending boundary; no silent precedence

GOVERNANCE CONFLICT applies ONLY to a future interpretation that applies NEW authoritative candidate configuration while bypassing CURRENT. Accepted comments/ACL/listener contract N5 requires SOURCE -> validation/resolution/preparation -> factual commit -> CURRENT -> epoch; accepted bootstrap/update D2.2-D2.3 require factual base/CAS and publication. Owner also explicitly prohibits changing CURRENT by simple reload. This direction does NOT select an exception, parallel effective authority or shadow configuration. Activating an already committed revision and committing a new administrative revision are possible distinct scopes to reconcile; neither interpretation is silently chosen here. Required focal specialization remains PROPOSED in the next-slice map. Current runtime LAB task is unaffected and continues without listener or CURRENT.

Accepted interface textual contract I1 still accepts decimal PersistentID only, without aliases. The selector must have a separately reviewed representation/resolution boundary if implemented; no syntax is frozen here. Exact LAN eligibility, ambiguity diagnostics, target capacities and lifetime/resource bounds remain implementation/normative gates, not adapter-up heuristics.

## Evidence and next work

[Integration map and one next slice](../integration/personal-listener-live-config-direction-20261023/integration-map-and-next-slice.md) records factual code limitations. [Future direction](../future/personal-listener-selection-and-live-configuration.md) preserves unimplemented details. Accepted authorities: ADR0007, ADR0009, interface-ref I1-I4, papacc.conf N1-N8, D2/D4, canonical root/CURRENT/Model E/ED08/ED11. Existing operational gates and production pin0.6.4 remain.

No network discovery executed, listener opened, source/parser/wire/network code modified for this addendum, service reconfigured, quotas increased, CA/key/certificate/bootstrap/CURRENT performed, commit/push/tag/release. Current code changes belong exclusively to the separate protected listener-disabled LAB runtime task.
