# C2 revision3 conformance and encoding plan

Date: 2026-10-11. Status: PROPOSED, ALL NEW RUNTIME/CODEC TESTS NOT EXECUTED.
Authority candidate: [ADR proposal](../adr/ADR-CANDIDATE-pacc-c2-operation-and-progressive-specification.md);
[contract revision3](../future/pacc-c2-versioned-contract-proposal-revision-3.md);
[protocol draft0.2](../protocol/pacc-c2-protocol-specification-draft-revision-0.2.md).
CG identifiers are documentary test IDs, not message allocations. Original
proposal P01-P06/N01-N24 and R2-T01-T10 remain required, not replaced/weakened.

## Traceable invariants and planned positive/negative oracles

| ID | Candidate / spec clause | Positive | Negative / exact rejection | Existing planned mapping |
| --- | --- | --- | --- | --- |
| CG01 | R3-I01 / spec4,14.1 | Two independently granted Sessions under one owner; old Mode B continues | C2 missing NETWORK_EGRESS cannot perform Mode B; no silent key addition | P06,N01,R2-T01 |
| CG02 | R3-I01 / spec4,14.1 | C2 CONTROL loss leaves independent old Session eligible | Cross-Session role attach even same Principal; duplicate runtime/owner admission | N02,N11,R2-T09 |
| CG03 | R3-I02 / spec5,6,14 | Old offer28 and new exact M(state,type) | M+1/undefined maximum/unnegotiated new type before allocation | N01,N20,R2-T02,T08 |
| CG04 | R3-I02 / spec10,14 | Reserved cancellation/final slots serviced boundedly | Saturated CONTROL/partial unrelated frame; no splice, unbounded queue or deadline refresh | P04,N10,N18,N24 |
| CG05 | R3-I03 / spec10,14.2 | Exact accepted/escaped prefix captured at local revoke | Revoke at every DATA write offset; no new submission after local transition; incomplete frame abandoned | N07,N10,N24,R2-T03 |
| CG06 | R3-I03 / spec14.2 | Receiver guards at its own observed revocation | Delayed CONTROL relative to DATA; no global instantaneous-revocation claim, no stale UI publication | N15,N17,R2-T03 |
| CG07 | R3-I04 / spec3,11 | Correct Principal/Session/incarnation/role/tuple/epoch binds once | Wrong token/guessed ID cannot abort victim; entropy failure denies | N02-N07,N21,R2-T06,T09 |
| CG08 | R3-I04 / spec3,11 | Local snapshot equality and fresh operation | Global generation inference, replay/restored token, substituted host/IP/port, early ID reuse | N03-N06,N16,N19,R2-T04 |
| CG09 | R3-I05 / spec7,9 | Both full role ACKs and exact site-ready precede TLS progress | Partial/queued/lost role ACK; token remains consumed; no early activation | P02,P03,N08,N09,R2-T05 |
| CG10 | R3-I05 / spec7,14 | Factual verified TLS plus full matching result precedes APPLICATION | Wrong generation/partial result/verification failure exposes no plaintext | P01,N08,N17,R2-T05 |
| CG11 | R3-I06 / spec14.3 | Four exact role/direction fences match downstream accepted prefixes | Fence reordered/missing/truncated/duplicate/wrong count/role/direction/generation; no clean completion | P03,P05,N13,N15,N16 |
| CG12 | R3-I06 / spec14.3 | Independent reverse traffic/role-control ACK after forward seal | Payload after seal, integer exhaustion, fake remote receipt or PST-internal consumption | P04,N13,N20,R2-T08 |
| CG13 | R3-I07 / spec8,9,14 | TLS subfact retained; all clean preconditions satisfied then one CONTROL outcome | Missing fence, loss/deadline/retention or lost final summary never yields inferred success | P05,N11-N16,N18,N19 |
| CG14 | R3-I07 / spec8,10 | DESTROYED exactly once; governed eligible PRECONDITION resolution | CLOSE_UNRESOLVED release retry/alternate close/reuse/borrowed-runtime destruction denied | N16,N17,N19,R2-T07 |
| CG15 | R3-I08 / spec10 | High/low interest removal/resume, exact partial counts | Full queue cancel no indefinite drain; role starvation/wake storm no deadline extension | P04,N10,N18 |
| CG16 | R3-I08 / spec10 | Admission charges payload+staging+CONTROL+retained owners | Cap exhaustion/overflow/tombstone eviction/retained-slot theft denies before work | N19,N20,R2-T07 |
| CG17 | R3-I09 / spec2,11 | Mode C authorized with TLS/operation but no remote egress grant | Accelerator public site connect, privileged trust override or IP-as-name prohibited | P01,N06,N23 |
| CG18 | R3-I09 / spec11 | Explicit scoped Legacy lab grant with lower assurance | Unapproved Legacy production, Secure downgrade or on-path assurance claim rejected | N02,N22 |
| CG19 | R3-I10 / spec4-6 | Old/new both ways preserve exact old goldens | Unknown versions, unoffered grants, reserved flags or package-only dispatch fail | P06,N01,R2-T02,T08 |
| CG20 | R3-I10 / spec5,6,12 | Approved byte codecs independent of native ABI | Packing/endian/length/optional/unknown/duplicate/trailing fields tested exactly | P03,N20,R2-T08,T10 |
| CG21 | R3-I11 / spec14.4 | Every accepted decision maps to spec/diagram/vector/test revision | Stale diagram/vector/codec hash or only implementation docs blocks qualification | R2-T10 |
| CG22 | R3-I12 / spec14.5 | Separate semantic/security/encoding/implementation/execution decisions | C1 or Model E PASS used to enable C2/C3/C7 or global accepted policy rejected | All gate prerequisites |

For each future run record Session/incarnation/operation/generation/role/direction,
current authority/epoch, accepted/submitted/validated/downstream counts, fence
identity, deadline start/expiry, full admission/queue peaks, PST result/release
disposition and physical lifetime. Sender writer completion is not peer receipt.
Separate application framing, operation terminal, security action and destruction.

## Encoding approval worksheet and codec plan

No IDs/layouts are selected here. PACC owner must approve every item below:

| Decision group | Required output before codec work |
| --- | --- |
| Registry/version | Operation capability ID/major, message/role enums, service compatibility, cumulative package/API identity |
| Binding fields | Correlation/incarnation/generation scopes, byte widths, invalid/nonreuse/exhaustion rules, local-only fields |
| Secret | Approved entropy/provider/construction/length/comparison, exposure/rate/work/expiry behavior |
| Tuple/TLS | Host text/canonicalization, numeric family/address/port, optionality, bounded ALPN/profile inputs and rules |
| CONTROL grammar | Each request/result/ready/TLS-ready/final message's channel/direction/required fields/exact size |
| DATA grammar | Bind/ACK/payload/fence/fence-ACK/direction facts, sealed direction restrictions, per-type lengths |
| Errors/outcome | Separate factual cause/action/ownership disposition; unknown values, scope/disclosure/no downgrade |
| Limits | M(state,type), parser scratch/staging, reserved CONTROL slots, token/authority/tombstone accounting |
| Optional extensions | Explicit presence/order/unknown/duplicate/trailing policy; no implicit TLV |
| Goldens | Valid minimal/maximal, partial/coalesced and every invalid boundary; old IDs1-15 unchanged |
| Portable API | C89/VC6 byte routines, caller capacities, checked arithmetic, no PST/Win32/runtime in package |

CONTROL candidate accounting is finite, not a frozen new byte limit:
reserved bytes = current in-flight header16+approved maximum payload
+ admitted operations*(one cancel header16+approved cancel maximum
+ one final-outcome header16+approved final maximum)
+ declared parser/decoder/writer scratch/staging.
Coalescing internal intents does not remove mandatory facts or permit duplicate
remote frames. All coefficients/maximums must be checked against resource policy
and target integer capacity before admission. Unknown maximum => no C2 admission.

Implementation order AFTER separate authorization:
1. Approved protocol revision/registry/grammar/security and human spec tables.
2. Canonical portable codecs/validators and independent vector derivation.
3. Old/new compatibility and malformed/fragmented state coverage.
4. Governed immutable package extraction with source/artifact hashes.
5. Consumer operation/role coordinator and C3 private socket owner separately.
6. Native ownership/resource/backpressure/target proof, C6 fixtures, human C7 gate.

Spec/encoding/test consistency currently PASS only for documentary cross-links,
scope labels and planned mapping. Actual C2 encoding, goldens/codecs,
independent-peer interoperability and runtime conformance remain NOT_EXECUTED/
NOT_READY. No E1-E12, build or previously valid qualification was rerun.
