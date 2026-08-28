# Connection I/O Scheduling Architecture

## Status and problem

Phase 2.D3B implements the initial single-threaded Win32 combined `select()`
loop. Listener readiness creates a fixed-slot Control Processor; Connection
read/write readiness drives bounded processor actions, and PAL-monotonic
deadlines reclaim stalled establishment attempts.

The dedicated D3B validation matrix covers processor, Session, and Channel
capacity isolation, the pre-`FD_SET` backend guard, state-derived write
interest, and rotating processor scan order.

Calling `papacc_framed_reader_next()` sequentially on blocking Connections is
incorrect: an idle peer can block in `recv()` and starve every other client.
Production Reader/Writer calls therefore require readiness scheduling.

## Initial Win32 baseline

The selected baseline is:

```text
single application-owned thread
+ one select()-based Server I/O Loop
+ nonblocking accepted Connection sockets
+ bounded round-robin work
```

This supports old WinSock and NT4/2000/XP, reuses existing `select()`
experience, avoids thread-per-Connection, and keeps ownership deterministic.
IOCP, `WSAPoll`, `WSAEventSelect`, worker pools, and multiple I/O threads are
not initial choices. A future readiness backend may replace `select()` without
changing portable protocol processing.

Readiness reduces `WOULD_BLOCK` but cannot replace nonblocking mode: readiness
may change before I/O. The Win32 transport/composition boundary applies
`ioctlsocket(FIONBIO)` after accept and before publication/processing. Failure
closes the accepted socket and publishes neither Connection nor processor.
Listeners may remain blocking because exactly one `accept()` follows observed
readability. No listener semantic change is required.

## Unified readiness and Acceptor evolution

Alternative B is selected: a future Server I/O Loop owns one readiness choice
covering listeners and Connections.

```text
Win32 Server I/O Loop (scheduling owner)
    |-- readable listener -> existing Acceptor accept-one flow
    |-- readable Connection -> portable processor receive turn
    `-- writable Connection with pending output -> processor send turn
```

Two independent blocking selects (Alternative A) would require coordination or
wakeups, duplicate policy, and complicate fairness/shutdown. The unified loop
must reuse existing accept/publication and rollback logic, never duplicate
`papacc_tcp_socket_win32_accept()` or manager publication. The Acceptor exposes
`papacc_server_acceptor_win32_accept_ready()` for one already-ready listener,
preserving failure atomicity. Compatibility `poll_once()` retains `select()`
and round-robin selection, then delegates to that operation.
`PAPACC_SERVER_NETWORK` remains passive infrastructure.

Normal errors are determined by read/write/EOF results; the design does not
depend on WinSock `exceptfds`.

## `FD_SETSIZE`

`FD_SETSIZE` limits the combined listener and Connection descriptor sets. It is
not a permanent PapinhoAccelerator limit and is not increased here. Multiple
set sharding is deferred. The current executable capacity of 64 Connections is
a temporary composition capacity, not protocol policy; future composition must
validate it against the platform's combined set capacity.

## Portable Protocol Connection Processor

A future portable `PAPACC_CONNECTION_PROCESSOR` (conceptual name) composes one
published Connection reference with:

```text
connection_instance_id/reference
Framed Reader and Framed Writer
fixed caller-owned read scratch storage
bounded establishment input/output storage
protocol establishment state
PAL-monotonic establishment deadline
```

`PAPACC_CONNECTION` remains a transport entity. Parser, Writer, messages,
handshake, and timeout policy do not move into it. Connection Manager owns
Connections; processor slots hold non-owning associations. Application-owned
fixed processor/scratch storage follows current project patterns and should
match Connection Manager capacity. Processor allocation/init failure closes or
rejects the Connection atomically.

Reader and Writer live in the processor slot and must shut down before their
Connection transport is destroyed. The application/processor aggregate owns
the Reader scratch buffer for its lifetime. There is no per-read heap. Each
processor has at most one Writer and one in-flight frame, not an outbound queue.

## Read scheduling and fairness

Readable may mean data, orderly shutdown, or error. It never authorizes an
unbounded drain. The initial budget is:

```text
at most one Framed Reader next()/event-or-progress action
per ready Connection per scheduling pass
```

The loop must not drain until `WOULD_BLOCK`. A round-robin Connection cursor
prevents every pass starting at slot zero; accepts are bounded too. The design
may later add byte/read/event budgets without changing ownership, but freezes
no larger numeric budget now.

Reader outcomes:

- `EVENT`: pass one framing event to the future message layer;
- `NEED_MORE_DATA`: yield until another turn;
- `WOULD_BLOCK`: await new readiness;
- clean EOF: close the Connection, creating no Session if still `PENDING`;
- `PROTOCOL_ERROR`, `NOT_SUPPORTED`, `LIMIT_EXCEEDED`: close without resync;
- fatal transport/internal error: close and release only that processor slot.

The message layer will consume `HEADER_READY`, `PAYLOAD_CHUNK`, and
`FRAME_COMPLETE` incrementally. Scheduling does not interpret unknown Message
Types or require full payload accumulation. Small establishment decoders may
use explicitly bounded fixed storage.

## Write scheduling and payload lifetime

A Connection enters the write `fd_set` if and only if its Writer has an
outbound frame awaiting progress (or a future queue actually contains output).
Idle TCP sockets are not monitored for writability, preventing busy loops.

One writable turn performs at most one Writer `step()`. Partial progress stays
pending. `WOULD_BLOCK` is normal: preserve Writer state and retry only after
later write readiness. Completion removes write interest unless another frame
starts. EOF or fatal error closes the Connection.

Initially there is at most one in-flight outbound frame per Connection. No
queue, queued-byte limit, priority, fairness, or cancellation policy is chosen.
The Writer retains no payload pointer, so the processor/message layer owns
stable reproducible storage for every unsent suffix through `FRAME_COMPLETE`.
Small handshake payloads may use fixed caller-owned encoding storage. Large
streaming producers and full backpressure policy belong to later data/compute
phases.

## Timeout and failure isolation

When processing becomes active, every `PENDING` Connection needs a bounded
establishment deadline owned by its Protocol Connection Processor. It uses the
PAL monotonic clock only, never wall clock. C4 selects owner and clock, not a
duration or implementation.

```text
one malformed/failed client != server failure
```

Timeout, framing error, EOF, and transport failure close only that Connection
and processor slot. Other Connections/listeners continue. Protocol code must
invoke existing lifecycle APIs. CONTROL loss therefore closes its Session and
DATA Channels through the existing Channel/Session contract, not duplicate
logic.

## Composition, future loop, and shutdown

Future composition keeps separate owners:

```text
Server Network              passive listeners/platform
Server Acceptor             accept/publication flow
Connection Manager          owns Connections
Session Manager             owns Sessions
Channel Manager             owns relationships
Protocol Processor storage  owns Reader/Writer/protocol slots
Win32 Server I/O Loop       owns readiness/scheduling
```

Implemented initial loop:

```text
while (!stop_requested) {
    determine combined readiness;
    accept a bounded number through the Acceptor;
    visit ready Connections round-robin;
    perform bounded read/write turns;
    process monotonic establishment deadlines;
}
```

The one I/O thread performs light protocol work only. Heavy compute and its
worker pool are a Phase 5 concern.

Shutdown order:

```text
stop new publication
 -> stop/wake and exit I/O scheduler
 -> shutdown processors (Reader/Writer before transport)
 -> Channel Manager lifecycle shutdown
 -> Session Manager lifecycle shutdown
 -> close/remove Connections and shutdown Acceptor/Connection Manager
 -> Server Network/listener shutdown
 -> global WinSock cleanup
```

Channel/Session coordination may refine relative actions once Control messages
exist, but no Reader/Writer may outlive its Connection. Cleanup is idempotent
and the console handler remains signal-only.

## Portability and security

Portable: Protocol Connection Processor, Reader/Writer/framing/message codecs,
Session/Channel coordination, and timeout semantics. Win32-specific: Server I/O
Loop, `select()` sets, native readiness, and accepted-socket `FIONBIO`. Future
transports supply their own readiness adapter/backend.

Logical layering remains:

```text
Transport -> future Transport Security -> Framing -> Protocol messages
```

Scheduling observes transport readiness but cannot bypass or weaken required
Transport Security.

## Phase 2.D boundary

The first protocol consuming this architecture is frozen in
[Control Establishment Protocol](control-establishment-protocol.md). D1 adds no
scheduler implementation. D3B now wires the portable processor to real Win32
RUN mode; post-establishment dispatch remains deferred.

Reader/Writer live in portable processor slots. The Win32 scheduler invokes
bounded processor turns only for buffered/readable/writable work. Errors close
their Connection scope. Establishment uses PAL-monotonic deadlines. Fairness is
bounded work plus round-robin traversal.

Still deferred: Message Type IDs, Control/Data payloads, Wire Session ID,
authentication, Transport Security mechanism, capability negotiation, general
send queues, workers, IOCP, POSIX poll/epoll/kqueue, and dynamic `FD_SETSIZE`
scaling.

**Checkpoint result: `CONNECTION I/O ARCHITECTURE READY`.** Scheduling,
ownership, fairness, errors, timeout, portability, and shutdown boundaries are
explicit enough for Phase 2.D without premature implementation.
