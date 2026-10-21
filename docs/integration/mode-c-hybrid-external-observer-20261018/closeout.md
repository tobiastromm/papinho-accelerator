# Hybrid external observer integration — 2026-10-18
TASK=PACC-PHASE6-MODE-C-HYBRID-EXTERNAL-OBSERVER-INTEGRATION-1
HEAD main c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Official escalated source operations approved; no approval denial or workaround. Existing functional owner, Browser, PST and wire unchanged. Accepted ED11/Mode C validity/Model E remain unchanged. Campaign is NOT executable by this result.

## Implementation
Adapted the existing tests/diagnostic/ed11_total_probe/ed11_c1_external_observer.c, not a second collector. It now accepts ONLY an observer-owned inherited duplicate process handle from a governed launcher. No OpenProcess(PID), product creation, application Stop, product termination or pipe. PID and process creation must match before image/metrics. Existing Win32 resource calls are reused. Actual QueryFullProcessImageName path is observed, file held share-read only, SHA256 before/after and stable volume/file-index/size/last-write identity checked. This is observed path/file association, not memory-page or all-DLL attestation; a preexisting launcher image lock/mapping proof remains important. Unknown build/config refs stay null, never invented. File access time is deliberately excluded from identity equality.

The obsolete one-argument fixture-launch CLI is now rejected. Historical C1 outputs are preserved; the original source is in preimages/. The legacy CMake target only gains its required bcrypt linkage; historical SDK selection/checks are not relaxed. Standalone documented MSVC19.51 x64 /TC /W4 /WX /MD Debug/Release recipes build this external tool without product/PST linkage. This is a modern diagnostic tool, not an NT4 or VC6 product qualification.

Each query emits QUERY_BEGIN before calls and a completion sample or explicit failure afterward. No completion means PENDING; timeout is not cancellation or cleanup. Requests have a finite count1..4096 and observer Sleep interval0..60000ms as tool input bounds only, not approved campaign duration/freshness/headroom. There is no automatic timeout kill, retry-until-PASS, provider/kernel bound or phase inference. An already exited process yields no resource sample and no local-teardown inference.

Output is a CREATE_NEW disk file, not stdout piping. Observer process handle, observed-image file and output handle belong to diagnostic process. Image/process close results are recorded once; close failure is retained and nonzero, no retry. Output close failure is nonzero. If output creation fails, process-handle close result is reported on stderr and return is nonzero. Process exit never proves product cleanup, even if OS ultimately reclaims diagnostic handles. A blocked query may leave observer alive/pending; nothing here forces its finalization or says CloseHandle can cancel it. Observer process must not be put in a kill-on-close job that owns the products.

Existing tools/offline/mode_c_pair.py gains a separate bounded offline correlate_external_resources function; legacy CLI/consumers remain unchanged. It checks expected artifact SHA256, run, process creation/PID/image hash against a reported existing receipt, exact query sequence/completion, local time order and closure records. Contradiction/integrity is INVALID; absent/blocked/retained evidence remains INCOMPLETE. Process-resource join is reported independently from phase/context. No CURRENT/epoch/grant/operation is generated or inferred. No new receipt format is required: it consumes diagnostic log events plus existing receipt identities.

## Validation and limits
Final Debug/Release builds pass zero warnings. Eighteen new synthetic offline cases pass; nineteen existing pair and twenty-eight provenance cases pass (65 total synthetic tests). Eight final native cases (four/configuration) pass: live Python-host observation, stale creation rejection, insufficient synchronization access rejection, and already-exited Python child. Actual measured targets were Python test processes, not Browser/Accelerator, TLS or HTTPS. Live cases record two real samples/configuration and expected closures; source image hash matches actual OS-observed Python path. PID reuse is represented by stale-lifetime input, not induced actual reuse.

Blocked query, native measurement failure and close-retained faults are SYNTHETIC_TESTED as diagnostic records, never claimed native OS failure injection. No native hung query was induced or forcibly terminated. Observer cannot block owner through a logical wait because it has no owner call/IPC dependency; actual product Stop under workload remains NOT_RUN. Host CPU/I/O/resource contention is not zero and must be included in campaign target headroom. No claim of universally bounded external query latency.

Initial failed offline evidence is preserved in native-Debug-1 and later numbered attempts: full file metadata comparison included mutable access-time and was corrected to stable identity plus hash. A rights-based failure oracle initially assumed QUERY_LIMITED would deny metrics on this host; actual native calls succeeded. The corrected test removes SYNCHRONIZE and expects conservative early absence. Exited-process image unavailability led to an explicit pre-query signaled-handle check. Final evidence uses native-{Debug,Release}-final and *-build-final.log. No warning suppression or toolchain substitution.

## ED11 and preservation
Runtime product managed delta0; no owner/storage/credit/quota change. Diagnostic observer has up to3 explicit Win32 handles: inherited process duplicate, image file and output file, plus temporary BCrypt algorithm/hash objects with checked destruction. Explicit hash scratch66560 bytes (65536 input+1024 object); record local8192 bytes; bounded main path/hex/digest/metadata values also coexist as appropriate. These are not a compiled whole-stack or total observer RAM census. BCrypt/CRT/OS internals remain additional diagnostic costs, not zero. No measurement history array or heap JSON store in product.
Offline Python verifier input bound8MiB, up to8200 rows plus parsing/library overhead; fixture runner/file hashing has its own host costs. It is not product quota and not a minimum-RAM claim. Real observer process footprint, observer-host overhead, sampling gaps and retention must be measured in the future campaign. Per-process values are not summed into one fictitious budget; working set/private/pagefile metrics overlap. Lifetime OS peaks are not phase peaks. Provider/kernel maxima and target quantitative parameters remain UNKNOWN/PENDING.

SDK Accelerator46/46 and Browser55/55 verified; historical binary archive hash336CA151941304952565DA538D5041DDFF59943405E0EDCE72B9A48D614062A1 unchanged. Pin hash3D347A0606C7AD03D2ED02B593DABA8853A2C627F85A146F28FB90C94BA05C39 unchanged. Root/producer hashes match prior reviews. No historical executable/map overwritten, no clean/clone/worktree/Git publication. Caller-created test outputs and failed attempts remain preserved.

CURRENT_TASK_COMPLETED=YES
HYBRID_OBSERVER=ADAPTED_EXISTING_DIAGNOSTIC_COMPONENT
PROCESS_IDENTITY_BINDING=HANDLE_PID_CREATION_OBSERVED_FILE_OFFLINE_PYTHON_ONLY
RESOURCE_SAMPLING=EXISTING_WIN32_QUERIES_EXTERNAL
OBSERVER_LIFECYCLE=FACTUAL_CLOSURES_OR_PENDING_RETAINED
STOP_INDEPENDENCE=NO_PRODUCT_WAIT_DEPENDENCY_STATIC_NATIVE_PRODUCT_STOP_NOT_RUN
PHASE_ASSOCIATION=INCOMPLETE
BUILDS=DEBUG_RELEASE_PASS
WARNINGS=0_FINAL
OFFLINE_TESTS=65_SYNTHETIC_PASS_8_NATIVE_DIAGNOSTIC_CASES_PASS
NETWORK_TESTS=NOT_RUN
ED11_TOTAL_STATUS=PARTIAL
PROVIDER_KERNEL=UNKNOWN
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=GOVERNED_PAIR_LAUNCH_AND_CAUSAL_PHASE_ASSOCIATION_NOT_EXECUTED
NEXT=RECONCILE_AND_APPROVE_EXISTING_MODE_C_A_H_CAMPAIGN