---
adr: ADR-0017
title: Protocolo wire de Network Offload em DATA Channel dedicado
status: accepted
decision-date: 2026-09-12
last-revised: 2026-09-12
revision: 1
scope: project
decision-makers: [Tobias Tromm]
---
# ADR-0017 — Protocolo wire de Network Offload em DATA Channel dedicado

## Decisão

Depois de DATA_ACCEPT, um DATA Channel aceita um único NETWORK_OPEN v1 com
hostname ASCII, IPv4 ou IPv6, porta, `TCP_PLAIN` ou `TLS_REQUIRED` e oferta ALPN
bounded. O cliente não fornece profile, interface, source IP, trust ou SNI.

IDs: `0x0009 NETWORK_OPEN`, `0x000A NETWORK_OPEN_RESULT`, `0x000B NETWORK_DATA`,
`0x000C NETWORK_HALF_CLOSE`, `0x000D NETWORK_CLOSE`, `0x000E NETWORK_ABORT`.
Direção de DATA é implícita pelo sender; não há Job/Stream ID wire. TCP_PLAIN
exige NETWORK_EGRESS e trata bytes como opacos. TLS_REQUIRED (Mode B) exige
NETWORK_EGRESS e TLS_OFFLOAD. HALF_CLOSE fecha apenas a direção emissora;
CLOSE é terminal normal e ABORT terminal excepcional. Backpressure é bounded.

