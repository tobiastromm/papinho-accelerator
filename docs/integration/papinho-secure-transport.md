<!-- SPDX-License-Identifier: MPL-2.0 -->

# PapinhoSecureTransport integration

Status: current live integration contract for PapinhoAccelerator.

This document records the current factual consumer integration state. It is not
an Integration Handoff, does not replace accepted ADRs, and does not make claims
beyond the pinned dependency, implementation and validated project evidence.
Historical versioned handoffs are preserved by Git history after consumption.

## Current dependency baseline

```text
PST_RELEASE_TAG=v0.6.1
LIBRARY_VERSION=0.6.1
API_VERSION=2.1.0
SPI_VERSION=3.0
TARGET=win32-x64-msvc-19.51-openssl3
ASSET=papinho-secure-transport-0.6.1-win32-x64-msvc-19.51-openssl3.zip
SHA256=e9965fbcaf6aaa0a96a71bbc38d0b37ca447459641a09a03f1c957a88d764e53
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
PST 0.6.1 pin/hash validation
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
consumed versioned handoffs for PST 0.5.0, 0.6.0 and 0.6.1.

```text
0.5.0 handoff
0.6.0 handoff
0.6.1 handoff
        ↓ consumed
current durable/factual integration knowledge
        ↓
docs/integration/papinho-secure-transport.md
        +
ADRs / dependency pin / Code / Tests / final audit
```

The removed handoff files remain recoverable through Git history and are no
longer active integration authority.
