<!-- SPDX-License-Identifier: MPL-2.0 -->

# PapinhoSecureTransport 0.6.0 integration handoff

Status: published dependency baseline adopted by the private, opt-in
PapinhoAccelerator security integration. This document records the factual
release contract; production server integration remains future work.

## Release identity and artifacts

```text
PST_RELEASE_TAG=v0.6.0
PST_RELEASE_URL=https://github.com/tobiastromm/papinho-secure-transport/releases/tag/v0.6.0
LIBRARY_VERSION=0.6.0
API_VERSION=2.1.0
SPI_VERSION=3.0
```

| Public asset | SHA-256 |
| --- | --- |
| `papinho-secure-transport-0.6.0-src.zip` | `6f4edd34fb281829698c38a55ba378dcf08055af77e9caf75040cbbfb618fb04` |
| `papinho-secure-transport-0.6.0-win32-x86-vc6-retrozilla-nss.zip` | `d8f41baaa77a79acdd083d94bf22a664cab4f3c6079db77b50b56abf8b6b190c` |
| `papinho-secure-transport-0.6.0-win32-x64-msvc-19.51-schannel.zip` | `ee2ad0ef1e1e93b0ac2fdd614e954f2fb5231f88f91aa6fda7f3704af0aa455c` |
| `papinho-secure-transport-0.6.0-win32-x64-msvc-19.51-openssl3.zip` | `4dfd99e2b25b6b447e88582d60d38646b92846ce52c294f78eb8e4f3909b7eb1` |
| `papinho-secure-transport-0.6.0-win32-x64-msvc-19.51-schannel-openssl3.zip` | `5eaf426586efe918643f95514c0a9b459d292044837c85a7dbfe0de5b911ca77` |
| `SHA256SUMS-packages.txt` | `274710700eb32743d4e8de6532df6044fa1c75fda02a635c7efb95545bc0d90a` |

Pin the exact target asset and hash required by the deployment. Do not consume
`latest`, a PST checkout, or an unverified local build. The VC6/x86 target uses
RetroZilla NSS; modern Windows may use Schannel, OpenSSL, or Combined according
to required capabilities and deployment policy.

## Scheduling and integration boundary

PST 0.6.0 supplies a portable wait-set for multiple secure connections,
timeout-zero polling, finite blocking waits, stable registration tokens,
cross-thread wake, and borrowed external/native sources. External sources let
the Accelerator-owned scheduler observe listeners or other native readiness;
PST never accepts a connection or closes a borrowed resource.

The Accelerator remains owner of listener/accept, protocol framing, session and
channel policy, authorization, deadlines, fairness and work budgets. PST owns a
connected transport only after an explicitly accepted ownership transfer. I/O
is incremental: partial reads/writes and backpressure are normal, buffers must
outlive pending operations, and consumer-owned deadlines must bound progress.
There is no provider fallback after binding.

Local Identity, Peer Authentication, Peer Trust and Expected Peer Name remain
separate snapshot-based inputs. CLIENT SNI is tri-state: `COMPAT` preserves the
historical expected-name behavior, `DISABLED` explicitly sends no SNI, and
`EXPLICIT` supplies an independent SNI value. Provider capability masks must be
checked before binding.

Provider capabilities are asymmetric. OpenSSL supplies the broadest validated
CLIENT/SERVER TLS 1.2/1.3 and SERVER ALPN set. Schannel SERVER is validated for
TLS 1.2 but does not advertise TLS 1.3 or complete PST SERVER ALPN semantics on
the validated environment. RetroZilla NSS SERVER supports TLS 1.2/1.3 but does
not advertise SYSTEM_TRUST or complete PST SERVER ALPN. Independent SNI control
is not complete in NSS.

## Validation and adoption status

The published bytes passed deterministic package reproduction, extracted-SDK
consumers, clean Windows x64 validation, real Windows NT 4.0 SP6 x86 validation,
API 2.1 scheduler validation and public download-back verification. M9 passed
all 9/9 TLS 1.2 provider pairs and all 4/4 eligible TLS 1.3 pairs.

```text
ACCELERATOR_3B4_ARCHITECTURAL_BLOCKER_RESOLVED=YES
```

This marker means the PST scheduling/transport foundation exists. It does not
claim that Accelerator production integration, protocol behavior, Principal
mapping, authorization, or deployment policy has been implemented.

## Accelerator adoption

Phase 3.B4 pins this exact OpenSSL target and uses only the public API 2.1
wait-set and Win32 external-source surface. The private scheduler owns its
wait-set, requires caller-provided bounded registration/event storage, assigns
stable non-pointer tokens, and rotates dispatch independently of PST event
order. Registrations are removed before member release. External sources and
their native listener remain borrowed and consumer-owned.

The deterministic opt-in harness multiplexes two PST connections and one
listener, exercises zero/finite timeout and cross-thread wake, and verifies
failure isolation and bounded dispatch. The production `papacc_server` accept
loop, PACC/CONTROL/DATA, Principal mapping and authorization are unchanged.
