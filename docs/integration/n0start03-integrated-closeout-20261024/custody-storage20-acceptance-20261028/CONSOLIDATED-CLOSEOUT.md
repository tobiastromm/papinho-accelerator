# Custodian and STORAGE_20 accepted — guarded root integration

TASK=PACC-PHASE6-ED11-CUSTODY-STORAGE20-ACCEPTANCE-AND-INTEGRATION;2026-10-28. Canonical main/HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preserved. No real credential or TPM operation.

## Independent normative records before code

Both complete JSON proposals were read and byte-checked: custodian40e4744a877980bb28fe2703b97913f23c1cb7e02642145406306c182eda0eb0; sourcea2b31e0266f03cbd489c2d2b3fccc4e215c882244e9176e4c7716d9e2a7d01a4. Owner explicitly approved them independently. Registered separate ACCEPTED documents before implementation:

- DECISION-q4-storage20-custodian-designation-20261028.md, SHA256dbc8288dedf0f71b3c8f928203977e84eb415990e658b128d0c9786ba91ad5fc.
- DECISION-q4-storage20-source-designation-20261028.md, SHA2562d5021788c01bef6bfa21a476470dd5b472f16f2365e8c67015ac42a5f709469.

Original proposed/null-decision source bytes remain historical and unchanged. W/L/L1/TC/cross-boot compatible; no accepted historical document rewritten. SID1108 is now normatively designated for the exact Q4 custody purpose, and Windows Tbsi_Get_OwnerAuth STORAGE_20/type13 is separately designated source. These decisions do NOT prove current token/secret possession/validity, actual ciphertext custody, original NV generation or grant value access. No NV/Q4/trust-anchor/revocation exception inferred. No operational approval was generated.

Historical metadata indication and displayed postexecution-hash limitation preserved; no native identity retroactively reconstructed. No source/possession query repeated.

## Effective technical integration

Added designation DATA API to existing canonical root/pin helper. It returns approved SID, separate exact decision hashes, type13, observed EK/certificate reference, candidate index and current guarded root DATA snapshot. It ALWAYS returns scope_data_only1/credential_access_denied1. Actual operator/root lease/authentic approval/provenance/resource facts remain mandatory. Existing TC SOURCE_MISSING/authority_denied is not cleared by normative designation and no fake authority setter exists.

Canonicalized the qualified memory-delivery implementation under tools/qualification/tpm-native-proof/storage20_delivery.c/.h and added storage20_binding.c/.h. CMake opt-in native library includes delivery; new binding library links the EXISTING root and native core, no new product owner/service/registry/IPC. Two new CTests are default nonoperational and only exist when existing opt-in candidate/root testing targets are available. Legacy native option remained OFF; no backend/toolchain switch.

Descriptor borrows original TP_OWNER/context and immutable root binding. Root guard reobtains exact accepted designation/current snapshot each boundary; rejects stale pin/Stop/root mutation, incorrect SID/EK/certificate/designation, missing request/approval binding, wrong actual source-function snapshot, original-context change, and buffer overlap with root/pin/binding. The native owner's boundary is mandatory and must independently validate authentic original Windows/module/token/approval/lifetime/resources; supplied SID/digest is NOT authentication. Boundary is rechecked before use and root remains current after callback. No plaintext copied to root or output. Immutable descriptors/pins/bindings must remain alive while retained.

Source export resolver accepts only borrowed module metadata at System32tbs.dll and resolves Tbsi_Get_OwnerAuth; it never opens a module/context or calls the API. Path/export metadata alone is NOT provenance: native boundary still owes original file/identity/protection checks. Tests reject null and wrong already-loaded Windows module without loading TBS. No actual provider activated in an operational root route; there is no new manual credential CLI or test-binary route pretending to be a real owner.

Same-user/admin trust boundary remains explicit. The mechanism does not isolate credentials from hostile code running as trusted same user/admin. No secret in argv/ENV/log/file/report/hash; one source value call only into locked disjoint caller scratch, no resize/retry. API success/nonempty shape is not NV authorization validity. Fixed64-byte scratch profile is a reviewed conservative mechanism bound, not universal TPM size or a memory grant.

BUSY/owner thread cover get/borrow/clear/unlock. Unknown borrow or uncertain clear/unlock preserves originals/lock/dependencies/BUSY/freeze; no forced wipe while borrower pending, no timeout/refund. Known synchronous completion wipes before checked unlock. Checked clear uses SecureZeroMemory and volatile read verification; failures freeze/retain and return failure. Caller must not let retained descriptors/owner/module/context or scratch go out of scope. Existing finish refuses retained dependencies. Process loss remains UNKNOWN.

## Builds, tests and corrections

Selected/Legacy Debug/Release product/root builds PASS zero warnings. New canonical binding/delivery targets built only Selected Debug/Release modern native profile; Legacy did not silently enable native TPM. Eight CTest invocations PASS:3each Selected configuration plus1each Legacy. Existing root DATA fixture497checks/config; canonical delivery82/config including prior75 and clear-failure regression; root integration543/config INCLUDING reused root DATA checks (not543new independent checks). Test logs and outputs copied into this package.

Coverage: custodian/SID, source pointer and decisions, TPM public reference, original/stale root pin, absent exact approval/request, foreign thread/reentry, buffer overlap/size, source missing/partial failure/invalid fixture credential, Stop, UNKNOWN and cleanup. Native Windows lock/wipe/threads and production DATA/binding/delivery code are real; root initial authority and native operator/source/approval boundary are explicitly modeled. Positive mocked native-boundary callbacks are NOT operational producers. Source only get_mock, no ownerAuth real access or NV.

A new clear-failure test found a genuine prototype bug: it retained UNKNOWN state but returned the earlier success flag. Fixed ok0 on failed clear; regression passes both configs. The initial Release negative also prompted a deterministic zero-padding DATA copy and a genuinely distinct wrong-source stub (the old stub delegated identical behavior and could be folded by optimization); do not claim an operational exploit or native credential exposure from that fixture. Original failure logs retained. Header include order fixed to avoid _WIN32_WINNT redefinition without warning suppression. A patch script parse error occurred before edits and was corrected. No contracts relaxed.

Original isolated75checks/config evidence remains unchanged historical; source changes justified the affected canonical regression reruns. Existing unrelated1929adapter/core crypto/Browser/product campaigns not repeated. Native-memory qualification never touched real TBS credentials.

## Storage/lifetime costs

Root owner remains Selected266656/Legacy98088bytes, root binding88 unchanged. New normative designation output328bytes is caller DATA, not persistent owner growth; binder560bytes borrowed state, delivery72bytes (original isolated56 + guard-context/source snapshots), API48bytes (original40 + checked-clear callback), scratch profile64. No quotas/arrays of product handles or storage budgets enlarged. Fingerprint constants and code/transient stack are costs, not free credit. Exact zero padding is canonicalized by memcpy from fully initialized DATA to avoid optimized struct-assignment padding ambiguity. No wire/carrier5/component512/CAS/worker14 format change.

Windows locking works at page granularity; fixture requests4096-byte regions and retains original UNKNOWN cases until fixture process loss. These requested/compiled sizes are not total reservedVA/stack/provider/kernel/transitive memory or future native peak. No actual credential/TBS resource capacity measured.

## Single future controlled-memory request

CONTROLLED-STORAGE20-MEMORY-REQUEST.DRAFT.json is the only prepared candidate: fixed custodian/source decisions, Q4-only purpose, reference EK/cert, one type13 value call,64-byte cap, synchronous shape-only consumer, no plaintext/secret hash output, no persistence of hierarchy credential, first-error/Stop/retained UNKNOWN, existing124/128/1024 obligations/no credit. Zero direct TPMSubmit, NV/Clear/hierarchy/Q4/Start/Stop/root publication and no auth-validation command permitted. It explicitly cannot prove NV auth validity or hardware generation.

It is NOT_EXECUTABLE and not ready to request a manual run. Actual original root-domain/predecessor/file manifest, live owner/pin/token/context/source-module bindings and an authenticated native approval boundary for THIS request are not supplied/active. No root values, future hashes or operational identity were invented, and no new owner was created to make a synthetic pin pass. Offline root/approval-positive fixtures cannot fill these facts. REQUEST-COMPLETENESS.json identifies these exact gaps, request.approvalnull and manual_commandnull. There is no premature executable command or approval in owner's name.

The next operational decision, once that original-owner binding is factual, is ONE memory-access-only manual approval under the finalized exact request/artifacts. Designation acceptance is not that approval. No NV effects may be bundled. First real proof remains controlled delivery and factual cleanup in the legitimately bound owner; its validity for NV remains separate. Narrow source delivery need not fabricate a pre-existing unprovisioned NV generation; downstream trust/revocation/generation gates stay DENY.

## ED11 and preservation

E21 updated with the two real decisions and offline integration, not secret delivery/custody persistence/NV/readback PASS. RevocationUNKNOWN, hardware original-generation, effective real delivery and NV credential validity remain unproved. E17 service identity/access/readiness and E18 Stop/lifecycle/quiescence remain factual independent operations. E19 D4/D5 target quantitative policy and E20 root/CURRENT/epoch/SiteTLS/backend/destination/one-operation provider composition still pending; no invented values or repeated equivalent campaigns. E22 derived pending. ED11/Phase6 PARTIAL, reservations/UNKNOWN preserved.

New product builds and library/source/test hashes are prospective derived evidence, not activation of historical operational pins. Consumed manual artifacts/approvals/source proposals and all other accepted decisions remain unchanged. No secret/NV/TPM/Q4/SCM/ACL/Start/Stop/CURRENT/signature/recovery/cleanup/refund, no commit/push/clone/worktree. Scoped patch/preimages/build failures/results and final SHA256SUMS manifest preserve the WIP-relative history.
