# WPRH03 manual result and first offline decoding

2026-10-23 local / coordinator UTC2026-10-24T01:23. Capture completed; no retry needed to obtain a trace. No operational admission or service effect.

## Verified artifacts

External ETL C:\Projetos\_tmp\PapinhoAccelerator\wpr-handle-attribution-20261023-03\handles.etl:66060288bytes (63MiB), SHA256c5e4bb7cdce671cb5f8bc2e42f61313a78553da815c68a5f9234013b27bb2679. Hash independently recomputed against coordinator output. Coordinator.log preserved beside ETL. Pasted console output at owner attachment967e3e9c-573f-4947-8768-aed4938d4635 matches log tail.

Host/boot accepted tuple was reported by the corrected coordinator; WPR start0 and explicit recording status preceded the qualifier launch. Supervisor820, launcher8420 creation134372785828487950. Stop request30192ms, return32326ms. Thus request overshot selected30s window by192ms, completion by2326ms; preserve factual deviation rather than claim hard30s guarantee. No automatic retry. Stop0 and post-stop elevated status WPR is not recording. This is owned session disposition, not universal kernel cleanup proof.

Prepare launcher exit2 is distinct from empty coordinator FIRST_ERROR. Empty wrapper error means coordinator steps succeeded, NOT admission PASS. Native receipt docs/integration/canonical-preflight-busy-diagnosis-20261023/manual-prepare-134372785848378082-final.json reports first170 PREFLIGHT_CENSUS, rejected conhost22520 parent8420; original actor8420, elevated12848, qualifier444 creation31286102:957726436 (=134372785867046628). Metric for qualifier141 handles, not historical147. Guard128 unchanged; no Start/Stop, intent/effect/diagnostic_created0; close0 and owned capture references CLOSED reported. Session/console association remains UNKNOWN. User pressed F10 after factual qualifier return; launcher termination was observed by coordinator.

## Existing-tool offline analysis

No WPA/wpaexporter/xperf found on PATH in this check; this does not establish global absence. Existing Windows tracerpt10.0.19041.3636 was used only on the saved ETL, not realtime. Command:

```text
tracerpt.exe <external handles.etl> -o <external analysis-tracerpt-01/events.csv> -of CSV -summary <external analysis-tracerpt-01/summary.txt>
```

Destination was new and exclusive under external capture directory. No -y overwrite, -rt, extra trace, preflight or installation. Exit0. Raw CSV can contain other processes' names/paths; keep external, do not commit. ETL remains immutable.

Native summary:63 buffers processed,629820 events processed,0 events lost reported, elapsed30sec (summary rounded). This is reported trace loss, not universal observation completeness. Object provider89497f50-effe-4440-8cf2-ce6b1cdcaca7 includes CreateHandle161150, CloseHandle169195, HandleDCEnd116603, TypeDCEnd66; opcode34/version3 events11217 are not decoded into named payloads by this local output. These are HOST-WIDE counts, never qualifier counts.

Decoded process Start names the pinned qualifier image and --prepare arguments, payload PID444,parent12848. Start event FILETIME134372785867046748,120x100ns after receipt creation; receipt instance and event image/argument context support this correlation without treating header creatorPID as target. End event134372785868052935 reports exit2. No PID-only reuse assumption is made.

Filtering CSV HEADER PID444 yields502 Object/CreateHandle and522 Object/CloseHandle records, plus9 unresolved opcode34 records. Header PID is execution context and is NOT automatically the destination/owner of every handle table affected. These counts are activity over the full process lifetime, not141 simultaneous handles or a leak balance. Some handle values in records begin0x8000; do not add them blindly to user-table handle count. Duplication/initial ownership/objects reused/cross-process activity require decoded schemas before reconstruction.

TypeDCEnd provides object-type index/name mapping. Example activity candidates include Key, Thread, EtwRegistration, File, Section, Event, Token and Process. This establishes available evidence classes, not a complete simultaneous attribution. No physical/kernel cost is inferred from one handle event. No allocation stacks were established from Light. Absence of loss does not fill schema/initial inheritance gaps.

## Current classification

A necessary qualifier-owned handles: code-backed identity/pin/admission references exist, but complete measured simultaneous allocation not yet reconciled.
B PowerShell/console: separate actor samples and process events; do not add their tables to qualifier141. Supervisor/WPR overhead is external diagnostic cost, not zero or B4 admission.
C inherited: unresolved. Parentage/stdout redirection alone are not inheritance evidence; inspect duplicate/source/destination and process-start tables in WPA.
D transient: repeated create/close activity demonstrated. Not all events are permanent refs; do not label all502 necessary baseline or522 releases of matching owned acquisitions.
E retained: governed close0 and capture-reference closure reported; no retroactive claim that every kernel/provider object is freed. Process End alone does not substitute for canonical cleanup receipts.
F unresolved:141 sample not fully attributed. Unnamed duplicate schema and missing exact GetProcessHandleCount timestamp prevent faithful full balance.

Observer source samples handles BEFORE PssCaptureSnapshot for that sample. Do not blame the subsequent snapshot as cause of141 without timeline evidence. Thread/registry/ETW activity may arise from Windows libraries, verification or observer paths; exact callsite attribution remains pending because Light stacks not established.

## Minimum next step

Analyze THIS ETL in an existing compatible WPA installation, offline; resolve handle duplicate/version3 schema and per-process table ownership, then align temporal census near qualifier log (sample lacks exact timestamp). No fresh trace or custom diagnostic collector is justified yet. WPA availability can be established separately; do not silently install an ADK or claim analysis already performed in WPA. If this ETL cannot resolve inheritance/callsite, state exact missing field before proposing any single new experiment.

No change to qualifier/launcher, threshold128, B4 topology, canonical exclusivity, quotas or authorization is supported yet. No memory/handle credit granted. Do not close security references merely to fit128. Minimal correction remains pending attribution; trace success is not operational PASS.

TRACE_CAPTURE=COMPLETED_WITH_WINDOW_OVERSHOOT_RECORDED
ETL_INTEGRITY=VERIFIED
WPR_FINAL_STATUS=NOT_RECORDING_REPORTED_ELEVATED
OFFLINE_DECODER=WINDOWS_TRACERPT_EXIT0
TRACE_EVENTS_LOST=0_REPORTED
WPA_ANALYSIS=PENDING_TOOL_AVAILABILITY
HANDLE_ATTRIBUTION=PARTIAL_EVENT_ACTIVITY_NOT_SIMULTANEOUS_BALANCE
QUALIFIER_HANDLE_SAMPLE=141
HANDLE_LIMIT=128_UNCHANGED
CANONICAL_PREFLIGHT=DENY_170_PRESERVED
START_SERVICE=NOT_RUN
STOP_SERVICE=NOT_RUN
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
OPERATIONAL_INTENT_CONSUMED=0_REPORTED
HOST_CREDITS=NONE_GRANTED
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=OFFLINE_WPA_ANALYSIS_OF_EXISTING_ETL_NO_RECAPTURE

[Microsoft tracerpt documentation](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/tracerpt) describes offline ETL parsing and CSV/summary output and lists Windows10 support. No extra diagnostic software installed. Production0.6.4/PST/Browser/wire/WIP/historical LABs preserved; no commits or publication.
