# Python native test supervisor feasibility — isolated experiment

TASK=PACC-PHASE6-PYTHON-NATIVE-TEST-SUPERVISOR-FEASIBILITY-1
2026-10-24; main; HEAD913c90cf2328f7939d8f5b927640601903ac6e91.

Conclusion: Python is feasible and substantially lighter as an EXTERNAL NONOPERATIONAL FIXTURE TEST SUPERVISOR on this Windows10 host. It does not inherently reduce process count relative to one already-elevated PowerShell or eliminate console infrastructure. pythonw did not eliminate an observed extra conhost for this fixture, including CREATE_NO_WINDOW. Recommend Python in an existing console for isolated engineering tests; keep the accepted single elevated PowerShell for the governed LAB operation unless a separately reviewed normative/source change replaces it. No qualifier/app/launcher change or operational qualification.

## Authorities and environment

Read accepted [N0-N9](../../adr/DECISION-first-native-lab-diagnostic-boundary-20261024.md) and [next minimal slice](../first-native-lab-diagnostic-boundary-acceptance-20261024/next-minimal-implementation-slice.md), governing B+D/B0-B6, current/previous console topology evidence, continuous-session Model E/ED08/ED11 and launcher source. Current scm-handle-requalification-uac.ps1 still has Prepare->UAC elevated PowerShell path and inherited-console ProcessStartInfo; it was READ ONLY, never executed. Operational Prepare, Check, positive qualifier and SCM were never invoked. The accepted future topology is ONE already-elevated PowerShell, distinct from the historical two-shell launcher.

Python discovered then used ONLY by absolute paths C:/Python314/python.exe and pythonw.exe, not py.exe auto-install/default resolution. Runtime query:3.14.7 (tags/v3.14.7:823f032, Aug5 2026,10:51:32), MSCv.1944, AMD64,64-bit. python.exe/pythonw.exe/python314.dll Authenticode status Valid, signer Python Software Foundation; hashes in python-provenance.json. This proves the checked file signature result, not a universal supply-chain or mapped-image guarantee. No installation, venv, pip/dependencies, registry/system policy/ACL change. Library standard only: subprocess/ctypes/json/pathlib/time etc.

Fixture built with existing VS18 BuildTools/MSVC19.51, /O2 /MD /W4 /WX, successful zero-warning compile. This is ONE isolated x64 fixture, not four product/profile builds or a Win7 qualification. Python3.14 documentation supports Windows10+, not Windows7; installing another Python was neither needed nor done.

## Experiment design and boundaries

All executable harness/fixture source and artifacts ONLY in C:/Projetos/_tmp/PapinhoAccelerator/python-native-test-supervisor-feasibility-20261024. Canonical repository receives report/selected measurements only. No clones/worktrees. Fixture has NO SCM/network/CA/config/intent authority: query own process metrics/birth/console context, emit bounded READY JSON, read exactly STOP newline on stdin, emit STOP_ACK, return0. CONIN/CONOUT probes are synthetic console availability observations, not proof of operator confirmation or host authentication. No canonical owner/intent was acquired or consumed. Fixture no operational file creation. Scratch outputs/receipts are experimental evidence only.

A controller established ONE hidden experimental console with CREATE_NEW_CONSOLE before all comparisons (SW_HIDE; no user window required). Therefore A/C test sharing an ALREADY EXISTING EXPERIMENT console, not the owner's elevated interactive desktop. Initial controller had one console client; console infrastructure is baseline, not zero/free or authenticated. Controller/outer launcher and their observer costs are external experiment overhead, NOT admitted B4 processes or credits. Replacing operational supervision with this measurement controller would add processes and is not proposed.

One campaign, four distinct cases, each once: A python.exe in inherited console, B1 pythonw.exe/default CUI child, B2 pythonw.exe/CREATE_NO_WINDOW child, C Windows PowerShell5.1 with same UseShellExecute=false/CreateNoWindow=false ProcessStartInfo mechanism as current launcher. All cases pipe child stdin/stdout/stderr for a uniform synthetic Stop+capture experiment. Actual launcher redirects stdout only and keeps operator input/stderr; C is a mechanism fixture, NOT executing the actual launcher or reproducing every byte/handle of it. No UAC/elevated native operation or manual confirmation test.

Supervisor emits ready/current/final metrics. Separate controller samples live original child-supervisor Popen process handle about100ms, collects Toolhelp PID/parent/name/thread snapshots, baseline/during/final, and waits for actual exit. Native fixture records self PID+creation, handles/threads/Private/WS and console probes. Python uses ctypes query APIs; PowerShell self metrics uses .NET Process, adding method-specific query/wrapper overhead. Samples are sequential, not atomic simultaneous maxima. No metrics are a universal ceiling; short processes/picos can be missed. Toolhelp new conhost rows are only observed PID/parent/name/thread correlations; no authenticated relationship or file provenance claim. All raw snapshots stay scratch, not expanded into repo reports.

The prototype uses CPython's private _handle for native sampling/explicit wrapper closure, so it is not a supported reusable operational API contract. Any durable harness would need reviewed handle-lifetime use independent of that private detail. This isolated supervisor is not production-ready or automatically qualified for UNKNOWN retention. Successful normal paths checked exit/pipe disposal/native query-handle close. Failure retention branch was prepared but NOT exercised: sample wait timeout would preserve process references and report UNKNOWN, no kill/retry. ReadLine could block for an unexpected fixture; no hard-cancellation guarantee is claimed. No subprocess.run(timeout) auto-kill policy was used.

## Factual comparison

READY is the matched observation point; units below Private Bytes MiB rounded, exact values in measurements.json.

| Case | Supervisor READY handles / threads / Private | Fixture handles | Newly observed operation-related rows relative to pre-case snapshot | Console outcome |
|---|---|---|---|---|
| A python.exe inherited |107 /4 /10.61MiB (11124736B)|46|python+fixture; no additional related conhost observed|Supervisor2 clients, fixture3; CONIN/OUT available |
| B1 pythonw, default child |102 /4 /10.64MiB (11161600B)|40|pythonw+fixture+new snapshot conhost20520 parent21744|Supervisor no console (error6); fixture1 client with CONIN/OUT |
| B2 pythonw, CREATE_NO_WINDOW child |102 /4 /10.98MiB (11517952B)|40|pythonw+fixture+new snapshot conhost2400 parent14904|Supervisor no console(error6); fixture1 client with CONIN/OUT. No elimination of host proved |
| C PowerShell mechanism |605 /25 /59.36MiB (62242816B)|46|PowerShell+fixture; no additional related conhost observed|Fixture3 clients; CONIN/OUT available |

At READY A versus C: **498 fewer supervisor handles**,21 fewer threads,51118080 bytes fewer Private Bytes (~48.75MiB). Relative sampled handle maxima108 versus606 produce the same498 delta; max private samples differ and are NOT universal peaks. Final self handles after fixture closure A103/B1 98/B2 98/C600; change from READY reflects closed child/pipe/wrapper resources plus runtime/observer variability, not a leak proof. Fixture metrics are not qualifier handles; historical qualifier106/128 and HANDLE_GATE NOT_REACHED remain unchanged. Total admin+auxiliary metrics were NOT qualified; console/kernel/transitive stack/host available capacity are not zero and no credit arises from this difference.

Application process delta A versus a single PowerShell mechanism is ZERO: one supervisor plus one fixture. Both share existing experimental console and no additional related console process was observed. pythonw has two application processes too but each B run observed one extra conhost candidate row. These are per-case sampled observations, not universal topology promises or old historical PID identities.

All four READY->STOP_ACK have consistent fixture PID+creation, exit0, no extra output bytes, no stderr bytes; supervisors exit0, child pipe/process wrappers closed and controller waited/disposed supervisor references. Elapsed case windows ~1.08s/1.43s/1.03s/2.21s include startup and deliberate750ms overlap, not a performance benchmark. Console hosts' independent termination/cleanup is NOT certified by fixture exit or a final snapshot. No retry/kill or unresolved timeout occurred. Evidence/results and exact source hashes in scratch-manifest.json; no operational result inferred.

## Confirmation, Stop, security and recommendation

- Python-console preserves a possible interactive transport at API level, but this run used pipes and hidden console; it did NOT verify human exact confirmation, elevation/no-impersonation, native administrative binding, Ctrl+C/Break cancellation, actual service Stop or canonical retained lifetime. Native qualifier remains sole authority; Python cannot approve its own plan or consume/rearm its intent.
- pythonw supervisor has no own confirmation/Stop console. The child's extra console/CONIN does not restore attributable operator authority. Synthetic stdin STOP works, but is not SCM ControlService or accepted Stop. No hidden/detached execution bypasses current census/confirmation/Stop clauses.
- Current accepted N1/R5 and source actor-path binding explicitly select Windows PowerShell. Replacing it by Python needs separate normative exact actor/topology/binding review and native negative qualification, not just interpreter signature. Adding Python alongside PowerShell/qualifier/service/auxiliary can create a FIFTH role and is forbidden without separate scope; this experiment authorizes none.
- Python standard subprocess simplifies argument arrays, bounded pipe capture, exit/error receipts and external fixture orchestration, with lower observed runtime footprint. It does not replace resource observation, required security/provenance facts, native exclusivity, one-shot consumption, Stop/quiescence or UNKNOWN. CPython dependency/OS support/pins and script/handle lifetime introduce their own review burden. Current interpreter cannot establish Windows7 compatibility.

Recommended supervisor: **Python console for isolated nonoperational native-fixture engineering tests; accepted single elevated PowerShell remains the governed LAB operator until a separately approved supervisor decision.** Do not select pythonw as a console-infrastructure solution from these results. For next operational slice prefer implementing already accepted N0-N9 rather than delaying first diagnosis for a new supervisor architecture. Keep Python optional/external to product; no mandatory product dependency added.

## References and preservation

Official [subprocess](https://docs.python.org/3.14/library/subprocess.html): Popen can pipe output and report exit; CREATE_NO_WINDOW is window suppression, not a contract of no OS infrastructure. run(timeout) kills on timeout; Popen.wait timeout raises without automatically killing. This distinction matters to retention and is preserved, not imported as cleanup permission. [Windows support](https://docs.python.org/3.14/using/windows.html#supported-windows-versions): Python3.14 Windows10+. Documentation retrieved may show newer3.14 patch; actual runtime version above is measured locally and pinned independently.

Pre-write repository inventory and capability preimage held in scratch. Verification checks unchanged HEAD, source/launcher/binary-policy pins and all preexisting git-listed/nonignored WIP except the intended documentation append. No changes to PST/Browser/wire/production0.6.4/quotas, commit/push/tag/release, bootstrap/CA/keys/certs/CURRENT. Native fixture build/test only; no service, operational qualifier, intent or ETW capture. No operational admission granted.

CURRENT_TASK_COMPLETED=YES
PYTHON_AVAILABLE=YES_PSF_SIGNED_X64
PYTHON_VERSION=3.14.7
PYTHON_SUPERVISOR_FEASIBILITY=YES_EXTERNAL_NONOPERATIONAL_FIXTURES_NOT_OPERATIONALLY_QUALIFIED
CONSOLE_PROCESS_DELTA=A_VS_SINGLE_PS_ZERO_BOTH_NO_NEW_RELATED_HOST_OBSERVED_PYTHONW_EXTRA_HOST_OBSERVED
HANDLE_DELTA=PYTHON107_MINUS_POWERSHELL605_EQUALS_MINUS498_AT_READY
SECURITY_COMPATIBILITY=CANONICAL_AUTHORITY_UNCHANGED_OPERATIONAL_SUBSTITUTION_NOT_AUTHORIZED
RECOMMENDED_TEST_SUPERVISOR=PYTHON_CONSOLE_FOR_FIXTURES_EXISTING_ACCEPTED_PS_FOR_LAB
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES
NEXT=CANONICAL_N0_N9_IMPLEMENTATION_OR_SUPERVISOR_DECISION
