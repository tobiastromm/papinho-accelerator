# Windows virtual service LAB host and runtime qualification

TASK=PACC-PHASE6-WINDOWS-VIRTUAL-SERVICE-LAB-RUNTIME-1, 2026-10-23. Active main, HEAD913c90cf2328f7939d8f5b927640601903ac6e91. Existing owner LAB registration evidence and accepted virtual-identity V0-V8, optional SCM, MVP, CA3.3, Model E/ED08 and both ED11 profiles govern this slice. No accepted ADR changed. Applicable governance/context from ongoing work was refreshed; future jobs/backup directions were examined and grant no runtime authority. Exact-scope-and-stop.md was officially approved before dependent changes. Source/preimage manifests and focused follow-up approvals are preserved.

## Independent components completed

A private BUILD_TESTING-only papacc_server_lab_host target compiles the COMPLETE canonical executable source list plus the same server_service_win32.c with the already existing fixed LAB profile. It reuses the exact main/config frontends/runtime/launcher libraries. No second implementation/service/loop/worker/root or dynamic identity selector. Four link maps prove direct LAB service symbols and shared server_run_win32 object; default static service implementation is not extracted. Product name/profile remains unchanged.

LAB-only ServiceMain rejects missing/divergent callback name and extra service-start arguments before acquiring host state. Exact name matches only identity shape, never authorizes runtime. Canonical service host argv remains exact service host; arbitrary name/extra arguments cannot choose an identity. The old registered image was not overwritten or reconfigured.

New host product/LAB models execute SAME dispatcher/ServiceMain/handler/status/receipt code with isolated native substitutions. Cases cover fixed table/name, dispatcher connection failure, extra CLI and service-start args, handler registration/status failure, startup DENY, duplicate invocation, wrong thread/no active callback, Stop-before-ready/during-readiness, quiescence receipts, missing terminal receipt despite integer0, retained status and terminal non-overwrite. Model callback setup is explicitly synthetic test state and never opens ps_binding_ready or starts a service.

The new test exposed a real second-check inconsistency: shared Stop delivered during SetServiceStatus(RUNNING) was not reconsulted after the call. Product and LAB models both failed line74 before correction (stop-race-before.json). Officially approved one-condition fix now checks BOTH host.stop and existing shared Stop bridge after the report, as the precheck already does. Same statuses/lifetime, no admission change. Both models/regressions pass. This is the only shared product behavioral change; the original scope's product-unchanged wording describes the initial planned name specialization, not this separately reviewed failure correction.

## Builds and qualification evidence

Selected/Legacy Debug/Release, same MSVC19.51 x64 Ninja and documented candidate SDK, /W4 /WX: final builds all PASS, warnings0. Fresh isolated build/windows-virtual-service-lab-runtime-20261023 trees, original LABs not replaced. Initial failures preserved: test globals shadowed adapter locals (/WX); renamed model identifiers without suppression. LAB target initially omitted existing config CLI source (LNK2019); corrected by reusing complete canonical source list. One direct incremental invocation lacked vcvars environment (windows.h unavailable); rerun via documented vcvars64 script, no toolchain change. These initial failures are not final PASS evidence.

11 component suites x4 configurations =44 PASS: two host models, five existing service DATA/native/query/virtual/ACL models, console, effect-plan DATA, memory census and issuer-denial. 18 CLI cases/configuration =72 PASS across product and LAB host: help, host outside SCM, extra host args, install/start/stop/startup/remove denied, direct startup denied at operational admission before network acquisition. No StartService was called; service start CLI cases stop at the unchanged mutation gate. DATA image/config/operator/install divergence/replay tests are model evidence, not positive native token facts.

Preflight Check and Plan return mandatory2 with fixed candidate/original/config hashes verified; unsupported Start mode rejects1. No script route launches binary, UAC or SCM effects. Old parent/config/image were reopened read-only with held handles, matching native IDs/hashes/no-reparse and three checked successful closes (original-protected-pins.json). Final SCM qc/queryex/qsidtype are read-only, preserved in scm-final-readonly.json: original command/account unchanged, DISABLED/STOPPED/PID0, UNRESTRICTED. Exit1077 indicates not started, not failed registration. No current-state claims are inferred from file hashes alone.

## STOP and remaining first-start gates

LAB name binding is component/model-qualified. Positive ServiceMain/runtime/token/ACL isolation are NOT qualified. ps_binding_ready()==0 is unchanged; no protected operational installation/config/token producer exists. Host callback argc1 does not supply configuration; application operational admission stays closed. Existing actual readiness follows network preparation, incompatible with claiming listener-disabled positive readiness merely from a running host. No guard removed and no separate idle loop introduced.

[Exact manual-cycle plan](manual-cycle-plan-STOP.md) identifies the missing producer, real token/access/CA-denial proof, resource admission and separately approved image/startup rebind. Current preflight is deliberately unarmed; a runnable Start/Stop launcher is not ready. This task stops at the user's executable/readiness/token conditions while preserving completed independent work. No request to approve an underspecified native effect is made.

## ED11

Measured named service storage remains native12816 + host96 + cli atomic4 =12916 bytes; zero added mutable service storage from the two conditions. No new managed pool/quota or worker thread. Existing PE headers/sections for each product/LAB/configuration and binary hashes are in artifact-and-pe-census.json. LAB Selected Debug image2990080, Release1847296; Legacy Debug2674688, Release1679360 bytes. These include code/static image categories and are NOT controlled managed allocations or total process peaks. PE main-thread reserve1048576 is address-space reservation, not full committed stack or SCM-thread stack measurement. Do not add named globals again to PE data.

Native SCM/dispatcher thread topology and stack, token/group/privilege/access-check resources, CRT/loader, full transitive stack, provider/kernel/LSA, retained coexistence and host margins remain UNKNOWN. No real service-process peak was measured because no start occurred. Prior persistent disabled LAB SCM object remains retained even though registration caller handles closed. Model parking records retention without real Sleep(INFINITE). Unknown is not zero. Legacy2097152/explicit Selected3145728 unchanged; total PARTIAL, operational admission DENY.

## Result

CURRENT_TASK_COMPLETED=NO
LAB_SERVICE_HOST=INDEPENDENT_BUILD_AND_NEGATIVES_PASS_POSITIVE_RUNTIME_STOP
LAB_SERVICE_NAME_BINDING=FIXED_LAB_DISPATCHER_AND_SERVICEMAIN_MODEL_PASS
EXECUTABLE_IDENTITY=NEW_CANDIDATE_HASHED_NOT_REGISTERED_ORIGINAL_PRESERVED
SERVICE_MAIN=MODEL_DENIAL_LIFETIME_PASS_NATIVE_POSITIVE_NOT_RUN
SERVICE_CONTROL_HANDLER=MODEL_PASS
RUNTIME_READINESS=STOP_PROTECTED_CONFIG_TOKEN_AND_LISTENER_DISABLED_COMPOSITION_ABSENT
STOP_QUIESCENCE=MODEL_PASS_NATIVE_NOT_RUN
TOKEN_QUALIFICATION=NOT_RUN
ACL_ISOLATION=ORIGINAL_ADMIN_PROTECTION_PRESERVED_SERVICE_TOKEN_NOT_QUALIFIED
ED11_TOTAL_STATUS=PARTIAL
BUILDS=SELECTED_LEGACY_DEBUG_RELEASE_PASS
WARNINGS=0_FINAL_INITIAL_FAILURES_PRESERVED
OFFLINE_TESTS=44_SUITE_CONFIGURATION_COMBINATIONS_PASS
NATIVE_TESTS=72_CLI_CASES_AND_ORIGINAL_READONLY_PINS_PASS_POSITIVE_SERVICE_NOT_RUN
NETWORK_TESTS=NOT_RUN
LAB_SERVICE_START=NOT_RUN
PRODUCT_SERVICE_INSTALL=NOT_RUN
BOOTSTRAP_EXECUTION=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=7692_BASELINE_FILES_CHECKED_NO_UNEXPECTED_CHANGE_EXACT_TASK_HUNKS_PRESERVED
FIRST_UNRESOLVED_DIVERGENCE=PROTECTED_SERVICE_RUNTIME_CONFIGURATION_TOKEN_PRODUCER_ABSENT
NEXT=WINDOWS_VIRTUAL_SERVICE_NATIVE_LAB_START_STOP_AFTER_PRODUCER_RUNTIME_TOKEN_AND_ADMISSION_GATES

No SCM/ACL/account/key/certificate/bootstrap/root/CURRENT mutation, no production install, no clock change/network, clone/worktree/commit/push/tag/release. Historical LABs preserved. Registration-only completion remains valid; it does not become running-token evidence.
