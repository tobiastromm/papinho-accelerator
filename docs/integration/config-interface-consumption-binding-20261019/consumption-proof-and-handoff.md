# Consumption binding proof and handoff

## Implemented scope

Officially reviewed exact operations: operations.json (four source/test files), selected-lifecycle-operation.json (test-only supplement), live-operations.json (two factual documentation appends). No rejected operation was retried. Normative decisions unchanged.

The actual consumer is papacc_server_launcher_activate in server_c8_resource_root_win32.inc. It copies participants.targets[0] to the listener entry and calls papacc_tcp_socket_win32_bind_owned. config_source_resolve is an exported offline bridge with no product consumer found; returning a resolved value there does not prove listener consumption.

| Function/boundary | Owner/lifetime | Fact and disposition |
|---|---|---|
| prepare_listener | canonical owner thread, participant-owned immutable copy | Native PersistentID copied before temporary enumeration storage disappears. Existing invocation/preparation revision and Stop guards remain. |
| interface_bind_committed_current | existing root transaction | Existing hook can stamp real post-commit R/A/P. This task neither publishes CURRENT nor executes its positive branch. |
| configuration_interface_current | same owner, real CURRENT pin | Existing identity/revision/Stop/retention checks. topology_proven and resolved remain false without proof. |
| interface_consumption_check | synchronous owner call, stack report | SELECTED only; requires current_bound, topology_proven and resolved. No query, allocation, callback, worker, pin acquisition or authority mutation. ALL is NOT_SUPPORTED by this helper. |
| activate, before acquisition | same owner/participant lifetime | SELECTED denied before activation reservation, heap, WinSock or socket effects. Existing READY/ED11/admission checks remain. |
| activate, immediately before bind | existing owned composition pin and resource lifetime | Rechecks the same guard. Failure follows existing RETAINED path; no alternate cleanup/retry. This second path is source-reviewed, not reached by the denied first guard. |
| Stop/rollback/retain | existing owner and Model E | Existing lifecycle unchanged. Tests prove denied activation after Stop, rollback and retention; no release by timeout/remote FINAL/process exit. |

## Topology limitation

bind_owned supplies IP/port/scope to native bind. It does not bind by PersistentID or interface_instance_id. SO_EXCLUSIVEADDRUSE is not interface identity proof. Pure snapshot equality proves relative observations only. No current producer supplies topology continuity/invalidation authority at consumption. Removed/disabled/renumbered interfaces, removed addresses, conflicts and the validation-to-consumption race therefore remain fail-closed, not positively qualified. No blocking topology query was added to Stop.

## Evidence and limits

Debug/Release final builds pass with zero compiler/linker warnings. Each configuration passes nine CTests and six dormant-root scenarios: 30 invocations total. Native enumeration supplies an actually observed PersistentID; diagnostic configuration/material remains TEST_ONLY and is never promoted to administrative authority. Tests check phase zero, no listener storage, no active network and no initialized WinSock on denied activation. Copy overwrite, revision mismatch, owner-thread mismatch, Stop, rollback and retention exercise existing lifetime mechanisms. Model tests cover topology divergence; physical adapters are not altered.

No positive CURRENT test, native bind/listen, port conflict, live publication or network was executed. No synthetic fixture proves operational applicability or native teardown. Retained diagnostic process exit is not DESTROYED evidence. New admissions, TLS, grants, Network Egress and named provider authority remain independent and denied/pending.

## ED11

Persistent root delta: zero. Existing copy 5928 bytes and metadata 80 bytes are counted once; root 182688, known managed 1807176, participant 6408. Existing preparation transient guard 10504. Actual SELECTED diagnostic combined reservation 1974219 within unchanged 2097152 limit; ALL 1974203. Difference 16 is existing selected-ID storage, not a quota increase. New helper uses one 96-byte stack report under the existing query scratch allowance; no heap/persistent snapshot/pin added. This is static/managed accounting, not native peak/headroom qualification. Provider/kernel UNKNOWN; freshness/headroom not invented.

## Next gate

Before any positive consumption claim: authorize a factual CURRENT-positive test separately; establish topology applicability at actual native consumption under accepted lifetime/Stop requirements; then qualify bind only under explicit network/admission authorization. Until then SELECTED stays DENY. Codec layout and transactional application remain separate gates; this patch does not implement them or publish configuration.
