# Coordinated manual WPR capture03

Owner requested preparation of a new guided manual execution after closing windows. This external coordinator removes manual ordering/copy ambiguity. It does NOT add production instrumentation or change the canonical launcher, census, admission owner or B4. External diagnostic topology includes one elevated supervisor PowerShell, an asynchronous original launcher PowerShell, its elevated child, native qualifier, console infrastructure and transient WPR processes. These observable costs are not zero and are NOT admitted under four-role operational policy; no new slot/credit/threshold is granted. Capture is diagnostic only and may retain census170.

File: tools/qualification/wpr-handle-prepare-manual.ps1
SHA25666c4f6547fe883dff1433c0075b7b8380daced54b664c141f2f41e23392aa158.
Identity WPRH-20261023-03; new destination C:\Projetos\_tmp\PapinhoAccelerator\wpr-handle-attribution-20261023-03, must be absent. Previous directory02/result are preserved, not reused or deleted. No script execution by agent. Native WPR success/error paths remain to be qualified by the manual run; syntax validation does not prove native behavior.

Safety review: supervisor checks self/tool/launcher/qualifier/candidate/policy hashes, elevated exact operator, no impersonation, host/boot, signature, no reparse path, absent output root, profile, existing WPR status and collector visibility. It shows available disk and collectors for owner review before confirmation. No automatic collector takeover. ETL file growth remains unbounded natively and explicitly accepted under the original diagnostic scope; monotonic30s collection window is operational only. Synchronous native calls/stop can exceed it. A failed start records possible partial acquisition, runs no Prepare, performs no speculative stop/cancel. Once start succeeds, finally requests stop once for its own instance; status/hash follow only factual successful stop. No retry/cancel/kill.

Prepare starts only after own WPR reports recording. Original launcher pins and mode Prepare remain unchanged. Child process reference is held until factual process termination, potentially while waiting for F10; supervisor never kills/closes running qualifier. A retained qualifier means leave both windows open and report it. If final WPR status is uncertain, preserve first failure and UNKNOWN rather than claiming cleanup. First diagnostic code remains original in original receipts; wrapper first error is separate. A child exit2/census170 does not make trace itself invalid or imply service effects.

## Owner instructions

Open ONE Windows PowerShell as administrator. Close no retained process merely for this procedure. Do not start WPR or Prepare separately. Run this single line:

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "C:\Projetos\PapinhoAccelerator\tools\qualification\wpr-handle-prepare-manual.ps1" -ExpectedScriptSha256 66c4f6547fe883dff1433c0075b7b8380daced54b664c141f2f41e23392aa158
```

At prompt, review free disk and collector conflict conditions, then type CAPTURE_WPRH_03 only if acceptable. The wrapper displays WPR_RECORDING_CONFIRMED before opening Prepare. Accept UAC if requested. Type no other command. Supervisor shows progress and requests trace stop by30s without waiting for F10. After original qualifier returns factually and displays result-preserved/F10, F10 can close that result window. If RETAINED/UNKNOWN or no factual return, do not close it or supervisor; report output.

At end, send FINAL/ETL lines and receipt path from qualifier. External outputs: coordinator.log and handles.etl under directory03; native logs/receipts remain in original governed receipt directory. No operational diagnostic file, rebind or Start/Stop service is permitted.

Checks: PowerShell parser and native Windows PowerShell5.1 parser PASS; static review of ordering/finally/one-start-one-stop/no-positive-mode. No native positive preflight/trace executed, no new builds needed. Launcher, qualifier code and accepted policy pins unchanged. No claim that147/141 handles will recur. ETL loss/baseline/rundown/stacks/heredity still require WPA verification. No WPR/WPA install, quota change, SCM/ACL change, production/PST/Browser/wire change, bootstrap/CA/certificate/CURRENT or git publication.

COORDINATOR_PREPARED=YES
COORDINATOR_NATIVE_QUALIFICATION=PENDING_MANUAL_RUN
TRACE_EXECUTED_BY_AGENT=NO
START_SERVICE=NOT_RUN
ED11=PARTIAL
CURRENT_SIGNATURE_RECOVERY=DENY

## Host verifier correction after FIRST_ERROR=HOST_STOP

Owner reported FIRST_ERROR=HOST_STOP under coordinator preimage66c4f6547fe883dff1433c0075b7b8380daced54b664c141f2f41e23392aa158. It failed before evidence-directory creation, WPR acquisition and Prepare. Directory03 remains absent in read-only query. Failure remains historical, not rewritten to PASS.

Source defect demonstrated: coordinator hashed Environment.MachineName while accepted exact-case Computer hash is from System/Kernel-General12 event XML. Canonical ig_boot uses that event field, not MachineName. Comparing different sources can reject same host. No inference that host actually changed and no widening/fallback/normalization is allowed. Correction reads exact Computer text UTF8 from the SAME event subsequently checked for record236441 and accepted StartTime, preserving expected2b9bba4... and boot gates. Missing Computer denies. Namespace-aware extraction supports event namespace without textual heuristic. No MachineName fallback, altered expected hash, changed boot, threshold or B4.

Corrected script SHA256ca6612762ed22c5d1a05b6aaa0adb1135441fc84f02ed8c759dd82bb1c898884. Earlier command/hash is superseded for future use; preserved above as history. Windows PowerShell5.1 parse and isolated function tests (namespaced event, plain XML, exact-case distinction, missing field denial) PASS. Test extracts ONLY function AST and does not execute main/coordinator/WPR/Prepare. Native host/boot query still must succeed in owner's elevated console; correction is not factual current-host PASS. If new HOST/BOOT error appears, STOP and preserve; no new identity granted.

Use same single-command entry with new ExpectedScriptSha256ca6612762ed22c5d1a05b6aaa0adb1135441fc84f02ed8c759dd82bb1c898884. All earlier acquisition/retention guards remain. No directory03 or trace created by agent. Original launcher/policy untouched. ED11 PARTIAL, authority DENY.
