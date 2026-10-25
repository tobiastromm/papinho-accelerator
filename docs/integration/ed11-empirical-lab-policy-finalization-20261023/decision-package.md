# Exact numeric risk choices — owner decision requested

Normative source is the SAME [B0–B6 candidate revision4](../../adr/ADR-CANDIDATE-ed11-single-lab-diagnostic-native-boundary-20261023.md). This review/summary is not a second contract. All values below are PROPOSED. No acceptance or operating authority follows from this package.

## Evidence versus choices

Facts: same host/boot236441, Windows10.0.19045 AMD64; historical physical capacity21300559872 bytes and20:29Z available RAM6285791232/commit17306345472. Current service20:45Z STOPPED,DEMAND,correct virtual account/SID type; currently registered image is STILL the old candidate. Selected Release instrumented candidate SHA256eda84c0cf91ad1266cee462e62c89b84ffa5873853a0122215d15d666701361b, config SHA256a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44. Boot/host/native file and all exact identity checks remain mandatory at operation review. Old observation age makes it INELIGIBLE now, not automatic capacity approval.

Proven controlled values: Selected3145728/Legacy2097152 bytes unchanged; diagnostic4400 nested once in named service25124, maximum2 direct diagnostic refs, ZERO added diagnostic threads,1024-byte append frame/2048-byte final logical file. Actual HELP/private baseline2539520–3031040 bytes and observed handles up to77 are finite baseline evidence, not runtime maxima. Compiler known stack prefixes63648 Debug/63280 Release exclude the native/CRT/callback remainder. No existing32MiB/8MiB candidate incremental envelope is relabelled a qualified quota.

| Proposed numeric decision | Why selected / what it does NOT prove |
|---|---|
| Preflight oldest metric age<=60000ms | One short manual confirmation window; expiry prevents indefinitely reused dossiers. Chosen operational tolerance, not observed safe drift |
| Final oldest capacity metric age<=1000ms before Start | Fresh recheck AFTER confirmation; no user wait/process launch in between. Narrower exposure to host changes, not an external memory reservation |
| At most2 pre-effect metric generations | Preflight and final; expired final => STOP, no auto-rearming loop |
| Sampling target100ms, maximum gap1000ms | Nominal10Hz observation; gap loses qualification and suppresses new effects. Does not catch all bursts |
| Available physical at admission>=5368709120 bytes (5GiB) | Selected4GiB host floor plus1GiB operational risk allowance |
| Available commit at admission>=9663676416 bytes (9GiB) | Selected8GiB floor plus same1GiB allowance; different capacity dimension, not another memory allocation |
| Host monitoring floors4GiB physical/8GiB commit | Preserve substantial background capacity; breach leads safe Stop/retention. No protection against all external consumers |
| One attempt, max4 admitted processes | Launcher PowerShell + elevated PowerShell + native qualifier + service. No extra compiler/receiver/worker in admitted interval |
| EACH PowerShell memory guard256MiB/handles1024/threads64/observable committed stack32MiB | Deliberately chosen finite administration tolerance for CLR/PowerShell; not a measured maximum |
| EACH native participant memory guard64MiB/handles128/threads16/observable committed stack8MiB | Small finite native diagnostics tolerance; baseline evidence only informs scale, not safety proof |
| Sum memory guards640MiB/handles2304/threads160/committed stack80MiB | Stack is within commit, not added; shared/overlapping counters not summed as independent allocations |
|1GiB allowance minus640MiB guards leaves384MiB policy cushion | Unallocated risk cushion, NOT proof that native UNKNOWN fits, not available host credit |
| HOST_CREDITS=NONE_GRANTED | No reusable/refund credit, quota difference or historical UNKNOWN resource refund |
| At most1 Start/1 Stop/1 exclusive diagnostic file | One consumed intent; error does not rearm; no automatic retry/delete/overwrite |

Rationale is explicitly an operational risk choice: the measured host capacity and limited intended workload permit proposing conservative finite floors/guards for OWNER review. No algebra turns the old memory snapshot or HELP peak into a safe service maximum. Selected allowances are not C2 quota increases, managed allocations, Windows Job limits or provider/kernel ceilings. Host5/9GiB is deliberately conservative eligibility, not an additional charge in a total-memory census; count actual owner storage once. A floor check is not an OS reservation. Existing live launcher/observer costs remain in the measured baseline, never subtracted as guessed credit.

## Remaining risk and exact approval question

Owner is asked to approve these exact choices plus the possibility of unobserved/in-flight or between-sample kernel/cache/stack/concurrency bursts, imperfect attribution and delayed synchronous I/O/Stop. Sampling/guard checks cannot enforce fail-before-all-native-allocation. This exception covers only the first instrumented measurement; measured service peak is the OUTCOME. Do not demand a prior service peak to approve that measurement. Native or process guard crossing is a non-clean diagnostic outcome, not permission to exceed controlled caps, evict retained refs or claim cleanup. If Stop cannot safely be accepted, preserve query-only retention/UNKNOWN; no kill/retry/fabricated STOPPED.

All required pre-effect process metrics apply only to already-live participants. Do not require or invent a service PID/token/stack before Start; those are actual future outcomes. Future implementation must demonstrate bounded observer buffers and canonical owner lifetimes, exact candidate/config/native references, pre-effect metrics/arithmetic/time checks, topology and exclusivity. NO concurrent project admission may spend the same host state; one GUID/process-local flag is insufficient proof. If existing-owner exclusivity cannot be qualified, exact-operation admission remains STOP even after normative acceptance. No new authority/registry/worker is authorized.

Invalidate on changed boot/host/image/config/native identity/service/name/account/SID/arguments/operator/token/OS/filesystem/topology or relevant environment, unexpected concurrency, stale/failed metrics or Stop. Actual token/readiness/first NTSTATUS/process state/retention must be captured factually during the cycle; missed facts remain UNKNOWN and cannot qualify a successful run.

The precise owner decision still required: approve or revise B4.B.2–B4.B.6 numerical risk tolerances and B4.D residual-risk scope as ONE B0–B6 profile. Approval is normative only. Afterwards record ACCEPTED through exact official review; then qualify the successor launcher/observer/exclusivity/rebind/file preparation and obtain separate manual operation approval. Nothing in this task starts a service, prepares a live diagnostic file, initializes CA or grants authority.

Governance: AGENTS.md requires human approval of new proposed durable decisions. Values were not previously approved by owner; automatic tool execution approval or passing the read-only test is NOT the missing normative risk acceptance. Profile stays PROPOSED until that explicit decision. No generic architecture reopening or new competing ADR.
