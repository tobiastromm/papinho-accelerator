# Phase 5 — Compute Offload Framework

Status: complete. Phase 6 has not started.

## Model

```text
Capability = WHAT may be used
Job        = one finite requested unit of work
Backend    = HOW that work executes
```

The portable framework is implemented in `src/compute/job.h` and
`src/compute/job.c`. It adds no protocol messages or wire IDs.

Each published Job has a nonzero runtime-only ID, exactly one owner Session,
a required Capability Key, an effective monotonic deadline and one of the
states `QUEUED`, `RUNNING`, `COMPLETED`, `FAILED` or `CANCELLED`. Terminal
states are immutable. The Job Manager owns fixed caller-provided storage and
enforces both global capacity and a configured per-Session limit.

Admission validates an ACTIVE Session and Phase 4 snapshot/contextual
enforcement before preparing and atomically publishing a Job. Missing or
disabled capability, policy denial/error, missing Session, exhausted capacity,
unavailable backend or preparation failure publishes nothing.

## Backend and scheduling

The injected Compute Backend contract has bounded `prepare`, `start`, `step`,
`request_cancel` and `destroy` operations. It permits immediate completion,
incremental progress, normal no-progress, failure and cooperative
cancellation. Backend-private execution state stays opaque. Workload output is
not stored in the generic Job record. Failure after binding never silently
restarts on another backend.

One scheduler call gives at most one Job one backend opportunity. Selection
rotates through storage and prefers a runnable Job owned by a Session different
from the previous opportunity, providing Session-aware round-robin behavior
without heap, thread-per-Job, mandatory thread pool or unbounded work quantum.

Queued cancellation is immediate. Running cancellation is cooperative.
Deadline expiry uses an injected monotonic timestamp, requests backend
cancellation and forces bounded terminal cleanup. Session loss suppresses
result publication, cancels owned nonterminal Jobs and allows records to remain
only until cleanup/reap.

## Inspection boundary

The fixed Job records expose bounded read-only metadata suitable for a future
Management/Inspection adapter: runtime ID, owner Session ID, Capability Key,
state, diagnostic backend name, timestamps, deadline, progress count, terminal
reason and cancel flag. Such an adapter must not expose backend-private state,
payloads, secrets, native handles or raw Principal representation. No GUI or
management protocol is implemented by Phase 5.

## Validation and scope

`tests/job_test.c` supplies a synthetic backend only for tests. It covers
admission, runtime IDs, Session isolation, global/per-Session limits,
capability denial, preparation failure, immediate/multi-step/failing/stalled
execution, cancellation, deadline, Session loss, cleanup and reap.

Phase 5 does not implement Image, Network/TLS, Media or Web workloads, does not
modify PST or PapinhoBrowser, and does not define Job wire messages.
