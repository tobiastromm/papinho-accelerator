# ED11 total-resource isolated probes

All source changes are test-only. No production C2/C3/wire, PST or Browser change.

Sources:
- ed11_policy_test.c: portable C89 fixed reservations, rollback/fault inputs, identity/Session/host-credit guards and saturated fairness. Final50 assertions; test-only provider/kernel/measurement absence is simulated policy input, not a native failure.
- ed11_lifecycle_loop.c: includes the immutable earlier ED11 observer, uses public normal PST and one owner. Up to512 bounded cycles, at most2 logical operations/4 endpoint contexts; actual PRECONDITION and eligible DESTROYED. Test fixture64MiB available-memory safety floor is not a product policy.
- ed11_virtual_map_probe.c: normal-package one/two-operation views at six phases; VirtualQuery address classes complement but must not be summed with private/working-set counters.
- ed11_c1_external_observer.c: modern external measurement of the immutable existing x86 C1 executable. No modern API added to Browser.
- ed11_c1_layout_vc6.c: qualified C1 struct extracted byte-exact and compiled against its actual headers in VC6; nested fields included, no native implementation rebuilt.

Modern build: documented PST tools/msvc-env.bat is environment-only. Configure this directory with Ninja, CMAKE_BUILD_TYPE Debug/Release, PST_SDK_ROOT the independently verified staged normal SDK0.6.5/API3/SPI4 commitda7190930700f54ecc92611f5edbcb0932caa5ec. /MD /W4 /WX; no PST build. Tests live in build/ed11-total-20261012; fail before execution on compile/link errors.

Run policy with no args. Run lifecycle loop with public test-PKI fixture directory and cycle count1..512. Run virtual-map probe with the same fixture directory. Run external observer with exact immutable C1 executable path and verify SHA2566B4ECEF516A6C208CB795B5E859EDCDA719C3A9F72AC2A1D499C1B3834185AB1. Capture stdout/stderr and exit; require explicit PASS. Observer snapshots are sampled, not phase barriers for the child.

VC6: use documented vc6-env.bat only for environment, compile /TC /MD /W4 /WX. Policy and extracted-layout probes link no PST/NSS library. Layout WIN32_LEAN_AND_MEAN excludes unrelated RPC headers while retaining /W4 /WX; no warning disabled. This is current Windows execution, not NT4.

Replay commands/flags are retained in build-*.cmd; public fixture files are existing build/ed08-c3-20261012/fixtures. SDK manifests/SHA256SUMS and all artifact/source hashes appear in capability audit. Historical runs/executable digests preceding bounded-loop extension remain separately labeled.

Scopes: controlled policy/storage behavior is qualified for these compilers; total opaque provider/socket kernel/Browser C2 and real NT4 are not. Retained policy slabs persist in a static owner until diagnostic process exit; exit is containment, never clean destruction. No R4 injection or NSS correction here.
