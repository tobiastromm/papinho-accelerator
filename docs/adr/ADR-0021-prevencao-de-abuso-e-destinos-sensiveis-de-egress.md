---
adr: ADR-0021
title: Prevenção de abuso e destinos sensíveis de Network Egress
status: accepted
decision-date: 2026-09-12
last-revised: 2026-09-12
revision: 1
scope: project
decision-makers: [Tobias Tromm]
---
# ADR-0021 — Prevenção de abuso e destinos sensíveis de Network Egress

## Decisão

Unspecified, multicast, broadcast aplicável, malformed e porta zero são
estruturalmente negados. Loopback, link-local, private IPv4, IPv6 ULA, shared
ranges, metadata, management e serviços/listeners locais exigem allow
administrativo explícito. Private não é proibição universal, mas ausência de
regra nega. Internet pública também exige capability e Destination Policy
ALLOW. Network Egress Policy não é firewall genérico nem filtro HTTP e nunca
implica TLS interception.

