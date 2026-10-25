# Native measurement and stack evidence

## Reproducibility and scope

Windows10.0.19045 x64, local fixed C: NTFS volume serial417614100, session1; no service or token substitution. Existing native file capsule is included unchanged. Six FINAL serial processes: three Debug and three Release using the host's MSVC19.51 /MD CRT, same optimization/debug flags (/Zi /Ob0 /Od /RTC1 versus /O2 /Ob2 /DNDEBUG), /W4 /WX. No production build relink/rebind. Four actual service translation units Selected/Legacy Debug/Release were separately compiled to /FAs listings with their exact Ninja command/flags and scratch outputs. Compile warnings0. This is not a rerun of all application builds/tests; diagnostics functional sources are unchanged.

Final raw files: evidence/measurement-final-*.jsonl; [run receipts](evidence/measurement-final-runs.json), [phase reconciliation](evidence/measurements-final-reconciled.json).156 snapshots across6 final runs; all memory/host/available/handle/I/O/stack queries returned success, native write/flush/close receipts success. Six earlier exploratory runs (initial Debug /MDd) remain separately archived and are not used to qualify host-CRT comparisons. Initial supervisor annotation failed after the first successful/closed run due to a Python stat-size typo; its existing success receipt was reconciled, never regenerated. Missing wall-clock annotations remain null. No native failure/orphan was produced. CALL-wrapper control transfer initially omitted Legacy listings; only the missing objects were compiled afterward, no measurement replay over old files.

Only new engineering scratch files, each CREATE_NEW with descriptor AT CREATION (OW/SYSTEM/BA fixture, NOT productive VSA qualification). Parent native volume checked; arm/write/flush/prep close then OPEN_EXISTING/same native ID/read/one append/flush/writer close, final content extraction and parent close. Six final + six exploratory files retained,2048 logical bytes each; no deletion/overwrite/old attempt reuse. Same code failure policy retains uncertain refs/parks and supervisor never kills/retries; this path was not needed because all runs completed cleanly.

Observer has a static24064-byte sample array, plus its own CRT/stack/file-metadata buffers and PSRF fixture states. It pre-touches sample storage, captures cold/warm/stable pre-file baselines and writes evidence only after sequence, avoiding logging writes during measured phases. Snapshots themselves cost resources/time and are not subtracted or assumed free. Parent overlaps preparer/writer; maximum explicitly-owned refs2. Output redirection is measurement evidence only; file sink does not depend on it. No listener/network/CA/SCM/Start/ACL-existing operation.

## Observed facts, not hard maxima

| Metric | Six final observations | Limit of interpretation |
|---|---|---|
| File output |2048 bytes and two bounded1024 writes per run | Logical output, not disk allocation or kernel memory |
| I/O counters |2 read ops/2048 bytes (arm plus extraction),2 write ops/2048 bytes | Process counters corroborate this fixture, not all OS background I/O |
| Process handles | Stable baseline67 or73; observed peak69 or75; +2; final baseline restored | Count checked against owner model; not per-handle kernel byte size or universal process peak |
| Private commit/bytes | Baseline/max1335296–1404928 across runs; observed post-baseline growth0 | Existing committed arenas/baseline can service work; does NOT mean zero allocation/native cost |
| Process quota pool counters | No rise above stable baseline in final snapshots | Charged process quotas are not all filesystem/cache allocations |
| Main-thread committed stack VA | Sampled24576 or28672 bytes | Not physical residency, not real ServiceMain stack, not a transitive upper bound |
| Stack address span |1048576 bytes; remainder reserved VA | Reserve is not physical RAM; do not add reserve+commit as two allocations |
| Host pool/cache/commit | Variable shared values; signed global deltas in JSON | Other processes and kernel activity concurrent. No per-sink attribution or universal ceiling |
| Host available physical | Final-series minima8058093568 (Debug),8078274560 (Release) bytes | Historical observation only; not future available capacity, margin policy or admission grant |

Queries bracket native calls, not continuous kernel sampling. OS process high-water counters and sampled current values are distinguished; opaque in-flight/shared peaks remain UNKNOWN. Host quota counters/cache-page totals are not silently zero nor charged wholly to one2048-byte file.

## Exact-candidate baseline calibration

Read-only --help path was audited: service CLI/dispatcher reject the non-service action before SCM acquisition; HELP returns before runtime/config/listener. Four pinned existing candidate executables run --help with CREATE_NO_WINDOW, original child process handles and read-only counters. All exit0,83 samples, no query errors. Results [JSON](evidence/host-help-process-baselines.json), output logs/hashes retained. No ServiceMain/psr_prepare is invoked.

Observed private-byte maxima: Selected Debug3031040, Selected Release2691072; Legacy Debug2854912, Legacy Release2539520. Those are loader/CRT/process observations, NOT controlled C2 allocations or a source-quota violation; Legacy's managed2097152 does not mean its whole process Private Bytes must be <=2097152. Costs remain in total/baseline/increment/host accounting. Help is not a service/runtime peak, and its baseline cannot be added to a probe process total as if both were one future native maximum.

## Real stack call graph

[evidence/stack-frames-and-callgraph.json](evidence/stack-frames-and-callgraph.json) records prologue sub-RSP, saved-register pushes and explicit call edges. All four raw compiler listings are retained. Known static frame extents EXCLUDING each function's return slot:

| Function | Debug | Release |
|---|---:|---:|
| ps_main |56|40|
| psr_prepare |61784|61768|
| psr_open |392|200|
| psrt_return |1032|1000|
| psrd_emit |88|56|
| psrf_finish |152|168|
| psrf_write (when emitted as standalone call) |88|56|

Selected and Legacy agree for these frames. Debug path ps_main→psr_prepare→psr_open→psrt_return→psrd_emit→psrf_finish→psrf_write has63648 known bytes INCLUDING one8-byte return slot per shown frame. Release inlines write/close helpers into finish; emitted corresponding path ends at finish and sums63280, not a sum of inlined and standalone copies. Success helper/prepared is also inlined in Release; its explicit known prefix is62064, versus62576 Debug. Descriptor-binding path known prefix63328 Debug /62704 Release. These are demonstrable OWNED-OBJECT PREFIXES, not complete transitive stack maxima.

psr_prepare's ~61KiB frame (PC_DOCUMENT/diagnostic/path locals) remains live while capture functions run. Summing only text768 + file-buffer1024 would miss it; conversely frame1024 is static owner storage and must not be added as another local frame. Formatting CRT calls are sequential siblings of sink calls: do not sum them as simultaneous descendants. _snprintf_s/_vsnprintf_s_l/stdio helpers, imported Win32/native calls, PC parser/other translation units, SCM callback thread setup/other threads, debugger/RTC helpers and hardware/kernel entry stack are not closed by these four object listings. /RTC and optimization affect prologues. No1MiB PE reserve is substituted for committed/resident usage or callback-stack proof. No existing native/CRT stack requirement is waived.
