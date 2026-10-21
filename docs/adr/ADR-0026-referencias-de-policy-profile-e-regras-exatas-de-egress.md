---
adr: ADR-0026
title: Referências de Network Egress Policy/Profile e regras exatas normalizadas
status: accepted
decision-date: 2026-09-29
last-revised: 2026-09-29
revision: 1
scope: project
decision-makers: [Tobias Tromm]
---

# ADR-0026 — Referências de Network Egress Policy/Profile e regras exatas normalizadas

## Contexto

O runtime Phase 6 já aplica Destination Policy, classificação de destinos
sensíveis, resolução única e Egress Profiles, mas o Shared Configuration Model
não identificava qual policy e qual profile deveriam ser materializados.
`allow_network_egress` é somente o master gate e não pode significar allow-all.

## Decisão

O Configuration Model armazena identificadores opacos e bounded
`egress_policy_ref` e `egress_profile_ref`, nunca callbacks ou objetos runtime.
Resolvers application-owned resolvem ambas as referências antes de RUN. Ref
ausente, desconhecida ou inválida e capacity excedida falham fechado.

A policy resolvida é um snapshot normalizado, bounded e imutável durante RUN.
V1 possui somente exact allow rules:

- hostname ASCII normalizado + porta exata;
- endereço IPv4/IPv6 canônico + porta exata.

Não existem em V1 wildcard, suffix, CIDR/subnet, port range, regex, regra deny,
URL path ou inspeção de conteúdo. Duplicatas são rejeitadas.

Para hostname, o fluxo obrigatório é: normalizar; exigir regra exata de nome;
resolver uma única vez; canonicalizar cada candidato; exigir regra exata de
endpoint; conectar somente o endpoint numérico validado. Literal IP exige
diretamente regra exata de endpoint. Não há segunda resolução.

Destinos sensíveis permanecem deny-by-default. A permissão de classe sensível
é derivada dos endpoints exatos presentes no snapshot e nunca autoriza sozinha
outro endpoint ou porta.

Destination Policy decide onde conectar. Egress Profile decide
interface/source/link/resolver. O profile é selecionado de catálogo
server-owned por referência exata, sem fallback. O cliente não escolhe recurso
privilegiado.

Mesmo com refs válidas, Legacy Endpoint não recebe `NETWORK_EGRESS_V1`.
Transport Security e Phase-6 wire não são alterados.

O ADR não congela INI, JSON, environment, Registry, GUI ou outra sintaxe de
Configuration Source.

## Consequências

- `allow_network_egress=FALSE` continua master deny;
- `TRUE` exige refs resolvidas e policy/profile válidos;
- CLI, GUI e fontes futuras convergem ao mesmo modelo;
- extensões da linguagem exigem revisão arquitetural explícita;
- snapshots possuem limite de implementação documentado, sem semântica wire.

## Relações

- ADR-0019 — Network Egress Policy e Egress Profiles server-owned.
- ADR-0020 — normalização, resolução única e endpoint validado.
- ADR-0021 — prevenção de abuso e destinos sensíveis.
- ADR-0024 — boundary concreta de policy e profile.
