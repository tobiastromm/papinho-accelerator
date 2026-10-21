---
adr: ADR-0019
title: Network Egress Policy e Egress Profiles server-owned
status: accepted
decision-date: 2026-09-12
last-revised: 2026-09-12
revision: 1
scope: project
decision-makers: [Tobias Tromm]
---
# ADR-0019 — Network Egress Policy e Egress Profiles server-owned

## Decisão

NETWORK_EGRESS responde se egress pode existir; Destination Policy decide o
target/porta/contexto; Egress Profile decide interface/source/link/resolver.
`allow_network_egress=FALSE` é master deny; TRUE apenas habilita avaliação
detalhada. Policy retorna ALLOW/DENY/ERROR, com ERROR fail-closed.

Profiles são normalizados e escolhidos pelo servidor; cliente não escolhe
recursos privilegiados. Default route é profile explícito. V1 não faz failover:
profile obrigatório indisponível falha. Mudanças afetam novas operações e não
migram Streams existentes.

