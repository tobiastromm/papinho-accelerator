# PapinhoBrowser Mode C / C2 factual handoff

Status: audit only; ready as input to the joint contract gate  
Date: 2026-10-07  
Task: `PAPINHOBROWSER-MODE-C-C2-FACTUAL-HANDOFF-1`

No implementation, experiment, PACC wire, PST, or Accelerator change is
authorized or performed by this report.

## Baseline and authority

PapinhoBrowser branch `main`, HEAD
`b3c81939dc0b6cf6239b3f729a88f51d61ed10d7`. The worktree was already heavily
modified and contained many untracked historical test artifacts. It was
preserved without reset, stash, clean, checkout, commit, or push.

Applicable accepted OrganizationEngineering, PapinhoEngineering and Browser
ADRs were read, especially Browser ADR-0001 and ADR-0007. Current code/tests are
factual evidence. The Accelerator coordination audit and both PST Mode C/C2
reports are gate inputs, not authority to invent Browser behavior.

## DNS and site endpoint ownership

| Fact | State | Evidence |
|---|---|---|
| URL parser preserves hostname and port separately | PROVEN | `src/papinho_browser.c:4159`, `5238` (`papinho_http_url`, `papinho_parse_url`) |
| Browser owns DNS for local routes | PROVEN | `platform/win32/papinho_socket_win32.c`, `papinho_win32_socket_connect()` |
| DNS API | PROVEN | blocking Winsock 1.1 `gethostbyname()` after `inet_addr()` |
| Normalization | PARTIAL | URL host is copied as supplied; no canonical DNS/IDNA normalization is proven |
| Address family | PROVEN IPv4-only | `sockaddr_in`, `AF_INET`, first `h_addr_list[0]` |
| Multiple addresses | PROVEN insufficient | only first returned address is attempted |
| Selected endpoint retention | BLOCKED | numeric address is stack-local and discarded after `connect()` |
| Second-resolution risk | PROVEN one resolution per current connect call | one `gethostbyname()` in the call, but retries/new calls may resolve again; no operation-bound endpoint exists |

`MODE_C_ORIGINAL_HOSTNAME_PRESERVABLE=YES` and
`MODE_C_SELECTED_ENDPOINT_PRESERVABLE=NO_CURRENTLY`. Mode C needs a value-owned
result containing original hostname, selected IPv4 endpoint and port, with no
second resolution inside the operation.

## Site TCP and NT4/VC6 I/O

Browser owns creation, connect, use and idempotent close of the local site
socket (`core/networking/papinho_socket.*`,
`platform/win32/papinho_socket_win32.*`). A raw `SOCKET` is never transferable
between machines.

| Behavior | State | Current fact / missing contract |
|---|---|---|
| socket/connect | PROVEN | synchronous `socket(AF_INET)` + blocking `connect()` |
| nonblocking mode | BLOCKED | no `FIONBIO`, `select`, `WSAEventSelect` or equivalent in site backend |
| partial send | PROVEN application helper | `papinho_socket_send_all()` loops, but blocks and has no WOULD_BLOCK state |
| partial recv | PROVEN chunks | `papinho_socket_receive_all()` loops until EOF, bounded only by response limit |
| readiness/wake | BLOCKED | absent from site socket abstraction |
| cancel | PARTIAL | navigation event checked between phases; cannot interrupt blocking DNS/connect/send/recv |
| deadline | BLOCKED | no site socket monotonic deadline |
| half-close/shutdown | BLOCKED | abstraction exposes close only, no `shutdown()` |
| EOF | PROVEN | `recv()==0` is raw peer close and ends receive-all |
| RST/error | PROVEN coarse | `recv/send/connect==-1`, last WSA error retained globally |
| close ownership | PROVEN | Browser local path calls idempotent `papinho_socket_close()` |

The current synchronous abstraction cannot be reused as the C2 record pump
without a bounded, cancellable, readiness-driven Browser-owned transport
contract. This is the primary Browser runtime blocker.

## Routing and policy

Browser ADR-0001 makes routing policy core-owned and fail-closed. The current
pure resolver in `src/papinho_routing_policy.*` independently models network
origin and Site TLS origin. It can produce local-network/Accelerator-TLS, but
the production dispatch at `src/papinho_browser.c:9720` implements only:

- local network + local TLS (Mode A); or
- Accelerator network + Accelerator TLS/plain stream (Mode B/TCP plain).

When resolution returns local network + Accelerator TLS, dispatch falls into
the local socket branch and calls local TLS. Thus the conceptual mixed route is
not a productive Mode C path. It must fail closed until a Mode C provider exists.

Browser owns authorization of local site egress. Accelerator
`NETWORK_EGRESS` must not be used as authority for Browser-owned site TCP.
Current routing policy selects origin, but has no concrete per-destination,
port, DNS-result or Mode C provider authorization. Smallest missing policy
contract: an explicit Browser core route/provider decision binding scheme,
normalized hostname, port and selected endpoint to one Mode C operation, with
no implicit direct/local-TLS fallback.

## PACC operation and channel facts

| Contract | State | Evidence / gap |
|---|---|---|
| persistent CONTROL Session | PROVEN Secure | `papinho_accelerator_persistent_session` and establishment path |
| Session generation/stale ticket protection | PROVEN current scope | `session_generation` and `data_ticket_session_generation` checks |
| reconnect/config generation | PROVEN current scope | reconnect generation and service request generation |
| DATA Channels | PROVEN sequential | fresh ticket/DATA per operation; `runtime.data` is singular |
| multiple simultaneous DATA per CONTROL | BLOCKED in Browser | PACC v1 client explicitly serializes one active DATA operation |
| two flows bound to one operation | BLOCKED | no shared operation identity or application/record role |
| cross-operation protection for Mode C | BLOCKED | current ticket binds Session, not two roles to one operation |

CONTROL framing, DATA ticket/attach, incremental framing and Session anchoring
are reusable foundations. Existing DATA mechanics may transport either flow
after a contract exists, but cannot distinguish or pair both today.
`NEW_MESSAGE_OR_CHANNEL_ROLE_REQUIRED=YES_CONTRACT_REQUIRED`; this report
assigns no message ID and selects no wire shape.

## Application and TLS-record flows

Mode B already provides bounded PACC `NETWORK_DATA` framing, HTTP request bytes,
Browser-owned response storage, HTTP completeness tracking and independent
terminal facts in `papinho_accelerator_client.*`. Those concepts are reusable,
but the Mode B executor assumes one DATA Channel whose remote endpoint is an
Accelerator-owned network stream. Therefore
`MODE_B_APP_FLOW_REUSABLE_FOR_MODE_C=PARTIAL_CONCEPTS_ONLY`.

Missing application-flow contract: one role-bound bidirectional plaintext
flow, partial PACC I/O, bounded request/response ownership, cancellation, and a
terminal linked to (but distinct from) the record flow.

The Browser TLS-record flow must own the site socket and:

- forward opaque record bytes both ways without TLS interpretation;
- preserve partial send/recv offsets and independent read/write interest;
- stop upstream reads at high watermark and resume at low watermark;
- distinguish WOULD_BLOCK, raw EOF, RST, half-close, cancel and deadline;
- wake on PACC/site readiness without polling;
- close exactly once and never manufacture `close_notify`.

None of that record-pump runtime exists today.

## Backpressure, memory and concurrency

Current factual bounds include PACC data payload maximum 16 KiB, persistent
request maximum 4 KiB, response maximum 1 MiB, initial response 4 KiB and job
queue limit 4 (`papinho_accelerator_client.h`). They describe Mode B job/storage,
not C2 record queues. Local socket send/receive has no bounded intermediary
queue; receive accumulation is bounded at higher HTTP limits.

`SOCKET_TO_PACC_QUEUE_BOUND=ABSENT`, `PACC_TO_SOCKET_QUEUE_BOUND=ABSENT`, and
`MAX_INFLIGHT_MODE_C_OPERATIONS=0_CURRENTLY`. The Accelerator proposal of four
64 KiB queues, two global operations and one per Session is only input. NT4/old
machine acceptance requires measured heap peak, queue high-water marks and
smaller-limit stress before approval.

## Cancellation, deadlines and terminality

Navigation owns a manual-reset cancel event. The PACC persistent service copies
jobs, supports queued/active cancellation, bridges cancel/wake into PST wait-set
progress, serializes one active operation and retains generation checks.
CONTROL loss tears down the persistent service; DATA terminal facts are kept
separate from HTTP completeness, including `TLS_TRUNCATED`.

Missing for Mode C: one cancellation generation spanning navigation, both PACC
roles and the site socket; interruptible DNS/connect/I/O; attach, handshake,
idle and shutdown deadlines; and exactly-once cross-flow teardown.

Raw site EOF/RST/half-close is a transport fact and must be sent to the Mode C
operation as such. It is never TLS `close_notify` and never proves HTTP
completeness. The Browser has no current half-close representation, so that
contract is blocked. Accepted ADR-0013 layering remains mandatory.

## E1 and E12 evidence

E1 Browser observations must include the exact hostname/port submitted, the
operation identity/correlation, returned application bytes and Accelerator/PST
attestation categories for negotiated TLS version, SNI, chain and hostname
verification. Browser must display/consume those facts but must not claim it
performed verification. Controlled wrong-name/trust cases must return no
application success or fallback.

E12 requires a controlled target and correlated interval with:

- Browser counters/logs for DNS result, selected numeric endpoint, socket
  create/connect/close and operation roles;
- OS socket inventory attributing the site connection to Browser PID;
- target observation of Browser-facing source address/connection;
- Accelerator instrumentation proving zero site TCP socket opens for that
  operation while PST/C2 and both PACC roles are active;
- exact hashes, PIDs, endpoint, timestamps and safe operation correlation.

`E1_BROWSER_READINESS=MISSING_CONTRACT` and
`E12_BROWSER_READINESS=MISSING_CONTRACT_AND_FIXTURE`; neither is PASS.

## Secure and Legacy profiles

Secure Principal has a TLS 1.3 mTLS CONTROL Session, immutable negotiated
capability snapshot, Session generation and DATA attach foundation. It lacks a
Mode C service/capability identity, operation authority, two DATA roles and
joint cancellation/terminal contract.

Legacy profile selection and plaintext CONTROL/DATA client paths exist, but
the routing resolver requires Secure Principal for Accelerator capabilities and
current accepted Accelerator policy denies Legacy Network Egress. Legacy Mode C
is not currently authorized. After a normative gate, Browser needs the same
functional two-flow provider under server-owned admission/grants/policy, with
profile-aware isolation and no Browser ACL system.

```text
LEGACY_MODE_C_CURRENTLY_AUTHORIZED=NO
LEGACY_MODE_C_NORMATIVE_SECURITY_GATE_REQUIRED=YES
LEGACY_MODE_C_FUNCTIONAL_PARITY_TARGET=YES
LEGACY_MODE_C_CRYPTOGRAPHIC_PARITY=NO
LEGACY_ASSURANCE_DISTINCTION_REQUIRED=YES
```

Browser UX must represent Secure as cryptographically authenticated and Legacy
as lower-assurance/plaintext with deployment isolation. This report does not
freeze final wording.

## Browser E1--E12 readiness

| Gate | Browser classification | Missing Browser fact/fixture |
|---|---|---|
| E1 valid TLS/name/trust | MISSING_CONTRACT | pass hostname and consume PST-owned verification evidence |
| E2 fragmentation | MISSING_CONTRACT | dual-flow pump with deterministic split/coalesce hooks |
| E3 stalls/backpressure | BLOCKED | readiness pump, bounded queues/watermarks |
| E4 cert/name failure | MISSING_FIXTURE | operation-level negative with no fallback |
| E5 cancel | BLOCKED | interruptible site DNS/connect/I/O and joint cancel generation |
| E6 timeout | BLOCKED | monotonic per-phase deadlines |
| E7 terminal matrix | BLOCKED | half-close/raw EOF/RST representation and injection |
| E8 PACC record loss | MISSING_CONTRACT | record role and joint teardown |
| E9 C2 attachment race | NOT_BROWSER_OWNED | Browser must observe operation failure/no fallback |
| E10 cross-operation swap | BLOCKED | two role-bound concurrent operations |
| E11 teardown ordering | BLOCKED | exactly-once counters/hooks across Browser resources |
| E12 Browser egress proof | MISSING_CONTRACT | retained endpoint, PID socket inventory, controlled target |

The facts are sufficient for the joint design gate, not for experiment
authorization.

## Joint contract gate decisions

Browser owner, Accelerator coordinator, PST owner where TLS semantics apply,
and human project owner must jointly decide:

1. Mode C service/capability identity and profile authorization;
2. operation authority, CONTROL generation binding and replay rules;
3. two DATA flow roles and activation/failure atomicity;
4. Browser endpoint-resolution value and no-second-resolution rule;
5. Browser nonblocking site transport/readiness interface;
6. end-to-end backpressure, queues, watermarks, fairness and NT4 budgets;
7. cancel, attach/handshake/idle/shutdown deadlines and wake propagation;
8. raw EOF/RST/half-close, PACC terminal and PST `close_notify` mapping;
9. exactly-once ownership/teardown and logging/redaction;
10. Secure versus Legacy admission/assurance/warnings and the required
    normative Legacy gate;
11. E1--E12 fixtures, hashes, criteria and explicit execution authorization.

No API or wire shape/ID is frozen here.

## Required report

```text
TASK=PAPINHOBROWSER-MODE-C-C2-FACTUAL-HANDOFF-1

BASELINE:
branch=main
HEAD=b3c81939dc0b6cf6239b3f729a88f51d61ed10d7
worktree=PREEXISTING_DIRTY_PRESERVED

DNS:
owner=Browser
normalization=PARTIAL_URL_HOST_COPY_NO_DNS_CANONICALIZATION
selected_endpoint_preservable=NO_CURRENTLY
original_hostname_preservable=YES

SITE_TCP:
owner=Browser
nonblocking_model=ABSENT_BLOCKING_WINSOCK_1_1
lifetime_owner=Browser_local_network_path
partial_IO=SEND_LOOP_AND_RECV_CHUNKS_PROVEN_BLOCKING
cancel=BETWEEN_PHASES_ONLY
half_close=ABSENT
EOF_RST=RAW_EOF_ZERO_AND_WSA_ERROR_PROVEN_COARSE

POLICY:
site_egress_policy=Browser_core_routing_owner_Mode_C_provider_absent
destination_authorization=ABSENT_FOR_CONCRETE_MODE_C_OPERATION
port_authorization=ABSENT_FOR_CONCRETE_MODE_C_OPERATION

PACC:
operation_model=MODE_B_JOB_ID_LOCAL_ONLY_ONE_ACTIVE_DATA
multiple_DATA_channels=SEQUENTIAL_YES_SIMULTANEOUS_NO_IN_CURRENT_BROWSER
two_flows_one_operation=NO
generation_protection=PARTIAL_SESSION_TICKET_RECONNECT_JOB
CONTROL_reuse=YES_FOUNDATION
DATA_roles=MISSING_CONTRACT

FLOW:
application_flow_reusable=PARTIAL_CONCEPTS_ONLY
TLS_record_flow_requirements=NONBLOCKING_BOUNDED_ROLE_BOUND_PUMP_MISSING

BACKPRESSURE:
socket_to_PACC=ABSENT
PACC_to_socket=ABSENT
queue_bounds=MODE_C_ABSENT
max_operations=ZERO_CURRENTLY

CANCEL:
navigation_cancel=EVENT_PROVEN_BETWEEN_LOCAL_SOCKET_PHASES
socket_cancel=ABSENT
PACC_cancel=PROVEN_FOR_CURRENT_SERIAL_MODE_B_JOB
deadlines=SITE_SOCKET_ABSENT_MODE_C_MISSING
exactly_once_teardown=MISSING_CROSS_FLOW_CONTRACT

E1:
Browser_requirements=HOST_PORT_OPERATION_AND_PST_OWNED_TLS_EVIDENCE
readiness=MISSING_CONTRACT

E12:
evidence_plan=BROWSER_PID_SOCKET_COUNTERS_OS_INVENTORY_TARGET_SOURCE_AND_ACCELERATOR_ZERO_SITE_SOCKET
readiness=MISSING_CONTRACT_AND_FIXTURE

SECURE:
binding=SESSION_MTLS_CAPABILITY_SNAPSHOT_GENERATION_AND_DATA_ATTACH_FOUNDATION
missing_contract=MODE_C_IDENTITY_TWO_ROLES_OPERATION_AUTHORITY_TERMINALITY

LEGACY:
profile_selection=EXISTS
CONTROL=EXISTS_PLAINTEXT
DATA=EXISTS_PLAINTEXT_FOUNDATION
capability_negotiation=CLIENT_MECHANISM_EXISTS_CURRENT_EGRESS_RESTRICTED
current_Mode_C_authorized=NO
normative_security_gate_required=YES
functional_parity_target=YES
cryptographic_parity=NO
assurance_UI_required=YES

E1_E12:
matrix=FACTUAL_INPUT_COMPLETE_ALL_EXPERIMENT_GATES_PENDING

JOINT_GATE:
decisions_required=OPERATION_ROLES_ENDPOINT_READINESS_BACKPRESSURE_BUDGETS_CANCEL_TERMINAL_CAPABILITY_PROFILES_EVIDENCE

CHANGES:
functional_code_change_count=0
PACC_wire_changed=NO
PST_changed=NO
Accelerator_changed=NO
experiment_executed=NO

STATUS=BROWSER_MODE_C_FACTUAL_HANDOFF_READY
FIRST_DIVERGENCE=BROWSER_SITE_SOCKET_IS_BLOCKING_AND_PACC_HAS_NO_TWO_FLOW_MODE_C_OPERATION
next_task=joint Mode C C2 contract gate
```

`BROWSER_MODE_C_FACTUAL_HANDOFF_READY=YES` means only that the joint gate can
proceed without pretending missing Browser contracts are implemented.

## 2026-10-25: typed target evidence integrated, admission remains denied

The normal PST065 C2 receipt now projects each existing first-reached native resource slot into a private versioned by-value view before aggregating observed process peaks. It records the original PID/creation, owner-thread references, mapping, root, request/operation/generation/epoch, phase/witness, lifetime, local GetTickCount interval, metric/unit/scope and presence. Export does not observe again, renew expiry, compare clocks between processes or grant freshness.

The existing normal SDK digest and NSS/x86 identifier describe the compiled package target. They do not attest the mapped provider DLL or prove a provider/kernel ceiling. Policy, applicable expiry and recipe remain absent; forged presences are rejected. Missing observations remain incomplete. Retained/stopped/destroyed snapshots stay historical; none enables a new operation. c2_target and c2_capacity remain fail-closed; no Boolean approval setter or additional owner/collector is installed.

VC6 x86 candidate and production Debug/Release builds pass at the canonical /W3 level with /WX. Two typed-view fixtures pass 4,071 checks each, including independent concurrent projections and identity/generation mismatch. Six production smoke runs and ten candidate smoke/offline controller runs pass. No network or service operation was executed. The view is 216 bytes of local scratch; facts/context persistent layout is unchanged. The actual VC6 c2_resource_json frame changes 376->568 Debug and 324->536 Release bytes; callee/CRT stack and runtime totals remain separate. No memory or handle credit is granted.

Evidence and exact hashes: C:/Projetos/PapinhoAccelerator/docs/integration/n0start03-integrated-closeout-20261024/q4-target-evidence-closure-20261025/browser/BROWSER-CLOSEOUT.md. Quantitative policy, evidence-backed provider/kernel recipe and actual composed qualification remain pending. CLIENT_ADMISSION=DISABLED; SITE_TLS_ADMISSION=DENY; ED11=PARTIAL.


## 2026-10-26 — Option B private owner provider snapshot

The existing candidate API3 PST owner now has papinho_pst_owner_provider_snapshot. It reads the public provider metadata under the existing registry lock and validates original owner identity/generation/thread, admission/close/retention state, output alias and post-query continuity. It does not initialize/release/query-after-close, change first failure, allocate a new owner, log, install capacity or grant credit. Expected NSS backend identity is checked; provider ID/initialized flag do not attest mapped module bytes or measure provider memory. The caller must retain a stable owner reference and independent output storage, and bind the read to its existing phase/clock/provenance; no timestamp is renewed by this metadata-only getter.

VC6 x86 candidate Debug/Release products PASS /W3 /WX, zero warnings; actual extracted getter fault fixtures pass35checks per build with public APIs simulated, not a native active PST Connection. Six rebuilt candidate smoke runs PASS without network. Owner layout remains148bytes; public metadata output60bytes and emitted own frame84/64 Debug/Release bytes remain separate from callee/CRT/allocator/kernel cost. No new persistent field or reserve/refund logic. Production0.6.4 remains unchanged. Current c2_target/c2_capacity remain absent-policy/DENY.

The Accelerator side also integrates passive C8 history/current-binding copy and a C3 owner metadata getter. Complete active pair evidence, mapped provider provenance and target quantitative freshness/headroom are still missing. D0–D9 revision2 is PROPOSED; B was selected technically only. No Q4 readback or operational effect. Canonical evidence: C:/Projetos/PapinhoAccelerator/docs/integration/n0start03-integrated-closeout-20261024/d0-d9-bounded-diagnostic-policy-20261026/CONSOLIDATED-CLOSEOUT.md.
