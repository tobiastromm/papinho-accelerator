# PACC C2 revision 1 — joint contract review

Date: 2026-10-11. Status: review completed; **final C2 wire approval NOT READY**.
Task: PAPINHOACCELERATOR-PAPINHOBROWSER-C2-PACC-VERSIONED-CONTRACT-JOINT-REVIEW.
This is documentation/evidence reconciliation, not owner acceptance of C2.

## Decision and authoritative inputs

C2_CONTRACT_ACCEPTABLE=NO for final versioned wire approval.
The semantic direction TWO_ROLES_ONE_OPERATION is coherent with accepted
ADR-0022/0029; no replacement design is proposed. C2 revision1 is preserved.
[Revision2 proposed](../future/pacc-c2-versioned-contract-proposal-revision-2.txt)
retains sections1-10 with a minimal joint-review addendum. No registry allocation,
encoding, C2/C3 implementation, production Legacy or E1–E12 is authorized.

Actual input root is C:/Projetos/_tmp/PapinhoBrowser/model-e-implementation-20261011.
The supplied C:/Projetos_tmp path does not exist; the evidence explicitly resolves
that shorthand. Proposal SHA256:
E69C822159FC0070AF6EC8C64419108B4342ADC63EB4E5722AFB3A36F131E9F9.
All exact input/output hashes and preservation checks are in the companion JSON.

Authorities read include ADR-0022 revision1; ADR-0029 revision1; Browser
ADR-0007 revision4 and recorded subsequent decisions; normative framing/
CONTROL/DATA specifications; ADR-0016/0018/0025; Secure Principal ADR-0001/0006,
security model, Secure DATA binding source and Legacy contracts;
family ADR-0013 accepted revision3; C0 and later C1 evidence/handoffs.
Historical proposed passages below the family ADR's acceptance record are
history, not current proposed status. Future directions do not override ADRs.

Current Accelerator and Browser branches are main, respectively HEAD
c50b8fb9a7e3293b5e6355fc08459bf85b56857a and
1eb028554f622672377953225e162ed9b6564a40. Dirty trees preserved.
Isolated source has no .git; no isolated HEAD or git diff claim.

## Preserved qualifications and factual protocol

Seven C1 source/build/binary hashes match c1-qualification-20261011-222128/hashes.json.
Current qualified Browser binary is
95467625A9DAAED0CA31B55BE4E0BBF85CFDEE9ADB68BD589108D2F85B96DA51.
C1 is an internal raw TCP component qualified on current Windows host, not
productive Mode C, Browser navigation integration or real NT4 acceptance.
Its later evidence supersedes historical NOT_IMPLEMENTED statements only
factually. C0 remains qualified for blocking FIRST_IPV4/snapshot/exact-connect
scope, not full normalization/all-candidate policy.

Normal Browser SDK0.6.5/API3/SPI4 VC6 x86 /MD: SHA256SUMS53/53 match.
Prior normal Model E and separate governed TEST_ONLY native-retention evidence
are preserved for their executed scopes; test-only evidence is not normal-SDK
native-failure/destruction proof. No qualified test or suite was rerun.

Public PACC API/package1.1.0, external contract generation2, Envelope1.0,
CONTROL1.0. Seven producer codecs match active Browser vendored bytes.
Existing IDs1-15 remain unchanged. Older overview/framing historical registry
summaries ending at0x000E and historical security pin0.6.1 are stale factual
summaries, not authority to erase accepted type0x000F or change current pin0.6.4.

## Review matrix

| Area | Revision1 / governing boundary | Joint finding / disposition |
| --- | --- | --- |
| Placement/ownership | ADR-0022/0029: Browser TCP, Accelerator Site TLS | CONSISTENT; no public site TCP on Accelerator, no socket transfer |
| Service version | Proposed distinct operation key plus TLS_OFFLOAD | OWNER DECISION; documentary name/revision not allocated wire ID/major |
| Offer size |2 keys/set; current CONTROL payload28 |28-byte offer compatible in size; new messages need separate bounds |
| Session compatibility | Offer omits NETWORK_EGRESS | R2-01: Mode B cannot share that grant snapshot; owner must define Session composition |
| Old/new peers | Unknown valid key omitted; no unnegotiated frames | CONSISTENT; no implicit probe/downgrade/renegotiation; old goldens required |
| Metadata vs wire | API/package/generation separate | CONSISTENT; none can authorize decoder activation |
| Message encoding | IDs/layouts/lengths deferred | BLOCKER before wire approval; R2-06 complete field worksheet |
| Required/optional fields | Semantic tuple exists, presence grammar absent | BLOCKER; no guessed TLV, skip-unknown or trailing-field acceptance |
| Framing | Envelope16/big-endian/flags0; full frame before commit | CONSISTENT; preserve zero-payload completion and buffered reader handoff |
| CONTROL dispatch | Current post-control reader max28,2 keys/set | R2-02: additive new grammar needs explicit negotiated states/per-type limits |
| Operation/stream identity | Server incarnation separate from provenance | CONSISTENT with R2-04; widths/scopes/nonreuse still owner encoding gate |
| Resolution generation | Local scope, no remote attestation | R2-04 clarifies N03; cannot claim globally ordered freshness |
| Two roles/atomic activation | Explicit APPLICATION/RECORD | CONSISTENT; not two NETWORK_OPENs; role claim/ACK/site-ready gates distinct |
| ACK and TLS-ready | Claim, publication and observation differ | R2-05; partial ACK/TLS-ready never enables APPLICATION |
| Entropy/attachment | Separate single-use role secrets, no counter fallback | CONSISTENT direction; length/provider/construction/comparison/limits need security signoff |
| Ordering/fences | Same-role prefix, no global CONTROL/DATA order | BLOCKER: exact representation/receipt and incomplete-fence rules pending |
| Backpressure |4 queues; native DATA pressure initially | CONSISTENT; total staging/control/retention admission not only512KiB payload |
| Cancel/revocation | No new delivery after revocation | R2-03: partial DATA frame abandonment/escaped prefix; no contradictory finish-after-revoke rule |
| Deadlines | Local monotonic, no peer wall clock | CONSISTENT; finite budget/start/expiry rules required, lab values not production |
| Terminal fact/action | First valid accepted cause immutable | CONSISTENT; explicit cause/action grammar and duplicate/error scope pending |
| Model E/exactly-once | OK+DESTROYED; retained owners remain | CONSISTENT; deadline not ownership release; per-lifetime identity required |
| Secure Principal | Each DATA mTLS, Principal + live Session + grants | CONSISTENT; equality alone cannot authorize another Session's operation |
| Legacy | Lower assurance, explicit server admission/D9A lab | CONSISTENT; D9B production remains pending, tokens are on-path observable |
| Portability | Protocol-only byte codecs, C89/VC6 | FEASIBLE DESIGN; no struct serialization; actual C2 portability/NT4 untested |
| Human specification | Added explicit owner requirement in this task | RECORDED; no protocol qualification without spec/encoding/test consistency |

No existing accepted-contract contradiction requiring a new design was found.
These are proposed clarification/approval gaps; no runtime C2 defect is claimed.

## Minimal differences and security risks

R2-01–R2-08 clarify Session scope, CONTROL dispatch, revocation prefixes,
local-vs-server generations, complete acknowledgements/activation, complete
encoding worksheet, admission/security accounting and the human specification.
They preserve the original service/role architecture and P01-P06/N01-N24 oracles.

| Threat | Existing/proposed safeguard | Remaining approval/evidence |
| --- | --- | --- |
| Cross-principal attachment | Existing independently authenticated DATA and Principal match before structural commit | New role/incarnation and same-Principal/different-Session negatives |
| Replay/stale binding | Per-role secret, irreversible consumption, expiry, nonreuse | Secret construction/width/lifetime and exact generation comparison |
| Destination substitution | Immutable hostname/numeric tuple and local exact connect | Normalization/all-candidate authorization; no physical remote attestation claim |
| Downgrade | No unnegotiated frames, Secure-to-Legacy or route fallback | Old/new both ways and mixed Session policy |
| Guessed victim operation | Invalid secret/ID rejected at candidate scope | Bound lookup/work/response rate and disclosure; no victim abort |
| Resource exhaustion | Fixed reservations/queues, retained owners charged | Full budget accounting, saturation/overflow and no eviction |
| Revocation during write | Owner revalidation and immutable first fact | Exact partial-frame/prefix/fence rules at every interleaving |
| CONTROL loss | Session-wide existing cascade | Role/bridge teardown once; other Sessions unaffected |
| DATA loss | Operation+sibling scope | Other operations remain independent |
| ACK loss | Committed authority not restored | ACK complete-write vs peer validation tests |
| Late callback | Identity/generation check before dereference | Quiescence/tombstone/nonreuse proof and manager retention |
| Native close failure | Model E terminal retention, no alternate close | Per-lifetime normal/test-only evidence kept separate |
| Legacy on-path attacker | Explicit lower assurance/isolation/warnings | Cannot be solved by tokens; D9B/C7 gates remain separate |

SECURITY_GAPS are approval/evidence gaps: token entropy/format/provider,
bounded invalid attempts/control buffers/retained resources, exact publication
revocation/fences, generation scope and approved Legacy admission. They are
not proof of exploitation or a request to modify PST.

## Positive/negative test matrix and evidence expectations

The preserved full P01-P06/N01-N24 matrix is reproduced in revision2 section9.
ALL C2 tests listed here are PLANNED_NOT_EXECUTED. This review does not run C7.

| Original tests | Boundary / exact oracle |
| --- | --- |
| P01,P02 | One authorized operation, distinct roles, exact endpoint, verified TLS; sequential DATA tickets preserve CONTROL |
| P03,P04 | Byte-by-byte/coalesced frames, partial I/O, exact prefixes, bounded queues, no early activation |
| P05 | Ordered end/fences, factual PST completion, one accepted terminal and per-resource finalization |
| P06,N01 | Old/new both ways; absent key/unoffered grant/oversize rejects before C2 effects |
| N02–N07 | Principal/profile/Session/generation/token/tuple/epoch mismatches and commit revocation; no cross-bind/data |
| N08,N09 | Missing readiness/role, partial ACK, loss after token consume; no application exposure or token restoration |
| N10,N15,N24 | Saturation/cancel and CONTROL ahead of DATA; escaped prefixes separate, no invented receipt/resync |
| N11,N12 | CONTROL cascade vs DATA operation+sibling; healthy other Session/operation preserved |
| N13,N14 | EOF/RST/half-close factual; complete/incomplete HTTP orthogonal to explicit TLS_TRUNCATED |
| N16,N17 | Duplicate/conflicting/late frames/callbacks; first valid fact, no resurrection/use-after-release |
| N18–N21 | Monotonic wrap/deadline, retention, cap/overflow and missing entropy; no timeout PASS/native fallback |
| N22,N23 | No unapproved Legacy enablement; Mode C needs no remote NETWORK_EGRESS or public site dial |
| R2-T01,T02 | Mixed Session offer effects and negotiated per-state28/new limits |
| R2-T03,T04 | Revocation at each byte offset and local-resolution scope vs reservation binding |
| R2-T05,T06 | Partial bind/TLS-ready publication, guessed-ID isolation and bounded response |
| R2-T07,T08 | Total/retained capacity and exact optional/unknown/text/arithmetic grammar |
| R2-T09,T10 | Same Principal different CONTROL and specification/independent peer/codec byte agreement |

Every future execution records operation/Session/incarnation/role/epoch,
accepted/submitted/received/delivered counts, native ownership identities,
queue peaks, exact terminal cause/action, PST result/disposition and retained
dependencies. Generic exit0/global counters cannot replace those facts.
Public protocol vectors are separate from native lifetime and real NT4 tests.

## Proposed decision and C2/C3 implementation plan

Recommendation: retain TWO_ROLES_ONE_OPERATION direction and revision2 as
PROPOSED. Do not mark the versioned wire accepted until all encoding/boundary
decisions and security signoffs are complete. There is no complete accepted
C2 ADR to prepare yet; this review/addendum is the decision candidate.
The only accepted new requirement recorded now is the human-specification gate
explicitly supplied by the owner, not acceptance of C2 semantics or bytes.

1. Joint owners settle service choice, Session composition, authority construction,
   delivery revocation, ACK/TLS-ready, fences and complete field/budget tables.
2. PACC owner approves IDs/major/exact encoding and compatible API/package
   metadata. Fill human specification and approved golden/malformed vectors.
3. Separate authorized codec task changes only canonical protocol package,
   with old/new compatibility and independent peer specification read-through.
4. Separately authorize C2 consumers: operation registry, two DATA role owners,
   C0/C1 authority/endpoint propagation, bounded dispatch and late callbacks.
5. Separately authorize C3: one-shot private IPv4 loopback exact-tuple pair,
   close listener after one accept; transfer endpoint using public PST ownership
   contract; remove readiness before Model E eligible finalization. No site dial.
6. Qualify impacted owners/retention/cancellation/budgets/security, build C6
   deterministic fixtures, seek human approval before C7 E1-E12.
7. C8 productive Secure, accepted C9/D9B before C10/C11 Legacy, actual C12 NT4
   remain later gates. No PST source/API/SPI or production pin change.

## Human specification deliverable and gate

[Protocol specification draft](../protocol/pacc-c2-protocol-specification-draft.md)
contains defined requirement language, glossary, existing framing/registry,
future exact-field worksheet, proposed sequences/state machines, compatibility,
security, error/ordering/budget/Model E clauses and conformance mapping.
It includes approved existing CONTROL/DATA vectors only; C2 vectors remain TBD
until encoding approval. A draft with those TBDs cannot implement an independent
interoperable peer yet and cannot qualify C2.

[Accepted documentation requirement](../pacc-c2-specification-requirements.md):
NO_C2_PROTOCOL_QUALIFICATION_WITHOUT_SPECIFICATION_ENCODING_AND_TEST_CONSISTENCY.
It derives from the explicit user section10; no existing accepted ADR changed.

## Validation and result

Read/hash/document checks only; no builds, runtime tests, Browser launch,
E1-E12, HKCU/config/credential changes, native cleanup or network sends.
Functional Browser/Accelerator/PST/PACC unchanged. C0/C1 preserved for named
qualification scope; real NT4 remains NOT_RUN. Original proposal/evidence,
qualified SDK/source/binary hashes and WIP preserved. Full before/after audit
and output SHA256 are in the companion JSON; new-file whitespace checks pass.

CURRENT_TASK_COMPLETED=YES
SLICE_COMPLETED=YES
C2_CONTRACT_STATUS=PROPOSED_REVISION_2_FINAL_WIRE_APPROVAL_NOT_READY
PACC_VERSIONING=VERSIONED_EXTENSION_REQUIRED_SERVICE_ALLOCATION_PENDING
WIRE_ENCODING_READY=NO
SECURE_SECURITY_REVIEW=SEMANTICALLY_ALIGNED_SIGNOFF_AND_ENCODING_GAPS_RECORDED
LEGACY_SECURITY_REVIEW=LOWER_ASSURANCE_EXPLICIT_PRODUCTION_NOT_AUTHORIZED
C2_IMPLEMENTATION_AUTHORIZED=NO
C3_IMPLEMENTATION_AUTHORIZED=NO
C0_STATUS=PRESERVED_QUALIFIED_FOR_SCOPE
C1_STATUS=PRESERVED_QUALIFIED_INTERNAL_CURRENT_HOST
PST_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
FIRST_UNRESOLVED_DIVERGENCE=SESSION_COMPOSITION_AND_WIRE_BOUNDARIES_NOT_APPROVED
NEXT=OWNER_REVIEW_OF_REVISION_2_THEN_PACC_ENCODING_SECURITY_APPROVAL
