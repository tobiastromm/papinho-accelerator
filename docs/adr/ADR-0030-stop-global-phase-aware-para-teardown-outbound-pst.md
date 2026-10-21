---
adr: ADR-0030
title: Stop global phase-aware para teardown outbound PST
status: accepted
decision-date: 2026-10-08
last-revised: 2026-10-08
revision: 2
scope: project
decision-makers: [Tobias Tromm]
---

# ADR-0030 — Stop global phase-aware para teardown outbound PST

## Status e limite

Esta decisão arquitetural foi explicitamente aceita pelo owner e sua implementação em slices governados está autorizada. O estado factual do runtime ainda não foi qualificado: S1-S12 permanecem não executados, G2 permanece não qualificado e esta decisão não autoriza alterações de API/ABI pública, PST ou PACC wire.

## Contexto

Uma conexão PST outbound registrada só pode ser liberada depois de removida do wait-set. O fluxo atual marca `stop_requested` antes da remoção, não repete a remoção numa solicitação subsequente e possui um `shutdown()` `void` que ignora falhas. Assim, o runner pode avançar até a destruição de Streams ainda dependentes de uma conexão registrada.

O contrato obrigatório é:

```text
REMOVE_FROM_WAITSET
-> CONFIRM_REMOVAL
-> PST_RELEASE
-> NATIVE_SOCKET_CLOSE
-> STREAM_DESTROY
```

Falha numa fase bloqueia todas as fases destrutivas posteriores daquele recurso. O último ponteiro válido, a identidade da operação e a causa factual permanecem preservados.

## Modelo de estado proposto

`PAPACC_SERVER_SECURE_IO_WIN32` recebe um enum privado:

```text
RUNNING
  -> STOP_REQUESTED
  -> REMOVE_PENDING
  -> DRAINING
  -> STOPPED

REMOVE_PENDING
  -> REMOVE_PENDING       remoção falhou; nova tentativa governada
  -> DRAINING             todas as memberships foram removidas
  -> ERROR_TERMINAL       orçamento monotônico esgotado
```

`ERROR_TERMINAL` significa parada incompleta e segura. Não significa cleanup concluído e não permite release, destruição de Stream ou liberação das estruturas que ainda guardam ownership.

O booleano `stop_requested` passa a ser derivado de `state != RUNNING`; ele não permanece como segunda autoridade independente.

Estado privado mínimo adicional:

- deadline monotônico absoluto do Stop, calculado uma vez;
- próxima oportunidade monotônica de retry;
- número bounded de tentativas, para diagnóstico e limite secundário;
- último resultado de remoção;
- identidade da primeira entrada outbound ainda registrada;
- causa original do Stop, imutável;
- flag `dispatch_active`, somente para impedir mutação durante dispatch.

As entradas `outbound_ready` continuam sendo a autoridade de connection, wait-token, Stream instance/generation e membership. Não se cria cópia de ownership.

## Autoridade e transições

Somente a thread owner de `PAPACC_SERVER_SECURE_IO_WIN32` altera o estado de Stop ou o scheduler.

### `request_stop()`

Primeira solicitação:

1. registra causa e deadline uma única vez;
2. muda `RUNNING -> STOP_REQUESTED`;
3. impede novos accepts e novos registros outbound;
4. se dispatch estiver ativo, muda para `REMOVE_PENDING` e acorda o owner;
5. fora de dispatch, tenta remover cada membership outbound;
6. somente depois de todas as remoções confirmadas muda para `DRAINING`.

Solicitação repetida é idempotente quanto à causa e ao deadline, mas não retorna sucesso enquanto existir remoção pendente. Em `REMOVE_PENDING`, retorna o último erro factual ou um resultado interno de pendência. Em `DRAINING`, retorna sucesso sem reiniciar o orçamento. Em `STOPPED`, retorna sucesso. Em `ERROR_TERMINAL`, retorna o erro persistente preservado.

Uma falha de remoção não limpa `outbound_ready`, não limpa `processor.connection`, não altera o Stream e não inicia teardown dependente.

### Progresso pelo owner

`poll_once()` é a única fronteira de retry durante Stop. Cada chamada realiza no máximo uma rodada de remoção. Entre tentativas, o wait-set é acordado ou espera até `min(proxima_tentativa, deadline)`, sem busy-loop.

Quando todas as memberships forem removidas, o owner entra em `DRAINING` e executa o shutdown incremental já existente. `STOPPED` exige candidatos, Connections, Sessions, DATA, Streams e memberships em estado terminal/zero.

### Falha persistente

O orçamento usa deadline monotônico absoluto e não é reiniciado por retry, wake ou solicitação repetida. O desenho admite um número máximo de tentativas como limite secundário; o primeiro limite atingido vence.

Ao esgotar o orçamento, o estado vira `ERROR_TERMINAL`. O owner preserva scheduler, entrada outbound, conexão, Stream, identidade e causa. Não existe fallback destrutivo. O processo reporta falha de shutdown e não desaloca essas estruturas enquanto a membership permanecer ativa.

## Shutdown seguro e fronteira interna

A assinatura pública do produto não muda. A menor fronteira interna necessária é uma função com resultado, por exemplo:

```text
papacc_server_secure_io_win32_try_shutdown(secure_io)
    -> OK somente se não existir membership outbound
    -> INVALID_STATE/PENDING enquanto remoção estiver pendente
    -> erro factual em ERROR_TERMINAL
```

Ela verifica antes de qualquer ação destrutiva:

- owner thread;
- dispatch inativo;
- nenhuma entrada `outbound_ready.registered`;
- scheduler sem a connection correspondente;
- estado `STOPPED` ou draining concluído.

O wrapper `void papacc_server_secure_io_win32_shutdown()` existente pode ser preservado por compatibilidade interna, mas deve ser não destrutivo quando as pré-condições falharem. Ele não pode esconder sucesso; chamadores que precisam de garantia usam `try_shutdown()`.

Nenhuma API/ABI pública, PST ou PACC wire muda. A API interna de composição e seus callers mudam.

## Runner

O runner:

1. verifica o resultado de `request_stop()`;
2. continua chamando `poll_once()` enquanto o estado for `STOP_REQUESTED`, `REMOVE_PENDING` ou `DRAINING` e o deadline não tiver expirado;
3. preserva o primeiro erro e registra o último erro de remoção;
4. não ignora erro de `poll_once()` durante recuperação;
5. chama `try_shutdown()` somente após `is_stopped()`;
6. libera buffers/scheduler/IO loop apenas depois de `try_shutdown()==OK`;
7. em `ERROR_TERMINAL`, retorna falha e mantém os owners vivos até o limite do processo, sem declarar shutdown limpo.

O loop é limitado pelo deadline absoluto e por uma tentativa por iteração.

## Concorrência e compatibilidade

- Durante dispatch, Stop apenas registra intenção e acorda o owner.
- Remoção ocorre depois do retorno de `dispatch_ready()`.
- Eventos tardios validam operação e geração antes de acessar o Stream.
- `REMOVE_PENDING` e `ERROR_TERMINAL` proíbem novas operações PST.
- Uma entrada removida/released não volta a ser registrada durante Stop.
- Release, provider destroy, `closesocket` e Stream destroy têm gates exactly-once vinculados ao objeto real.

O desenho preserva CONTROL persistente, DATA por operação, TLS_REQUIRED, NETWORK_CLOSE, NETWORK_ABORT, deadline, cancelamento, perda de CONTROL e Stop global. Terminais PACC seguem seus contratos próprios; estado de Stop não inventa frame terminal.

## Plano de testes

Cada teste registra estado inicial/final, operação/geração, membership, tentativas e sucessos de remove/release/provider destroy/native close/Stream destroy.

| Caso | Injeção | Critério de PASS |
|---|---|---|
| S1 | Stop normal | `RUNNING -> STOP_REQUESTED -> DRAINING -> STOPPED`; todas as remoções precedem releases. |
| S2 | Stop repetido | Causa/deadline imutáveis; nenhuma segunda remoção ou liberação após sucesso. |
| S3 | Remove falha uma vez | Primeira falha preserva objetos; tentativa posterior remove e conclui exactly-once. |
| S4 | Remove falha persistentemente | Deadline leva a `ERROR_TERMINAL`; zero release/close/destroy do recurso registrado; sem busy-loop. |
| S5 | Shutdown com membership ativa | `try_shutdown()` rejeita; wrapper `void` não destrói; identidade permanece válida. |
| S6 | Stop durante dispatch | Nenhuma mutação do scheduler dentro do callback; remoção ocorre após dispatch. |
| S7 | Stop durante shutdown TLS pendente | NEED_WRITE permanece factual até remoção; Stop preserva causa e não produz falso clean close. |
| S8 | Múltiplos outbound | Remoção parcial preserva entradas restantes; cada recurso conclui exatamente uma vez. |
| S9 | Stop após perda CONTROL | Causa CONTROL permanece; cascata DATA não duplica release. |
| S10 | Stop após deadline | Causa SHUTDOWN_DEADLINE não é sobrescrita por Stop; cleanup mantém ordem. |
| S11 | Callback tardio | Operação/geração terminal é rejeitada sem resurrection ou acesso após release. |
| S12 | Exactly-once nativo | Um provider destroy e um `closesocket` efetivo por socket adquirido; ponteiro zerado não conta como prova. |

Negativos obrigatórios incluem: sucesso falso em Stop repetido, deadline reiniciado, remove falho seguido de release, mutação durante dispatch, segunda publicação/liberação, callback de geração antiga e `STOPPED` com membership.

## Evidência necessária antes de aceitação

- S1–S12 PASS em build isolado identificado;
- regressões de scheduler, server I/O loop e owner hooks;
- requalificação dos casos A–F e G1 afetados;
- G2 integrado executado separadamente;
- ausência de hooks test-only no build normal;
- `git diff --check` e zero warnings novos.

## Relações

- ADR-0003 — ownership e lifecycle de Connection/Session/Channel;
- ADR-0004 — I/O não bloqueante, limitado e justo;
- ADR-0015 — lifecycle de Network Stream;
- ADR-0017 — Network Offload em DATA dedicado;
- ADR-0023 — lifecycle terminal de TLS_REQUIRED v1;
- PapinhoSecureTransport ADR-0005 — wait-set e readiness;
- PapinhoSecureTransport `docs/lifecycle-ownership.md`.

## Consequências

O runtime ganha estado privado e uma API interna com resultado. A recuperação fica limitada e diagnosticável. Uma falha persistente pode impedir shutdown completo do processo, mas não viola ownership nem converte erro em sucesso.