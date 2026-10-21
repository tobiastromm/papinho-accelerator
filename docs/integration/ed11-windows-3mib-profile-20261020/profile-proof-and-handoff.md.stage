# Windows 3 MiB profile — conformance, implementation and bounded handoff

Task PACC-PHASE6-ED11-WINDOWS-3MIB-PROFILE-1. main / c50b8fb9a7e3293b5e6355fc08459bf85b56857a,2026-10-20. Prior storage-strategy manifest verified; baseline4031 files recorded. Scope is one Windows Accelerator PROCESS, not client/Session/Connection. No runtime root, worker, journal, observer or authority was added.

## Normative review and registration

GOVERNANCE CONFLICT was reported before dependent implementation: ADR-0034 and frozen C2 specification11.3 explicitly state managed2MiB. Owner-authorized exact W1–W6 were registered through official executor approval as DECISION-ed11-windows-3mib-process-profile-20261020.md, with post-write SHA verification BEFORE code. Historical accepted ADRs, frozen specification, wire vectors and proposals remain byte-exact.

Precedence is numeric and conditional only: selected Windows Accelerator process ceiling3145728 replaces2097152 in corresponding local process-budget clauses. Other/legacy/nonselected targets and Browser remain unchanged. Upper OrganizationEngineering ADR0002/0003 govern recording/lifecycle; PapinhoEngineering ADR0007 explicitly permits local deployment specialization behind portable boundaries. No upper-layer numeric ED11 cap was found in locally available accepted records. Higher-authority hashes are retained; no external repository was modified or network accessed. Model E, ED08, root/C8, shared credits, individual quotas and all current authority/gate semantics remain normative.

## Explicit implementation

Use CMake `-DPAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE=ON` ONLY with existing Windows canonical API3 candidate and canonical C8 root/testing prerequisites. Default OFF preserves2097152. Selector accepts ON/OFF only; wrong platform, malformed selection or missing composition fails rather than silently selecting another profile. No config/CLI/env/runtime policy selects this cap.

Private src/core/ed11_process_profile.h contains constants and preprocessor checks only, no native types, allocation or platform APIs. PAPACC_ED11_PROCESS_LIMIT is3145728 only when the explicit selector1 and Windows target match; otherwise legacy2097152. Malformed defined values and Windows profile on non-Windows reject. The header adds no runtime identity/epoch. Existing context.profile still means its original TLS/target context, not this build selector.

Fifteen Windows-specific source/header/include files now use this single process ceiling: launcher, C8 resource/stage/policy, C2 participant/configuration/dispatch, administrative root/source/attachment/pending guards, CNG backend budget and normal/dormant manager sizeof checks. Exact hunks/preimages are in source-operations.json/exact-source.patch. No broad repository replacement occurred; only audited process-budget literals were replaced. All individually sized pools, queues, operation/Session counts, per-Connection managed/user/kernel charges and source material reservations remain unchanged. The existing shared-ledger logic is byte-identical; it still sums all live/retained charges once and uses checked remaining-cap arithmetic. A bigger cap is not a separate budget for each participant.

Compile-time aliases previously sharing the2097152 ceiling remain aliases of the same selected PROCESS value. Per-stage16384, stage count8, two global operations, one per Session, four64KiB flows/watermarks and the512KiB aggregate remain unchanged. Source/backend guards still price inputs, outputs, material and already-reserved concurrent storage; nothing receives a new grant merely because it fits. No wire/API/codec/schema or root policy projection was changed.

Production0.6.4/default selection is preserved. PST/Browser are unchanged. Legacy builds were independently built/tested with the opt-in OFF, not inferred from a macro test. Non-Windows default was additionally compiled as a constant-only model; box/ISA/NT4 are not qualified.

## Accounting before/after

No struct, allocation, buffer capacity, heap, stack reservation, slot/pin or retained object changed. Fresh sizeof census fields match EXACTLY across Selected/Legacy Debug/Release (only PROCESS_LIMIT differs). Known layout floor1790872, commit89328, pending34712, fixed graph1367992 and caller/source reservations from the previous storage reconciliation remain intact.

Known conservative current composition2067655, arithmetic remaining1078073 under3145728. Future ROOT08/issuer scenario2142951, remaining1002777. Those are accounted scenario sums, not measured process RAM/commit/handle peaks or approved reservations. The same future scenario remains45799 over legacy2097152; no legacy target acquired the new margin.

The future scenario already contains carrier, SNT1/CP2/LST1 and the proposed root/packet images, so do not add those nested components again. Actual packet/committed capacities remain16896; no ROOT08 worker widening or version2 transaction adoption happened in this task. A fitting future sum does not prove the actual adapter/lifetime/authority path.

CNG key480 and core216 are confirmed in fresh census; backend context696 is the same unchanged struct from pinned prior compiler evidence (including ports/descriptors, not plus those again). One caller arena example4096 gives known additional5488 if instantiated. They are not product-startup objects. Adding these to the future scenario gives2148439 BEFORE additional public inputs/outputs, CA backing and full callbacks/stack.

An explicitly illustrative prior fixture CSR645 + output capacity2048 and additive client RSP-chain4976 would yield2156108, leaving989620 BEFORE unpriced backing, source leases, registers, native/provider frames and retained/coexisting graphs. Fixture sizes are not maxima; the chain is not a full stack bound or proof that it is additional to existing stack reservations. SERVER public path is an alternative, not automatically simultaneous with CLIENT. Full CSR/TBS/DER/CA/prefix/history/source/pin/retained multiplicities and native resources remain unqualified. Full managed activation peak is UNKNOWN; provider/kernel UNKNOWN, host freshness/headroom unselected, ED11 total PARTIAL. No physical RAM availability was converted into credit.

## Validation

Selected3MiB and actual Legacy2MiB server builds pass Debug/Release with the same MSVC19.51/Ninja/x64 /W4 /WX and normal SDK0.6.5 (46 hashes verified). Sixteen selected suites per profile/configuration pass, including historical ROOT8/LST1, v2 carrier/data model, issuer/checkpoint/public-X509/production denial gates, census/layout and five new profile/shared-credit tests.

New ledger tests reuse the unchanged real ledger implementation over synthetic evidence: two fixed-size connection charges fill one shared process cap, a third is denied, unused rollback refunds once, retained LIVE charge cannot be rolled back, Stop preserves retained usage and denies new reservation, and above-cap base is denied. Per-connection65536 is a fixture charge and unchanged across profiles, not a new normative quota. No native measurement authority is claimed by synthetic facts.

Native D3 guard profile-success uses synthetic already-reserved counts3137536 (selected) or2088960 (legacy), with actual file/TxF/CAS. It proves the compiled worker guard recognizes its profile, not just a header constant. Existing exact-cap overage guards still reject. Selected native tests additionally pass six D3 modes, seven ROOT06 cases and seven bootstrap cases per configuration, covering first/last-byte conflict, rollback/UNKNOWN/Stop and pinned snapshots. Authority/policy/CA metadata and diagnostic administration are synthetic/compile-isolated, not operational CURRENT or qualification of admin token. No real CA/key/certificate, native private signature, productive bootstrap, admission or network was executed.

Four expected-failure tests confirm malformed header selector, Windows selector on non-Windows, invalid CMake boolean and missing canonical composition; diagnostic messages were checked. Default/off process profile and unchanged other-target constants compile successfully. The existing isolated dormant participant passes43 checks per configuration under its original legacy/closed profile.

Initial validation attempts are preserved: an unavailable dormant target was first requested, then the incompatible combination of isolated-dormant and C8 build exposed its old include/context mismatch. Scripts were separated into the supported canonical and original closed participant configurations; root-dependent offline options were removed only from the isolated test configuration, without changing source dependencies/security or toolchain. One new test had an explicit U64-to-U32 assignment warning; exact test-only correction split constant assignments, preserving /WX. One direct Ninja invocation lacked vcvars64 include environment and failed on stddef.h; final build used the documented vcvars64 wrapper. Final builds have zero compiler warnings; these logs are not erased or relabeled as successes.

## Remaining handoff

The numeric Windows budget gate is resolved for the explicit candidate profile. Next prepare the exact ROOT08 issuer worker adaptation using this profile, actual same-root snapshot/pin/source lifetime, carrier history, complete transitive/retained storage and native outcome proof. Current packet capacity and v2 activation are still unsupported; do not publish CURRENT or sign from decoded data. Continue denying provider/kernel/host-unknown admission and authoritative recovery without qualified checkpoint/fencing. No automatic PST0.6.5 release or Phase6 completion.
