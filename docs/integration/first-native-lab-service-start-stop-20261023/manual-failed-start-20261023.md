# Manual first-Start failure: factual state and retained qualifier

2026-10-23. Attempt563826bacc49449f954b20cc48ce9b36. Original intent and incrementally flushed log preserved. Log SHA256 `037f207ee4e40c889a16af06173a551066eef7b53aca32507ed8e900414bf832`. Candidate creation confirmed by native output:417614100:589824:2690484, SHA25662087dbe13712c468dc92c5848de684cbc7cba6f921d15259f895fa8664d2716. Independent byte hash matches the approved source; no replacement or retry performed.

Read-only SCM query confirms the existing LAB record now points to quoted protected papacc_server_lab_host.exe plus service host, DEMAND_START, same virtual account. State STOPPED/PID0, Win32 exit5, service-specific0. Windows System/Service Control Manager event7023 independently reports access denied; event7040 records DISABLED→DEMAND. These are confirmed partial persistent effects, not rollback. Service is not observed RUNNING and no controlled Stop was requested by this qualifier path.

The native qualifier reports phase RUNNING_READINESS, error5023 (unexpected state instead of RUNNING), then deliberately parks with LIVE_DISPOSITION=UNKNOWN. The earlier service failure is Win32 ERROR_ACCESS_DENIED5. Do not confuse the qualifier's secondary5023 with the service's original5. At inspection the qualifier process28452 remains alive (98 handles observed), while no papacc_server_lab_host process was returned by the name query. Name absence/SCM PID0 is not a retained handle proof of termination of the original process instance; no such process handle was captured before this failure. Thus the qualifier's strict retirement guard is NOT overridden.

## Located failure boundary, cause not yet proved

The exact pinned host's private LAB ServiceMain registers its handler and START_PENDING before psr_prepare. On psr_prepare error it performs checked psr_close/ps_close; only if both succeed does it report STOPPED with native.error. Failure in those closes would park rather than report this STOPPED. Source and observed status therefore locate the rejected path in protected preparation before invocation of the shared application runtime. This is source-backed boundary analysis, not a positive runtime/token/cleanup trace or authorization to release the external qualifier.

The current code has several distinct ways to return5: failed native image/SCM reads, factual token/identity/group/impersonation rejection, native relative-open error collapsed to5, effective AccessCheck/descriptor rejection or later token/source checks. It lacks a first-failed-stage/API/raw-status receipt. Exact failing call/predicate is UNKNOWN. Do not assume UAC elevation, a DLL ACL regression, missing service groups or a particular Windows permission is the cause.

SCM service descriptor was read, not changed: SY/BA have their configured rights and SU service-logon group has query-config/query-status rights. This alone does not prove the actual service token or AccessCheck outcome. No running token/group/privilege/endpoint/module/process resource capture occurred: cycle_live was never reached. No listener/CA/CURRENT is authorized; the shared runtime was not admitted. Provider/kernel/stack and retained process costs remain UNKNOWN, ED11 PARTIAL, never zero.

## Required next discriminating work

Preserve the qualifier/window, its held file/SCM/admin references, intent, log, candidate file and DEMAND binding. Do not close/kill/retry or rerun any ACL/registration launcher. No automatic second Start, Stop, deletion, restore, ACL/UAC/account/policy change is authorized or performed here.

Prepare a focal private-LAB diagnostic successor through official review that records first preparation stage, API/predicate and uncollapsed native error/status without logging secrets or changing admission/identity rules. Audit the qualifier's post-failed-start factual retirement separately; SCM STOPPED alone does not fabricate an original process handle or authorize killing the retained qualifier. A further positive invocation needs a newly reviewed exact manual operation; never reuse the consumed attempt or overwrite its pinned live artifacts. No architectural/bootstrap reopening is required.

```text
CURRENT_TASK_COMPLETED=NO
CANDIDATE_PLACEMENT=CONFIRMED_EXCLUSIVE_NEW_FILE
LAB_SERVICE_REBIND=CONFIRMED_LAB_ONLY_DEMAND
LAB_SERVICE_START=REQUESTED_SERVICE_FAILED_PREPARATION_WIN32_5
LAB_SERVICE_RUNNING=NOT_REACHED
LAB_SERVICE_STOP=NOT_REQUESTED
LAB_SERVICE_STOPPED=OBSERVED_FAILED_START_PID0_WIN32_5
SERVICE_TOKEN=NOT_CAPTURED
SERVICE_SID=SCM_IDENTITY_PRESERVED_RUNNING_TOKEN_NOT_QUALIFIED
ACL_ISOLATION=ACTUAL_TOKEN_RESULT_UNKNOWN_NO_PERMISSION_CHANGES
RUNTIME_READINESS=DENY_PSR_PREPARE_ACCESS_DENIED
STOP_QUIESCENCE=CONTROLLED_CYCLE_NOT_QUALIFIED_EXTERNAL_REFS_RETAINED
QUALIFIER=RETAINED_NO_RETRY_NO_KILL
LISTENER=DISABLED_BY_PROFILE_NO_POSITIVE_ENDPOINT_CAPTURE
ED11_TOTAL_STATUS=PARTIAL
NETWORK_TESTS=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
FIRST_UNRESOLVED_DIVERGENCE=PSR_PREPARE_FIRST_ACCESS_DENIED_CALL_OR_PREDICATE_UNKNOWN
NEXT=FIRST_NATIVE_LAB_READINESS_ACCESS_DENIED_DIAGNOSIS
```


## Subsequent read-only diagnosis / owner update — 2026-10-23

Earlier retained/alive statements describe the observation at that time. Owner now reports the qualifier window is no longer open; historical PID28452 OpenProcess returns Win32 87. No final cleanup receipt exists: final reference disposition remains unconfirmed. Executor did not release original references, retry Start or mutate SCM/ACL. Do not replay the consumed launcher. Exact failure remains unknown; proposed instrumentation was not applied. See [focused diagnosis](../first-native-lab-readiness-access-denied-20261023/closeout.md).
