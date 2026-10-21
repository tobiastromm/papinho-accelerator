# PapinhoPACC C2 operation extension — protocol specification draft

Revision 0.1 / 2026-10-11. **DRAFT: ENCODING APPROVAL PENDING.**
Input: semantic proposal revision1 and proposed revision2 addendum.
MODE_C_OPERATION and documentary revisions are not allocated wire identifiers.

This draft satisfies preparation of the [human specification requirement](../pacc-c2-specification-requirements.md).
It is NOT yet sufficient to implement an interoperable C2 peer. Every TBD below
requires owner resolution; no C2 numeric IDs or byte vectors are definitive.

## 1. Requirement language and authority

MUST denotes an obligation, MUST NOT a prohibition, SHOULD a recommendation
whose departure requires a documented reason, MAY an allowed option.
**ACCEPTED** clauses reference existing accepted contracts. **PROPOSED** C2
clauses become normative only after owner approval. **TBD** blocks approval
and is not an implementation-defined choice. Examples do not allocate IDs.

Authorities: Accelerator ADR-0022 revision1 (placements), ADR-0029 revision1
(Mode C), ADR-0025 (protocol ownership), ADR-0016/0018 (negotiation/registry),
ADR-0012 (snapshot), framing/CONTROL/DATA specifications, family ADR-0013
revision3 (terminal/application distinction), Browser ADR-0007 revision4
and its recorded subsequent decisions. Current public package/API1.1.0,
external contract generation2, Envelope1.0 and CONTROL1.0 remain distinct.
An older overview ending at0x000E does not remove generation2 type0x000F.

## 2. Responsibilities and trust boundaries

**ACCEPTED:** Browser owns hostname, one-resolution endpoint snapshot, local
destination policy, exact numeric site connect and TCP/IP egress. Accelerator
owns operation/service/destination authorization and PST Site TLS/SNI/name/
chain/ALPN verification. PST providers are private; use public package APIs.
PACC transports bytes and codecs, not either consumer's policy or UI.

APPLICATION carries bidirectional ordered plaintext application bytes.
RECORD carries bidirectional opaque Site TLS records between Browser site TCP
and the private Accelerator bridge. Both roles belong to ONE authorized
operation. Accelerator does not parse HTTP; Browser does not interpret Site
TLS records to invent closure/identity. CONTROL/DATA Secure Principal TLS is
different from Site TLS. A SOCKET, pointer, HWND or private runtime ID is not
wire authority. Declared tuple plus verified TLS identity does not attest a
malicious remote client's physical socket. No tunnel or open proxy is defined.

## 3. Vocabulary

| Term | Meaning |
| --- | --- |
| Principal | Secure authenticated internal identity, never inferred from IP/ticket |
| Session | CONTROL-anchored lifecycle and immutable negotiated grant upper bound |
| CONTROL | Ordered control connection; loss cascades dependent resources |
| DATA | Separately attached connection; ticket alone does not grant a C2 role |
| Capability | ID/major grant, necessary but insufficient for operation |
| Operation ID | Nonsecret correlation; separate server-owned authorized reservation |
| Generation | Scoped incarnation/nonreuse guard; wire representation TBD |
| Resolution generation | Browser-local provenance, not remote authority |
| APPLICATION / RECORD | Two distinct role slots; wire enum values TBD |
| Attachment authority | Unpredictable, single-use, role-bound secret |
| Fence | Proposed ordered end-of-prefix fact on its role DATA channel |
| Retained owner | Logically terminal resource with unconfirmed physical destruction |

## 4. Establishment, versioning and compatibility

**ACCEPTED:** Secure TLS1.3/mTLS/papacc/1 precedes PACC classification.
CONTROL_OPEN/ACCEPT select CONTROL1.0; CAPABILITY_NEGOTIATE/RESULT publish an
immutable eligible subset. Ordinary DATA_TICKET/ATTACH/ACCEPT remain unchanged.
Software/API/package/contract generation does not authorize new wire.
Unknown valid capability keys are omitted, never implicitly granted.
Unsupported Envelope/CONTROL versions and malformed/extra grants fail closed.

**PROPOSED:** TLS_OFFLOAD v1 plus a distinct operation key/major before any C2
exchange. Numeric allocation TBD. Two supported and two preference keys fit
the current negotiate payload:4+6*(2+2)=28 bytes. This does NOT prove new
operation messages fit the current post-CONTROL reader's28-byte limit.
The offer omits NETWORK_EGRESS and cannot authorize Mode B on that Session.
Mixed service Session composition needs owner decision R2-01; no third key to
unknown peers, silent replacement, in-place renegotiation or inferred grant.

| Combination | Required behavior |
| --- | --- |
| Old server / new offer | Unknown operation key omitted; no new frames/site connect/PST setup |
| New server / old client | Existing requested grants and old grammar/goldens preserved |
| Both C2 keys granted | Admission eligibility only; current local/server policy still required |
| Missing either key | No C2 side effects |
| Snapshot lacks C2 | No silent expansion/renegotiation |
| Same Principal / different Session | No cross-Session operation binding |
| Legacy | Same grammar; explicit lower-assurance policy, no Secure downgrade |

C2 absence never authorizes placement/profile/plaintext fallback. A separately
authorized fresh Browser routing operation is different from protocol recovery.
New dispatch states and per-message bounds MUST be approved before adoption.

## 5. Existing framing and message registry

**ACCEPTED** Envelope1.0 uses a16-byte header and big-endian multibyte integers.

| Offset | Bytes | Field / validation |
| ---: | ---: | --- |
| 0 | 4 | Magic 50 41 43 43 |
| 4 | 1 | Envelope major1 |
| 5 | 1 | Envelope minor0 |
| 6 | 2 | Header length16 exactly |
| 8 | 2 | Message type; zero invalid; stateful dispatch checks registered context |
| 10 | 2 | Flags0 |
| 12 | 4 | Payload length; checked against per-message bounds before allocation |

Partial/coalesced frames preserve exact bytes. FRAME_COMPLETE can carry final
payload bytes; zero payload completes at header. Mid-frame EOF invalidates
framing. No resynchronization scan. Parser/buffered bytes survive valid handoff.
Byte buffers are not packed/native C structs; checked arithmetic precedes use.

| Existing IDs | Existing payload/semantics |
| --- | --- |
| 0x0001/0x0002 | CONTROL_OPEN/ACCEPT, payload4: U16 major, U16 minor |
| 0x0003/0x0004 | Ticket request0 / ticket16 |
| 0x0005/0x0006 | DATA_ATTACH16 / DATA_ACCEPT0 |
| 0x0007/0x0008 | Capability negotiation/result; canonical bounded sets |
| 0x0009–0x000E | Existing Network Open/Result/Data/Half-close/Close/Abort |
| 0x000F | Negotiated Network Egress v2 Abort Reason; payload2, TLS_TRUNCATED=1 |

C2 MUST NOT reinterpret these grammars, reserved flags or v2 terminal authority.
Framing accepting an unknown nonzero type is not permission to dispatch/skip it.

## 6. C2 exact-message specification worksheet

Names below are semantic descriptions already in the proposal, NOT assigned
message identifiers. Every row requires approved ID, channel/direction/version,
field order/type/width/endianness/lengths, optional presence and validation.

| Semantic exchange | Intended context | Content | Encoding |
| --- | --- | --- | --- |
| Authorize/reserve request | CONTROL | Immutable site tuple and permitted TLS inputs | TBD |
| Reservation result/denial | CONTROL | Correlation, incarnation, role authorities/outcome | TBD |
| Role-bind request | Accepted DATA | Operation/generation/role/secret | TBD |
| Role-bind acknowledgement | Same DATA | Exact claim/outcome; complete-frame barrier | TBD |
| Site-ready declaration | CONTROL | Factual readiness for captured attempt | TBD |
| Verified-TLS result | Owner-selected channel TBD | Verification/ALPN and publication authority | TBD |
| APPLICATION/RECORD traffic | Bound role DATA | Ordered opaque span | TBD |
| Direction end / fence | Same ordered role DATA | Preceding prefix and factual end | TBD |
| Cancel/revocation | CONTROL/loss boundary TBD | Authorized cause/action/generation | TBD |
| Terminal summary | Channel/ordering TBD | First cause, disposition and required fences | TBD |

| Semantic field | Decisions required |
| --- | --- |
| Operation correlation/incarnation | Width, invalid values, scope and nonreuse |
| Session/CONTROL binding | Implicit context vs explicit field; no invented Wire Session ID |
| Operation/stream generation | Width, equality scope, exhaustion/wrap and replay |
| Role | APPLICATION/RECORD wire enum and reserved values |
| Authority | Entropy/provider/construction/length/comparison/expiry |
| Hostname | Original/normalized text, charset/length/canonicalization |
| Numeric endpoint | Family enum/address bytes/port and validation |
| Resolution provenance | Echo/owner rules, not global freshness |
| TLS profile/ALPN | Server-approved references/counts/lengths |
| Fence/prefix | Directional ordering, representation, receipt/overflow |
| Cause/security action | Separate enums, unknown outcome handling |
| Error | Scope, bound, disclosure and retry prohibition |

The immutable server record can bind local-only fields; not every semantic
field must appear on every frame. The owner must label wire-required,
optional-with-presence-rules and local-only fields explicitly. No TLV or
skip-unknown-field scheme is selected. Trailing/duplicate/unknown field rules,
exact/max+1 lengths and VC6/x86 overflow safety are encoding approval gates.

## 7. Normal sequence — PROPOSED semantic steps, not new message names

~~~mermaid
sequenceDiagram
    participant B as Browser
    participant A as Accelerator
    participant P as PST
    participant S as Site
    B->>A: Existing Secure CONTROL and capability negotiation
    B->>B: C0 resolve once; local policy
    B->>A: Proposed authorize/reserve tuple
    A-->>B: Proposed reservation and role authorities
    B->>S: C1 exact numeric TCP connect
    B->>A: Existing DATA ticket/attach/accept for RECORD
    B->>A: Proposed RECORD bind
    A-->>B: Complete proposed bind acknowledgement
    A->>P: Prepare C3 socket attachment (no TLS progress)
    B->>A: Existing second DATA ticket/attach/accept
    B->>A: Proposed APPLICATION bind
    A-->>B: Complete proposed bind acknowledgement
    B->>A: Proposed factual site-ready declaration
    A->>A: Recheck grants and atomically activate
    A->>P: Incremental Site TLS handshake
    P-->>A: TLS records through private bridge
    A-->>B: Proposed ordered RECORD bytes
    B->>S: Opaque records on site socket
    S-->>B: Opaque response records
    B->>A: Proposed ordered RECORD bytes
    A->>P: Bridge input
    P-->>A: Factual verified handshake and ALPN
    A-->>B: Proposed verified-TLS result
    B->>A: Proposed APPLICATION bytes
    A->>P: Public plaintext API write/read
    A-->>B: Proposed APPLICATION bytes
~~~

Repeated bidirectional handshake/I/O turns are abbreviated; no fixed TLS packet
shape is asserted. Construction, ACK enqueue and HTTP success are not activation.

## 8. Error, cancellation and closure sequences — PROPOSED

~~~mermaid
sequenceDiagram
    participant B as Browser
    participant A as Accelerator
    participant P as PST
    alt Cancel or policy revocation
        A->>A: Revoke publication authority; record escaped prefix
        A-->>B: Bounded terminal/action if authorized and publishable
        B->>B: Cancel/wake C1; reject late results
    else RECORD raw EOF/reset or DATA loss
        A->>A: Preserve raw cause; stop operation and sibling
        A->>P: Governed factual transport/shutdown progress
        P-->>A: TLS close/truncation/error fact
    else Authorized normal end
        B->>A: Ordered APPLICATION end/fence
        A->>P: Incremental reciprocal shutdown
        P-->>A: Outbound closing records
        A-->>B: Ordered RECORD records/fence
        B->>B: Account prefix before site direction-end
        P-->>A: Factual shutdown completion
    end
    A->>P: Eligible Model E try_release
    alt OK and DESTROYED
        P-->>A: Destruction confirmed
        A->>A: Governed finalization; no second release
    else PRECONDITION_RETAINED
        P-->>A: Ownership retained
        A->>A: Preserve dependencies; resolve eligibility only
    else CLOSE_UNRESOLVED_RETAINED
        P-->>A: Terminal retained ownership
        A->>A: No I/O/release retry/rebind/eviction/alternate close
    end
~~~

CONTROL loss uses existing Session cascade. Required DATA loss terminates
its operation/sibling, not unrelated operations. Wrong token/guessed ID cannot
terminate a victim. Remote duplicates remain violations; internal late callbacks
are rejected before dereferencing obsolete identities.

## 9. Session, operation and channel state machines

Existing Session structural ACTIVE alone does not establish C2 eligibility.
Negotiated keys/current grant and assurance/admission context are additional
gates. CONTROL loss removes eligibility; reconnect cannot resurrect operation.

~~~mermaid
stateDiagram-v2
    [*] --> RESERVED: Valid bounded admission
    RESERVED --> ROLES_PREPARING: Exact connect and attachment
    ROLES_PREPARING --> ACTIVE_HANDSHAKE: Both acknowledged roles and site ready
    ACTIVE_HANDSHAKE --> ACTIVE_APPLICATION: Factual verified TLS
    ACTIVE_APPLICATION --> SHUTTING_DOWN: Authorized normal end
    SHUTTING_DOWN --> TERMINAL_RECORDED: First valid terminal
    RESERVED --> TERMINAL_RECORDED: Expiry or cancel
    ROLES_PREPARING --> TERMINAL_RECORDED: Loss or revocation
    ACTIVE_HANDSHAKE --> TERMINAL_RECORDED: Failure or deadline
    ACTIVE_APPLICATION --> TERMINAL_RECORDED: Cancel or loss
    TERMINAL_RECORDED --> RELEASED: All destruction confirmed
    TERMINAL_RECORDED --> TERMINAL_RETAINED: Ownership unresolved
    RELEASED --> [*]
~~~

All new operation states are PROPOSED, not encoded enum assignments.
TERMINAL_RETAINED has no transition to active I/O. Governed eligibility
resolution of PRECONDITION_RETAINED is distinct from close-unresolved retention.

| Channel transition | Prerequisite |
| --- | --- |
| Candidate → structural DATA bound | Existing full authenticated/authorized attach commit |
| Bound → DATA_ACCEPT complete | No role processing before existing complete write |
| Accepted unbound → role claim | Negotiated C2, full valid binding and atomic secret consumption |
| Claim → acknowledged | Complete ACK write and corresponding peer validation |
| Acknowledged → active | Both roles/site ready/current grants |
| Active → fenced/terminal | Same-channel prefix order and factual authorized cause |
| Terminal → new operation | Invalid before quiescence/confirmed finalization |

Final transition tables must enumerate every message/direction/state violation,
partial ACK, expiry, loss, revocation and late callback. Queued frames are not
completion; a lost ACK never restores consumed authority.

## 10. Ordering, errors, budgets and Model E

Each of four queues is bounded; high water removes upstream reads and low water
resumes. Charge parser/writer, bridge, CONTROL notifications and retained owners.
ADR-0029 lab-only:2 operations global/1 per Session, four64KiB queues,
high48/low24KiB,512KiB payload aggregate, fragments≤16KiB,16KiB/flow,
8 opportunities/wake. These are not production/kernel-memory limits.
Production/NT4 8/16/32/64KiB measurements remain pending.

Accepted/submitted/peer-received/application-delivered prefixes are different.
A role fence follows its prefix on the same DATA stream. CONTROL cannot prove
DATA receipt. Strict revocation stops new authorized payload; partial frames
cannot be spliced or completed with newly unauthorized DATA. Abandon affected
transport if needed; record escaped prefix/non-clean fact. Final fence receipt/
acknowledgement rules are owner decisions, not inferred cross-socket order.

Deadlines use local monotonic bounded budgets; wake does not extend totals.
Lab attach5s/handshake10s/no-progress30s/shutdown5s/cleanup5s are not production
settings. Timeout is failure, not successful qualification or destructive power.

Raw EOF/reset/half-close is not close_notify. PST alone supplies TLS facts.
Browser independently validates HTTP; explicit TLS_TRUNCATED plus valid complete
CL/chunked may remain usable, never generic abort/CONTROL loss.
Each physical resource has one owner. OK+DESTROYED permits finalization;
retention preserves provider/runtime/transport/lease/WinSock and identity.
No native retry, alternate close, deadline eviction or early reuse.

## 11. Security

Secure DATA requires existing Principal match AND Session/incarnation/role/
operation validation. Role secret is distinct from predictable structural ticket.
Missing entropy denies admission. Entropy/provider/length/construction/comparison
primitive require security-owner approval; no custom crypto is specified.
Recheck epoch/grants at commit, activation, TLS-ready and each payload publication.
Separate hostname/TLS identity from numeric destination and Browser preference.

Legacy plaintext is lower assurance, vulnerable to on-path token/data observation
and tampering. Site TLS offload does not protect the first hop. Lab D9A scope/
isolation/quotas/warning and separately approved experiment gate remain required;
production D9B/C9-C11 remains unapproved. No Secure-to-Legacy fallback.
Bound invalid-bind work/responses and retained reservations; guessed correlation
must not become victim teardown or an unbounded existence/resource oracle.

## 12. Examples and conformance

**Approved existing CONTROL_OPEN1.0 only, NOT C2:**

~~~text
50 41 43 43 01 00 00 10 00 01 00 00 00 00 00 04 00 01 00 00
~~~

Decoded: Envelope1.0/header16/type1/flags0/payload4, CONTROL1.0 U16/U16.

**Approved existing DATA_ACCEPT only, NOT a role-bind acknowledgement:**

~~~text
50 41 43 43 01 00 00 10 00 06 00 00 00 00 00 00
~~~

C2 hex vectors are NOT POPULATED until encoding approval. Each exchange then
requires exact minimal/maximal/fragmented/coalesced positive and malformed
length/version/flags/role/generation/token/state/trailing-field negatives.
Original P01-P06/N01-N24 and revision2 R2-T01-T10 are planned, not executed.
[Review and test mapping](../capabilities/pacc-c2-joint-review-20261011.md).

Qualification requires the same approved specification/codec/test revision,
old/new both directions, portable C89/VC6 checks and independent-peer
implementation/read-through. Separate synthetic/native/current-host/NT4 evidence.
C1 PASS and Model E do not qualify C2 or authorize C7 E1-E12.

## 13. Pending decisions and maintenance

Session composition; service/major/IDs; new grammar and state bounds; token
security; generation/nonreuse; hostname/address/ALPN/optional-field encoding;
ACK/TLS-ready completion; terminal fences/causes/actions; resource accounting;
API/package metadata all require owner approval.
Maintain exact layouts, sequences, state tables and vectors with canonical
public codecs/tests and accepted contracts. An incomplete draft cannot claim
independent interoperability. Production PST pin0.6.4 unchanged; no promotion.
