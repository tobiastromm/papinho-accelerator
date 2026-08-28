# PapinhoAccelerator Protocol Framing

## Status and scope

This document normatively freezes wire Envelope 1.0. It defines framing only:
no message family or numeric assignment, payload schema, Wire Session/Channel
ID, authentication, capability negotiation, or Transport Security mechanism.
Phase 2.C2 implements the portable semantic header model, exact 16-byte
encoder/decoder, and allocation-free incremental parser. Transport I/O and
higher-layer production integration remain deliberately absent. Phase 2.C3B1
adds a portable receive-only Framed Reader that joins abstract transport reads
to the parser. Phase 2.C3B2 adds its portable outbound sibling, the Framed
Writer. Production Connection protocol processing remains unintegrated.

## Ordered byte-stream contract

The input is an ordered byte stream. TCP satisfies this contract, but TCP and
IP endpoints are not framing requirements. A future transport can reuse the
framing only with equivalent stream semantics or an adapter.

```text
one recv() != one frame
one send() != one frame guaranteed
```

The implementation MUST support partial headers and payloads, multiple frames
per input span, arbitrary fragmentation, zero-length payloads, EOF at every
boundary, bounded lengths, and overflow-safe arithmetic.

## Envelope 1.0 header

The base header is exactly 16 bytes:

| Offset | Size | Field | Representation |
|---:|---:|---|---|
| 0 | 4 | Magic | `50 41 43 43` (`PACC`) |
| 4 | 1 | Envelope Major | unsigned byte, 1 |
| 5 | 1 | Envelope Minor | unsigned byte, 0 |
| 6 | 2 | Header Length | U16 big-endian, 16 |
| 8 | 2 | Message Type | U16 big-endian |
| 10 | 2 | Flags | U16 big-endian, 0 |
| 12 | 4 | Payload Length | U32 big-endian |

```text
Byte 0   P (0x50)
Byte 1   A (0x41)
Byte 2   C (0x43)
Byte 3   C (0x43)
Byte 4   envelope_major
Byte 5   envelope_minor
Byte 6   header_length MSB
Byte 7   header_length LSB
Byte 8   message_type MSB
Byte 9   message_type LSB
Byte 10  flags MSB
Byte 11  flags LSB
Byte 12  payload_length byte 3 (most significant)
Byte 13  payload_length byte 2
Byte 14  payload_length byte 1
Byte 15  payload_length byte 0 (least significant)
```

All multi-byte wire integers are big-endian/network byte order. Portable code
SHOULD encode/decode bytes directly and MUST NOT require WinSock conversions.

### Magic and resynchronization

Magic is exactly ASCII `PACC`. Any other bytes are malformed/non-PACC framing.
Envelope 1.0 does not scan for a later magic value: loss of framing makes that
stream protocol-invalid.

### Envelope version

```text
Envelope Version != software version
Envelope Version != application protocol/capability version
```

Major 1, Minor 0 versions only the common envelope. Incompatible framing needs
a new Major. Unknown Major MUST NOT be interpreted as 1.0 and should produce
`NOT_SUPPORTED`. Minor is reserved for explicitly defined compatible changes;
1.0 accepts only Minor 0. Unknown Minor is also `NOT_SUPPORTED`, with no
automatic forward compatibility.

### Header Length

Header Length is U16 big-endian and MUST equal 16 for Envelope 1.0. Values
below or above 16 should produce `PROTOCOL_ERROR`. The field reserves explicit
future evolution, but 1.0 does not skip extension headers.

### Message Type

Message Type is U16 big-endian, range 0..65535. Zero is RESERVED/INVALID and
should produce `PROTOCOL_ERROR`. No nonzero type is assigned here. An unknown
nonzero type is syntactically valid; only a future dispatcher decides whether
to ignore, reject, answer, or close a higher-level entity.

### Flags

Flags is U16 big-endian. Every bit is reserved in 1.0, therefore Flags MUST be
zero. Nonzero flags should produce `PROTOCOL_ERROR`. No compression,
encryption, security, or other semantic flag is defined.

### Payload Length and limits

Payload Length is the U32 big-endian number of bytes after the header and does
not include the 16 header bytes. Zero is valid; per-message payload validity is
a future message-layer concern.

The field represents 0..`0xFFFFFFFF`, but this is wire capacity, not a product
or allocation limit. The future parser MUST accept a caller/policy maximum and
reject a declared length above it with `LIMIT_EXCEEDED` before allocation,
combined-size arithmetic, or accumulation. No official limit is selected.

## Incremental parser contract

The parser MUST NOT require allocating Payload Length bytes. It owns only small
state; callers own input/output storage. Preferred flow:

```text
input spans -> header metadata -> payload chunks -> frame complete
```

The implementation exposes `PAPACC_FRAME_PARSER_STATE_READING_HEADER`,
`READING_PAYLOAD`, and terminal `ERROR`. Each feed reports its exact byte count
and at most one event: `NONE`, `HEADER_READY`, `PAYLOAD_CHUNK`, or
`FRAME_COMPLETE`. The caller can feed the unconsumed suffix again without a
copy.

`HEADER_READY` carries validated semantic header metadata. Payload events point
directly into the caller-owned input span and the pointer remains valid only as
long as that input storage does. `FRAME_COMPLETE` may carry the final nonempty
payload chunk; callers must process that chunk as part of the frame. A
zero-length frame produces `FRAME_COMPLETE` immediately after its header.
After completion the parser is already ready for the next header.

- Header and payload may arrive one byte at a time or in any chunk sizes.
- Payload length 1000 split as 7 + 200 + 793 is equivalent to one span.
- One span may contain `[frame A][frame B][partial frame C]`.
- Zero payload completes immediately after the valid header.
- After completion, parsing returns to `READING_HEADER` for the next byte.

EOF exactly between frames is clean framing EOF; the upper layer decides the
lifecycle response. EOF mid-header or before the declared payload completes is
a truncated frame and protocol error. Malformed/truncated input puts the parser
in terminal error for that stream. It never searches for another `PACC`; reset
is meaningful only after the caller abandons the affected connection.

`papacc_frame_parser_finish()` represents EOF. `papacc_frame_parser_reset()`
clears partial/error state while preserving the caller's payload limit. The
parser retains no payload pointer and performs no heap allocation.

### Representation and arithmetic safety

Parser and encoder MUST process fields byte-by-byte. They MUST NOT cast a byte
buffer to a header struct or depend on alignment, host endianness, padding,
`#pragma pack`, `__attribute__((packed))`, or equivalent. Wire format is bytes,
not a C ABI.

Calculations such as `header_length + payload_length` MUST use sufficient width
and explicit overflow checks. Allocation/accumulation can occur only after
field validation, caller-limit validation, and safe combined arithmetic.

## Implemented C2 error taxonomy

Phase 2.C2 adds `PAPACC_RESULT_PROTOCOL_ERROR` to the portable result enum.

| Condition | Recommended result |
|---|---|
| Local API misuse | `INVALID_ARGUMENT` / `INVALID_STATE` |
| Unsupported Major or Minor | `NOT_SUPPORTED` |
| Payload above caller limit | `LIMIT_EXCEEDED` |
| Bad magic/length/flags/type zero/truncation | `PROTOCOL_ERROR` |
| Unknown nonzero Message Type | valid framing; defer policy |
| Impossible internal condition | `INTERNAL_ERROR` |

## Encoder, parser, and backpressure boundaries

The C2 encoder receives semantic fields and writes exactly 16 bytes: fixed
magic/version/header length, big-endian type/flags/payload length. It rejects
type zero and unsupported flags, is host-endianness independent, and uses no
packed struct.

```text
frame encoder != transport writer
frame parser  != transport reader
```

Encoder does not call `send()`/`WSASend()`; parser does not call
`recv()`/`WSARecv()`. Framing does not assume one frame equals one successful
write. The Transport Connection can now represent partial byte-stream I/O, but
only the standalone Framed Reader is wired to framing for caller-driven reads.
Production Connections do not own or run a Reader. Future outbound integration
owns pending bytes, backpressure, cancellation, and bounded queues.

The Framed Reader owns parser/cursor state but not its Transport Connection or
caller-provided scratch buffer. It performs at most one transport read and
publishes at most one framing event per call. Buffered bytes are processed
before another read. Payload event pointers refer into the scratch buffer and
remain valid until the next Reader `next`, reset, or shutdown operation.
`next` may block when it needs a blocking transport read.

The Framed Writer stores one encoded 16-byte header and tracks at most one
in-flight frame. It streams caller-provided payload spans through at most one
transport write per step, retains no payload pointer, and reports partial
progress, payload demand, would-block, frame completion, and end-of-stream
explicitly. A step may block on the current blocking transport. There is no
write-all loop, outbound queue, scheduling, or backpressure policy.

Writer reset/shutdown discards only local state. If any header or payload byte
has already reached the stream, it cannot undo or resynchronize those bytes;
normal production recovery is to abandon the affected transport.

Production scheduling is frozen separately in
[Connection I/O Scheduling](connection-io-scheduling.md): a portable processor
will own Reader/Writer state while a Win32 readiness loop schedules bounded
turns. This is architectural only; no production Connection currently invokes
Reader or Writer.

## Deliberately absent fields

The base header has no runtime or wire Session ID, Channel ID, Connection ID,
sequence number, request/job correlation ID, checksum/CRC, compression flag,
or encryption/security flag.

- Runtime IDs are never wire IDs; initial Connections may be unassociated.
- Establishment/association metadata belongs to specific future messages.
- TCP supplies ordered reliable stream semantics; operation correlation, if
  needed, belongs to message/job semantics.
- CRC does not provide security, TCP detects accidental transit corruption,
  and future Transport Security supplies authenticated integrity when required.
- Compression/transcoding belongs to capability/message semantics.
- Encryption belongs below framing and is not a frame flag.

## Transport Security boundary

```text
Transport Security != TLS_OFFLOAD

Transport
 -> Transport Security (optional/required by future policy)
 -> Framing
 -> Messages
 -> Channel / Session logic
```

Framing sees authenticated plaintext when security is active. No TLS mechanism
or library is selected, and capability negotiation cannot weaken required
Transport Security.

## Normative golden examples

Three-byte payload; type `0x1234` is illustrative only and NOT assigned:

```text
50 41 43 43  01 00  00 10  12 34  00 00  00 00 00 03
AA BB CC
```

Zero-byte payload; type `0x0001` is illustrative only and NOT assigned:

```text
50 41 43 43  01 00  00 10  00 01  00 00  00 00 00 00
```

## Required C2 test matrices

Parser:

- whole header; header one byte per feed;
- whole and byte-by-byte payload; zero payload;
- two frames in one feed; frame plus partial next frame;
- wrong magic; unsupported Major and Minor;
- Header Length below and above 16; type zero; nonzero flags;
- payload exactly at limit and one byte above;
- EOF between frames, mid-header, and mid-payload;
- reset and terminality after error.

Encoder:

- exact 16 bytes, magic, and version;
- big-endian header length, type, flags, and payload length;
- zero payload and maximum U32 representation;
- rejection of type zero and unsupported flags;
- deterministic output independent of host endianness.

Tests use byte arrays and require no sockets.

## Compatibility and rationale

This design remains suitable for Windows NT4/2000/XP, C99, low-memory clients,
and machines without 64-bit CPUs. It requires no unaligned native load, SIMD,
modern Windows API, or packing extension.

The 16-byte header is small, explicitly versioned and length-delimited, simple
to parse in portable C, wide enough for U32 payload lengths, and free of
higher-layer metadata. Conceptual alignment is not a native ABI.

## Explicitly deferred

- actual Message Type assignments and a number registry;
- CONTROL establishment and DATA association payloads;
- Wire Session ID and Wire Channel ID, if ever needed;
- authentication, capability, error, and heartbeat messages;
- job IDs and request/response correlation;
- Transport Security handshake, mechanism, and library;
- official payload/queue limits.

Names such as HELLO, CONTROL, DATA, AUTH, ERROR, PING, and CAPABILITIES are not
assigned messages. The first real message family will create its own registry.
