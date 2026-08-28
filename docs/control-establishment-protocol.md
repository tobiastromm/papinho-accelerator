# Control Establishment Protocol 1.0

## Status and layering

This document normatively freezes the first message family. Phase 2.D2 now
implements its portable codecs and server-side establishment processor. D3B
integrates that processor into the real Win32 RUN mode. Security,
authentication, Session wire identity, DATA association, and a general
post-establishment Control dispatcher remain absent. Phase 2 `ACTIVE` records
only completed Control establishment, not trust or protection.

```text
Transport -> Framing -> Control Establishment Messages -> runtime lifecycle
```

Framing does not know Control; Connection remains a transport entity; Session
does not know wire messages; Channel Manager remains relationship authority.

## Message Type registry

| ID | Name | Direction |
|---:|---|---|
| `0x0000` | RESERVED / INVALID | none |
| `0x0001` | `CONTROL_OPEN` | client -> accelerator only |
| `0x0002` | `CONTROL_ACCEPT` | accelerator -> client only |

No other ID is assigned. Earlier test use of `0x0001` was syntactic; it now
normatively means `CONTROL_OPEN`. Wrong-direction receipt is a protocol-state
violation, not a framing error.

## Independent version

Control Establishment Protocol version is Major 1, Minor 0.

```text
Envelope Version != Control Protocol Version != software version
```

The baseline accepts exactly 1.0. Major other than 1 or Minor other than 0 is
unsupported and maps to `PAPACC_RESULT_NOT_SUPPORTED`; there is no implicit
minor forward compatibility.

## `CONTROL_OPEN`

Envelope: Message Type `0x0001`, Flags 0, Payload Length exactly 4. Payload:

| Offset | Size | Field | Encoding |
|---:|---:|---|---|
| 0 | 2 | Protocol Major | unsigned U16 big-endian |
| 2 | 2 | Protocol Minor | unsigned U16 big-endian |

Version 1.0 payload is `00 01 00 00`. Normative 20 bytes:

```text
50 41 43 43 01 00 00 10  00 01 00 00 00 00 00 04
00 01 00 00
```

Payload Length 0, 1..3, or 5+ is a message `PROTOCOL_ERROR`, even when its
envelope is valid. A future decoder may accumulate the four bytes in fixed
storage across arbitrary fragmentation; no generic payload allocation follows.

## `CONTROL_ACCEPT`

Envelope: Message Type `0x0002`, Flags 0, Payload Length exactly 4. Payload has
Selected Protocol Major and Minor as U16 big-endian. Normative 1.0 bytes:

```text
50 41 43 43 01 00 00 10  00 02 00 00 00 00 00 04
00 01 00 00
```

The response repeats the explicitly selected version rather than relying on a
local default. In 1.0 requested and selected versions are equal. The same exact
length and version error rules apply.

`CONTROL_ACCEPT` contains no runtime ID, Wire Session ID, token, nonce,
Connection ID, Channel ID, credential, capability, client name/version,
network-egress choice, compression, or encryption option. Envelope Flags stay
zero.

## Server state machine and first-frame rule

Conceptual processor states, distinct from Session states:

```text
UNINITIALIZED
 -> WAITING_CONTROL_OPEN_HEADER
 -> READING_CONTROL_OPEN
 -> CONTROL_COMMIT
 -> WRITING_CONTROL_ACCEPT
 -> ESTABLISHED
terminal: CLOSED / ERROR
```

For a `PENDING` Connection, the first semantically processed frame MUST be
`CONTROL_OPEN`. Any other nonzero type remains framing-valid but violates the
current protocol state and closes the Connection. A second `CONTROL_OPEN` on
the same Connection is likewise invalid and never creates another Session.
Receiving `CONTROL_ACCEPT` at the server, or `CONTROL_OPEN` at the client, is a
direction/state violation.

On `HEADER_READY`, verify type and Payload Length before decoding. No Session
exists at header receipt. Commit is permitted only after `FRAME_COMPLETE`, all
four bytes, and supported 1.0 validation.

## Transactional structural commit

After a complete valid `CONTROL_OPEN`, composition performs atomically:

```text
Session Manager publish -> Session ESTABLISHING
Channel Manager bind CONTROL -> Connection ASSOCIATED + CONTROL BOUND
```

If Session publication or Channel binding/capacity fails, remove any Session
created for the attempt, leave no partial relationship, and close/reject the
Connection. Existing Sessions/Channels remain intact; this is not server-fatal.

Only after both operations succeed may the server begin `CONTROL_ACCEPT` in
that Connection's Framed Writer. Session remains `ESTABLISHING` while any
header/payload bytes remain pending or writes would block. Only Writer
`FRAME_COMPLETE` commits `papacc_session_activate()`:

```text
no Session -> ESTABLISHING -> CONTROL_ACCEPT complete -> ACTIVE
```

In Phase 2, `ACTIVE` means only that currently implemented Control
establishment completed. It never means trusted, authenticated, authorized,
Transport-Secure, or capability-negotiated. Phase 3 may insert security,
authentication, and authorization between Control establishment and activation
without changing the Session runtime model. A client considers Phase 2 Control
established after validating `CONTROL_ACCEPT 1.0` and learns no Session ID.

## Errors, EOF, and timeout

Recommended future result mapping:

- malformed/wrong-length message: `PAPACC_RESULT_PROTOCOL_ERROR`;
- unsupported Control version: `PAPACC_RESULT_NOT_SUPPORTED`;
- local lifecycle misuse: `PAPACC_RESULT_INVALID_STATE`;
- manager capacity: `PAPACC_RESULT_LIMIT_EXCEEDED`.

No `CONTROL_REJECT` or generic Error message exists. Invalid, unsupported, or
uncommittable establishment closes the Connection without a response.

- EOF before/during OPEN: close; create no Session (mid-frame is framing
  `PROTOCOL_ERROR`).
- EOF or fatal Writer failure after CONTROL bind but before ACCEPT completes:
  invoke CONTROL-loss lifecycle, closing Channel, Session, and Connection;
  Session never becomes ACTIVE.
- A failure affects only its Connection; the server continues.

The Protocol Connection Processor owns a PAL-monotonic establishment deadline
starting when the Connection becomes eligible for processing. It covers wait,
OPEN receive/commit, and ACCEPT send through ACTIVE. Expiry closes the
Connection and any already-bound CONTROL/Session, without mandatory response.
No duration is frozen; it remains server policy.

## Identity and future DATA association

`connection_instance_id`, `session_instance_id`, and `channel_instance_id`
remain runtime-only, non-authoritative, and never serialized. Control
establishment needs no remote Session reference. Phase 2.E must design the
reference/association mechanism for a second Connection becoming DATA.
Knowledge of that future reference alone MUST NOT become final authorization;
Phase 3 must add secure authentication/binding. No insecure placeholder or
random-looking unauthenticated token is defined.

## Implementation boundaries

Phase 2.D2 may implement portable OPEN/ACCEPT codecs, processor state machine,
Reader event consumption, Writer response generation, deadline metadata, and
transactional Session/Channel coordination, without Win32 scheduler/executable
integration.

Phase 2.D3 owns accepted-socket nonblocking transition, Win32 combined
listener/Connection `select()`, processor slots, readiness, round-robin,
production integration, and timeout execution under the C4 architecture.

D2 tests must cover encode/decode, fragmented four-byte payload, exact/short/
oversized lengths, unsupported versions, unexpected/duplicate OPEN, Session
publication, CONTROL bind, association, ESTABLISHING while ACCEPT is pending,
golden ACCEPT bytes, partial/blocked writes, activation only at completion,
EOF/failures, manager exhaustion, rollback, deadline lifecycle, and
per-Connection isolation.

Still deferred: DATA protocol, Wire Session identity, authentication,
Transport Security, capabilities, credentials, client metadata, egress,
compression/encryption negotiation, rejection/error messages, and all other
Message Type IDs.

**Checkpoint result: `CONTROL ESTABLISHMENT DESIGN READY`.**
