---
adr: ADR-0003
title: Connection, Session e Channel — ownership e lifecycle
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

# ADR-0003 — Connection, Session e Channel — ownership e lifecycle

## Contexto

A Phase 2 do PapinhoAccelerator consolidou três conceitos que não podem ser confundidos:

```text
Transport Connection
Session
Channel
```

Uma conexão TCP aceita é apenas um recurso de transporte. Ela não representa, por si só:

- identidade;
- autenticação;
- Session;
- Control Channel;
- Data Channel;
- autorização;
- segurança;
- negociação concluída.

A arquitetura também precisa definir quem possui cada recurso, como relações entre objetos são representadas e quais eventos de lifecycle propagam fechamento.

Sem essa separação, seria fácil:

- transformar um socket aceito diretamente em Session;
- misturar ownership físico do transporte com relações lógicas;
- duplicar referências reversas entre Connection e Session;
- tratar IDs runtime como credenciais;
- permitir que perda de CONTROL deixe uma Session parcialmente viva;
- transferir semântica de protocolo para objetos de transporte.

Esta decisão formaliza as fronteiras duráveis já estabelecidas durante a Phase 2, sem copiar detalhes de wire format, message IDs ou scheduling.

## Forças da decisão

- ownership explícito;
- lifecycle determinístico;
- separação entre transporte e contexto lógico;
- segurança futura;
- associação CONTROL/DATA;
- portabilidade;
- ausência de shared ownership implícito;
- isolamento de falhas;
- clareza de autoridade;
- possibilidade de múltiplos transports futuros;
- capacidade de evoluir protocolo sem redefinir objetos fundamentais.

## Alternativas consideradas

### Alternativa A — Connection também representa Session

Uma conexão aceita já cria ou contém diretamente o estado lógico do cliente.

**Vantagens**

- menos objetos;
- implementação inicial aparentemente simples.

**Desvantagens / trade-offs**

- mistura transporte e protocolo;
- torna difícil suportar múltiplos Data Channels;
- associa lifecycle de transporte ao lifecycle lógico de forma rígida;
- dificulta handoff, reclassificação e transportes futuros;
- pode confundir conexão com identidade/autorização.

### Alternativa B — Session possui diretamente sockets/Connections e coleções reversas

A Session mantém ownership direto das Connections e cada Connection conhece a Session e seus Channels.

**Vantagens**

- navegação direta entre objetos.

**Desvantagens / trade-offs**

- cria shared ownership implícito;
- aumenta acoplamento;
- dificulta rollback e failure atomicity;
- multiplica fontes de verdade relacionais;
- complica shutdown e remoção parcial.

### Alternativa C — Connection, Session e Channel separados, com managers e relações explícitas

Connection representa transporte, Session representa contexto lógico e Channel representa a relação funcional entre uma Connection e uma Session.

**Vantagens**

- separação clara de responsabilidades;
- ownership explícito;
- relações centralizadas;
- CONTROL/DATA podem ter lifecycles diferentes;
- transportes futuros podem ser substituídos;
- segurança pode ser inserida sem redefinir Session.

**Desvantagens / trade-offs**

- mais estruturas e managers;
- exige IDs runtime e regras de lifecycle claras;
- composição inicial é mais elaborada.

## Decisão

O PapinhoAccelerator adota três conceitos distintos:

```text
Connection != Session != Channel
```

### Connection

`Connection` representa uma conexão de transporte estabelecida.

Sua responsabilidade conceitual inclui:

- lifecycle do transporte;
- estado de associação;
- endpoints quando aplicável;
- referência ao recurso de transporte abstrato;
- identidade runtime/correlação local quando existente.

Uma Connection nova começa como:

```text
PENDING / UNCLASSIFIED
```

e permanece sem Session até que uma camada superior realize classificação e associação explícitas.

Uma Connection aceita **não é evidência de identidade, autenticação, autorização ou segurança**.

### Session

`Session` representa o contexto lógico do cliente.

Pode conter ou governar semanticamente:

- lifecycle lógico;
- identidade futura;
- policy;
- capabilities;
- relações CONTROL/DATA;
- estado de estabelecimento e ativação.

Uma Session não é um socket e não deve depender de tipos nativos de transporte.

Lifecycle conceitual:

```text
UNINITIALIZED
    ↓
ESTABLISHING
    ↓
ACTIVE
    ↓
CLOSING
    ↓
CLOSED
```

Publicação/estabelecimento inicial não torna a Session automaticamente `ACTIVE`.

A ativação deve ocorrer por transição explícita após os gates exigidos pelo protocolo/policy.

### Channel

`Channel` representa a relação lógica entre:

```text
1 Connection
       +
1 Session
       +
1 role
```

Os roles iniciais são:

```text
CONTROL
DATA
```

Um Channel não assume automaticamente ownership da Connection nem da Session.

O `Channel Manager` é a fonte relacional autoritativa.

Connection e Session não devem manter coleções/reverse pointers redundantes de Channels quando essa informação já pertence ao manager.

### Managers e autoridade de ownership

A arquitetura separa ownership físico de relações lógicas.

Conceitualmente:

```text
Connection Manager
→ ownership de Connections publicadas

Session Manager
→ ownership de Sessions publicadas

Channel Manager
→ relações Connection ↔ Session ↔ role
```

Managers podem cooperar no lifecycle, mas não devem criar shared ownership implícito do mesmo recurso.

### Transferências de ownership

Ownership de recursos de transporte deve transferir de forma explícita e exatamente uma vez.

Fluxo conceitual:

```text
native accept
    ↓
backend owns temporary accepted resource
    ↓
Connection construction/publication succeeds
    ↓
Connection owner assumes resource
```

Falha antes da publicação:

```text
close acquired resource
publish nothing
```

Nenhuma falha intermediária deve deixar ownership ambíguo.

### CONTROL governa a Session inicial

A baseline inicial possui exatamente um primary Control Channel por Session estabelecida.

```text
Session
   │
   ├── CONTROL (primary)
   ├── DATA
   ├── DATA
   └── ...
```

Perda do primary CONTROL inicia fechamento da Session.

Consequência:

```text
CONTROL lost
    ↓
Session closing
    ↓
associated DATA Channels closed/cancelled
```

Session survival, replacement de CONTROL ou resumption exigem uma decisão futura explícita.

Não podem ocorrer implicitamente.

### Falha de DATA é isolada

A perda ou fechamento de um Data Channel não encerra automaticamente:

- a Session;
- o primary CONTROL;
- outros DATA Channels.

```text
DATA lost
→ close that DATA scope
→ preserve Session + CONTROL when otherwise valid
```

### IDs runtime não são autoridade

IDs como:

```text
connection_instance_id
session_instance_id
channel_instance_id
```

são locais ao runtime e servem para:

- diagnóstico;
- correlação;
- management.

Eles não são automaticamente:

- persistentes;
- wire-visible;
- credenciais;
- prova de identidade;
- autorização;
- stable Wire IDs.

Conhecer um ID runtime nunca concede autoridade para associar uma Data Channel ou adquirir privilégios.

### Associação estrutural não é associação segura

A arquitetura distingue:

```text
internal structural binding
        !=
secure remote association
```

Um mecanismo estrutural que relaciona DATA a uma Session não deve ser interpretado como autenticação ou autorização.

Transport Security, identidade e policy podem adicionar gates posteriores sem alterar os papéis fundamentais de Connection, Session e Channel.

### Server Network permanece infraestrutura de listening

`PAPACC_SERVER_NETWORK` permanece responsável por infraestrutura de rede/listeners.

Não deve adquirir:

- Session lists;
- estado de Connections aceitas;
- autenticação;
- parsers;
- ownership de protocolo.

Connection/Session/Channel lifecycle pertence às camadas/managers apropriados acima dessa infraestrutura.

## Justificativa

Separar Connection, Session e Channel impede que a topologia de transporte determine a semântica lógica do produto.

Essa separação é necessária porque:

- uma Session pode possuir mais de uma Connection;
- CONTROL e DATA têm responsabilidades distintas;
- transportes futuros podem não ser TCP;
- identidade/autorização não podem ser inferidas de um socket;
- falhas precisam fechar apenas o menor escopo seguro;
- segurança futura precisa se encaixar sem redefinir o runtime.

Centralizar relações no Channel Manager reduz duplicação e mantém uma única fonte de verdade para bindings.

Manter IDs runtime como diagnóstico evita transformá-los acidentalmente em credenciais.

## Consequências

### Positivas

- fronteiras claras entre transporte e protocolo;
- ownership determinístico;
- suporte natural a múltiplos Data Channels;
- falha de DATA pode permanecer isolada;
- perda de CONTROL possui consequência definida;
- segurança futura pode ser adicionada por gates;
- transportes futuros podem reutilizar Session/Channel;
- runtime IDs permanecem não autoritativos;
- relações não precisam ser duplicadas em múltiplos objetos.

### Negativas / trade-offs

- mais estruturas e managers;
- operações de bind/unbind precisam ser transacionais;
- shutdown exige coordenação explícita;
- relações não podem depender de ponteiros informais espalhados pelo código.

### Neutras ou operacionais

- exatamente um primary CONTROL é a baseline inicial;
- múltiplos primary CONTROL ou Session resumption permanecem decisões futuras;
- número máximo de DATA Channels continua sujeito a policy/resource limits;
- este ADR não define wire IDs, tickets ou mensagens de protocolo.

## Regras derivadas

1. `Connection`, `Session` e `Channel` são conceitos distintos.
2. Uma Connection aceita começa pending/unclassified.
3. Uma Connection não cria Session ativa apenas por existir.
4. Connection não representa identidade ou autorização.
5. Session representa contexto lógico, não transporte nativo.
6. Channel representa relação Connection–Session–role.
7. Channel Manager é a autoridade relacional.
8. Connection e Session não devem duplicar coleções/reverse mappings de Channels sem necessidade arquitetural real.
9. Ownership de recursos de transporte deve transferir explicitamente.
10. Falha antes da publicação deve liberar recursos e publicar nada.
11. IDs runtime são diagnósticos e não autoritativos.
12. ID runtime não é automaticamente Wire ID.
13. Inicialmente existe exatamente um primary CONTROL por Session.
14. Perda do primary CONTROL inicia fechamento da Session e DATA associados.
15. Perda isolada de DATA não encerra automaticamente Session/CONTROL.
16. Structural DATA binding não equivale a associação segura.
17. Segurança/autorização futuras devem adicionar gates sem redefinir esses papéis fundamentais.
18. `PAPACC_SERVER_NETWORK` permanece infraestrutura de listening e não assume ownership de Session/protocolo.

## Impacto

### Código

Managers e APIs devem preservar separação entre Connection, Session e Channel.

Operações de publish/bind/unbind/close devem manter ownership e rollback determinísticos.

### Build e toolchains

Nenhum impacto direto.

A decisão é compatível com `PapinhoEngineering/ADR-0007`, pois Session/Channel permanecem independentes do backend concreto.

### Documentação

Specifications de wire continuam responsáveis por:

- message IDs;
- payload layouts;
- ticket formats;
- framing.

Este ADR registra apenas a arquitetura de runtime e lifecycle.

Documentos históricos da Phase 2 devem permanecer preservados e referenciar este ADR como decisão canônica quando apropriado.

### Compatibilidade

A decisão é independente de transport/backend específico.

TCP é implementação atual, não identidade de Session/Channel.

### Testes

Devem existir testes que provem, conforme aplicável:

- rollback sem publicação em falha;
- associação única e consistente;
- fechamento de Session após perda de CONTROL;
- isolamento de falha de DATA;
- ausência de autoridade derivada de IDs runtime;
- lifecycle idempotente;
- invariantes de managers e bindings.

## Verificação de conformidade

Pode-se verificar se:

- Connection não contém semântica de Session indevida;
- Session não depende de `SOCKET` ou backend nativo;
- Channel Manager permanece fonte relacional;
- accepted transport resources possuem owner inequívoco;
- failure paths não deixam recursos parcialmente publicados;
- CONTROL loss propaga fechamento da Session;
- DATA loss não derruba Session quando não há outra razão;
- runtime IDs não são usados como credenciais;
- wire specifications não foram duplicadas dentro deste ADR.

## Relações

### ADRs relacionadas

- `PapinhoAccelerator/ADR-0002` — Core portável entre deployments e backends substituíveis.
- `PapinhoEngineering/ADR-0007` — Fronteiras portáveis entre core, plataforma e backends substituíveis.
- `PapinhoAccelerator/ADR-0001` — Secure Principal e Legacy Endpoint.

### Capability Documents relacionadas

Nenhuma específica neste momento.

### Documentos relacionados

- `docs/phase2-transport-session-design.md`
- `docs/architecture.md`
- `docs/protocol-overview.md`
- `docs/control-establishment-protocol.md`
- `docs/data-association-protocol.md`
- `docs/phase2-integration-audit.md`

## Princípio

**Uma conexão transporta bytes; uma Session representa contexto lógico; um Channel relaciona os dois por um papel explícito.**

E:

```text
Connection != Session != Channel
runtime ID != authority
CONTROL loss != DATA loss
```

## Histórico de revisões

| Revisão | Data | Descrição |
|---:|---|---|
| 1 | 2026-09-06 | Formalização das decisões da Phase 2 sobre separação Connection/Session/Channel, ownership, autoridade relacional e lifecycle CONTROL/DATA. |
