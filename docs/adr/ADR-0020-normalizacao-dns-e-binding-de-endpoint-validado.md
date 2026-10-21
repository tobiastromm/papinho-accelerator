---
adr: ADR-0020
title: Normalização, DNS e binding de endpoint validado
status: accepted
decision-date: 2026-09-12
last-revised: 2026-09-12
revision: 1
scope: project
decision-makers: [Tobias Tromm]
---
# ADR-0020 — Normalização, DNS e binding de endpoint validado

## Decisão

V1 aceita hostname DNS ASCII, IPv4 e IPv6; Unicode/IDNA falha fechado. Hostname
é normalizado e avaliado antes de uma resolução pela boundary injetável. Cada
candidate é canonicalizado (inclusive IPv4-mapped IPv6) e submetido a policy de
IP/subnet/sensibilidade. Candidate negado nunca é conectado; um permitido pode
ser escolhido. Connect usa exatamente o endpoint numérico já aprovado, sem
segunda resolução. Nova resolução exige nova avaliação. Resolver/profile que
não possa garantir o binding falha fechado. Redirect HTTP fica fora do core.

