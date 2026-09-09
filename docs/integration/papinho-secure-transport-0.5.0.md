<!-- SPDX-License-Identifier: MPL-2.0 -->

# PapinhoSecureTransport 0.5.0 integration handoff

Status: published dependency baseline approved for resuming PapinhoAccelerator
Phase 3.B2. This document does not implement the Accelerator integration and
does not change Accelerator production code.

## Release identity

```text
PST_VERSION=0.5.0
API_VERSION=2.0.0
SPI_VERSION=3.0
PST_RELEASE_TAG=v0.5.0
PST_RELEASE_URL=https://github.com/tobiastromm/papinho-secure-transport/releases/tag/v0.5.0
```

PapinhoSecureTransport 0.5.0 is the secure-transport dependency baseline for
resuming PapinhoAccelerator Phase 3.B2. Integration must use the public PST API
and the immutable published SDK appropriate to the Accelerator target.

## Published artifacts

| Artifact | SHA-256 |
| --- | --- |
| `papinho-secure-transport-0.5.0-src.zip` | `815ec2e492e3e35895f674517d3b869242bc553d1e82aaf8fd2df0ca38905bbc` |
| `papinho-secure-transport-0.5.0-win32-x86-vc6-retrozilla-nss.zip` | `9a7e56648bd04a816c80428a05c636707d118c7ed9f31d6238aeec2e39f038c5` |
| `papinho-secure-transport-0.5.0-win32-x64-msvc-19.51-schannel.zip` | `9acd633f348b9bb186066fbb45f0e994aa42be8e9e270813f7bacb8391c85c25` |
| `papinho-secure-transport-0.5.0-win32-x64-msvc-19.51-openssl3.zip` | `5464d8bd0617f01c5bc30b7d36b784c2348580057f469b8928a598f22e8db62b` |
| `papinho-secure-transport-0.5.0-win32-x64-msvc-19.51-schannel-openssl3.zip` | `8006251c7c411a016a7c7097d9c5bb41f8d3206b71bf4c5155bbf0b514a17b07` |
| `SHA256SUMS-packages.txt` | `68e69bace3696be99c1fea248300dfc02a067bd69a07bcc652313afe54f465a9` |

## Providers and role-scoped capabilities

```text
OPENSSL_CLIENT=YES
OPENSSL_SERVER=YES
SCHANNEL_CLIENT=YES
SCHANNEL_SERVER=YES
RETROZILLA_NSS_CLIENT=YES
RETROZILLA_NSS_SERVER=YES

OpenSSL
Aggregate=0x00007fff
CLIENT=0x00007eb7
SERVER=0x0000777b

Schannel
Aggregate=0x00007efd
CLIENT=0x00007eb5
SERVER=0x00007679

RetroZilla NSS
Aggregate=0x00007aff
CLIENT=0x00007ab7
SERVER=0x0000727b
```

SERVER facts are intentionally provider-specific:

```text
OPENSSL_SERVER_TLS12=YES
OPENSSL_SERVER_TLS13=YES
OPENSSL_SERVER_ALPN=YES
OPENSSL_SERVER_SYSTEM_TRUST=YES

SCHANNEL_SERVER_TLS12=YES
SCHANNEL_SERVER_TLS13=NOT_ADVERTISED_ON_VALIDATED_ENVIRONMENT
SCHANNEL_SERVER_ALPN=NOT_ADVERTISED_WITH_COMPLETE_PST_SEMANTICS

NSS_SERVER_TLS12=YES
NSS_SERVER_TLS13=YES
NSS_SERVER_SYSTEM_TRUST=NOT_ADVERTISED
NSS_SERVER_ALPN=NOT_ADVERTISED_WITH_COMPLETE_PST_SEMANTICS
```

## Selection and lifecycle contract

```text
PROVIDER_SELECTION_PER_CONNECTION=YES
EXACT=YES
ORDERED=YES
AUTOMATIC=YES
ROLE_SCOPED_FILTERING=YES
NO_POST_BINDING_FALLBACK=YES

LOCAL_IDENTITY=YES
PEER_AUTHENTICATION=YES
CUSTOM_TRUST=YES
SYSTEM_TRUST=ROLE_PROVIDER_DEPENDENT
EXPECTED_PEER_NAME=CLIENT_ONLY
MTLS=YES
PEER_INFO=YES
INCREMENTAL_READINESS=YES
RECIPROCAL_SHUTDOWN=YES
TRUNCATION_DETECTION=YES
SECRET_SAFE_LOGGING=YES
```

## Ownership boundary

PapinhoAccelerator owns bind, listen, accept, the Accelerator protocol,
channel/session architecture, application authorization and Accelerator
capability negotiation.

After connected-transport ownership is accepted, PST owns the TLS lifecycle,
provider selection and pinning, TLS policy, identity/authentication/trust, TLS
readiness, encrypted I/O, TLS shutdown and normalized TLS diagnostics and Peer
Info. PST does not own the listener or accept operation.

Do not create a parallel Accelerator-specific TLS abstraction unless a future
architecture decision explicitly changes this boundary. Do not copy PST
private/provider headers, vendor arbitrary local PST build outputs, depend on
`feature/server-side`, reimplement PST provider selection/readiness/trust/TLS
lifecycle inside Accelerator, or assume fallback after provider binding.

## Validation evidence

```text
CROSS_PROVIDER_INTEROPERABILITY=PASS
SECURITY_LIFECYCLE_NEGATIVE_MATRIX=PASS
WIN10_X64_CLEAN_MACHINE_PACKAGE_VALIDATION=PASS
COMBINED_REAL_TLS_CLEAN_MACHINE=PASS
NT4_SP6_X86_PACKAGE_VALIDATION=PASS
PACKAGE_REPRODUCIBILITY=PASS
PUBLISHED_ASSET_HASH_VERIFICATION=PASS

ACCELERATOR_PST_BASELINE=0.5.0
ACCELERATOR_PHASE_3B2_UNBLOCKED=YES
```

The current dependency pin remains historical until Phase 3.B2 performs and
validates the consumer migration. That phase must update the pin to an exact
0.5.0 release asset and SHA-256 under ADR-0008; it must not use `latest`, a PST
checkout, or an unverified local build.

## Accelerator Phase 3.B2 adoption

Phase 3.B2 completed that consumer migration. The dependency pin now names the
exact published 0.5.0 target asset and SHA-256. The outbound proof uses an
explicit CLIENT role, while the private `papacc_security_composition` owns the
runtime, local credentials, peer trust and exact SERVER provider/profile for
the Secure Principal. Construction is failure-atomic and release is
idempotent.

This adoption does not attach PST to accepted sockets and does not implement
handshake/I/O scheduling, the PST logging adapter, Principal mapping,
authorization, secure DATA association, Browser integration or `TLS_OFFLOAD`.
