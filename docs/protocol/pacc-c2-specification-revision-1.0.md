# PACC C2 human-readable specification - normative revision 1.0
Date: 2026-10-12. Semantic revision 3; exact encoding 1. **ACCEPTED CONTRACT; NOT IMPLEMENTED OR RUNTIME QUALIFIED.**

Authority: [PapinhoAccelerator/ADR-0034](../adr/ADR-0034-pacc-c2-revision3-contract-and-separated-gates.md). This is a new normative edition derived from preserved draft0.3 and supplements0.3.2–0.3.4. Historical drafts/candidates remain history, not concurrent approval blockers. Contract approval, implementation authorization, implementation qualification and production enablement are four independent states. No implementation or experiment is authorized here.

MUST, MUST NOT, SHOULD and MAY mean required, prohibited, recommended with justified exceptions and optional respectively. Normative clauses are independent of examples. Examples are public illustrative fixtures, never live tokens or runtime evidence. Clause11 below refines closure/resource/security interpretation and takes precedence over earlier generic phase diagrams; it changes no message ID, byte layout or enum.

## 1. Scope, roles and authority
Browser owns original hostname, one-resolution immutable tuple, local all-candidate destination policy, exact site socket and site-facing TCP/IP. Accelerator owns Site TLS authorization, SNI/name/chain verification, PST plaintext and private IPv4 loopback bridge. PACC carries opaque APPLICATION plaintext and RECORD TLS records belonging to TWO_ROLES_ONE_OPERATION; it implements neither HTTP nor policy. PST remains package-only and unmodified. Secure CONTROL/DATA TLS is independent from Site TLS.

A Session is its live CONTROL incarnation and immutable negotiated grants, not a number supplied by a client. Principal is authenticated internal identity, never IP/ticket/role token. No pointer, handle or private runtime identity crosses wire. Server records bind Session, profile, Principal when Secure, tuple, TLS inputs, policy epoch, request/operation/generation, two slots, expiry and resource reservations. Browser snapshot provenance is equality within an attempt, not server-global DNS freshness or socket attestation.

## 2. Versions and existing negotiation
Envelope1.0/header16/flags0 and CONTROL1.0 are unchanged; public package1.1.0 and contract generation2 are separate metadata, not authorization. Assigned service MODE_C_OPERATION has capability ID3/U32 and major1/U16. TLS_OFFLOAD remains ID2/major1. No existing ID is reassigned. Old message types1..15 remain byte-exact, including type15 NETWORK_ABORT_REASON.

Dedicated C2 Session offers sorted keys (2,1),(3,1) in supported and preference sets, counts2/2: existing type7 payload28. Existing type8 result is count U16 plus six bytes/key, so two grants are payload14, NOT16. Only both granted keys authorize new grammar. Server must finish negotiation-result write; Browser must fully validate result before C2. Unknown valid keys are omitted by an old server. Without either key, do not send C2, dial site, create roles or set up PST. No silent fallback, third offer key, renegotiation or capability inference from API version.

A separately authorized existing-service Session retains Mode B grants/grammar under ONE integration owner/shared-runtime serialization, at most one of each per selected endpoint/profile. No Session migration, Secure/Legacy mixing or duplicate runtimes. C2 loss cascades only its dependants; owner-wide Stop governs both. Profile is selected before transport; Secure uses TLS1.3/mTLS/ALPN papacc/1, Legacy explicitly plaintext and lower assurance.

New major requires separate negotiation. Minor documentary edits do not change service major. Any future incompatible layout needs an approved new major; no optional TLV extension is defined. Public C API/package revision needs a separate owner decision before shipping codecs; no version bump is performed here.

## 3. Envelope, byte rules and validation
Header offsets: 0 magic 50 41 43 43 (4); 4 major1 (1); 5 minor0 (1); 6 header-length16 (2); 8 type (2); 10 flags0 (2); 12 payload-length (4). All multi-byte integers are unsigned big-endian. Byte arrays are literal octets; reserved fields MUST be zero. All fields listed are mandatory; only explicitly length-delimited tails may be absent.

U64 denotes eight wire bytes, not a required compiler native integer. Portable C89/VC6 implementations use two U32 limbs or checked byte operations, never packed structures, host long, casts of unaligned buffers or mandatory codec heap. Addition/multiplication/count/offset must be checked before allocation, indexing or advancing. Counter wrap at 2^64-1 fails closed. U32 generations/epochs in Browser snapshots do not wrap within a live attempt; allocate fresh only after governed quiescence.

Before accumulating payload, check negotiated service, type, channel, sender, state and exact/max length. Unknown types, enums, bits, extra/trailing bytes, length mismatch, nonzero reserved or illegal state are violations; no skip-unknown/resynchronization. Mid-frame EOF is non-clean; fragmented/coalesced frames preserve bytes, including final payload returned with FRAME_COMPLETE. An old framed unknown nonzero type is not dispatch authority.

The following tables define offsets relative to payload start. Fixed prefix is followed by exactly the stated tail. Variable-length equation is checked in widened/checked arithmetic; no implicit padding.

### C2_OPEN — assigned 0x0010

CONTROL; sender B>A; payload 42..803 bytes. Tail: original[1..254], canonical[1..253], ALPN vector[0..256].

| Offset | Bytes | Field | Encoding |
| ---: | ---: | --- | --- |
| 0 | 8 | request_id | U64BE |
| 8 | 4 | attempt_generation | U32BE |
| 12 | 4 | resolution_generation | U32BE |
| 16 | 1 | family | U8BE |
| 17 | 1 | name_kind | U8BE |
| 18 | 2 | port | U16BE |
| 20 | 4 | ipv4 | octets |
| 24 | 2 | original_length | U16BE |
| 26 | 2 | canonical_length | U16BE |
| 28 | 4 | tls_profile_ref | U32BE |
| 32 | 1 | require_alpn | U8BE |
| 33 | 1 | alpn_count | U8BE |
| 34 | 2 | alpn_bytes | U16BE |
| 36 | 4 | reserved | octets |

### C2_OPEN_RESULT — assigned 0x0011

CONTROL; sender A>B; payload 108..108 bytes. Tail: none.

| Offset | Bytes | Field | Encoding |
| ---: | ---: | --- | --- |
| 0 | 8 | request_id | U64BE |
| 8 | 8 | operation_id | U64BE |
| 16 | 8 | generation | U64BE |
| 24 | 8 | policy_epoch | U64BE |
| 32 | 4 | attempt_generation | U32BE |
| 36 | 4 | resolution_generation | U32BE |
| 40 | 2 | status | U16BE |
| 42 | 1 | assurance | U8BE |
| 43 | 1 | reserved | octets |
| 44 | 32 | application_token | octets |
| 76 | 32 | record_token | octets |

### C2_BIND — assigned 0x0012

DATA accepted/unbound; sender B>A; payload 68..68 bytes. Tail: none.

| Offset | Bytes | Field | Encoding |
| ---: | ---: | --- | --- |
| 0 | 8 | operation_id | U64BE |
| 8 | 8 | generation | U64BE |
| 16 | 8 | policy_epoch | U64BE |
| 24 | 4 | attempt_generation | U32BE |
| 28 | 4 | resolution_generation | U32BE |
| 32 | 1 | role | U8BE |
| 33 | 3 | reserved | octets |
| 36 | 32 | token | octets |

### C2_BIND_ACK — assigned 0x0013

role DATA; sender A>B; payload 28..28 bytes. Tail: none.

| Offset | Bytes | Field | Encoding |
| ---: | ---: | --- | --- |
| 0 | 8 | operation_id | U64BE |
| 8 | 8 | generation | U64BE |
| 16 | 8 | policy_epoch | U64BE |
| 24 | 1 | role | U8BE |
| 25 | 1 | status | U8BE |
| 26 | 2 | reserved | octets |

### C2_SITE_READY — assigned 0x0014

CONTROL; sender B>A; payload 32..32 bytes. Tail: none.

| Offset | Bytes | Field | Encoding |
| ---: | ---: | --- | --- |
| 0 | 8 | operation_id | U64BE |
| 8 | 8 | generation | U64BE |
| 16 | 8 | policy_epoch | U64BE |
| 24 | 4 | attempt_generation | U32BE |
| 28 | 4 | resolution_generation | U32BE |

### C2_TLS_READY — assigned 0x0015

CONTROL; sender A>B; payload 32..287 bytes. Tail: selected ALPN[0..255].

| Offset | Bytes | Field | Encoding |
| ---: | ---: | --- | --- |
| 0 | 8 | operation_id | U64BE |
| 8 | 8 | generation | U64BE |
| 16 | 8 | policy_epoch | U64BE |
| 24 | 2 | tls_version | U16BE |
| 26 | 2 | verification_mask | U16BE |
| 28 | 2 | alpn_length | U16BE |
| 30 | 2 | reserved | octets |

### C2_DATA — assigned 0x0016

role DATA; sender both; payload 29..16384 bytes. Tail: opaque payload[1..16356].

| Offset | Bytes | Field | Encoding |
| ---: | ---: | --- | --- |
| 0 | 8 | operation_id | U64BE |
| 8 | 8 | generation | U64BE |
| 16 | 1 | role | U8BE |
| 17 | 1 | direction | U8BE |
| 18 | 2 | reserved | octets |
| 20 | 8 | offset | U64BE |

### C2_FENCE — assigned 0x0017

role DATA; sender both; payload 32..32 bytes. Tail: none.

| Offset | Bytes | Field | Encoding |
| ---: | ---: | --- | --- |
| 0 | 8 | operation_id | U64BE |
| 8 | 8 | generation | U64BE |
| 16 | 1 | role | U8BE |
| 17 | 1 | direction | U8BE |
| 18 | 2 | reserved | octets |
| 20 | 8 | prefix_count | U64BE |
| 28 | 2 | kind | U16BE |
| 30 | 2 | reserved_tail | octets |

### C2_FENCE_ACK — assigned 0x0018

role DATA; sender both (ack opposite direction); payload 32..32 bytes. Tail: none.

| Offset | Bytes | Field | Encoding |
| ---: | ---: | --- | --- |
| 0 | 8 | operation_id | U64BE |
| 8 | 8 | generation | U64BE |
| 16 | 1 | role | U8BE |
| 17 | 1 | direction | U8BE |
| 18 | 2 | reserved | octets |
| 20 | 8 | prefix_count | U64BE |
| 28 | 2 | status | U16BE |
| 30 | 2 | reserved_tail | octets |

### C2_RECORD_EVENT — assigned 0x0019

RECORD DATA; sender B>A; payload 32..32 bytes. Tail: none.

| Offset | Bytes | Field | Encoding |
| ---: | ---: | --- | --- |
| 0 | 8 | operation_id | U64BE |
| 8 | 8 | generation | U64BE |
| 16 | 1 | role | U8BE |
| 17 | 1 | direction | U8BE |
| 18 | 2 | reserved | octets |
| 20 | 8 | prefix_count | U64BE |
| 28 | 2 | event | U16BE |
| 30 | 2 | reserved_tail | octets |

### C2_CANCEL — assigned 0x001a

CONTROL; sender both; payload 40..40 bytes. Tail: none.

| Offset | Bytes | Field | Encoding |
| ---: | ---: | --- | --- |
| 0 | 2 | target_kind | U16BE |
| 2 | 2 | fact | U16BE |
| 4 | 2 | action | U16BE |
| 6 | 2 | reserved | octets |
| 8 | 8 | request_id | U64BE |
| 16 | 8 | operation_id | U64BE |
| 24 | 8 | generation | U64BE |
| 32 | 8 | policy_epoch | U64BE |

### C2_FINAL — assigned 0x001b

CONTROL; sender A>B; payload 76..76 bytes. Tail: none.

| Offset | Bytes | Field | Encoding |
| ---: | ---: | --- | --- |
| 0 | 8 | request_id | U64BE |
| 8 | 8 | operation_id | U64BE |
| 16 | 8 | generation | U64BE |
| 24 | 8 | policy_epoch | U64BE |
| 32 | 2 | fact | U16BE |
| 34 | 2 | action | U16BE |
| 36 | 2 | outcome | U16BE |
| 38 | 2 | tls_fact | U16BE |
| 40 | 2 | ownership | U16BE |
| 42 | 1 | fence_mask | U8BE |
| 43 | 1 | assurance | U8BE |
| 44 | 8 | app_ba_count | U64BE |
| 52 | 8 | app_ab_count | U64BE |
| 60 | 8 | record_ba_count | U64BE |
| 68 | 8 | record_ab_count | U64BE |


## 4. Identity and field rules
Request IDs are nonzero monotonically increasing U64 per CONTROL incarnation; exhaustion ends admission. Each request may open once; server remembers highest accepted request ID and live/retained pending records. Replays or reordered request IDs cannot extend deadlines. Operation IDs are server-minted protocol-only monotonically increasing U64 per server lifetime, never private manager IDs. Generation is a nonzero server-issued U64 reservation counter scoped to Session. No wrap, reuse or reconnect resurrection. Restart destroys prior Session eligibility; tokens and old channels are invalid. New operation IDs/generations are denied when retained/callback ownership prevents safe capacity reuse.

Attempt_generation and resolution_generation are nonzero U32 Browser-local captured provenance, echoed exactly. Policy_epoch is nonzero U64 assigned from server current policy; epochs never wrap while references exist. Only server advances epoch. Client cannot grant policy by declaring it; every operation-bound use compares immutable epoch plus live policy locally. A server revocation CANCEL carries captured operation epoch, not a fabricated replacement grant.

Role1=APPLICATION,2=RECORD; direction0=Browserâ†’Accelerator (BA),1=Acceleratorâ†’Browser (AB). Other values invalid. DATA sender must match direction. ACK refers to opposite physical sender's fenced direction. Each role channel has exactly one immutable operation/role binding; metadata on every role frame must match it.

C2_OPEN fixed-prefix40; total=40+original_length+canonical_length+alpn_bytes, 42..803. Family1=IPv4, name_kind1=DNS, port1..65535; other family/kind unsupported. Address four octets in network order, never text/native SOCKET. This first major supports DNS identities plus IPv4 selected endpoints only; IPv6, numeric TLS identities and IDNA require separately approved grammar/qualification, not a silent downgrade.

Original hostname is ASCII DNS text, 1..254 octets, no NUL/space/control/Unicode; optional ONE trailing dot, case preserved. Canonical is lowercase original with that one terminal dot removed, 1..253 bytes; labels1..63, letters/digits/hyphen, no leading/trailing hyphen, empty label, wildcard or numeric dotted address. Server computes canonical and requires exact match; original normalized identity remains SNI/expected-name input, never selected IP. Preserve original locally for UI/audit. No second DNS lookup or claimed all-candidate policy attestation at server; Browser must complete its own accepted policy before opening.

tls_profile_ref is U32 server-approved opaque catalog reference, zero means server-approved default if present; nonzero must be explicitly authorized. It is not runtime ID, arbitrary CA, privileged interface or trust bytes. Inbound client CA and outbound Site trust stay independent. require_alpn is0/1; count0..8; alpn_bytes is exact vector length0..256. Vector has count entries [U8 length1..255, that many opaque octets], no duplicates; order is preference order. count0 iff bytes0, require1 requires nonempty vector. No implicit NUL/string encoding. Server profile constrains offered protocols; accepted selection must match an offer, with empty result permitted only when require0 and profile permits no ALPN.

OPEN_RESULT always108. Status0=GRANTED;1=DENIED;2=UNSUPPORTED_INPUT;3=CAPACITY;4=EXPIRED;5=ENTROPY_FAILURE. Errors disclose no grant/Principal/token/secret details. On grant op/gen/epoch nonzero, echoed provenance equal request and two independent role tokens present. On denial op/gen/epoch and both token arrays zero; request/provenance echo; assurance still matches transport. Malformed OPEN is CONTROL violation, not an admission error disguised as success.

Assurance1=SECURE_PRINCIPAL,2=LEGACY_ENDPOINT, matching configured listener only, never negotiated client preference. Token is32 octets from approved server OS/provider CSPRNG (256-bit unpredictability), independently generated per role; all-zero or live duplicate rejected, at most4 generation attempts then deny. No custom PRNG, counter, ticket, hash-of-ID or secret log. Constant-time vetted comparison is required; public mismatch checks precede comparison. Browser stores secret only for reservation lifetime and erases when no borrower remains. Entropy implementation/provider must be reviewed before code; no PST API extension is required or authorized.

BIND68 matches op/gen/epoch/provenance/role and that role's token after existing DATA_ATTACHâ†’DATA_ACCEPT complete barrier. Validate Secure Principal equality and exact Session first, then current policy/expiry/empty slot, full fields and token. Commit consumes token atomically once, including ACK failure. Wrong token/stale/foreign candidate gets governed candidate close without an existence-distinguishing response or victim cleanup; valid token cannot be consumed on wrong Principal/Session. Authorized committed-channel violations may stop operation. ACK28 status0 only means role claim; no error ACK restores secret. Browser must receive both full ACKs; server must complete both writes before activation.

SITE_READY32 is emitted only after exact site connect and both role ACK receipts, matches captured tuple provenance. It is not proof of a malicious client's physical destination. Server activates only with both claimed roles, complete ACK writes, ready and current policy. PST construction/bridge preparation is not TLS progress authority.

TLS_READY=32+alpn_length (0..255). tls_version=0x0304; verification_mask=0x0007 exactly means observed TLS1.3, valid chain and expected DNS identity, each fact from governed public PST/verification boundary, not guessed from ACK/construction. Reserved0; selected ALPN validates request/profile. No failed TLS_READY; failures go to terminal lifecycle. Accelerator allows APPLICATION only after verified TLS and full TLS_READY write; Browser only after full matching receipt. No plaintext accepted early. If a required fact is not observable from public SDK, implementation is blocked, not fabricated.

## 5. Streaming, prefixes, EOF and fences
DATA prefix28 then1..16356 opaque bytes; totalâ‰¤16384 INCLUDING metadata. offset is exact count of preceding payload in this operation/role/direction, starting0. Receiver checks current authority and offset before delivery, advances only payload counts, never headers/fences; gaps/overlap/wrap are errors. RECORD is allowed in ACTIVE_HANDSHAKE and later factual TLS progress; APPLICATION only after TLS_READY barriers. Both remain forbidden after local revoke or their direction seal.

FENCE32 kind1=END_PREFIX only, reserved_tail0. Sender seals exact admitted/submitted payload count on SAME ordered role stream. No subsequent DATA in that direction; reverse payload and necessary role-control frames remain eligible. Receiver waits for exact downstream accounting before ACK32 status0/reserved_tail0 with same role/direction/count. Missing/partial/duplicate/conflicting ACK or fence is not success.

Downstream boundaries: RECORD BA = Accelerator private C3 native send accepted; RECORD AB = Browser C1 native site send accepted; APPLICATION BA = public PST plaintext write accepted; APPLICATION AB = Browser authorized owned consumer storage. ACK does not attest remote TCP receipt, PST internal consumption, HTTP completeness or UI commit. Track received, downstream-accepted, submitted and escaped counts separately locally. Four fence bits: bit0 APP BA, bit1 APP AB, bit2 RECORD BA, bit3 RECORD AB; upper bits zero. Final counts are sender's admitted payload totals; clean requires exact matching complete ACKs for all four, mask0x0F.

RECORD_EVENT32 sent only by Browser on RECORD: event1=PEER_EOF,2=PEER_RESET,3=SOCKET_FAILURE with direction BA;4=LOCAL_SEND_ENDED with direction AB. The BA count must equal prior BA payload prefix on same stream. AB count must match the acknowledged outbound prefix; SEND_ENDED requires actual successful governed C1 send-direction end after prefix, never speculation. At most one end/error fact per native direction; native error diagnostics stay local, not raw provider codes on wire. This event records site socket facts, not TLS close_notify. It may precede a BA FENCE for the same final prefix; raw reset/failure permits only terminal-authorized fact handling and cannot restore I/O. Such facts may trigger PST failure but do not force clean fences or fabricate EOF.

Clean closure requires factual PST reciprocal completion, no pending authorized output beyond seal, all four validated fence ACKs, and confirmed operation-owned destruction/borrowed-lease finalization. TLS COMPLETE alone is a subfact. If public PST can still produce closing records, RECORD must not seal early. If bounded drain/seal cannot complete, fail non-clean. Partial native/PST writes remain pending, no arbitrary sleeps or synthetic counters.

## 6. Cancellation, terminal and ownership
CANCEL40 target_kind1=REQUEST,2=OPERATION. REQUEST uses nonzero request_id and zero op/gen/epoch; Browser may cancel an outstanding accepted request before result. Server maps only its Session record; cancellation after result write still revokes mapped operation. OPERATION uses request_id0 and exact nonzero op/gen/captured epoch; both owners may send. Unknown/replayed targets cannot terminate another operation or create a reply loop.

Fact U16:0=NONE (FINAL clean only),1=LOCAL_CANCEL,2=POLICY_REVOKED,3=DEADLINE,4=CONTROL_LOST,5=DATA_LOST,6=SITE_EOF,7=SITE_RESET,8=SOCKET_FAILURE,9=TLS_TRUNCATED,10=TLS_FAILURE,11=PROTOCOL_VIOLATION,12=ATTACH_FAILURE,13=CLOSE_UNRESOLVED,14=ADMISSION_DENIED. Action U16:1=ABORT_GOVERNED,2=RETAIN_GOVERNED,3=FINALIZE_CLEAN. CANCEL allows facts1..3 and action1 only; server POLICY_REVOKED authoritative, Browser cannot grant/revoke server policy (it may cancel locally). Other causes are local lifecycle facts/FINAL, no arbitrary remote declarations of PST success. Repeated local Stop coalesces intent; remote duplicate CANCEL is a protocol violation, never double teardown.

FINAL76 is first valid operation terminal summary on still-live CONTROL after governed cleanup facts. Request/op/gen/epoch match granted record. Outcome1=CLEAN,2=NON_CLEAN,3=RETAINED. tls_fact0=UNKNOWN,1=VERIFIED,2=RECIPROCAL_COMPLETE,3=TRUNCATED,4=FAILED. ownership0=NO_OPERATION_RESOURCE,1=DESTROYED,2=PRECONDITION_RETAINED,3=CLOSE_UNRESOLVED_RETAINED denotes aggregate worst unresolved Accelerator-owned operation ownership, not a raw PST result and not destructive attempt count. All individual results/dispositions/lifetimes remain local evidence. FINAL is the Accelerator owner summary, not remote attestation of Browser resource destruction. Browser MUST combine it with its own confirmed C1/DATA ownership finalization before local clean-operation completion; retained Browser resources prohibit local clean completion even if the server summary is CLEAN. No frame here attests native destruction on another machine. NONE requires no created operation resources; DESTROYED means each relevant owned resource destruction confirmed, borrowed leases released; shared provider/runtime need not be destroyed if still legitimately borrowed. Retained dispositions require outcome3/action2. Clean requires fact0/action3/outcome1/tls_fact2/ownership1/fence_mask15. Non-clean uses fact1..14/action1/outcome2/ownership0 or1; no missing fence/loss/deadline/retention converted to clean. For an admission denial, FINAL is not sent: OPEN_RESULT is the decision.

PRECONDITION_RETAINED preserves references/manager/dependencies; only governed membership/eligibility resolution then eligible release is allowed. CLOSE_UNRESOLVED_RETAINED is terminal-retained: safe queries only, no I/O/shutdown/release/native retry/rebind/reuse/alternate cleanup. Aggregate chooses CLOSE_UNRESOLVED above PRECONDITION; release attempt/result/disposition/destruction distinct. FINAL retention preserves first cause (e.g. cancel) and encodes retention separately rather than rewriting it as close failure. New local cleanup progress never sends a second FINAL or resurrects operation.

CONTROL loss produces local non-clean cascade, cannot publish FINAL on another Session. Summary loss/partial write leaves peer unknown; no status query, inferred success or auto replay. APPLICATION framing completeness remains separate: only explicit TLS_TRUNCATED with independently valid complete CL/chunked is family-ADR13's usable-application exception, never clean transport or arbitrary abort.

Each owner serializes authority check, bounded submission and accepted count. Local revocation forbids new payload work; previously lower-layer-accepted bytes can escape later. Do not complete an incomplete DATA frame using unauthorized payload; abandon affected channel/operation without splicing and preserve borrowed buffers until safe. Narrow CANCEL/FINAL authority does not restore DATA permission. Independent CONTROL/DATA do not promise instantaneous remote revocation; receiver stops on its own observation.

## 7. State/type dispatch table
Every row also requires matching identity, local grant/policy, direction and bound lengths. No unlisted new message/state transition is valid; unrelated old messages retain their own existing grammar.

| Message | Eligible state | Factual transition |
| --- | --- | --- |
| OPEN | Negotiated C2 CONTROL, admission capacity | Request outstanding; reserve or deny once |
| OPEN_RESULT | Browser matching outstanding request | Grantâ†’RESERVED; denialâ†’quiesce |
| BIND | Accepted unbound DATA, live RESERVED/PREPARING | Atomic empty role claimâ†’ACK_WRITING |
| BIND_ACK | Browser matching claim, PREPARING | Full receipt marks only that role acknowledged |
| SITE_READY | Both roles ACK-written, preparing | Validate readyâ†’ACTIVE_HANDSHAKE |
| TLS_READY | Handshake verified, no revoke, roles active | Full write/receipt barriersâ†’ACTIVE_APPLICATION independently |
| DATA RECORD | Activated handshake/application/shutdown, unsealed | Ordered opaque payload |
| DATA APPLICATION | TLS-ready barriers/application/shutdown, unsealed | Ordered plaintext only |
| FENCE/ACK | Activated role/end phase, matching pending prefix | Seal/confirm only matching direction |
| RECORD_EVENT | Activated RECORD, actual native fact | Preserve fact; EOF may allow reciprocal progress, reset/error terminate |
| CANCEL | Live pending request or any nonterminal matching operation | Local authority revokeâ†’TERMINATING |
| FINAL | Browser matching operation, unfinalized live CONTROL | Valid summaryâ†’RELEASED or TERMINAL_RETAINED |

Server denying before reservation must emit one result; request pending cancellation racing grant invalidates tokens and activation. Browser dropping a late result after local cancel must still safely quiesce owned context; it must not connect. FINAL can arrive before delayed DATA callbacks; callbacks revalidate incarnation before dereference/delivery. Fence ACK controls may be processed during normal shutdown while payload remains revoked only as specifically authorized bounded completion; terminal retention allows no role I/O.

Session states: CLASSIFYâ†’CONTROL1â†’NEGOTIATINGâ†’C2_ELIGIBLEâ†’LOST; LOST cannot return. Role states: DATA_ACCEPTEDâ†’CLAIMEDâ†’ACK_WRITING/OBSERVEDâ†’ACTIVEâ†’SEALEDâ†’RELEASED or RETAINED. Any required loss revokes operation/sibling under owner lifecycle; foreign candidate failure does not tear down a victim.

### 7.1 Error scopes and duplicate handling
A malformed/unnegotiated/wrong-state CONTROL frame invalidates that CONTROL Session and cascades only its dependencies; the independent existing-service Session is unaffected. Before role authority is established, malformed or unauthorized DATA closes only that candidate, never the named victim. After a valid role commit, a DATA framing/identity/state violation terminates that operation and sibling under governed ownership; no destructive fallback. Transport-integrity loss uses the same minimal safe scope.

Valid OPEN admission denial has exactly one OPEN_RESULT and no retry extension; role rejection has no error ACK. Duplicate remote OPEN, READY, TLS_READY, ACK, FENCE, EVENT, CANCEL or FINAL is a state violation; existing DATA_TICKET_REQUEST idempotent reissue remains unchanged. No generic C2 ERROR, heartbeat or recovery message is proposed. Internal duplicate cancellation intents may coalesce before serialization; wire duplicates are never silently accepted.

APPLICATION fences require its TLS-ready activation, RECORD fences require actual activation and no further authorized TLS record production. Each sender can emit one fence/direction, each receiver one matching ACK only after downstream accounting. A sender may receive the opposite direction's fence before its own end; one seal does not disable remaining legitimate reverse flow. Deadline/loss/retention cannot relax these guards.

## 8. Numeric bounds and progress
Existing negotiation input max28 remains. C2 CONTROL per-type maxima803,108,32,287,40,76; no universal enlargement of old readers. Each normal CONTROL frame staging bound=16+803=819; inbound scratch803; reserved CANCEL frame56 and FINAL frame92 per admitted operation. Reserve ordinary staging plus notifications even when queues full. At most one admitted operation/Session and two global, including retained owners; global notification reserves2*(56+92)=296. No heap growth on hostile declared length.

Four payload queues64KiB each, high48/low24KiB, max512KiB global payload, fragment total16KiB; charge frame staging separately and cap: one RX and one TX role frame per DATA connection (2*(16+16384)), two DATA/op =>65600 bytes/op. No multiplication by arbitrary queued frame count. Bridge endpoints, authority/owner/callback/tombstone/PST/provider memory require measured fixed admission caps before implementation qualifies; wire-byte bounds alone are NOT a total process/kernel-memory proof. Retained memory occupies original budget; never evict it.

Bounded invalid-binding work: each accepted unbound DATA admits one BIND only, â‰¤4 unbound C2 DATA globally andâ‰¤2/Session, charged independently from two role slots. Each Session allows at most8 failed candidates per rolling owner-monotonic10s; then reject fresh C2 candidates until window clears, without destroying existing roles. These are bounded implementation/lab policy, not production safety qualification. Scheduler processes at most8 opportunities/wake with CONTROL opportunity before more DATA; payload16KiB/flow. Partial CONTROL cannot be spliced; timeout cascades once.

Local attach5s, handshake10s, no-progress30s, shutdown5s, cleanup5s lab budgets unchanged; duplicate/wake cannot reset absolute phase budgets. No-progress timer starts only in active progress phase; observed actual payload/TLS progress may refresh this inactivity budget, not fixed handshake/shutdown total. Partial result/ACK stalls cannot indefinitely retain reservation. No timer creates destructive fallback. Capacity/provider/RNG absence denies before effects.

## 9. Normal and error sequences
~~~mermaid
sequenceDiagram
    participant B as Browser
    participant A as Accelerator
    participant P as PST
    participant S as Site
    B->>A: Existing CONTROL and negotiation (2,1),(3,1)
    A-->>B: Existing result, both keys
    B->>A: C2_OPEN immutable tuple
    A-->>B: C2_OPEN_RESULT grant, two role authorities
    B->>S: C1 exact connect after local policy
    B->>A: Existing DATA attach then C2_BIND RECORD
    A-->>B: Complete C2_BIND_ACK RECORD
    B->>A: Existing DATA attach then C2_BIND APPLICATION
    A-->>B: Complete C2_BIND_ACK APPLICATION
    B->>A: C2_SITE_READY after both ACK receipts
    A->>P: Activate factual TLS progress
    A-->>B: C2_DATA RECORD
    B->>S: Opaque TLS records
    S-->>B: Opaque TLS records
    B->>A: C2_DATA RECORD
    P-->>A: Verified TLS1.3/name/chain/ALPN facts
    A-->>B: Complete C2_TLS_READY CONTROL
    B->>A: C2_DATA APPLICATION
~~~
~~~mermaid
sequenceDiagram
    participant B as Browser
    participant A as Accelerator
    participant P as PST
    B->>A: Same-role C2_FENCE after payload prefix
    A->>A: Account exact declared downstream
    A-->>B: C2_FENCE_ACK same prefix
    A->>P: Governed reciprocal shutdown
    P-->>A: Factual completion or retention
    Note over B,A: Repeat all four directional fences, no global socket order
    alt All clean guards and operation destruction confirmed
        A-->>B: C2_FINAL CLEAN on live CONTROL
    else Cancel, loss, failed drain or retained close
        A->>A: Preserve first cause and disposition
        A-->>B: C2_FINAL NON_CLEAN or RETAINED if authorized
    end
~~~
~~~mermaid
stateDiagram-v2
    [*] --> RESERVED: Granted OPEN
    RESERVED --> PREPARING: Governed roles
    PREPARING --> HANDSHAKE: Both ACKs plus ready
    HANDSHAKE --> APPLICATION: TLS-ready barriers
    APPLICATION --> SHUTDOWN: Ordered end
    SHUTDOWN --> RELEASED: Four ACKs plus TLS and destruction
    RESERVED --> TERMINATING: Cancel or expiry
    PREPARING --> TERMINATING: Loss or revoke
    HANDSHAKE --> TERMINATING: Error or deadline
    APPLICATION --> TERMINATING: Cancel or loss
    SHUTDOWN --> TERMINATING: Failed guards
    TERMINATING --> RELEASED: Destruction confirmed
    TERMINATING --> RETAINED: Model E retention
    RELEASED --> [*]
~~~

### 9.1 Revocation and incomplete-frame sequence
~~~mermaid
sequenceDiagram
    participant O as Sending owner
    participant T as DATA transport
    participant R as Receiving owner
    O->>T: Authorized bounded write
    T-->>O: Actual accepted prefix count
    O->>O: Linearize revoke, suppress unsubmitted payload
    O->>T: Governed abandon if frame incomplete
    R->>R: Revoke at local observation; reject stale delivery
    Note over O,R: Previously accepted bytes irreversible, no global clock
~~~

## 10. Examples, security and qualification
[Approved encoding vectors](pacc-c2-wire-vectors-revision-1.0.json) contain header+payload bytes for every assigned type, plus negotiation and denial. Tokens are deterministic PUBLIC TEST VALUES, MUST NOT be used in live operation. State-dependent vectors are isolated illustrative frames, not an observed runtime transcript. [Layouts](pacc-c2-wire-layouts-revision-1.0.json) carry exact offsets/limits and registry source hashes. Human-readable hex follows below.

### C2_OPEN — 73 frame bytes

~~~text
50 41 43 43 01 00 00 10 00 10 00 00 00 00 00 39
00 00 00 00 00 00 00 01 00 00 00 01 00 00 00 01
01 01 01 BB 7F 00 00 01 00 07 00 07 00 00 00 00
01 01 00 03 00 00 00 00 61 2E 74 65 73 74 78 61
2E 74 65 73 74 78 02 68 32
~~~

### C2_OPEN_RESULT — 124 frame bytes

~~~text
50 41 43 43 01 00 00 10 00 11 00 00 00 00 00 6C
00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01
00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01
00 00 00 01 00 00 00 01 00 00 01 00 01 02 03 04
05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14
15 16 17 18 19 1A 1B 1C 1D 1E 1F 20 21 22 23 24
25 26 27 28 29 2A 2B 2C 2D 2E 2F 30 31 32 33 34
35 36 37 38 39 3A 3B 3C 3D 3E 3F 40
~~~

### C2_BIND — 84 frame bytes

~~~text
50 41 43 43 01 00 00 10 00 12 00 00 00 00 00 44
00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01
00 00 00 00 00 00 00 01 00 00 00 01 00 00 00 01
01 00 00 00 01 02 03 04 05 06 07 08 09 0A 0B 0C
0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C
1D 1E 1F 20
~~~

### C2_BIND_ACK — 44 frame bytes

~~~text
50 41 43 43 01 00 00 10 00 13 00 00 00 00 00 1C
00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01
00 00 00 00 00 00 00 01 01 00 00 00
~~~

### C2_SITE_READY — 48 frame bytes

~~~text
50 41 43 43 01 00 00 10 00 14 00 00 00 00 00 20
00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01
00 00 00 00 00 00 00 01 00 00 00 01 00 00 00 01
~~~

### C2_TLS_READY — 50 frame bytes

~~~text
50 41 43 43 01 00 00 10 00 15 00 00 00 00 00 22
00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01
00 00 00 00 00 00 00 01 03 04 00 07 00 02 00 00
68 32
~~~

### C2_DATA — 49 frame bytes

~~~text
50 41 43 43 01 00 00 10 00 16 00 00 00 00 00 21
00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01
01 00 00 00 00 00 00 00 00 00 00 00 68 65 6C 6C
6F
~~~

### C2_FENCE — 48 frame bytes

~~~text
50 41 43 43 01 00 00 10 00 17 00 00 00 00 00 20
00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01
01 00 00 00 00 00 00 00 00 00 00 05 00 01 00 00
~~~

### C2_FENCE_ACK — 48 frame bytes

~~~text
50 41 43 43 01 00 00 10 00 18 00 00 00 00 00 20
00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01
01 00 00 00 00 00 00 00 00 00 00 05 00 00 00 00
~~~

### C2_RECORD_EVENT — 48 frame bytes

~~~text
50 41 43 43 01 00 00 10 00 19 00 00 00 00 00 20
00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01
02 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00
~~~

### C2_CANCEL — 56 frame bytes

~~~text
50 41 43 43 01 00 00 10 00 1A 00 00 00 00 00 28
00 02 00 01 00 01 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01
00 00 00 00 00 00 00 01
~~~

### C2_FINAL — 92 frame bytes

~~~text
50 41 43 43 01 00 00 10 00 1B 00 00 00 00 00 4C
00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01
00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 01
00 00 00 03 00 01 00 02 00 01 0F 01 00 00 00 00
00 00 00 05 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00
~~~

### CAPABILITY_OFFER_CANDIDATE_KEYS — 44 frame bytes

~~~text
50 41 43 43 01 00 00 10 00 07 00 00 00 00 00 1C
00 02 00 02 00 00 00 02 00 01 00 00 00 03 00 01
00 00 00 02 00 01 00 00 00 03 00 01
~~~

### CAPABILITY_RESULT_CANDIDATE_KEYS — 30 frame bytes

~~~text
50 41 43 43 01 00 00 10 00 08 00 00 00 00 00 0E
00 02 00 00 00 02 00 01 00 00 00 03 00 01
~~~

[Approval/security review](../capabilities/pacc-c2-exact-wire-security-review-20261011.md) maps attacks, original P/N and CG tests to owner decisions. No test qualification is claimed by document serialization. Future goldens must match owner-approved spec/codecs/tests, independent peer, both compatibility directions, C89/VC6 and real NT4. New positive/min/max/split/coalesced and all malformed lengths/enums/reserved/trailing/overflow cases must execute after authorization, along with lifetime tests.

C2_IMPLEMENTATION_AUTHORIZED=NO; C3_IMPLEMENTATION_AUTHORIZED=NO; E1_E12=NOT_EXECUTED; LEGACY_PRODUCTION_AUTHORIZED=NO. Local human-specification consistency gate stays mandatory. Global ecosystem principle remains proposed. ADR-0034 accepts the exact contract/encoding and registry; owner must separately authorize implementation, qualification executions and production enablement. Historical accepted ADRs, old drafts, C0/C1 and SDK preserved; production pin0.6.4.

## 11. Normative gate separation and resolved ED08/ED11 clauses

### 11.1 Security and admission before effects

The modern x64 Accelerator MUST use BCryptGenRandom(NULL, buffer, 32, BCRYPT_USE_SYSTEM_PREFERRED_RNG), require STATUS_SUCCESS, independently per role. Unsupported target/API or RNG failure denies admission; no fallback or Browser RNG requirement is introduced. Fixed32-byte token equality MUST use vetted full-length content comparison with no secret-dependent early exit, access pattern or branches. Before integration, record the exact comparison/erasure utility source, revision, SHA256, license and portability recipe; audit optimized machine code on each compiler/target. Modern owner erasure uses SecureZeroMemory; Browser VC6 uses a separately reviewed non-elidable C89 erasure port with full-byte wipe tests and optimized-output inspection. Ordinary memset is not proof. These are concrete implementation artifact/verification gates, not pending wire/security design choices. No candidate utility source is approved merely by being suggested in an old review.

All secret copies become unusable immediately on revocation/claim/expiry; physical erasure waits for borrower/write/callback quiescence. Secrets MUST NOT be logged. Retained copies/dependencies stay charged and confer no authority. Enforce independent pre-auth connection/handshake quotas and deadlines before expensive effects; BIND rate limits alone are insufficient. Secure Principal/Session/grant equality precedes role token claim. Legacy remains plaintext lower assurance and requires separate lab admission; production Legacy is not authorized.

TLS_READY MUST use successful public API3 CLIENT handshake and successful peer-summary queries before CLOSED/shutdown/release: certificate_present, chain_validated, peer_name_validated, peer_authenticated exactly PST_KNOWN_TRUE; TLS exactly PST_TLS_VERSION_1_3; map to wire0x0304/mask0x0007 explicitly. Query public selected ALPN with bounded storage and exact offer/profile validation. UNKNOWN/FALSE/UNSUPPORTED/NOT_APPLICABLE/errors never satisfy required facts. Revalidate operation/generation/policy before full TLS_READY publication. The exact normal package/client capabilities remain implementation/target gates.

### 11.2 ED08 factual closure partial order

The authoritative order is the audited supplement0.3.2 phase machine, not a reordered shorthand of BA/shutdown steps:

APPLICATION_DELIVERY_FINISHED → PUBLIC_RECIPROCAL_COMPLETE → APP_AB_FENCE_FULL_RECEIPT → BROWSER_BA_PRODUCER_FROZEN → BA_EXACT_PREFIX_AND_ACK_FULL_WRITE → WAITSET_REMOVAL_CONFIRMED → PST_CHILD_RELEASE → DESTROYED → AB_ORDERED_EOF_DRAIN → AB_EXACT_PREFIX_AND_ACK → OWNER_FINALIZATION.

Required APPLICATION delivery and the APPLICATION BA fence/downstream ACK MUST finish before terminal PST shutdown where required for that prefix. No required plaintext read may be postponed until C_SHUTTING. APP AB END_PREFIX is eligible only after reciprocal COMPLETE and no further admissible plaintext output; Browser must fully observe that authorized fence before voluntarily freezing new RECORD BA admission. Such freeze ends an admitted prefix; it is not a fabricated native EOF. Preserve RECORD AB native send while BA freezes.

Already admitted BA payload MUST be submitted to C3 with exact prefix accounting and matching BA FENCE_ACK fully written before PST child release. No release until governed wait-set removal and all other public preconditions are factual. DESTROYED confirms only the PST child; keep manager, C3 consumer peer, role channels and shared leases for their independent lifetimes. Then bounded ordered C3 EOF plus exact AB accounting and provider-output guarantee permit AB seal; Browser C1 native-send acceptance precedes its matching AB ACK. Native send acceptance is not Site TCP delivery.

COMPLETE, DESTROYED, PST_RECORD_OUTPUT_DRAINED, PACC prefix acceptance and Browser native acceptance MUST remain separate facts. Exact-package/provider shutdown/output-preservation proof is mandatory before clean target qualification. COMPLETE/idle/WOULD_BLOCK/timeouts alone cannot imply drain. PRECONDITION_RETAINED permits only governed precondition correction; CLOSE_UNRESOLVED_RETAINED is query-only, non-clean, preserves every dependent owner/lease/quota and forbids I/O/rebind/release/native retry/reuse/fallback. Do not fabricate target EOF or AB seal in retention.

Final clean requires all four exact ACKs, public TLS facts and confirmed operation-owned cleanup plus Browser's separate local finalization. Deadline/reset/CONTROL loss/cancel/prefix mismatch/retention follows truthful NON_CLEAN or RETAINED with immutable first cause. R4 controlled-provider class and native Winsock-failure class remain distinct.

~~~mermaid
sequenceDiagram
    participant B as Browser owner
    participant A as Accelerator owner
    participant P as PST child
    A->>A: Finish required APPLICATION delivery/prefix obligations
    A->>P: Reciprocal shutdown
    P-->>A: Public COMPLETE
    A-->>B: APP AB end-prefix fence
    B->>B: Full fence receipt; freeze new RECORD BA, preserve AB send
    B->>A: Admitted BA prefix and matching fence
    A->>A: Exact C3 BA native acceptance
    A-->>B: Full BA ACK write
    A->>P: Remove membership, eligible try_release
    alt OK plus DESTROYED
        P-->>A: Child destruction confirmed
        A->>A: Keep owner/C3 peer; exact ordered AB drain to EOF
        A-->>B: AB prefix and fence
        B->>B: Exact C1 native-send acceptance
        B-->>A: Matching AB ACK
        A-->>B: FINAL after all owner guards
        B->>B: Independent local cleanup guards
    else Retained
        A->>A: Retain owner/dependencies/quota; no clean/EOF fabrication
        A-->>B: Authorized retained summary only on live CONTROL
    end
~~~

~~~mermaid
stateDiagram-v2
    APPLICATION --> QUIESCING: Required plaintext finished
    QUIESCING --> TLS_COMPLETE: Public reciprocal completion
    TLS_COMPLETE --> BA_FROZEN: Browser observed APP AB fence
    BA_FROZEN --> CHILD_RELEASE_ELIGIBLE: Exact BA prefix, full ACK, membership removed
    CHILD_RELEASE_ELIGIBLE --> AB_DRAIN: OK and DESTROYED
    CHILD_RELEASE_ELIGIBLE --> RETAINED: Close unresolved
    CHILD_RELEASE_ELIGIBLE --> PRECONDITION: Eligibility retained
    PRECONDITION --> CHILD_RELEASE_ELIGIBLE: Governed correction only
    AB_DRAIN --> FINAL_ELIGIBLE: Ordered EOF, exact AB ACK and all owner guards
    FINAL_ELIGIBLE --> RELEASED: Authorized FINAL, separate Browser cleanup
    RETAINED --> RETAINED: Safe query only
~~~

### 11.3 ED11 controlled limits and target gates

Managed cap is2MiB per Browser/Accelerator process for C2 controlled allocations. Preserve ADR-0029 experimental two global operations/one per Session, four64KiB queues with high48/low24KiB,512KiB aggregate payload,16KiB flow service and at most8 opportunities/wake. Metadata/staging/allocator/controller overhead MUST be charged separately and fit the managed cap; no payload subtotal is total process memory. Browser application consumer storage is charged, not unlimited.

Reserve fixed CONTROL803/819, CANCEL56/FINAL92, bounded role framing and callbacks independently of DATA saturation. Use checked arithmetic, finite identity-safe slots, one CONTROL opportunity before more DATA and bounded rotating flow service. Commit only after capacity/identity/authority/required dependency checks. Partial allocation or cancel before effects rolls back only newly acquired unconsumed storage. Session CONTROL quota survives zero operations and is reclaimed only after its own destruction and borrower quiescence.

Preserve candidate incremental user/kernel envelopes: Browser20MiB/2MiB; Accelerator32MiB/8MiB, beyond factual baseline, never RAM minima or process-total ceilings. These totals/provider/kernel/NT4 are not numerically qualified. Track total, baseline, increment, peak, shared attribution, retained charge and available target margin separately. Admission MUST account for both incremental quota and host/process physical/commit/address-space capacity; reserve/refund shared host credits so concurrent admissions cannot spend one snapshot twice. Retention keeps original quota without eviction.

Unknown mandatory provider/kernel/resource bound, stale/missing required measurement or insufficient host margin denies operational enablement before effects. No empirical envelope silently becomes a hard bound. Target metric, freshness/headroom values and any empirical-lab uncertainty exception require separate evidence-backed target approval before runtime admission. Unknown values may be represented explicitly and tested as denial while implementation proceeds. Contract approval therefore does not depend on measuring code that does not yet exist; successful operational admission/qualification does.

### 11.4 Four independent states and implementation gates

CONTRACT_APPROVAL is accepted by ADR-0034. IMPLEMENTATION_AUTHORIZATION is NOT granted here. IMPLEMENTATION_QUALIFICATION remains pending mandatory codec/security/resource/real-owner/target tests. PRODUCTION_ENABLEMENT remains denied until separately approved after qualification. No implication between these states is automatic.

Before qualification require exact revision/layout/vector/codec agreement; old-peer both-direction compatibility; secret utility source/optimized-output gates; all Secure/Legacy isolation/replay/revocation negatives; initialized real owner publication attempt/authorized/delivered/rejected evidence; ED08 clean and R4 non-clean lifetimes; total target/provider/kernel admission evidence; and actual legacy target execution when claimed. E1-E12, API/package publication revision, Browser implementation and PST promotion remain separate authorizations. No missing implementation test is a reason to undo accepted contract semantics; a discovered normative contradiction instead requires a new governed decision.
