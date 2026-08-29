# Phase 2 Integration Audit

## Scope and conclusion

This is the authoritative closure audit for Phase 2 — Transport Layer & Session
Management. It covers portable runtime/protocol layers, Win32 transport and
single-thread composition, executable integration, ownership, lifecycle,
failure isolation, shutdown, portability, security boundaries, and tests.

Phase 2 provides structural communication only. It implements no authentication,
authorization, Transport Security, capability negotiation, compute work, network
egress, TLS offload, or post-DATA application protocol.

## Final architecture

```text
Server Network -> Server Acceptor -> Connection Manager
                                      |
                                Protocol Slot
                                      |
                                Classifier
                                  /       \
                       CONTROL_OPEN       DATA_ATTACH
                            |                  |
                  Control Establishment   DATA Attach Processor
                            |                  |
                      Post-Control    Association consume -> Channel Manager
                            |
                  Association Manager issue

Session Manager <------ Channel Manager ------> Connection Manager
Association Manager references Session/Channel Managers but owns neither.
```

One application thread owns one combined Win32 `select()` over listeners and
all Protocol Slot interests. Each pass accepts at most one Connection and
performs at most one I/O action per ready slot. No-I/O handoff transitions may
follow that action. Listener and slot scans rotate independently.

## Component inventory and ownership

Portable components: types/results, IP/endpoint, Transport Connection,
Connection/Session/Channel runtimes, Frame encoder/parser, Framed Reader/Writer,
Control codecs and establishment processor, DATA association codecs and Ticket,
Association Manager, Post-Control Processor, Connection Classifier, and DATA
Attach Processor.

Win32/application components: WinSock lifecycle, TCP bind/listen/accept,
nonblocking accepted-socket adapter, Multi-Listener Set, Server Network,
Server Acceptor, Protocol Slot, combined I/O Loop, ticket candidate provider,
and server runner.

| Component | Owns | Does not own |
|---|---|---|
| Server Acceptor | Connection Manager aggregate and publication | Server Network/listeners |
| Connection Manager | published Connection slots | caller storage |
| Win32 transport context | accepted `SOCKET` after move | listener socket |
| Session Manager | Session entities | Channels or tickets |
| Channel Manager | CONTROL/DATA relationships | Sessions, Connections, transports |
| Association Manager | ticket entries in caller storage | Session/Channel Managers |
| Classifier | local Reader/classification state | transport, scratch, Managers |
| Control Processor | local Reader/Writer and state | transport, scratch, Managers |
| Post-Control Processor | local Reader/Writer and state | transport, scratch, Managers |
| DATA Attach Processor | local Reader/Writer and state | transport, scratch, Managers |
| Protocol Slot | one Connection composition and 64-byte scratch | runtime Managers |

The scratch and all capacities are application-private choices, not wire limits,
product configuration, or protocol guarantees.

## Wire registry and invariants

| ID | Message |
|---:|---|
| `0x0000` | RESERVED / INVALID |
| `0x0001` | CONTROL_OPEN |
| `0x0002` | CONTROL_ACCEPT |
| `0x0003` | DATA_TICKET_REQUEST |
| `0x0004` | DATA_TICKET |
| `0x0005` | DATA_ATTACH |
| `0x0006` | DATA_ACCEPT |

No other normative Phase 2 IDs exist. Envelope 1.0 is `PACC`, Major 1,
Minor 0, 16-byte header, flags zero, and big-endian integers. Encoding uses
explicit bytes, not packed structs or unaligned casts. Control Protocol 1.0 is
distinct from Envelope and software versions.

```text
CONTROL_OPEN   50 41 43 43 01 00 00 10 00 01 00 00 00 00 00 04 00 01 00 00
CONTROL_ACCEPT 50 41 43 43 01 00 00 10 00 02 00 00 00 00 00 04 00 01 00 00
TICKET_REQUEST 50 41 43 43 01 00 00 10 00 03 00 00 00 00 00 00
DATA_ACCEPT    50 41 43 43 01 00 00 10 00 06 00 00 00 00 00 00
```

DATA_TICKET and DATA_ATTACH are a 16-byte header plus one opaque, nonzero,
16-byte ticket. Connection, Session, and Channel runtime IDs are local,
diagnostic, non-authoritative, and never encoded. There is no Wire Session ID
or Wire Channel ID.

## Ticket and security boundary

The ticket is structural association material only: all-zero invalid, one
outstanding per ACTIVE Session, monotonic expiry, one-time consumption, and
replay rejection. Consume is irreversible even if bind or DATA_ACCEPT fails.
The server provider is an application-private, replaceable, monotonically
incremented 128-bit opaque counter. It uses no runtime IDs, `rand()`, or crypto
provider and claims no secrecy or unpredictability.

Session `ACTIVE` means only that Phase 2 Control establishment completed. It
does not mean authenticated, authorized, trusted, encrypted, secure, or
capability-negotiated.

## Lifecycle matrix

| Flow | Runtime transition | Success |
|---|---|---|
| CONTROL | PENDING -> classified -> Session ESTABLISHING + CONTROL BOUND + Connection ASSOCIATED -> ACCEPT | Session ACTIVE, Post-Control active |
| DATA | PENDING -> classified -> ticket consumed -> DATA bind | Connection ASSOCIATED, DATA BOUND, ACCEPT sent |
| DATA loss | close that DATA Channel | Session, CONTROL, other DATA survive |
| CONTROL loss | Channel Manager closes CONTROL | Session, every DATA, and tickets close/invalidate |

Multiple DATA Channels use sequential request/consume cycles. There is no
permanent one-DATA limit.

## No-lost-byte invariant

**No protocol-layer handoff may discard bytes already read from transport.**
Framed Reader move transfers parser state, buffered offsets/length, scratch and
transport references without I/O or copying and resets its source. Classifier
to Control, Classifier to DATA, and Control to Post-Control use this primitive.
Partial-header, buffered-payload, multiple-frame, fully buffered DATA, and
pipelined CONTROL/request tests cover the invariant.

## Scheduling, time, and capacity

Accepted Connections receive FIONBIO before publication; listeners remain
blocking. One select observes listeners and mixed Protocol Slots. Buffered data
forces a zero timeout. Write interest exists only for CONTROL_ACCEPT,
DATA_TICKET, or DATA_ACCEPT. Established DATA has no read/write interest.

FD_SETSIZE is guarded before FD_SET and is only a Win32 backend limitation.
Private capacities are 64 each for Connections, Sessions, Channels, Protocol
Slots, and Associations. Capacity failures are isolated and storage reusable.

The saturated PAL-monotonic establishment deadline is 15 seconds and moves
unchanged through handoff. Ticket lifetime is a separate saturated monotonic
30 seconds. No wall clock participates. Every poll executes Association expiry.

## Reclamation and shutdown

The dependency-ordered reaper is:

```text
terminal Protocol Slots
-> expired/lifecycle-invalid tickets
-> CLOSED Channels
-> CLOSED Sessions
-> CLOSED Connections
```

It does not remove ACTIVE Sessions, BOUND Channels, live ASSOCIATED Connections,
or live slots. CONTROL cascade tests prove all related slots become reusable
and a new Session can establish afterward.

Shutdown stops scheduling, shuts slots, Association/provider, Channels, and
Sessions. The runner then shuts Acceptor/Connections, frees fixed arenas, and
its caller shuts Server Network/listeners, WinSock, and console lifecycle.
Shutdown APIs are idempotent.

## Failure isolation

| Failure | Scope | Server/unrelated clients | Reclaimed |
|---|---|---|---|
| malformed/unknown first frame | candidate only | survive | yes |
| unsupported Control / timeout | candidate and uncommitted state | survive | yes |
| Session/Channel/Slot capacity | admission scope | survive | yes |
| Association capacity | affected CONTROL under current no-error-schema policy | survive | yes |
| zero/unknown/expired/replayed ticket | DATA candidate only | survive | yes |
| DATA bind/ACCEPT failure | candidate/new DATA; ticket stays consumed | survive | yes |
| CONTROL protocol failure/EOF | CONTROL, Session, its DATA and tickets | survive | yes |

Peer protocol, unsupported, admission, EOF, and timeout failures are not
RUN-fatal. PAL clock failure, unexpected select failure, or broken aggregate
invariants may terminate RUN as infrastructure failures.

## Coverage matrix

| Requirement | Component | Test/scenario | Layer | Status |
|---|---|---|---|---|
| Envelope and vectors | Frame/Control/DATA codecs | codec tests | portable unit | pass |
| incremental framing | Parser/Reader/Writer | fragmentation, partial I/O, WOULD_BLOCK | portable unit | pass |
| no-lost-byte | Reader/classifier/processors | partial header, buffered payload, pipelined frames | portable integration | pass |
| lifecycle cascade | Channel Manager | CONTROL/DATA relationship tests | portable unit | pass |
| expiry/replay | Association/DATA Attach | equality deadline, replay, invalid lifecycle | portable integration | pass |
| server sweep | I/O Loop | periodic expire path plus deterministic expiry tests | cross-layer | pass |
| isolation/fairness | Win32 loop | idle/malformed beside valid client | Win32 integration | pass |
| capacities/FD guard | Managers/slots/select | exhaustion, reuse, synthetic FD_SETSIZE | Win32 integration | pass |
| full CONTROL + DATA | runner and loop | OPEN, ticket, second Connection, ATTACH/ACCEPT | executable | pass |
| idempotent/replay/multi-DATA | real loop/manual RUN | duplicate, replay EOF, second ticket | integration | pass |
| CONTROL cascade/recovery | real loop | CONTROL + two DATA, reap, fresh Session | Win32 integration | pass |
| active-DATA shutdown | runner | official console stop with CONTROL + DATA | executable | pass |

Coverage is layered rather than duplicated. Portable deterministic expiry plus
server sweep covers expiry without waiting 30 seconds. Portable pipelined tests
exercise the exact handoff APIs composed by Win32. Fully buffered DATA is tested
portably and through the real loop.

## Portability and build boundary

Portable protocol/runtime targets contain no Win32 types, SOCKET, select,
FIONBIO, WSA APIs, or per-frame heap allocation. Managers/processors use
caller-owned fixed storage. Only the runner allocates its fixed arenas once.
Dependency direction remains portable runtime/protocol -> Win32/application ->
runner. Targets retain C99, extensions OFF, and `/W4`.

## Intentional limitations and deferred work

- no Authentication, Authorization, Transport Security, credentials, or policy;
- no capability negotiation, compute work, network egress, or TLS offload;
- no DATA application messages or general outbound queue;
- no automatic idle established-DATA EOF observation or `MSG_PEEK`;
- no multithreaded protocol I/O, IOCP, WSAPoll, or POSIX backend;
- select/FD_SETSIZE scaling and application-private fixed capacities;
- application-private structural ticket provider and lifetimes.

Phase 3 may insert Authentication and Authorization at Control establishment
and DATA association, and Transport Security between Transport and Framing. It
may replace ticket generation or binding while retaining the opaque 16-byte
field if its security design permits. Phase 4+ owns capability, policy, compute,
and egress concerns.

## Closure validation

The final audit configured and generated the existing CMake build, performed a
clean 152-step Debug rebuild, and ran the complete CTest suite: 41/41 passed.
The clean `/W4` build emitted zero compiler warnings and `git diff --check`
reported no whitespace errors. The executable smoke test and
`--list-interfaces` both returned zero. A real `--all-interfaces` RUN probe
opened IPv4 and IPv6 wildcard listeners and entered Control acceptance; the
automated runner test separately proves graceful console stop while an active
CONTROL plus DATA association exists.

## Final recommendation

PHASE 2 READY
