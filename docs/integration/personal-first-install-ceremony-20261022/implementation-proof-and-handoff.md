# Personal ceremony: independent DATA implementation and native gate

Task PACC-PHASE6-PERSONAL-FIRST-INSTALL-CEREMONY-1; 2026-10-22.
main / HEAD 913c90cf2328f7939d8f5b927640601903ac6e91.
Result PARTIAL. Official executor approved operations.json (9 files), then stop-operations.json (2 updates). These exact manifests/hunks are retained; no accepted contract changed.

## Implemented and owned by the existing root

PAPACC_PERSONAL_PLAN_DATA is embedded once in the existing canonical pending packet, not a new owner/registry/journal/worker. It owns an immutable bounded copy of up to8192 plan bytes and a32-byte attempt DATA value; input pointers never escape. Capture rejects zero/oversize spans, zero ID, aliased input and reuse of a nonempty state before mutation. Private portable C helpers match the complete plan bytes and attempt, enforce presentation/confirmation sequence, invalidate on mismatch and refuse skipped transitions/replayed confirmation/double consumption. Retirement wipes the entire plan capacity and keeps the consumed marker; no timeout or automatic rearm. Tests explicitly reset only model state to set up independent cases.

This is DATA sequencing, NOT factual confirmation, protected native association or administrative authority. The caller-supplied ID is NOT represented as qualified CSPRNG output. The model present transition is NOT actual display of a plan. No boolean/callback is added to assert native eligibility. Primitive helpers require serialized caller ownership/readable spans; they do not qualify cross-thread atomicity or a native handoff. Production integration must still enforce owner-thread transitions and complete F1–F5 before any effect.

Canonical capture checks existing root/thread/mode, Stop, slots/readers/retention and pending resources. A live DATA attempt participates in existing pending_blocks, preventing unrelated prepare/commit/reuse. Retirement checks workers, native handles, hash activity/retained resources and unresolved ownership before erasure. It never closes or resets old BCrypt/native refs. Canonical Stop invalidates DATA synchronously without wiping its plan or consumed flag; no native wait is added.

The real custody entry now has an explicit deny-only personal_native_ceremony_ready gate. DATA cannot satisfy it. The existing compile-isolated checkpoint91 diagnostic bypass remains isolated, unchanged in purpose, and was NOT executed as an administrative proof. There is no operational bypass or positive authority added. Actual config first-install remains the existing input-only denial; no incomplete plan is displayed for acceptance and no native object is created.

## First unresolved native integration

Existing packet9 bootstrap_worker acquires the parent and creates the child in the same worker operation; token/root/container proof is captured only after creation. Accepted F3 requires the parent already protected when presenting/confirming the exact plan. Reusing the post-creation proof to confirm before creation would be circular. The minimum missing integration is a parent-only, native-identity/eligibility preflight in the SAME root/packet, plus exact protected source/configuration presentation, qualified ID generation and owner-bound confirmation, followed by qualified consume/handoff/revalidation to existing exclusive creation.

No such preflight/UI/source/lifetime proof is claimed by this DATA component. No actual eligible/ineligible native operator case was executed. Existing token functions remain concrete reusable producers, not invoked through a fake source. CA_INITIALIZE remains separate; no actual CA owner/reference was acquired and no key/cert/signature authority created. The user-required incomplete-binding STOP applies to dependent native ceremony/effect integration. The independently complete DATA component is preserved.

## Prepatch memory and lifetime gate

The proposed metadata/owned-copy layout was compiled in isolation before application:8240 bytes x64. The declared increment is charged once in the existing root, no separate buffer pool or reuse credit. Scenario Legacy1799192/2097152 and Selected1964688/3145728 are within their managed caps; only Legacy was measured in this build campaign. The Selected number is prior fixed ledger plus the same member, not a new full measurement/qualification. No operational headroom follows.

Inputs are copied synchronously into the existing packet and no external plan borrow survives. New component uses no heap, token, handle, provider, callback, background operation or native key. Protected native participants and live source/material/CA coexistence are still absent, not priced zero. Full nested CRT/native stack, runtime/code/init/host attribution and provider/kernel costs remain UNKNOWN. ed11.json and COFF headers/symbols/disassembly are adjacent. No total peak or operational capacity PASS is claimed. ED11 PARTIAL; no quotas increased.

## Actual builds/tests and limits

MSVC Legacy Debug/Release built the real candidate server and selected tests, final zero compiler warnings. Nine suites each pass: personal_plan.data, bootstrap.scratch, config_binding.model/native_guard, phase6.memory_census, store_hash.owner/lifetime, issuer.production_gate/cng_production_gate. Thirty real candidate CLI negative invocations also pass; the CLI still returns OPERATOR=UNBOUND/TARGET=NATIVE_UNBOUND/AUTHORIZATION=ABSENT and never acquires a store/key.

New tests use model root/nonce/plan, real same-owner DATA implementation and a local Windows CriticalSection only. They cover invalid spans/alias, premature/skipped transitions, wrong attempt, changed plan, repeated confirmation/consume, retirement denied while model worker active, full plan wiping after quiescence, stopped capture denial and actual canonical Stop invalidating confirmation. They do NOT qualify native operator/target identity, real user display/confirmation, RNG provenance, authority, CA custody, actual UNKNOWN cleanup or concurrent native handoff. Those requested positive native cases are NOT_RUN, not replaced by models.

The first incremental Debug run mixed new owner code with cached old-header objects. Its censo still reported old sizes and store_hash.owner failed; this run is preserved as Debug-first-stale-build.log and is not counted as success. Both configurations were then rebuilt using --clean-first; layout and retention tests passed. Stop invalidation integration was subsequently reviewed/applied and all nine suites passed again in final logs. Future builds after this owner-header change must clean-rebuild affected consumers as required by the project's existing localized include-dependency limitation; do not reuse old layout objects.

Measured Legacy ROOT174704, PENDING42992, known managed1799192; delta8240 verified. None of these counts is a complete operational bound. No bootstrap/native namespace, packet9/10 commit, certificate generation, CA open/create, CURRENT publication, sockets/network or clock change was executed. No PST/Browser/wire/production0.6.4 change. No clone/worktree/commit/push/tag/release.

## Next exact implementation gate

Implement and prove parent-only eligibility/identity acquisition and retained disposition before enabling actual plan presentation, confirmation and native consumption. Reuse these DATA semantics only after full actor/target/source/CA/lifetime and resource proofs; no new normative front is required merely because native integration is unfinished. Keep the existing custody entry denied until that binding and immediately-before-effect revalidation are qualified. Subsequent native LAB commands still require their applicable exact approval; this report authorizes none.
