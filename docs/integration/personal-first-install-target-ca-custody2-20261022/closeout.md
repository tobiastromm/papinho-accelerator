# Personal first-install target creation and CA custody — scoped closeout

Task PACC-PHASE6-PERSONAL-FIRST-INSTALL-TARGET-CA-CUSTODY-2, 2026-10-22.
Branch main; HEAD 913c90cf2328f7939d8f5b927640601903ac6e91 unchanged.

CURRENT_TASK_COMPLETED=NO. Independent target-to-worker implementation and native LAB qualification are complete; the production CLI still deliberately submits only the previously qualified read-only ceremony. An effect-capable confirmed plan and the actual CA_INITIALIZE/custody reference are not supplied. No existing read-only confirmation is upgraded into an effect grant. This closeout does not claim a completed first bootstrap.

## Implemented boundary

The existing pending_worker dispatches private packet13 before historical initialization can overwrite held source/parent references. Packet12 remains read-only. There is no additional worker, owner, queue, registry, journal or pin table, and no persisted format/wire changes. The same owner freezes the attempt, plan, relative leaf and retained native parent. A common start requires confirmed unconsumed DATA, matching attempt/leaf, no conflicting publication/preparation/thread/transaction, inclusive reservation and actual parent identity. Production also requires the canonical effect-admissibility gate and native token/parent/source revalidation. That gate remains false; no caller boolean/reservation opens it. LAB93 entry exists only in the explicitly compiled native component test and labels its authorization as MODEL.

Consumption occurs once before submitting the existing serialized worker. Submission failure never rearms it. Worker rechecks Stop/attempt/leaf and, for the production path, native context/source before the effect. The shared bootstrap_create_child uses NtCreateFile FILE_CREATE relative to the SAME retained parent handle: no textual parent reopen and no CreateDirectory/open gap. It rejects malformed leaves, existing files/directories and changed parent identity. Identity is captured only from the returned new directory handle. No child identity is invented before creation.

A definite name collision with no returned handle is CONFLICT, preserving the existing object's native identity. Other native failures or inconsistent creation/identity observations are UNKNOWN, not assumed rollback. UNKNOWN makes the owner unresolved and retains worker, parent, child and packet; no timeout-based release or retry. Stop before creation denies; Stop after confirmed creation prevents further effects while preserving the created fact. After factual thread signal and successful thread close, child closes before parent, with a disjoint once-only close-attempt bit. Close failure retains child/parent and blocks retry/double-free. The child is not deleted and confirmed creation is not described as rollback.

## CA dependency — factual, not synthetic

Canonical custody is PAPACC_CA_KEY in apps/server/server_ca_key_win32.h/.c, with provider/key, thread, retained token/local resources, actual operator/service SIDs, generation/name, first failure, state, close attempts and one borrower. Its create/open/validate/borrow/return/close APIs remain unchanged and uncalled here. Existing issuer CNG signing borrows that owner only with root/status/identity proofs; it is not the initial CA ceremony and cannot bootstrap its own prerequisite.

No actual PAPACC_CA_KEY owner instance is supplied to config first-install. The existing custody query receives NULL and reports absence in this invocation, not global key-store absence. READY metadata with zero native handles remains DATA only and effect_denied=1. No key/reference is fabricated or exported.

The first missing operation is a distinct explicit CA_INITIALIZE request/confirmation under F7 and issuer custody clause6, bound to the same operator/attempt and the newly observed native target. It must supply factual authority/generation identities, actual configured operator/service identities, a durable protected CA owner and reservations, then acquire/validate custody and preserve Stop/retention/correspondence. Current F10 plan explicitly states CA_INITIALIZE is not requested and persistent effects are denied. No CA key generation, open, borrow, certificate or signing was performed. Private-key protection remains unqualified for this invocation. No new normative proposal is needed to describe this accepted boundary.

## Qualification

Official executor review approved exact functional manifests and the exact isolated native runner before effects. A file-lock interrupted one application after CMake; preimages were re-read, two unintended historical-target link changes restored, and the remaining exact correction applied after builds ended. No automatic approval rejection occurred. Historical failed build logs are preserved separately from final passing logs.

Final MSVC19.51 x64 Legacy/Selected Debug/Release builds pass /W4 /WX with zero warnings. Selected explicitly selects Windows3MiB; Legacy remains2MiB. The new test links the existing storage SHA-256 capsule explicitly; Selected server CLI receives the missing private issuer include path. No toolchain/backend/quota substitution.

Ten offline suites pass in each of four builds (40 suite runs). The DATA regression now explicitly asserts sticky unresolved after a retained handoff, and resets only its resource-free MODEL fixture between independent cases; no product reset/rearm was added. Existing native read-only negatives pass.

44 approved isolated native cases pass (11 per build): new directory, existing directory/file, Stop before/after creation, injected UNKNOWN, injected close failure, parent/attempt mismatch, replay/double submission and metadata-only CA. These use a real retained filesystem parent and actual child handles/identities, with MODEL authorization only. Existing-object identity is compared before/after; child DELETE lease is denied while custody is held; pending close/retire is denied; no CURRENT/commit/CA authority appears. UNKNOWN/close-failure processes demonstrate retained references before exit; process exit is NOT evidence of backend destruction/rollback. Diagnostic timeouts fail without releasing resources.

Effects are confined to build/personal-target-ca-qualification-20261022/effects, a previously absent disposable root. Child directories, preexisting-fixture files and logs remain for evidence; no cleanup or historic LAB mutation. Test is deliberately not auto-registered in CTest. No external or loopback network, elevation, clock change, certificates, productive target, bootstrap or CURRENT.

F10 positive receipt48091b1698c841e6a63bb11ee5b0ab30, its binary, launcher and UI source remain byte-exact. Its normal F10 return0 and mandatory CLI final DENY2 remain distinct from the earlier interrupted-window result. No repeat UAC/F10 qualification was run. This preserves that old binary's scope; it does not qualify the new effect-capable path as an administrative ceremony.

## Capacity and handoff

No durable owner/layout growth. Legacy root175112/pending43400/known managed1799600, Selected root340608/pending73288/known managed1965096. Nominal arithmetic remainder is297552/1180632 respectively, NOT operational margin. Existing serialized64KiB worker reservation is not charged twice. Target adds at most one child handle in the existing field; test Stop barrier adds two LAB-only event handles. Retained UNKNOWN occupies the same owner and blocks new submission, not a growing pool.

Own compiled frames were inspected in Selected LAB objects: Debug child helper600, target worker104, target start72, dispatcher344, native close200 bytes; Release440/56/104/216/88 bytes, excluding return address and nested callees. These are frame observations, not complete peaks or a proof of platform API/CRT/provider/kernel stack. See ed11.json. Native filesystem/handle/thread/token/security-descriptor runtime, full nested stack, CA provider/kernel and future coexistence costs remain UNKNOWN. No operational ED11 PASS or zero opaque cost is claimed.

First unresolved divergence: effect-capable plan/admissibility and factual CA_INITIALIZE owner/reference are absent from the current read-only invocation; full native ED11 also remains unqualified. Preserve product effect gate. Next work must prepare that distinct authorized operation without reusing the consumed F10 confirmation, followed by resource qualification; no bootstrap/publication is authorized by NEXT.

## Result

CURRENT_TASK_COMPLETED=NO
PERSONAL_FIRST_INSTALL_CEREMONY=READ_ONLY_PRESERVED_EFFECT_PATH_GATED
F10_POSITIVE_QUALIFICATION=PRESERVED
TARGET_EXCLUSIVE_CREATION=LAB_NATIVE_PASS_PRODUCT_DENY
NATIVE_CHILD_IDENTITY=FACTUAL_IN_ISOLATED_LAB
PROTECTED_WORKER_HANDOFF=SHARED_PACKET13_IMPLEMENTED_LAB_QUALIFIED
CA_CUSTODY_BINDING=FACTUAL_REFERENCE_ABSENT_EXACT_DEPENDENCY_IDENTIFIED
CA_PRIVATE_KEY_PROTECTION=NOT_QUALIFIED_NO_KEY_OPERATION
STOP_RETENTION=LAB_PASS_UNKNOWN_RETAINS
ED11_TOTAL_STATUS=PARTIAL
BUILDS=MSVC_LEGACY_SELECTED_DEBUG_RELEASE_PASS
WARNINGS=0_FINAL_BUILDS
OFFLINE_TESTS=40_SUITE_RUNS_PASS
NATIVE_TESTS=44_LAB_CASES_PASS_MODEL_AUTHORIZATION_ONLY
NETWORK_TESTS=NOT_RUN
BOOTSTRAP_EXECUTION=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES_BASELINE1224_FILES_OUTSIDE_REVIEWED_SCOPE_BYTE_EXACT
FIRST_UNRESOLVED_DIVERGENCE=EFFECT_PLAN_AND_CA_INITIALIZE_FACTUAL_REFERENCE_ABSENT
NEXT=PERSONAL_FIRST_INSTALL_INITIAL_BOOTSTRAP_COMPLETION
