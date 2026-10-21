# ED11 isolated normal-package resource observer

This instrumented copy of ED08 leaves the original source and28 traces untouched. It uses public PST0.6.5/API3/SPI4 normal OpenSSL, MSVC19.51 x64 /MD /W4 /WX. No PST rebuild.

Configure this directory with Ninja and CMAKE_BUILD_TYPE Debug/Release, PST_SDK_ROOT the verified normal SDK, ED11_QUEUE_CAP8192/16384/32768/65536. Only65536 is the original approved experiment configuration; smaller queues are explicitly identified measurement variants.

Run ed11_resource_probe.exe CASE FIXTURE_DIRECTORY openssl OPERATION_ID. Cases: positive,local,precondition,cancel,eof,reset,isolation. Fixtures are the same public test-PKI DER/PKCS8 inputs as ED08. Require exit0, finalPASS and empty stderr; verify actual loaded-module paths and hashes. Scope identities by run/process/op/generation, not operation number across processes.

Each factual event samples process PrivateUsage, working set, peak/pagefile commitment, pool quota charges, handles and machine memory. Metrics overlap; do not sum them. Socket buffers are observed hints. Event snapshots are sampled peaks; PeakPagefileUsage is process historical high-water. Fixture server and diagnostic stacks/logging are part of these process totals. Actual opaque PST allocation attribution and total socket kernel memory are unavailable.

Probe contains two RECORD queues, not four production APPLICATION/RECORD queues or CONTROL/DATA Sessions. Active-membership PRECONDITION is real; auxiliary synthetic CloseUnresolved guards never qualify R4.

Run ed11_admission_test.exe accelerator or browser. Both are x64 isolated candidate budget models, NOT execution of Browser x86. They allocate/touch fixed core/app/Session storage, validate fail-closed reservation and retained credit rules. Synthetic retained slabs deliberately persist until diagnostic process exit, which is containment, not clean destruction or quota recovery. No PST outcome is injected.

See the governed closeout/evidence and proposed RFC0.3.3 for remaining full-topology/kernel/Browser/NT4/owner gates. Build logs and run hashes are retained in build/ed08-c3-20261012.
