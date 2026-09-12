# Phase 4 — Capability Framework & Policy Engine

Status: complete. Phase 5 has not started.

## Architecture

```text
Capability = WHAT can be done
Backend    = HOW it is done
```

The portable Capability Key is `(stable numeric ID, semantic major version)`.
Zero is invalid for either component. Text names are bounded metadata only.
Registry and sets use caller-owned fixed storage, explicit capacities and
canonical `(id, major)` ordering. They require no heap. Duplicate registration,
invalid keys and capacity exhaustion fail explicitly; unknown keys are never
effective.

No concrete workload ID is assigned by Phase 4. `TLS_OFFLOAD`, network, image,
media and web names remain conceptual until their own capability contracts
mature.

## Effective evaluation

```text
SERVER_SUPPORTED
∩ SERVER_ENABLED
∩ PRINCIPAL_ALLOWED
∩ CLIENT_SUPPORTED
∩ CLIENT_PREFERENCE
= Session Capability Snapshot
```

The Principal-policy callback is supplied already bound to the authenticated
stable Principal/context. It cannot create or reinterpret identity. Missing
sets, missing policy, policy error, unknown key or any absent input denies.
Client inputs are programmatic/test-only until a separately approved wire
negotiation exists.

## Session snapshot and enforcement

The Session snapshot is a separately owned, bounded, immutable upper bound
keyed by runtime `session_instance_id`. There is no API to insert into a
published snapshot. Cleanup releases it when CONTROL/Session lifecycle ends;
Sessions of the same Principal can carry independent snapshots.

Operation-time enforcement requires snapshot membership, current server enable
and current contextual-policy ALLOW. Disable/revocation takes effect
immediately. Re-enable may allow a key already present in the snapshot. Neither
policy nor runtime state may add a key absent from it.

Future explicit renegotiation is required to deliberately expand a live
Session. It must re-evaluate all five inputs and atomically replace a snapshot
under a future lifecycle/wire contract. Phase 4 implements no renegotiation,
generation field, serialization, TLV or PACC message.

## Compatibility and boundaries

`PAPACC_SERVER_CONFIG.allow_network_egress` remains unchanged. It is not
migrated into this framework. Network egress is only a motivating future use
for immediate disable and explicit renegotiation.

Transport Security is infrastructure, not a normal capability. Capability
evaluation cannot disable, negotiate or weaken the Phase 3 TLS 1.3 mTLS
profile. AuthN/AuthZ remains a prerequisite and separate authority.

## Implementation and tests

- `src/capability/capability.h`: portable model and boundary;
- `src/capability/capability.c`: key, registry, sets, evaluation, snapshot and
  contextual enforcement;
- `tests/capability_test.c`: invalid IDs/versions, ordering, duplicates,
  capacity, unknown keys, five-input evaluation, policy deny/error,
  deterministic repeats, immutable snapshots, immediate disable/re-enable,
  missing snapshot, isolation and cleanup.

Wire remains unchanged at message IDs `0x0001` through `0x0006`.
