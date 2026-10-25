# CA LAB finalization launcher completion — 2026-10-22

Task PACC-PHASE6-CA-LAB-FINALIZATION-LAUNCHER-COMPLETION-1. Scoped implementation/negative qualification complete. Positive native creation/finalization is NOT RUN. Branch main, HEAD913c90cf2328f7939d8f5b927640601903ac6e91.

## Implemented scope

Accepted G1–G5 is implemented only in the existing LAB launcher and standalone executor. Fixed new identity remains PapinhoAccelerator.CA.2345e07adf714045a20d1c8b6fdf5aaf.a2bcd6d2ed3043d083e7e50342d6fab0, Microsoft Software KSP/MACHINE. Previous name is untouched and UNKNOWN. No new identifier, pre-query absence oracle, overwrite, retry, deletion or fallback. No complete ceremony atomicity is claimed.

The canonical production server_ca_key_win32.c/header remain byte-exact. backend-lab.c instruments their existing translation unit ONLY in this LAB build: no code copy/fork, native arithmetic, new owner or resource manager. Wrappers validate the exact algorithm/name/spec0/MACHINE flags and check Stop before Create, every property and Finalize. Same native APIs/sequence are used. Each Create/LENGTH/EXPORT_POLICY/USAGE/DACL/Finalize status is printed and flushed separately, preserving native last-error. All failures, including NTE_EXISTS, return through existing failure/retention handling; no Finalize after failed Create or setter. Native first-error snapshots remain in the existing owner.

The executor no longer imports or calls NCryptDeleteKey. On successful creation/finalization/public validation and borrow/return checks, it closes only the handles through the existing owner. The LAB key remains persisted. Confirmed close is not deletion. Failure or Stop keeps persisted disposition UNKNOWN and reports retained owner fields/borrows, never reopens/deletes/retries. Process exit is not destruction of a persisted object. No protected production CA reference, signature or certificate is created. The expected local close-while-borrowed denial can populate sticky first-error after initialization; per-phase initialization output distinguishes it from a native Finalize error.

Launcher keeps native RunAs/manual-only UAC, real console, expected effective operator SID, enabled Administrators/elevation/no impersonation and canonical/reparse/hash checks. It presents exact key/provider/MACHINE/properties, persisted-key/no-delete consequences and a fresh 32-byte CSPRNG attempt identifier. User must type the entire attempt-specific visible confirmation. Esc/incorrect/empty confirmation stops before consumption; no passwords are requested/logged. A second token/context/hash check precedes CreateNew receipt consumption and exactly one fixed child invocation. The SAME existing started/result receipt paths block reexecution or reuse of earlier interrupted observations. No receipt is reset or deleted. Existing receipt means STOP, even if the old operation was read-only.

Output capture is bounded to128 lines of at most1024 characters for evidence; overflow makes result UNKNOWN, without terminating/releasing a pending child as successful. Child exit, ordered six native phase successes, public/borrow aggregate, factual final owner close and explicit persisted-key marker are all required for LAB_PASS. Wrapper exit0 alone cannot override a native error. Failures/interruption preserve the consumed receipt; no automatic retry. Ctrl-C/native interruption may leave only the started receipt, which remains UNKNOWN and blocked. Stop does not guarantee native cancellation.

## Qualification

Final MSVC19.51 x64 Debug (/Od /Zi /MDd /RTC1) and Release (/O2 /MD), /TC /W4 /WX: executor and mock negative tests compile without warnings. Initial C4005 from header ordering was corrected by matching _WIN32_WINNT0x0600 before Windows headers in LAB instrumentation/test; no warning was disabled. Existing product backend was not changed. Direct compile recipes are archived; this does not claim VC6/NT4/box/ISA or a full product rebuild.

C negative tests compile native Create/set/finalize calls out and invoke the SAME guards with injected statuses. They cover name mismatch, overwrite rejection before provider call, NTE_EXISTS, NTE_PERM, invalid Finalize flags, failed property/finalization and Stop preventing calls. They are models, not actual KSP collision or custody proofs. Model executable does not acquire a KSP. Production LAB executable safe describe/invalid/context-only branches were tested; current noninteractive context was denied before key calls.

PowerShell syntax/Check/Test pass. Models exercise wrong operator, elevation/role/console/query/impersonation failures, consumed attempt, empty/wrong confirmation, collision, Stop/error outputs, malformed/empty/duplicate/overflow results and uncertain close despite exit0. A complete synthetic output baseline validates parser reachability, not a cryptographic fixture or authority. Missing script pin and the actual ineligible/noninteractive token are denied. Native UAC cancellation, actual interactive confirmation, real collision and positive Finalize remain NOT EXECUTED; no fabricated qualification.

Binary imports inspected: no NCryptDeleteKey or NCryptSignHash. Native Create is present only in the effectful LAB binary. Four historical LABs and old binaries are preserved in their original directories; new builds use build/ca-lab-finalization-launcher-20261022 only. See hashes, tests, imports and headers.

## ED11

No native provider/key resource was acquired in these automatic tests. New LAB build, not product layout. Existing owner480 bytes x64 plus Stop4 and holder4, actor/SID/token/public-property/ACL scratch and wrapper call frames remain charged. Release PE reports code0x4400 and initialized data0x2a00; executable28672 bytes. These are file/image properties, not total live memory or peak. PE stack and heap reservation each0x100000, initial commit each0x1000; reservation is not a proven call-stack or heap bound. No default linker/resource quota was increased.

Launcher holds script+one binary read streams, managed attempt/plan/confirmation, bounded128x1024-character captured output plus CLR/PowerShell/token/console/receipt/runtime overhead. Own code performs no new cryptographic heap allocation, but CRT/provider/SDK transitive allocations are not assumed absent. New persisted LAB key remains a retained namespace resource on success; previous UNKNOWN object is independently charged. Provider/kernel/isolation memory, full nested stack, CRT/PowerShell/runtime, host margin and complete coexistence remain UNKNOWN. ED11 PARTIAL; no operational cap/margin PASS.

## Preservation and next gate

Exact functional patches were submitted to official write review before application. No rejection was bypassed. Postimages, source/launcher/executable pins and existing WIP preimages are checked. Accepted ADRs, production sources, PST, Browser, wire, quotas, production0.6.4 and four historical LABs untouched. No clone/worktree/commit/push/tag/release/network/bootstrap/certificate/CURRENT. Documentation of earlier read-only gates remains historical; this closeout describes their scoped successor implementation.

CURRENT_TASK_COMPLETED=YES
G1_G5_IMPLEMENTATION=LAB_ONLY_IMPLEMENTED
LAB_LAUNCHER=PREPARED_FOR_MANUAL_OWNER_EXECUTION
NONOVERWRITE_GUARD=EXACT_MACHINE_FLAGS_NTE_EXISTS_STOP_NO_DELETE
MANUAL_COMMAND=SEE_manual-command.md
POSITIVE_EXECUTION=NOT_RUN
BUILDS=MSVC_X64_DEBUG_RELEASE_PASS
WARNINGS=0_FINAL_BUILDS
NEGATIVE_TESTS=PASS_MODELS_AND_SAFE_LOCAL_BRANCHES
NETWORK_TESTS=NOT_RUN
PREVIOUS_LAB_KEY_DISPOSITION=UNKNOWN
CA_CUSTODY_INITIALIZATION=DENY_UNTIL_POSITIVE_QUALIFICATION
BOOTSTRAP_EXECUTION=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=MANUAL_ELEVATED_LAB_FINALIZATION_NOT_EXECUTED
NEXT=OWNER_EXECUTES_CA_LAB_ELEVATED_FINALIZATION


## Subsequent manual owner result

The owner has now executed the pinned manual LAB command successfully: Create/properties/Finalize0, expected borrow-guard denial, factual handle closure, persisted LAB key NOT deleted, native/parent exits0. Earlier NOT_RUN statements above are the pre-manual implementation snapshot. See [verified manual evidence](manual-elevated-result-20261022.md). No productive custody/authority or ED11 PASS follows. NEXT=PERSONAL_FIRST_INSTALL_CA_CUSTODY_BINDING.
