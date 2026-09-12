# Phase 3.G — Security Integration and Final Audit

Status: complete. Phase 3 is READY. Phase 4 has not started.

## Scope and conclusion

The integrated Phase 3 system was audited against all accepted transversal and
local ADRs, the public PST 0.6.1 contract, implementation, tests, build graph,
package metadata and living documentation. No accepted-ADR conflict, wire
change, PST change or new architectural decision was required.

The production executable still lacks an operational credential source and
Secure-listener CLI wiring. The complete Secure Principal server pipeline is an
opt-in private controller with real process-isolated interoperability evidence;
this is the intended Phase 3 boundary and is not a security claim about the
default executable. Legacy Endpoint also has no complete production surface.

## ADR conformance matrix

| Requirement | Implementation | Test evidence | Documentation | Result |
|---|---|---|---|---|
| Secure Principal and explicit Legacy separation | listener configuration and `server_secure_io_win32` routing | server configuration and secure integration tests | ADR-0001, ADR-0009, transport profiles | PASS |
| Portable core/backend boundary | PST types confined to private security/server units | baseline plus security build graph | ADR-0002, architecture | PASS |
| Connection/Session/Channel ownership | managers plus secure controller cleanup/reap | lifecycle, DATA and server integration tests | ADR-0003, server integration | PASS |
| Nonblocking bounded fair scheduling | `pst_secure_scheduler` and controller dispatch | scheduler fairness and server integration tests | ADR-0004, I/O scheduling | PASS |
| Capability/policy authority | AuthZ gates remain separate; Transport Security is not negotiated | AuthN/AuthZ and Secure DATA tests | ADR-0005, capability documents | PASS |
| TLS 1.3 mTLS profile | `security_composition` exact PST policy | composition, real mTLS and process client tests | ADR-0006, security model | PASS |
| Release-pinned PST | pin, acquisition script and `papacc_pst_consumer` | package/manifest/hash audit and security build | ADR-0008, PST 0.6.1 handoff | PASS |
| Immutable profile per listener | normalized listener model | listener configuration and secure server tests | ADR-0009 | PASS |
| Opaque security reference resolved before RUN | server security resolver/composition boundary | resolver failure and lifecycle tests | ADR-0010 | PASS |

The transversal requirements for factual targets, structured consumer-owned
logging, Shared Configuration Model, living capability documents, mandatory
governance loading, portable boundaries, role-neutral identity/trust concepts
and honest provider capabilities are also satisfied.

## Final security invariant matrix

| Invariant | Implementation | Test | Result |
|---|---|---|---|
| TLS 1.3 only | exact min/max in Security Composition | composition + real mTLS + reference client | PASS |
| mTLS required | local identity, peer certificate and custom trust required | real mTLS negatives | PASS |
| ALPN `papacc/1` | required exact ALPN policy | missing/wrong ALPN rejection | PASS |
| no downgrade | immutable listener profile and candidate-local failure | plaintext/malformed TLS negatives | PASS |
| stable Principal | resolver output copied into bounded opaque value | rotation maps distinct certificate to same Principal | PASS |
| AuthN/AuthZ separation | peer evidence → resolver → authorization provider | NOT_ENROLLED, DISABLED, DENY and ERROR | PASS |
| secure CONTROL | TLS/AuthN/AuthZ before classifier and CONTROL_OPEN | process-isolated CONTROL | PASS |
| secure DATA | independent TLS plus Principal/policy gate | process-isolated DATA | PASS |
| foreign Principal cannot burn ticket | inspect/check/authorize/revalidate/commit | foreign B then rightful A retry | PASS |
| replay rejected | one-time exact commit | replay attempt | PASS |
| graceful bounded shutdown | incremental PST shutdown and monotonic deadline | cooperative and stalled shutdown matrix | PASS |
| exactly-once transport close | ownership transfer to PST after attach | ownership/failure integration tests | PASS |
| candidate failure isolation | local close versus structural propagation | TLS/AuthN/AuthZ/protocol negative matrix | PASS |
| fairness | bounded rotating scheduler passes | readiness/fairness regressions | PASS |
| no lost bytes | movable reader and secure plaintext adapter | fragmented CONTROL and DATA | PASS |
| reference client boundary | separate executable using public PST CLIENT API | CTest and 5-run gate | PASS |
| NT4-compatible client profile | required mask subset of `0x00047ab7`, COMPAT SNI | mask assertions and prior PST 0.6.1 evidence | PASS (profile only) |
| wire unchanged | existing registry/codecs | baseline and secure interoperability | PASS |

## CONTROL and DATA order

```text
CONTROL: TCP → TLS 1.3 mTLS → ALPN → peer evidence → Principal → AuthZ
         → secure plaintext adapter → classifier → CONTROL_OPEN → Session
         → CONTROL_ACCEPT

DATA:    independent TCP/TLS/AuthN → DATA_ATTACH inspect → Session context
         → Principal equality → DATA AuthZ → revalidated ticket commit
         → Channel bind → DATA_ACCEPT
```

The classifier never receives ciphertext and is not invoked for pre-PACC
security failures. Candidate TLS, AuthN, AuthZ and protocol failures close only
that candidate. Listener and scheduler structural failures remain reportable.

## Ticket and ownership audit

Tickets remain opaque 16-byte structural values, one-time, monotonic-expiring,
one outstanding per Session, and are neither Principal, credential nor bearer
authorization. A successful attach consumes exactly once; mismatch, DENY and
provider ERROR preserve a still-valid ticket.

| Resource | Owner |
|---|---|
| logical listener | Accelerator |
| native listener socket | Accelerator |
| accepted socket before transfer | Accelerator |
| accepted connected transport after transfer | PST |
| `pst_connection` | secure processor/controller |
| `pst_runtime` | Security Composition |
| Connection/Session Security Context | Accelerator security layer |
| ticket | Association Manager |
| Channel | Channel Manager |
| Session | Session Manager |

The runtime outlives its connections, listeners never become PST-owned and the
connected transport has exactly one close owner.

## Scheduler, maintenance and shutdown

The baseline remains one application-owned I/O thread, readiness-driven, with
no thread per connection and no polling quantum. Work and readiness rounds are
bounded, dispatch rotates fairly, cross-thread wake is supported, and ready
tokens need not be returned in one wait batch.

Maintenance reaps CLOSED manager entries without readiness, is idempotent,
preserves live resources and removes stale bound Channels. Shutdown stops
accepts, removes listeners, advances non-CONTROL secure candidates before
CONTROL, continues readiness for `close_notify`, enforces one monotonic deadline
and publishes stopped only after teardown/reap completes.

## PST, logging, test and wire boundaries

- Pin: PST `0.6.1`, API `2.1.0`, SPI `3.0`.
- Target: `win32-x64-msvc-19.51-openssl3`.
- External SHA-256: `e9965fbcaf6aaa0a96a71bbc38d0b37ca447459641a09a03f1c957a88d764e53`.
- Package manifest, consumer link contract and every internal hash: PASS.
- Only public PST API and public Win32 helpers are consumed.
- No direct OpenSSL, NSS, NSPR or Schannel API dependency exists.
- No checkout, `latest`, patched binary or provider fallback is used.
- Logs do not emit ticket bytes, raw Principal, full fingerprint, DER, key,
  trust anchor, password/PIN/token, payload or native handles.
- `security_test_support`, reference client and PKI fixtures remain test-only;
  no production install/package rule includes them.
- Wire IDs remain `0x0001` through `0x0006`; ticket size remains 16 bytes.

## Validation

```text
BASELINE_MSVC_CTEST=42/42 PASS
SECURITY_OPT_IN_MSVC_CTEST=52/52 PASS
REFERENCE_CLIENT_COMPLETE_SCENARIO=5/5 PASS
NOT_RUN=0
UNEXPECTED_FAILURES=0
W4_NEW_WARNINGS=0
GIT_DIFF_CHECK=PASS
PST_INTERNAL_HASHES=PASS
PST_EXTERNAL_SHA256=PASS
```

The current target was built with the MSVC 19.51 toolchain family, Ninja and
Windows SDK 10.0.26100.0. No stale MinGW cache was used for this proof.

## NT4 evidence scope and residual limitations

Prior PST 0.6.1 evidence covers real Windows NT 4.0/RetroZilla NSS TLS 1.3,
mTLS, CLIENT ALPN and graceful shutdown. The Accelerator reference-client mask
is a subset of NSS CLIENT `0x00047ab7`, requires graceful shutdown, uses
`PST_SNI_MODE_COMPAT` and does not require `PST_CAP_SNI_CONTROL`.

Real NT4 interoperability against this current Accelerator build was not run
because no usable NT4/VC6/NSS runtime environment was present. Accepted Phase 3
criteria require preservation of the compatible profile, not a new NT4 run, so
this remains a factual residual validation item rather than a blocker.

Other explicit non-features are operational Secure credential/configuration
sources, production Secure CLI wiring, complete Legacy Endpoint exposure,
pairing/enrollment, Browser integration, capability negotiation, `TLS_OFFLOAD`,
network egress and post-DATA application payload. None is silently claimed.

## Closeout markers

```text
PHASE3_TLS13_ONLY=YES
PHASE3_MTLS_REQUIRED=YES
PHASE3_ALPN_PAPACC_1_REQUIRED=YES
PHASE3_NO_DOWNGRADE=YES
PHASE3_STABLE_PRINCIPAL=YES
PHASE3_AUTHN_AUTHZ_SEPARATED=YES
PHASE3_SECURE_CONTROL=YES
PHASE3_SECURE_DATA=YES
PHASE3_FOREIGN_PRINCIPAL_CANNOT_BURN_TICKET=YES
PHASE3_REPLAY_REJECTED=YES
PHASE3_GRACEFUL_SHUTDOWN_BOUNDED=YES
PHASE3_CANDIDATE_FAILURE_ISOLATED=YES
PHASE3_EXACTLY_ONE_CONNECTED_TRANSPORT_CLOSE=YES
PHASE3_NO_LOST_BYTE=YES
PHASE3_REFERENCE_CLIENT_INTEROP=YES
PHASE3_NT4_CLIENT_PROFILE_COMPATIBLE=YES
PHASE3_WIRE_UNCHANGED=YES
PHASE3_PST_PRIVATE_API_DEPENDENCY=NO
PHASE3_PLAINTEXT_FALLBACK=NO
PHASE3_TEST_PKI_IN_PRODUCTION_PACKAGE=NO
```

Phase 4, PapinhoBrowser integration, `TLS_OFFLOAD`, network egress and PST/wire
changes were not started by this audit.
