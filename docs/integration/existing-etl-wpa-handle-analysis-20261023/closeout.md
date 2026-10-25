# Existing ETL offline WPA handle analysis — bounded review

TASK=PACC-PHASE6-EXISTING-ETL-WPA-HANDLE-ANALYSIS-1
2026-10-23. main / HEAD913c90cf2328f7939d8f5b927640601903ac6e91. No new capture, preflight, native runtime or parser implementation.

## Evidence identity and path discrepancy

Owner requested C:\Projetos_tmp\PapinhoAccelerator\wpr-handle-attribution-20261023-03\handles.etl. That literal path does not exist. The original WPRH03 receipt and all previous observations identify C:\Projetos\_tmp\PapinhoAccelerator\wpr-handle-attribution-20261023-03\handles.etl. This original file exists and independently matches the owner's exact requested SHA256c5e4bb7cdce671cb5f8bc2e42f61313a78553da815c68a5f9234013b27bb2679,66060288bytes. It is the same historical artifact, not a new or substituted capture. No moving/copying/renaming or rewriting was performed. Hash rechecked before/after review.

Existing decoded outputs under external analysis-tracerpt-01 belong to THIS ETL only. Original ETL is preserved byte for byte. No other ETL used. See [manual capture result](../wpr-handle-attribution-20261023/manual-result-03.md).

## WPA search and scope

- PATH: no wpa.exe/wpaexporter.exe/xperf.exe located.
- Recursive filename search in installed Windows Kits10/8.1, Program Files, Program Files(x86), user Local Programs and Downloads: no matching executable found in readable locations. C:\Tools/C:\Sysinternals not present in searched roots.
- No WPT/WPA/ADK matching uninstall entry or WPA App Paths entry found in queried HKLM/HKCU inventory.
- Current user Get-AppxPackage *WindowsPerformance*:0 packages. WindowsApps alias query found no WPA match.
- Some locations are unreadable, notably WindowsApps. Get-AppxPackage -AllUsers returned access denied even under the approved read-only execution context. This is an OS access failure, not an automatic approval rejection. No policy/ACL changed to enumerate it.

Conclusion: NO USABLE WPA LOCATED within observed scope; absolute absence across all protected/other-user paths is NOT established. No candidate executable means version/signature/provenance of an installed WPA cannot be certified. No WPA launched or installed. The literal task's WPA analysis remains pending. Existing tracerpt output is supplementary evidence, not relabeled WPA analysis.

## Factual identity and timeline

| Event | Identity/time | Meaning |
|---|---|---|
| Boot | event236441 / StartTime2026-10-23T18:36:34.5000000Z / accepted Computer SHA | Historical capture epoch, not current admission |
| External supervisor | PID820 creation134372785718124734 | Diagnostic observer, not free baseline |
| Launcher | PID8420 creation134372785828487950 | Original launcher actor |
| Elevated launcher | PID12848 creation134372785848378082 | Separate admitted actor in preflight model |
| Qualifier creation receipt | PID444 /134372785867046628 | Exact original instance |
| ETW Process Start |134372785867046748, payload PID444,parent12848, pinned image/--prepare |12microseconds after receipt creation; header creator PID is not payload target PID |
| Handle metric |141; precise per-sample timestamp not logged | Required simultaneous count cannot be tied to an exact event boundary |
| Process End |134372785868052935,exit2 | About100.6ms after creation; does not replace canonical cleanup receipt |
| WPR stop request/return |30192ms /32326ms | Selected30s collection window overshoot preserved |

Qualifying receipt manual-prepare-134372785848378082-final.json reports first170/PREFLIGHT_CENSUS, rejected conhost22520,parent8420; starts0,stops0,intent_consumed0,effect0,diagnostic_created0,close0. Reported owned capture refs CLOSED is limited to those refs; no universal cleanup inference. Topology association/publisher/image questions stay independent and UNKNOWN as captured.

## What is decoded, and what is missing

Windows tracerpt reported629820 events,0 lost,63 buffers and rounded30s duration. This is reported loss inside the trace, not evidence that all possible handle facts were emitted. Event types decoded include Process Start/End, Object CreateHandle/CloseHandle, HandleDCEnd and TypeDCEnd.

Filtering HEADER execution PID444 shows502 CreateHandle,522 CloseHandle and9 Object-provider opcode34/version3 records. Those are lifetime activities in execution context, NOT the balance141 or a proven leak. Header PID does not prove which process table is affected, particularly duplication. Do not add PowerShell/conhost handle counts to qualifier table. Some records contain values beginning0x8000; do not simply count every observed event as a user-table handle.

Exact demonstrated decoder gap: all9 inspected opcode34/version3 rows expose only `DataSize=26` as user data. For example timestamp134372785867151897, Object provider89497f50-effe-4440-8cf2-ce6b1cdcaca7,header PID0x1BC,version3,opcode34,payload `DataSize=26`. The local CSV does not expose source process/table/handle, destination process/table/handle, close-source options or decoded object/type for those records. It therefore cannot reconcile cross-table duplication or inherited baseline. They are duplicate candidates pending authoritative schema resolution, not fabricated decoded duplicates.

IMPORTANT: this demonstrates fields missing from the LOCAL DECODING, not missing from the ETL bytes. Do not declare the ETL insufficient until WPA/official schema-aware tooling examines it. No custom ETW parser or guessed26-byte struct was implemented.

TypeDCEnd maps type indices to Process,Thread,Token,File,Section,Key,Event,EtwRegistration,ALPC Port and others. This supports activity classification, not a simultaneous owner/type census. Light does not establish allocation stacks. A missing exact sample timestamp is known from existing qualifier log format; no round-trip adjustment is made to force balance141. Complete initial/inherited handle-table state, schema-aware duplicate ownership and sampled-event boundary remain unresolved.

## Requested attribution status

| Class | Evidence | Decision |
|---|---|---|
| Qualifier instance | receipt creation + ETW image/args/start/end | Correlated factual instance, not PID-only |
| Owner/type table | some object type/event fields decoded | PARTIAL; no full simultaneous table |
| PowerShell/conhost | distinct actors and process metrics | Separate costs; no sum into141 |
| Inherited/preexisting | duplicate payload and initial table unresolved | UNKNOWN |
| Transient | observed create/close activity | Present; no proof each delta is same owner |
| Retained | governed close0, reported own-ref closure | No universal kernel/OS release claim |
| Simultaneous balance | sample141; no exact sample timestamp/full table | NOT_PROVED |

ED11 comparison:141 exceeds128 by13 arithmetically. It does not identify13 unnecessary refs. No change to limit, security pins, canonical mutex or retained dependencies is justified. Source samples GetProcessHandleCount BEFORE PssCaptureSnapshot for that sample, so that later PSS operation is not a demonstrated explanation of141. Existing WinVerifyTrust/CRT/loader/observer paths are hypotheses until temporal/callsite attribution. Do not close mandatory references to pass.

## Minimum resolution and next step

Use an existing qualified WPA elsewhere, or install only official Windows Performance Toolkit after owner approval, then analyze THIS ETL. [Prepared installation and analysis procedure](wpa-installation-PROPOSED.md). No recapture is needed to discover whether the existing26-byte payload can be decoded; recapture currently NOT JUSTIFIED. No code correction is proposed before attribution. If WPA resolves duplicates but exact sample boundary remains ambiguous, report a bounded interval/range instead of inventing exact simultaneous ownership.

CURRENT_TASK_COMPLETED=NO
ETL_INTEGRITY=PASS_ORIGINAL_SHA256_MATCH
WPA_AVAILABILITY=NOT_LOCATED_READABLE_SCOPE_PROTECTED_SCOPE_INCOMPLETE
HANDLE_OWNERSHIP=PARTIAL_PENDING_WPA
DUPLICATION_RESOLUTION=NOT_DECODED_LOCAL_DATASIZE26
SIMULTANEOUS_HANDLE_BALANCE=NOT_PROVED
QUALIFIER_HANDLE_SAMPLE=141
HANDLE_LIMIT=128
MINIMUM_CORRECTION=OFFICIAL_WPA_AVAILABILITY_THEN_ANALYZE_SAME_ETL_NO_FUNCTIONAL_PATCH
TRACE_RECAPTURE=NO
PREFLIGHT_RETRY=NO
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=NO_USABLE_WPA_FOUND_DUPLICATE_OWNER_FIELDS_UNRESOLVED
NEXT=ED11_HANDLE_LIMIT_FOCUSED_RESOLUTION

No binary/code/accepted ADR changes, builds, installation, trace session, quotas, production0.6.4, PST/Browser/PACC wire, SCM/ACL, key/certificate/bootstrap/CA/CURRENT, clone/worktree or commit/push/tag/release effects. Only review documents/live status updated. Task remains incomplete as actual WPA attribution is unavailable; permitted fallback preparation is complete.

## WPT installed and offline analysis resumed

Owner reported instalado. Located C:\Program Files (x86)\Windows Kits\10\Windows Performance Toolkit\wpa.exe, wpaexporter.exe and xperf.exe. All FileVersion10.0.19041.5856; all Authenticode Status0, Microsoft Corporation signer. SHA256 WPA3501072b84352c9b8ea5e0c5c361ce6d7a8bc60a495cd03f27776847d44093b7; WPAExporter a2a5253cee7774673cc97d965f6c562e37e9ae77c97fbebfcfc3ade6ca861766; Xperf6ea8682bcb964a276f9b5ff657d07f33cebd05175998759eb28b24789b3a3144. Installed-payload verification is factual now; bootstrapper's signature alone was not substituted. Runtime host environment changed by owner tooling install; this does not rearm any operational admission.

Existing ETL hash checked before and after remains c5e4bb7cdce671cb5f8bc2e42f61313a78553da815c68a5f9234013b27bb2679. No recorder, preflight or service executed. Outputs only under external analysis-wpt-01. Original ETL untouched.

Official Xperf offline command succeeded0:

```text
xperf.exe -i <same handles.etl> -o <analysis-wpt-01/object-events.csv> -a dumper -provider {89497f50-effe-4440-8cf2-ce6b1cdcaca7} -add_fieldnames
```

This is the Microsoft decoder, not a custom ETW parser. It exposes named HandleCreate,HandleClose,HandleDuplicate plus ObjectType/Name. The previously unresolved version3 records are decoded into Object,SourceHandle,TargetHandle,TargetProcess fields. These are demonstrably present in the ETL. Earlier missing DataSize26 interpretation is superseded as a decoder limitation; original evidence preserved.

## Nine qualifier-related duplicate records

Times are Xperf microseconds relative to trace origin. Process/handle labels come from native decoded output, not raw-header PID inference.

| Time us | Source value | Target value | Target process |
|---|---|---|---|
|4127374|0xe8|0x80000ef0|System4|
|4127376|0x80003a3c|0xec|qualifier444|
|4127435|0xf0|0xf8|qualifier444|
|4128487|0xfffffffe|0x14c|qualifier444|
|4130759|0x154|0x180|qualifier444|
|4130795|0x158|0x180|qualifier444|
|4130832|0x15c|0x180|qualifier444|
|4184738|0x180|0x184|qualifier444|
|4193076|0x184|0x188|qualifier444|

Thus eight destinations name qualifier444 and one names System4. Do not treat all nine as nine permanent process-owned handles or automatically subtract a source on duplicate; source-close flags not exposed in this dump. Source high-bit/pseudo values must not be treated as ordinary source-table acquisitions. Target180 is reused across generations; object identity/time and closes distinguish them.

Xperf native process-name filtering yields501 CreateHandle,419 CloseHandle and9 Duplicate records in the qualifier context, whereas earlier tracerpt HEADER_PID filter gave502/522/9. These are different attribution views and lifetime activities, not simultaneous balances. Type-name lookup is available, so PowerShell/conhost tables remain separate.

CSV-only exploratory reconciliation (NOT canonical WPA simultaneous balance): excluding high-bit values and applying explicit target444 duplicates, observed additions reached156 entries at4213043us, with no conflicting ordinary-handle reuse in that model. However12 close records had no observed acquisition: File0x4,0x54,0x58,0x5c,0x578,0xae4; Section0x1dc,0x1e0,0x2f8,0x2fc,0xaa4; Event0x4d0. These cannot be silently set to zero or confidently declared inherited from these facts alone. The model's terminal observed-set empty does not establish universal cleanup. Consequently156 is an exploratory event-set result, NOT a claimed exact OS peak or authoritative replacement for141. Exact sample time still absent. No leak conclusion or limit increase follows.

Exploratory156 type split: Event14,WaitCompletionPacket11,IoCompletion5,TpWorkerFactory4,IRTimer8,EtwRegistration36,Directory2,File12,ALPC Port5,Mutant4,Semaphore12,Key7,WindowStation2,Desktop1,Token1,Process3,Thread28,Section1. These are provisional entries at a different event instant, not the141 sample's type partition. Libraries/thread pools/observer calls are candidates only; no allocation stack proof from Light.

## WPAExporter limitation and bounded next step

WPAExporter help was verified from installed binary. External profiles were based on installed Catalog XML conventions; Handles column names/GUIDs were read from the signed Microsoft.Performance.Shell HandleHdvViewModelSource.Schema. No product code or binary ETW parser was implemented. Profiles/logs retained externally. Initial invalid view GUID corrected in a new file; wildcard expansion '*' was rejected, and corrected WPA query syntax was used in later configurations. Successful exports returned0 but emitted only synthetic Lifetime groups (AIFI/AOFI/AIFO/AOFO) or header-only after filter. Exit0 therefore does NOT certify individual handle export. No data was invented from these outputs.

Default shell initialization logged missing MyPresets.wpaPresets and created normal WPA user state/Documents WPA Files. These are tool-profile state, not Accelerator operational resources. One WPA help window from this review received a graceful close request after exact PID/path/start-time verification; no qualifier/service handle or process was terminated. A broad exporter completed naturally; a later stop request found its process already absent and performed no termination. No native process-disappearance-based canonical cleanup claim is made.

Stop further hand-authored profile iterations: obtain a profile saved by the installed WPA UI with Handles table actually visible and expanded, preferably filtered to owning qualifier PID444. This is offline work on THIS ETL; does not require new trace or preflight. Save profile/export in analysis-wpt-01 under a new name. Then WPAExporter can reuse actual UI state and reconcile owner/type/lifetime rows. Do not widen operational policy to make observer fit.

Current status supersedes tooling search gate above:

WPA_AVAILABILITY=VERIFIED_INSTALLED_MICROSOFT10.0.19041.5856
WPA_EXPORT=PARTIAL_GROUPS_ONLY_NO_INDIVIDUAL_ROWS
DUPLICATION_RESOLUTION=OFFICIAL_XPERF_FIELDS_DECODED_9_RECORDS_TABLE_EFFECTS_PARTIAL
HANDLE_OWNERSHIP=PARTIAL
SIMULTANEOUS_HANDLE_BALANCE=NOT_PROVED
QUALIFIER_HANDLE_SAMPLE=141
HANDLE_LIMIT=128_UNCHANGED
CURRENT_TASK_COMPLETED=NO_PENDING_UI_SAVED_HANDLE_PROFILE
MINIMUM_CORRECTION=NONE_JUSTIFIED_BEFORE_INDIVIDUAL_HANDLE_TABLE
TRACE_RECAPTURE=NO
PREFLIGHT_RETRY=NO
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY

[Microsoft WPA Exporter](https://learn.microsoft.com/en-us/windows-hardware/test/wpt/exporter) documents offline trace/profile CSV export. [WPA query syntax](https://learn.microsoft.com/en-us/windows-hardware/test/wpt/wpa-query-syntax) describes series-name and regex queries. No claimed complete table based merely on exporter exit0. WIP/product/accepted contracts unchanged except live documentation.

## UI-saved profile received; individual WPA export completed

Owner saved handles-ui.wpaProfile at analysis-wpt-01,4553bytes, SHA2567bae1331785123b5c7c6f3c9798395c4b5d3ca5f75fb2838f07f3802123cf430. It is the installed UI's version2 WpaProfileContainer with actual Handles graph7b47975b-040d-4901-9a3f-e410228e30df. It contains Owning Process,Creating Process,Closing Process,Creation Method,Handle,Object,Handle Type,Create/Close Time. Original preserved. UI groups by Creating Process and cannot by itself prove table ownership.

Official WPAExporter on this profile succeeded0. Its grouped Count for creator444 is511; owner overview is453. Neither is simultaneous141. Exports also warn about unrelated Regions of Interest/Thread Activities filter; no ROI result is used. This warning does not erase successfully exported Handles rows.

A copy preserving native graph/schema, exposing owner/handle/object/type/creator/closer/method/times/count, and setting KeyColumnCount0 (flat, no grouping) exported individual records successfully0. Authoritative native output: external analysis-wpt-01/wpa-ui-flat-01/Handles_Flat_handle_lifetimes.csv,37983570bytes. Earlier collapsed/empty exports are not used as measurements. No hand-written ETW decoder; all ETL parsing by Microsoft WPA/Xperf. Python only joins/aggregates these native CSV fields.

## Native owner attribution and complementary-row ambiguity

Exactly453 rows have Owning Process=papacc_scm_instrumented_lab.exe (444):422 Create,19 Duplicate,12 Unknown. All counts1; these are lifetime rows, not453 simultaneously open refs. PowerShell/conhost tables excluded by explicit owner field. Original native output retained without edits.

Eleven rows document duplicate creation by elevated PowerShell12848 at4.1166799–4.1166859s, before qualifier Process Start4.1168591s. They target the qualifier table: File handles0x54,0x58,0x5c,0x578,0xae4; Section0x1dc,0x1e0,0x2f8,0x2fc,0xaa4; Event0x4d0. These are factual startup transfers consistent with inheritance; this trace does not expose the Win32 bInheritHandles/attribute-list choice. Do not declare each one unnecessary.

For each of those11, WPA also emits an Unknown-creation row at trace-time0 with the SAME owner,handle,object and actual close by qualifier at4.2176390–4.2178095s. The corresponding duplicate row has Closing Process Unknown and synthetic trace-end close30.7576148s. Naively summing both produces spurious overlapping lifetimes for the same table entry. A one-to-one complementary-row reconciliation candidate joins exact owner+handle+object, duplicate creation before actual close, and requires unique pair/no reuse. All11 uniquely pair. This produces442 candidate lifetimes:422 Create,19 Duplicate,1 acquisition Unknown. Native export remains unchanged; candidate file explicitly labels reconciliation.

Native Xperf CloseHandle names for0x54,0x58,0x5c,0x578 are Device ConDrv. The remaining Unknown-acquisition0x4 is also File/Device ConDrv and closes4.2175753s. Its start time is NOT proved by native synthetic0; do not treat it as zero cost, confidently inherited or live throughout all trace. Section/Event transfers and unnamed file0xae4 remain their observed types, not fabricated purpose.

The19 Duplicate lifetime rows include these11 parent startup transfers plus8 into qualifier from the previously decoded9 qualifier-context duplicate events; the ninth targets System4 and is not counted in qualifier's table. This resolves why execution-context event counts differ from table-owned transfer counts.

## Temporal balance and limits

External qualifier-native-analysis.json preserves exact CSV hash, all11 pairs, unknown entry and sweep results; qualifier-native-lifetimes.csv records442 reconciliation candidates. Events use exact exported decimal nanoseconds and half-open intervals, closes processed before creates at equal timestamp. Source CSV exports nine-decimal seconds; no invented original API timestamps.

Raw native453-row overlay peaks179, but double-counts11 complementary entries. Candidate reconciliation peaks168 at4.2130426s. Excluding the one unproved-acquisition entry gives167 known-acquisition candidate entries at that instant. Neither is a universal OS maximum or a replacement for GetProcessHandleCount141. They are explicit CSV lifetime models with the stated pairing assumption; no unexplained initial credit or UNKNOWN-to-zero conversion.

At candidate168: EtwRegistration36,Thread28,File18,Event15,Semaphore12,WaitCompletionPacket11,IRTimer8,Key7,Section6,IoCompletion5,ALPC Port5,TpWorkerFactory4,Mutant4,Process3,Directory2,WindowStation2,Desktop1,Token1. Candidate167 differs only by File17. This is a DIFFERENT instant from the un-timestamped141 sample, so do not present it as the141 partition.

There are18 distinct candidate time intervals with count141. Hence the single log number does not identify one unique state/type vector. Missing exact GetProcessHandleCount timing and missing acquisition for ConDrv0x4 preclude exact141 ownership proof. Light stacks/callsite attribution remain unavailable. The original ETL fields are richer than tracerpt decoding; a second capture is NOT justified merely to obtain duplication fields. Full exact sample binding would require independent timestamp/phase correlation not present in the receipt, not guessed timing from screenshot or boot wall clock.

All candidate lifetimes have observed/synthetic closure, but an empty candidate set at trace end is NOT a canonical resource-release receipt or proof every kernel/provider charge disappeared. Original factual receipt/Stop/retention semantics remain separate.

## Minimum concrete correction proposal, not implemented

The141 sample was produced INSIDE IG_CP_CAPTURE_REJECTION after census170, not evidence of a successful normal-admission phase. That rejection diagnostic executes WinVerifyTrust and process/PSS observation. CMake3316 also links wintrust directly into the qualifier, so dependencies can load before that branch executes. Do not infer a universal clean-preflight baseline from the post-denial sample or attribute36 ETW registrations/28 threads to a specific DLL without stacks.

Prepare a focal follow-up to remove/move NON-AUTHORITATIVE console provenance and rich resource diagnosis out of the canonical qualifier into the already available Windows tools, preserving first170, canonical census/exclusivity, operator/binary/config/SCM identity pins and mandatory ED11 facts. Specifically review the diagnostic-only WinVerifyTrust dependency and reject-path PSS workload before touching security references. This follows owner preference for external tools and avoids raising128. It is a proposed scope, NOT proven to save13 handles or sufficient to pass; code, contracts and pins unchanged in this task. No arbitrary handle closure, conhost whitelist, new coordinator service or quota increase.

Analysis outcome: actual WPA ownership/lifetime rows and duplication transfers are obtained; complete exact141 partition remains underdetermined. Do not repeat capture/preflight in this task. Use this existing evidence for focused qualifier-diagnostic reduction review; only define a later measurement if required to validate a separately approved patch.

WPA_AVAILABILITY=PASS_INSTALLED_SIGNED_MICROSOFT
WPA_INDIVIDUAL_EXPORT=PASS
HANDLE_OWNERSHIP=453_NATIVE_LIFETIME_ROWS_EXPLICIT_OWNER444
DUPLICATION_RESOLUTION=19_OWNED_DUPLICATE_ROWS_11_STARTUP_TRANSFERS_8_OTHER_TARGETS
SIMULTANEOUS_HANDLE_BALANCE=CONDITIONAL167_168_MODEL_EXACT141_SAMPLE_UNRESOLVED
QUALIFIER_HANDLE_SAMPLE=141
HANDLE_LIMIT=128_UNCHANGED
CURRENT_TASK_COMPLETED=NO
FIRST_UNRESOLVED_DIVERGENCE=EXACT141_SAMPLE_TIME_AND_ONE_ACQUISITION_UNKNOWN
TRACE_RECAPTURE=NO
PREFLIGHT_RETRY=NO
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=ED11_HANDLE_LIMIT_FOCUSED_RESOLUTION

Original ETL SHA256 checked afterward still c5e4bb7cdce671cb5f8bc2e42f61313a78553da815c68a5f9234013b27bb2679. User profile unchanged. Only external analysis artifacts/live documentation changed; no product code, WIP removal, accepted ADRs, quota, runtime, SCM/ACL, PST/Browser/wire or production0.6.4 changes.
