# Phase 2 Consumer Validation Closeout

## Build directory audit and policy

The original closeout audit found no Git-tracked build artifact. `.gitignore` uses `/build*/`,
covering all repository-root out-of-source build trees without ignoring source,
documentation, scripts, or checked-in protocol definitions.

| Directory | Generator/tool | Contents at audit | Disposition |
|---|---|---|---|
| `build/` | Visual Studio 18 2026 attempt | four small CMake cache/generator files; no product source | SAFE TO REMOVE |
| `build-ninja/` | Ninja + MSVC 19.51 | complete working tree and executables | KEEP CANONICAL |
| `build-phase1-audit/` | Ninja + MSVC 19.51 | complete temporary Phase 1/2 audit output | SAFE TO REMOVE |
| `build-vs2019/` | Visual Studio 16 2019 attempt | four small CMake cache/generator files; no product source | SAFE TO REMOVE |

The table above preserves the directory names and dispositions observed during
the original closeout. Subsequent repository housekeeping standardized the
current canonical tree as `build/ninja/`; the former `build-ninja/` tree was
deleted and regenerated rather than moved, so no CMake cache was reused.

The permanent convention is the repository-root `build/ninja/` out-of-source
tree. Developers use a Visual Studio Developer shell, so CMake, Ninja, MSVC,
the Windows SDK, and their environment are supplied together without embedding
machine-specific absolute paths in project files.

```text
cmake -S . -B build\ninja -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build\ninja
ctest --test-dir build\ninja --output-on-failure
```

## Server observability

`papacc_server.exe --help` originally documented
`--log-level error|warn|info|debug`.
INFO is the default and records listener/server lifecycle, accepted remote IPv4
or IPv6 endpoints, runtime Session IDs, ticket issuance without ticket bytes,
DATA attachment, close, and cleanup. WARN reports only rejection categories the
current processors can establish. DEBUG adds ticket expiry lifecycle. The
existing explicit `PAPACC_LOGGER` is reused; no global or second logger exists.

Post-Phase-2 CLI housekeeping subsequently added the sole extra value `off`.
The default remains INFO. OFF is a runtime logger state that suppresses every
ERROR/WARN/INFO/DEBUG sink delivery without redirecting stdout/stderr; explicit
administrative output such as `--help` and `--list-interfaces` is unaffected.

Complete ticket values, arbitrary payloads, credentials, and user data are
never logged. Runtime Connection/Session IDs remain local diagnostics and are
not new Wire IDs. Endpoint rendering uses the existing portable endpoint/IP
models; no `SOCKET` or `sockaddr` enters portable Core APIs.

## Consumer and integration validation

PapinhoBrowser on Windows NT 4.0 completed the Phase 2 flow over LAN TCP against
PapinhoAccelerator on modern Windows. The Accelerator remained optional and the
browser remained functional.

The official Win32 I/O-loop integration test additionally validates:

- invalid DATA ticket rejection without cross-Session binding;
- rejection when replaying an already consumed ticket;
- complete Sessions repeated without restarting the listener;
- two concurrent Sessions with distinct tickets and DATA Channels bound to
  distinct runtime Sessions;
- server health and storage reuse after cleanup.

These validations close Phase 2 only. Authentication, Authorization, Transport
Security, capabilities, compute, network egress, and application DATA payloads
remain outside this closeout. Phase 3 has not started.

## Official fresh validation

The canonical tree was deleted and regenerated from source with Ninja and MSVC
19.51.36256.0 in the Visual Studio Developer environment. Configure completed,
the build completed all 152 steps with `/W4` and zero compiler warnings/errors,
and CTest passed 41/41 tests. `papacc_server.exe --help` returned zero and the
runner test verified the INFO lifecycle messages, IPv4 peer rendering, runtime
Session ID reporting, DATA attachment, and server stop without exposing ticket
bytes.

```text
PHASE2 INVALID DATA TICKET INTEGRATION PASS
PHASE2 DATA TICKET REPLAY REJECTED PASS
PHASE2 REPEATED SESSION INTEGRATION PASS
PHASE2 TWO CLIENT ISOLATION PASS
```
