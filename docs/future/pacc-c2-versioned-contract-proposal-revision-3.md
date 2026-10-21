# C2 revision 3 — consolidated semantic contract candidate

Date: 2026-10-11. Status: PROPOSED; owner/security/PACC approval required.
Input revision2 remains preserved byte-exact. This document consolidates its
open semantic choices; it allocates no message/capability/role IDs or layouts.
The clauses below are intended MUST/MUST NOT requirements if approved.
They do not supersede accepted ADR-0022/0029 or authorize C2/C3/E1–E12.

## Traceability and proposed resolutions

Decision candidate: [C2 ADR candidate](../adr/ADR-CANDIDATE-pacc-c2-operation-and-progressive-specification.md).
Protocol: [draft0.2](../protocol/pacc-c2-protocol-specification-draft-revision-0.2.md).
Oracles/encoding plan: [conformance matrix](../capabilities/pacc-c2-revision3-conformance.md).
R3-I01–I12 below are documentary requirement identifiers, not wire IDs.

### R3-I01 — Session composition and grants

Retain the proposed operation-service key plus TLS_OFFLOAD v1, two keys/set,
on a dedicated C2 Session. Existing Mode B Session/offers remain unchanged.
Neither grant snapshot contains powers not offered/granted. In particular,
C2's two-key snapshot does not contain NETWORK_EGRESS and cannot execute Mode B.
Do not add a third key to an unknown peer or renegotiate/replace a Session
silently. Missing C2 grant returns unavailable before site connect/role/PST effects.

Browser candidate composition permits at most one existing-service Session and
one C2 Session per explicitly selected profile/endpoint configuration. Both
are coordinated by ONE integration owner and the existing shared PST-runtime
serialization/credential lease rules, not parallel production workers/runtimes
or an unbounded pool. They have independent CONTROL incarnations, grants,
deadlines and dependent DATA. No Secure/Legacy mixing or automatic route fallback.
An operation selects its authorized placement/Session before side effects;
existing started operations never migrate. C2 loss does not close the independent
Mode B Session; integration-wide Stop closes both under existing owner authority.
Retained resources consume capacity; admission cannot evict/reuse them.
This composition is a proposed consumer contract, NOT existing implementation.

### R3-I02 — Versioned CONTROL dispatch and bounded progress

Envelope/CONTROL1.0, old IDs, old negotiation payload28 and two-key bound remain.
The operation service major/IDs and exact new payload limits are encoding gates.
A new per-message decoder can become eligible only after validated grant and
complete negotiation-result publication/observation at the respective owner.
Header validation checks type/direction/state and that message's exact allowed
length BEFORE accumulation/allocation. Preserve buffered bytes across handoff;
no universal oversized limit, flags reuse or generation-metadata inference.

For each C2 state, the encoding candidate must define a finite maximum M(state,
type), staging requirement and arithmetic bound; undefined M forbids admission.
Admission reserves a normal in-flight CONTROL slot plus bounded cancellation
and terminal-notification capacity for each admitted operation. The slot size is
derived from approved maximal grammar, not an invented number in this contract.
Duplicates coalesce only internal intents, never silently accept duplicate
remote frames. No unbounded notification list or unsolicited negative responses.
CONTROL receives a bounded scheduling opportunity before further DATA turns;
the existing per-flow/wake budgets remain. Priority cannot splice a partial
frame. An already-authorized unrelated in-flight CONTROL frame may finish within
its existing finite deadline; if CONTROL cannot progress, expire/cascade once.
C2 admission fails before reservations/site/PST work if these bounds are absent.
Lab attach5s/handshake10s/no-progress30s/shutdown5s/cleanup5s stay experimental.
Timer start, elapsed arithmetic and remaining budget are owner-local monotonic;
wake or a replayed request cannot extend an absolute phase/total deadline.

### R3-I03 — Local revocation and partial-send treatment

Each owner serializes eligibility check, one bounded I/O submission and its
accepted count against its local revocation transition. No new payload
submission/delivery is authorized after that LOCAL transition. An already
entered bounded call and irreversible transport/provider/kernel bytes are
recorded in the escaped prefix; revocation does not retroactively retract them.
Operation accepted/submitted/peer-validated/downstream-delivered counts differ.

The server rechecks live policy at commit/activation/TLS-ready and each payload
boundary; it stops its own delivery immediately upon observed revocation.
Browser revokes at local cancel, authenticated revocation observation, CONTROL
loss or local expiry. Independent CONTROL/DATA streams cannot create an
instantaneous global revocation clock: pre-observation in-flight bytes may
arrive. This limitation is explicit, not cryptographic endpoint attestation.
After local revocation, queued unsubmitted payload is suppressed; borrowed
buffers remain until the call/owner no longer references them.
A partially submitted operation DATA frame is abandoned with that channel/
operation rather than completed with newly unauthorized payload or spliced/
reused. The sibling terminates logically; physical cleanup stays owner-only/
Model E. Termination notification has separate narrow owner authority and
cannot revive payload permission. If CONTROL notification cannot finish,
use bounded Session-loss behavior, not an unbounded flush or new permission.

### R3-I04 — Identity, generation and admission

An operation record binds its live CONTROL/Session incarnation, assurance/
admitted context, server-issued correlation/incarnation, Browser snapshot
provenance, hostname/numeric tuple, grants/epoch, two immutable role slots,
deadlines and reserved resources. Local resolution generation is equality/
provenance within that snapshot scope, not a server-global freshness counter.
Browser checks its current owner/snapshot before exact connect/publication.
Server verifies the immutable reserved tuple and operation incarnation, not
the physical remote socket. New host/endpoint/port/redirect/profile requires
new authorization/attempt; IP does not replace SNI/verification identity.

Exactly one unpredictable single-use authority per role. A structural DATA
ticket is not that secret. Validate existing authenticated DATA Principal,
Session/incarnation, live policy/epoch and complete role frame, then atomically
consume authority and claim that role. A mismatch before commit preserves a
legitimate unused token; a committed token is never restored on ACK failure.
Guessed ID/wrong authority rejects only the unauthorized candidate, never a
victim operation. Once operation authority is established, a protocol violation
can terminate that operation/sibling. No stale identity reuse until retained
owners/callbacks/frames are quiescent; exhausted generation fails closed.
Entropy strength/length/provider/construction/comparison and rate/work limits
remain security-owner/encoding decisions; missing parameters forbid admission.

### R3-I05 — ACK meaning and activation barrier

Separate role claim, ACK enqueue, complete local ACK write, validated peer
receipt, activation, TLS verification, result publication and result observation.
Role ACK means only the exact role/operation claim completed; it proves neither
Site TLS nor downstream byte delivery nor resource destruction.
Browser observes both complete valid role ACKs before sending site-ready.
Server checks both committed roles and complete ACK writes plus exact ready
attempt/current policy before ACTIVE_HANDSHAKE. Partial or missing ACK never
activates; retransmitting a consumed authority is a replay, not recovery.
PST construction is not permission to progress TLS before activation.
Verified-TLS result is proposed on CONTROL and explicitly operation-bound;
server enables APPLICATION input only after factual verification and complete
result publication. Browser enables its APPLICATION payload only after full
matching result validation. Failed/partial/lost result yields bounded failure,
never queued plaintext exposure or implicit fallback.

### R3-I06 — Role/direction fences and measurable downstream completion

For each operation/incarnation, role and direction, sender maintains the exact
payload prefix submitted before seal. A proposed fence follows that prefix on
the SAME ordered role DATA channel. Its semantic binding is operation,
generation, role, direction and final prefix length; representation TBD.
Receiver validates the full matching fence and exact observed prefix, then
acknowledges only after preceding bytes reach that role's declared downstream
boundary. A fence ACK refers to the matching operation/generation/role/direction/
prefix. Its reverse direction does not acknowledge other forward/reverse fences.

Boundary definitions: Browser outbound RECORD -> C1 native send accepted bytes;
Accelerator inbound RECORD -> private bridge native send accepted bytes;
Accelerator inbound APPLICATION -> public PST plaintext-write accepted bytes;
Browser inbound APPLICATION -> owned authorized application-consumer storage.
These are NOT proof of peer TCP receipt, TLS internal record consumption, HTTP
completeness or navigation/UI commit. Partial native/PST writes remain pending.
Sender/receiver validate duplicate/conflicting fences and overflow; no DATA
payload after that direction is sealed. TCP direction-end follows accounted
outbound bytes. Closing one direction leaves the other usable until its own end.
ACKs/status evidence are control semantics within the role, not payload bytes.
Exact grammar, counter widths and all directional valid/invalid states are
encoding gates; no existing NETWORK_HALF_CLOSE is reinterpreted.

For clean shutdown the operation owner records factual PST reciprocal completion,
seals the actual admitted role prefixes, obtains all required fence confirmations
and confirms eligible operation-resource finalization. Record transport EOF/
reset and PST close facts remain separate; no need to fabricate EOF when PST
has completed. Later bytes beyond a sealed prefix are invalid, not drain forever.
Missing fence/ACK, CONTROL/DATA loss, deadline or retained destruction prevents
final clean-operation success. Lower-layer TLS COMPLETE remains a preserved
subfact; it is not the first accepted clean operation terminal until all clean
preconditions hold. In-flight bytes that cannot be safely drained fail boundedly.
No cross-socket timestamp/order inference. No claim of PST private-byte counters.

### R3-I07 — Terminal facts, publication and Model E

First VALID operation terminal accepted by its owner is immutable; lower-layer
subfacts, security action and destruction disposition are distinct.
Cancel/revocation/loss/deadline cannot be turned into success by complete HTTP.
Family ADR-0013's explicit TLS_TRUNCATED exception applies only to independently
validated complete application framing, not unknown abort/CONTROL loss.

Final operation outcome is proposed on still-authorized CONTROL, not an already
released DATA channel. Narrow terminal publication authority persists only for
the captured incarnation. If CONTROL is lost or summary publication fails, the
peer has unknown/non-clean completion, not inferred success or automatic retry.
No unapproved terminal-query/recovery message is introduced.

Physical operation-owned site/PST/bridge/DATA release and borrowed dependency
release are distinct from destroying a shared runtime. Shared providers/runtime/
credential leases remain alive while any governed owner requires them.
PRECONDITION_RETAINED allows only governed eligibility resolution before eligible
retry; CLOSE_UNRESOLVED_RETAINED permits no I/O/new release/native retry/rebind/
reuse/alternate close. Retained operation resources are charged, not evicted.
Exactly-once destructive completion is per lifetime; attempts and dispositions
are not counted as destructions. Clean-summary gating is new C2 candidate scope
and does not rewrite existing Network Offload terminal semantics.

### R3-I08 — Backpressure and full budgets

Preserve four directional bounded queues with per-queue high/low water and
read-interest removal/resume. Count parser/writer staging, two DATA, bridge
endpoints, CONTROL slots, authority records, callbacks/tombstones and retained
dependencies in admission. Maximums must be declared/validated before admission.
ADR-0029 lab bounds and C1 two-live-owner cap remain; smaller 8/16/32/64KiB
measurements and production budgets are later gates. Reaching any cap denies
new work, never steals a live/retained slot or grants destructive cleanup.
Cancellation wakes owners and does not wait for a saturated payload queue to
drain indefinitely. TLS cross-direction readiness is factual; no busy polling.

### R3-I09 — Independent Site TLS and assurance

Mode C requires operation-service + TLS_OFFLOAD grants and both Browser local
egress permission and server Site TLS/destination policy. It does NOT require
Accelerator NETWORK_EGRESS and opens no public site socket there. Trust source/
TLS profile and grants are server-owned; Browser cannot select privileged
trust/interface or reuse inbound CA implicitly. Legacy admission does not
create a cryptographic Principal. Same functional Site TLS target, lower first-
hop assurance: no token protects Legacy from an on-path observer/attacker.
D9A materialized lab grant and separately approved execution remain necessary;
D9B/C9-C11 production Legacy still unapproved; Secure downgrade prohibited.

### R3-I10 — Compatibility and portable encoding gate

Old peers omit the unknown valid service key; send no new grammar before grant.
Old CONTROL/DATA/framing/capability/Network Offload IDs1-15 and bytes remain
unchanged. Envelope/CONTROL versions, service major, C API/package metadata
and documentary revisions are separate. Reserved flags cannot negotiate C2.
Exact field types/order/width/charset/required-optional rules, type allocation,
counter/token representation and error enums are separate owner decisions.
No packed structs, native pointer/long layout or mandatory heap in codecs.
Define unknown/duplicate/trailing fields and max+1/overflow rejection explicitly;
no implicit TLV or skip-unknown rule. Wire Encoding Ready remains NO.

### R3-I11 — Progressive ADR-linked specification

Every proposed/accepted decision has a local ADR candidate/accepted record,
separate human protocol clauses, sequence/state updates and conformance/test
mapping. The specification does not become an ADR appendix containing the
whole protocol. Approval records exact revisions/hashes; later decisions use
new ADR/revision under governance, never retroactively introduce them into
accepted ADRs. The existing local documentation-consistency gate is preserved.
Global ecosystem extension is a separate PROPOSED principle for central review.

### R3-I12 — Approval and execution limits

This candidate closes semantic ambiguity for owner review, not accepted policy.
Approval must cover R3-I01-I10 together or identify dependencies and dispositions.
Encoding/security/version/compatibility review must then provide complete
field tables, allocated registry, portable goldens/negatives and package metadata.
C2/C3 implementation and C7 E1-E12 require their separate authorizations.
C0/C1 qualifications remain scoped and unchanged; actual NT4 still NOT_RUN.
PST pin0.6.4; no upstream changes, promotion, commit/push/tag/release.

## Explicit revision2 differences

| Revision2 gap | Revision3 proposed resolution | Still pending |
| --- | --- | --- |
| R2-01 Session/grants | Dedicated bounded C2 Session + existing Session; one owner | Owner acceptance; actual consumer qualification |
| R2-02 CONTROL | State/type maxima, reserved finite notification slots, bounded progress | Exact grammar-derived sizes; owner review |
| R2-03 partial revocation | Owner-local linearization and irrevocable prefixes; abandon partial DATA | Security acceptance of distributed observation bound |
| R2-04 generations | Registered tuple equality, nonreuse; no global resolver freshness | Wire representations |
| R2-05 ACK | Distinct claim/write/receipt and explicit CONTROL TLS-ready barrier | Encoding and negative execution |
| Fence gap | Four role/direction prefix fences + matching downstream ACKs | Numeric counter/enum/grammar allocation |
| Terminal/retention | TLS subfact vs accepted operation terminal; CONTROL final summary | Exact cause/action/disposition schema |
| Spec governance | ADR-linked progression and consistency gate | Central global principle acceptance |

No accepted normative conflict was established. Semantic trade-offs are explicit
candidate decisions. Full wire layouts/IDs/security parameters are unapproved;
do not present this as an implementable peer specification or wire acceptance.


## Current lifecycle checkpoint 2026-10-12

PARTIALLY_ABSORBED. Local semantic/encoding decisions are absorbed by [ADR-0034](../adr/ADR-0034-pacc-c2-revision3-contract-and-separated-gates.md), [specification1.0](../protocol/pacc-c2-specification-revision-1.0.md) and its registry/freeze. The proposal body is historical provenance, not competing approval authority. Remaining future direction is the central ecosystem principle in R3-I11 and its separate global proposal, plus separately governed public API/package/target and implementation progression; none is accepted globally here. Current local status/gates: [live matrix](../capabilities/pacc-c2-normative-approval-and-gates-20261012.md).
