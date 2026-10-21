# Phase 6 — Network & TLS Offload

Status atual (2026-10-18): Phase 6 integral OPEN / NOT COMPLETE.
O READY/complete do relatório original abaixo qualifica somente o escopo
histórico Network & TLS Offload/Mode B; não fecha a integração canônica posterior
nem Production Secure Provisioning. Phase 7 não é habilitada por esta tarefa.

## Production client provisioning — Phase 6 gates

Obrigação normativa: [decisão expressa do owner](adr/DECISION-production-client-provisioning-20261018.md).
Dois modos permanentes convergem na mesma autoridade/cadastro/issuer,
Principal-policy/grants e CredentialGeneration/Reference/store/leases.

| Entrega obrigatória | Status | Evidência necessária para fechar |
|---|---|---|
| P6-PROVISIONING-MANUAL-PACKAGE | OPEN | Gerador admin e importação Browser reais; formato/issuer/custódia aprovados, integridade/destinatário/servidor/limites e commit transacional, preservando importação granular. |
| P6-PROVISIONING-AUTOMATIC-ENROLLMENT | OPEN | Address, bootstrap verificável, confirmação por código, chave local/CSR-PoP conforme contrato, emissão e commit comuns; negativos/replay/recovery qualificados. |
| P6-PROVISIONING-OPEN-CLOSED | OPEN | OPEN sem login inicial; CLOSED no enrollment valida password dentro de TLS antes de aprovação; políticas ortogonais e sem uso operacional pendente. |
| P6-PROVISIONING-AUTO-MANUAL-APPROVAL | OPEN | Configuração e callers reais; validações completas, decisão manual vinculada ao request exato, sem grants implícitos. |
| P6-PROVISIONING-HOT-UPDATE | OPEN | Add/approve/renew/revoke sem restart na mesma root PREPARE/COMMIT/CURRENT/EPOCH factual; falha/concorrência/recovery/retenção comprovados. Primeiro commit dormente atual não basta. |
| P6-PROVISIONING-REVOCATION-RENEWAL | OPEN | Revoke persistente/deny novas conexões, política aprovada para sessões ativas, renovação/trust rotation/recovery e safe retirement/leases comprovados. |

Implementação e qualificação efetivas são necessárias; fixtures/documentos
não fecham entregas. Alteração de escopo requer decisão normativa explícita.
Não declarar fechamento integral Phase6 com essas obrigações abertas.
Outros gates canônicos, E10/ED11 e qualificação Mode C continuam independentes.
PST0.6.5 só será publicado após fechamento integral da Phase6.

[Matriz e lacunas](integration/production-client-provisioning-consolidation-20261018/RECONCILIATION.md) ·
[Handoff Browser](integration/production-client-provisioning-consolidation-20261018/BROWSER-HANDOFF.md) ·
[Próximo slice manual](integration/production-client-provisioning-consolidation-20261018/NEXT-MANUAL-GENERATOR.md).
Produção064/API2 preservada; candidato065 normal opt-in; R2 TEST_ONLY isolado.
CLIENT_ADMISSION=DISABLED; SITE_TLS_ADMISSION=DENY; ED11_ADMISSION=DENY.

## Relatório histórico do escopo anterior

O texto seguinte preserva resultados/limitações do escopo original. Suas
menções a Browser/Mode C ausentes não substituem o estado canônico posterior em
[canonical-phase6-consolidation](capabilities/canonical-phase6-consolidation.md).

## Scope and result

Phase 6 implements Accelerator-owned outbound connectivity and the first
productive external-TLS placement. It adds initial capability negotiation,
Network Offload messages on a dedicated DATA Channel, bounded establishment
Jobs and Network Streams, explicit egress policy/profile composition, safe DNS
handling, TCP forwarding and outbound TLS through the public
PapinhoSecureTransport API.

It does not implement a generic proxy, Browser integration, Mode C, image
acceleration, firewall/router/NAT behavior or TLS interception.

## Architecture

```text
Secure Principal + Session Capability Snapshot
                    ↓
NETWORK_EGRESS operation-time authorization
          + TLS_OFFLOAD for TLS_REQUIRED
                    ↓
allow_network_egress master gate
                    ↓
Destination Policy — WHETHER the target/port is allowed
                    ↓
Egress Profile — HOW the approved connection exits
                    ↓
one DNS resolution + per-candidate numeric policy
                    ↓
bounded establishment Job
                    ↓ success, failure-atomic transfer
bounded Network Stream owned semantically by Session
```

Capability, policy, backend availability and resource admission remain
separate gates. Missing or ambiguous state fails closed.

## Capabilities

The canonical keys are:

```text
NETWORK_EGRESS = (1, 1)
TLS_OFFLOAD    = (2, 1)
```

`NETWORK_EGRESS` authorizes Accelerator-owned outbound connectivity subject to
contextual policy. `TLS_OFFLOAD` authorizes TLS processing by the Accelerator.
They are orthogonal: Mode B needs both; `TLS_OFFLOAD` does not universally
imply `NETWORK_EGRESS`.

Transport Security protects PACC CONTROL/DATA and is not a negotiated workload
capability:

```text
Transport Security != TLS_OFFLOAD
```

The initial wire negotiation publishes an immutable upper-bound Session
Capability Snapshot. Runtime policy may disable and later re-enable a key that
was already present, but cannot silently add a missing key. Explicit
renegotiation remains deferred.

## Network and TLS placement

| Mode | Network | TLS | Phase 6 status |
|---|---|---|---|
| A | local | local | Preserved outside Accelerator |
| B | Accelerator | Accelerator | Implemented and validated |
| C | local | Accelerator | Future/deferred; not implemented |
| D | Accelerator | local | Foundation preserved by opaque `TCP_PLAIN`; Browser integration deferred |

Mode B is the primary v1 Phase 6 path. The placements remain architecturally
orthogonal.

## Wire registry

The historical assignments are unchanged and the new assignments are governed
by ADR-0016 and ADR-0017:

| ID | Message |
|---:|---|
| `0x0001` | `CONTROL_OPEN` |
| `0x0002` | `CONTROL_ACCEPT` |
| `0x0003` | `DATA_TICKET_REQUEST` |
| `0x0004` | `DATA_TICKET` |
| `0x0005` | `DATA_ATTACH` |
| `0x0006` | `DATA_ACCEPT` |
| `0x0007` | `CAPABILITY_NEGOTIATE` |
| `0x0008` | `CAPABILITY_NEGOTIATE_RESULT` |
| `0x0009` | `NETWORK_OPEN` |
| `0x000A` | `NETWORK_OPEN_RESULT` |
| `0x000B` | `NETWORK_DATA` |
| `0x000C` | `NETWORK_HALF_CLOSE` |
| `0x000D` | `NETWORK_CLOSE` |
| `0x000E` | `NETWORK_ABORT` |

The wire carries no runtime Job, Network Stream or Session ID, Principal
internal ID, backend identity, Egress Profile, interface, source IP, resolver,
trust source or independent SNI selector. One accepted DATA Channel binds one
Network Stream.

## Network Stream lifecycle and ownership

Network establishment is a finite Job. A hidden Stream reservation precedes
socket work; only complete success publishes the Stream. Failure releases the
reservation and every acquired resource.

```text
Session                semantic owner of Jobs and Streams
Job Manager            physical owner of bounded Job records
Network Stream Manager physical owner of bounded Stream records and buffers
outbound connector     socket owner before transfer
PST                    secure transport owner after TLS adoption
Network Stream         owner of the live transport aggregate
```

Storage is caller-owned and bounded. Global/per-Session Stream limits, Job
limits, Capability Store capacity and per-direction TX/RX buffers are explicit.
The portable core requires neither mandatory heap nor thread per Stream.

DATA loss closes exactly its Stream. Session/CONTROL loss cancels all owned
Jobs, closes all owned Streams and removes the capability snapshot. Server
shutdown performs bounded cleanup. Zero-readiness maintenance continues
cleanup/reap without stale slots.

## TCP_PLAIN

`TCP_PLAIN` carries opaque bytes bidirectionally with bounded buffers,
incremental frames and backpressure. Partial socket writes consume only the
accepted prefix; remote reads stop when RX is full.

`NETWORK_HALF_CLOSE` drains pending client TX and performs directional TCP
shutdown while preserving reverse traffic. `NETWORK_CLOSE` is normal terminal
close; `NETWORK_ABORT` is immediate exceptional teardown.

## TLS_REQUIRED / Mode B

Outbound TLS uses only the public PST CLIENT API. The v1 profile requires:

- TLS 1.3 exactly;
- server certificate and chain validation;
- hostname or exact IP identity validation;
- server-owned SYSTEM or CUSTOM trust without fallback;
- normalized DNS hostname as SNI and expected peer name;
- bounded, policy-governed ALPN;
- 0-RTT disabled;
- resumption disabled;
- client certificate disabled;
- nonblocking, readiness-aware handshake with monotonic deadline;
- no plaintext fallback.

The Accelerator does not directly call OpenSSL, NSS, NSPR or Schannel. PST is
release-pinned and consumed through its supported public package boundary.

For `TLS_REQUIRED`, `NETWORK_HALF_CLOSE` is `NOT_SUPPORTED` and does not invoke
PST shutdown. `NETWORK_CLOSE` advances incremental `close_notify` shutdown with
a bounded deadline. `NETWORK_ABORT`, DATA loss and Session loss use immediate
terminal cleanup. EOF without `close_notify` is unclean/truncated.

### TLS terminal lifecycle recertification gate

ADR-0023 records the normative A-I regression matrix for peer-first clean
close, truncation/reset, cancellation/deadline, CONTROL loss, duplicate
terminal, late DATA and exactly-once resource release. Qualification is
per-case; partial existing coverage is not a PASS for the remaining cases.

For duplicate or concurrent terminal events, the first valid terminal
effectively accepted by the operation owner wins. Acceptance follows operation
identity, generation, state, authority, contract and factual-cause validation;
arrival alone grants no precedence. Once accepted, terminal type and cause are
immutable, later events cannot publish or release twice, and the operation
cannot resurrect. CLOSE/CLOSE preserves one CLOSE, ABORT/ABORT preserves one
ABORT, CLOSE/ABORT preserves the accepted CLOSE, and ABORT/CLOSE preserves the
accepted ABORT. The owner serializes concurrent events. A terminal arriving
after teardown is rejected without accessing released objects. Internal
deduplication does not suppress required diagnosis of duplicate or invalid
remote PACC frames.

G qualification therefore requires six real scenarios: G1 CLOSE/CLOSE, G2
ABORT/ABORT, G3 CLOSE/ABORT, G4 ABORT/CLOSE, G5 a competing terminal during
real PST shutdown NEED_WRITE/INTEREST_WRITE, and G6 a terminal after
teardown. Each must correlate event receipt, owner acceptance, publication or
suppression and per-resource release against the real Stream/server I/O loop;
the synthetic counter selftest remains complementary evidence only.

Test preparation now includes `papacc_network_stream_ai_injection_test`.
Its D-I command modes exercise the production Network Stream state machine
through test-only I/O injection and resource observers. The oracle selftest
rejects missing/reused causes, owner-not-reached, duplicate or missing expected
terminal, resource-close-as-terminal, absent `PST_CLOSE_CLEAN`, and invalid
resource-release counts and rejects absent, inherited or owner-divergent
resource causes. A-F are qualified with PST 0.6.5. G1 is also qualified:
the first `NETWORK_CLOSE` was owner-accepted, DATA became `TERMINAL` with no
second wire dispatch, and the serialized owner duplicate was suppressed without
changing terminal or cause. The sole later publication attempt was attributed to
the site's clean close after completed PST shutdown; DATA was already terminal,
the framed writer returned `PAPACC_RESULT_INVALID_STATE`, and no PACC terminal
publication completed. Nine negative oracle mutations were rejected and all four
acquired resources were released exactly once. G remains `NOT_QUALIFIED` until
G2-G6; H-I remain `NOT_QUALIFIED`.
Isolated PST 0.6.5 execution qualified A (reciprocal graceful shutdown and
`NETWORK_CLOSE`) and B (fragmented truncation matrix). The focused
`papacc.phase6.reference.tls_required_tcp_reset` test qualified C: it
established TLS_REQUIRED/PST, exchanged an application byte, injected TCP RST
with `SO_LINGER {1,0}`, observed `TLS_TRUNCATED` and emitted no false
graceful `NETWORK_CLOSE`. Its non-invasive test observer preserved
`stream->io.context`, called the original `io.close` once with the original
context, and observed exactly one successful release of each acquired PST
connection, PST-owned site socket, Network Stream and DATA attachment, with no
remaining reference. D was qualified with the governed PST 0.6.5 test-only
shutdown gate: peer clean close was observed independently, reciprocal shutdown
reached real `NEED_WRITE`/`INTEREST_WRITE`, and server Stop reached owner state
`STOPPED`. Global Stop closes DATA without a PACC terminal frame; the reference
client proved zero terminal frames rather than treating resource close as
publication. PST/socket/Stream/DATA were each released once.

E was qualified with the same governed test-only SDK. Reciprocal shutdown
remained in real `NEED_WRITE`/`INTEREST_WRITE`; the owner observed expiration
of its 5000 ms monotonic deadline and preserved internal cause
`SHUTDOWN_DEADLINE`. The reference client used one non-restarting 7000 ms
budget (5000 ms plus a 2000 ms observation margin), observed exactly one
generic `NETWORK_ABORT`, and did not infer the internal cause from that frame.
The PST connection, PST-owned site socket, Network Stream and DATA attachment
were each released exactly once, and every resource record independently
derived `SHUTDOWN_DEADLINE` from the factual observer state. F reached a real
TLS_REQUIRED peer clean close with the PST 0.6.5 test-only BIO gate, observed
`NEED_WRITE`/`INTEREST_WRITE`, then lost CONTROL after that pending boundary.
The owner preserved `CONTROL_TRANSPORT_LOSS`, performed one dependent DATA
cascade, left zero orphan DATA or resurrection, and released PST connection,
site socket, Network Stream and DATA attachment exactly once. G-I were not
executed and I remains `NOT_QUALIFIED`.

The current slice still reuses the establishment timeout for shutdown. A
dedicated shutdown timeout remains explicit technical debt; the E qualification
does not freeze or change the production duration.
The isolated qualification must use the canonical PST 0.6.5 OpenSSL x64 SDK
package with SHA-256
`0c134e0fc842e22e80dd6b35f6e7f207a7859f097f0be5b7d0be9c08d9d05c67`
from the Accelerator R7 bundle with SHA-256
`dd986b98bb101b3d498dc31107a63c034ef0ba4779592057c4bbf63da3f855cf`.
These artifacts are distinct from the NSS/VC6 Browser package.

Cases G/H/I require focused test-only controls for duplicate terminal, late
DATA and per-resource exactly-once counters. The execution gate is
`PAPINHOACCELERATOR-PST065-A-I-ISOLATED-QUALIFICATION`; it remains separately
authorized. No Browser Mode B result, C0 closeout or C1 authorization is implied.

## Egress policy and profiles

```text
allow_network_egress = master gate
Destination Policy   = WHETHER destination/port/context is allowed
Egress Profile       = HOW an approved connection exits
```

`FALSE` denies before policy. `TRUE` is not allow-all. Missing policy,
`DENY`, `ERROR`, missing/invalid/unavailable profile and mandatory source bind
failure all fail closed. Default route exists only as an explicit profile.
There is no implicit default or silent profile failover. Clients cannot select
profiles or privileged egress resources. Configuration changes affect new
operations and do not migrate active Streams.

## DNS and sensitive destinations

V1 accepts normalized ASCII DNS names and binary IPv4/IPv6 literals.
Unicode/IDNA and malformed names fail closed. Host policy runs before one
injected resolution; every numeric candidate is canonicalized and evaluated
individually. IPv4-mapped IPv6 becomes IPv4 before classification. Connect uses
the exact already-approved numeric endpoint and performs no second DNS lookup.

Unspecified, multicast, applicable broadcast, malformed endpoints and port zero
are structurally denied. Explicit administrative authorization is required for
loopback, link-local, private IPv4, IPv6 ULA/private, CGNAT/shared, metadata,
management and local Accelerator service/listener endpoints. Public unicast is
not an implicit allow and still needs matching Destination Policy.

## Scheduling and fairness

The application-owned I/O loop offers bounded opportunities and rotates across
Streams and Sessions. Establishment Jobs coexist with active Streams;
`TCP_PLAIN`, established TLS and pending TLS handshakes coexist. Normal
`WOULD_BLOCK` never causes an internal busy loop. No thread-per-Stream model is
introduced.

## Independent reference client

The Phase 6 reference client runs as a separate process, uses the public PST
CLIENT API plus portable public PACC framing/codecs, and has no server-private
dependency. It receives no runtime IDs and cannot choose Egress Profile or
server-owned policy inputs.

It validates `TCP_PLAIN`, `TLS_REQUIRED`, 32 KiB binary fragmented traffic,
TCP half-close, TLS close/abort behavior, missing capability, destination deny,
hostname failure, recovery and simultaneous isolated DATA Channels.

## Security and abuse matrix

The portable adversarial matrix and existing integration tests cover:

- missing negotiation/snapshot/capability and contextual deny/error;
- Legacy Endpoint denial;
- master gate, policy and profile failure;
- port/domain/IP and IPv4-mapped IPv6 handling;
- SSRF-sensitive classes and public-unicast policy;
- one-resolution DNS candidate filtering;
- Principal, Session, ticket and DATA isolation;
- bounded/reusable Capability, Job, Stream and buffer capacity;
- malformed/partial wire, disconnect, cancellation, timeout and abort races;
- TLS 1.2-only, trust, hostname and ALPN rejection;
- healthy sibling traffic during denied/failed operations;
- secret-safe logs and bounded wire error categories.

Logs and wire results do not expose payloads, private keys, credentials, full
certificate DER, authorization tokens, raw provider diagnostics or runtime
internal IDs.

## Production composition and fixtures

The PST-enabled server composition receives the shared Configuration Model.
Outbound policy, profile, runtime, provider and trust are server-owned.
Required PST runtime/trust initialization failure aborts startup; egress never
downgrades to plaintext.

Security PKI and trust fixtures are test-only. Their private keys and fixture CA
are not production defaults and are not silently trusted by production
composition.

## Conformidade com ADRs

| ADR | Implementation conforms | Test evidence | Docs conform |
|---|---|---|---|
| ADR-0015 — establishment/Stream lifecycle | YES | YES | YES |
| ADR-0016 — capability negotiation wire | YES | YES | YES |
| ADR-0017 — Network Offload wire | YES | YES | YES |
| ADR-0018 — registry allocation | YES | YES | YES |
| ADR-0019 — policy/profile | YES | YES | YES |
| ADR-0020 — DNS/validated endpoint | YES | YES | YES |
| ADR-0021 — sensitive destinations | YES | YES | YES |
| ADR-0022 — outbound PST TLS profile | YES | YES | YES |
| ADR-0023 — TLS half-close unsupported | YES | YES | YES |
| ADR-0024 — concrete composition boundary | YES | YES | YES |

Phase 3 security, Phase 4 capability semantics and Phase 5 Job lifecycle remain
unchanged and pass their regression suites.

## Deferred work and known limitations

The following remain explicit future/deferred work:

- explicit capability renegotiation;
- Mode C remote TLS engine;
- PapinhoBrowser integration;
- PapinhoNetwork extraction only if a generic primitive must later be duplicated;
- explicit egress failover policy;
- IDNA/Unicode hostnames;
- outbound client certificates/mTLS profiles;
- generic Web Proxy/application filtering;
- TLS interception;
- generic firewall/router/NAT.

After Phase 6, the next planned milestone is PapinhoBrowser ↔
PapinhoAccelerator integration, including preservation of Mode A, consumption
of Mode B, Mode D over opaque `TCP_PLAIN`, and a future architecture gate for
Mode C. This document does not start that work or assign it a permanent phase
number.

Phase 7 Image Acceleration remains unstarted. Generic reusable image
codecs/algorithms are intended for a sibling `PapinhoImage` component consumed
by PapinhoBrowser and PapinhoAccelerator; no such component is created here.

## PapinhoNetwork extraction gate

`PAPINHONETWORK_REQUIRED_NOW=NO`. Phase 6 found no factual cross-product
duplication requiring extraction. If a generic networking primitive must later
be duplicated across Papinho products, the extraction gate must be reopened.

## Final validation

Validated on 2026-09-13 with MSVC 19.51, Ninja and Windows SDK 10.0.26100.0:

```text
clean MSVC /W4 build       PASS, 227 steps, 0 warnings
full CTest                 67/67 PASS
Not Run                    0
unexpected failures        0
hung tests                 0
ASan Phase 6               11/11 PASS, 0 findings
portable security repeats  5/5 PASS
high-risk six-gate matrix   3/3 PASS
git diff --check           PASS
```
