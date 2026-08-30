# Phase 3 Transport Profiles Decision

Status: architectural decision recorded after the successful 3.A2B-R3 backend
closeout. This document defines direction only. Neither profile, listener,
security runtime nor `PapinhoSecureTransport` is implemented here, and Phase
3.B remains not started.

The PACC protocol does not acquire separate "secure" and "insecure" wire
versions. The same protocol may eventually be carried by two explicitly
configured transport profiles. A connection is assigned to a profile by
listener/configuration, never by failed TLS negotiation or payload sniffing.

## Secure Principal

```text
TCP
  -> TLS 1.3 mTLS
  -> authenticated cryptographic Principal
  -> PACC
  -> CONTROL / DATA
```

This is the normal, secure and default production direction. TLS 1.3 and mTLS
are mandatory. Each device has an individual certificate; successful
authentication resolves a stable internal Principal that is separate from
certificate/key bytes. Authorization remains a distinct decision. A secure
DATA association must have the required relationship with the Principal of
the CONTROL Session. TLS, identity or integrity failure is fatal and cannot
fall back to plaintext.

## Legacy Endpoint

```text
TCP
  -> PACC
  -> CONTROL / DATA
```

This is a possible future compatibility profile for a platform that cannot run
modern TLS. It has **no Transport Security on the client-to-Accelerator hop**
and makes no claim of strong cryptographic identity. It must be explicitly
enabled, disabled by default, visibly warned about, and subject to its own
potentially stricter authorization policy. An IP address, TCP endpoint,
password sent in plaintext or structural DATA ticket must not be represented
as secure authentication or as a Secure Principal.

`LEGACY ENDPOINT` means an explicitly authorized legacy endpoint without
strong cryptographic identity. It is not equivalent to `SECURE PRINCIPAL`, and
it must never be selected automatically after any TLS/security failure.

## Listener separation and downgrade rule

The recommended topology is:

```text
Secure Listener -> TLS 1.3 mTLS required
Legacy Listener -> plaintext explicitly configured
```

Distinct listeners are preferred over TLS/plaintext auto-detection on one
socket. A Legacy Listener should preferably use a dedicated port, interface,
network or VLAN. This makes downgrade impossible by negotiation, reduces
ambiguity, improves audit logging and permits network isolation. Concrete
listener configuration and policy remain future work.

```text
NO AUTOMATIC DOWNGRADE: SECURE PRINCIPAL -> LEGACY ENDPOINT
```

## TLS offload is independent

```text
Transport Security != TLS_OFFLOAD
```

Transport Security concerns the Papinho client-to-Accelerator hop.
`TLS_OFFLOAD` is a future capability for external client-to-site/service TLS.
For example, a future Windows 3.11 client might use an explicitly isolated
plaintext Legacy Endpoint and ask the Accelerator to use modern TLS toward an
Internet service. That does not make the first hop confidential, authenticated
or secure, and capability negotiation cannot change its transport profile.

## Future PapinhoSecureTransport boundary

`PapinhoSecureTransport` is planned as an independent portable secure-transport
library. Its first mechanism is TLS and the first proven legacy backend is
RetroZilla NSS/NSPR. It will wrap mature cryptographic backends; it will not
implement AES, ChaCha20, Poly1305, X25519, HKDF, certificate validation or a TLS
state machine itself.

The future boundary must provide opaque backend handles, explicit ownership,
incremental nonblocking handshake and I/O, backend-specific readiness, peer
identity results, clean-close/truncation distinction and fail-closed errors.
NSS/NSPR types such as `PRFileDesc` must not cross its public API.

Only Secure Principal connections pass through `PapinhoSecureTransport`.
Legacy Endpoint remains directly on Transport/PACC and must not be disguised as
a no-crypto backend inside the secure-transport library.
