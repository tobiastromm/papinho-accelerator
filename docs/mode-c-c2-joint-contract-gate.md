# Mode C / C2 joint contract gate

Status: contract approved; implementation and experiment execution pending  
Date: 2026-10-07  
Task: `PAPINHO-MODE-C-C2-JOINT-CONTRACT-GATE-1`

This document consolidates facts and records the contract approved by the
project owner on 2026-10-07. The durable decision is canonical in
`docs/adr/ADR-0029-contrato-mode-c-c2-e-autorizacao-laboratorial.md`. This
report does not allocate wire IDs, freeze an API, authorize implementation or
authorize execution of E1--E12, and it does not modify functional code.

## Inputs and normative limits

Read in full:

- Browser: `C:/Projetos/PapinhoBrowser/docs/mode-c-c2-factual-handoff.md`;
- Accelerator: `docs/mode-c-c2-coordination-contract-audit.md`;
- PST: `mode-c-byte-transport-feasibility.md` and
  `mode-c-c2-minimal-experiment-design.md`;
- applicable accepted governance, PapinhoEngineering ADR-0013, Accelerator
  ADR-0028 and accepted capability/PACC/Network Egress ADRs.

Accepted ADRs remain normative. The current-phase clarification permits a
narrow Mode C lab/test authorization to be proposed for Secure and Legacy; it
does not itself implement that authorization or replace ADR-0028. Final Legacy
production policy remains separate.

The factual baseline is: Browser owns a blocking IPv4 site path and does not
retain its selected DNS endpoint; its PACC client serializes one active DATA;
Accelerator/PACC can host multiple DATA Channels but cannot bind two roles to
one operation; PST 0.6.4 can attach C2 through its published socket transport;
C1 is unproven; Mode C and E1--E12 are not implemented/executed.

## Contract invariants

```text
Browser owns original hostname, DNS, selected endpoint, site TCP and IP egress.
Accelerator owns PST Site TLS, SNI/peer name, verification and plaintext I/O.
PACC carries bounded application and encrypted-record roles.
```

Numeric socket handles never cross hosts. Mode B is not renamed Mode C. There
is no BadVPN/tunnel/open proxy, automatic Secure-to-Legacy change, routing-mode
fallback, plaintext TLS fallback, IP-as-TLS-identity or second DNS resolution
for an established operation.

## D1 -- Operation identity and binding

### Recommendation

Create a bounded server-coordinated Mode C operation record containing:

- non-secret correlation identity;
- separate cryptographically unpredictable, single-use attachment authority;
- CONTROL Session identity and generation;
- assurance profile (`SECURE_PRINCIPAL` or explicitly admitted `LEGACY`);
- original normalized hostname, selected numeric endpoint and port;
- Site TLS profile/trust/ALPN inputs owned by the server;
- immutable application and record role slots;
- cancel/terminal generation, policy/grant epoch, expiry and resource state.

The authority is delivered only inside the already selected profile's CONTROL
context. It is never the current predictable DATA ticket, a runtime pointer or
the log correlation ID. Each role attaches once with operation, role and
generation proof. Wrong Session/profile/endpoint/epoch, stale/expired/replayed
authority and duplicate/cross-role attachment fail before bytes or PST setup.

```text
CROSS_OPERATION_INJECTION_REJECTED=YES
STALE_GENERATION_ATTACHMENT_REJECTED=YES
DUPLICATE_ROLE_ATTACHMENT_REJECTED=YES
```

Alternative: infer pairing from attachment order. Rejected because reconnect,
concurrency and cross-operation swapping become ambiguous. Alternative: reuse
the DATA ticket as secret. Rejected because it binds only Session and the
current generator is predictable. PapinhoPACC owns the eventual encoding;
no ID is allocated. In the Legacy profile, the random authority is only an
operation-attachment control: it is not cryptographic peer authentication and
does not protect the plaintext PACC hop against an on-path attacker.

## D2 -- Application and record roles

### Recommendation

One operation has exactly two bidirectional logical roles:

```text
APPLICATION: Browser HTTP/application bytes <-> PACC <-> PST plaintext API
RECORD: Browser site TCP <-> PACC <-> private C2 bridge <-> PST records
```

CONTROL authorizes/reserves the operation. Browser snapshots and connects the
exact endpoint. RECORD attaches; Accelerator creates C2/PST and reaches a
record-ready state. APPLICATION attaches. Only after both roles and the site
socket are ready does activation succeed atomically and TLS handshake start.
Missing/duplicate/wrong roles time out and destroy the reservation. No
application bytes are accepted before activation.

Each role preserves ordered reliable bytes and independent directional
readiness/close facts. A role can become locally closed, but any required-role
loss drives the operation's joint terminal state. Browser never parses TLS;
Accelerator never parses HTTP as part of Mode C transport. Alternative single
mixed stream is rejected because it confuses plaintext/records and independent
backpressure. Approval and a PACC design are required.

## D3 -- Browser endpoint snapshot and exact connect

### Recommendation

Browser resolves the normalized original hostname once per candidate attempt,
classifies all returned addresses under Browser core routing policy, selects
one permitted numeric endpoint, and publishes an immutable value snapshot:

```text
original hostname + address family + numeric address + port + resolution epoch
```

The Browser socket connects directly to that selected numeric value. The
original hostname remains the only SNI/expected peer name. An IP never
substitutes TLS identity. Accelerator grants the operation against the same
hostname/endpoint/port tuple without owning the site socket. Redirect creates a
new authorization/operation; retrying another address requires a new candidate
selection and explicit operation attempt, never silent rebinding.

`SECOND_DNS_RESOLUTION_FOR_SELECTED_OPERATION=NO` and
`IP_NEVER_SUBSTITUTES_TLS_IDENTITY=YES`. Alternative `gethostbyname()+connect`
is insufficient because the chosen endpoint is discarded. Browser owns this
change; detailed multi-address/retry behavior remains an implementation-design
matter constrained by this immutable endpoint contract.

## D4 -- Browser nonblocking, cancellable site transport

### Recommendation

Add a Browser-internal, value-owned WinSock transport for Mode C, separate from
the current blocking convenience API. It must support nonblocking exact-endpoint
connect, partial send/recv, WOULD_BLOCK, independent read/write interest,
readiness wait plus explicit wake, monotonic deadline, navigation cancel, raw
EOF/error, directional shutdown fact and idempotent close.

Use only primitives proven on the actual NT4/VC6 target. The nonblocking,
wakeable contract is approved; the concrete WinSock primitive remains open for
qualification in slice C1/NT4. `select()` plus a Browser-owned wake mechanism
is an acceptable candidate, not frozen here.
Readiness grants one bounded attempt and never promises progress. Cancellation
wakes the owner thread, which unregisters then closes. Sleep polling and a
thread blocked indefinitely in DNS/connect/recv are not correctness mechanisms.

```text
NO_SLEEP_POLLING_AS_CORRECTNESS_MECHANISM=YES
BOUNDED_CANCELLABLE_PROGRESS=YES
```

Alternative retaining the blocking socket is rejected. Browser owns the
contract; the exact primitive needs target evidence and qualification.

## D5 -- Backpressure and resource budgets

### Recommendation

Maintain four independent bounded queues per operation: application B->A and
A->B, record B->A and A->B. A full downstream queue disables the corresponding
upstream read interest; service resumes only below its low watermark. TCP,
PACC and PST readiness remain distinct. Scheduling rotates operation first,
then role/direction, with bounded attempts and early stop after a no-progress
round.

Initial **experiment-only** limits:

| Item | Proposed limit |
|---|---:|
| operations | 2 global; 1 per Session (two Sessions for concurrency case) |
| each directional queue | 64 KiB hard cap; high 48 KiB; low 24 KiB |
| four-queue payload/operation | 256 KiB |
| aggregate queue payload | 512 KiB |
| PACC payload fragment | at most current 16 KiB |
| service quantum | 16 KiB/flow; max 8 services/wake |
| metadata | fixed caller-owned records, separately measured/capped |

These limits are approved exclusively for the experiment; they are not
measured or production-approved. NT4 qualification
must sweep 8/16/32/64 KiB queues and record allocation peak, high-water marks,
wakes, zero-progress wakes, latency and throughput. Production limits remain
`UNDECIDED`. Smaller all-target values may replace them after evidence.

## D6 -- Cancellation, deadlines and terminality

### Recommendation

One operation generation owns a terminal compare-and-transition. The first
factual terminal cause is retained for diagnosis; later facts remain secondary
cleanup observations. Separately, the termination/security action is selected
by policy and may prevail over the factual cause: policy/security revocation or
explicit cancel; CONTROL loss; verification/PST failure; required DATA loss;
site reset/raw EOF; deadline; graceful completion. Action precedence never
rewrites the factual cause and never turns a failure into success.

Proposed experiment deadlines: role attach 5 s, TLS handshake 10 s,
no-progress 30 s, graceful shutdown 5 s and forced cleanup 5 s. All are
monotonic and independently categorized. Browser navigation cancel, policy
revocation, CONTROL loss, either DATA loss, site failure, PST failure and
process teardown wake both owners and converge on the same generation.

PST alone determines reciprocal `close_notify`. Raw site EOF/reset/half-close
is forwarded as a transport fact and is unclean unless PST already proved clean
TLS. APPLICATION completeness remains independent under PapinhoEngineering
ADR-0013. Required-role loss cancels its sibling; accepted bytes may be drained
only where the primary terminal policy permits. Each physical owner closes its
resource once; slots/queues are cleared before reuse. Orphans expire and abort.

Alternative independent uncoordinated teardown is rejected due to leak,
double-close and false-clean risk. Joint approval is required.

## D7 -- Private C2 bridge

### Recommendation

Use an Accelerator-process private IPv4 loopback connected pair: bind
`127.0.0.1:0`, create the owned connector, record its exact tuple, accept only
that tuple, then immediately close the one-shot listener. Transfer one endpoint
through PST's public socket transport; the operation bridge owns the peer.

No wildcard/public bind, firewall exception, destination command or reusable
listener. Wrong local peer is closed and aborts setup without affecting other
operations. Generation, exact tuple and operation ownership protect lifetime;
an additional local nonce is defense in depth, not a DATA-ticket reuse. The
wait-set owns readiness registration; unregister precedes idempotent close.

```text
C2_PUBLIC_LISTENER=NO
C2_ARBITRARY_PROXY=NO
```

Alternative C1 is not selectable because feasibility is unproven. Accelerator
owns implementation; PST owns TLS semantics. The design contract is approved;
implementation and target evidence remain pending.

## D8 -- Capability and service model

### Recommendation

Reuse the existing conceptual `TLS_OFFLOAD` capability for Accelerator-owned
Site TLS. ADR-0018 explicitly preserves Mode C as TLS_OFFLOAD with local network
ownership. Do not require or grant Accelerator `NETWORK_EGRESS`, because Browser
owns site TCP/IP egress.

Mode C still needs a distinct versioned **service/operation contract** defining
endpoint snapshot, two roles and lifecycle. Whether that service identity is a
new capability key, a compatible major/version of a TLS Offload operation, or
another negotiated descriptor must be decided during the PACC design; no ID is
allocated here. `TLS_OFFLOAD` alone never grants Browser local egress policy.

Alternative modeling Mode C as `NETWORK_EGRESS` is rejected as false ownership.
Alternative adding a new capability immediately is premature. Approval is
required; a wire change is expected for operation/roles, but its shape is open.

## D9 -- Legacy security and admission

Legacy is plaintext PACC and never mTLS-equivalent. Browser selects the profile,
consumes grants/denials, shows assurance and never auto-downgrades. Accelerator
alone owns admission, grants, destination policy, quotas, audit and revocation.

### D9A -- Current-phase lab/test authorization

**Status: approved as architecture for controlled lab/test use**, recorded
normatively by ADR-0029. Execution remains unauthorized until a new gate after
the prerequisites are implemented. Provide a test-only, explicit opt-in grant injected by
the controlled harness/shared configuration boundary, not a product default.
It is limited to named lab listeners/interfaces, exact fixture hostname,
numeric endpoints and port, test trust material, bounded duration and the
experiment quotas in D5. It grants only the Mode C service needed for E1--E12.

Secure and Legacy runs are distinct profiles and evidence sets. Legacy uses a
separate listener/profile, lower-assurance label and visible test warning. No
Internet/public listener, arbitrary destination, wildcard policy, automatic
fallback, saved reusable password or Browser ACL. Policy epoch is bound into
each operation. Revocation/CONTROL loss aborts all its operations. Loss of the
declared isolated lab interface/zone fails closed. Category logs record profile,
grant epoch, destination category, decision and bytes, never tokens/payloads.

This temporary switch/env/harness grant is scaffolding, not final CLI, Registry,
GUI or production policy. Interface-scoped admission is a valid direction; no
syntax is frozen. D9A supplies safe test authority without pretending current
ADR-0028 runtime denial has already changed.

### D9B -- Production Legacy policy

Status: `PROPOSED`, not an experiment blocker after D9A approval. Production
must later decide listener/interface binding, source subnet/zone, admission
mode, capabilities/services, destinations/ports, time/context rules, quotas,
audit, revocation and loss-of-isolation behavior through the shared
Configuration Model. It remains in current-phase slices C9--C11.

```text
LEGACY_FUNCTIONAL_PARITY_TARGET=YES
LEGACY_CRYPTOGRAPHIC_PARITY=NO
LEGACY_MODE_C_CURRENTLY_AUTHORIZED=NO
D9A_STATUS=APPROVED_CONTRACT_NOT_IMPLEMENTED
D9B_STATUS=PROPOSED
BROWSER_LEGACY_ACL_OWNER=NO
ACCELERATOR_SERVER_POLICY_OWNER=YES
```

Alternative waiting for the entire production editor/policy is rejected by the
approved current-phase clarification. Alternative broad Legacy enablement is
rejected as unsafe. D9B needs later durable decisions in slices C9--C11.

## D10 -- E1--E12 readiness and authorization

| Gate | Owners and fixture | Prerequisite | Current readiness | Deterministic pass evidence |
|---|---|---|---|---|
| E1 valid Site TLS | PST/Accelerator + controlled TLS1.3 target + Browser | D1--D8, D9A for profile | BLOCKED implementation | original hostname as SNI/peer, chain/name/version PASS, exact application response over both roles |
| E2 fragmentation | Browser/PACC/Accelerator split fixture | role pumps/partial I/O | BLOCKED | exact bytes, no loss/duplication/spin |
| E3 stalls/watermarks | joint controlled stall | D4/D5 queues/readiness | BLOCKED | caps/watermarks hold, sibling fairness/recovery |
| E4 verification negatives | PST target wrong cert/name | immutable TLS authority | BLOCKED | fail before app success, zero fallback |
| E5 cancel | Browser navigation + both runtimes | D6 wake/cleanup | BLOCKED | handshake/I/O cancel once, no resurrection/leak |
| E6 deadlines | monotonic controlled withholding | D6 phase deadlines | BLOCKED | exact timeout category, bounded no-spin cleanup |
| E7 terminal matrix | target close/reset injector | ADR-0013 mapping | BLOCKED | only reciprocal close_notify clean; data preserved separately |
| E8 PACC role loss | PACC channel fault injector | D2/D6 | BLOCKED | record/app loss aborts joint operation, siblings isolated |
| E9 local attach race | Accelerator C2 fixture | D7 exact tuple | BLOCKED | wrong peer denied; authorized pair unaffected |
| E10 cross-operation swap | two Sessions/operations | D1 roles/auth | BLOCKED | swap/replay/duplicate rejected, no confused deputy |
| E11 reverse teardown | all owners with counters | D6 idempotence | BLOCKED | each handle/socket/channel/PST object closes once within deadline |
| E12 Browser egress | controlled target + OS PID/tuple inventory | D3/D4 and correlation | BLOCKED | Browser owns site socket, Accelerator count zero, target observes correlated Browser connection |

E12 cannot rely only on source IP under NAT. Every run records full source,
package, executable, fixture/certificate and manifest hashes; PIDs/tuples;
monotonic timestamps; queue/wake/byte/handle metrics; provider/TLS verification
categories; and zero secret/private-key/payload log hits.

`D10_CONTRACT_STATUS=APPROVED_EXECUTION_NOT_AUTHORIZED`. After implementation
of the listed prerequisites, a new gate and separate human authorization are
required before running Secure and Legacy E1--E12.
`EXPERIMENT_READY_FOR_HUMAN_AUTHORIZATION=NO` today.

## Current-phase slices after approval

```text
C0 Browser endpoint snapshot/exact-connect
C1 Browser nonblocking/cancellable site-socket pump
C2 Mode C operation + APPLICATION/RECORD PACC contract
C3 Accelerator private C2 bridge
C4 Secure Mode C authorization/binding
C5 budgets/backpressure/cancel/deadline/terminal implementation
C6 deterministic E1--E12 fixtures
C7 Secure Mode C E1--E12 experiment
C8 production Secure Mode C integration
C9 Legacy Mode C normative production-security approval
C10 Legacy Mode C implementation
C11 Legacy Mode C validation/profile matrix
C12 NT4 Mode C qualification
```

C9--C12 remain in this integration phase. D9A is recorded before lab
implementation; D9B/C9 records production policy before production Legacy code.

## Human decision sheet

| Decision | Recommended option | Alternatives | Main consequences / missing evidence | Owner | Approval |
|---|---|---|---|---|---|
| D1 | random single-use authority + separate correlation ID | order inference; DATA ticket reuse | rejects replay/swap; encoding and entropy implementation missing | PACC + Accelerator + Browser | YES |
| D2 | two explicit roles, atomic activation, joint terminal | one mixed stream; independent operations | clear ownership/backpressure; protocol shape missing | PACC + both consumers | YES |
| D3 | immutable hostname/endpoint/port snapshot and exact connect | current resolver; re-resolve | prevents rebinding/identity confusion; multi-address tests missing | Browser + Accelerator policy | YES |
| D4 | nonblocking readiness transport with wake/cancel/deadline | blocking socket; sleep polling | bounded NT4 progress; exact proven primitive missing | Browser | YES |
| D5 | four queues and experiment caps above | unbounded; single shared queue | bounded/fair; NT4 measurements missing | Browser + Accelerator | YES |
| D6 | one terminal generation, precedence and owner-close-once | independent teardown | prevents false clean/UAF; race tests missing | all runtime owners/PST semantics | YES |
| D7 | one-shot exact-tuple loopback C2 | C1; reusable listener | no public proxy; local race tests missing | Accelerator + PST review | YES |
| D8 | TLS_OFFLOAD + distinct Mode C service contract | NETWORK_EGRESS; immediate new key | preserves ownership; final capability/wire form missing | capability/PACC owners | YES |
| D9A | narrow explicit Secure/Legacy lab grant | wait for full production policy; broad enable | enables safe testing, not default; normative record/fixtures missing | human + Accelerator security | YES |
| D9B | server-owned production Legacy policy in C9--C11 | Browser ACL; permanent ban | functional not crypto parity; detailed model missing | human + Accelerator | LATER YES |
| D10 | approve contract now, execution only after implementation review | execute now | avoids assumption-driven tests; all gates currently blocked | human project owner | YES |

The approvals in this table were supplied by the project owner on 2026-10-07
and are recorded canonically in ADR-0029. D9B remains proposed, and D10 does
not authorize experiment execution.

## Required report

```text
TASK=PAPINHO-MODE-C-C2-JOINT-CONTRACT-GATE-1

INPUTS:
Browser_report=C:/Projetos/PapinhoBrowser/docs/mode-c-c2-factual-handoff.md
Accelerator_report=docs/mode-c-c2-coordination-contract-audit.md
PST_reports=mode-c-byte-transport-feasibility.md;mode-c-c2-minimal-experiment-design.md

OPERATION:
identity=NONSECRET_CORRELATION_PLUS_RANDOM_SINGLE_USE_AUTHORITY
session_generation_binding=REQUIRED
endpoint_binding=HOSTNAME_NUMERIC_ENDPOINT_PORT_IMMUTABLE
role_binding=EXACTLY_ONE_APPLICATION_AND_ONE_RECORD
cross_operation_protection=REPLAY_STALE_DUPLICATE_SWAP_REJECTED

FLOWS:
application_role=BROWSER_APP_BYTES_TO_PST_PLAINTEXT
record_role=BROWSER_SITE_TCP_TO_PRIVATE_C2_PST_RECORDS
activation_contract=BOTH_ROLES_AND_EXACT_SITE_SOCKET_FAILURE_ATOMIC
independent_close=DIRECTIONAL_FACTS_PRESERVED
joint_terminal=ONE_OPERATION_GENERATION

BROWSER_SOCKET:
endpoint_snapshot=REQUIRED_BROWSER_OWNED
exact_connect=REQUIRED_NO_SECOND_DNS
nonblocking_contract=REQUIRED_READINESS_DRIVEN
cancel_contract=WAKE_OWNER_UNREGISTER_THEN_CLOSE
half_close=RAW_TRANSPORT_FACT_NOT_TLS_CLOSE
EOF_RST=UNCLEAN_UNLESS_PST_ALREADY_PROVED_CLOSE_NOTIFY

C2_BRIDGE:
private_endpoint=ONE_SHOT_IPV4_LOOPBACK_EXACT_TUPLE
attachment_auth=OPERATION_GENERATION_PLUS_TUPLE_OPTIONAL_NONCE
public_exposure=NO
lifecycle=OPERATION_OWNED_IDEMPOTENT

BACKPRESSURE:
contract=FOUR_BOUNDED_QUEUES_STOP_UPSTREAM_READS
high_low_water=48_KIB_24_KIB_EXPERIMENT_APPROVED_ONLY
fairness=OPERATION_THEN_ROLE_ROUND_ROBIN_BOUNDED

BUDGETS:
experiment_limits=2_GLOBAL_1_PER_SESSION_64_KIB_PER_QUEUE_512_KIB_AGGREGATE
production_limits_status=NOT_APPROVED_UNDECIDED
NT4_measurement_required=YES_8_16_32_64_KIB_SWEEP

CANCEL_TERMINAL:
operation_cancel=GENERATION_STAMPED_ALL_OWNERS_WOKEN
deadlines=ATTACH5_HANDSHAKE10_IDLE30_SHUTDOWN5_CLEANUP5_SECONDS_PROPOSED
factual_terminal_cause=FIRST_FACT_RETAINED_FOR_DIAGNOSIS
termination_action=POLICY_SECURITY_ACTION_MAY_PREVAIL_WITHOUT_REWRITING_FACT
exactly_once_cleanup=PHYSICAL_OWNER_CLOSE_ONCE_CLEAR_BEFORE_REUSE

TLS_AUTHORITY:
SNI=ORIGINAL_BROWSER_HOSTNAME
peer_name=ORIGINAL_BROWSER_HOSTNAME
chain_verify=PST_ACCELERATOR
hostname_verify=PST_ACCELERATOR
raw_EOF_semantics=TRANSPORT_TRUNCATION_NOT_CLOSE_NOTIFY

CAPABILITY:
Mode_C_service_model=TLS_OFFLOAD_PLUS_DISTINCT_VERSIONED_OPERATION_CONTRACT
NETWORK_EGRESS_independent=YES_BROWSER_LOCAL_POLICY_OWNS_SITE_EGRESS
wire_change_required=YES_CONTRACT_SHAPE_UNDECIDED
IDs_allocated=NO

SECURE:
assurance=TLS13_MTLS_AUTHENTICATED_SESSION
binding=SESSION_GENERATION_OPERATION_AUTHORITY_TWO_ROLES

LEGACY:
functional_parity_target=YES
cryptographic_parity=NO
currently_authorized=NO
attachment_authority_security=NOT_CRYPTOGRAPHIC_AUTH_NOT_ON_PATH_PROTECTION
security_gate=D9A_LAB_CONTRACT_APPROVED_D9B_PRODUCTION_PROPOSED
Browser_ACL_owner=NO
server_policy_owner=YES

E1_E12:
matrix=APPROVED_ALL_CURRENTLY_BLOCKED_ON_IMPLEMENTATION_AND_NEW_EXECUTION_GATE
E1_ready=NO
E12_ready=NO

EXPERIMENT:
ready_for_human_authorization=NO
blockers=PREREQUISITE_IMPLEMENTATION_FIXTURES_REVIEW_NEW_EXECUTION_GATE

CURRENT_PHASE:
implementation_slices=C0_THROUGH_C12
Legacy_Mode_C_in_current_phase=YES
NT4_Mode_C_in_current_phase=YES

HUMAN_DECISIONS:
D1=APPROVED_WITH_LEGACY_AUTHORITY_CLARIFICATION
D2=APPROVED
D3=APPROVED
D4=APPROVED_PRIMITIVE_REMAINS_FOR_C1_NT4_QUALIFICATION
D5=APPROVED_EXPERIMENT_ONLY_NOT_PRODUCTION_LIMITS
D6=APPROVED_FACTUAL_CAUSE_SEPARATE_FROM_TERMINATION_ACTION
D7=APPROVED
D8=APPROVED
D9=D9A_APPROVED_D9B_PROPOSED_FOR_C9_C11
D10=CONTRACT_AND_E1_E12_MATRIX_APPROVED_EXECUTION_NOT_AUTHORIZED

CHANGES:
functional_code_change_count=0
PACC_wire_changed=NO
PST_changed=NO
Browser_changed=NO
Accelerator_changed=NO
experiment_executed=NO

CANONICAL_ADR=docs/adr/ADR-0029-contrato-mode-c-c2-e-autorizacao-laboratorial.md
STATUS=MODE_C_C2_CONTRACT_APPROVED_IMPLEMENTATION_PENDING
FIRST_DIVERGENCE=APPROVED_PREREQUISITES_NOT_YET_IMPLEMENTED
next_task=PLAN_IMPLEMENTATION_SLICES_C0_C1_C2_UNDER_ADR_0029
```

`MODE_C_C2_CONTRACT_APPROVED=YES` and
`MODE_C_C2_EXPERIMENT_EXECUTION_AUTHORIZED=NO`.
