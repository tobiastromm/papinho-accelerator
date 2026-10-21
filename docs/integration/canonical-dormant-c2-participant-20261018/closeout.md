# Canonical dormant C2 configuration participant

Date: 2026-10-18. HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a, main.
Task: PAPINHOACCELERATOR-PHASE6-CANONICAL-DORMANT-C2-CONFIGURATION-PARTICIPANT-1.
CURRENT_TASK_COMPLETED=YES
SLICE_COMPLETED=YES

## Implemented scope

The default-OFF PAPACC_ENABLE_DORMANT_C2_PARTICIPANT target builds a private configuration participant. It is not linked into the application launcher/listener and adds no second running owner. apps/server/server_c2_participant_win32.c prepares the actual historical C2 operation manager with closed policy/capacity gates. Preparation success follows actual manager initialization, not a caller readiness boolean or a synthetic preparation callback. The historical operation manager initializer is extracted without functional changes into c2_operation_dormant_win32.c. Four historical internal headers are copied byte-exactly for the existing manager layout; no operational implementations or permissive stubs accompany their declarations.

The participant validates the existing canonical Configuration Model, requires Secure profile, rejects egress and unsupported enabled capabilities, verifies enabled keys against the actual capability registry/support sets, requires TLS support when Mode C is enabled, and copies model/interface IDs/enabled keys. No caller model, registry, key or interface-array pointer survives preparation. The root identity address is an opaque non-owning identity, never dereferenced or interpreted as authentication. It and the participant storage must be durable, root-owned and noncopyable for their lifetime. Identity checks cover participant address, root address, invocation and actual current thread.

No Principal, Session, Connection, grant, token or epoch is created. No policy evaluation is treated as CURRENT. A future root must still prove all other participants, native durable commit, policy CURRENT and subsequent factual epoch issuance. This target has no API to mark commit/CURRENT or issue an epoch. Preparation is not runtime support qualification, policy authorization, configuration publication or operational admission.

## Transition/dependency matrix

| Transition | Factual requirement | Effect / failure |
|---|---|---|
| EMPTY -> READY | Fresh zero storage; root/invocation; valid canonical configuration and capability sets; full checked storage charge; successful real manager init | Empty initialized manager, copied configuration; no dispatch |
| Invalid prepare | Missing identity/configuration/dependency; unsupported configuration; insufficient budget | Reject before manager ownership/effects; storage stays EMPTY |
| READY validation | Same address/root/invocation/thread; intact charge/model; empty manager, unchanged deny gates | No authority or mutation |
| READY -> CLOSED | Same identity; empty never-consumed manager; no Session/operation/callback state | Roll back manager and component charge; preserve CLOSED identity tombstone |
| CLOSED -> CLOSED | Repeated Stop/rollback with matching identity | Idempotent, no second release |
| READY -> RETAINED | Unexpected pending/dependent state or invalidated prepared state on Stop | Preserve manager, references and full charge; no cleanup fallback |
| RETAINED -> anything | Not authorized by this component | Reject; no early reuse or retry cleanup |
| READY -> COMMITTED/CURRENT/EPOCH | Not implemented here; future root transaction authority required | No positive path |

The source is derived from historical server_configuration_participants_win32.inc prepare_c2 and rollback logic. Its bounded manager initializer and types are reused. The historical full launcher, provider transaction, empirical harness, IO owner and server_c2_configuration_apply/associate are not imported. Future integration must use this same manager, not allocate a duplicate. When adding the full operational manager translation unit later, move/share its initializer rather than linking duplicate definitions. Registration in the existing root transaction remains the next gate, deliberately separate from this participant component slice.

## Offline evidence

MSVC 19.51.36256 x64 /W4 /WX on new targets. Debug and Release compile/link PASS, zero warnings/errors in initial and final logs. New outputs: build/canonical-dormant-c2-20261018/{Debug,Release}. No clean or historical output overwrite. No PST library is used by this dormant target; production and normal/R2 selections remain unchanged.

Final CTest: four of four PASS in each configuration: papacc.c2.dormant_participant, papacc.server.configuration, papacc.capability.framework, papacc.papinhopacc.wire_golden. The focal executable reports 43 assertions per configuration. Earlier 40-assertion runs are preserved as intermediate evidence, not counted as separate qualification coverage.

| Required case | Evidence / scope |
|---|---|
| Participant absent/present | Null/EMPTY validation refused; actual initialized participant validates |
| Valid/rejected prepare | Actual config validation, bad port, missing TLS prerequisite, invalid invocation, insufficient budget |
| Rollback/failure before commit | READY rollback succeeds only empty; rejected preparation leaves no initialized manager |
| Commit unproved / epoch absent | Structural source boundary: no commit/publication/epoch API or call; no operational eligibility follows READY |
| UNKNOWN/admission | Both installed functional gates return DENY unconditionally, including without any target/Principal/Session evidence; this is not a positive ED11 target measurement |
| Stop/repeated Stop | Empty Stop closes; repeat does not release twice; reprepare of tombstone rejected |
| Retention | Controlled synthetic pending callback field prevents rollback; charge/reference remain; repeated rollback rejected |
| Identity/copy lifetime | Wrong root/invocation rejected; caller config and selected-ID mutation do not alter copied participant snapshot |
| No dispatch/admission | No operational implementation linked; zero Session/operation; policy/capacity deny |
| Budget boundary | Exact 2097152 combined bytes accepted for preparation only; arithmetic excess rejected |

These are real local component execution and synthetic input/error cases. They are not native PST teardown/Model E release, authenticated Session, listener, provider/kernel or integrated C8 qualification. Failure-before-commit is precommit preparation rollback; no native durable commit failure was exercised because persistence is outside this target.

## ED11 and lifetime

Identical compiled x64 sizes in Debug/Release:
- participant 528360 bytes, including manager 527808, canonical config 352 and all IDs/keys/identity/accounting/alignment;
- one future root-owned participant is required, not another manager plus this manager;
- combined checked reservation = other_reserved + sizeof(participant), cap 2097152 unchanged;
- standalone headroom after one participant = 1568792 bytes, not an integrated-root capacity claim;
- unexpected retention keeps all 528360 charged;
- CLOSED clears component logical reservation only after zero-child proof; the containing fixed storage physically remains resident and must remain in a future process static-memory census;
- no heap, network, filesystem, waits, PST calls or callback reentrancy in preparation/validation/rollback;
- large objects are caller-owned durable storage, not automatic function locals. No compiled stack high-water measurement was performed. Small automatic variables/pointers only; future root census must include callers and stack;
- offline harness owns three static fixture participants, 1585080 bytes, separately from production/root reservation. Its retained fixture is intentionally preserved through test exit; exit is not proof of resource destruction.

Provider/kernel UNKNOWN remains UNKNOWN. No historical 2MiB census or Browser budget was imported. All other root/provider/lease/listener metadata and future authority storage remain to be composed and counted. ED11_TOTAL_STATUS=PARTIAL.

## Preservation / provenance

baseline.json hashes 383 existing modified/untracked files before implementation. Only CMakeLists.txt changed among them; its exact prior content is preserved as CMakeLists.txt.preimage. All other 382 baseline files compare byte-exactly. New sources/tests/evidence are additive. Historical reference is read-only. Header provenance and before/after hashes are in source-hashes.json. source.diff includes new files and the focal CMake change.

SDK normal: all 46 entries in SDK-normal/SHA256SUMS.txt verified. Historical COMPOSITE-BASELINE-BINARY-PREIMAGES.zip matches its recorded SHA256 and all six expected binary members match their original SHA256. Neither package nor historical bytes were written. The two missing historical .map preimages remain NOT_RECOVERED; this task does not repair them.

Seven scoped official executor calls, including this documentary closeout, were approved; no denial or alternative write mechanism was used. No Git trust, ACL or ownership changes. git diff --check exit0; no test/compiler/linker/ninja processes remained at final observation.

DORMANT_C2_PARTICIPANT=IMPLEMENTED_OFFLINE_VALIDATED
C2_PREPARATION=REAL_EMPTY_MANAGER_AND_VALIDATED_COPIED_CONFIGURATION
CONFIG_TRANSACTION=PARTICIPANT_COMPONENT_READY_ROOT_WIRING_PENDING
COMMIT_FACTUAL=NOT_CLAIMED
POLICY_CURRENT=NOT_CLAIMED
EPOCH_ISSUANCE=NONE
CLIENT_ADMISSION=DISABLED
ED11_ADMISSION=DENY
BUILDS=DEBUG_RELEASE_PASS
WARNINGS=0
OFFLINE_TESTS=4_OF_4_PER_CONFIGURATION
NETWORK_TESTS=NOT_RUN
C2_OPERATIONAL_DISPATCH=NOT_INTEGRATED
C3_SITE_TLS=NOT_INTEGRATED
C8_INTEGRATED_QUALIFIED=NO
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_WITH_CMAKE_PREIMAGE
EXECUTOR_APPROVALS=7_APPROVED_0_DENIED
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=NONE_WITHIN_PARTICIPANT_SLICE
NEXT=SCOPED_FACTUAL_ROOT_POLICY_INTEGRATION