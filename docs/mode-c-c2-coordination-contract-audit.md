# Mode C / C2 coordination contract audit

Status: audit and integration design only  
Date: 2026-10-07  
Task: `PACC-MODE-C-C2-COORDINATION-CONTRACT-AUDIT-1`

This report authorizes no experiment, implementation, production, API/SPI/wire,
package, or release change. `PROPOSED` always requires owner approval and the
applicable governance step.

## Baseline and evidence

Accelerator: branch `main`, HEAD
`c50b8fb9a7e3293b5e6355fc08459bf85b56857a`. The worktree was already heavily
modified, including untracked Phase 6 code, ADRs, docs and tests. It was
preserved: no reset, stash, clean, checkout, commit, or push.

Read: `AGENTS.md`; applicable accepted OrganizationEngineering and
PapinhoEngineering ADRs; all accepted local ADRs; related live capability,
security, architecture, protocol, Phase 6 and future documents; current code
and tests; and both required PST reports in full:

- `C:/Projetos/PapinhoSecureTransport/docs/mode-c-byte-transport-feasibility.md`
- `C:/Projetos/PapinhoSecureTransport/docs/mode-c-c2-minimal-experiment-design.md`

The PST feasibility report sections covering the public socket transport,
C1/C2 and security prove only that PST 0.6.4 remains socket-backed, C1 is not
proven, and a private loopback C2 is the smallest experiment without changing
PST. The C2 report's attachment, queues, terminal table and E1--E12 are design,
not executed evidence. It records PST branch
`fix/idle-connection-waitset-close`, abbreviated HEAD `a189379...`, and
published ZIP SHA-256 `26b6afd5...`; a future evidence pack must capture the
full hashes independently.

Accelerator evidence inspected: public codecs in `include/papacc/pacc/`;
Session/Channel managers in `src/runtime/`; DATA association and network state
in `src/protocol/`; `src/network_egress/network_authorization.c` and
`network_stream.*`; Win32 I/O/wait-set composition in `apps/server/`; security
composition in `src/security/`; and related portable, wire/runtime, security
matrix, server and fixture tests. No build or test was run; existing tests are
not reported as a fresh pass.

### Existing documentation divergence

`docs/capabilities/network-egress.md` describes current Phase 6 implementation,
but later still claims no production external connection, Egress Profile or
wire negotiation. That conflicts with its own current-state table and
`docs/phase6-network-tls-offload.md`. This pre-existing live-doc nonconformity
is recorded, not corrected by this report-only task.

## Factual contract matrix

| Area | State | Evidence / owner | Smallest missing contract |
|---|---|---|---|
| Secure Principal authentication | PROVEN | ADR-0001/0006; `src/security/`; Accelerator/PST | none for current Secure profile |
| Legacy assurance | PROVEN lower-assurance | ADR-0001/0009/0028; server listener config | approved Legacy Mode C admission/grants |
| Session and CONTROL ownership | PROVEN | ADR-0003; Session/Channel managers | Mode C operation scoped to CONTROL generation |
| Multiple DATA Channels/Session | PROVEN | Channel and DATA association managers/tests | semantic application/record flow roles |
| DATA attach to Session | PROVEN | 16-byte ticket; inspect/commit in `data_association.c` | operation- and role-bound attach |
| Ticket replay after consume | PROVEN denied | commit clears entry | none for current attach |
| Ticket unpredictability | BLOCKED for secret use | `server_data_ticket_generator.c` is a 128-bit counter | purpose-specific random authority if secrecy is required |
| Wire operation ID | UNKNOWN/ABSENT | ADR-0017/codecs intentionally carry no runtime ID; PapinhoPACC owns spec | profile-aware identity/uniqueness |
| One DATA Channel/Network Stream | PROVEN | ADR-0015/0017; network state/stream | two flows under one operation |
| Cross-operation isolation | PARTIAL | current Session/channel/ticket binding | explicit operation+role rejection matrix |
| Partial framed I/O | PROVEN | reader/writer/codecs/tests | Browser integration evidence |
| Wire credit/window | UNKNOWN/ABSENT | no credit message in registry | decide bounded readiness versus credit |
| Runtime backpressure | PROVEN for Mode B | 1 KiB/direction and 256-byte quantum in server loop/stream | four-queue Mode C policy |
| Fair scheduling | PROVEN for current Streams | ADR-0004; session-aware rotation | schedule two flows as one operation |
| Capability/policy authorization | PROVEN | ADR-0005/0011/0012; capability store/authorization | Mode C grant semantics |
| Destination/DNS policy | PROVEN for Mode B | ADR-0019--0021/0026; egress code | Browser DNS/socket binding for Mode C |
| Cancel/deadline | PARTIAL | Jobs, DATA/Session cascades, PST readiness | cross-component operation state machine |
| Terminal mapping | PARTIAL | ADR-0023; v2 `TLS_TRUNCATED` | Mode C site/PACC/PST mapping |
| Exactly-once cleanup | current managers PROVEN; C2 PROPOSED | manager tests; PST C2 teardown table | cross-component ordering tests |
| Logging/redaction | PROVEN current paths | logging capability/security matrix | non-secret Mode C correlation |
| Wait-set/PST ownership | PROVEN current server | server secure I/O and PST adapter | C2 endpoint registration/lifetime |
| Legacy Network Egress | BLOCKED | ADR-0028 says `LEGACY_ENDPOINT_NETWORK_EGRESS_V1=NO`; authorization requires Secure | accepted decision plus grants/quotas/audit/revocation |

`PACC_MULTIPLEXING=PARTIAL`: multiple physical DATA Channels exist, but there
is no shared Mode C operation or flow role. Current Network Offload cannot be
relabeled Mode C without a new approved contract.

## Proposed two-flow architecture

```text
Browser                                        Accelerator
original hostname + DNS                       destination authorization
site-facing TCP socket / site IP egress       PST Site TLS + name/cert checks

application bytes === PACC APPLICATION FLOW === PST plaintext API
site TCP bytes     === PACC TLS RECORD FLOW ===== private C2/PST socket
```

Browser owns DNS and the site socket. Accelerator owns Site TLS through PST.
A numeric `SOCKET` never crosses machines. Accelerator-owned site TCP would be
Mode B, not Mode C.

The application flow carries only application bytes. The record flow carries
opaque encrypted bytes between the Browser site socket and the private C2 PST
endpoint. They close independently but belong to exactly one operation.

### Proposed binding, not a wire assignment

Both flows need a single-use, unpredictable operation authority bound to one
live Session/CONTROL generation; Secure Principal or explicitly admitted
Legacy context; normalized hostname, port and approved endpoint set; immutable
TLS/trust/ALPN policy; exactly one application role and one record role;
expiry, cancellation generation and resource reservation.

Wrong/expired Session, profile, destination, role, duplicate, replay or
cross-operation attach is rejected before bytes. Activation occurs only after
both roles and the Browser site socket are ready; failure is atomic. Runtime
IDs are not authority tokens.

The current `DATA_TICKET` binds only a Session and its generator is predictable.
Two independent `NETWORK_OPEN` channels have no common operation identity.
Extending DATA attach, adding an establishment exchange, or another bounded
shape are alternatives; none is selected and no wire ID is assigned here.

## Trust and profile matrix

PST Site trust validates the original site. Secure PACC mTLS authenticates the
Browser--Accelerator Session. Legacy PACC is plaintext/lower-assurance. Neither
trust implies the other and failure never selects plaintext, another profile,
destination or local TLS implementation.

| Property | Secure Principal Mode C | Legacy Mode C target |
|---|---|---|
| Functional two flows | target YES | target YES |
| PACC peer authentication | mTLS YES | NO |
| Admission | enrolled Principal | explicit lower-assurance model, undecided |
| Grants/policy/quotas/revocation | server-owned, Principal/Session scoped | server-owned Legacy context; never Browser ACL |
| Destination policy | server-owned | server-owned, at least equally strict |
| Browser UX | Secure + site result | persistent plaintext/lower-assurance warning |
| Deployment | governed Secure profile | explicit isolation and restrictive defaults |
| Prohibited claim | site TLS is not arbitrary egress | no authenticated identity/confidentiality/integrity on first hop |
| Current readiness | contract gaps | BLOCKED by ADR-0028 and missing admission model |

Secure-first is valid minimal-experiment sequencing, not a permanent Secure-only
product decision.

## Private C2 bridge and threat model

Proposed PST-0.6.4 sequence: bind only `127.0.0.1:0`; create/retain connector
and record its exact tuple; connect; accept exactly that loopback peer; close
listener after one accept; transfer one endpoint to PST's public Win32 socket
transport; retain the peer in the bounded bridge wait-set. Any ambiguity fails
closed and cleanup is idempotent.

No wildcard/public listener, firewall exception, inherited/external socket,
destination command, port reuse, tunnel, BadVPN, generic forwarding or open
proxy. Pre-created connector plus exact tuple mitigates local accept races. A
separate random local nonce may add defense in depth; current DATA tickets must
not be reused.

| Threat | Control | State |
|---|---|---|
| remote bridge access | loopback only | PROPOSED, PST design review PASS |
| local accept race | exact pre-created connector tuple, one accept | PROPOSED |
| stale reuse | operation generation, listener closes | PROPOSED |
| arbitrary destination | no destination command; immutable approved operation | PROPOSED |
| cross-operation injection | role-bound authority/endpoints | BLOCKED pending PACC contract |
| exhaustion | global/Session/operation admission and budgets | PROPOSED |
| disclosure | category logs; no token/record/payload | REQUIRED |

Accelerator owns bridge endpoints and PST objects; PST owns provider/TLS
semantics; Browser owns the site socket and its PACC record pump.

## Proposed budgets and backpressure

Measurement seeds, not production limits:

| Limit | Proposal | Rationale |
|---|---:|---|
| concurrent operations | 2 global, 1/Session | exercise isolation with small scope |
| each of four directional queues | 64 KiB | multiple current 16 KiB payloads/TLS records |
| payload per operation / aggregate | 256 KiB / 512 KiB | explicit hard cap; metadata measured separately |
| high/low watermark | 48/24 KiB | hysteresis |
| PACC fragment | <= current 16 KiB | not a TLS record boundary |
| scheduler | 16 KiB/flow, 8 services/wake | <=128 KiB/wake, operation/flow round robin |
| attach / handshake deadline | 5 s / 10 s | distinct setup phases |
| inactivity / graceful shutdown / forced cleanup | 30 s / 5 s / 5 s | monotonic, bounded terminality |

Measure high-water marks, bytes/service turns, WOULD_BLOCK by layer,
wake-to-progress latency, allocation peak, handshake/shutdown and teardown.

Backpressure propagates by ceasing reads from the preceding layer: site TCP
not writable stops record-flow consumption, fills its bounded PACC queue,
stops C2 reads and causes PST transport WOULD_BLOCK. Application backpressure
is independent and symmetric. TCP, PACC and PST readiness remain distinct;
readiness permits one bounded attempt, partial I/O/WOULD_BLOCK is normal,
cancellation wakes/unregisters all layers, and no path polls or busy-spins.

## Terminality and ownership

| Event | Mapping |
|---|---|
| reciprocal PST `close_notify` | drain accepted application data, then clean close both flows/site socket |
| raw site EOF/reset/half-close before clean TLS | record failure -> PST/operation `TLS_TRUNCATED`; never success |
| application PACC EOF/reset | cancel operation; only bounded policy-permitted TLS shutdown |
| record PACC EOF/reset | fail bridge/PST and application flow |
| verification/provider failure | no application success; explicit fail-closed category |
| cancel/timeout | generation-stamped wake and exactly-once abort |
| CONTROL/Session loss | cancel all owned operations |
| reconnect | new generation; old authority remains invalid |

Browser owns/once-closes site TCP. Managers physically own PACC Channels while
the operation owns them semantically. Bridge setup owns listener/peer until
listener close and endpoint transfer; PST then owns its imported endpoint.
Accelerator owns the PST runtime, which outlives connections. Terminal
compare-and-transition guards make repeated notifications no-ops; queues are
zeroed before reuse. Logs exclude tokens, keys, full certificates, records and
application payload.

## E1--E12 readiness (not executed)

| Gate | Owner/criterion | Prerequisites | State |
|---|---|---|---|
| E1 valid TLS | PST+Accelerator: exact TLS1.3/name/trust/response | C2/two flows; Browser DNS/socket pump | BLOCKED |
| E2 fragmentation | PACC+Browser: exact bytes under all splits | role-bound incremental flows | BLOCKED |
| E3 stalls | joint: watermarks hold and sibling progresses | queues/fair scheduler/injection | BLOCKED |
| E4 cert/name | PST: explicit failure, zero fallback/data | immutable name/trust | BLOCKED |
| E5 cancel | joint: attach/handshake/I/O leave zero resources | cancellation/wake in both processes | BLOCKED |
| E6 timeout | joint monotonic fixture, one terminal | cross-layer deadline state | BLOCKED |
| E7 terminal matrix | controlled site: only reciprocal close clean | mapping and Browser injection | BLOCKED |
| E8 PACC record loss | abort PST/application, no hang | two-flow teardown | BLOCKED |
| E9 attach race | wrong/late/duplicate peer denied, no leak | tuple validation implementation | BLOCKED |
| E10 cross-swap | two operations; swaps denied, sibling survives | role authority + two Browser sockets | BLOCKED |
| E11 teardown order | every permutation exactly once <=5 s | generation guards/ownership hooks | BLOCKED |
| E12 egress ownership | OS inventory: Browser site socket, none in Accelerator | correlated inventories/exact endpoint | BLOCKED mandatory |

Every authorized run must record full source/package/runtime hashes,
toolchain/target, fixture/policy identity, monotonic timestamps and safe logs.
E1 and E12 cannot be replaced by reasoning.

Secure and Legacy both lack the common two-flow contract. Secure can anchor
binding in existing Session trust but lacks Mode C grant semantics. Legacy has
no cryptographic anchor and is additionally blocked by current policy,
admission, warning and isolation gaps. `EXPERIMENT_READY_FOR_AUTHORIZATION=NO`.

## Browser dependency handoff

Browser owner must return source-located, PROVEN/UNKNOWN/BLOCKED evidence for:

1. ownership of original-name normalization, DNS, site connect/socket,
   proxy/NAT interaction and close;
2. authorization of the Mode C site socket, endpoint binding/rebinding defense
   and proof that no direct/local fallback exists;
3. NT4/VC6 wait primitive, partial I/O, wake/cancel and teardown;
4. runtime mapping of one operation to one site socket and two PACC roles,
   including stale-generation/cross-swap rejection;
5. reusable public CONTROL/DATA components and missing PapinhoPACC contract;
6. Legacy warning without a parallel Browser ACL/policy authority;
7. acceptable oldest-target memory/concurrency/deadline budgets;
8. independent E12 proof that Browser owns site egress and Accelerator does not;
9. deterministic fragmentation, stall, EOF/reset/half-close, cancel and
   reconnect injection.

Return branch/HEAD/worktree, target/toolchain, source locations, ownership
diagram and proposed budgets. Do not implement or invent PST behavior.
`BROWSER_HANDOFF_REQUIRED=YES`.

## Legacy parity slices in the current integration program

1. **L0 governance:** explicitly decide evolution from current ADR-0028 denial.
2. **L1 admission:** lower-assurance listener/deployment admission, grants,
   revocation and audit correlation without identity claims.
3. **L2 policy/quotas:** server-owned destination, concurrency, byte/time/rate
   limits; default deny; no Browser ACL.
4. **L3 binding:** profile-aware one-use authority isolated from Secure.
5. **L4 UX/isolation:** persistent warning, admin opt-in, trusted-network
   guidance, no Internet-exposed open service.
6. **L5 validation:** E1--E12 plus impersonation, replay, cross-profile attach,
   quota exhaustion and revocation negatives.

No slice enables Legacy before preceding normative/security gates pass.

## Owner decisions and next task

- **ADR CANDIDATE:** Mode C placement, two-flow operation, trust separation and
  no fallback.
- PapinhoPACC design gate: choose operation/role/terminal contract only after
  both project audits; no wire IDs now.
- **ADR CANDIDATE:** Legacy Mode C admission/authorization and explicit
  evolution from `LEGACY_ENDPOINT_NETWORK_EGRESS_V1=NO`.
- Approve/revise C2 budgets, Secure-first E1--E12 evidence protocol, and whether
  wire credit is needed.

`AUDIT-PACKAGE-001` and `ADR0007-SEC-LOCAL-TRUST-001` remain separate tracks.
Next: obtain the Browser handoff, then hold an Accelerator/PapinhoPACC/Browser
contract gate. Only approved, normatively recorded decisions may authorize a
Secure C2 experiment; Legacy L0--L5 remain tracked in the same integration
program.

## Task report

```text
TASK=PACC-MODE-C-C2-COORDINATION-CONTRACT-AUDIT-1
ACCELERATOR_BRANCH=main
ACCELERATOR_HEAD=c50b8fb9a7e3293b5e6355fc08459bf85b56857a
WORKTREE_STATUS=PREEXISTING_DIRTY_PRESERVED
PST_REPORTS_READ=YES_BOTH_IN_FULL
PST_PINNED_VERSION=0.6.4
MODE_C_BROWSER_OWNS_SITE_TCP=REQUIRED
MODE_C_ACCELERATOR_OWNS_SITE_TLS=REQUIRED
LEGACY_MODE_C_FUNCTIONAL_PARITY_TARGET=YES
LEGACY_MODE_C_CRYPTOGRAPHIC_PARITY=NO
MODE_C_OPERATION_BINDING_PROFILE_AWARE=REQUIRED
PACC_AUTHENTICATED_OPERATION_BINDING=PROVEN_FOR_SECURE_SESSION_ONLY_MODE_C_BINDING_MISSING
PACC_MULTIPLEXING=PARTIAL_PHYSICAL_MULTI_DATA_NO_SHARED_MODE_C_OPERATION
PACC_BIDIRECTIONAL_FLOW_CONTROL=PARTIAL_BOUNDED_STREAMS_NO_WIRE_CREDIT_OR_FOUR_QUEUE_CONTRACT
PACC_CANCEL_DEADLINE=PARTIAL_NO_CROSS_COMPONENT_MODE_C_STATE_MACHINE
PACC_TERMINALITY=PARTIAL_MODE_B_PROVEN_MODE_C_MAPPING_PROPOSED
PACC_RESOURCE_BUDGETS=PROPOSED_NOT_APPROVED
C2_PRIVATE_ENDPOINT_CONTRACT=PROPOSED_DESIGN_REVIEW_PASS_IMPLEMENTATION_UNAUTHORIZED
BROWSER_DEPENDENCIES=OPEN_SOURCE_EVIDENCE_REQUIRED
BROWSER_HANDOFF_REQUIRED=YES
E1_E12_READINESS=BLOCKED_ON_PACC_CONTRACT_BROWSER_EVIDENCE_AND_AUTHORIZATION
E1_TLS_PROOF=PENDING
E12_BROWSER_EGRESS_PROOF=PENDING
EXPERIMENT_READY_FOR_AUTHORIZATION=NO_UNTIL_GATES_RESOLVED
SELECTED_IMPLEMENTATION=NONE_PENDING_APPROVAL
API_SPI_WIRE_CHANGED=NO
PRODUCTION_CODE_CHANGED=NO
EXPERIMENT_EXECUTED=NO
BUILD_EXECUTED=NO
PACKAGE_GENERATED=NO
COMMIT_CREATED=NO
PUSHED=NO
FIRST_DIVERGENCE=CURRENT_PACC_BINDS_DATA_TO_SESSION_NOT_TWO_FLOWS_TO_ONE_MODE_C_OPERATION
NEXT_TASK=BROWSER_FACTUAL_HANDOFF_THEN_CROSS_PROJECT_CONTRACT_GATE
```
