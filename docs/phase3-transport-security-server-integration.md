# Phase 3.E — Transport Security Server Integration

Status: complete as an opt-in Win32 server integration boundary. Phase 3.F
subsequently validated it from a separate reference-client process.

## Implemented boundary

`server_secure_io_win32` owns one PST readiness scheduler spanning native
listener sources and accepted secure connections. A `SECURE_PRINCIPAL`
listener performs TLS 1.3 mTLS and exact `papacc/1` ALPN before peer evidence,
Principal resolution, authorization, Connection publication, framing or PACC
classification. Listener profile and credential resolution follow ADR-0009
and ADR-0010.

Each DATA socket performs an independent handshake. The 3.D gate requires the
DATA Principal to match the CONTROL Session Principal and authorizes before
consuming the one-time ticket. A foreign Principal does not consume the ticket;
replay after successful commit is rejected.

Candidate-local TLS, authentication, authorization and protocol failures close
only that candidate. They do not become listener-fatal, mutate the listener
profile, invoke a plaintext pipeline or retry insecurely. Structural
scheduler/listener failures remain reportable.

Shutdown stops new accepts, wakes the wait set, advances PST close incrementally
and applies a monotonic deadline. `is_stopped()` becomes true only after secure
candidates, protocol slots and manager-owned Connections have been reaped.
Listeners remain Accelerator-owned; accepted connected transports transfer
exactly once to PST.

## Validation

`server_secure_io_win32_integration_test.c` uses PST 0.6.1, shared test-only
PKI support, real loopback TCP and the real scheduler, controller, AuthN/AuthZ
and PACC processors. It covers CONTROL and DATA, fragmented input, replay,
foreign-Principal preservation, TLS 1.2, missing/mismatched ALPN, missing client
certificate, `NOT_ENROLLED`, authorization denial, malformed TLS, plaintext
PACC, post-auth protocol rejection, healthy-peer isolation and bounded
cooperative shutdown.

`security_test_support.*` is test infrastructure only. Fixture certificates and
private keys are not part of a production package or configuration source.

This phase does not expose a secret-bearing CLI, change PACC wire bytes, start
Phase 3.F, implement `TLS_OFFLOAD`, network egress, Browser integration or a
Legacy Endpoint.
