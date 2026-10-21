# Pair Stop delivery qualification — offline auxiliaries only
Task PACC-PHASE6-MODE-C-PAIR-STOP-DELIVERY-QUALIFICATION-1. Date2026-10-18.

## Scope and result
Implemented tests/diagnostic/ed11_total_probe/pair_stop_delivery_test.c as a fixed self-child diagnostic of existing launcher/controller delivery primitives. It links the UNCHANGED apps/server/server_console_win32.c used by Accelerator, not an emulated console Stop flag. This is not a new operational supervisor or a generic arbitrary-process signal utility. No Browser/server product, listener, HTTPS, network, trust installation, admission or accepted ADR change occurred.

Debug/Release MSVC19.51 x64 /TC /W4 /WX /MD builds passed without warnings. Final evidence is native-{Debug,Release}-final2 and *-build-final2.log/*-offline-final2.log. All final bootstrap/controller/helper exits were observed, and their recorded handle closures succeeded. No test failed to receive its requested signal. No TerminateProcess or retry-until-Stop is present.

## Factual delivery
Bootstrap starts only its exact self executable in CREATE_NEW_CONSOLE with SW_HIDE. The host console is never detached or attached. Controller creates its fixed console target and sentinel with CREATE_NEW_PROCESS_GROUP, sharing only the controller's dedicated console. This avoids CREATE_NO_WINDOW and the ignored process-group/new-console flag combination. It records actual created process handles/PID/creation and verifies QueryFullProcessImageName against its own canonical module path before delivery. Expected hashes alone are not runtime identity; the manifest pins final files and observed path checks are recorded as test evidence, not page/DLL attestation.

The controller emits CTRL_BREAK_EVENT to the exact still-identified target child group ID, never group0 or Ctrl-C broadcast. The existing console handler sets its normal Interlocked Stop flag; the helper reads that flag and emits STOP_RECEIVED_EXISTING_HANDLER. After target exit/receipt, sentinel is still live, controller's own existing handler flag is false and pending-observer fixture is still live. Sentinel later receives its own targeted Ctrl-Break for governed fixture cleanup. These tests demonstrate isolation among owned groups and the unchanged handler. They do not assert that the real server's drain/try_shutdown/native dependencies were qualified.

A separate hidden window helper uses class PapinhoPairStopDiagnostic, not a fake Browser-class window. Controller finds a unique top-level unowned window in that exact created process, checks process creation/image, HWND PID, primary thread TID and exact class immediately before asynchronous WM_CLOSE. Helper writes WM_CLOSE_RECEIVED and WM_DESTROY_RECEIVED using DefWindowProcW/normal message dispatch. Posting success alone is not the oracle: receipts plus helper exit are required. Wrong-process and stale-creation bindings are rejected before any message; equivalent process negatives precede console signal. No actual Browser HWND was observed or claimed.

A pending fixture waits on its own inherited release event throughout target console Stop and window closure; both complete before that event is released. This is SYNTHETIC_QUERY_PENDING, not a hung native Win32 API test or a second collector. It demonstrates no logical wait dependency in the diagnostic delivery controller. Existing real external observer is unchanged, still independent, with no product pipe dependency. Host scheduling/I/O contention and real product Stop progress remain unqualified.

## Ownership, retention and failure
Logs are CREATE_NEW ordinary files with HANDLE_FLAG_INHERIT cleared, not pipes read by the observer. Only fixture readiness/release event handles are deliberately inherited; release inheritance is disabled after creating its pending child. Ready handles, child process/thread handles and release event have individual closure records, no close retry. Image/module handles used for identity are borrowed, not released as owned leases.

On unconfirmed child exit, dispose_child records CHILD_RETAINED and keeps its process/thread references instead of executing that close path. WAIT_TIMEOUT/WAIT_FAILED versus WAIT_OBJECT_0 disposition is SYNTHETIC_TESTED. No actual native close failure, hang or PST retention was induced. Helper finite timeouts report failure/absence of Stop, never successful product teardown. Failed bootstrap wait or controller process exit is containment, not proof that retained resources were destroyed. This test is NOT the production policy for abandoning a real pair; future real-pair control must keep its durable references/retention reporting.

No product owner, operational credit or native/PST dependency is released by the diagnostic. The source still requires confirmed actual receipts for each product's quiescence and destruction. Signal delivery, callback receipt, process exit, handle CloseHandle and Model E DESTROYED remain separate facts.

## Resources and preservation
Operational managed delta0. Diagnostic controller explicitly holds up to8 child process/thread handles, one release event, one transient ready event and one direct log file (peak11 explicit handles), plus console/CRT/system objects not claimed bounded/zero. Bootstrap has2 child handles. Helpers have inherited event/log references; window fixture adds HWND/timer/class/message-queue resources. Main paths, command buffers and the4KiB log-read scratch are bounded in source, but whole-stack/runtime/CRT/kernel costs are not measured by this qualification. These are separate diagnostic-host costs and must be included in a future campaign's headroom; no product quota or provider/kernel bound changes.

SDK Accelerator46/46 and Browser55/55 verified. Product console source hash09751563EF0ADBF42EF7ECF059615C4D1F72AB59C78B7BA0ED5F31065967C7CD unchanged. Active producer/root and production pin hashes unchanged. Browser main/controller hashes checked unchanged during this task. No historical binaries/maps overwritten, no clone/worktree/clean/commit/push/release. Intermediate diagnostic logs/source snapshots are preserved; final evidence refers only to the final build, not claimed byte-exact earlier intermediate images.

## Product handoff / remaining gate
Existing Browser main class is PapinhoBrowserMainWindow (papinho_browser.c:32/9590), functional close goes WM_CLOSE:9118 -> DefWindowProc -> WM_DESTROY:9432 -> existing cancel/join/root shutdown. The actual Browser launcher currently runs a fixed hidden selftest. Browser Codex must bind its actual main HWND/creation/thread and preserve product controller receipts; see browser-handoff.md. Do not launch the Browser in this task, guess an HWND by title, import fixture identity or treat the auxiliary class as Browser proof.
Accelerator real server can reuse this proved isolated-console/group delivery pattern only after the exact pair launcher/configuration is bound and approved. Its actual handler-received/drain/teardown receipts remain separate future workload evidence. Administrative config/CURRENT/epoch/Principal/grants/Site trust inputs, quantitative ED11 parameters and phase association remain pending. Campaign revision2 remains non-executable and no laboratory exception is accepted here.

CURRENT_TASK_COMPLETED=YES
BROWSER_HWND_BINDING=AUXILIARY_MECHANISM_QUALIFIED_REAL_BROWSER_HANDOFF
BROWSER_STOP_DELIVERY=AUXILIARY_WM_CLOSE_DESTROY_PASS_PRODUCT_NOT_RUN
ACCELERATOR_CTRL_BREAK=ISOLATED_OWNED_GROUP_AUXILIARY_PASS
ACCELERATOR_STOP_RECEIPT=UNCHANGED_HANDLER_NATIVE_AUXILIARY_ONLY
OBSERVER_INDEPENDENCE=NATIVE_DELIVERY_WITH_SYNTHETIC_PENDING_OBSERVER
BUILDS=DEBUG_RELEASE_PASS
WARNINGS=0_FINAL
OFFLINE_TESTS=DEBUG_RELEASE_AUXILIARY_SCENARIOS_PASS_SYNTHETIC_RETENTION_GUARD_PASS
NETWORK_TESTS=NOT_RUN
ED11_TOTAL_STATUS=PARTIAL
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=REAL_BROWSER_HWND_AND_PAIR_LIFECYCLE_NOT_QUALIFIED
NEXT=RECONCILE_PAIR_STOP_AND_ADMINISTRATIVE_INPUTS