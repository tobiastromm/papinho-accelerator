# Portable issuer core closeout — 2026-10-19

Task PACC-PHASE6-PORTABLE-ISSUER-CORE-IMPLEMENTATION-1.

Portable implementation/offline slice delivered in canonical Accelerator. This is NOT a complete operational issuer: the native durable-root adapter, physical journal codec/recovery and live publication remain unimplemented. No interface/mock is promoted to proof of actual durable storage. See implementation-and-root-handoff.md for exact adapter obligations and remaining work.

## Implementation

src/issuer/issuer_core.h/.c: platform-neutral CA/generation/SPKI identity, immutable request correlation, 8+8 byte serial, CAS reservation, durable-before-sign protocol, eight states, exact-certificate idempotent lookup, revocation, new-key renewal linkage, conservative recovery, Stop, reentrancy guards and retained leases/output. Interfaces cover protected key access, generation/entropy, certificate signing/verification, trusted UTC, authorization and persistence by the SAME root. Generation ports are not invoked by issuance/recovery. No independent registry/file/store, root/listener, policy grant or native epoch generator added.

No native root adapter is installed. Existing first-dormant transaction guards and BODY0.5 remain unchanged. Production rejects missing root capability and test-only adapters; no dormant guard bypass, fake commit, fixed epoch or CURRENT claim. The PUBLISHED state is exercised only in the explicit synthetic model; real publication remains unavailable. Current core library is not attached to product startup.

CMake adds papacc_issuer plus isolated test executables. Test acceptance macro is private to separately compiled offline core; it is absent from papacc_issuer. Source/header dependency declarations cover new objects despite the existing localized include scanner. Prior CNG backend/diagnostic and product pin hashes matched; no Browser/PST/wire changes.

## Validation

Real MSVC x64, /W4 /WX. Authoritative final CMake Debug/Release build6 + ctest6 logs:117 core assertions plus4 production gate checks per configuration;2/2 named CTests pass each. Include/import/symbol inspections show no Win32 API headers or native crypto dependency in the portable issuer component. Only existing C runtime/compiler headers are used. CNG/OS crash/filesystem atomicity/hardware are NOT qualified by these tests.

Coverage includes serial/endian/max; idempotence/no re-sign; immutable-request conflicts; interleaved competing writers; uncertain durable reservation; crash before signer after SIGN_REQUESTED; signing uncertainty, retention and no retry; bad/missing backend, denied auth; renewal/revocation/abandonment/history; output integrity; archived DER lookup without private signer; policy/SPKI replacement; UTC regression/expiration; Stop during signing and failed lease release; invalid root proofs/rollback; production rejection of model adapters. No new key or real certificate generated. Test DER marker has no cryptographic significance.

An intermediate CMake test failed because the gate object was stale after a header change; explicit dependencies fixed it and final objects/tests passed. The failed test/build logs are preserved; earlier passes were not reused for current source.

## ED11

Compiled x64: core216, record288, root view176, backend88, request136, root port40 bytes. Root-port copy is embedded in core and is not charged twice. Root record table/journal/DER are NOT allocated by core; callback/caller retains real storage and credit obligations. Synthetic model5408 includes its16 records/certificate slots; output128 is test-only. No productive capacity/threshold selected from fixture values.

Debug compiled rsp allocations: prepare1256, issue1224, store640, load64; full disassembly/table saved. These are individual frames, not a complete bound; nested frames, saved registers, runtime and backend scratch/coexistence/retention still require composition accounting. Library has no internal heap/new product instance; existing root quotas unchanged. Real journal, backend, provider/kernel costs remain pending/UNKNOWN. No ED11 total PASS.

## Preservation and result

Initial read observed main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a and519 WIP entries. Captured baseline includes the newly created evidence directory (520 entries); it is not falsely counted as preexisting work. Preimages of CMake/live docs preserved. No clean/reset/checkout, clone/worktree, security change, key operation, network, commit/push/tag/release. All scoped executor submissions approved; no denied operation bypass. Earlier unknown key remains UNKNOWN and is not used by core.

```ini
CURRENT_TASK_COMPLETED=YES
PORTABLE_ISSUER_CORE=IMPLEMENTED_OFFLINE_VALIDATED
PLATFORM_BACKEND_INTERFACE=IMPLEMENTED_NO_NATIVE_ADAPTER_ENABLED
SERIAL_PERSISTENCE=CORE_CAS_PROTOCOL_VALIDATED_NATIVE_ROOT_STORE_PENDING
ISSUER_STATE_MACHINE=EIGHT_STATES_IMPLEMENTED
ROOT_INTEGRATION=SAME_AUTHORITY_PORT_IMPLEMENTED_NATIVE_ADAPTER_PENDING
LIVE_PUBLICATION=NOT_IMPLEMENTED_DENY
BUILDS=CMAKE_DEBUG_RELEASE_PASS_FOCAL_TARGETS
WARNINGS=0_FINAL
OFFLINE_TESTS=117_CORE_ASSERTIONS_AND_4_GATE_CHECKS_PER_CONFIGURATION
NETWORK_TESTS=NOT_RUN
CNG_NATIVE_STATUS=UNRESOLVED
BOX_ISA=NOT_QUALIFIED
REAL_CA_CREATED=NO
REAL_CERTIFICATES_ISSUED=NO
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
ED11_TOTAL_STATUS=PARTIAL
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_SCOPED_PREIMAGES_AND_PRESERVATION_HASHES
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=CANONICAL_ROOT_HAS_NO_VERSIONED_ISSUER_DURABLE_ADAPTER
NEXT=INTEGRATE_PORTABLE_ISSUER_WITH_WINDOWS_CNG_AND_LIVE_ROOT
```

Completion refers to the portable-core/offline slice. Serial durability, same-root native storage and actual publication are explicitly pending integration requirements, not claimed PASS. Do not enable issuance by substituting the test root/backend. Accepted profile2.5 requires the versioned private issuer layout/capacity review before root write integration; current BODY bytes must not be repurposed silently.