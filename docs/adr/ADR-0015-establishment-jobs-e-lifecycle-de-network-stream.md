---
adr: ADR-0015
title: Establishment Jobs e lifecycle de Network Stream
status: accepted
decision-date: 2026-09-12
last-revised: 2026-09-12
revision: 1
scope: project
decision-makers: [Tobias Tromm]
---
# ADR-0015 — Establishment Jobs e lifecycle de Network Stream

## Decisão

Network establishment é Job finito. Seu sucesso transfere, de forma
failure-atomic, o recurso outbound para um Network Stream de longa duração.
Session é owner semântico; Network Stream Manager possui registros em storage
bounded do caller, com limites global e por Session. V1 associa exatamente um
DATA Channel a um Stream, sem Stream ID wire.

Somente Secure Principal autenticado pode usar Network Offload v1. Perda de
CONTROL cancela establishments e fecha todos os Streams da Session; perda de
DATA fecha somente seu Stream. Falha isolada não derruba outros Streams.
Reserva hidden precede establishment; falha fecha recursos e publica nada.
Inspection é read-only e cross-Session access é negado.

## Consequências

Job permanece finito, Streams não ficam órfãos e não há heap ou thread por
Stream obrigatórios.

