# First native LAB readiness: access-denied diagnosis — 2026-10-23

Task: PACC-PHASE6-FIRST-NATIVE-LAB-READINESS-ACCESS-DENIED-1.
HEAD main: 913c90cf2328f7939d8f5b927640601903ac6e91.

## Finding and limits

The exact first failing API/predicate remains UNKNOWN. No correction was applied. Existing Win32 5 does not establish an ACL root cause: psr_open maps every negative NtCreateFile NTSTATUS to ERROR_ACCESS_DENIED, and other authorization predicates also return 5. Error 5023 is the qualifier's secondary response to unexpected STOPPED, not the original cause.

The source-backed path is ServiceMain / ps_main -> psr_prepare -> nonzero error -> checked host cleanup -> STOPPED with native.error. Shared runtime readiness is admitted only after successful prepare. The observed SCM result is STOPPED, PID 0, Win32 5, service-specific 0; no RUNNING was observed. This boundary inference does not identify the first failed operation or prove external qualifier cleanup.

## Candidate branches in unchanged source

| Source/function | API or predicate | Requested access / object | Interpretation |
|---|---|---|---|
| server_service_win32.c / ps_image_open | CreateFileW, file information/read, reparse/directory predicates | current module, GENERIC_READ 0x80000000 | Native error or explicit policy 5 possible |
| server_service_runtime_win32.inc / psr_prepare:121 | OpenSCManagerW / OpenServiceW | CONNECT 1; QUERY_CONFIG + QUERY_STATUS 5 | Native error 5 possible; not evidence SCM is the root cause |
| server_service_virtual_win32.inc / ps_virtual_readback_start | QueryServiceConfig, QueryServiceConfig2, LookupAccountName | service/account readback | Original native error can propagate |
| server_service_runtime_win32.inc / psr_token:31 | thread/process token, GetTokenInformation, DuplicateToken; identity predicates | process TOKEN_QUERY + TOKEN_DUPLICATE 0xA | Unexpected impersonation, token/type/SID/group predicate or API failure can yield 5; original service token unavailable |
| server_service_runtime_win32.inc / psr_open:65 | NtCreateFile | parent 0x001200a0; files 0x80120000; share READ; FILE_OPEN; no reparse | Any negative NTSTATUS becomes 5; original NTSTATUS lost |
| server_service_runtime_win32.inc / psr_current:94 | token statistics, identity stability | process TOKEN_QUERY 8 | Token mismatch/impersonation can yield 5 |
| server_service_runtime_win32.inc / psr_acl:75 | GetSecurityInfo, AccessCheck, SD/immutability predicates | OWNER/GROUP/DACL; READ_CONTROL 0x20000; MAXIMUM_ALLOWED 0x02000000 | Native failure or rejected effective rights/predicate can become 5 |

The fixed path/QueryDosDevice branches return 123; configuration/hash/file-identity validation and several token information failures return 13. They cannot independently explain the observed 5. Group SID validation can return 1337. See preserved preimages for exact compound branches. No original service TokenUser, groups, privileges, impersonation or effective AccessCheck result was captured; an administrative observation token is not a substitute.

## Read-only observations

See evidence/object-acl-observation.json: parent, config, original executable, two DLLs and installed LAB host have current static descriptors matching approved values. Hashes and recorded identities were preserved. Installed host identity 417614100:589824:2690484, SHA256 62087dbe13712c468dc92c5848de684cbc7cba6f921d15259f895fa8664d2716. Static conformance does not prove past effective service access.

SCM account NT SERVICE\PapinhoAcceleratorLab20261023 and service SID S-1-5-80-3567404667-26456103-3018599297-139793421-3962028451 remain readback evidence, not token evidence. Service DACL permits query rights to SU; actual service group membership was not captured. System events 7023/7040 corroborate access denied and DISABLED -> DEMAND. Security log query was denied by Windows privilege checks: unavailable, not an empty result. No audit policy or privileges were changed.

## Retention update

Original attempt 563826bacc49449f954b20cc48ce9b36 parked at RUNNING_READINESS with UNKNOWN live disposition. The owner now reports the qualifier window is no longer open. Read-only OpenProcess for historical PID 28452 returns Win32 87. No final cleanup receipt exists. No release, close, kill, Stop or retry was performed by this executor. Final disposition of original references remains unconfirmed; window closure is not evidence of safe release or rollback. Preserve intent/log and do not replay the consumed launcher.

## Qualification and ED11

Only offline proposal checks were run: model Debug/Release PASS; actual service translation-unit compilation Debug/Release /W4 /WX PASS, zero warnings. No full linked product build or native diagnostic execution is claimed. Trace disabled: 37 text sections and relocation targets match the original translation unit. Proposal recorder is 120 bytes, charged to proposed storage accounting. Stack, debug receiver and provider/kernel costs remain incompletely qualified. No quotas changed; ED11 PARTIAL. Existing failed-path/external retention UNKNOWN is not zero.

All 7919 baseline file fingerprints were unchanged before documentary updates. Source, protected artifacts, accepted decisions and historical LABs remain preserved. Only this report/evidence/proposal and additive documentary cross-references are authorized changes.

```text
CURRENT_TASK_COMPLETED=NO
FIRST_DENIED_OPERATION=UNDETERMINED_EXISTING_EVIDENCE
DENIED_OBJECT=UNDETERMINED
REQUESTED_ACCESS=UNKNOWN_FOR_FAILED_CALL_CANDIDATES_DOCUMENTED
EFFECTIVE_TOKEN=ORIGINAL_SERVICE_TOKEN_NOT_CAPTURED
ACL_CONFORMANCE=CURRENT_STATIC_SIX_OBJECTS_MATCH_APPROVED_EFFECTIVE_ACCESS_UNKNOWN
WIN32_ERROR=5
SECONDARY_ERROR=5023
ROOT_CAUSE=UNKNOWN_NATIVE_STATUS_OR_PREDICATE_NOT_RECORDED
MINIMUM_CORRECTION=NOT_DETERMINED_NOT_APPLIED
INSTRUMENTATION=PROPOSED_OFFLINE_CHECKED_NOT_APPLIED
START_RETRY=NO
ACL_MUTATIONS=NONE
SCM_MUTATIONS=NONE
RETAINED_RESOURCES=NO_RELEASE_BY_EXECUTOR_FINAL_DISPOSITION_UNCONFIRMED_AFTER_REPORTED_WINDOW_CLOSURE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_BASELINE_HASHES_CHECKED_DOCUMENTARY_ADDITIONS_ONLY
FIRST_UNRESOLVED_DIVERGENCE=EXACT_ORIGINAL_DENIAL_UNOBSERVABLE
NEXT=FIRST_NATIVE_LAB_READINESS_FOCUSED_CORRECTION
NEXT_GATE=EXACT_REVIEW_OF_INSTRUMENTATION_AND_ONE_DISCRIMINATING_CAPTURE
```
