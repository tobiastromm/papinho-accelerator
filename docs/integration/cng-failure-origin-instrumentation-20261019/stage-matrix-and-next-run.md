# Failure origin stage matrix and next native-run gate

Task PACC-PHASE6-CNG-EXACT-FAILURE-ORIGIN-INSTRUMENTATION-1. Date2026-10-19.
Instrumentation only; historical NTE_PERM origin is NOT determined by synthetic evidence. Existing diagnostic key state remains UNKNOWN and has not been queried/created/deleted in this task.

## Receipt contract

PAPACC_CA_KEY.first_failure is a56byte inline sticky value, no logger/allocator/callback. First failure recorded only for caller-owned zeroed storage or actual owning thread; wrong-thread/null-owner calls return the unchanged error without writing/fabricating a receipt. No reset API. A new empty owner is required for a new diagnostic lifetime; never zero/reinitialize retained/live owner.

Fields: present, function, stage, origin, native_api, original_code, backend_code, win32_error, state, created, finalization, key_owned, provider_owned, ownership. No key bytes, passwords, SPKI, SID/token, DACL text or native handle values. original_code is untouched NCrypt status for NATIVE_NCRYPT; for failed Win32 calls it is original GetLastError and backend_code remains the existing mapped result. Other local guards record their existing result. Win32/mapped failures use LOCAL_GUARD plus specific Win32 stage and raw win32_error, never masquerade as NCrypt. There is no new normalization/authorization decision.

Snapshot is taken at first failure, before subsequent cleanup when applicable. finalization0=not observed,1=attempted/unconfirmed,2=FinalizeKey returned success. A successful open of an existing key does not fabricate a FinalizeKey receipt. Ownership0=no held native resources,1=owner holds handles,2=retained,3=closed; this is a private backend snapshot, NOT PST DESTROYED or persisted-key deletion. Compare final owner state/close receipts separately after cleanup. created means CreatePersistedKey returned success, not final persisted key existence. The sticky first receipt survives later validation/cleanup failures; original functional error/cleanup/retention behavior remains intact.

## Stage mapping

Exact numeric enum mapping is in server_ca_key_win32.h and stage-map.json; do not infer by error code alone.

| Backend function / stage family | Origin / API | Evidence this task |
|---|---|---|
| begin create/open ENTRY; identity SID_VALID/IDENTITY_EQUAL | LOCAL_GUARD/NONE | Invalid identity and null/wrong-thread guards; six historical argument negatives |
| identity THREAD_TOKEN/PROCESS_TOKEN/TOKEN_INFO/TOKEN_CLOSE | LOCAL_GUARD, raw Win32 error plus original mapped return | Receipt helper tested with simulated ERROR_ACCESS_DENIED; real Win32 failure not induced. Read-only token queries used by doubles harness |
| begin OPEN_PROVIDER/CREATE_KEY/OPEN_KEY | NATIVE_NCRYPT, respective exact API | All three synthetic native failures; no native key API called |
| begin SET_LENGTH/SET_EXPORT/SET_USAGE/SET_DACL | NATIVE_NCRYPT/NCryptSetProperty | Each synthetic failure distinguished; native parameters/order unchanged |
| begin SID_SIZE/SID_COPY/SID_TEXT/SD_FORMAT/SD_PARSE/SD_SIZE | LOCAL_GUARD; Win32 raw where applicable | Instrumented; not all individual local fault paths induced. Same conditions/short-circuit order retained |
| begin FINALIZE | NATIVE_NCRYPT/NCryptFinalizeKey | Synthetic error records finalization1; subsequent synthetic validation failures record2 |
| property GET_LENGTH/GET_EXPORT/GET_USAGE | NATIVE_NCRYPT/NCryptGetProperty | Each failure injected; raw non-NTE_PERM propagation also tested |
| property LENGTH_VALUE/EXPORT_VALUE/USAGE_VALUE | LOCAL_GUARD/NONE | Synthetic length mismatch; export/usage mismatches instrumented, not separately injected |
| validate EXPORT_PUBLIC/PUBLIC_VALUE | NATIVE_NCRYPT/NCryptExportKey(public) versus LOCAL_GUARD | Synthetic native failure and exponent mismatch distinguished; no native export |
| ACL GET_DACL | NATIVE_NCRYPT/NCryptGetProperty | Synthetic native failure |
| ACL SD_VALID/ACE_READ/ACE_FLAGS/ACE_TYPE/ACE_SID/ACE_ALLOWED/ACE_MASK | LOCAL_GUARD/NONE | Synthetic literal-mask rejection; individual malformed ACE classes not all exercised. No guard relaxed or effective-rights reinterpretation |
| close FREE_KEY/FREE_PROVIDER | NATIVE_NCRYPT/NCryptFreeObject | First-close failures and later cleanup failure preservation tested; retained refs/disposition unchanged |
| close ENTRY/ALREADY_RETAINED/RETAINED_TOKEN, begin LOCAL_FREE_* | LOCAL_GUARD/NONE | Retained/closed/wrong-thread tested; actual LocalFree/CloseHandle failures not induced |

CA_D_PROPERTY_VALUE is a reserved unused enum marker; precise LENGTH_VALUE/EXPORT_VALUE/USAGE_VALUE are emitted instead. Function/API maps separate numeric namespaces. Native returns are not evidence of actual CNG failure in these synthetic tests.

## Neutrality and evidence boundary

Same original guard predicates, SID checks, DACL string/masks/flags, KSP, key name, algorithm, export/usage values, call arguments, native operation and cleanup order. Changes are failure recording, a diagnostic-only finalization marker, split equivalent SD guard for exact diagnosis, and additional fixture printf lines. LastError preserved by receipt helper. Diagnostic fields are not consulted by operational guards or cleanup. No new thread/lock/authority/fallback or retry.

The test bridge compiles exact preimage .c/.h against the SAME synthetic NCrypt functions.19 cases compare functional return, state/created/close/error/held-key/provider/token projections and full NCrypt stage trace. All match. Additional oracles verify exact failure function/API/stage/raw code, first-error immutability across later cleanup, wrong-thread no-write, cleanup retention and LastError preservation.598 assertions per Debug/Release plus6/6 argument negatives per configuration. Assertions are not598 independent native runtime proofs. Final executables built with real MSVC x64 /W4 /WX, no final warnings.

Synthetic executable imports inspected by dumpbin: no NCrypt functions or ncrypt.dll in either configuration, including baseline bridge. Native-linked fixture is built but executed ONLY without --native-key, so its native branch is not run. No repeated creation/inventory/deletion or ACL/security change. Earlier key UNKNOWN is unchanged; no new receipt retroactively diagnoses it.

## ED11

Compiled owner472bytes versus prior408: delta64 per owner (56byte receipt,4byte diagnostic marker plus layout padding). Original fixture holds2 owners: additional128bytes. No dynamic allocation in recorder; helper stack/frame cost is not claimed as compiled full stack census. New source adds no root/product instance because backend remains a private focal component not integrated into server startup. Future N coexistent owners require64*N added managed reservation, including retained owners; no quota increase or process/provider total PASS. Provider/kernel UNKNOWN.

## Next native execution — procedure proposal, NOT authorized now

1. Owner reviews exact instrumented source/hunks, hashes, function/stage/API maps and no-behavior-change evidence. Obtain separate explicit authorization for any native create/open/cleanup test; this task grants none.
2. Keep old exact diagnostic name/authority/generation untouched and KEY_STATE_UNKNOWN. Never reuse it. Any newly authorized test uses fresh CSPRNG authority/generation IDs in an explicitly isolated diagnostic lifetime; creation is not automatic after this closeout.
3. Record executable hash and actual process creation/image/token/SID/integrity without secrets before the authorized run. No changed token, broader ACL, different KSP/user scope, exportable fallback or new retry policy to force PASS.
4. Run at most the separately authorized operation and capture first_failure fields plus actual return/current owner state. Decode through stage-map.json/header. NATIVE_NCRYPT identifies API+original status; LOCAL_GUARD identifies rejection/Win32 mapping. No receipt implies NOT_OBSERVED, not success.
5. Stop on first failure and preserve native/cleanup/persistent-key uncertainty. Handle close does not prove key absence; no automatic retry. A matching raw code alone is not sufficient to equate new failure with the previous historical cause.
6. Existing --native-key fixture contains its original checked deletion of the NEW fixture key on complete success. DO NOT execute that branch under a create-only/open-only authorization. Future owner approval must explicitly cover that exact retirement path, or authorize a separately reviewed diagnostic-only flow that stops before deletion. No deletion of the prior UNKNOWN key is implied. No execution command is authorized by this procedure.
7. No serial/issuer/live root work, real CA/certificates, Browser/PST/wire/network/admission until their respective gates. Next outcome is owner review of factual receipts, not automatic permission correction.