# Canonical Secure source/lease participant — partial implementation

Task: PAPINHOACCELERATOR-PHASE6-CANONICAL-SECURE-ROOT-LIFETIME-AND-POLICY-PARTICIPANTS-1.
Date 2026-10-18. Canonical HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a, main.
CURRENT_TASK_COMPLETED=NO
SLICE_COMPLETED=YES

## Completed source slice

The existing canonical server_security_source_win32.c/.h now contain the historical transactional material/lease implementation byte-exactly. The same environment resolver, Principal Resolver and lab authorization callback remain the source; no parallel provider, resolver, administrative policy engine, application owner or listener was added. Exact preimages preserve both modified canonical files and CMakeLists.txt.

The participant owns decoded certificate/private-key/trust bytes plus bounded encoded staging through the existing loader. It prices requested allocation bytes before malloc against finite credit and checks other_reserved + sizeof(transaction) + credit <= 2097152. Preparation validates actual decoded material and existing configuration references. PREPARED is not native TLS readiness. Commit/publication methods describe local material lease availability ONLY, not application CONFIG_COMMITTED, policy CURRENT, authenticated Principal, C2 grants or epoch.

Borrowing returns one of eight exact local tickets and a read-only view; borrower count prevents finish. Unborrow rejects duplicate/stale tickets within the same non-reused transaction. RETAINED rejects new borrows, finish and rollback, preserves material and allows an existing borrow to be returned. CLOSED cannot be re-prepared. No PST release, retry cleanup, invented DESTROYED, runtime creation, socket or network call occurs in these material/lease tests.

The default-OFF PAPACC_ENABLE_SECURE_SOURCE_PARTICIPANT build requires the explicit normal API3 candidate/PST consumer boundary. Its isolated target builds the real canonical security/configuration/composition/bootstrap code against the governed normal x64 SDK. It excludes the API2 processor/release implementation and does not claim that the full application has migrated to API3. Existing runtime/PAL logging is reused rather than duplicated. Production dependency pin and candidate/R2 selections are unchanged.

## Ownership graph and unresolved boundary

| Edge | Lifetime / current status |
|---|---|
| source transaction -> decoded material and staging | Owned bounded heap, actual acquisition/release; retained on uncertainty |
| resolved material view -> certificate/trust items | Points into owned material; borrower must keep transaction valid |
| Principal callback -> resolver storage | Actual source-owned material context, unchanged identity semantics; no authenticated Principal produced by tests |
| authorization callback -> context | Existing stateless lab callback, NULL context; not Site TLS/CURRENT policy authority |
| composition inputs -> logger | Transaction-owned logger copy in transactional prepare |
| copied logger -> sink/context | Borrowed external sink context; copying logger does not make that context durable |
| IO loop/Secure -> composition/resolved config | Existing application still passes automatic objects from server_run_win32 |
| application -> network/console/config/logger | Existing main/server_run aggregates still require coordinated durable ownership and shutdown integration |
| root -> runtime/provider/credentials/trust/wait-set/callbacks | Not yet made transitively durable by this slice; no native acquisitions performed here |
| root -> dormant C2 participant | Previous private component remains unchanged; not attached to live application |
| root -> administrative policy/commit/epoch | Not integrated; no CURRENT, grants or epoch claimed |

STOP boundary: directly attaching the future root transaction or retained Secure leases to the current server_run_win32 automatic composition/configuration would leave references to an ended stack. In addition, a copied logger retains its external sink_context; that context needs actual durable ownership or a separately proved process-lifetime contract. The material participant API alone cannot fix those application lifetimes. This task did not refactor the running application graph, so SECURE_ROOT_DURABILITY remains NOT_RESOLVED. No direct unsafe wiring was added. This is an implementation gap, not an executor denial or a new normative conflict.

## Validation, including unsuccessful attempts

MSVC19.51 x64 normal API3/SPI4, /MD, /W4 /WX. New build outputs: build/canonical-secure-source-20261018/{Debug,Release}. Final Debug/Release build/link PASS, zero warnings. Full application/Model E native lifecycle was not built or executed.

Initial Debug build failed C1083 because a duplicate standalone log.c source lacked pal_time.h. The target was corrected to use the existing papacc_runtime/PAL library; the failed log is preserved. Initial Debug offline run failed one fixture assertion that incorrectly required the stateless authorization context to equal the release context. Source inspection proved it is NULL; only that test expectation was corrected. Both failed attempts remain recorded, not concealed as PASS.

Final CTest papacc.security.source_participant PASS in both configurations, 39 assertions each. It uses existing fixed certificate/base64 fixture files and process-local environment inputs. It reads actual files, decodes actual material, performs bounded actual allocations and exercises the real lease methods. No resolver/authentication success is fabricated. Callback pointer/context relationships are inspected; native TLS, Principal authentication, asynchronous callbacks, Stop of an active owner or provider/kernel qualification are NOT executed or proved.

Covered: credit rejection before allocation; insufficient material credit/failure cleanup; valid material preparation; validation; borrow denied before local publication; actual published view; borrowed finish denied; duplicate unborrow denied; quiescence FALSE denied; successful material finalization; duplicate finish/reuse denied; precommit rollback; explicit retained state; no new borrow in retention; existing reference return; retained finish/rollback denied. The tests use static transaction storage. Retained fixture remains retained through process exit; exit is not destruction evidence.

Not covered/completed: durable full root ownership, native PST release/disposition, native pending callbacks, real application Stop/quiescence, administrative policy participant, persistent root commit and postcommit epoch, integrated C2/C8. Existing dormant-participant tests were not rerun because their source/layout inputs did not change. Prior evidence is preserved and not promoted.

## ED11 component accounting

Compiled transaction size: 416 bytes in both configurations. Test material credit 32768; per-instance requested-byte reservation 33184 plus caller-supplied other_reserved. Actual observed requested-byte peak4057 and retained material2987 for these fixtures. Retention does not refund the credit. Four static fixture transactions cost1664 total; these are harness storage, not four production roots. No root aggregate compiled census was produced.

The source ledger counts allocation requests; allocator bookkeeping, caller stack and full process/kernel/provider costs are not thereby measured or bounded. They must be priced in the future root aggregate. Loader fixed stack arrays, staging coexistence, secure composition, runtime/provider and other owners remain distinct costs. No old C8/E10 or Browser census was imported. Physical memory, kernel/provider remain UNKNOWN; ED11_ADMISSION=DENY. A successful fixture allocation is not operational admission.

## Provenance and preservation

Baseline408 modified/untracked existing files recorded before writes. Only apps/server/server_security_source_win32.c, its .h and CMakeLists.txt changed among them; all405 other baseline entries match. Their exact preimages are retained. New test/evidence files are additive. Historical source/header copy identities, canonical pre/post hashes and binary hashes are in pre-post-sha256.json/final-manifest.json. SDK normal46 checksum entries verified; historical archive and six expected executables verified byte-exact. Prior participant artifact manifest verifies. Two historic map preimages remain unrecovered.

Nine scoped official executor calls including this documentation were approved; zero denials, no alternate mechanism or ownership/ACL/Git trust change. No clean, historical overwrite, Browser/PST edit, wire edit, pin change, commit/push/tag/release. git diff --check0. No test/compiler/linker/ninja process remained at final observation.

SECURE_ROOT_DURABILITY=NOT_RESOLVED_APPLICATION_GRAPH_PENDING
TRANSITIVE_DEPENDENCIES=MATERIAL_LEASE_COMPONENT_INTEGRATED_FULL_GRAPH_PENDING
SECURE_POLICY_PARTICIPANTS=SECURITY_MATERIAL_ONLY_ADMINISTRATIVE_POLICY_PENDING
MODEL_E_RETENTION=MATERIAL_COMPONENT_VALIDATED_NATIVE_PENDING
MODEL_E_EXACTLY_ONCE=MATERIAL_COMPONENT_VALIDATED_NATIVE_PENDING
CONFIG_COMMITTED=NOT_CLAIMED_UNTIL_FACTUAL
POLICY_CURRENT=NOT_CLAIMED_UNTIL_FACTUAL
EPOCH_FACTUAL=NOT_CLAIMED_UNTIL_FACTUAL
CLIENT_ADMISSION=DISABLED
ED11_ADMISSION=DENY
BUILDS=COMPONENT_DEBUG_RELEASE_PASS
WARNINGS=0_FINAL
OFFLINE_TESTS=39_ASSERTIONS_PER_CONFIGURATION_PASS_AFTER_RECORDED_FIXTURE_CORRECTION
NETWORK_TESTS=NOT_RUN
C2_OPERATIONAL_DISPATCH=NOT_INTEGRATED
C3_SITE_TLS=NOT_INTEGRATED
C8_INTEGRATED_QUALIFIED=NO
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_WITH_THREE_PREIMAGES
EXECUTOR_APPROVALS=9_APPROVED_0_DENIED
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=APPLICATION_SECURE_PARENT_AND_LOGGER_CONTEXT_LIFETIME_NOT_DURABLE
NEXT=COMPLETE_DURABLE_EXISTING_APPLICATION_GRAPH_BEFORE_C2_ROOT_TRANSACTION_WIRING