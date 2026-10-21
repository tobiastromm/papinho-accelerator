<!-- SPDX-License-Identifier: MPL-2.0 -->

# PapinhoSecureTransport integration

## Closeout da Phase 6

O build PST-enabled compõe o runtime outbound produtivo pela API CLIENT pública
do PST. A policy exige TLS 1.3, validação de cadeia e hostname/IP, trust
server-owned, ALPN governado, 0-RTT/resumption/client certificate desabilitados
e nenhum fallback plaintext. O Accelerator não inclui nem chama diretamente
OpenSSL, NSS, NSPR ou Schannel. Detalhes e evidência final estão em
[`docs/phase6-network-tls-offload.md`](../phase6-network-tls-offload.md).

Status: current live integration contract for PapinhoAccelerator.

This document records the current factual consumer integration state. It is not
an Integration Handoff, does not replace accepted ADRs, and does not make claims
beyond the pinned dependency, implementation and validated project evidence.
Historical versioned handoffs are preserved by Git history after consumption.

## Consumer outbound da Phase 6.F

`src/network_egress/pst_outbound_tls.[ch]` é a boundary privada para conexões
externas `TLS_REQUIRED`. Ela usa somente a API pública 2.1 do SDK release
fixado; não inclui internals de NSS/OpenSSL/Schannel e não depende do checkout
fonte do PST.

O socket conectado é transferido uma vez ao transporte Win32 PST. Após a
aceitação, PST é a única close root. O stream só pode ser publicado depois de
TLS 1.3, cadeia, autenticação do peer, nome e ALPN configurado serem confirmados
pela API pública. O servidor fornece runtime, trust, provider e identidade
validada por `papacc_server_io_loop_win32_set_outbound_tls`; endereço de rota
não é promovido implicitamente a identidade criptográfica.

### Production composition

No composition root Win32, `allow_network_egress` origina um modelo
normalizado com profile de rota default. Em builds PST-enabled, o mesmo gate
cria runtime PST com o adapter de logging do consumidor e um trust object
explicitamente `PST_TRUST_SOURCE_SYSTEM`. Falha de bootstrap/runtime/trust
encerra o startup; ausencia de configuracao faz `TLS_REQUIRED` falhar fechado
e nunca seleciona plaintext como fallback. Harnesses podem injetar custom
trust test-only.

A identidade de verificacao e derivada pelo estabelecimento do destino
validado. Hostnames ASCII sao normalizados antes da resolucao, avaliados pela
policy antes do DNS e preservados como Expected Peer Name/SNI; os endpoints
numericos retornados sao novamente filtrados e o connect usa somente o endpoint
aprovado, sem segunda resolucao. Para destino IP literal, a identidade IP exata
e usada e exige SAN IP correspondente. Nao existe entrada separada para SNI
arbitrario.

A matriz hermetica da boundary outbound cobre TLS 1.3 valido, versao TLS
incompativel, trust incorreto, nome incorreto, ALPN incorreto, stall com deadline
e reset durante o handshake. Essa matriz e evidencia da boundary PST, nao
substitui a prova PACC completa da Phase 6.F1.

## Current dependency baseline

```text
PST_RELEASE_TAG=v0.6.4
LIBRARY_VERSION=0.6.4
API_VERSION=2.1.0
SPI_VERSION=3.0
TARGET=win32-x64-msvc-19.51-openssl3
ASSET=papinho-secure-transport-0.6.4-win32-x64-msvc-19.51-openssl3.zip
SHA256=f1af50c077b10fecf099ea112c4f02f5eb761e6befb0578cc4f3061f23d3630c
```

The machine-readable pin remains authoritative in:

```text
dependencies/papinho-secure-transport.txt
```

Do not consume `latest`, a PST checkout or an unverified local build. Dependency
selection and release pinning remain governed by PapinhoAccelerator/ADR-0008.

## Integration boundary

```text
PapinhoAccelerator
├── owns bind/listen/accept
├── owns PACC framing and protocol
├── owns Connection / Session / Channel policy
├── owns Principal mapping and authorization
├── owns application deadlines, fairness and work budgets
└── consumes only public PST API / public platform helpers

PapinhoSecureTransport
├── accepts an explicitly transferred connected transport
├── owns the secure transport lifecycle after accepted transfer
├── owns provider selection and pinning
├── owns TLS policy and provider-facing secure I/O
├── owns TLS readiness semantics
├── owns TLS shutdown progression
└── exposes normalized diagnostics / peer evidence
```

PST does not own the listener or `accept()`. PapinhoAccelerator does not
reimplement PST provider selection, trust, TLS lifecycle or provider-private
behavior. There is no provider fallback after binding.

## Security profile consumed by Accelerator

The validated Secure Principal path uses the public PST API with:

```text
TLS 1.3 only
mTLS required
ALPN = papacc/1 required
Local Identity / Peer Authentication / Peer Trust kept separate
provider selected before binding and pinned for connection lifetime
```

The Accelerator security composition remains behind the private consumer
boundary. No direct OpenSSL, Schannel, NSS or NSPR API is a PapinhoAccelerator
production dependency.

## Scheduler / readiness integration

PST API 2.1 supplies the wait-set used by the private secure scheduler.

Current integration contract:

```text
PST wait-set
├── multiple secure connections
├── timeout-zero observation
├── finite blocking waits
├── stable registration tokens
├── cross-thread wake
└── borrowed external/native sources
```

PapinhoAccelerator remains the scheduling owner:

```text
PST readiness
        ↓
Accelerator scheduler
├── bounded dispatch
├── rotating fairness
├── application deadlines
└── listener/external-source composition
```

External/native sources are borrowed. PST never closes the Accelerator-owned
listener/native resource merely because it is registered in a wait-set.
Registrations are removed before member release.

## Incremental I/O and ownership

Secure I/O is incremental. Partial reads/writes and backpressure are normal.
Consumer-owned buffers must remain valid for the lifetime required by the
public PST operation contract. Application deadlines bound progress.

Connected transport ownership transfers only through the explicit PST attach
boundary. After PST accepts ownership, failure does not roll back to plaintext
or return transport ownership to the Accelerator.

## Graceful shutdown contract

The current consumer requires:

```text
PST_CAP_GRACEFUL_SHUTDOWN
PST_TLS_POLICY.require_graceful_shutdown = PST_FEATURE_REQUIRED
```

The PST feature values:

```text
PST_FEATURE_DISABLED = 0
PST_FEATURE_OPTIONAL = 1
PST_FEATURE_REQUIRED = 2
```

declare feature/provider eligibility and policy. They do **not** by themselves:

```text
start TLS shutdown
create an application deadline
make release perform implicit graceful shutdown
```

PapinhoAccelerator explicitly invokes incremental shutdown and owns the
monotonic deadline used to bound it.

```text
feature requirement
        ↓
provider eligibility / connection policy

explicit shutdown calls
        ↓
close_notify progression
        ↓
consumer-owned deadline
```

Abrupt EOF remains truncation. Release is not a substitute for the required
cooperative shutdown path.

## SNI and peer-name boundary

CLIENT SNI and Expected Peer Name remain separate PST concepts. The current
Accelerator compatibility/reference-client profile uses `PST_SNI_MODE_COMPAT`
and does not require independent `PST_CAP_SNI_CONTROL` for that profile.

Provider capabilities remain role-scoped and factual; consumers must not infer
SERVER support or a provider feature from unrelated TLS capabilities.

## Current validation state

The Phase 3 final audit records the integrated security baseline as complete for
its defined scope, including:

```text
PST 0.6.4 pin/hash validation
TLS 1.3 mTLS
required papacc/1 ALPN
AuthN/AuthZ separation
secure CONTROL
secure DATA
bounded graceful shutdown
candidate failure isolation
exactly-once connected-transport ownership
scheduler fairness
reference-client interoperability
wire unchanged
```

The current Phase 3 audit remains the detailed evidence source:

```text
docs/phase3-security-final-audit.md
```

This live integration document must be updated when the pinned PST baseline or
consumer contract changes.

## Current non-claims

This document does not claim that every future deployment/configuration surface
is complete. In particular, operational credential/configuration sources,
production Secure CLI wiring, complete Legacy Endpoint exposure, Browser
integration, `TLS_OFFLOAD`, Network Egress and unrelated future capabilities
remain governed by their own current documentation and implementation state.

## Related decisions and documents

- `PapinhoAccelerator/ADR-0004` — nonblocking bounded fair I/O scheduling.
- `PapinhoAccelerator/ADR-0006` — Secure Principal Transport Security profile.
- `PapinhoAccelerator/ADR-0008` — release-pinned Papinho dependencies.
- `PapinhoAccelerator/ADR-0009` — explicit transport profile per listener.
- `PapinhoAccelerator/ADR-0010` — opaque server security configuration reference/resolution.
- `docs/connection-io-scheduling.md`
- `docs/security-model.md`
- `docs/phase3-authentication-authorization.md`
- `docs/phase3-security-final-audit.md`
- `dependencies/papinho-secure-transport.txt`

## Handoff consumption provenance

This live document absorbs the still-current integration knowledge from the
consumed versioned handoffs for PST 0.5.0, 0.6.0, 0.6.1 and 0.6.4.

```text
0.5.0 handoff
0.6.0 handoff
0.6.4 handoff
        ↓ consumed
current durable/factual integration knowledge
        ↓
docs/integration/papinho-secure-transport.md
        +
ADRs / dependency pin / Code / Tests / final audit
```

The removed handoff files remain recoverable through Git history and are no
longer active integration authority.
