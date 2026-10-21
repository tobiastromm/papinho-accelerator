# CNG first-failure instrumentation closeout — 2026-10-19

Task: PACC-PHASE6-CNG-EXACT-FAILURE-ORIGIN-INSTRUMENTATION-1.
HEAD main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Baseline captured 510 existing WIP entries; no reset/clean/checkout or historical output replacement. Exact preimages preserved in preimages/. All scoped writes/builds/tests submitted through executor approval; no denial.

## Delivered

Private backend header/source now carry an inline sticky first-failure receipt, exact function/stage/origin/API/raw code, original mapped backend result, preparation/finalization/held-resource and ownership snapshot. Native NCrypt returns remain unchanged. Existing Win32-to-backend mappings remain unchanged, with original Win32 error additionally exposed. No new permissions, relaxed guard, retry, fallback, secret logging, thread or authority. Wrong-thread access cannot write the receipt. First receipt survives subsequent cleanup failures.

See stage-matrix-and-next-run.md and stage-map.json for precise scope, omissions and future authorization requirements. The native-linked fixture prints the receipt but its native branch was NOT executed. The historical incident cannot be diagnosed retrospectively from injected failures.

## Validation

Final build-Debug/Release recipes and build6 logs: real MSVC x64 /W4 /WX, zero warnings, linkage complete. Each configuration: 598 synthetic assertions, 19 differential preimage scenarios, six argument negatives. Original/new functional returns, ownership projections and NCrypt call order match. Synthetic executable imports inspected: no ncrypt.dll/NCrypt APIs. Native-linked fixture executed without --native-key only. Individual local Win32/ACE failures are not all induced; no native permission failure was reproduced. Intermediate failed builds are retained and superseded only by final build6 evidence.

## ED11 and preservation

Compiled owner 408 -> 472 bytes: +64/owner, receipt 56 bytes. Two-owner fixture +128 bytes; retained owners must remain charged. Recorder allocates no dynamic storage. Full product/stack/provider census is not claimed; provider/kernel UNKNOWN. No quota, credit, admission, policy, epoch, PST, Browser, wire or production pin change. Earlier diagnostic executable/log and pin hashes were reverified unchanged. Live capability note links current factual scope without accepting new decisions.

```ini
CURRENT_TASK_COMPLETED=YES
CNG_FAILURE_INSTRUMENTATION=APPLIED_OFFLINE_VALIDATED
FIRST_FAILURE_STAGE=HISTORICAL_NOT_OBSERVED
FIRST_FAILURE_ORIGIN=HISTORICAL_UNDETERMINED
FIRST_FAILURE_NATIVE_API=HISTORICAL_UNDETERMINED
FIRST_FAILURE_CODE=HISTORICAL_0x80090010_ORIGIN_NOT_PROVEN
KEY_STATE=UNKNOWN
KEY_CREATED=NO
KEY_DELETED=NO
ACL_CHANGED=NO
BUILDS=DEBUG_RELEASE_FOCAL_PASS
WARNINGS=0_FINAL
OFFLINE_TESTS=598_ASSERTIONS_AND_6_ARGUMENT_NEGATIVES_PER_CONFIGURATION
NETWORK_TESTS=NOT_RUN
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
ED11_TOTAL_STATUS=PARTIAL
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_PREIMAGES_PRESERVED
FIRST_UNRESOLVED_DIVERGENCE=HISTORICAL_FAILURE_ORIGIN_REQUIRES_SEPARATELY_AUTHORIZED_NATIVE_RECEIPT
NEXT=OWNER_REVIEW_OF_FACTUAL_CNG_FAILURE_ORIGIN
```

No new native creation/open/deletion is authorized by this closeout. Existing --native-key success path deletes its NEW test key; future authorization must cover that exact path before execution. Never reuse/delete the prior UNKNOWN key implicitly.