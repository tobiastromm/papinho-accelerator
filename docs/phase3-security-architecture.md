# Phase 3 Security Architecture and Threat Model

Status: authoritative Phase 3.A1 architecture checkpoint. It builds on the
validated `PHASE 2 READY` baseline; it does not change Phase 2 bytes or claim
that security is implemented. Its previously open mechanism questions are now
resolved for the initial profile by [Phase 3 Initial Transport Security and
Credential Profile](phase3-transport-security-profile.md).

## Non-negotiable rules and present baseline

PapinhoAccelerator **MUST NOT invent a custom cryptographic protocol**. Future
confidentiality, integrity and peer authentication must use standard, reviewed
protocols and primitives. This checkpoint selects no protocol, version,
algorithm, library, credential, key store or wire representation.

Phase 2 provides structural Session/Channel association, syntactic framing
validation, one-time structural DATA tickets, lifecycle isolation and resource
limits. It does **not** provide confidentiality, cryptographic integrity,
server or client authentication, authorization, credential protection, MITM
protection, cryptographic replay protection or secure peer identity. Session
`ACTIVE` currently means structural Control establishment only.

The Phase 2 registry remains unchanged:

```text
0x0001 CONTROL_OPEN            0x0004 DATA_TICKET
0x0002 CONTROL_ACCEPT          0x0005 DATA_ATTACH
0x0003 DATA_TICKET_REQUEST     0x0006 DATA_ACCEPT
```

No security message or ID is assigned here.

## Security boundary and layering

```text
Transport -> Transport Security -> Framing -> Protocol Messages -> Session
```

Transport Security protects each PapinhoAccelerator client/server byte stream.
It is infrastructure below framing, not an ordinary negotiable capability.
Consequently the receive pipeline must eventually become:

```text
TCP accept
    -> incremental Transport Security establishment
    -> decrypted secure stream
    -> Connection Classifier
    -> Framing and Protocol Messages
```

The classifier must not see cleartext before required Transport Security has
succeeded. The current `PAPACC_SERVER_PROTOCOL_SLOT_WIN32` will therefore need
an outer security-establishment composition state before `CLASSIFIER`; crypto
internals do not belong in portable Connection entities.

```text
Transport Security != TLS Offload
```

Transport Security protects CONTROL and DATA connections between client and
Accelerator. Conceptual `TLS_OFFLOAD` is a future processing capability for
TLS on an external client-to-site/service operation. Disabling it cannot
disable Transport Security, and capability negotiation cannot weaken Session
security policy.

Every distinct TCP CONTROL or DATA connection must independently establish
Transport Security. A secure CONTROL socket does not secure a separate DATA
socket. A future standard multiplexed secure transport could revisit topology,
but cannot remove the required properties.

Portable upper layers must continue to contain no `SOCKET`, `HANDLE`, Schannel
or Win32 crypto handles, or `Windows.h`. A future abstraction must support
incremental context initialization/readiness, decrypted reads, plaintext
writes, `WOULD_BLOCK`, clean close, fatal security failure and peer-authentication
result. It must preserve nonblocking sockets, the single `select()` scheduler,
bounded work per slot, round-robin fairness, bounded buffers and backpressure;
it must contain no blocking handshake, write-all loop or unbounded handshake
loop. Server and client may use different backends while implementing the same
standard interoperable security protocol.

## Assets

| Asset | Why it matters |
|---|---|
| Control messages | They create and govern Sessions, policy and future work. |
| DATA messages and future acceleration payloads | They may contain private content and high-value computation inputs/results. |
| Future external network requests | They may disclose destinations/data or exercise the server's public IP. |
| Session and DATA association | Corruption permits cross-client attachment, hijack or lifecycle confusion. |
| Client identity | It is the basis for accountability and authorization. |
| Server identity | It prevents a client from sending secrets/work to an impostor. |
| Authentication credentials | Disclosure enables impersonation. |
| Authorization policy | Modification can grant capabilities, egress or excess resources. |
| Cryptographic keys | Disclosure or misuse defeats protected channels and identity proofs. |
| Future capability negotiation | Tampering can enable forbidden work or induce downgrade. |
| Future offloaded TLS material | External-session keys/plaintext would be especially sensitive and is distinct from protocol Transport Security. |

## Trust boundaries and actors

```text
Client process <-> client local OS
       |              (endpoint trust boundary)
       v
untrusted network / other LAN hosts
       v
Accelerator TCP endpoint
       v
Transport Security boundary
       v
protocol/session runtime
       v
future compute and network backends
```

The local server administrator is trusted to configure and operate the server;
remote clients and other LAN hosts are untrusted until authenticated and still
constrained after authentication. A passive observer captures packets and
metadata. An active MITM can intercept, modify, drop, inject, reorder, replay
or redirect traffic. An unauthenticated remote client can open many sockets and
send arbitrary bytes. An authenticated but unauthorized client has a valid
identity but requests forbidden actions. A compromised client may use its
legitimate credentials; the server must still enforce policy, isolation,
quotas, egress and capability boundaries, but cannot make that endpoint's
secrets or authorized actions trustworthy.

Endpoint administrator/root compromise, memory extraction from a fully
compromised endpoint, malicious replacement of the executable and physical
hardware compromise are out of Phase 3 scope. These exclusions do not relax
network protections.

## Required properties

| Property | Required? | CONTROL? | DATA? | Mechanism selected? | Responsible phase |
|---|---:|---:|---:|---:|---|
| Confidentiality | Yes | Yes | Yes | No | 3.A2/3.E |
| Cryptographic integrity/anti-tampering | Yes | Yes | Yes | No | 3.A2/3.E |
| Server authentication | Yes | Yes | Yes | No | 3.A2/3.C/3.E |
| Client authentication | Yes for protected production use | Yes | Yes | No | 3.A2/3.C |
| Authorization | Yes | Yes | Yes | No | 3.C/3.D |
| Replay resistance | Yes where semantics require | Yes | Yes | No | 3.A2/3.C/3.D |
| Downgrade resistance | Yes | Yes | Yes | No | 3.A2/3.C |
| DATA-to-Session identity binding | Yes | N/A | Yes | No | 3.D |
| Credential non-disclosure | Yes | Yes | Yes | No | 3.A2--3.G |
| Fail-closed operation | Yes | Yes | Yes | No | 3.B--3.G |

Server authentication must let the client establish that it reached the
intended Accelerator. Client authentication must resolve which authorized
client is connecting before protected functionality is granted. A source IP,
TCP connection or knowledge of a DATA ticket is not authentication.
Authentication answers “who”; authorization independently answers “what may
this identity do”. Authorization gates include server access, Session creation,
DATA attachment, capability use, network egress and resource limits. In
particular, using Accelerator compute does not imply permission to use its
public IP, and authentication does not authorize every future capability.

A credential is proof material; a principal is the resulting internal
authenticated identity. It is not password/key/certificate bytes, Wire Session
ID, Connection ID or Channel ID. Multiple clients need independent contexts,
and one principal may own multiple Sessions unless later policy forbids it.
Conceptual security-layer states are `UNAUTHENTICATED`, `AUTHENTICATING`,
`AUTHENTICATED` and `FAILED`; they need not become Session enum values.

## Session publication and CONTROL recommendation

The production direction is fail closed: a Session must not become `ACTIVE`
until required Transport Security, authentication and authorization have fully
succeeded. This is compatible with the runtime model by keeping an internal
Session `ESTABLISHING` while security gates run; no runtime change occurs in
3.A1. Publication may occur internally as non-usable `ESTABLISHING` state when
needed for ownership/cleanup, but no observer may treat it as authenticated,
authorized, secure or usable before atomic success.

Later protocol design should use choice **B: required authentication and
authorization complete before `CONTROL_ACCEPT`**. It gives `CONTROL_ACCEPT` a
clear usable-Session milestone and prevents ticket issuance before trust.
Because Phase 2's current bytes and semantics are preserved, 3.C must design a
dedicated, explicitly versioned evolution rather than silently reinterpret the
existing exchange. No wire decision is made here.

Required security or authentication failure creates no authenticated Session,
no authorized DATA attach and no protected capability access. Required security
must never silently fall back to cleartext or a weaker/obsolete mode. Production
must be secure by default. 3.A2/3.C must explicitly decide whether an insecure
development-only mode exists, whether localhost differs, and how per-client
policy works; any exception must be explicit, isolated and impossible to reach
by failed negotiation.

## DATA association

The current 16-byte ticket is structural association material only: not client
identity, credential, bearer authorization token or proof of secure ownership.
Its sequential `PAPACC_SERVER_DATA_TICKET_GENERATOR` is deliberately replaceable
and non-security; it is not cryptographic randomness.

A future DATA attach succeeds only when the DATA connection and target Session
have authenticated security contexts, their identities/authorization
relationship is valid, and the structural ticket is valid. The strong design
candidate is the same authorized principal on DATA and CONTROL:

```text
DATA TCP Connection
    -> Transport Security
    -> authenticated peer principal
    -> DATA_ATTACH structural ticket
    -> same-principal and authorization validation
    -> DATA Channel bind
```

Ticket secrecy, cryptographic ticket binding or replacement generation remains
for later design. Transport, handshake, association and application-message
replay are separate threats; a one-time structural ticket addresses only a
limited association replay case.

Phase 3 must gate `DATA_TICKET_REQUEST` before the Post-Control Processor can
issue a ticket, and extend the DATA Attach Processor's effective ticket +
`ACTIVE` + CONTROL-bound condition with identity and authorization binding.
Neither processor changes in 3.A1.

## Security context and configuration boundaries

Keep `PAPACC_SESSION` and `PAPACC_CONNECTION` minimal. Prefer separate Session
Security Context and Connection Security Context aggregates, keyed by internal
runtime identities and owned by security/composition. Implementation-specific
handles never enter core entities. Failure-atomic publication applies to these
contexts.

`Configuration Source != Configuration Model`. Credential acquisition and
provisioning are separate from the portable identity/security model.
`PAPACC_SERVER_CONFIG` should remain free of raw secrets and hold, if later
needed, references to separately managed credential configuration. The same
internal management model must eventually serve headless CLI and the future GUI.

Secrets must not be embedded in source, logged, placed in ordinary diagnostics
or casually passed as command-line arguments such as `--password`, `--psk` or
`--secret`. They need explicit ownership, lifetime, clear/reset behavior and no
reuse after shutdown. Concrete zeroization rules wait for concrete key types.
`--log-level off` is operational control, not a security feature: credentials,
private/shared/session keys, raw proofs, full security tokens and protected
plaintext must be absent at every log level.

## Timing, resource use and compatibility

Security establishment needs an explicit PAL-monotonic deadline, reviewed
independently rather than automatically inheriting the Phase 2 15-second value.
Wall time is excluded from protocol deadlines except when a selected standard
mechanism explicitly requires validity time (for example certificates); trusted
wall-clock availability, especially on legacy clients, is a 3.A2 concern.

Unauthenticated connection limits, attempt limits, memory bounds, handshake CPU
amplification and malformed-handshake handling must preserve Phase 2 DoS
discipline. Any required randomness must use a cryptographically secure provider;
the sequential Phase 2 ticket provider does not qualify.

PapinhoBrowser targets systems including Windows NT 4.0. Mechanism selection
must therefore evaluate portable client implementation, CPU, memory, code size,
latency and algorithm availability instead of assuming modern Windows APIs.
This **does not** justify obsolete cryptography. Modern cryptography may be
provided portably, and protocol/version policy must be explicit, fail closed
and downgrade resistant rather than inherit unsafe OS defaults.

Transport Security protects content but normally still exposes IP addresses,
ports, timing and traffic volume.

## Failure and lifecycle matrix

| Failure | Required scope |
|---|---|
| Security failure before classification | Close candidate Connection; create no Session. |
| CONTROL authentication/authorization failure | Close CONTROL; no authenticated/authorized usable Session. |
| Security failure on candidate/established DATA | Close that DATA; preserve CONTROL/Session unless explicit policy requires broader closure. |
| Integrity/compromise failure on established CONTROL | Close CONTROL, Session and every associated DATA connection. |
| Integrity failure on established DATA | Close that DATA; preserve CONTROL/Session. |
| CONTROL clean/security close | Preserve existing CONTROL-loss cascade and Channel Manager ownership. |
| DATA clean/security close | Preserve DATA-loss isolation and Channel Manager ownership. |

Cryptographic integrity/authentication failure is not a framing
`PROTOCOL_ERROR`. Future error taxonomy and logging must preserve that
distinction while unauthenticated errors avoid account/credential disclosure,
username enumeration and exploitable timing detail. Current `PAPACC_RESULT`
categories are adequate for present generic control flow but cannot express
security cause/scope precisely; 3.B should design security-specific internal
categories without leaking backend/native codes or excessive wire detail.

## Threat table

| Threat | Attacker capability | Asset affected | Phase 2 protection | Required Phase 3 mitigation | Residual risk |
|---|---|---|---|---|---|
| Passive sniffing | Capture traffic | Messages, payloads, credentials | None | Confidential Transport Security on every connection | Metadata remains visible |
| MITM modification | Alter/inject stream | Commands, payloads, policy | Syntactic framing only | Integrity, peer auth, fail closed | Drop/delay remains possible |
| Server impersonation | Redirect client | Server identity, client secrets | None | Authenticated server identity and trust bootstrap | Compromised trust anchor/endpoint |
| Client impersonation | Present as client | Client identity, authorization | None | Client authentication and policy | Stolen endpoint credential |
| Ticket observation/replay | Observe/reuse ticket | DATA association | One-time structural consumption | Secure channels, identity binding, replay policy | Compromised authenticated client |
| Connection injection | Open arbitrary sockets/bytes | Runtime availability/state | Parsing, limits, lifecycle | Pre-classifier security, auth and limits | Bandwidth exhaustion |
| Downgrade | Tamper negotiation | All security properties | None | Authenticated explicit version/policy, no fallback | Administrator misconfiguration |
| Credential theft on network | Observe proofs | Credentials/principal | None | Protected non-disclosing authentication | Endpoint memory compromise |
| Malformed handshake DoS | Send pathological inputs | CPU/memory/availability | Socket/runtime limits only | Bounded parser/work/buffers and timeout | Distributed resource pressure |
| Unauthenticated CPU exhaustion | Many handshakes | Availability | Connection limits | Admission/attempt limits and amplification review | Distributed attacks |
| CONTROL hijack | Read/modify/control stream | Session and all DATA | Lifecycle cascade only | Mutual identity requirements, integrity, replay resistance | Compromised endpoint |
| DATA hijack | Attach/modify DATA | Payload and Session association | Structural one-time ticket | Per-DATA security and same-principal authorization | Authorized malicious client |

## Future trust and CONTROL flow

```text
TCP Connection
    -> Transport Security establishment and server authentication
    -> authenticated client principal
    -> decrypted stream / Framing
    -> CONTROL establishment and authorization gate
    -> atomic CONTROL_ACCEPT + Session ACTIVE/usable milestone
```

Exact ordering of client authentication within the standard security protocol
or a future versioned application exchange remains a 3.A2/3.C decision.

## Remaining Phase 3 sequence

1. **3.A2 — Transport Security Mechanism & Credential Model:** select a standard,
   versions, trust/bootstrap, credentials and implementation strategies.
2. **3.B — Portable Security/Identity Runtime Foundation:** result taxonomy,
   principals, policy-facing contexts and abstract nonblocking contracts.
3. **3.C — Authentication & Authorization Protocol Design/Implementation:**
   dedicated wire freeze, CONTROL gate and version evolution.
4. **3.D — Secure DATA Association Binding:** authenticated principal binding,
   ticket policy and replay behavior.
5. **3.E — Transport Security Implementation & Server Integration:** place the
   security state before classifier and integrate lifecycle/scheduling.
6. **3.F — PapinhoBrowser Consumer Integration:** portable legacy-compatible
   client backend and provisioning path.
7. **3.G — Security Integration and Final Audit:** adversarial, interoperability,
   lifecycle, logging and regression validation.

## Decisions required from 3.A2

3.A2 must explicitly answer:

1. Which standard Transport Security protocol and allowed versions?
2. Which portable/client-side crypto implementation strategy?
3. Which server-side implementation strategy?
4. How is server identity initially bootstrapped and trusted: certificate trust,
   pinning, pre-shared trust, pairing/provisioning, or another standard model?
5. How is client identity authenticated, and what initial credential type?
6. How are client and server credentials provisioned, stored and rotated?
7. How are DATA connections bound to the authenticated CONTROL Session?
8. What explicit downgrade and version policy applies?
9. Which NT4 constraints materially affect a secure interoperable choice?
10. Is production security mandatory, and can a strictly isolated development
    mode or localhost exception exist?
11. What handshake deadline/admission policy and certificate wall-clock model
    are required?

These questions were the handoff from 3.A1. Phase 3.A2A subsequently froze the
initial mechanism and credential answers in
[Phase 3 Initial Transport Security and Credential Profile](phase3-transport-security-profile.md);
the text above is retained as the historical decision checklist.
