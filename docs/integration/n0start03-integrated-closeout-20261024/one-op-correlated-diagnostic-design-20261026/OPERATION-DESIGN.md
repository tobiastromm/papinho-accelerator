# One-operation correlated Browser/C2/C3/provider diagnostic

Technical direction: ONE logical Mode C operation. No operational identity/intent or objects are created. The current normal route remains DENY. This design is conditional on an explicit limited diagnostic policy decision and a later exact manual operation approval; neither is implied by this document.

## Topology from real sources

| Existing component | Operation role and lifetime | Accounting |
|---|---|---|
| Browser candidate VC6/x86/NSS | Existing UI, navigation/request worker and canonical shared process root/credential/session owners. Exact TIDs/creation and any transport/worker bridge threads must be observed; do not assume they are all the same thread or count them twice. | One process ledger, existing controller/worker ownership; provider/CRT/internal threads UNKNOWN |
| Browser C2 Session | CONTROL plus APP and RECORD streams under one C2 child context and borrowed existing runtime; independent ordinary-service parent may already live. | Child reservation1,858,020; parent/shared runtime counted once; existing parent not zero incremental cost |
| Browser Site pump | One raw Site TCP socket; RECORD carries TLS records to/from Accelerator. Browser owns actual destination networking. | Real socket lifetime/options and close facts, not a per-socket kernel ceiling |
| Accelerator candidate Selected normal x64/OpenSSL | Existing canonical launcher root, C2 participant/manager, serialized secure-I/O owner and shared scheduler/runtime. C2/C3 run on validated existing owner thread; no new collector thread or owner. | Separate process ledger; no Browser private-memory subtraction or transferred credit |
| Accelerator C3 | One PST TLS client child, two embedded bounded queues; setup uses a loopback listener, connector and accepted peer. Listener closes once before connector transfer; connector ownership passes through existing socket attachment; peer remains bridge-owned. | Source allows a transient three-socket setup, then two loopback endpoints with different owners. No new Internet Site socket in Accelerator; do not infer total handle peak from this inventory |
| Site TLS peer | Existing authorized external endpoint, not a new local test server. Its hostname/SNI/trust/configuration must be bound. | Remote endpoint resources outside two local ledgers; DNS/kernel/transport effects remain within required local evidence where applicable |
| Existing console/supervision | Manual supervision, any existing stdout channel and observer remain explicitly inventoried. | Do not import LAB four-role/1024/128 allowance as product capacity; preserve those limits in their actual applicable scope |

Source boundaries: Browser papinho_browser.c route around10250, papinho_c2_s6.inc preparation around520 and cleanup around490; c2_budget.inc; Accelerator c3_bridge_win32.c private_pair/init/attachment and server_c2_owner_win32.c serialized checks. Artifact/recipe hashes bind exact inspected bytes; line references are navigation aids. Production0.6.4 is excluded.

Choose a single text/plain HTTPS response under an ALREADY approved exact destination/profile, no redirects or subresources, one requested navigation, no retry or background operation. This is a workload restriction to verify, not an implemented global prohibition. Unexpected background workers/connections or a second operation => no admission/Stop under permitted scope, retain facts. No new listener/service/test origin is authorized here. Actual endpoint and administrative roots remain UNBOUND.

## Phases and source witnesses

1. Baseline: existing Browser controller and child preallocation samples; Accelerator original producer BASELINE/PREPARED. Baseline is not before process/DLL initialization and must name its scope.
2. Initialization/setup: Browser child native slots B/C, actual Session/group/CONTROL and provider getters under held owner borrow. Accelerator raw producer plus C3 setup/child and public provider info. An initialized0 runtime-only sample cannot cover this phase.
3. Handshake/roles: Browser bind acknowledgements slotD; TLS_READY slotE; existing protocol-receipt correlation joins roles. C3 PST peer facts must verify TLS1.3/chain/name/auth/ALPN under current Site TLS profile; no trust or TokenGroups predicate relaxation.
4. Active: Browser first reached prefix boundary slotF, canonical flow counters, C3 queue/prefix lifetimes, existing Accelerator collection outside dispatch. Samples are boundary samples, not whole-transfer peaks.
5. Stop/drain: existing Stop/cancel propagation, Browser slotG, Accelerator shutdown/draining history. Stop requested/accepted/completed are distinct; remote FINAL does not close local dependencies.
6. Final/retained: Browser slotH plus controller worker join/handle close, existing release receipts and retained charges; Accelerator copied durable final/retained history while owner still exists. Never query a destroyed PST child or synthesize a terminal sample. If final copy is unavailable, keep absent/UNKNOWN.

## Instrumentation qualification and integration boundary

Reuse existing native resource producers, C2 receipt/adapters, mode_c_pair verifier and provenance checks. Additive offline assess_one_operation_coverage validates bounded private analysis annotations against the EXISTING receipts and exact artifact bytes. It checks one join, local PID/creation/image/SDK/root consistency, backend, phase order/replay, local clock coherence, and ACTIVE prefix values against original receipt facts. It never authenticates arbitrary JSON and never grants capacity, even with all fields. Process peaks remain separate; private and commit overlap, pool quotas are partial; socket options are configuration facts, not kernel allocation totals.

The isolated owner-provider-readonly-probe.h prototype calls only public runtime/provider, connection/provider and release-info getters using a required existing serialized borrow validation before/between/after calls. It has no heap, handles, logger, release, retry, owner or policy. Snapshot sizeof168 bytes in both tested architectures; callee stack/total memory is NOT measured by that number. Failure preserves first getter failure and invalidates completeness; held-owner release/Stop remain the caller's obligations. Matching provider IDs do not attest mapped modules. Retention is copied, never converted to CLOSED or credit.

The prototype is NOT integrated into the operational owners. Its borrowed objects/getters are simulated in qualification, not a new native TLS Connection. Integration must use actual existing owner borrow/Busy/lifetime and output alias checks, never a caller Boolean. No getter may run after freeze or destruction, no reentrant callback during the borrow. Remaining integration sites: Browser c2_prepare_observed/c2_receipt_copy and existing worker lifetime, Accelerator existing C3 owner windows. Keep the same existing receipt/output boundary; no new recorder or generic IPC.

Concrete current blocker: Accelerator typed projection/owner export rejects network-active/dispatch, and provider_runtime is deliberately UNKNOWN in its version1 diagnostic contract. Raw target_refresh can observe ACTIVE outside dispatch but does not make the restricted typed export legal. Do NOT remove that guard. A future bounded observation snapshot/copy must be explicitly integrated in an allowed owner window after normative scope resolution. No schema promotion or fabricated active fields now.

Kernel/socket and mapped-provider collection are not newly implemented by the prototype. Actual source handles/owners must supply read-only socket option/object associations without transferring ownership; mapped images need protected original file/object evidence, not just compiled SDK hashes. Existing external observer can report process identity/metrics but its own decoder deliberately leaves phase association/provider/kernel_max incomplete. It cannot be relabelled a complete phase observer.

## Exact static binding and unknown identities

STATIC-ARTIFACT-BINDINGS.json verifies current normal candidate Browser and Selected Release server bytes and controlled layout sources. These are pre-integration artifacts; future instrumentation integration requires rebuilt hashes and derived pins. Original PID/creation, boot/operator, committed root/CURRENT/epoch, approved Site TLS, destination and mapped module identities must be freshly bound. Null fields are missing facts and block execution. No GUID/token/file ID is invented.

## Freshness, resources and Stop

Use accepted local source/creation domains; no cross-process clock comparison, wrap repair, export-time renewal or shared Session-ID inference. Diagnostic annotation checks time coherence only. Quantitative target max-age, collection validity and host headroom remain unapproved, not zero or inherited from LAB/C8. Root/profile/backend/module/workload/lifetime change invalidates reuse. Existing local tick/prefix sequence replay and origin mismatch reject evidence. Retention cannot extend freshness.

Controlled Browser charge fits only the narrow fixed arithmetic. Actual paths include outer524,289-byte response and child variable output+NUL, request/configuration/session/root allocations, allocator overhead, threads/stacks and provider/kernel effects. The existing charge's conservative components are not a proof that all these allocations are disjointly covered. Do not declare an undercharge or consume slack without complete lifetime attribution; record the allocation categories and observed coexistence.

Preserve2MiB Browser and existing Accelerator2MiB/default or3MiB/Selected profiles; no quota transfer. Unknown active provider/kernel category or controlled lifetime means DENY under current policy. Stop only through already authorized existing owners when factually safe; no forced kill, deletion, cleanup, refund, retry, rollback or release of uncertain dependencies. E19/E20 cannot pass from isolated probes or a sampled maximum.
