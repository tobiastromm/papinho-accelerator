# Canonical pair execution safety — preparation only
Task PACC-PHASE6-MODE-C-CANONICAL-PAIR-EXECUTION-SAFETY-1. Date2026-10-18.
No executable pair runner or new operational supervisor is created. No product, listener, HTTPS, certificate operation or external measurement was executed. The existing campaign remains PROPOSED_NOT_EXECUTABLE.

## Verified baseline
Accelerator main HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a; Browser HEAD1eb028554f622672377953225e162ed9b6564a40. Both have WIP, not clean HEAD builds. Revision2 manifest SHA2565F59B333B8A1F8AC242FFBF5BFC24DF1A4BB10147EDF63D386A626C829918DAD and all referenced targets/evidence checked. Normal SDK files Accelerator46/46 and Browser55/55 verified.
Exact Release targets remain Accelerator AB3CC7C07457854690E51515DBD41FB623D87F4A815BB6B79D6DEC11E55DC9CA; Browser BAAE6B779D7452368C8A4DF87A6E9AB832410A1179F84478B78AE291EE179437; observer3429E2B60E3FFAE6640D956BA750B5F601E8FA692DB7E184485A2878F5053298. Future actual process/image observation remains mandatory. Python diagnostics do not qualify this pair.

## Existing launch control and missing integration
Reuse Browser tests/c2-s6-composition/pair_receipt.py native_existing_child: it holds the actual executable file against write/delete, creates a suspended process, captures OS process creation/image, resumes, preserves direct disk stdout/stderr, validates observed file/build/SDK references and records individual closures. Its current command is fixed to --candidate-controller-selftest, flags CREATE_NO_WINDOW|CREATE_SUSPENDED; its30s wait is an offline test bound, not campaign timing. It is NOT currently a pair/campaign launcher. Do not replace its command with real networking arguments or assume its finally wait/closure protocol solves pair Stop.

Prepared control sequence (not executed):
1. Close administrative/TLS/target/safety inputs and obtain separate exact invocation authorization. Recheck revision2 hashes and source/build linkage. All gates remain DENY until the applicable separate decisions; this preparation changes no gate.
2. Reuse held executable/file identity, process/thread handles and creation/image checks for each exact child. Create only the declared canonical products after approval, with explicit command/environment references, protected secrets, independent direct disk logs and a restricted inheritance list. Keep per-child factual state, not PID-only lookup. Do not silently create a new root/listener or synthesize administrative readiness.
3. Establish actual committed root/CURRENT/epoch, Secure identity/grants and Site profile through existing product paths before requesting the single logical operation. The canonical components/APIs exist; a declared launch command that actually drives the administrative transaction for this campaign has not been established. A selftest or fixture cannot substitute for it.
4. Give the existing external observer its own inherited duplicate of each held process object, minimum query/read/synchronize rights and independent CREATE_NEW log. No product waits for observer sample, exit, stdout or log parsing. Do not share kill-on-close jobs. Observer pending/failure is evidence incomplete and a reason for the independent existing Stop path, not a reason to kill the product. Diagnostic handles have their own closure/retention state.
5. Poll control state independently of measurement queries. Do not perform a synchronous observer wait in the same branch responsible for delivering product Stop. Keep owner functional results/credits separate from observation. No implementation of that coordinated branch is claimed here.
6. Deliver Stop as below; only then observe joins/quiescence and per-owner closure/disposition. Timeouts produce PENDING/RETAINED and original credit, never destruction. Do not retry native close, force termination or reuse uncertain handles/owners. Process exit and the observer's process-handle closure are separate from product Model E evidence.

## Stop proof and boundaries
|Product|Existing functional path|What source demonstrates|Missing proof for this campaign|
|---|---|---|---|
|Accelerator|server_console_win32.c handler accepts CTRL_C/CTRL_BREAK, atomically signals flag; server_run_root_win32.inc loop checks flag and cleanup calls request_stop, draining, is_stopped then try_shutdown|Console callback does not sample resources or wait for observer. Owner-thread cleanup retains uncertain dependencies|Delivery to exact process group/console, actual Stop progress under this workload, quiescence and closure results have not run|
|Browser|papinho_browser.c WM_CLOSE falls through DefWindowProc; WM_DESTROY begins shutdown, cancels/joins controllers and invokes existing PST root finalization|candidate_controller_stop signals its cancel event BEFORE resource observation, then waits/records join/CloseHandle once; retained outcomes preserved. No external observer dependency|Exact main-window identity, UI message delivery, cancel/progress and all-owner native disposition under real operation remain unproved|

Accelerator delivery candidate is targeted CTRL_BREAK to a separately controlled console process group in an approved launcher console context. Current CREATE_NO_WINDOW launcher cannot be treated as satisfying this. CREATE_NEW_PROCESS_GROUP alone does not prove console attachment; do not combine incompatible console flags, broadcast group0, detach the user's console or send events to unrelated processes. No console manipulation was attempted here. Delivery protocol needs a focal offline proof before any real pair launch.
Browser delivery candidate is asynchronous WM_CLOSE to the actual main HWND bound to the held process lifetime, not a title/PID guess or arbitrary options window. Revalidate PID/thread/window provenance before requesting it. Message enqueue success is not destruction, and a missing/hung UI cannot justify termination. No SendMessage wait may introduce observer-dependent backpressure. The existing launcher currently exports no verified main-HWND Stop binding.

Browser controller has a five-second join observation after its cancel signal; it is not proof of successful teardown or an ED11 freshness limit. Its internal post-signal resource query can have latency; worker cancellation was requested first, but finalization latency is NOT thereby bounded. External observers are not logically in either product's Stop path; host resource contention and actual progress remain unqualified. Thus PAIR_STOP_CONTROL is source-path identified, NOT proven for execution. No offline mock is offered as proof of GUI/console delivery that is not integrated.

## Administrative and TLS binding
|Binding|Required existing authority/fact|Current disposition|
|---|---|---|
|Config/root|Existing accepted root transaction: prepare, participants, durable commit identity/integrity|No selected campaign administrative input or launcher binding|
|CURRENT/epoch|Existing root policy transaction and actual postcommit issuance|Record when issued; never populate from idle/test epoch or expected value|
|Principal/grants|Secure peer certificate mapping to existing resolver; actual policy capability/operation grant|No selected authorized mapping or actual grant for campaign|
|Site TLS|Accepted server administrative catalog, CURRENT revision/ref/expiry and authorization|No selected profile/context for invocation; unknown remains DENY|
|Channel mTLS|Exact server/client public chains, key lease references, hostname and configured trust|No material selected or validated; no secret/env dump performed|
|Origin TLS|Separate SYSTEM trust, valid SAN hostname/SNI, TLS1.3/http1.1, exact existing loopback origin/config|No origin/port/certificate/response selected. No fixture CA created or installed|

Existing CLI help exposes a lab environment security source (certificate/private key/client CA and mapping inputs). Its existence is not administrative commit, a Site TLS catalog or approval to inject fixtures. Do not place private key contents in argv/logs/manifests. Use only already authorized protected credential references; exact material/trust validation must occur after selection and before separately approved effects. No certificate presence, SYSTEM trust acceptance, mTLS or TLS negotiation PASS is claimed by this review.

## Causal association and ED11
Retain existing root transaction, OPEN_RESULT/BIND/ACK/TLS_READY/prefix/fence/ACK/FINAL and local Stop/teardown/retention references. Actual request/op/generation/epoch is the protocol join; local Session IDs are not peer IDs, timestamps are not barriers. External samples are process-scope until an existing causal reference proves more. PHASE_ASSOCIATION=INCOMPLETE remains the consumer result. No new wire/schema/barrier is proposed.
Managed limits, credit/retention and observer diagnostic overhead remain as revision2. No source/storage delta in this preparation. Freshness/headroom remain PENDING; provider/kernel UNKNOWN; proposed laboratory exception is not accepted. Missing independent Stop delivery, identity/authority, mandatory observation or safe bounds blocks execution, not a reason to bypass admission.

## Result
CURRENT_TASK_COMPLETED=YES_PREPARATION_ONLY
PAIR_LAUNCH_CONTROL=EXISTING_COMPONENTS_MAPPED_REAL_PAIR_NOT_WIRED
PAIR_STOP_CONTROL=FUNCTIONAL_PATHS_IDENTIFIED_DELIVERY_NOT_PROVEN
OBSERVER_INDEPENDENCE=REQUIRED_ARCHITECTURE_NO_PRODUCT_WAIT_PATH
ADMINISTRATIVE_BINDING=AVAILABLE_AUTHORITIES_CAMPAIGN_VALUES_UNBOUND
TLS_INPUTS=REQUIRE_SELECTED_VALIDATED_MATERIAL
CAUSAL_PHASE_BINDING=INCOMPLETE
ED11_TARGET_PARAMETERS=PENDING
NORMATIVE_EXCEPTION=PROPOSED
EXECUTION_AUTHORIZED=NO
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
BUILDS=NOT_RUN
OFFLINE_TESTS=NOT_RUN_STOP_INTEGRATION_NOT_AVAILABLE
NETWORK_TESTS=NOT_RUN
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_DOCUMENTATION_ONLY
FIRST_UNRESOLVED_DIVERGENCE=EXACT_PAIR_STOP_DELIVERY_NOT_PROVEN
NEXT=OWNER_REVIEW_OF_SUPERVISED_PAIR_EXECUTION_SAFETY