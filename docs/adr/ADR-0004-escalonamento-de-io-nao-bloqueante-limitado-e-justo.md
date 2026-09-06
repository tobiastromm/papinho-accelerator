---
adr: ADR-0004
title: Escalonamento de I/O não bloqueante, limitado e justo
status: accepted
decision-date: 2026-09-06
last-revised: 2026-09-06
revision: 1
scope: project
decision-makers:
  - Tobias Tromm
supersedes: []
superseded-by: []
---

# ADR-0004 — Escalonamento de I/O não bloqueante, limitado e justo

## Contexto

O PapinhoAccelerator precisa processar múltiplas Connections sem permitir que um peer ocioso, lento ou malformado bloqueie o atendimento dos demais.

Chamadas sequenciais de leitura em Connections bloqueantes são inadequadas: uma única Connection sem dados disponíveis pode permanecer presa em `recv()` e impedir progresso de outras Connections.

A Phase 2 consolidou uma arquitetura inicial de scheduling baseada em:

```text
single application-owned I/O thread
+ readiness-driven scheduling
+ nonblocking accepted Connections
+ bounded work per scheduling pass
+ round-robin fairness
```

No backend Win32 inicial, essa arquitetura é implementada por um loop combinado baseado em `select()` que observa listeners e Connections.

Entretanto, `select()` é uma implementação específica da plataforma atual. A decisão durável não deve transformar `select()` em identidade permanente do core, porque backends futuros podem usar mecanismos diferentes de readiness.

Esta decisão formaliza os princípios de scheduling, fairness, failure isolation, timeout e ownership estabelecidos na Phase 2, preservando `select()` como baseline Win32 atual e não como requisito universal.

## Forças da decisão

- responsividade com múltiplas Connections;
- compatibilidade com WinSock antigo;
- suporte a Windows NT4/2000/XP;
- ausência de thread-per-Connection;
- fairness;
- trabalho limitado por passagem;
- isolamento de falhas;
- ownership determinístico;
- portabilidade para readiness backends futuros;
- backpressure explícito;
- uso de tempo monotônico;
- shutdown ordenado.

## Alternativas consideradas

### Alternativa A — I/O bloqueante sequencial

O servidor chama operações de leitura/escrita diretamente em Connections, uma após outra.

**Vantagens**

- implementação simples;
- pouco estado de scheduler.

**Desvantagens / trade-offs**

- um peer ocioso pode bloquear o servidor;
- starvation entre clientes;
- baixo controle de fairness;
- shutdown e timeouts tornam-se difíceis;
- inadequado para múltiplas Connections simultâneas.

### Alternativa B — Thread por Connection

Cada Connection recebe uma thread dedicada.

**Vantagens**

- modelo conceitual simples por conexão;
- operações bloqueantes podem ser usadas localmente.

**Desvantagens / trade-offs**

- alto custo de recursos em plataformas antigas;
- ownership e shutdown mais complexos;
- fairness depende do scheduler do sistema operacional;
- escala pior para ambientes limitados;
- conflita com a intenção de manter processamento leve e determinístico.

### Alternativa C — Readiness + nonblocking + trabalho limitado

Um scheduler da aplicação observa readiness e concede pequenos turnos de trabalho por Connection.

**Vantagens**

- evita bloqueio global;
- permite fairness explícita;
- funciona com uma única thread inicial;
- suporta backpressure;
- failure isolation é mais simples;
- readiness backend pode ser substituído por plataforma.

**Desvantagens / trade-offs**

- exige state machines incrementais;
- Reader/Writer precisam preservar progresso;
- scheduler e lifecycle ficam mais elaborados;
- limites de mecanismos como `FD_SETSIZE` precisam ser respeitados.

## Decisão

O PapinhoAccelerator adota scheduling de I/O orientado por readiness, com Connections aceitas em modo não bloqueante, trabalho limitado por turno e fairness explícita.

A arquitetura durável é:

```text
Readiness Backend
        ↓
Application-owned Scheduler
        ↓
bounded accept/read/write turns
        ↓
portable protocol processors
```

### Baseline Win32 atual

No backend Win32 inicial:

```text
single application-owned thread
+ one combined select()-based Server I/O Loop
+ nonblocking accepted Connections
+ bounded round-robin work
```

`select()` é a implementação inicial de readiness para Win32.

Ele **não é** uma exigência permanente do core.

Um backend futuro poderá usar:

```text
poll
epoll
kqueue
IOCP
WSAPoll
WSAEventSelect
custom device readiness
outro mecanismo apropriado
```

desde que preserve os contratos arquiteturais deste ADR.

### Unified readiness

Listeners e Connections devem ser coordenados por uma única autoridade de scheduling por backend/composition quando isso evitar loops de espera independentes e problemas de coordenação.

No Win32 atual:

```text
Server I/O Loop
    ├── readable listener
    │      → accept bounded work
    ├── readable Connection
    │      → receive turn
    └── writable Connection with pending output
           → send turn
```

Dois loops bloqueantes independentes para listeners e Connections não são a baseline adotada.

### Accepted Connections são nonblocking

Readiness não substitui nonblocking mode.

Mesmo após readiness ser observada, o estado pode mudar antes da operação.

Portanto:

```text
readiness observed
        !=
I/O guaranteed to complete without WOULD_BLOCK
```

Connections aceitas devem entrar no processamento produtivo em modo não bloqueante.

Falha ao fazer a transição nonblocking antes da publicação deve fechar apenas o novo recurso e publicar nada.

### Trabalho é limitado por turno

Readiness autoriza progresso, não drenagem ilimitada.

A baseline inicial é:

```text
at most one Reader action
per ready Connection
per scheduling pass
```

e:

```text
at most one Writer step
per writable Connection
per scheduling pass
```

A arquitetura pode evoluir para budgets por bytes, eventos ou operações, mas qualquer evolução deve preservar a propriedade:

```text
one ready Connection
        !=
unbounded scheduler ownership
```

### Fairness explícita

O scheduler não deve iniciar toda passagem sempre no mesmo slot.

A baseline utiliza cursor/varredura round-robin.

A propriedade normativa é:

```text
ready peers receive bounded opportunities to progress
```

e não a obrigação permanente de uma implementação específica de round-robin.

Uma estratégia futura pode mudar, desde que preserve fairness e bounded work.

### Write interest apenas quando necessário

Connections ociosas não devem ser monitoradas continuamente para writability.

Write readiness deve existir quando houver saída pendente.

```text
pending outbound work
        → write interest

no pending outbound work
        → no write interest
```

Isso evita busy loops em sockets normalmente graváveis.

### I/O é incremental

Reader e Writer devem suportar progresso parcial.

Resultados como:

```text
PROGRESS
WOULD_BLOCK
NEED_MORE_DATA
END_OF_STREAM
```

são condições normais do modelo incremental e não justificam loops internos ilimitados.

Partial write mantém estado pendente para um turno futuro.

### Processor separado de Connection

Connection permanece entidade de transporte.

Reader, Writer, parser, handshake e estado de protocolo pertencem ao processor/composição apropriados e não devem ser empurrados para dentro da Connection apenas por conveniência de scheduling.

O processor mantém associação não-owning com a Connection.

Connection Manager permanece owner da Connection conforme o ADR-0003.

### Buffer/lifetime explícitos

Buffers usados por processamento incremental devem possuir lifetime compatível com o estado pendente.

A baseline privilegia storage caller-owned/fixed para establishment e scratch buffers, sem heap por read.

O Writer não deve depender de payload cujo lifetime termine antes de `FRAME_COMPLETE`.

### Timeout usa tempo monotônico

Deadlines de establishment e outros timeouts de lifecycle/scheduling devem usar relógio monotônico.

```text
protocol duration
        !=
wall-clock time
```

Mudanças de horário civil não podem alterar duração de timeout.

### Isolamento de falhas

A regra é:

```text
one malformed/failed client
        !=
server failure
```

Timeout, framing error, EOF, protocol error e transport failure devem fechar o menor escopo seguro.

Normalmente:

```text
connection-scoped failure
        ↓
close Connection / processor scope
        ↓
other Connections and listeners continue
```

Quando o erro afeta CONTROL, propagação para Session/DATA deve ocorrer pelas regras do ADR-0003, sem duplicar lifecycle no scheduler.

### `FD_SETSIZE` não é limite de protocolo

No Win32 `select()` atual, `FD_SETSIZE` limita os sets combinados.

Esse limite é propriedade do backend/composition atual, não uma política permanente do PACC ou do PapinhoAccelerator.

A capacidade deve ser validada antes de inserir descriptors no backend.

Scaling futuro pode usar sharding ou outro backend sem alterar o contrato portátil.

### Heavy compute fica fora do I/O thread

A thread de I/O executa trabalho leve e bounded de protocolo.

Operações pesadas não devem bloquear o scheduler de rede.

Compute pesado pertence a worker/Compute Backend apropriado em fases posteriores.

### Shutdown ordenado

O scheduler deve parar antes da destruição dos recursos que ele observa.

Ordem conceitual:

```text
stop new publication
    ↓
stop/wake scheduler
    ↓
exit scheduler
    ↓
shutdown processors
    ↓
shutdown Channel/Session lifecycle
    ↓
close Connections / Acceptor
    ↓
shutdown listeners/platform
```

Reader/Writer não podem sobreviver à Connection que utilizam.

Cleanup deve permanecer idempotente.

### Segurança não pode ser bypassada por scheduling

Readiness observa o transporte efetivo usado pelo backend.

Quando Transport Security for requerida:

```text
Transport
    ↓
Secure Transport
    ↓
Framing / Protocol
```

O scheduler não pode criar caminho alternativo que permita ao protocolo consumir bytes fora da camada de segurança obrigatória.

## Justificativa

Readiness scheduling com I/O não bloqueante evita que uma Connection detenha o servidor inteiro.

Trabalho limitado por turno permite fairness explícita e torna o comportamento previsível em máquinas limitadas.

Separar o contrato durável da implementação `select()` permite manter compatibilidade com sistemas antigos hoje sem impedir backends mais adequados em Linux, appliances ou hardware futuro.

State machines incrementais também se alinham ao framing e ao lifecycle já definidos pelo projeto.

## Consequências

### Positivas

- um peer lento não bloqueia os demais;
- fairness pode ser verificada;
- uma única thread atende a baseline atual;
- backpressure é natural;
- falhas permanecem isoladas;
- timeouts são robustos a alteração de relógio civil;
- o backend de readiness pode evoluir;
- heavy compute não precisa contaminar o scheduler.

### Negativas / trade-offs

- maior complexidade de state machines;
- processamento precisa ser incremental;
- buffers/lifetimes precisam ser explícitos;
- backend `select()` possui limites como `FD_SETSIZE`;
- futuras filas de saída exigirão política própria.

### Neutras ou operacionais

- `select()` continua sendo baseline Win32 atual;
- IOCP, poll/epoll/kqueue e outros mecanismos não são implementados por este ADR;
- budgets numéricos maiores permanecem futuros;
- general send queues, priorities e cancellation policy continuam fora do escopo atual.

## Regras derivadas

1. I/O produtivo de múltiplas Connections deve ser readiness-driven.
2. Accepted Connections entram no processamento em modo nonblocking.
3. Readiness não elimina a possibilidade de `WOULD_BLOCK`.
4. Trabalho por Connection deve ser bounded por scheduling pass.
5. Fairness deve ser explícita.
6. O scheduler não deve drenar uma Connection indefinidamente.
7. Write interest existe apenas quando há saída pendente.
8. Reader/Writer operam incrementalmente.
9. Connection permanece entidade de transporte, não processor de protocolo.
10. Processor não assume ownership implícito da Connection.
11. Buffers pendentes devem possuir lifetime suficiente.
12. Timeouts usam relógio monotônico.
13. Falha de uma Connection não derruba o servidor.
14. Lifecycle de CONTROL/Session deve ser delegado às regras do ADR-0003.
15. `FD_SETSIZE` não é limite permanente do protocolo/produto.
16. Heavy compute não deve bloquear a thread de I/O.
17. Scheduler deve parar antes da destruição dos recursos observados.
18. Readiness/scheduling não pode bypassar Transport Security requerida.
19. `select()` é baseline Win32, não identidade arquitetural permanente.

## Impacto

### Código

Schedulers, Reader/Writer e processors devem preservar bounded work e estado incremental.

Backends de transporte devem suportar as condições necessárias ao scheduler correspondente.

### Build e toolchains

A baseline Win32 deve permanecer compatível com os targets definidos pelo projeto.

Backends futuros podem selecionar mecanismos de readiness diferentes por target.

### Documentação

`docs/connection-io-scheduling.md` permanece como documento histórico/técnico detalhado.

Este ADR passa a ser a decisão canônica sobre scheduling.

Specifications de framing/protocolo não devem absorver política de scheduler.

### Compatibilidade

A decisão é compatível com backends diferentes.

Somente a implementação `select()` é específica do backend Win32 atual.

### Testes

Testes devem verificar, conforme aplicável:

- accepted Connections nonblocking;
- handling de `WOULD_BLOCK`;
- bounded read/write work;
- fairness/rotating traversal;
- write interest somente com saída pendente;
- failure isolation;
- monotonic deadlines;
- backend capacity guard antes de `FD_SET`;
- shutdown sem Reader/Writer sobrevivendo à Connection.

## Verificação de conformidade

Pode-se verificar se:

- nenhuma Connection ready é drenada de forma ilimitada;
- uma Connection ociosa não bloqueia outras;
- traversal não favorece permanentemente slots iniciais;
- `WOULD_BLOCK` é tratado como condição normal;
- write readiness não cria busy loop;
- failures fecham apenas o menor escopo seguro;
- timeouts usam PAL monotônica;
- `select()` não aparece em contratos portáveis do protocolo;
- backend valida limites de descriptors;
- heavy compute não roda no hot path do scheduler;
- shutdown respeita ownership/lifetime.

## Relações

### ADRs relacionadas

- `PapinhoAccelerator/ADR-0002` — Core portável entre deployments e backends substituíveis.
- `PapinhoAccelerator/ADR-0003` — Connection, Session e Channel — ownership e lifecycle.
- `PapinhoEngineering/ADR-0007` — Fronteiras portáveis entre core, plataforma e backends substituíveis.
- `PapinhoAccelerator/ADR-0001` — Secure Principal e Legacy Endpoint.

### Capability Documents relacionadas

Nenhuma específica neste momento.

### Documentos relacionados

- `docs/connection-io-scheduling.md`
- `docs/phase2-transport-session-design.md`
- `docs/control-establishment-protocol.md`
- `docs/data-association-protocol.md`
- `docs/protocol-framing.md`

## Princípio

**Readiness concede uma oportunidade limitada de progresso; não entrega o servidor inteiro a uma Connection.**

E:

```text
ready != unbounded
WOULD_BLOCK != failure
one failed client != server failure
select() != permanent architecture
```

## Histórico de revisões

| Revisão | Data | Descrição |
|---:|---|---|
| 1 | 2026-09-06 | Formalização da arquitetura de scheduling não bloqueante, bounded fairness, failure isolation e separação entre contrato durável e baseline Win32 baseada em select(). |
