# C2 revision3 exact encoding and security owner matrix
2026-10-11. DOCUMENTARY REVIEW; PROPOSED encoding1, not approved/implemented/qualified.

Read authority chain: AGENTS, accepted ADR-0022/0029, existing PACC Envelope/CONTROL/DATA/capability/security/Principal/Legacy contracts, preserved semantic candidate/R3-I01-I12, revision2 closeout, draft0.2, CG01-CG22 and wire-approval handoff. Sources and complete hashes are in the companion audit. No accepted ADR or original proposal was rewritten.

## Owner decision matrix
| ID | Decision candidate | Existing constraint / traceability | Required decision |
| --- | --- | --- | --- |
| ED01 | Capability3 major1, messages0x0010..0x001B | Existing registry1..15, keys1/2; R3-I10, CG19/20 | PACC owner allocate, verify external registry and API/package revision |
| ED02 | Envelope/CONTROL1.0 unchanged, old offer28, result14 for two keys | Existing codec, ADR12/16/18; R3-I01/02 | Joint owner approve dedicated two-key Session and bounded composition |
| ED03 | Explicit integer/byte schemas, no TLV/padding/flags | ADR25 portable public codecs; CG03/04/19/20 | Encoding approval includes exact layouts and all enum values |
| ED04 | DNS ASCII/IPv4 only first major; original/canonical equality, server TLS catalog | ADR22/29 ownership and exact snapshot; CG07/08/17 | Accept limited coverage; C0 all-candidate policy still must be implemented/qualified |
| ED05 | Nonzero U64 protocol-only identity/epoch and local U32 provenance | R3-I04; no private manager IDs | Owner approve scopes, exhaustion and nonreuse/callback fences |
| ED06 | Independent32-byte OS CSPRNG role tokens, comparison, fixed negative work | ADR29 unpredictable authority, Secure/Legacy; CG07/08/18 | Security owner approve entropy source, comparison, deletion and rate policy before implementation |
| ED07 | Both role ACK receipts/writes and explicit factual TLS_READY | R3-I05; CG09/10 | Public SDK fact availability and consumer activation validation; missing fact blocks implementation |
| ED08 | Four directional offset/fence/ACK prefixes and downstream definitions | R3-I06; CG11/12 | Approve counters/fence grammar; bounded TLS closure drainage must be demonstrated |
| ED09 | LOCAL revocation, irretrievable accepted prefix; partial frame abandonment | R3-I03; CG05/06 | Security owner accept distributed observation limit, test every byte boundary |
| ED10 | First fact/action/ownership separate; CONTROL FINAL after owner facts | R3-I07, familyADR13; CG13/14 | Approve server-summary scope vs Browser local destruction, no global attestation |
| ED11 | Exact CONTROL staging/notification bounds; fixed per-role staging | ADR29 lab caps; R3-I02/08, CG03/04/15/16 | Approve candidate lab limits; measured total metadata/PST/kernel cap remains implementation gate |
| ED12 | No remote NGE requirement, no privileged TLS trust from request | ADR22/29, R3-I09; CG17/18 | Site TLS and destination authorization independent from Network Egress |
| ED13 | Lower-assurance explicit Legacy D9A only, no production/fallback | ADR1/28/29; CG17/18 | No Legacy production authority; separate execution gate still required |
| ED14 | Spec/codecs/tests/ADR match exact approved hashes | Local specification gate, R3-I11/12 CG21/22 | Owner accept canonical ADR and encoding; separate code/C7 authority |

All ED rows remain OWNER_APPROVAL_PENDING. Semantic candidate decisions are reconciled as proposals, not already accepted C2 refinements. Existing accepted ownership/security constraints remain authoritative. No new normative conflict established; proposed restrictions must be accepted explicitly.

## Threat matrix and planned conformance
Each row requires positive valid operation, negative mutation, identity-scoped zero unauthorized effect and applicable owner lifecycle negatives. These are PLANNED, NOT EXECUTED. Preserve original P01-P06/N01-N24/R2-T01-T10 and CG01-CG22; EW identifiers add encoding-specific coverage, never replace old oracles.

| Case | Attack / first guarded boundary | Required negative / oracle | Remaining assurance |
| --- | --- | --- | --- |
| EW01 | Cross-operation binding at BIND | Swap op/gen or token between two live ops; no consume/claim/victim teardown | Token secrecy relies on Secure; Legacy on-path not protected |
| EW02 | Cross-Session/Principal at existing attach and role commit | Same Principal different CONTROL, different enrolled Principal, wrong profile; preserve legitimate token | Physical socket/destination not cryptographically attested |
| EW03 | Stale generation/replay at all frame and callback boundaries | Old BIND/ACK/DATA/CANCEL/FINAL and generation wrap; no reuse/resurrection | Connection-incarnation binding, no reconnect recovery |
| EW04 | Downgrade/missing grant before side effects | Old peer omits key3, incompatible major, forged grants, third offer key, TLS failure | No Secure→Legacy or placement fallback |
| EW05 | Destination substitution at OPEN/ready/connect | Wrong original/canonical/IP/port/provenance/profile; no second DNS/public Accelerator dial | Server trusts authenticated authorized Browser facts, not DNS attestation |
| EW06 | Partial revocation before each bounded send/delivery | Cancel at every header/payload byte, bytes accepted before revoke preserved; no newly submitted payload | Remote notification latency and escaped bytes unavoidable |
| EW07 | CONTROL starvation/queue exhaustion at admission/scheduler | Full four queues, stalled normal CONTROL, cancellation/final reserves, saturated metadata/retained owners | Numeric wire caps alone not total memory/kernel proof |
| EW08 | DATA-before-activation/TLS-ready | One missing/partial ACK, early ready, unverified TLS, lost TLS_READY, early APP | No queued APP accepted as future-authorized plaintext |
| EW09 | Late callback after Stop/release | Token/generation/owner mismatch checked before dereference; repeated local Stop | No callback count substitutes for lifetime identity |
| EW10 | CONTROL/DATA loss and summary race | Lost CONTROL cascades; partial FINAL no success; sibling loss preserves unrelated Session | Peer may have unknown terminal, no status-query retry |
| EW11 | Prefix/fence spoof and duplicate | Wrong offset/count/role/direction, overflow, payload after seal, partial ACK, duplicate FENCE | ACK only stated downstream accepted boundary |
| EW12 | Raw EOF/reset vs TLS closure | Factual RECORD_EVENT distinct; no fabricated CLOSED/close_notify, failed drain non-clean | TLS-truncation application exception separate from transport success |
| EW13 | Release/lifetime retention | Observe attempt/result/info/disposition before cleanup, PRECONDITION/UNRESOLVED, retained manager/provider/runtime | Exactly-once per lifetime, no alternate close or forced deadline cleanup |
| EW14 | Token guessing/entropy/candidate work | Gate no RNG, all-zero/duplicate, wrong token, consumed replay, 9th failed candidate/window, second role unaffected | Thresholds lab candidate, not a production DoS proof |
| EW15 | Malformed encoding | Every message min-1/max+1, reserved/enum/flags, wrong sender/channel, lengths/trailing/count arithmetic | Reject before allocating/staging side effects |
| EW16 | Portable compatibility | Old/new both directions, independent peer, C89/VC6 U64 limbs, unaligned buffers, real NT4 | Not executed; modern Python document vectors are not codec qualification |

Security review is complete at contract/proposal level; implementation security evidence is absent. Secure requires TLS1.3 mTLS/ALPN, Principal equality and Session/current policy in addition to token. It does not protect against an authorized malicious Browser lying about its physical socket. Legacy exposes both role tokens and APPLICATION plaintext to on-path attackers and cannot claim anti-replay/integrity/confidentiality against them. Site TLS does not repair this first-hop gap. No Legacy production admission is authorized by ED approval.

## Rejected alternatives
| Alternative | Rejection reason |
| --- | --- |
| Reuse NETWORK messages/flags for C2 | Changes old semantics and hidden version authority |
| Three offer keys or universal oversized CONTROL parser | Violates existing two-key/28-byte negotiated reader assumptions |
| DATA ticket/counter as role secret | Predictable and not per-role authority |
| Client-selected arbitrary CA/runtime/interface | Privilege escalation and destination/trust substitution |
| ACK as TLS-ready or TCP receipt | Factual boundaries differ |
| CONTROL cancel as global instantaneous byte fence | Independent streams, irreversible accepted bytes |
| Global counters as lifetime proof | Attempt/disposition/destruction and identities differ |
| Drain forever/force close/evict retained | Violates bounded lifecycle and Model E |
| Skip unknown fields/TLV without major | Makes old peer behavior and validation ambiguous |
| Legacy cryptographic parity or automatic retry | No protected first hop; may replay/rebind operations |

## Implementation plan after explicit approval
1. Owner accepts exact semantic3/encoding1/spec0.3 hashes and allocates canonical ADR/registry; decides public package/API metadata. Approval alone does not authorize implementation.
2. Separately authorized portable codecs use caller-owned bounded buffers and limb arithmetic; old codec goldens remain untouched. Generate min/max/malformed fixtures from approved schema, not this candidate as an unreviewed oracle.
3. C2 consumer admission/roles/queues/authority integrates one owner and public PST; prove public verification facts, fixed allocation caps, token entropy, tuple policy and generation fences before activation.
4. C3 public PST socket attach/private exact loopback bridge separately reviewed; no public site dial on Accelerator.
5. Execute positively/negatively all original/CG/EW required tests only when authorized. Then native per-lifetime teardown, compatibility/independent peer and actual NT4. C7 E1-E12 remains a separate human execution gate.

## Documentary validation and status
Full baseline hashes cover449 Accelerator,3142 Browser and1566 isolated-source visible files; normal SDK53 and C1 seven hashes reverified, seven existing shared codecs byte-identical. Current Accelerator main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Existing dirty work preserved. Candidate bytes are checked for header lengths/schema widths/offsets and represent documented fixtures only, not an operational PASS.

Current deliverables: [spec0.3](../protocol/pacc-c2-protocol-specification-draft-revision-0.3.md), [layout registry](../protocol/pacc-c2-exact-wire-layouts.json), [vectors](../protocol/pacc-c2-exact-wire-candidate-vectors.json), [encoding ADR candidate](../adr/ADR-CANDIDATE-pacc-c2-exact-encoding-and-security.md), [owner handoff](../integration/pacc-c2-exact-wire-security-owner-handoff.md), companion audit JSON. Previous drafts/reports remain byte-exact historical evidence.

CURRENT_TASK_COMPLETED=YES
SLICE_COMPLETED=YES
C2_REVISION=3
WIRE_ENCODING_PROPOSAL=COMPLETE_CANDIDATE_1_NOT_APPROVED
WIRE_IDS_COLLISION_FREE=YES_AGAINST_CAPTURED_LOCAL_REGISTRY_NOT_ALLOCATED
SECURITY_REVIEW=DOCUMENTARY_COMPLETE_IMPLEMENTATION_NOT_QUALIFIED
SECURE_PRINCIPAL=TLS13_MTLS_PRINCIPAL_SESSION_ROLE_POLICY_REQUIRED
LEGACY=LOWER_ASSURANCE_LAB_ONLY_PRODUCTION_NOT_AUTHORIZED
OLD_PEER_COMPATIBILITY=PROPOSED_UNCHANGED_OLD_BYTES_RUNTIME_NOT_EXECUTED
PACC_SPECIFICATION=DRAFT_0_3_WITH_CANDIDATE_LAYOUTS_AND_BYTES
ADR_STATUS=PROPOSED
OWNER_DECISIONS_REQUIRED=ED01_TO_ED14
C2_IMPLEMENTATION_AUTHORIZED=NO
C3_IMPLEMENTATION_AUTHORIZED=NO
C0_STATUS=PRESERVED_QUALIFIED_FIRST_IPV4_SCOPE_NOT_FULL_POLICY
C1_STATUS=PRESERVED_QUALIFIED_INTERNAL_CURRENT_HOST_NOT_NT4_OR_MODE_C
PST_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
FIRST_UNRESOLVED_DIVERGENCE=OWNER_APPROVAL_AND_IMPLEMENTATION_GATES_PENDING
NEXT=OWNER_APPROVAL_OF_EXACT_C2_ENCODING_AND_SECURITY_CONTRACT
