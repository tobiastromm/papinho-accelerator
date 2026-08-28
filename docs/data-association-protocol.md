# Data Association Protocol 1.0

## Status, scope, and security boundary

Phase 2.E1 freezes the wire design only. It implements no codec, ticket
manager, classifier, processor, scheduler integration, authentication,
Transport Security, capability, or DATA application protocol.

DATA association uses a one-time 16-byte ticket obtained over an established
CONTROL Channel. The ticket is opaque correlation material, not an
authentication token, credential, identity, or final authorization. Phase 2
does not claim resistance to observation, theft, guessing, replay interception,
MITM, or connection hijacking. Phase 3 must be able to insert authenticated and
authorized association validation before the existing lifecycle checks/bind.

Runtime `session_instance_id`, `connection_instance_id`, and
`channel_instance_id` remain diagnostic, non-authoritative, and never appear on
wire. No stable Wire Session ID or Wire Channel ID is introduced.

## Message registry, direction, and channel

| ID | Message | Direction and location |
|---:|---|---|
| `0x0000` | RESERVED / INVALID | none |
| `0x0001` | `CONTROL_OPEN` | client -> accelerator, new CONTROL candidate |
| `0x0002` | `CONTROL_ACCEPT` | accelerator -> client, CONTROL candidate |
| `0x0003` | `DATA_TICKET_REQUEST` | client -> accelerator, established CONTROL only |
| `0x0004` | `DATA_TICKET` | accelerator -> client, established CONTROL only |
| `0x0005` | `DATA_ATTACH` | client -> accelerator, first frame of new PENDING Connection |
| `0x0006` | `DATA_ACCEPT` | accelerator -> client, new DATA Connection |

No additional type is assigned. Wrong direction or channel is a protocol-state
violation, not a framing error. Flags are zero for all four E1 messages.

## Ticket value and server-side state

A ticket is exactly 16 opaque bytes (128 bits), with no timestamp, runtime ID,
counter, checksum, version, or other wire-visible subfield. All-zero is
RESERVED/INVALID. Issuance must produce a nonzero value unique among currently
valid outstanding tickets and must not serialize runtime IDs. The generation
algorithm and library are local implementation details, not wire format or a
cryptographic-authentication claim.

The Data Association layer, preferably a separate portable fixed-capacity
manager keyed internally by Session runtime identity, owns:

```text
outstanding_ticket + ticket_valid + ticket_deadline_ns
```

These fields do not belong in the minimal `PAPACC_SESSION` lifecycle model.
Association-state capacity should cover Session Manager capacity; this is
composition policy, not wire policy.

There is at most one outstanding unconsumed ticket per ACTIVE Session. This
requires no queue, bounds state, and still permits 0..N DATA Channels by
sequential request/attach/consume cycles.

Tickets expire against a server-side PAL-monotonic deadline which is never sent
on wire. E1 freezes no duration. Ticket expiry and a candidate Connection's
establishment/classification deadline are distinct.

An outstanding ticket is valid only while its Session is ACTIVE and its primary
CONTROL Channel is BOUND. CONTROL loss or Session close invalidates it.

## Ticket request and response

`DATA_TICKET_REQUEST` (`0x0003`) has Payload Length 0 and is valid only over a
BOUND CONTROL relationship for an ACTIVE Session and ASSOCIATED Connection.

```text
50 41 43 43 01 00 00 10
00 03 00 00 00 00 00 00
```

If a valid ticket is already outstanding, a repeated request re-sends the same
ticket, allocates no new state, and does not refresh its expiry. This is
idempotent recovery without a general Error schema. After consumption, or
after invalidating an expired ticket, a later request issues a new ticket.

`DATA_TICKET` (`0x0004`) has Payload Length 16 containing the unchanged opaque
ticket. With synthetic example bytes `00..0F` (not a constant or generation
rule):

```text
50 41 43 43 01 00 00 10
00 04 00 00 00 00 00 10
00 01 02 03 04 05 06 07
08 09 0A 0B 0C 0D 0E 0F
```

The client temporarily stores the ticket, presents it in `DATA_ATTACH`, and
discards it after `DATA_ACCEPT` or attach failure/Connection close.

## Connection classification and DATA_ATTACH

The Phase 2.D Control-only first-frame rule is now contextual. A new PENDING
Connection may begin with exactly `CONTROL_OPEN` or `DATA_ATTACH`. A classifier
built on Framed Reader/Frame Parser inspects the first `HEADER_READY` and routes
to a distinct Control Establishment Processor or future Data Attach Processor.
It must not peek native bytes or force DATA logic into
`PAPACC_CONTROL_PROCESSOR`. Unknown first type closes only that Connection.

`DATA_ATTACH` (`0x0005`) must be the first DATA-candidate frame and has Payload
Length exactly 16. Other lengths are message-layer `PROTOCOL_ERROR`. Example:

```text
50 41 43 43 01 00 00 10
00 05 00 00 00 00 00 10
00 01 02 03 04 05 06 07
08 09 0A 0B 0C 0D 0E 0F
```

No structural commit occurs at `HEADER_READY`. It requires the complete exact
ticket, nonzero validity, outstanding/unexpired resolution, target Session
ACTIVE, and primary CONTROL BOUND.

Once a valid outstanding ticket resolves, it is consumed before or atomically
with the structural bind attempt. It is never restored if Channel capacity or
another later bind step fails. Replay closes the candidate Connection and does
not affect an already established DATA Channel. Unknown, zero, expired, or
consumed tickets likewise close only the candidate and create no relationship;
zero is preferably `PROTOCOL_ERROR`, while expiry needs no new public result.

The structural operation uses existing `papacc_channel_manager_bind(...,
DATA, ...)`, which transitions the candidate Connection from PENDING to
ASSOCIATED and creates a BOUND DATA Channel. It publishes no Session and does
not replace CONTROL; the target Session stays ACTIVE. Channel-capacity failure
consumes the ticket, closes the candidate, and leaves Session/CONTROL intact.

## DATA_ACCEPT and failure lifecycle

After structural bind, `DATA_ACCEPT` (`0x0006`) is written on that DATA
Connection with Payload Length 0:

```text
50 41 43 43 01 00 00 10
00 06 00 00 00 00 00 00
```

It echoes no ticket or runtime identifier. Structural commit (ticket consumed,
DATA BOUND, Connection ASSOCIATED) is distinct from peer-visible completion at
Writer `FRAME_COMPLETE`. During partial write/`WOULD_BLOCK`, those runtime
entities remain committed but DATA payload processing must not begin. After
completion the future Data Attach Processor becomes ESTABLISHED; Session stays
ACTIVE.

Writer EOF/fatal failure before completion closes only the DATA Channel through
Channel Manager lifecycle. Its Connection closes; Session, CONTROL, and other
DATA Channels remain alive. More generally:

```text
DATA loss != Session loss
CONTROL loss -> Session closes -> every DATA closes -> tickets invalidate
```

No `DATA_REJECT` or generic Error message is defined. Early association failure
closes the candidate Connection.

## Processor handoff and future implementation boundaries

`PAPACC_CONTROL_PROCESSOR` remains establishment-only. At ESTABLISHED its local
Reader/Writer may be shut down without closing the ACTIVE Session, BOUND
CONTROL, or ASSOCIATED Connection. A separate portable post-establishment
Control Session Processor will initialize its own Reader/Writer and initially
accept only `DATA_TICKET_REQUEST`; other current-state messages violate protocol
state.

The future Data Attach Processor owns Reader, exact ticket staging, resolution
and consumption, lifecycle validation, DATA bind, DATA_ACCEPT Writer, and a
PAL-monotonic attach deadline. Conceptual states are UNINITIALIZED,
WAITING_DATA_ATTACH_HEADER, READING_DATA_ATTACH, WRITING_DATA_ACCEPT,
ESTABLISHED, CLOSED, and ERROR. No DATA payload subprotocol is defined after
association.

E2 is the portable implementation boundary: four message constants/codecs,
ticket value, fixed association manager, issue/reissue/expiry/consume rules,
post-Control ticket processor, Data Attach Processor, classification support,
and deadline metadata. E2 may split into Ticket Runtime/Codecs and portable
processors without revisiting wire decisions.

E3 integrates post-Control processing, classification, Data Attach processors,
ticket storage/deadlines, Win32 readiness, and executable composition while
preserving single-threaded bounded round-robin work and failure isolation. E3
must not alter IDs, sizes, vectors, or one-time semantics.

## Future security insertion point

```text
Phase 2:
ticket resolve -> Session/CONTROL lifecycle validation -> DATA bind

Phase 3-capable:
ticket resolve -> authenticated/authorized association validation
               -> Session/CONTROL lifecycle validation -> DATA bind
```

Channel Manager ownership semantics remain unchanged. Tickets are not final
secure authorization, and E1 defines no authentication, authorization,
Transport Security, TLS, capability negotiation, egress negotiation, jobs,
media, chunks, or request/response DATA protocol.

## Required future tests

E2 must cover ticket zero/equality/validity; issue and same-ticket idempotent
reissue without expiry extension; consume/replay/expiry/new issuance;
CONTROL/Session invalidation; no runtime-ID serialization; four exact vectors
and lengths; post-Control state/direction, fragmentation, partial writes and
`WOULD_BLOCK`; Data Attach wrong type/length, zero/unknown/expired/consumed
ticket, lifecycle prerequisites, bind/rollback, consumed-on-capacity-failure,
exact/partial DATA_ACCEPT, isolated Writer failure, and sequential second DATA.

E3 must cover real Control establishment and ticket request, exact DATA_TICKET,
second TCP Connection, fragmented DATA_ATTACH, exact DATA_ACCEPT, ACTIVE Session
with BOUND CONTROL/DATA, two sequential DATA Channels, replay/expiry rejection,
idle classifier fairness, malformed-candidate isolation, DATA-loss isolation,
CONTROL-loss cascade/invalidation, capacity reclaim, and clean shutdown.

**Checkpoint result: `DATA ASSOCIATION DESIGN READY`.**
