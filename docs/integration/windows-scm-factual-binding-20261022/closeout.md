# SCM factual binding — focused qualification, 2026-10-22

STOP. No functional source, accepted contract, account, credential, ACL or service was modified. Main HEAD 913c90cf2328f7939d8f5b927640601903ac6e91. This task does not close the factual binding.

## First missing producer

The accepted dedicated local standard account is an architectural selection class, not an identified/provisioned account. No account name, native installation target or configuration was supplied for this task. Requested these identifiers from the owner; no substitute was chosen. Do not enumerate unrelated accounts as a way to select an operational identity. Account provisioning remains separately authorized. Preinitialization P3/P4 and MVP M1/M5 require actual SAM SID and operational TokenUser, separate administrative context, protected private-key denial and factual isolation; a resolved name or derived service SID cannot satisfy them.

## Concrete existing code and boundaries

- server_service_data.c: ps_parse accepts service install --account .\name; lexical DATA only. No account resolution or authorization occurs.
- server_service_win32.c: ps_binding_ready returns false. Every mutating command denies before SCM acquisition. ps_mutate install returns ERROR_NOT_SUPPORTED; there is no CreateService implementation or protected credential channel. Do not claim otherwise.
- Existing query uses OpenSCManagerW/SC_MANAGER_CONNECT, OpenServiceW/SERVICE_QUERY_STATUS, QueryServiceStatusEx and checked CloseServiceHandle. Access denied is an error; missing service is a point-in-time observation only. Close failure retains exact references, prevents reuse and does not retry. At most two caller-owned query handles; opaque costs remain unknown.
- StartServiceW, ControlService, ChangeServiceConfigW and DeleteService branches exist behind the unqualified binding. No native positive mutation was executed or qualified. Delete requested is not proven absence.
- Existing personal native producer captures the administrative operator's actual TokenUser; it is not the future service account's token. Reusing that SID as the runtime SID would violate the separation.
- Installation binding lacks a protected service-install attempt holding target/config/executable native identity and immutable bytes/hash through the SCM effect. Existing personal parent/source owners are available patterns, not proof of an installed service. A printed path/hash alone is insufficient.
- No protected SCM credential producer is connected. Do not place a password in argv, .conf, logs or a default. Administrator privilege is not CA authority. No SYSTEM/LocalService fallback.

## Runtime and lifetime

ServiceMain reports START_PENDING, then denies before acquiring runtime/root/worker; STOPPED is valid for that empty resource set only. It never reports RUNNING. Handler only signals the existing Stop bridge. Status/registration failures retain containment without a factual STOPPED report.

main.c.api3.inc papacc_server_run installs console handling and calls papacc_application_require_operational_admission before network acquisition. That admission function returns INVALID_STATE. The console runtime supplies an integer exit, not a complete SCM readiness/quiescence receipt. Merely enabling ps_binding_ready would both leave the credential/identity gap and fail to provide readiness. Do not enable it. The future host binding must use the same runtime, report RUNNING only after all required owners/admission are ready and report STOPPED only after factual teardown; timeout and integer return are insufficient. Current tests cover DATA/negative/model behavior, not service runtime readiness.

## Verification and resources

Re-ran the existing documented MSVC /WX build recipes for Selected/Legacy Debug/Release: PASS, incremental no-work builds, zero warnings. Four targeted suites per profile passed (16 suite/configuration combinations); a second captured evidence run also passed 16. Existing tests cover DATA lifecycle, native mutation denial, modeled query/close/status failures and console Stop. Read-only actual service status observed ABSENT with exit 0. No new account/SID/executable-divergence binding tests are claimed because that binding is absent; existing status/model tests do not prove it.

No functional layout changed. Prior named SCM metadata remains 116 bytes, direct nominal subtotal Selected 1965236 / 3145728 and Legacy 1799740 / 2097152. Remainders 1180492 and 297412 are nominal only, not operational margins. Provider/RPC/kernel/CRT, service thread stacks, complete nested stack, host and retained operational resources remain UNKNOWN. ED11 remains PARTIAL; no capacity PASS.

Source/authority hashes, binary hash, build and test logs are in evidence.json and evidence/. Compared service/native/main hashes to the previous closeout; unchanged. Existing WIP was not reset, cleaned or rewritten; no broad external-project integrity claim is inferred from this local check. Four historical LAB directories were not modified by this task.

## Next bounded operation

Owner supplies the existing dedicated account and installation/configuration identifiers. Qualify supported read-only local account resolution/standard-account eligibility and native target/config/executable custody without treating either as authorization. Prepare a protected credential operation and actual operational-token verification, including distinct account/service SID provenance. Connect readiness and final resource receipts in the existing runtime only after applicable admission facts are available. A positive LAB CreateService/start/stop/remove command requires separate exact official approval and an isolated existing account; no such operation is authorized or prepared as runnable today.

CURRENT_TASK_COMPLETED=NO
SCM_FACTUAL_BINDING=STOP_MISSING_FACTUAL_PRODUCERS
OPERATIONAL_ACCOUNT=NOT_IDENTIFIED_NOT_CREATED
ACCOUNT_SID=NOT_PROVEN
SERVICE_SID=NOT_PROVEN_NOT_SUBSTITUTED_FOR_ACCOUNT_SID
INSTALLATION_IDENTITY=NOT_BOUND_TO_SCM_ATTEMPT
RUNTIME_READINESS=DENY_NO_OPERATIONAL_ADMISSION
STOP_QUIESCENCE=DATA_NEGATIVE_MODEL_PASS_POSITIVE_SERVICE_NOT_QUALIFIED
ED11_TOTAL_STATUS=PARTIAL
BUILDS=SELECTED_LEGACY_DEBUG_RELEASE_INCREMENTAL_PASS
WARNINGS=0
OFFLINE_TESTS=16_TARGETED_SUITE_CONFIGURATION_COMBINATIONS_PASS_PLUS_CAPTURED_RETEST
NATIVE_TESTS=READ_ONLY_STATUS_PASS_ABSENT_AND_EXISTING_NEGATIVE_SUITES
NETWORK_TESTS=NOT_RUN
PRODUCT_SERVICE_INSTALL=NOT_RUN
BOOTSTRAP_EXECUTION=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=NO_FUNCTIONAL_EDITS_NO_RESET_OR_CLEAN
FIRST_UNRESOLVED_DIVERGENCE=DEDICATED_ACCOUNT_AND_INSTALLATION_FACTUAL_IDENTIFIERS_NOT_SUPPLIED
NEXT=WINDOWS_SCM_NATIVE_LAB_LIFECYCLE_AFTER_FACTUAL_BINDING_AND_EXACT_APPROVAL
