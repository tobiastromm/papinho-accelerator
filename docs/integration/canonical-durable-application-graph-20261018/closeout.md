# Canonical durable application graph — storage/logger slice and native teardown stop

Task PAPINHOACCELERATOR-PHASE6-CANONICAL-DURABLE-APPLICATION-GRAPH-1.
Date2026-10-18. HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a, main.
CURRENT_TASK_COMPLETED=NO
SLICE_COMPLETED=YES

## Source changes

The EXISTING application launcher now owns static network, console, configuration and logger objects. It copies the CLI configuration and keeps the transferred CLI persistent-ID allocation alive through the runner; previously those IDs were freed before the Secure listener copied their view. The existing console logger sink uses CRT stderr, whose process lifetime covers this executable's callbacks. No new service, application owner, listener, PST owner, thread, scheduler or operational C2 path was added.

With PST compiled in, the existing run function's acceptor, IO loop, egress source, resolved security/material views, Secure composition, listener/configuration set, Secure IO, outbound TLS/log adapter and persistent allocation pointers now have static storage. These are the objects whose addresses are borrowed across the run frame; temporary enums, loop indices and call-only runtime/trust option descriptors remain automatic. Original heap arrays stay under the same owners; nothing was indiscriminately moved to heap. A serialized one-invocation guard prevents overwrite/reuse of PST-bearing storage, including retained storage. The non-Secure/no-egress path, which never creates PST children, releases that guard after its original cleanup so its sequential invocation behavior remains available. This behavior was source-reviewed and compiled, not network-executed.

server_application_lifetime_win32.c holds fixed metadata of that SAME application owner, not a second application graph. Canonical main registers its actual static parents. A Secure/egress caller without that parent registration is denied before run allocation. This is a new private lifetime precondition, not an authentication/authorization gate or proof of CURRENT. Canonical registration call sites provide durable objects; arbitrary external callers cannot infer lifetime merely from passing pointers and must honor the documented private ownership contract.

The bridge owns its logger copy and forwards synchronously to the existing canonical sink/context. A non-waiting interlocked gate excludes seal while a sink callback is active; nested/contending logging is dropped rather than waiting or recursing. No record/message pointer is retained. Seal clears forwarding pointers and leaves a permanent bridge tombstone, so late callbacks do not dereference an ended external context or reach another invocation. No re-registration is permitted, including after seal. RETAINED keeps parent objects and forwarding context alive; no logger finalization/cleanup is forced. Seal means logger forwarding sealed ONLY, never application FINISHED or native resources DESTROYED.

On unresolved Secure stop/shutdown the existing runner now records RETAINED before returning. Canonical main then skips parent network/console/CLI-ID cleanup. Parent objects and referenced heap remain reachable. After successful existing cleanup, released storage aliases are cleared. C2 and material lease participants remain unchanged; no epoch, grant or administrative readiness is manufactured.

## Ownership graph

| Node / dependency | Owner / acquisition | Borrowers and validity | Quiescence / disposition |
|---|---|---|---|
| network, console, config | Same main launcher; static storage; CLI IDs transferred from parser | IO/Secure/listener/config views | Retained on unresolved run; native cleanup evidence limitation below |
| external logger/sink | Static main logger, console sink function, CRT stderr | Owned bridge -> stable sink/context | Seal only with bridge gate idle; late forwarding blocked; process exit is containment |
| logger bridge/metadata | Same application lifetime, fixed88 bytes, one registration | Runtime log adapters and IO log views | No heap, no wait, no callback reentrancy; tombstone not reused |
| acceptor, IO loop, egress configuration | Same runner; static with PST enabled | Protocol slots, managers, callbacks, DNS/config views | Original owner shutdown order retained; unresolved Secure path does not free them |
| runtime/trust/outbound TLS/log adapter | Existing creation APIs, same owner; pointers and adapters static | Secure children, outbound TLS and native log callbacks | No native lifecycle behavior changed; factual API3 release proof remains pending |
| Secure composition/resolved material | Existing resolver/composition, static aggregate plus original owned material heap | Candidates, Principal Resolver, authorization context, Secure loop | Kept with unresolved children; no early parent cleanup on retained return |
| listener config/set | Same static runner objects; CLI-ID view now kept by main | Secure IO | Aliases cleared after existing confirmed shutdown path |
| candidate/member/event/Connection/Session/processor arrays | Existing bounded allocations and managers, pointer roots static | Scheduler/transport callbacks and protocol managers | Retained as one graph on unresolved Secure stop; original release order preserved |
| C2 dormant participant | Previous private component, unchanged | Future root transaction only | No Session/operation/epoch; existing rollback/retention evidence preserved |
| material/lease transaction | Previous private component, unchanged | Future root source borrowers | Not silently installed into normal run; existing material-only states remain distinct from commit/CURRENT |

## Concrete remaining STOP

The storage/logger slice does not complete factual native teardown. Existing src/platform/win32/tcp_socket_win32.c papacc_tcp_socket_win32_close discards closesocket's result then clears the handle metadata. Existing tcp_platform_win32.c shutdown discards WSACleanup's result and clears initialized. Listener/network shutdown can then discard their containing storage. Existing pst_secure_processor.c still uses void API2 pst_connection_release and clears the processor. These pre-existing source paths cannot establish API3 OK+DESTROYED or preserve all uncertain native ownership facts under failure. No native failure occurred in this task; this is a static finding, not a platform crash claim.

Therefore full MODEL_E_EXACTLY_ONCE, full native retained-graph correctness and application FINISHED are NOT qualified. No clean FINAL was introduced. The task's STOP rule for unsafe/unknown lifecycle prevents promotion to completed root readiness. These existing owners require focused factual failure reporting/retention and API3 consumer migration as applicable before claiming complete root teardown or operational integration. No substitute cleanup, destructive retry, permissive wrapper or invented native outcome was added. This is an implementation limitation, not a new normative conflict, SDK defect or executor rejection.

## Builds / tests

MSVC19.51 x64. New outputs only under build/canonical-durable-graph-20261018.
- Debug/Release real main.c and server_run_win32.c compiled as objects with Secure/PST enabled against normal API3/SPI4, /MD, /W4 /WX, zero warnings. This does NOT link or qualify the full API3 application/processor.
- Debug/Release papacc_server foundation WITHOUT PST enabled compiled and linked successfully. Executables were never run. This is not a production0.6.4 PST-linked build or a normal API3 full-service build.
- Debug/Release lifetime test and census executables linked successfully, zero warnings.
- Final offline suite3/3 PASS per configuration: logger lifetime, retained lifetime, compiled-layout census. Two logger cases each have13 assertions. They prove static parent/bridge survival after the preparing helper returns, wrong identity/re-registration denial, busy/reentrant seal denial, seal/tombstone rejection of late callbacks, retained forwarding with preserved stable context, and no re-registration/reuse. They use controlled records and static fixture contexts, not PST/native network callbacks.
- No actual listener, Session, TLS/mTLS, native PST child, runtime, network, service, E10, C8 empirical, Legacy or NT4 execution. Native Stop/retention/late callback and full API3 linkage remain pending. No synthetic case is promoted to native Model E proof.

The first configure attempt failed because two appended CMake blocks lacked a separating newline; the syntax was corrected and the initial log preserved. Final builds have no warnings/errors. The final guard/alias correction required recompiling the actual run objects; unchanged bridge tests were not unnecessarily repeated afterward.

## ED11

Compiled x64 sizeof subtotal for explicit ONE-listener coexistence profile:
root/static requested18096; base arrays257536; Secure arrays19256; dormant C2 plus source participant528776; material credit32768; subtotal856432 bytes. Metadata bridge88 is included once. C2 manager payload/metadata is embedded in its participant and not counted twice. The source transaction is416 and C2 participant528360 from actual compiled layouts. Baseline connection capacity remains64; this is not a new admission capacity approval.

This is a compiled-layout requested-byte SUM, not a complete aggregate memory reservation or measured process total. Per-symbol/linker padding, CLI ID multiplicity, allocator bookkeeping, preparation/runtime peaks, stack, provider/kernel and full live lifecycle resource costs still require complete composition. Unknowns were not zeroed. No historical census or Browser budget imported. Existing managed limit2097152 unchanged. Requested subtotal fitting the limit does NOT qualify ED11 total. Retained static/heap remains resident/charged, with no eviction or quota release inferred from process exit.

CLIENT_ADMISSION=DISABLED for new C2/C8 profile; no new listener/admission is enabled. Existing foundation behavior is not reclassified as Mode C production.
ED11_ADMISSION=DENY
ED11_TOTAL_STATUS=PARTIAL

## Preservation / next gate

Baseline425 existing modified/untracked files captured. Only CMakeLists.txt, apps/server/main.c and apps/server/server_run_win32.c changed among them; their exact preimages preserve prior WIP. All422 other baseline entries match. server_run_win32.h was also preserved but unchanged. New sources/tests/evidence are additive. Source diff, hashes and output manifest accompany this report.

Normal SDK46 checksum entries verified. Preserved archive identity and all six historical executable members verified byte-exactly. Historical sources/SDKs/Browser/PST were not written. Two historical map preimages remain unrecovered. Production pin file SHA256 remains3D347A0606C7AD03D2ED02B593DABA8853A2C627F85A146F28FB90C94BA05C39. Fourteen scoped executor calls including documentary closeout approved, zero denials. No bypass, ACL/ownership/Git trust change, clean, clone/worktree, Git publication or PST promotion. No test/build processes remained at final observation; git diff --check0.

APPLICATION_ROOT_DURABILITY=STATIC_STORAGE_REMEDIATED_FULL_NATIVE_LIFECYCLE_PENDING
LOGGER_LIFETIME=CANONICAL_STATIC_PARENT_AND_SEALED_DURABLE_BRIDGE_OFFLINE_VALIDATED
SECURE_DEPENDENCIES=TRANSITIVE_RUN_STORAGE_DURABLE_NATIVE_RELEASE_PENDING
TRANSITIVE_BORROWERS=ROOT_LOGGER_VALIDATED_NATIVE_CALLBACKS_PENDING
MODEL_E_RETENTION=ROOT_PARENT_RETENTION_IMPLEMENTED_NATIVE_FAILURE_OWNERSHIP_PENDING
MODEL_E_EXACTLY_ONCE=NOT_QUALIFIED_NATIVE_TEARDOWN_STATUS_MISSING
CONFIG_COMMITTED=NOT_CLAIMED
POLICY_CURRENT=NOT_CLAIMED
EPOCH_FACTUAL=NOT_ISSUED
BUILDS=SECURE_OBJECTS_AND_FOUNDATION_LINK_DEBUG_RELEASE_PASS
WARNINGS=0_FINAL
OFFLINE_TESTS=3_OF_3_PER_CONFIGURATION
NETWORK_TESTS=NOT_RUN
C2_OPERATIONAL_DISPATCH=NOT_INTEGRATED
C3_SITE_TLS=NOT_INTEGRATED
C8_INTEGRATED_QUALIFIED=NO
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_WITH_PREIMAGES
EXECUTOR_APPROVALS=14_APPROVED_0_DENIED
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=NATIVE_TEARDOWN_OWNERS_DISCARD_FACTUAL_FAILURE_RESULTS
NEXT=FACTUAL_NATIVE_TEARDOWN_STATUS_AND_RETENTION_BEFORE_ROOT_C2_WIRING