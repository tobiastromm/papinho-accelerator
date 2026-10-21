# PapinhoPACC — public portable PACC contract

Status: PapinhoPACC 1.1.0 local contract/package complete; external release publication pending.
Canonical decision:
[ADR-0025](../adr/ADR-0025-contrato-publico-portavel-papinhopacc.md).

`papacc_protocol` compiles the single authoritative implementation of the
16-byte framing/parser and CONTROL, DATA, capability-negotiation and Network
Offload codecs. The public headers live in `include/papacc/pacc/`; portable
sources remain in `src/protocol/` and are copied, not forked, by the package
target. Server-specific processors, capability policy, Session/Channel/Job/
Stream state, network adapters and PST are outside the public package.

The package release is `1.1.0`; the C API version is `1.1.0`; the external wire-contract generation is `2`.
These are separate from the existing Envelope 1.0 and CONTROL 1.0 wire fields.
Generation 2 is cumulative: generation 1 keeps message IDs `0x0001`–`0x000E`
byte-identical and adds negotiated `NETWORK_EGRESS` major 2 plus
`NETWORK_ABORT_REASON` (`0x000F`). Its two-byte big-endian reason payload
defines `TLS_TRUNCATED=1`; zero is invalid and unknown nonzero values remain
generic non-clean terminal failures.

`cmake --build build/ninja --target papacc_protocol_package` produces a
self-contained source tree at
`build/ninja/papinhopacc-package/PapinhoPACC-1.1.0/`. It contains public
headers, seven codec source files, CMake instructions, README and a consumer
smoke test. The package can be built separately without server-private include
paths, sockets, Win32 or PST. The package build target is not a high-level SDK.
Release 1.0.1 fixed an internal VC6 compilation dependency on `UINT32_MAX`;
the immutable 1.0.x packages retain API/wire generation 1. The local 1.1.0 ZIP
SHA-256 is
`1E3637506A8746EC19B75F41AE54022F037E3D6305D003459967BB20586328A4`.
Validation: all seven packaged sources compile with VC6 `/W4` without warnings;
the VC6 consumer smoke test links and runs; the isolated C90 build, public
codec goldens and package consumer test pass. The complete Accelerator build
passes; 63 of 77 repository tests pass, while 14 shared secure
fixture/integration paths currently fail before protocol traffic and remain
outside this protocol-only package change.

The Phase 6 Reference Client now includes the public protocol headers and
links `papacc_protocol`; its client transport, PST and scenario logic remain
separate. Security and egress authorization are unchanged; protocol sharing
does not grant Legacy Endpoint network egress or create a secure/plaintext
fallback.

Publication remains pending: this repository has no LICENSE/NOTICE file to
include. The versioned local ZIP and SHA-256 can be pinned as an artifact, but
no GitHub release has been published by this task. Publication terms are an
owner decision. Browser integration I.A remains paused.
