# Native NtCreateFile status preservation and LAB diagnosis — 2026-10-23

TASK=PACC-PHASE6-NATIVE-NTCREATEFILE-STATUS-DIAGNOSIS-1. Branch main, HEAD913c90cf2328f7939d8f5b927640601903ac6e91. Implementation/offline qualification complete; original failure diagnosis remains unresolved. No new Start.

## Implementation and authorities

The preceding [diagnosis](../first-native-lab-readiness-access-denied-20261023/closeout.md), focal proposal/postimages/manifest, failed-Start evidence, protected-runtime proof, accepted virtual identity V0–V8, Model E/pre-close/Stop and ED08/ED11 remain the authority/evidence set from this continuous task. Relevant future listener/live-config direction is unchanged; its separate CURRENT conflict is not affected. PROJECT-CONTEXT.md is absent. No normative record changed.

All four proposed source preimages matched. Official exact executor review approved their SHA-checked application, followed by a separately reviewed model-test hunk. See evidence/source-manifest.json. Original PROPOSED artifacts remain preserved as history.

The private recorder is enabled only by PAPACC_PSR_TRACE_LAB with the existing fixed LAB runtime/registration profile; default OFF. It preserves API return/LastError and first failure: API/source line, object role, access mask, raw NTSTATUS/native-valid flag, predicate, returned error and actually queried bounded identity data. No passwords, private material, pointers or token handles. Missing identity facts stay unavailable. No new worker, service, registry or authority.

The compatibility return from psr_open remains Win32 5 for negative NTSTATUS: changing authorization or normalized behavior is outside this task. The original NTSTATUS is now separately preserved before that conversion, rather than lost. No status-to-Win32 mapping table was invented. Interpret NTSTATUS only when nt_valid=1; raw_win32=0 then is not native success. Propagation cannot overwrite first failure. PARENT/CONFIG/IMAGE roles identify the existing fixed native objects, not arbitrary authorized paths.

## Qualification

Selected/Legacy Debug/Release scoped host/service/runtime/console/effect-plan/census/issuer-gate builds PASS, /WX, zero warnings. Same MSVC19.51/Ninja/candidate SDK and existing budgets. New outputs only under build/ntcreatefile-status-diagnosis-20261023; no historical or registered executable rebuilt/replaced. Explicit diagnostic build flag /DPAPACC_PSR_TRACE_LAB=1 is not a runtime/config switch. Production pin0.6.4 unchanged.

Four configurations each passed14 suites:56 combinations. Native evidence is negative token/gate and closed-manager components, NOT a running service. The real psr_open is exercised with injected NtCreateFile calls; no native file creation. Tests verify exact access mask, returned error, first raw status/propagation and uncertain retention with/without a returned synthetic handle.

| Injected status | SDK value |
|---|---|
| STATUS_ACCESS_DENIED | C0000022 |
| STATUS_OBJECT_NAME_NOT_FOUND | C0000034 |
| STATUS_OBJECT_PATH_NOT_FOUND | C000003A |
| STATUS_SHARING_VIOLATION | C0000043 |
| STATUS_OBJECT_NAME_COLLISION | C0000035 |
| STATUS_INVALID_PARAMETER | C000000D |
| unknown model failure | E1234567 |

Six named values checked against installed SDK10.0.26100.0/shared/ntstatus.h. Unknown fixture preserved verbatim, not classified or attributed to the real failure. Existing runtime models cover Stop before/during readiness, quiescence, failed close/retention and identity/config divergence. Separate recorder Debug/Release tests against the applied header pass first-error/LastError preservation with a deliberately disruptive fake output sink.

Selected Release diagnostic artifact SHA256=051ff57e3bac81cb1823cf0685c93c7f795372a307c04138f26537351d0a9ebe. This is build identity only: not placed under protected parent, not registered or authorized for Start. All four hashes are in evidence/build-results.json.

## Read-only reconciliation

SCM still reports PapinhoAcceleratorLab20261023 STOPPED/PID0, Win32 exit5/service-specific0, DEMAND_START, previous quoted registered image plus service host. Virtual account and SID configuration preserved. Current six protected descriptors and recorded hashes/identities match approved evidence. Observation handles/descriptors were closed by the read-only helper; this does not release original references.

Historical qualifier PID28452 OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE) returns87; process-name query found no qualifier/host. Owner reported window no longer open. Original attempt563826bacc49449f954b20cc48ce9b36 still has retained log/intent and no final cleanup receipt. Original retained lifetime remains UNKNOWN: no executor release, kill or reuse. Window absence/PID0 is not original-instance cleanup proof.

Next Start is NOT READY. The missing original process-instance handle/identity cannot be reconstructed from PID0 or a reused PID. Minimum additional admissible evidence would be an existing exact-instance termination/retirement record and canonical-owner lifetime reconciliation, if available; not another Start or permission probe. Existing Security-log access was denied, not an empty result. No policy/privilege bypass or repeated blind query. Without evidence, a separately governed disposition decision is needed; no retirement guard is overridden.

## ED11

Static recorder delta120 bytes is charged to service storage accounting. No application thread/OS handle added; existing bounded token buffers are borrowed. Formatting has fixed local buffers including nested768-byte record and384+192-byte token locals. Transitive CRT/debug-receiver/OS stack/provider/kernel costs and retained coexistence remain UNKNOWN, never zero. No quota increase; ED11 PARTIAL. See [conditional handoff](manual-handoff.md).

```text
CURRENT_TASK_COMPLETED=NO
NTCREATEFILE_DIAGNOSTIC=IMPLEMENTED_LAB_ONLY_OFFLINE_QUALIFIED
ORIGINAL_NTSTATUS=UNKNOWN_NO_NEW_NATIVE_CAPTURE
FAILED_OBJECT=UNKNOWN_ORIGINAL_ATTEMPT
REQUESTED_ACCESS=UNKNOWN_ORIGINAL_FAILED_CALL_MODEL_MASK_VERIFIED
RUNTIME_STAGE=ORIGINAL_PSR_PREPARE_FIRST_FAILURE_NOT_CAPTURED
ERROR_PROPAGATION=FIRST_NATIVE_STATUS_PRESERVED_COMPATIBILITY_RETURN_UNCHANGED
PREVIOUS_RETAINED_REFERENCES=UNKNOWN_NO_FINAL_RECEIPT_NO_EXECUTOR_RELEASE
BUILDS=SELECTED_LEGACY_DEBUG_RELEASE_SCOPED_TARGETS_PASS
WARNINGS=0
OFFLINE_TESTS=56_SUITE_CONFIGURATION_COMBINATIONS_PLUS_RECORDER_DEBUG_RELEASE_PASS
NATIVE_TESTS=NEGATIVE_TOKEN_GATES_AND_CLOSED_COMPONENTS_PASS_SERVICE_CAPTURE_NOT_RUN
NETWORK_TESTS=NOT_RUN
START_RETRY=NOT_RUN
ACL_MUTATIONS=NONE
SCM_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES_7942_BASELINE_EXCEPT_FIVE_REVIEWED_SOURCE_CHANGES_AND_DOCUMENTATION
FIRST_UNRESOLVED_DIVERGENCE=PREVIOUS_INSTANCE_RETIREMENT_UNCONFIRMED_AND_NO_NATIVE_CAPTURE
NEXT=FIRST_NATIVE_LAB_READINESS_EXACT_CAUSE_CORRECTION
```
