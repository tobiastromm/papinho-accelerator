---
adr: ADR-0018
title: Registry de capabilities Network Egress e TLS Offload
status: accepted
decision-date: 2026-09-12
last-revised: 2026-09-12
revision: 1
scope: project
decision-makers: [Tobias Tromm]
---
# ADR-0018 — Registry de capabilities Network Egress e TLS Offload

## Decisão

O registry canônico aloca IDs estáveis, nunca reutilizados; zero é reservado e
major faz parte da key. Nome é metadata. Capability ID, Backend ID e Message ID
são domínios distintos.

```text
NETWORK_EGRESS = (0x00000001, 1)
TLS_OFFLOAD    = (0x00000002, 1)
```

NETWORK_EGRESS autoriza solicitar conectividade outbound mediada pelo servidor,
sujeita à policy contextual. TLS_OFFLOAD autoriza processamento TLS pelo
Accelerator. TLS_OFFLOAD não implica ontologicamente NETWORK_EGRESS: Mode B
exige ambas; Mode C futuro pode combinar TLS remoto com network local.

