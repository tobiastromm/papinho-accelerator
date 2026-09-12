# Phase 3.F — Secure Reference-Client Interoperability

Status: complete. Phase 3.G has not started.

## Boundary

`papacc_secure_reference_client` is a proof-only executable launched as a
process separate from the real 3.E server fixture. It is not an SDK or product.
Its compile/link boundary contains only public PST 0.6.1 headers and libraries,
the PST public Win32 socket helper, WinSock/Crypt32, public portable scalar
types, frozen PACC wire bytes and generic test-only PKI loading. It does not
include or link server controllers, Principal resolution, authorization,
Session/Channel managers, secure DATA gates or private PST/provider headers.

The parent server fixture and child coordinate shutdown through named Win32
events with bounded waits. No sleep-based startup assumption is used.

## Interoperability evidence

The child process proves, through client-visible TLS and wire results:

- TLS 1.3 mTLS with exact ALPN `papacc/1`;
- `CONTROL_OPEN` → exact `CONTROL_ACCEPT`;
- `DATA_TICKET_REQUEST` → exact 16-byte opaque `DATA_TICKET`;
- a second independent TLS connection and `DATA_ATTACH` → `DATA_ACCEPT`;
- replay rejection;
- credential B rejection against credential A's Session without consuming the
  ticket, followed by successful retry with credential A;
- TLS 1.2, wrong ALPN and missing-client-credential rejection;
- TLS-valid `NOT_ENROLLED` and authorization-denied application establishment;
- cooperative DATA close and clean server-initiated CONTROL close during the
  bounded server shutdown.

Success is determined only by PST public results, peer summary, negotiated
ALPN, PACC response bytes, clean-close facts and process exit code.

## Legacy-profile compatibility

The required CLIENT mask is checked as a subset of the published RetroZilla
NSS CLIENT mask `0x00047ab7`. It requires graceful shutdown, uses
`PST_SNI_MODE_COMPAT`, and does not require `PST_CAP_SNI_CONTROL`.

```text
REFERENCE_CLIENT_PROFILE_NT4_NSS_COMPATIBLE=YES
REFERENCE_CLIENT_REQUIRES_SNI_CONTROL=NO
REFERENCE_CLIENT_USES_SNI_COMPAT=YES
NT4_RUNTIME_INTEROP_EXECUTED=NO
reason=no usable NT4/VC6/NSS runtime environment was present in this workspace
```

The current process-isolated run uses `win32-x64-msvc-19.51-openssl3` and
retains the prior PST 0.6.1 NT4 runtime evidence as the platform proof.

No PACC wire change, public client API, Browser integration, PST modification,
`TLS_OFFLOAD`, network egress, credential storage or production CLI was added.

## Validation closeout

The MSVC security configuration built with `/W4` and completed all 52 CTest
entries. The non-security baseline built and completed all 42 CTest entries.
The complete process-isolated interoperability scenario also passed five
consecutive executions with the same CONTROL, DATA, replay, foreign-credential,
rightful-retry and compatibility results.

```text
SECURITY_CTEST=52/52 PASS
BASELINE_CTEST=42/42 PASS
REFERENCE_CLIENT_INTEROP_STABILITY=5/5 PASS
BUILD_WARNINGS=0
```
