# Windows virtual service native LAB registration — implementation and manual handoff

TASK=PACC-PHASE6-WINDOWS-VIRTUAL-SERVICE-NATIVE-LAB-REGISTRATION-1, 2026-10-23. Branch main, HEAD913c90cf2328f7939d8f5b927640601903ac6e91. Latest manual DATA receipt reviewed: read_only_data_pass=true, native_bound_closed=true, native_exit0, operator elevated/interactive ...1108; previous name resolution remains pending, no prior registration authority inferred. Accepted V0–V8/Model E/ED08/ED11 and scoped owner instruction restrict this implementation. [Exact operation scope](exact-operation-authorization.md) officially reviewed BEFORE dependent code. No accepted production ADR changed.

## Identity/profile

Fixed exclusive LAB name PapinhoAcceleratorLab20261023, account NT SERVICE\PapinhoAcceleratorLab20261023, passwordNULL. Not the productive name/account. Compile-time private identity header supplies LAB constants and SERVICE_DISABLED ONLY to qualification targets. Default canonical product remains PapinhoAccelerator/NT SERVICE\PapinhoAccelerator, DEMAND_START, with ps_binding_ready() still0. No runtime/argv/ENV name override, public credential API or product mutation grant.

The qualification target includes the SAME server_service_win32.c/PS_NATIVE owner, corrected ps_install_virtual and ps_virtual_readback, not a copied second SCM manager. Tests reuse same model under product and LAB constants. CMake BUILD_TESTING adds private registration executable and MODEL test only; actual effect executable is never a CTest test or installed product command. No new root, worker, journal, registry or pin system.

The original protected LAB image remains exact SHA2563737e83be95613a2616b9fb764fe95d72796912746e9440e37b3ed2a77e01abb, native417614100:262144:1119989, fixed command quoted image + service host. Configuration remains exact SHA256a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44, native417614100:393216:1120240; parent417614100:1966080:79426. All owner/ACLs preserved, no executable replacement. The old image is NOT a qualified LAB-name runtime host and does NOT contain the latest post-create SID correction. The registration executor DOES contain that corrected adapter and tests it directly; it never runs that image. SERVICE_DISABLED contains accidental start; future lifecycle requires separately approved compatible LAB host/binding and start-mode change. No runtime-readiness claim.

## Native effect and lifetime

Executor tools/qualification/scm-registration-lab.c accepts only --check or --register-lab <32 lowercase hex attempt>. --check binds exact files and observes absence, explicitly claims no operator authorization and calls no create. Positive route additionally validates actual process TokenUser against fixed approved operator SID, elevation/nonlimited token, enabled Administrators, no thread impersonation; retains primary token and compares TokenId/AuthenticationId/ModifiedId immediately before effect. Console is opened via CONIN$/CONOUT$ for real mode/visible line input even while outer PowerShell captures stdout.

Protected parent/config/image handles stay open, shareREAD and no write/delete sharing; native IDs/no-reparse, immutable content hashes using existing BearSSL storage capsule and actual unchanged scm_descriptor/PROTECTED policy required. Initial SD digests are captured and must match on revalidation. No DATA -> authority promotion; LAB authority is the owner's exact new instruction plus separate same-invocation confirmation, not the historical read-only receipt.

One connect-only SCM owner first observes LAB absence; existing object is STOP, not adopted or changed. Positive path displays exact LAB name/account, DISABLED/no-start scope, image/config/hashes, operator and requested actions directly to the real console. Only typing the entire REGISTER_SCM_LAB <attempt> line permits continuation. Input is visible; no password. Native one-shot manual-native-intent.txt is CREATE_NEW/flushed/closed AFTER confirmation and BEFORE CreateService; preexisting intent blocks retry. Input cancellation/Stop denies. No prior F10/read-only confirmation is reused.

After revalidation, SAME owner opens SCM connect+create permission. Existing ps_install_virtual performs one CreateService with matching LAB virtual account, OWN_PROCESS, DISABLED, NORMAL, NULL password/no dependencies; configures UNRESTRICTED; bounded QueryServiceConfig/2 readback checks exact account/command/type/start/dependencies/SID type; LookupAccountNameW validates actual SID length/namespace/shape. Qualification additionally checks NORMAL error control and exact status buffer size, SERVICE_STOPPED and PID0. GetStringSid output is actual resolved SID, not sc showsid derivation or live TokenUser. File bindings revalidated after operation. A configured SID type and resolved SID remain distinct from a running service token.

No StartService, DeleteService or runtime is called by the qualification entrypoint. Reused generic adapter source contains those functions for other product lifecycle commands, but they are unreachable from this fixed main route; imports alone are not presented as an executed call graph. Models explicitly intercept Start/Delete and require both counters0. Product gates remain untouched.

Create/configure/readback is NOT atomic. Disposition tracks requested/no-effect-collision/uncertain/created/configured. Error after create preserves the service, no delete/overwrite/adopt/retry. Native intent retains exact name/attempt even if outer receipt fails. All synchronous calls return before reference retirement; no async worker/root/consumer created. ps_close reuses existing ordered checked closure/retention. Qualification additionally keeps failed temporary closes in two bounded auxiliary slots and an uncertain SD pointer, never retries cleanup. Any close/SD/string uncertainty makes final result2, never CLOSED/PASS. OS process termination is not claimed as successful API cleanup; the durable SCM object remains retained independent of caller process lifetime.

## Manual UAC launcher

File tools/qualification/scm-lab-registration-uac.ps1, SHA256 8a19fadc007e185b4f34944496fee3f97e582070ae9e4b59ba9cb6009fbd3512.
Qualified executable build/windows-virtual-service-native-lab-registration-20261023/Selected/Release/papacc_scm_registration_lab.exe, SHA256 cb1e3eb35901652e51400e797f7f137b168660e9ddb9253176e92726756b83d7.

Fixed paths/controlled arguments, read-held SHA pins for self/qualifier/original image/conf/prior DATA receipt, reparse-path denial; exact eligible elevated operator and real console, no impersonation. Start-Process -Verb RunAs only after MANUAL owner Launch. No passwords, persistent PowerShell/UAC policy edits, scheduled tasks/services or fallback elevation. Source-control/host trust remains separate from operational installation authority; this is an isolated qualification tool, not a qualified productive installer.

Owner command, not executed by agent:

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-lab-registration-uac.ps1" -Mode Launch
```

Native console prints full exact plan and requires REGISTER_SCM_LAB <displayed attempt>. Enter alone cancels. F10 closes the preserved result window only. Do not start the service, change DISABLED, delete it, or retry after error. Do not run any ACL remediation launcher.

Outer wrapper captures native output/exit and writes exclusive manual-<attempt>.json after return. PASS requires BOTH exact registered/configured SID/stopped/PID0 line and final ERROR0/CLOSE0/RETAINED0/AUX_RETAINED0/EFFECT_REQUESTED1/DISPOSITION4/READBACK1 at FILES_REVALIDATED, plus native exit0. No success solely from an exit code. Partial/error output and exact native disposition preserved. If native/outer process is interrupted or receipt creation fails, retain intent, STOP, no rollback or repeat assumption. Expected positive leaves a persistent DISABLED LAB service; no deletion is authorized in this task.

## Build and evidence

Fresh Selected Debug/Release builds using documented same MSVC19.51 x64 Ninja /W4 /WX and explicit Windows3MiB build profile. First build stopped on deprecated sprintf/_snwprintf warnings; replaced with fixed hex encoding/bounded copies, no warning suppression/toolchain change. Final build-final/build-verified/build-complete logs warning-free; no productive binary touched. Default and LAB model configurations pass.

negative-and-component-tests.json: 22 passing suite/configuration combinations across Debug/Release (11 suites each), including LAB profile plus existing product lifecycle/native/query/virtual/ACL/console/personal DATA/native readonly/memory census/issuer-denial suites. Existing service, account/command/SID readback divergence, change/query failure after create, Stop, cleanup uncertainty/no-retry and valid SID capacity tests are modeled, not real service effects. Source model asserts matching virtual account/NULL password and profile startup, zero Start/Delete calls.

Six native check/argument-negative cases pass: both configurations --check report bound files/absence, no operator authority/no create/no effect; malformed attempt and unknown action deny. Latest final-native-check.log confirms new final artifact; final launcher Check2 shows executor lacks elevated/console eligibility, no native effect. Wrong Elevated script hash2 denies before effect. No automatic Launch or valid positive --register-lab call executed. No real service exists at preparation time, no native intent or manual positive receipt created.

Read-only EventLog API control tests ONLY QueryServiceConfig/2 return semantics (8192-byte buffer ->364 needed, SID info4 ->4), checked closes, no mutation. This discriminates real API length contracts before positive LAB test and is not adoption/runtime execution of that service. Existing product-name trust error is preserved; new LAB name's post-registration mapping will be tested only by owner-approved manual operation.

## ED11 reconciliation

census.log measured PS_NATIVE12816, host96, cli atomic4, qualification extra mutable globals212, TOKEN_STATISTICS56, hash context128. Named mutable subtotal13128 is NOT total process memory. Existing native query8192/SID68/domain128WCHAR buffers included in PS_NATIVE; no double counting. Global extra212 includes three file/token handles, SID/SD refs, two auxiliary retained slots+count, token snapshot and three SD digests. Constants and code measured separately in ed11-census.json PE sections, not added again as heap.

Native controlled handle peak7: parent/config/image/token4 + connect-only SCM1 + two real console handles during confirmation. After create6 (four binding/token + manager/service). Temporary token/intent belong to separate stages; unexpected impersonation denies before further work. Up to two failed auxiliary closes retained, no retry. One GetSecurityInfo allocation at a time; return length checked<=65536 but provider allocation peak/overhead remains UNKNOWN. SID conversion string separate and checked LocalFree. Persistent SCM object/database/RPC/kernel/LSA costs UNKNOWN, not zero after caller handles close.

Confirmation arrays alone4610bytes, hashing context/arrays1249bytes from measured128-byte context, excluding compiler padding/nested OS frames; these are source-level amounts, NOT a proven stack peak. Full stack, CRT/loader/control callbacks, LocalAlloc backing, LSA/provider/kernel, durable SCM storage, host capacity and PowerShell/UAC/CLR/pipeline costs remain UNKNOWN. Outer UAC parent/child each holds five pinned streams; capture/process/pipe resources not converted to zero. No pool/quota transfer, no operational admission or ED11 PASS. Legacy2097152/explicit Selected3145728 unchanged; only isolated behavior qualification authorized.

## Result

CURRENT_TASK_COMPLETED=YES
SCM_LAB_REGISTRATION=PASS_MANUAL_ELEVATED_LAB_ONLY
SERVICE_CREATED=YES_LAB_DISABLED_STOPPED
SERVICE_SID_CONFIGURATION=PASS_NATIVE_UNRESTRICTED
POST_REGISTRATION_SID_RESOLUTION=PASS_NATIVE_32_BYTES
SERVICE_CONFIG_READBACK=PASS_NATIVE_AND_INDEPENDENT_READ_ONLY
SERVICE_RUNTIME=NOT_STARTED
ED11_TOTAL_STATUS=PARTIAL
BUILDS=SELECTED_DEBUG_RELEASE_PASS
WARNINGS=0_FINAL_BUILDS_INITIAL_FAILURE_PRESERVED
NEGATIVE_TESTS=22_MODEL_REGRESSION_COMBINATIONS_AND_6_NATIVE_NEGATIVE_READONLY_CASES_PASS
NETWORK_TESTS=NOT_RUN
PRODUCT_SERVICE_INSTALL=NOT_RUN
BOOTSTRAP_EXECUTION=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=EXACT_APPROVED_SOURCE_CHAIN_AND_ORIGINAL_PROTECTED_PINS_VERIFIED
FIRST_UNRESOLVED_DIVERGENCE=NONE_IN_REGISTRATION_SCOPE; FUTURE_HOST_AND_TOKEN_NOT_QUALIFIED
NEXT=WINDOWS_VIRTUAL_SERVICE_NATIVE_LAB_LIFECYCLE_AFTER_HOST_AND_ADMISSION_GATES

Source/build/test evidence is factual; documentation is not an administrative/runtime authority grant. Official source/launcher exact preimages and postimages preserved. No account/ACL/CA/key/certificate/root/CURRENT mutation, productive service install, clone/worktree/commit/push/tag/release. No four historical LAB cleanup.

## Manual completion addendum

Earlier preparation statements are historical. Owner completed the manual registration successfully; see [verified manual result](manual-elevated-result-20261023.md). Independent read-only queries confirm DISABLED/STOPPED/PID0. No runtime or token qualification; no start/delete/retry authorized by this result.
