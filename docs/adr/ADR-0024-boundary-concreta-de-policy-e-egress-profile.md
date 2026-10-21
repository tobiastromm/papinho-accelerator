---
adr: ADR-0024
title: Boundary concreta de Network Egress Policy e Egress Profile
status: accepted
decision-date: 2026-09-12
last-revised: 2026-09-12
revision: 1
scope: project
decision-makers: [Tobias Tromm]
---

# ADR-0024 — Boundary concreta de Network Egress Policy e Egress Profile

## Contexto

`allow_network_egress` é apenas autoridade administrativa de alto nível. O
runtime precisa receber policy de destino e forma de saída sem acoplar essas
decisões a CLI, GUI, plataforma ou ao cliente remoto.

## Decisão

O composition root recebe explicitamente uma Network Egress Policy, um catálogo
bounded de Egress Profiles e seleção server-owned de exatamente um profile.
Fontes de configuração apenas constroem esse modelo normalizado.

`allow_network_egress=FALSE` nega antes da policy. `TRUE` apenas permite sua
avaliação; policy ausente, `DENY`, `ERROR`, profile ausente, inválido ou
indisponível negam de forma fail-closed. Destination Policy decide se o destino
pode ser alcançado; Egress Profile decide como uma operação já autorizada sai.

Roteamento normal do OS exige profile explícito de default route. A existência
desse profile não autoriza destinos e sua ausência nunca causa fallback.
Seleção de profile, interface, source IP, link e resolver pertence ao servidor;
o cliente não fornece esses valores. Mudanças afetam novas operações e não
migram operações já admitidas.

Destinos sensíveis, inclusive loopback, private, link-local, metadata,
management e serviços locais, exigem autorização administrativa explícita.
Fixtures injetam policy restrita ao endpoint e modo testados pela mesma boundary
de produção; essa policy não é default produtivo.

Depois de policy, seleção e resolução, o establishment Job recebe somente
endpoint numérico aprovado, binding de source/profile normalizado, modo e
deadline. O backend não executa policy, não resolve DNS e não faz failover.

## Consequências

- ausência de configuração mantém o servidor operacional, mas egress deny-all;
- catálogo e capacidades são explícitos por instância e não exigem heap;
- não há default route, profile failover ou autorização sensível implícitos;
- CLI e GUI futuras convergem para o mesmo modelo.

## Relações

- ADR-0019 — Network Egress Policy e Egress Profiles server-owned.
- ADR-0020 — endpoint numérico aprovado e ausência de segunda resolução.
- ADR-0021 — destinos sensíveis e prevenção de abuso.

