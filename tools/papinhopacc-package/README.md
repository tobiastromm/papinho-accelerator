# PapinhoPACC source package

Package release: 1.1.0. API version: 1.1.0. PACC wire contract generation: 2. Envelope and CONTROL
wire versions: 1.0 each. Package release, API version and wire version are independent; the
contract generation is package metadata, not a new wire field.

Generation 2 cumulatively adds negotiated `NETWORK_EGRESS` major 2 and
`NETWORK_ABORT_REASON` (`0x000F`). The legacy generation-1 messages, including
empty-payload `NETWORK_ABORT` (`0x000E`), remain byte-identical. Terminal reason
zero is invalid; unknown nonzero reasons remain non-clean generic failures.

This package contains the public C headers and the single portable source
implementation of PACC framing and message codecs. It contains no server
runtime, socket, PST or platform implementation. Build with:

```text
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Consumers pin the published package/version and link `papacc_protocol`, or
compile the listed `src/*.c` with both `include/` and `include/papacc/pacc/`
as include roots. No prebuilt library ABI is assumed across toolchains.
Caller-owned buffers/sets and their capacities are explicit. The package
implements wire representation only; transport security, authentication,
authorization, egress policy and connection ownership remain with consumers.

The repository currently has no LICENSE or NOTICE file to copy into this
package. This generated archive is a local build artifact; publishing a release
and selecting its distribution terms are separate owner actions.
