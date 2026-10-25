# Protected service runtime: implementation and offline qualification

2026-10-23. TASK=PACC-PHASE6-WINDOWS-SERVICE-PROTECTED-RUNTIME-1. Branch main; HEAD913c90cf2328f7939d8f5b927640601903ac6e91. Scoped implementation/offline qualification completed after exact official executor patch reviews. No first Start was requested or executed. Existing productive gates remain closed.

The private fixed-name LAB profile now obtains protected configuration and the actual operational primary token through the existing SCM owner. It retains parent/config/image handles, native identities, exact bytes/hash and ownership through the shared runtime's factual cleanup. Relative native opens reject unexpected reparses; checked closes and uncertain outcomes retain resources. Readback of configured virtual identity is distinguished from actual TokenUser/groups/privileges/impersonation and effective AccessCheck. The access-check token is duplicated from the same primary operational token; an administrative token is never substituted. Module-path file identity is observed; this is not attestation of mapped executable pages.

The existing root, managers, maintenance loop, Stop and teardown are reused. Closed qualification initializes actual empty managers without network initialization, sockets, listener, client admission, CA or CURRENT. Readiness checks owner thread, protected source/token stability, initialized closed managers and Stop before reporting RUNNING. A clean terminal receipt follows real teardown; timeout is never quiescence. Product admission is unchanged. Model fixtures do not enter the native executable's authority path.

Selected/Legacy Debug/Release final builds PASS with /W4 /WX and zero warnings. Four configurations each pass14 suites:56 suite/configuration combinations. Additionally72 CLI cases pass. Tests distinguish OS-substituted source/token/SCM models, actual native token denial and actual closed-manager components. Modeled Stop before/during/after readiness, divergent token/config/object, unsafe rights, wrong thread without token query, retained cleanup and uncertain outcomes reject appropriately. These results do not prove a running service token, effective CA isolation or system-wide socket telemetry.

The original LAB parent/config/image were queried through held handles: IDs and hashes match, DACL protection remains set, checked frees/closes succeed. No ACL mutation. Read-only SCM observation shows the existing LAB service DISABLED/STOPPED/PID0 with its original image and matching virtual account/SID type. The candidate was NOT copied into the protected installation or registered. Its future required protected filename is papacc_server_lab_host.exe. The current preflight accepts Check/Plan only, validates pins and exits2 deliberately; Start is rejected. No armed Start command is supplied.

[Implementation, resource limits and future handoff](implementation-proof-and-handoff.md). [Evidence index](evidence/index.md). [Owner listener/live-configuration integration map](../personal-listener-live-config-direction-20261023/integration-map-and-next-slice.md).

```text
CURRENT_TASK_COMPLETED=YES
PROTECTED_RUNTIME_CONFIGURATION=IMPLEMENTED_LAB_OFFLINE_PASS_NATIVE_SERVICE_PENDING
SERVICE_TOKEN_PRODUCER=IMPLEMENTED_REAL_QUERY_NATIVE_NEGATIVE_PASS_POSITIVE_FIRST_START_PENDING
LISTENER_DISABLED_READINESS=SHARED_MANAGERS_MODEL_AND_NATIVE_COMPONENT_PASS
SHARED_RUNTIME=SAME_ROOT_LOOP_AND_TEARDOWN
SERVICE_MAIN=LAB_PRODUCER_INTEGRATED_NATIVE_START_NOT_RUN
STOP_QUIESCENCE=MODEL_AND_CLOSED_COMPONENT_PASS_NATIVE_SERVICE_PENDING
ED11_TOTAL_STATUS=PARTIAL
BUILDS=SELECTED_LEGACY_DEBUG_RELEASE_PASS
WARNINGS=0_FINAL
OFFLINE_TESTS=56_SUITE_CONFIGURATION_COMBINATIONS_PASS
NATIVE_TESTS=72_CLI_CASES_PASS_REAL_TOKEN_DENIAL_AND_CLOSED_COMPONENTS_PASS
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
WIP_PRESERVED=YES_BASELINE7746_HASH_CHECKED_EXCEPT_EXACT_APPROVED_CHANGES
FIRST_UNRESOLVED_DIVERGENCE=PROTECTED_CANDIDATE_DEPLOYMENT_AND_FIRST_NATIVE_SERVICE_TOKEN_QUALIFICATION_PENDING
NEXT=WINDOWS_VIRTUAL_SERVICE_NATIVE_LAB_START_STOP
```
