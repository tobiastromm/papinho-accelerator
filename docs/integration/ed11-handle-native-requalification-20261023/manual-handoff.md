# Manual ED11 handle requalification — prepared, not executed

Date: 2026-10-23. Task: OWNER DIRECTION — ED11 HANDLE LIMIT NATIVE REQUALIFICATION.
Branch main, HEAD 913c90cf2328f7939d8f5b927640601903ac6e91. This preparation does not authorize executing the capture. Separate exact owner approval remains required.

## Scope and identity

One external read-only capture ED11H-20261023-01, tied to existing operational identity75330d11cc7e4123ae62eadc9a59c2f5 WITHOUT consumption. Existing service PapinhoAcceleratorLab20261023, virtual account NT SERVICE\PapinhoAcceleratorLab20261023. New qualifier C:\Projetos\_tmp\PapinhoAccelerator\ed11-handle-native-requalification-20261023\Selected\Release\papacc_scm_instrumented_lab.exe, SHA256 42af729e154426dc7cbdd7e79f42e0c0d29490d0df2937cd3995ea4b0960763d. New launcher tools/qualification/scm-handle-requalification-uac.ps1, SHA256 858264a1e6cb7533af9415fbaa2b3edc03dc05c904ffec1a66543b0d5e88e3fc. Old launcher and pinned qualifier remain byte-identical and must not be used for this experiment.

Candidate source remains build/session0-diagnostic-runtime-integration-20261023/Selected/Release/papacc_server_lab_host.exe, SHA256 eda84c0cf91ad1266cee462e62c89b84ffa5873853a0122215d15d666701361b. Protected runtime parent/configuration/image and two dependency identities, hashes and descriptors remain under existing native validators, with no replacement or ACL remediation. Candidate is only read/pinned, never run or rebound.

Operator SID S-1-5-21-785699550-1440458426-1682014700-1108; AMD64 Windows10.0.19045. Existing boot pin remains System Kernel-General12 record236441 / StartTime2026-10-23T18:36:34.5000000Z, event Computer SHA256 2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff. These are EXPECTED pins, not a new factual observation. Native ig_boot must match current event/version/host before proceeding. Any mismatch => STOP, no pin edits or retry. Exact native LAB query-config/query-status requires STOPPED/PID0, existing account/SID/command/startup binding and protected file checks. No service adoption or mutation.

## Focal observation boundary

The reduced candidate previously returned immediately after census rejection, before ordinary metrics. One GetProcessHandleCount(GetCurrentProcess()) query now executes after successful close of the census snapshot and before returning170, at the post-census/pre-metrics/pre-PSS boundary. It opens no process handle, adds no recorder, thread, ETW session, WinVerifyTrust or PSS call. Captured PID plus existing qualifier creation identity/tick bind the observation. Sampling failure reports UNKNOWN with native metric error and cannot replace first170 or authorize admission. Snapshot close failure retains existing UNKNOWN behavior and skips this observation.

This is the same logical post-census/pre-own-PSS handle observation boundary after removal of supplemental work. It is not an identical workload to the historical141 sample, which included supplementary console/trust work. No numeric saving can be inferred until the new factual observation exists; even then a one-sample difference is not a universal peak.

A census rejection means HANDLE_GATE=NOT_REACHED regardless of observed count. The observed count can be compared to128 but is not a gate PASS. If normal preflight completes, existing required metrics supply role2 and native final0 proves gate PASS. Ambiguous failed metric evaluation is conservatively NOT_REACHED/not proved rather than inventing which predicate executed. Full raw native output is preserved. No global census or thresholds changed.

## Qualification and review

Four Selected/Legacy Debug/Release builds PASS, /W4 /WX, zero compiler warnings. Four17-case busy-model runs and four34-case policy-model runs PASS. Four cross-process fixture runs each22 cases PASS: exclusivity, Stop, duplicate/divergent identity, consume once, UNKNOWN retention, quiescence and release. Five pure receipt-classification cases PASS, including127/128/129 under census denial, metric failure UNKNOWN and full success. PowerShell5.1 parser PASS. Launcher Check PASS validates script/qualifier/PowerShell/candidate/policy hashes WITHOUT native entry, UAC, SCM query or preflight. New qualifier was never executed. Source preimages/build logs/test logs remain in external scratch C:\Projetos\_tmp\PapinhoAccelerator\ed11-handle-native-requalification-20261023; source/pins/test summaries accompany this handoff.

Launcher modes restricted to Check/Prepare/ElevatedCheck; only native argument --prepare is reachable. Interactive exact confirmation is required after elevation and pin verification. Administrative claim/log/final evidence uses CreateNew in this report directory; these are not operational intent or service diagnostic files. A preexisting claim denies another execution. No automatic retry or claim deletion. Native UNKNOWN is retained; no forced close/kill. Windows console handler and canonical Stop/retirement semantics remain unchanged.

## Owner procedure AFTER separate approval

1. Use the usual interactive PowerShell under the approved owner. Do not run the old launcher or another capture/preflight. Keep the windows available for evidence.
2. Execute this exact command once and approve its UAC prompt:

```powershell
& "C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -ExecutionPolicy Bypass -File "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-handle-requalification-uac.ps1" -Mode Prepare -ExpectedScriptSha256 858264a1e6cb7533af9415fbaa2b3edc03dc05c904ffec1a66543b0d5e88e3fc
```

3. In the elevated window verify displayed ED11H-20261023-01 and qualifier hash, then type exactly:

```text
REQUALIFY_ED11_HANDLES_READ_ONLY ED11H-20261023-01
```

4. Wait for native final output and the JSON receipt. Rejection170/exit2 is possible and does not imply capture failure if the handle observation was recorded. Share the output and ED11H-20261023-01-final.json. If IG_RETAINED/UNKNOWN appears, preserve the window/references and stop; no retry. F10 is only offered after factual native return and closure; no automatic cleanup of unknown resources.

Expected effects: read/pin files, read native host/boot/operator/process identities, acquire/release existing canonical exclusivity for query, read SCM configuration/status, temporary existing configuration byte lock, process census, one self handle observation on rejection or existing metrics on success; create administrative evidence only. No StartService/StopService/rebind, ACL change, operational file, intent consumption, trace, CA, network or CURRENT. File locks/mutex are transient query guards, not SCM configuration changes.

## Prepared result

HANDLE_COUNT_OBSERVED=UNKNOWN_UNTIL_MANUAL_CAPTURE
HANDLE_LIMIT=128
HANDLE_GATE=NOT_REACHED
FIRST_FAILED_STAGE=UNKNOWN_UNTIL_MANUAL_CAPTURE
FIRST_FAILED_PREDICATE=UNKNOWN_UNTIL_MANUAL_CAPTURE
CONSOLE_CENSUS=UNCHANGED_RESULT_PENDING
CANONICAL_EXCLUSIVITY=UNCHANGED_OFFLINE_QUALIFIED_NATIVE_PENDING
RESOURCE_RETENTION=UNCHANGED_NATIVE_PENDING
EXACT_OPERATION_APPROVAL=PENDING
PREFLIGHT_EXECUTED=NO
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
TRACE_RECAPTURE=NO
HOST_CREDITS=NONE_GRANTED
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES_EXCEPT_AUTHORIZED_SCOPE
NEXT=MANUAL_ED11_HANDLE_REQUALIFICATION
