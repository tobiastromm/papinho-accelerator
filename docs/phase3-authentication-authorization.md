<!-- SPDX-License-Identifier: MPL-2.0 -->

# Phase 3.C — Authentication and Authorization Integration

Status: complete as a private, opt-in foundation. It is not connected to the
production server path.

The implementation copies normalized certificate evidence from PST-owned
`pst_peer_info`, then releases that snapshot before resolving identity. A
resolver maps the certificate SHA-256 lookup key to an opaque, stable
Principal. A separate authorization provider decides whether that Principal
may create a CONTROL Session. Connection and Session security contexts receive
the Principal by value only after both steps succeed.

The boundary is fail-closed: missing or malformed evidence, NOT_ENROLLED,
DISABLED, resolver/provider error, and policy DENY publish neither an
authenticated Principal nor an authorized context. A certificate fingerprint
is a credential lookup key, not a Principal; distinct certificates can resolve
to the same Principal during rotation.

## Real integration evidence

The opt-in CTest `papacc_authentication_authorization_real_mtls_test` uses only
the pinned PST 0.6.1 SDK, repository test fixtures, and TCP loopback. It runs
real PST CLIENT and SERVER roles with TLS 1.3, mutual certificate
authentication, custom trust, required ALPN `papacc/1`, disabled resumption and
early data, and `PST_FEATURE_REQUIRED` graceful shutdown.

The harness validates enrolled/ALLOW, TLS-valid/NOT_ENROLLED, enrolled/DENY,
certificate rotation to the same Principal, and DISABLED credential. It reads
real SERVER peer information, copies the evidence into PAPACC-owned storage,
and completes cooperative incremental shutdown through the bounded readiness
scheduler.

The CLIENT capability requirements are a subset of the published RetroZilla
NSS 0.6.1 CLIENT mask `0x00047ab7`. They include
`PST_CAP_GRACEFUL_SHUTDOWN`, exclude `PST_CAP_SNI_CONTROL`, and use
`PST_SNI_MODE_COMPAT`. This is a compatibility proof of the profile, not a
claim that the host test ran the NSS provider.

The fixtures under `tests/fixtures/security` are test-only, require neither an
OpenSSL CLI nor a PST source checkout at runtime, and are excluded from
production installation and staging.

Phase 3.C does not implement Secure DATA association binding, alter DATA
tickets or DATA_ATTACH, connect PST to `papacc_server`, or change PACC,
CONTROL, or DATA wire behavior.

Later-state note: Phase 3.D now consumes these contexts through a separate
private Secure DATA boundary. It did not embed Principal or security pointers
in the runtime Connection, Session, or Channel entities.
