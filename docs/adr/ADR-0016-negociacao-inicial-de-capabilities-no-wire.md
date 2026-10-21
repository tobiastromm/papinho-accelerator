---
adr: ADR-0016
title: Negociação inicial de capabilities da Session no wire
status: accepted
decision-date: 2026-09-12
last-revised: 2026-09-12
revision: 1
scope: project
decision-makers: [Tobias Tromm]
---
# ADR-0016 — Negociação inicial de capabilities da Session no wire

## Decisão

Após CONTROL_ACCEPT, `0x0007 CAPABILITY_NEGOTIATE` transporta sets canônicos
CLIENT_SUPPORTED e CLIENT_PREFERENCE; `0x0008 CAPABILITY_NEGOTIATE_RESULT`
retorna o effective set. Key wire é U32 ID + U16 major, big-endian, ordenada por
ID/major. Zero, duplicatas, desordem e preference fora de supported rejeitam a
negociação. Keys desconhecidas válidas são omitidas.

O snapshot só é publicado após os cinco inputs do ADR-0012. Há uma negociação
bem-sucedida por Session; nova tentativa é rejeitada. Renegociação continua
futura e nunca ocorre silenciosamente.

