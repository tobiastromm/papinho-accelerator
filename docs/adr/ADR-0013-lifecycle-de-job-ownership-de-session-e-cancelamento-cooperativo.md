---
adr: ADR-0013
title: Lifecycle de Job, ownership de Session e cancelamento cooperativo
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

# ADR-0013 — Lifecycle de Job, ownership de Session e cancelamento cooperativo

## Contexto

A Phase 5 precisa representar trabalho finito sem confundir capability,
identidade de backend, payload de workload ou wire. Também precisa limitar
admissão, impedir Jobs órfãos e definir o efeito do fim de uma Session.

## Decisão

`Capability` expressa o que pode ser usado, `Job` uma unidade finita solicitada
e `Backend` como ela executa. Cada Job publicado possui ID runtime não zero,
único entre Jobs vivos e nunca usado como ID wire.

A Session é o owner semântico de exatamente seus Jobs. O Job Manager possui
fisicamente os registros publicados em storage bounded do caller, com
capacidade global e limite por Session explícitos. O vínculo é feito pelo
`owner_session_instance_id`; Job sem Session válida é proibido.

Os estados publicados são `QUEUED`, `RUNNING`, `COMPLETED`, `FAILED` e
`CANCELLED`; os três últimos são terminais e imutáveis. Pedido de cancelamento
é metadata ortogonal. Job `QUEUED` cancela sem iniciar; Job `RUNNING` usa
cancelamento cooperativo e oportunidades bounded. Conclusão que ocorrer antes
do cancelamento efetivo pode vencer. Estados terminais nunca são reabertos.

Admissão valida Session, snapshot, enforcement contextual, limites e backend,
prepara a execução e somente então publica atomicamente. Falha anterior não
publica ID/Job nem vaza estado de backend.

Todo Job possui deadline monotônico efetivo. Expiração impede trabalho e
publicação normal adicionais, solicita cancelamento e conduz a cleanup bounded.
Perda da Session impede nova admissão e publicação de resultado, solicita
cancelamento de todos os Jobs não terminais e permite manter o registro apenas
durante cleanup interno. Jobs terminais possuem reap determinístico; o manager
não mantém histórico ilimitado.

O manager possui apenas metadata genérica de lifecycle, resultado, tempo e
progresso. Payload e recursos específicos pertencem ao workload/backend. O
estado runtime deve ser inspecionável futuramente por boundary read-only de
management, sem payloads, segredos, ponteiros privados ou handles nativos.

Scheduling oferece trabalho bounded e fairness Session-aware: oportunidades
rotacionam entre Sessions executáveis e entre Jobs de uma Session. Uma Session
com muitos Jobs não monopoliza o progresso.

## Consequências

- Jobs não sobrevivem semanticamente à Session.
- Não há heap obrigatório, fila ilimitada ou thread por Job.
- Resultados não podem migrar entre Sessions.
- Limites concretos permanecem configuráveis por instância.
- Wire, GUI e taxonomia de workload continuam fora desta decisão.

## Relações

- ADR-0003 — ownership e lifecycle de Session.
- ADR-0004 — trabalho bounded e fairness.
- ADR-0012 — snapshot e enforcement contextual.
- ADR-0014 — contrato de Compute Backend.

