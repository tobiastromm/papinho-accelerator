<!-- SPDX-License-Identifier: MPL-2.0 -->

# PapinhoSecureTransport 0.6.1 integration handoff

Status: published bugfix baseline adopted by the private, opt-in
PapinhoAccelerator security integration.

## Release identity

```text
PST_RELEASE_REPOSITORY=https://github.com/tobiastromm/papinho-secure-transport
PST_RELEASE_TAG=v0.6.1
PST_RELEASE_COMMIT=56c51e75ca2584cc4e7dc44f16f37cf64b34d396
PST_RELEASE_URL=https://github.com/tobiastromm/papinho-secure-transport/releases/tag/v0.6.1
LIBRARY_VERSION=0.6.1
API_VERSION=2.1.0
SPI_VERSION=3.0
```

The Accelerator pins the exact public OpenSSL target
`papinho-secure-transport-0.6.1-win32-x64-msvc-19.51-openssl3.zip` with SHA-256
`e9965fbcaf6aaa0a96a71bbc38d0b37ca447459641a09a03f1c957a88d764e53`.
Its link libraries are `papinho_secure_transport.lib`, `libssl.lib`,
`libcrypto.lib`, `ws2_32.lib`, and `crypt32.lib`; its runtime files are
`libssl-3-x64.dll` and `libcrypto-3-x64.dll`. Do not consume `latest`, a PST
checkout, or an unverified local build.

## Graceful-shutdown feature contract

`PST_TLS_POLICY.require_graceful_shutdown` accepts
`PST_FEATURE_DISABLED=0`, `PST_FEATURE_OPTIONAL=1`, and
`PST_FEATURE_REQUIRED=2`. These values declare provider eligibility; they do
not start shutdown, impose a deadline, or make release perform implicit TLS
shutdown. The consumer invokes incremental shutdown and owns deadlines.
Abrupt EOF remains truncated and provider fallback after binding is forbidden.

`PST_CAP_GRACEFUL_SHUTDOWN=0x00040000` is part of
`PST_CAP_KNOWN_MASK=0x0007ffff`. REQUIRED makes it mandatory before binding;
OPTIONAL and DISABLED do not. All validated OpenSSL, Schannel, and RetroZilla
NSS CLIENT/SERVER roles advertise the capability factually.

## Validation and Accelerator return

The published release evidence records:

```text
SERVER_CREATE_WITH_REQUIRED=PASS
CLIENT_CREATE_WITH_REQUIRED=PASS
TLS13_MTLS_WITH_REQUIRED=PASS
GRACEFUL_SHUTDOWN_REQUIRED=PASS
CLEAN_CLOSE=PASS
TRUNCATED_CLOSE=PASS
INVALID_FEATURE_VALUE_REJECTED=PASS
OPENSSL=PASS
SCHANNEL=PASS
RETROZILLA_NSS=PASS
NT4=PASS
CLEAN_MACHINE=PASS
PACKAGE_VALIDATION=PASS
CLEAN_SDK_CONSUMER=PASS
ASSET_HASH_VERIFIED=PASS
```

PapinhoAccelerator Phase 3.C may retain `PST_FEATURE_REQUIRED` without a
workaround and must continue to invoke shutdown explicitly.

The autonomous Phase 3.C loopback harness now validates real OpenSSL-provider
TLS 1.3 mTLS, required `papacc/1` ALPN, copied SERVER peer evidence, all five
AuthN/AuthZ scenarios, and cooperative incremental graceful shutdown. Its
CLIENT required-capability mask is a subset of the published RetroZilla NSS
0.6.1 CLIENT mask `0x00047ab7`, uses `PST_SNI_MODE_COMPAT`, and does not require
`PST_CAP_SNI_CONTROL`. The shared PKI/loading helpers remain test-only. Phase
3.E additionally consumes the released SDK through the private boundary in a
real Win32 secure-listener controller and validates decrypted PACC CONTROL/DATA
over independent TLS 1.3 mTLS connections. Executable CLI wiring remains out
of scope.

PST GRACEFUL SHUTDOWN FEATURE CONTRACT FIX RELEASE READY FOR PAPINHOACCELERATOR
