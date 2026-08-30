# Phase 3 Transport Security and Credential Profile

Status: authoritative Phase 3.A2A-R1 security-profile revision. It supersedes
the initial external-PSK choice made by Phase 3.A2A. The earlier choice and its
backend investigation remain historical evidence; they are not normative.
This document selects no TLS library, adds no implementation and freezes no new
PACC Wire Protocol messages.

Normative terms below apply to the future protected production implementation.

## Normative profile

| Property | Requirement |
|---|---|
| Transport protocol | Standard TLS 1.3 only |
| Authentication | Mutual TLS (mTLS) on legacy and modern platforms |
| Trust model | Papinho private CA or administrator-selected enterprise CA |
| Server credential | Server certificate and private key |
| Client credential | One individual device certificate and private key per client device |
| Forward secrecy | Required through ephemeral key exchange |
| Preferred group | X25519, subject to proof on the NT4/VC6 target |
| Compatibility candidate | P-256 |
| Cipher suites | `TLS_CHACHA20_POLY1305_SHA256` required/preferred; `TLS_AES_128_GCM_SHA256` allowed |
| ALPN | Mandatory exact value `papacc/1` |
| 0-RTT | Disabled |
| Resumption | Disabled/deferred |
| Downgrade | TLS 1.2 and cleartext fallback forbidden |
| DATA authentication | A new, full, independent mTLS handshake |
| DATA association | Same principal as CONTROL, valid ticket and authorization all required |

TLS, PACC framing, Control protocol and software versions are independent
version domains. ALPN identifies the protected Papinho application profile; it
does not replace validation in any other domain.

The same security profile applies to PapinhoBrowser on legacy Windows and to
modern Accelerator deployments. Different conforming TLS backends are allowed;
weaker platform-specific profiles are not. Failure to meet this profile fails
closed. No TLS 1.2 or cleartext retry follows a TLS 1.3 failure.

## Why mTLS supersedes external PSK

| Concern | Per-client external PSK | Selected mTLS profile |
|---|---|---|
| Client identity | Symmetric credential record | Individual device certificate mapped to a principal |
| Server authentication | Same shared relationship can authenticate server | Server certificate chained to configured private/admin CA |
| Server compromise | Exposes all stored client PSKs | Does not disclose client private keys |
| Client compromise | Exposes that client relationship | Exposes only that device key and certificate |
| Enrollment | Requires pre-shared secret delivery | Supports explicit, verifiable certificate enrollment |
| Rotation/revocation | Custom PSK record lifecycle | Certificate renewal/revocation plus principal policy |
| Enterprise operation | Custom provisioning | Can use administrator-managed PKI |
| Backend evidence | RetroZilla NSS lacks required public external-PSK API | Standard TLS certificate authentication is the revised proof target |

The Phase 3.A2B/R2 investigation demonstrated that requiring TLS 1.3 external
PSK would force substantial NSS backport/API work and retained symmetric-secret
exposure at the server. mTLS uses standard TLS authentication semantics, gives
each client device its own asymmetric key, and provides a cleaner foundation
for enrollment, revocation and enterprise administration. This is a profile
revision, not a claim that legacy feasibility has already been proved.

## Trust, credentials and authenticated principal

PapinhoAccelerator uses either a Papinho-specific private CA or an explicitly
configured administrator/enterprise CA. Public Web PKI is not required. The
server holds its server certificate and private key. Each client device holds
its own certificate and private key.

Client private keys are generated on the client and remain there. Server
private keys are generated or imported on the server and remain server-side.
The private-CA signing key remains on a local administrative system, preferably
offline when practical, or is controlled by the external enterprise PKI. The
CA signing key is not a runtime server credential and must not be distributed
to clients.

Successful certificate validation and proof of the corresponding private key
map to a stable internal authenticated principal:

```text
validated trust chain + certificate policy + private-key proof
    -> credential/device record
    -> stable authenticated principal
```

The principal is deliberately distinct from certificate bytes, public-key
bytes, subject text, serial number, fingerprint, IP address, Connection ID,
Session ID, Channel ID and DATA ticket. Certificates may be renewed or replaced
without changing the principal. Subject strings are presentation metadata, not
authorization identities. Arbitrary valid certificates do not automatically
create principals; enrollment and server policy determine admission.

Authentication proves which enrolled principal controls the connection.
Authorization separately decides whether that principal may create a Session,
attach DATA Channels, consume resources, negotiate capabilities or request
future network egress. Authentication never implies unrestricted access.

## First-use trust and pairing

There is no silent trust on first use. An IP address, DNS name, discovery
result or successful TCP connection is not server identity. PapinhoBrowser must
not silently accept a first server certificate merely because it is reachable.

The browser's first-use experience must show, at minimum:

- the claimed server/presentation name and network endpoint;
- the server identity and issuing trust domain;
- the server certificate SHA-256 fingerprint in a human-verifiable form;
- whether the chain is already trusted or requires explicit enrollment;
- a clear verification action and the actual reason for any failure.

Fingerprint verification must use an independent trusted channel, such as an
administrator-provided display, document or enterprise management channel.
Copying a fingerprint from the same untrusted connection does not authenticate
it.

A pairing code is not a reusable password and cannot stand alone as server
identity. It must be high entropy enough for its intended lifetime, short
lived, single-use, rate limited, and cryptographically bound to:

- the expected server identity/certificate or its SHA-256 fingerprint;
- the current pairing session and transcript;
- the client-generated public key or certificate request;
- the intended trust domain and enrollment purpose.

The design must resist relay: a code observed for server A cannot authorize a
certificate at server B, a later pairing session, or a different client key.
Both endpoints must confirm the same server identity and current pairing
context. Address proximity, LAN membership and source IP are not proof.

### Pairing levels

| Level | Intended use | Required trust action |
|---|---|---|
| End-user pairing | Small/private deployment | Explicit server fingerprint verification plus a bound, expiring pairing code and visible confirmation |
| Server-administrator enrollment | Managed Papinho server | Administrator approves a client-generated certificate request against the verified server/CA identity and assigns a principal/policy |
| Enterprise enrollment | Organizational deployment | Existing enterprise PKI/management authenticates server and device, issues certificates and applies authorization policy |

For Level 1, the ordinary user selects the Accelerator, chooses Pair, sees the
server identity/certificate and a verification code or SHA-256 fingerprint,
compares it through an independent channel, and explicitly confirms. Enrollment
may then complete automatically and later connections are automatic; the UI
need not require X.509 terminology. Level 2 exposes the pending device and the
same bound code/fingerprint to the server administrator for an explicit
Authorize/Reject decision while the client still verifies server identity.
Level 3 permits manual certificate import/export, scripts, deployment packages,
configuration-management systems, pre-provisioned trust anchors/certificates,
POLEDIT-compatible policy on legacy Windows where practical, and Group Policy
/ GPO on modern domain Windows. No POLEDIT/GPO integration is implemented here;
the credential/trust model merely remains compatible with future automation.

Conceptual enrollment flow:

```text
client generates private key locally
    -> client verifies server/CA identity independently
    -> client submits public-key enrollment request in a bound pairing context
    -> authorized issuer approves and signs a device certificate
    -> server maps the enrolled credential to a stable principal and policy
    -> client stores certificate, key and expected trust anchor
```

This is an enrollment architecture, not a Wire Protocol design. No `PACC`
pairing message, numeric message ID, payload encoding, storage format, GUI or
pairing implementation is defined in Phase 3.A2A-R1.

An unpaired client receives no normal CONTROL Session, DATA ticket, DATA
Channel, capability access or network egress. The Accelerator may retain client
certificates/public keys, principal mappings, issuer/trust information and
revocation/authorization metadata, but must never require or store client
private keys.

## CONTROL and DATA security flow

Every accepted TCP connection completes TLS before any PACC frame reaches the
classifier.

```text
CONTROL TCP accept
    -> TLS 1.3 mTLS, ALPN papacc/1
    -> validated client device credential
    -> stable authenticated principal
    -> decrypted PACC framing
    -> CONTROL_OPEN and separate Session authorization
    -> CONTROL_ACCEPT
```

`CONTROL_ACCEPT` remains the peer-visible success point after required
transport authentication and CONTROL authorization. The current Phase 2
message IDs and payloads do not change.

Each DATA connection performs a separate full mTLS handshake; CONTROL TLS state
is not reused as DATA authentication.

```text
DATA TCP accept
    -> independent TLS 1.3 mTLS, ALPN papacc/1
    -> authenticated DATA principal
    -> decrypted PACC framing
    -> DATA_ATTACH structural ticket
    -> resolve target Session without consuming ticket
    -> DATA principal == CONTROL Session principal
    -> DATA authorization
    -> atomic conditional ticket consumption
    -> DATA_ACCEPT
```

The attach condition is conjunctive:

```text
same authenticated principal as CONTROL
AND valid structural ticket
AND explicit DATA attachment authorization
AND existing Session/CONTROL lifecycle invariants
```

A ticket alone is never identity or authority. A principal mismatch must not
consume a legitimate ticket. Later implementation must supply a failure-atomic
resolve/check/consume operation without changing wire bytes in this revision.

## Cryptographic and extension policy

Forward secrecy is mandatory through ephemeral key exchange. X25519 is the
preferred mandatory-group candidate, but remains conditional on a concrete
NT4/VC6 build and runtime proof. P-256 is the compatibility candidate. The
backend spike must freeze at least one interoperable group without weakening
forward secrecy.

`TLS_CHACHA20_POLY1305_SHA256` is required and preferred for predictable
portable software performance on CPUs without AES acceleration.
`TLS_AES_128_GCM_SHA256` is allowed. Obsolete cipher suites are forbidden.

ALPN `papacc/1` is mandatory. Missing or different ALPN fails the handshake.
0-RTT is disabled because of replay and authorization ambiguity. Resumption is
disabled until a later design freezes principal binding, revocation, expiry and
replay behavior. Thus CONTROL and DATA currently require full handshakes.

## Certificate lifecycle and compromise scope

| Event | Required behavior |
|---|---|
| Client renewal | Issue a replacement certificate for the same principal; bounded overlap may be allowed |
| Client revocation | Reject new handshakes for that credential; define existing-connection policy before implementation |
| Client-key compromise | Revoke that device credential; other client private keys remain unaffected |
| Server renewal | Replace server certificate/key while retaining explicitly configured trust and identity continuity |
| Server-key compromise | Replace/revoke server credential, notify clients and re-establish verified server identity |
| CA rotation | Explicitly distribute and verify a new trust anchor with a bounded transition; never learn it silently from the affected channel |
| CA-key compromise | Treat the trust domain as compromised; revoke/replace CA and issued credentials and require explicit re-enrollment/retrust |
| Malicious enrollment | Revoke the credential and principal authorization; audit the approving actor and pairing context |

Revocation is an authorization/trust operation, not merely deletion of a file.
Certificate rotation must not silently change the authenticated principal or
broaden its policy. Private keys and secret-bearing buffers require explicit
ownership, lifetime, access protection and cleanup in the later backend API.

## Wall clock and certificate validity

Certificate path validation requires a credible wall clock in addition to the
PAL monotonic clock used for deadlines. A missing, invalid or implausible system
time produces an explicit certificate-time/security error and fails closed.
The implementation must not disable `notBefore`/`notAfter` checks to support a
legacy host. Clock remediation is an operational prerequisite, not a security
downgrade.

## Entropy and user-visible failures

Key generation, TLS ephemeral keys, signatures, nonces and pairing secrets
require an approved cryptographic entropy source. Failure or unavailability
fails closed and must be distinguishable from certificate, clock, protocol and
network failures. The RetroZilla-style environmental fallback is explicitly
rejected for PapinhoAccelerator security material.

Phase 3.A2B-R3 must audit rather than assume entropy on every supported target:

| Platform family | Required proof |
|---|---|
| Windows NT 4.0 | Available cryptographic OS source or reviewed bundled source, runtime behavior and failure path |
| Windows 2000 / XP | Same, including API availability and failure propagation |
| Windows 95 / 98 / Me | Explicit source and runtime proof if supported |
| Windows NT 3.x | Explicit source and runtime proof if supported |
| Windows 3.11 / Win32s | Explicit source and runtime proof if supported; otherwise declare unsupported |
| Modern Windows | Current system CSPRNG integration and failure propagation |
| POSIX targets | Platform CSPRNG source, blocking/startup semantics and failure propagation |

PapinhoBrowser must render an internal error page with the actual actionable
failure class and, where truly causal, the actual provider, API or DLL name.
It must distinguish DLL missing, API missing, provider unavailable, provider
initialization failure, secure-RNG failure, unsupported OS, backend failure,
certificate-validation failure and invalid clock rather than guessing or always
blaming a DLL. Failure classes include at least: untrusted/wrong server
identity, client certificate rejected/revoked/expired, certificate not yet
valid, invalid system
clock, ALPN/profile mismatch, unsupported secure backend, entropy failure,
handshake integrity failure and ordinary network failure. Diagnostics must not
leak private keys or unnecessarily reveal server-side enrollment records.

## Threat-model additions

The profile explicitly addresses:

- pairing MITM and silent TOFU through independent fingerprint verification;
- fake-IP identity through certificate-based identity rather than address;
- pairing relay through binding to server certificate, client key and session;
- stolen client key through per-device scope and revocation;
- stolen server key through server credential replacement and retrust handling;
- stolen CA key through trust-domain replacement and re-enrollment;
- malicious enrollment through explicit approval, principal policy and audit;
- ticket theft through independent DATA mTLS, principal equality and
  authorization before atomic ticket consumption;
- downgrade through TLS 1.3-only fail-closed policy.

## Papinho profile, HTTPS and backend boundary

This document defines the PapinhoAccelerator transport-security policy. It does
not redefine general HTTPS behavior used when PapinhoBrowser connects directly
to external sites. `TLS_OFFLOAD`, if implemented later, is a separate
Capability Framework concern and cannot weaken Papinho Transport Security.

```text
PapinhoAccelerator Transport Security != TLS Offload
PapinhoAccelerator mTLS profile != general Web/HTTPS policy
```

A future `PapinhoSecureTransport` component may be justified to isolate
nonblocking TLS mechanics, credential handles, handshake state, secure I/O,
errors and backend adaptation. This revision does not create that project or
freeze its API. Its boundary must remain:

```text
mechanism: TLS engine, certificate operations, secure stream I/O
policy:    Papinho TLS version, suites, ALPN, trust, principal and authorization rules
```

Backends implement mechanism; they do not silently choose or weaken Papinho
policy. No library is selected by this document.

## Revised Phase 3.A2B-R3 proof gates

The next compatibility spike must provide concrete evidence for:

- TLS 1.3 mTLS on the Accelerator and PapinhoBrowser legacy targets;
- certificate-chain validation against private/admin trust anchors;
- client and server certificate/private-key operations without exporting key
  material across the portable boundary;
- ChaCha20-Poly1305, AES-128-GCM, X25519 and P-256 availability/performance;
- mandatory ALPN `papacc/1`, disabled 0-RTT and disabled resumption;
- nonblocking incremental handshake/read/write and custom socket integration;
- clean close, truncation and fatal-error distinctions;
- certificate-time validation and explicit invalid-clock handling;
- the complete platform entropy matrix above, including fail-closed behavior;
- NT4/VC6/C89 build feasibility, footprint, memory, stack and CPU cost;
- license and distribution compatibility;
- a principal-extraction interface that exposes no backend certificate structs
  to Core/Session code;
- browser-facing structured errors sufficient for the internal error page;
- pairing/enrollment primitives only at the conceptual feasibility level, with
  no new PACC wire design.

External PSK support is no longer a proof gate. Historical R2 measurements
remain valid evidence explaining this revision. Phase 3.A2B-R3 may report the
revised profile infeasible with concrete evidence, but may not silently weaken
TLS version, mutual authentication, forward secrecy, trust validation, entropy,
ALPN or downgrade policy.

No production code, dependency, CMake target, pairing flow, Wire Protocol
message or `PapinhoSecureTransport` project is introduced by this revision.
