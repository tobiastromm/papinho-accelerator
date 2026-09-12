---
adr: ADR-0014
title: Contrato portátil incremental de Compute Backend
status: accepted
decision-date: 2026-09-12
last-revised: 2026-09-12
revision: 1
scope: project
decision-makers:
  - Tobias Tromm
supersedes: []
superseded-by: []
---

# ADR-0014 — Contrato portátil incremental de Compute Backend

## Contexto

Compute Backends futuros podem usar CPU, GPU, bibliotecas, hardware ou
coprocessadores. O core precisa executá-los sem conhecer mecanismo, payload,
tipos nativos ou modelo interno de concorrência.

## Decisão

Compute Backend responde somente como o Job executa. Ele não autentica,
autoriza, concede capability nem admite Job. O contrato é portátil,
substituível, injetável e compatível com C89/VC6, sem PST, handles de OS ou
tipos de workload no framework genérico.

O provider oferece operações equivalentes a `prepare`, `start`, `step`,
`request_cancel` e `destroy`. Cada chamada é bounded. `step` pode informar
progresso, ausência normal de progresso/`WOULD_BLOCK`, conclusão, falha ou
cancelamento. Conclusão síncrona ou incremental é válida; uma oportunidade do
scheduler nunca executa um loop até terminar.

Cancelamento é bounded, idempotente e cooperativo. Após solicitá-lo, passos
posteriores podem ser necessários até o terminal. `destroy` não substitui o
cancelamento e realiza cleanup determinístico exatamente sob o ownership do
binding Job–Backend.

Estado privado é opaco para o core e liberado explicitamente. Payload de
resultado permanece pertencente ao workload/backend; o core recebe somente
metadata genérica. O provider/selector é injetado na admissão. Ausência ou
falha de preparação não publica Job. Depois do binding, falha não provoca
fallback/restart silencioso em outro backend.

O framework não requer thread por Job nem thread pool. Um backend pode usar
threads ou hardware internamente se ainda apresentar operações bounded e não
bloqueantes ao framework.

## Consequências

- Backends heterogêneos preservam um lifecycle comum.
- `WOULD_BLOCK` não é falha nem autoriza busy-spin.
- Readiness especializada continua decisão futura do adapter.
- Seleção sofisticada, retry/fallback e wire permanecem fora do escopo.

## Relações

- ADR-0002 — backends portáteis e substituíveis.
- ADR-0004 — scheduling não bloqueante e justo.
- ADR-0005 — backend não concede capability.
- ADR-0013 — lifecycle e ownership de Job.

