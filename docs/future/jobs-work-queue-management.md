# PapinhoAccelerator --- Future Architecture Direction: Jobs / Work Queue Management & Inspection

**Status:** future architecture direction / deferred management
requirement\
**Implementation status:** not declared by this document\
**GUI status:** not implemented by this document\
**Wire/API status:** not frozen

## Purpose

Preserve the owner-approved requirement that future PapinhoAccelerator
management interfaces can inspect the runtime Jobs / Work Queue without
making the GUI the owner of scheduling or job lifecycle.

The design must preserve:

``` text
Job Manager / Scheduler
        ↓
Management / Inspection API
        ↓
GUI / CLI / Web Management / Telemetry
```

The GUI observes and administers through a management boundary; it does
not own the queue.

## Core direction

The future GUI should provide a Jobs / Work Queue view.

Conceptual example:

``` text
Jobs / Work Queue

ID      Session   Principal   Capability      State      Backend   Time
101     42        user-a      IMAGE_RESIZE    RUNNING    CPU       120 ms
102     43        user-b      AVIF_DECODE     QUEUED     GPU       -
103     42        user-a      AUDIO_TRANSCODE WAITING    CPU       2.1 s
104     44        guest       IMAGE_RESIZE    CANCELLED  -         -
```

Selecting a job may show details such as:

``` text
Job #101
Owner Session:       42
Principal:           user-a
Capability:          IMAGE_RESIZE v1
State:               RUNNING
Created:             ...
Deadline:            ...
Backend:             cpu-x64
Progress:            65%
Cancel requested:    no
Result:              pending
```

Exact fields, identifiers, labels and layout are not frozen here.

## Filtering and aggregate status

Future management UI may support filters by:

``` text
Session
Principal
Capability
State
Backend
```

and aggregate indicators such as:

``` text
Running:    3
Queued:    12
Failed:     1
Cancelled:  4
Capacity:  15 / 64
```

Exact counters and capacity semantics remain subject to the future
job/scheduler contract.

## Architectural boundary

Normative future direction:

``` text
Scheduler / Job Manager
        │
        ├── owns queue/lifecycle/scheduling state
        │
        ▼
Management / Inspection Model/API
        │
        ├── exposes bounded/safe runtime inspection
        │
        ├── applies authorization/policy
        │
        ▼
Frontends
        ├── Win32 GUI
        ├── CLI
        ├── future Web Management
        └── telemetry/diagnostics
```

Therefore:

``` text
GUI != Job Manager
GUI != Scheduler
GUI != source of runtime truth
```

The management frontend must not maintain a parallel queue model that
can drift from the server runtime.

## Required inspectability

The future runtime model should be designed so management can inspect,
when applicable:

``` text
Job identity
owner Session
owner Principal / policy subject
Capability Key/version
state
queue/admission state
selected backend
creation/start/end timing
deadline, when applicable
progress, only when the workload can report it honestly
result category
failure/diagnostic category
cancellation state
resource/capacity information, when defined
```

Not every job/capability is required to expose every optional field.

Unknown/not-applicable must remain distinguishable from fabricated
values.

## Administrative actions --- future candidates

Potential management actions include:

``` text
cancel Job
pause admission of new Jobs
resume admission
drain queue
inspect failure reason
inspect selected backend
inspect usage/time by Session or Principal
```

These actions are **not approved as concrete APIs by this document**.

Each mutating action must eventually have:

``` text
authorization
policy
audit
race/lifecycle semantics
failure semantics
```

Inspection and mutation should remain distinguishable.

## Security and privacy

Management visibility must not automatically expose secrets, payload
contents or sensitive user data.

Future inspection should favor:

``` text
stable IDs
bounded metadata
state categories
timing/resource summaries
safe diagnostic categories
```

rather than arbitrary payload dumps.

Principal/session visibility and administrative actions must obey
authorization policy.

## Relationship to scheduling

Heavy compute / worker-pool scheduling belongs to the future compute/job
architecture, not to the GUI.

This document requires **inspectability by design**:

``` text
Phase that defines Jobs / queue
        ↓
must not hide all useful runtime state inside GUI-inaccessible internals
```

But it does not freeze:

``` text
queue data structure
worker-pool implementation
priority algorithm
fairness algorithm
Job ID wire encoding
Management protocol
GUI toolkit/layout
remote-admin transport
```

## Future ADR candidates

When the Jobs/worker-pool architecture is concretely designed, evaluate
whether an ADR is needed for:

1.  Job ownership/lifecycle and stable identity.
2.  Queue/admission/scheduling authority.
3.  Management/Inspection boundary and authorization.
4.  Cancellation/drain semantics, if they become durable cross-phase
    decisions.

Do not reserve ADR numbers now.

## GUI direction

Conceptually:

``` text
PapinhoAccelerator Server
│
├── Status
├── Connections / Sessions
├── Capabilities
├── Jobs / Work Queue
│   ├── running
│   ├── queued
│   ├── waiting
│   ├── failed
│   └── cancelled
└── future management areas
```

This is a conceptual information architecture, not a frozen visual
design.

## Cross-references to maintain

When the project creates live documentation for Jobs / scheduler /
worker pool, that live document should cross-reference this file.

When an accepted ADR freezes part of this direction:

``` text
Future doc
→ reference accepted ADR

Live Job/Scheduler doc
→ reference accepted ADR

AGENTS.md
→ requires relevant docs/future/ during architectural work
```

## Principle

**The Job Manager owns runtime work; Management/Inspection exposes a
safe view; the GUI consumes that view and never becomes the queue's
source of truth.**
