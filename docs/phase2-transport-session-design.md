# Phase 2 Transport & Session Architecture Checkpoint

## Status and scope

This checkpoint defines the enduring Phase 2 boundaries. Portable Session and
Channel foundations implement lifecycle and internal structural relationships.
The Transport Connection now exposes portable partial byte-stream read/write,
and framing has a portable incremental parser. A standalone portable Framed
Reader now provides caller-driven receive integration for those abstractions,
but production Connections do not own or run it. There is still no outbound
production processing, wire-driven association, authentication, or Transport
Security implementation. A standalone Framed Writer now provides caller-driven
outbound integration without attaching itself to a Connection.

The central invariant is:

```text
Transport Connection != Session
```

An accepted TCP stream is only a transport resource. It is not proof of identity, a Session, a Control Channel, or a Data Channel. Classification requires sufficient future protocol information.

## Layer model

```text
Listener
    passive resource that accepts transports
        ↓ ownership transfer
Transport Connection
    bidirectional stream, endpoints, transport-resource lifecycle
        ↓ optional future protection
Transport Security
        ↓
Framed Protocol Stream
        ↓ classification and establishment
Control Channel / Data Channel
        ↓ logical ownership and policy
Session
```

Responsibilities are separated as follows:

- **Listener:** owns listening resources and accepts a new native transport. It has no client, protocol, authentication, or Session state.
- **Transport Connection:** owns one accepted bidirectional transport and exposes abstract read/write/close behavior plus optional local/remote endpoint metadata. It starts `UNCLASSIFIED`/`PENDING`.
- **Session:** owns the logical client context after establishment has begun, including lifecycle, future identity, policy, capabilities, and its channel relationships.
- **Control Channel:** the primary Session channel for setup, negotiation, commands, status, heartbeat, errors, and shutdown coordination.
- **Data Channel:** an additional Session-associated channel for large payloads, media, framebuffer, or job data.

The portable Channel relationship foundation now represents CONTROL and DATA
bindings through runtime IDs. A bound Channel relates exactly one published
Connection to one published Session without taking ownership of either entity.
The Channel Manager is the sole relationship table; Connection and Session do
not contain reverse Channel pointers or collections.

`channel_instance_id` is a nonzero, manager-assigned `PAPACC_U64` for runtime
diagnostics and management only. It is not persistent, grants no authority,
is not automatically wire-visible, and is not a future Wire Channel ID.

After binding, a Connection moves from `PENDING` to `ASSOCIATED`. This means
only that a higher logical layer has claimed it through a Channel; it does not
mean authenticated, secure, negotiated, or application-ready.

Initially the implemented transport will be TCP. Session and protocol layers must never depend on `SOCKET`; future local transports may have different endpoint and I/O semantics.

### Transport stream I/O

`PAPACC_TRANSPORT_CONNECTION` is an owned ordered byte stream with abstract
read, write, and close callbacks. A successful read or write may make partial
progress. Normal stream conditions are explicit and separate from API errors:
`PROGRESS` transfers one or more bytes, `WOULD_BLOCK` transfers none, and
`END_OF_STREAM` transfers none. Zero-length operations are valid
`OK + UNSPECIFIED` no-ops and do not invoke the backend.

Each wrapper invokes its backend at most once. It has no receive buffer, send
queue, or write-all loop. The caller owns all buffers, must retain an unsent
suffix after partial write, and may retry later according to future readiness
and backpressure policy. The Win32 TCP adapter maps `recv() == 0` to clean EOF,
maps `WSAEWOULDBLOCK` explicitly, and limits one native span to `INT_MAX`
without changing the portable `PAPACC_SIZE` contract. The Win32 Acceptor sets
accepted sockets nonblocking before publication; transition failure closes
only the new socket. The standalone Framed Reader still has no Server Runner
or combined readiness-loop integration.

The Framed Writer encodes one semantic header into fixed local storage and
streams payload spans owned by its caller. Each step performs at most one
transport write and never retains a payload pointer. Partial writes and
`WOULD_BLOCK` require the caller to resubmit the unconsumed suffix. No outbound
queue, write-all behavior, readiness integration, or production protocol
processing is defined.

## Connection contract

### Conceptual model

A future `PAPACC_CONNECTION` (name not frozen) should contain portable runtime metadata separately from platform transport state.

Portable metadata may include:

- lifecycle state;
- `UNCLASSIFIED`, future CONTROL, or future DATA role;
- optional local and remote IP endpoints for TCP;
- runtime-only connection correlation identity;
- monotonic timestamps/deadlines;
- association with a Session only after an explicit ownership transition.

The native TCP socket belongs behind a transport-specific handle/context and operations. It must not appear in a portable Connection or Session header. A portable layer may hold an opaque transport reference whose implementation owns the native state; the exact ABI is deferred until the accepted TCP lifecycle is designed.

### Connection runtime identity

A runtime `connection_instance_id` is recommended for logs, diagnostics, and management. It is process/runtime-local, is not persistent, is not a Session ID, conveys no authority, and is never automatically wire-visible. Its type, allocator, and wrap policy are deferred.

### Endpoints

A dedicated portable IP endpoint model is recommended for the next subphase:

```text
PAPACC_NETWORK_ENDPOINT
    PAPACC_IP_ADDRESS address
    PAPACC_U16 port
    PAPACC_U32 scope_id
```

This is an IP/TCP metadata value, not a universal transport endpoint. Future non-IP transports may report another metadata form or no IP endpoint.

```text
PAPACC_BIND_TARGET != PAPACC_NETWORK_ENDPOINT
```

`PAPACC_BIND_TARGET` is an administrative/runtime listening target and contains interface association. A Connection Endpoint describes the actual local or remote endpoint of an established transport and includes the effective port. Neither model should be reused as the other merely because both contain an address.

## Accept responsibility and ownership

The future primitive belongs to the TCP backend. It accepts from an active listener and produces an unpublished accepted TCP transport plus local/remote endpoint information. Application/transport composition wraps that result in a runtime Connection. Session code does not call native accept.

Ownership transitions must be explicit:

1. Listener Set owns only listening sockets.
2. During accept, the backend owns a native accepted socket locally.
3. If native accept or Connection construction fails, the backend/composition closes every acquired accepted resource and publishes nothing; the listener remains operational after recoverable errors.
4. On successful publication, ownership transfers exactly once to the pending Connection owner, expected to be a future Server Acceptor/Connection Manager.
5. Before Session establishment, that manager owns and closes pending Connections.
6. When a Connection is promoted to or associated with a Session channel, ownership transfers explicitly to the Session/channel aggregate. There is no shared implicit socket ownership.
7. On Session or server shutdown, the current owner closes the Connection idempotently.

`PAPACC_SERVER_NETWORK` remains responsible only for TCP Platform, listeners, listener-entry storage, and listening infrastructure. It must not acquire Session lists, accepted Connection state, authentication, or parsers. A future `PAPACC_SERVER_ACCEPTOR` or `PAPACC_SERVER_CONNECTION_MANAGER` should sit above it.

## Blocking and threading baseline

Transport primitives remain individually capable of blocking. Initial
production Connection processing is now architecturally frozen as one
application-owned Win32 I/O thread using a unified `select()` loop for
listeners and nonblocking accepted Connections. Work is bounded and
round-robin; write readiness is monitored only for pending output.

Thread-per-Connection, IOCP, `WSAPoll`, and worker pools are not the initial
baseline. The normative readiness, ownership, fairness, timeout, failure, and
shutdown contract is [Connection I/O Scheduling](connection-io-scheduling.md).
D3A implements the accepted-socket nonblocking transition and an accept-ready
composition seam, but none of the combined scheduling loop.

## Session contract

The first normative wire transition toward this runtime is frozen in
[Control Establishment Protocol](control-establishment-protocol.md). It creates
a Session/CONTROL relationship transactionally and activates only after the
complete ACCEPT frame is written. The Win32 Server I/O Loop now invokes its
portable processor in production RUN mode.

A Connection does not create an active Session merely by connecting. The conceptual Session lifecycle baseline is:

```text
UNINITIALIZED
    ↓ sufficient establishment input
ESTABLISHING
    ↓ negotiation/authentication/policy success
ACTIVE
    ↓ local stop, control loss, fatal error, or timeout
CLOSING
    ↓ owned channels/resources released
CLOSED
```

The Session Foundation implements these state names. Manager publication creates an `ESTABLISHING` Session, and only an explicit runtime transition can make it `ACTIVE`. No production protocol event calls that transition yet.

### Session identity

The implemented `session_instance_id` is a nonzero, manager-assigned `PAPACC_U64` used only for runtime diagnostics and management. It is not persistent, conveys no authority, and is never automatically wire-visible.

```text
session_instance_id != future Wire Session ID
```

The size, representation, randomness source, encoding, cryptographic properties, and wire form of a future protocol Session ID remain deliberately undefined. Knowledge of either identifier is never authentication and is insufficient to associate a Data Channel or acquire authority.

### Control Channel baseline

The initial baseline is exactly one primary Control Channel per established Session. Multiple primary Control Channels and seamless replacement are not supported by the initial contract. Loss of the primary Control Channel initiates Session closing and closure/cancellation of associated Data Channels. Session survival or resumption after control loss requires a future explicit design and must not occur implicitly.

### Data Channels

A Session may later have zero or more Data Channels. No fixed count is chosen; server/session policy and resource limits will bound them.

A new Connection remains pending until it presents future protocol information sufficient to classify it. A Data Channel must prove authorized association using something beyond Session ID. Possible future inputs include an authenticated token, nonce, channel binding, or Transport-Security-derived secret, but no mechanism is selected here.

The implemented DATA bind API is an internal structural operation requiring an
ACTIVE Session and an existing bound CONTROL Channel:

```text
internal structural DATA binding != secure remote Data Channel association
```

Knowledge of `session_instance_id`, `channel_instance_id`, or any future wire
identifier grants no authority to attach a remote Data Channel. No wire
Session/Channel identifier or association proof is defined by this foundation.

## Protocol stream requirements

The Envelope 1.0 byte layout and incremental parser/encoder requirements are
now frozen in [Protocol Framing](protocol-framing.md). That specification
defines only the common envelope; actual message types and payloads remain
unassigned.

TCP is a byte stream:

```text
recv() boundary != message boundary
```

The implemented framing layer supports partial headers and payloads, multiple
frames per read, incremental parsing, bounded lengths, overflow-safe
arithmetic, version validation, and deterministic malformed-input cleanup. Its
normative layout remains defined only by the framing document.

Writes are incremental through the Framed Writer. `send()` may accept fewer
bytes than requested or apply backpressure. Future queues/producers must be
bounded, cancellation-aware, and explicitly owned; no queue is implemented or
sized by this checkpoint.

## Timeouts and resource policy

Future pending-connection, establishment, authentication, idle, heartbeat, and shutdown deadlines must use the existing PAL monotonic clock, never wall clock.

Policies will be required for maximum pending Connections, Sessions, Data Channels per Session, queued bytes, handshake duration, idle duration, and work/resource consumption. This checkpoint assigns no numeric defaults. Exhaustion must reject or close the narrowest safe scope without corrupting listeners or existing Sessions.

## Shutdown ordering

The future server shutdown order is:

```text
stop accepting/publishing new Connections
        ↓
signal active Sessions to close
        ↓
close pending and Session-owned Connections
        ↓
shutdown listeners
        ↓
WinSock cleanup
```

Protocol processors and their Readers/Writers must shut down before their
Connection transports. Detailed ordering is frozen in
[Connection I/O Scheduling](connection-io-scheduling.md). Shutdown remains
idempotent and the console handler must not perform competing cleanup.

## Security, authentication, and capabilities

```text
Transport Security != TLS Offload
```

The architecture reserves Transport Security between raw Transport Connection and framed protocol processing, so protocol/session layers consume a protected stream when policy requires it. No TLS mechanism or library is chosen.

Authentication begins after transport establishment and before a Session becomes fully ACTIVE. Exact sequencing relative to Transport Security and negotiation is deferred to the security/protocol checkpoint. Authentication failure must not promote the pending context.

Capability negotiation belongs to the primary Control Channel and Session policy. It never belongs to Listener or Transport Connection, and it cannot weaken required Transport Security. No capability ID is assigned.

## Errors and logging

Error ownership follows layer boundaries:

- transport errors originate in the transport backend and are translated from native errors;
- protocol errors originate in framing/message/state validation;
- Session errors originate in lifecycle, association, policy, or resource decisions.

Native WSA codes must not become wire error values. A later diagnostic design may retain native detail locally while exposing portable categories upward.

The existing logging foundation should correlate listener, runtime Connection, Session, and channel through local metadata. Correlation does not require identifiers to be persistent or wire-visible, and logs must not turn IDs into authorization credentials.

## Test strategy for implementation subphases

Upcoming subphases should add deterministic real-socket tests for listener continuity, client fixture connect, accept, local/remote endpoints, close, rollback, and reaccept after disconnect. A simple WinSock client in tests is a fixture, not a production PapinhoAccelerator Client.

Tests must avoid topology-specific addresses and arbitrary race-masking sleeps. Connection tests precede protocol bytes. CTest remains the gate; external GUI automation is not required.

## Decisions

1. Connection and Session are distinct objects and lifecycles.
2. New accepted Connections begin pending/unclassified.
3. TCP backend owns the native accept primitive; application composition constructs runtime Connections.
4. Accepted resource ownership transfers exactly once and never becomes implicitly shared.
5. Portable Connection/Session APIs do not expose `SOCKET`.
6. A new IP endpoint value is preferable to reusing Bind Target.
7. Connection correlation identity is runtime-only and non-authoritative.
8. Initial Sessions have exactly one primary Control Channel.
9. Primary Control Channel loss closes the initial Session baseline.
10. Session ID alone cannot associate a Data Channel.
11. Framing and backpressure sit above transport stream I/O.
12. All lifecycle timeouts use monotonic time.
13. Server Network remains listening infrastructure only.
14. Initial production I/O uses one application-owned Win32 `select()` loop,
    nonblocking accepted Connections, bounded round-robin turns, and no
    thread-per-Connection.
15. Transport Security can be inserted between raw transport and framing.

## Recommended implementation sequence

1. **2.A1 — Portable Network Endpoint Model:** IP endpoint initializer, validity, equality, IPv6 scope and tests; no socket.
2. **2.A2 — Win32 TCP Accept Primitive:** accepted socket ownership, endpoint capture, failure atomicity and real client fixture tests.
3. **2.A3 — Accepted TCP Connection Lifecycle:** backend context, close/shutdown, portable abstraction boundary and runtime correlation metadata.
4. **2.A4 — Server Acceptor Foundation:** listener readiness, pending Connection ownership, stop coordination and reaccept tests; no protocol.
5. **2.B1 — Portable Session Foundation:** lifecycle and ownership containers without wire encoding.
6. **2.B2 — Channel Role/Association Foundation:** pending classification and logical Control/Data relationships without authentication mechanism.
7. **2.C1 — Protocol Framing Requirements Freeze:** limits, incremental parser contract, version behavior and error ownership.
8. **2.C2 — Framing Implementation:** only after 2.C1 approves a wire design.

## Deferred decisions

This checkpoint still deliberately does not choose:

- real message numbers or message payload layouts beyond the already frozen
  common Envelope 1.0;
- Session ID type, size, generator, encoding, or wire representation;
- authentication messages, credentials, Data Channel association proof, or cryptographic mechanism;
- TLS library or Transport Security configuration;
- capability numeric IDs;
- maximum Connections, Sessions, channels, queue sizes, or timeout values;
- universal endpoint representation for non-IP transports;
- worker count, IOCP use, or long-term replacement for the frozen initial
  `select()` event loop;
- UDP;
- Session resumption, multiple/replacement Control Channels, or survival after control loss.

No code, message ID, packet layout, default port, or security implementation is introduced by this document.
