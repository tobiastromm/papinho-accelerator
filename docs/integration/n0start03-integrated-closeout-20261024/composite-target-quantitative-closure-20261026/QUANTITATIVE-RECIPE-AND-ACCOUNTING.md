# Quantitative recipe: controlled storage and observed core; not capacity authority

Date: 2026-10-26. Scope: normal API3 candidate, Accelerator OpenSSL/x64 and Browser VC6/NSS/x86. Existing typed producers are reused unchanged. No LAB object, service, Connection, socket, trust store, key, CA or new IPC was used by this experiment. Product0.6.4 is untouched.

## Compiler-backed controlled quantities

`COMPILED-CONTROLLED-LAYOUTS.json` contains six /WX compilation results and actual private source expressions. These TUs are never linked/executed. Debug/Release agree within each profile. They measure sizeof/layout and existing reservation expressions, not allocator metadata, native handles, provider internals, whole-process memory or stack high-water marks.

| Quantity | Bytes | Accounting and lifetime |
|---|---:|---|
| Accelerator C2 flow | 65,624 | Four embedded in each operation; included below |
| C2 operation | 263,760 | Two embedded in manager, including retained slots |
| C2 manager | 527,824 | Includes both operations, four session records and manager metadata |
| C3 queue | 65,560 | Two embedded per bridge |
| C3 bridge | 132,448 | Two embedded in C8 storage, borrowed runtime/scheduler not counted as owned again |
| C8 storage Selected / Legacy | 1,542,248 / 1,376,752 | Actual profile-dependent private containment |
| Existing known-managed ledger Selected / Legacy | 1,965,128 / 1,799,632 | Existing macro includes storage, declared stack reservations, fixed arrays and specified listener/ID scope |
| Existing managed limit Selected / Legacy | 3,145,728 / 2,097,152 | Preserved; remaining arithmetic is not granted credit or whole-target headroom |
| Browser C2 context | 701,984 | Actual x86 context, including receipt, three roles, HTTP and record buffers |
| Browser role / session storage / PST group | 32,836 / 23,232 / 168 | Role embedded in context; session/group components used by existing charge expression |
| Browser job / PST owner | 4,520 / 148 | Existing expression charges four jobs and three owners |
| Browser existing child reservation | 1,858,020 | Exact expression in c2_create; includes context, session/group, jobs, owners, two HTTP buffers and65,536 bytes |

Payload-only subset: four65,536-byte C2 flows plus two65,536-byte C3 queues =393,216 bytes/operation,786,432 at structural capacity2. These bytes are ALREADY inside the manager/bridge/C8 measurements. Do not add them again, add sizeof to process commit, or add a shared runtime per connection.

Browser charge arithmetic: 2,097,152 -1,858,020 =239,132 bytes of the existing controlled cap before other charges. Twice the charge is3,716,040 and cannot fit. The two-slot maximum (including retained charges) is an upper bound on slots, not a guarantee of two simultaneous normal operations. Current policy remains absent, so this arithmetic authorizes zero operations. No cap increase, charge reduction or sharing assumption is proposed to manufacture PASS. A future one-operation diagnostic is the minimum candidate; its whole-process/provider/kernel capacity remains unproved.

## New native observations

Four new runtime fixtures, /MD with backend-specific normal SDKs, compiled Debug/Release without warnings. Twenty fresh processes,12 sequential runtime lifetimes each:240 cycles,500 process samples and240 provider-info records. Public backend registration is descriptor registration; runtime creation alone does not initialize the provider. All successful records report available1/initialized0 for openssl or retrozilla-nss. No Connection or provider initialization is requested. The public release call returns and clears the fixture reference; its void return is not a backend/kernel quiescence receipt.

| Backend/configuration | Private bytes, sampled range across five processes | Handle range across five processes |
|---|---:|---:|
| OpenSSL/x64 Debug | 1,179,648–1,208,320 | 66 |
| OpenSSL/x64 Release | 1,183,744–1,232,896 | 66–72 |
| NSS/VC6/x86 Debug | 905,216–970,752 | 59–67 |
| NSS/VC6/x86 Release | 888,832–954,368 | 59–67 |

Every individual process has zero sampled private/handle growth above its own baseline during these phases and zero final difference for those two metrics. This does NOT prove zero allocation: heap reuse/granularity, lazy initialization, before-entry module loading, stdout capture and unsampled peaks remain relevant. Cross-process ranges are not per-connection deltas. Working set/pool/peak-commit values and local tick intervals are preserved separately in raw CSV/JSON. No absolute fixture count is a Browser/LAB budget or a proven ceiling.

Observer ownership: GetCurrentProcess pseudo-handle is borrowed; kernel32 module is borrowed; system PSAPI is explicitly loaded and FreeLibrary is checked once on successful runs. Measurement uses process counters, not all kernel resources. PSAPI image/system loading and stdout redirection are included in baseline and cannot be subtracted as free credit. Loader provider-module identity is NOT attested by backend name or source-package hashes. Artifact hashes bind supplied bytes, not mapped-image provenance.

## Category coverage and remaining unknowns

| Category | Metric/source | Known result | Missing property |
|---|---|---|---|
| Controlled embedded storage | Native compiler layout and source caps | Values above, exact arch/profile | Complete variable allocations and concurrent live/retained attribution |
| Browser reservation | Exact production expression compiled VC6 | One fits arithmetic, two cannot | Actual authorized host/provider/kernel policy and all other charges |
| Runtime core | Public normal SDK; own-process PSAPI/count/times | Observed scopes above | Active provider or connection costs; no hard envelope |
| Provider NSS/OpenSSL | Public available/initialized, exact SDK files hashed | Available; not initialized in fixture | Initialization, TLS handshake/session, trust/cache/threads, failed release, retained lifecycle |
| Socket/kernel | Process paged/nonpaged quotas, no socket in fixture | Process counters only | Socket buffers, global/shared kernel/cache, attribution and justified empirical residual scope |
| Shared runtime/modules | Existing process root, compiled metadata | One runtime at a time in isolated fixture | Active Browser/Accelerator roots, actual mapped modules, shared ownership trace |
| Stack/threads | Existing declared reserves/layout | Included only in applicable controlled ledger | Actual composition/thread/stack-VA observations and unsampled transient peaks |
| Retention | Existing charge/owner contracts; fixture return values | No new operational release/refund | Historical Q4 global quiescence and controlled reservations remain unchanged |

For API meaning and allocator/socket limitations reuse `../q4-target-evidence-closure-20261025/PROVIDER-RECIPE-PRIMARY-SOURCE-CHECK.md` and its official primary links. OpenSSL allocator hooks/counts and socket buffer options cannot supply a universal byte ceiling. No hooks or SDK internals were changed. Local PST source explains lazy initialization but is not substituted for pinned binary provenance.

## Failures preserved

`INITIAL-UNREGISTERED-CORE.log`: the first isolated fixture omitted normal public descriptor registration. Runtime creation succeeded with zero registered providers, which violated the fixture expectation; first13, logical runtime/module references retained, then process exited. This was a test-assumption failure, not proof of native leakage, quiescence or refund. No automatic retry in that process, no operational effect. Corrected fixtures use public registration in fresh processes. Initial /WX C4996 compile failure is archived separately; bounded memcpy corrected it, no warning suppression. Both failures remain in evidence.

The earlier wrong-LAB-profile rejected fixture in the prior package remains independently recorded; its uninstrumented internal mutex outcome remains UNKNOWN. This task neither reran it nor relabelled it PASS.

## Proof boundary

The new evidence closes the missing compiled reservation arithmetic and runtime-core observation slices. It does not close active provider/kernel coverage, correlated target capacity, freshness policy, host headroom, E17/E18/E21 or E22. Bounds on controlled quantities plus empirical samples are ingredients, not an accepted hybrid guarantee. Policy ABSENT, RESOURCE_UNKNOWN and admission DENY remain unchanged.
