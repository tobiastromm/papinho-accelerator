# ED08 isolated public-PST C3 probe

Standalone MSVC19.51 x64 /MD harness. Read the [scoped closeout](../../../docs/capabilities/pacc-c2-ed08-openssl-c3-factual-closeout-20261012.md) before interpreting PASS.

Configure this directory with Ninja, CMAKE_BUILD_TYPE Debug or Release, and PST_SDK_ROOT set to an independently verified normal API3/SPI4 SDK. Use the documented PST msvc-env.bat only to establish toolchain environment; do not build PST. CMake validates manifest identity and copies package OpenSSL DLLs beside the executable. /W4 /WX is mandatory.

Run: `ed08_c3_probe.exe <case> <fixture-directory> <openssl|auto> <operation-id>`.
Cases: positive, local, precondition, cancel, eof, reset, isolation. Fixtures: ca.der, client-a.der, client-a.pk8, server.der, server.pk8 from the public test PKI. Capture stdout JSONL and stderr separately, require exit zero and final PASS, verify loaded-module paths/hashes. Combined auto must show runtime provider count 2 and public OpenSSL binding for both roles.

This is a bounded opaque-record bridge between two public PST endpoints, not Browser or production PACC. Each operation has generation 7 and distinct client/server lifetime identities. Do not compare clocks across processes; owner events use monotonic sequence within each operation. Candidate frames use a private test-only socket. Synthetic CloseUnresolved guards never qualify real PST/native retention. Consumer peer sockets are owned by the fixture; transferred PST sockets are never closed by the consumer.

The preserved run manifest records all trace/executable/log hashes. Its initial-source trace is historical; revision2 runs correspond to the current C source. No unsafe native-close fault injection is provided.
