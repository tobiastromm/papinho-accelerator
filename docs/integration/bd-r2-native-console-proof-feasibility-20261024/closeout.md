# B+D R2 native console proof feasibility

TASK=PACC-PHASE6-BD-R2-NATIVE-CONSOLE-PROOF-FEASIBILITY-1
Review date: 2026-10-24. Branch main; HEAD 913c90cf2328f7939d8f5b927640601903ac6e91.

## Decision and scope

R2_NATIVE_PROOF_FEASIBILITY=NOT_DEMONSTRATED.
The supported interfaces reviewed demonstrate several necessary component facts, but no qualified source currently bridges the authorized actor's console to the retained original host process AND its native mapped-image provenance over the required admission interval. This is a bounded feasibility conclusion, not proof that Windows can never expose such evidence. No implementation or normative change follows from the review. CONSOLE_ADMISSION=DENY_UNTIL_PROVEN.

Authorities read: [accepted B+D R2](../../adr/DECISION-canonical-lab-console-topology-bd-20261023.md), [core closeout](../canonical-lab-bd-topology-implementation-20261024/closeout.md) and next-slice, [accepted B0-B6](../../adr/DECISION-ed11-single-lab-diagnostic-native-boundary-20261023.md), specification 1.0 sections 11.1-11.4 (Model E/ED08/ED11 composition), historical relationship-report, prior console observations, and relevant personal-listener future direction. Continuous-session governance review remains applicable. No accepted ADR changed. The future listener direction is unrelated to console proof and remains unchanged.

## Exact blockers

R2 requires factual console relationship, native image identity/provenance, original handle/creation, session/token, lifetime and metrics with revalidation. R3 forbids replacing these missing mandatory facts by risk acceptance or diagnostic correlation. R6 additionally requires a usable console for confirmation and Stop in this topology. The first missing property is an independently qualified association from the actor's console endpoint to the held original infrastructure process. The second unresolved property is binding provenance of the verified image object to that process's actual mapped image; opening today's pathname is insufficient.

Current core ps_bd_console in apps/server/server_service_admission_bd_win32.h validates component fields but returns ERROR_NOT_SUPPORTED for native proof sources. Its synthetic positive source is restricted to PAPACC_SERVICE_BD_MODEL test compilation. Filling evidence bits or accepting a callback's unsupported assertion cannot qualify a source. No adapter, source enum, launcher, pins, census or authorization code was changed here.

## Evidence matrix: supported API guarantees versus R2

All observations below describe contracts/candidate methods, not new native measurements. Historical boot236441 is not freshly attested or adopted as the current boot by this research.

| R2 requirement / interface | Property actually supplied | Limits / substitution / revalidation | ED11 responsibility |
|---|---|---|---|
| Original process: OpenProcess + GetProcessId + GetProcessTimes | Native process object reference, PID and creation time | Open by PID can race; validate creation against the intended instance after opening, hold original handle and compare at use. No console relationship. Query-limited rights plus SYNCHRONIZE when waiting; no ALL_ACCESS or debug privilege workaround | Retained process handle; original reference remains charged until confirmed close; kernel cost UNKNOWN |
| Live lifetime: wait on original process handle | Whether that process object has terminated at observation | Keeping a handle does not keep a process alive; liveness sample does not guarantee future execution or quiescence. Recheck around admission; loss invalidates | Existing wait/Stop ownership; no timeout-based release |
| Token: OpenProcessToken/GetTokenInformation; relevant thread token checks | User, token IDs/type/elevation/session and available groups/privileges | Token context is not console ownership. Process primary token alone does not prove no thread impersonation. Preserve canonical actor/control-thread continuity checks and deny unavailable required facts | Token handles, bounded token buffers and retained context; transitive cost UNKNOWN |
| Session: ProcessIdToSessionId / token session | Session membership | PID must be tied back to held creation identity; same session not ownership. Revalidate required context | Bounded query storage, no new authority |
| Console clients: GetConsoleProcessList | Processes attached to calling process's current console | Not a documented host-process identity return. Insufficient buffer produces no usable full list; fixed-bound failure denies. Repeated client list cannot manufacture host link | Fixed PID buffer; no extra collector/thread needed |
| Console window: GetConsoleWindow + GetWindowThreadProcessId | Console-associated HWND and process which created that window | Creator is not contractually guaranteed to be the console API-server process in every supported topology. Pseudoconsole HWND is for message queue. Historical creator was the elevated PS, not rejected conhost. HWND/PID reuse and interval change require checks; no automatic AttachConsole repair | Bounded calls/buffers; no retained HWND identity equivalent to process handle |
| Named pipe: GetNamedPipeClientProcessId / server-peer counterpart | PID of peer of the specified qualifying named-pipe instance | Client API requires a CreateNamedPipe-created handle. No demonstrated such endpoint binds existing console connection to conhost. ConDrv reference/console handle is not thereby a documented named-pipe instance; a newly created pipe would prove that pipe peer only | New pipe/collector not justified; no implementation or resource exemption |
| Image name: QueryFullProcessImageName / GetProcessImageFileName | Name/path of executable image, Win32/native-device forms | Neither returns a native mapped image file handle or verified publisher. Same path before/after is not object equality | Fixed UTF-16 buffer; process handle already counted |
| Opened image file: GetFileInformationByHandleEx(FileIdInfo), hash, WinVerifyTrust | Identity/bytes/trust result for the actual opened file or supplied trust object | No automatic equality with process mapped section. Must establish a qualified binding and replacement protection; successful trust is not console ownership. Old offline trust failure remains UNKNOWN, not unsigned/malicious proof | File/trust resources, explicit close; do not reintroduce rejected supplemental trust/PSS path |
| Host/boot/plan/confirmation | Existing canonical context/identity binding can include exact approved host, boot and held objects | Does not prove the missing association. Old receipt cannot approve a new instance; freshness/token/config/image/policy revalidation remain mandatory | Existing bounded descriptor/context; no new owner |
| Observable resources | Existing process metrics can attribute Private/commit/WS, handles, threads and observable stack to held process | Samples not universal peaks; kernel/cache/transitive stack remain UNKNOWN. Historical106 is qualifier sample, HANDLE_GATE NOT_REACHED, not full admission | Console separately measured, shared admin budget256MiB/1024handles/64threads/32MiB stack; native each64MiB/128/16/8MiB; four actual processes; credits NONE_GRANTED |

## Implementation-source and ETW findings

Microsoft publishes IOCTL_CONDRV_GET_SERVER_PID in terminal/dep/Console/condrv.h. This is a concrete internal mechanism worth distinguishing from name/parent heuristics. Its presence in open-source main is NOT documentation of a supported third-party Windows10.0.19045 API/ABI, acceptable handle contract, version compatibility, security semantics, error behavior or revalidation guarantee. No IOCTL was invoked or copied into the qualifier. Public DeviceIoControl alone does not make every private IOCTL a supported console interface. Qualifying this path would require explicit support/contract evidence and separate source/security/resource review; it is not an implementation recommendation under the present supported-interface requirement.

Microsoft terminal source also distinguishes ConDrv server/reference objects from ordinary redirected streams. That implementation evidence reinforces why named-pipe PID APIs cannot simply be applied to CONIN$/console references. Current main source is not proof of exact installed Windows10 binary behavior.

Documented ETW Process_V2 start/end/rundown supplies process identity/context fields; its schema does not establish console attachment-to-host relation. Parent/process/session/command line are not that relation. Terminal tracing source and the official Terminal WPR troubleshooting procedure establish available diagnostic tracing, not a versioned security/admission attestation for the installed inbox console host. No reviewed supported Console-provider event contract demonstrated the full R2 tuple and coverage of all relevant attach/detach/host changes on this target. This is a precise qualification gap, not a claim that no private/version-specific event exists. Any future ETW proposal must name provider GUID/version/event/fields and supported meaning, demonstrate source coverage/loss handling/contemporary original-handle correlation and revalidation, and charge collector/storage/lifetime. An event header's executor PID is not automatically a console owner. No new trace, parser, provider enumeration or recorder was run; old WPR Handle trace is not promoted into missing console-association proof. Terminal's documented tracing can include console input/output; broad tracing is not a minimal authorization mechanism.

CreatePseudoConsole is supported since Windows10 1809 and returns HPCON, not a documented process identity/provenance attestation for an existing host. Owning a newly created pseudoconsole would change topology, I/O/Stop duties and resources; no new console/pipe/worker is authorized or justified by this review.

## Alternative decision

| Alternative | Finding | Decision |
|---|---|---|
| A complete supported native API proof | Process identity/lifetime/context can be queried; full console-host and mapped-image binding not demonstrated | NOT_DEMONSTRATED; no source activation |
| B qualified composite retained proof | Retention prevents accepting a reused process object, but cannot create missing edge between console and host. Window/client/parent/session/current-file composition remains insufficient here | NOT_DEMONSTRATED; no heuristic authentication |
| C remove host dependency | Detaching only qualifier leaves interactive PowerShell host. FreeConsole detaches caller and does not necessarily destroy shared console. True console-independent topology changes explicit confirmation and canonical Stop requirements | Candidate alternative only; requires focal R1/R5/R6 decision and separately qualified binding/Stop, no silent change |
| D focal normative correction | Define an observable, testable evidence contract and threat boundary before functional work, or explicitly choose C. Cannot restore revision1's rejected UNKNOWN-as-eligibility waiver | Recommended minimum next decision; keep current R2/R3 DENY until approved |

Minimum resolution: one focused normative review of R2 evidence requirements together with R6 dependency. Owner should choose whether to retain strict R2 pending a supported association/mapped-image evidence contract, or commission a console-independent administrative/Stop topology. Do not add another recorder or repeat preflight to rediscover the same missing property. This report does not draft/accept a weaker contract, approve internal APIs, alter four slots/128 handles, grant credits or authorize operation. It closes feasibility investigation at NOT_DEMONSTRATED with the missing properties explicit.

## Primary sources reviewed

Links are official API contracts unless explicitly labeled implementation source. Retrieved during this review; repository main references are discovery evidence, not immutable target qualification.

- [GetConsoleProcessList](https://learn.microsoft.com/en-us/windows/console/getconsoleprocesslist)
- [GetConsoleWindow](https://learn.microsoft.com/en-us/windows/console/getconsolewindow)
- [GetWindowThreadProcessId](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowthreadprocessid)
- [GetNamedPipeClientProcessId](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getnamedpipeclientprocessid)
- [OpenProcess](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocess)
- [GetProcessTimes](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getprocesstimes)
- [QueryFullProcessImageName](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-queryfullprocessimagenamea)
- [GetProcessImageFileName](https://learn.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-getprocessimagefilenamew)
- [GetTokenInformation](https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-gettokeninformation)
- [ProcessIdToSessionId](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-processidtosessionid)
- [GetFileInformationByHandleEx](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getfileinformationbyhandleex)
- [Process ETW schema](https://learn.microsoft.com/en-us/windows/win32/etw/process-v2-typegroup1)
- [ETW overview](https://learn.microsoft.com/en-us/windows/win32/etw/about-event-tracing)
- [CreatePseudoConsole](https://learn.microsoft.com/en-us/windows/console/createpseudoconsole)
- [FreeConsole](https://learn.microsoft.com/en-us/windows/console/freeconsole)
- [Console definitions](https://learn.microsoft.com/en-us/windows/console/definitions)
- [Microsoft implementation: ConDrv declarations](https://github.com/microsoft/terminal/blob/main/dep/Console/condrv.h)
- [Microsoft implementation: reference/server distinction](https://github.com/microsoft/terminal/blob/main/src/winconpty/winconpty.h)
- [Microsoft implementation: host tracing](https://github.com/microsoft/terminal/blob/main/src/host/tracing.cpp)
- [Microsoft Terminal diagnostic trace procedure](https://github.com/microsoft/terminal/wiki/Troubleshooting-Tips)

## Qualification and preservation

Research/documentation only. No new builds/tests are necessary for unchanged source; prior four zero-warning builds and offline/model qualification remain historical, not native R2 proof. Current WIP/source/binaries/accepted policies checked against pre-write hash inventory (verification.json). Only this report/verification and appended live-capability entry are intended additions/changes. External scratch holds before.json and capability preimage. No operational native query/measurement, preflight, service start/stop, SCM/ACL change, trace, reboot, bootstrap, keys/certs, publication, clone/worktree or git publication. PST/Browser/PACC wire and production0.6.4 unchanged.

CURRENT_TASK_COMPLETED=YES
R2_NATIVE_PROOF_FEASIBILITY=NOT_DEMONSTRATED
SUPPORTED_APIS=COMPONENT_FACTS_ONLY_FULL_R2_NOT_QUALIFIED
MISSING_PROPERTY=CONSOLE_ENDPOINT_TO_ORIGINAL_HOST_AND_MAPPED_IMAGE_PROVENANCE_BINDING
MINIMUM_RESOLUTION=FOCAL_R2_R6_NORMATIVE_REVIEW_NO_AUTOMATIC_CHANGE
CONSOLE_ADMISSION=DENY_UNTIL_PROVEN
PREFLIGHT_EXECUTED=NO
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=R2_MANDATORY_RELATIONSHIP_SOURCE_NOT_QUALIFIED
NEXT=BD_R2_FACTUAL_PROOF_OR_NORMATIVE_CORRECTION
