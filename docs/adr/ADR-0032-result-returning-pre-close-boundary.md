---
adr: ADR-0032
title: Fronteira interna result-returning de pre-close
status: accepted
decision-date: 2026-10-09
last-revised: 2026-10-09
revision: 2
scope: project
decision-makers: [Tobias Tromm]
---

# ADR-0032 — Fronteira interna result-returning de pre-close

## Status e limite

Este ADR esta em estado `accepted`. A decisao foi aprovada explicitamente pelo owner em 2026-10-09. A implementacao permanece sujeita aos slices governados e aos testes P1-P12 definidos neste documento.

O escopo e exclusivamente o contrato interno de fechamento de `PAPACC_TRANSPORT_CONNECTION` e `PAPACC_CONNECTION`. Nao ha alteracao de API/ABI publica, PST, PACC wire, Browser ou do pin PST de producao.

## Contexto

O close root generico atual copia o callback e o contexto de `PAPACC_TRANSPORT_CONNECTION`, limpa a estrutura de transporte e somente depois chama um callback `void`. Essa limpeza e a primeira operacao irreversivel: se a preparacao especifica do transporte falhar depois dela, o owner nao consegue mais alcancar o handle original pela Connection nem propagar a falha.

No adapter PST, o fechamento seguro depende da remocao confirmada do processor do wait-set antes de qualquer release. A remocao pode falhar. Um callback `void` nao consegue expressar essa falha, e o fluxo generico pode marcar a Connection `CLOSED`, remove-la do manager e permitir reconciliacao prematura do candidato TLS, embora processor, adapter, socket ou membership ainda existam.

ADR-0030 exige recuperacao coordenada pelo owner, com remocao do wait-set antes de release. ADR-0031 exige fechamento factual da Connection e satisfacao das pre-condicoes de identidade e ownership antes de tornar um candidato TLS reutilizavel.

## Decisao proposta

Adicionar a `PAPACC_TRANSPORT_CONNECTION` um hook interno e opcional de pre-close com resultado explicito. O destructive close callback permanece separado.

O modelo interno e:

```text
prepare_close(context, out_ready) -> PAPACC_RESULT

PAPACC_RESULT_OK + ready = TRUE  -> READY
PAPACC_RESULT_OK + ready = FALSE -> DEFERRED
PAPACC_RESULT diferente de OK    -> FAILED, preservando a causa factual
```

`DEFERRED` autoriza apenas progresso futuro pelo owner; nao representa sucesso final. Nenhum novo valor publico de `PAPACC_RESULT` e criado.

Transportes sem pre-condicao de fechamento deixam o hook ausente. A ausencia significa READY e preserva o comportamento atual desses transportes, inclusive o transporte TCP nativo.

Uma operacao interna result-returning de Connection deve distinguir pedido/autorizacao de close de conclusao efetiva. A entrada `CLOSING` pode registrar a intencao duravel, mas `CLOSED` somente pode ser publicado depois de READY e do close root concluido.

## Primeira operacao irreversivel

A primeira operacao irreversivel atual e limpar `PAPACC_TRANSPORT_CONNECTION` antes da chamada ao callback `void` de close. O pre-close deve ocorrer antes dessa limpeza e antes de:

- release PST/provider;
- fechamento do socket nativo;
- estado `CONNECTION_CLOSED`;
- remocao da Connection do manager;
- reconciliacao ou reutilizacao do candidato TLS.

Se pre-close nao confirmar READY, a estrutura de transporte nao pode ser limpa.

## Ownership em DEFERRED ou FAILED

Em `DEFERRED` ou `FAILED`, preservar integralmente:

- a Connection e seu estado de close pendente;
- o handle/contexto do transporte;
- o processor PST;
- o adapter PST;
- o socket nativo;
- o connection instance ID;
- a geracao do candidato e identidades correlacionadas;
- a causa factual;
- a membership no wait-set;
- a capacidade de recuperacao governada.

Nesses resultados e proibido:

- executar release PST;
- fechar o socket nativo;
- limpar callback ou contexto;
- marcar a Connection `CLOSED`;
- remover a Connection;
- marcar o candidato TLS `CLOSED` ou reutilizavel;
- criar fallback destrutivo.

Uma falha nao pode ser transformada em sucesso, close gracioso ou causa generica.

## Ordem normativa de sucesso

A ordem proposta e:

```text
TERMINAL_INTENT
-> SLOT_QUIESCE
-> CONNECTION_CLOSING
-> PRE_CLOSE
-> WAITSET_REMOVAL_CONFIRMED
-> DESTRUCTIVE_CLOSE_ROOT
-> PST_RELEASE
-> NATIVE_SOCKET_CLOSE
-> CONNECTION_CLOSED
-> CONNECTION_REMOVE
-> OWNER_RECONCILIATION
-> CANDIDATE_CLOSED
```

`SLOT_QUIESCE` impede novo scheduling e novos callbacks validos para a operacao, mas nao constitui prova de que a Connection foi fechada. A autorizacao fornecida por READY tambem nao e prova isolada de conclusao: a conclusao exige o close root, releases e transicoes subsequentes observadas.

O manager nao pode limpar a entrada nem decrementar sua contagem antes da conclusao factual do close.

## Dispatch e autoridade

`PAPACC_SERVER_SECURE_IO_WIN32`, exclusivamente em sua thread owner, e a autoridade para:

- executar ou repetir pre-close PST;
- confirmar remocao do wait-set;
- autorizar o close root;
- decidir remocao da Connection;
- reconciliar o candidato TLS.

Durante dispatch ativo e proibido:

- executar pre-close destrutivo;
- remover processor do wait-set;
- executar release PST;
- reconciliar candidato.

Protocol processors e callbacks podem registrar a intencao terminal, mas nao adquirem autoridade para liberar uma conexao PST ainda registrada. Identidade e geracao continuam obrigatorias; callback tardio nao pode retargetar nem ressuscitar outra instancia.

## Retry e integracao com ADR-0030

Nao e criado retry independente. `DEFERRED` e falha recuperavel sao tratados exclusivamente pelo owner progress definido no ADR-0030, usando:

- thread owner;
- deadline monotonico;
- progresso limitado;
- recuperacao limitada;
- estado `ERROR_TERMINAL` quando aplicavel;
- preservacao da causa factual.

A repeticao opera sobre a mesma Connection e a mesma identidade. Nao pode existir busy-loop, retry durante dispatch ou tentativa paralela fora do owner.

Este ADR complementa o ADR-0030 ao fornecer a fronteira result-returning ausente; nao o substitui nem relaxa remove-before-release.

## Integracao com ADR-0031

A reconciliacao do ADR-0031 somente pode marcar o candidato TLS `CLOSED` quando o fechamento factual da Connection estiver concluido e todas as pre-condicoes de identidade e ownership forem verdadeiras.

Em particular, desaparecimento do Protocol Slot, intencao de close ou READY isolado nao autorizam reconciliacao antecipada. `DEFERRED` ou `FAILED` mantem candidato, Connection, processor e adapter nao reutilizaveis.

## Compatibilidade normativa

A proposta e compativel com:

- ADR-0017: preserva o lifecycle de CONTROL/DATA e nao altera frames ou regras PACC;
- ADR-0023: preserva TLS_REQUIRED, causa tipada, shutdown governado, exactly-once e proibicao de half-close direcional;
- ADR-0030: concretiza remove-before-release e recuperacao phase-aware pelo owner;
- ADR-0031: impede reconciliacao antes do fechamento factual e da identidade correspondente;
- PST ADR-0005 e contrato de lifecycle PST: nao libera conexao registrada no wait-set.

O hook e interno e opcional. Nao ha novo simbolo publico, mudanca de layout publico, novo resultado publico ou mudanca de wire.

## Plano de verificacao P1-P12

### P1 — pre-close READY

Com uma Connection real elegivel, comprovar `PAPACC_RESULT_OK`, `ready=TRUE`, remocao confirmada e autorizacao para avancar exatamente uma vez.

### P2 — pre-close FAILED

Injetar falha factual de remocao. Comprovar resultado nao-OK, `ready=FALSE`, causa preservada e nenhuma operacao destrutiva subsequente.

### P3 — pre-close DEFERRED

Produzir condicao factual ainda nao pronta. Comprovar `PAPACC_RESULT_OK`, `ready=FALSE`, Connection em close pendente e progresso posterior somente pelo owner.

### P4 — idempotencia

Repetir pre-close READY e progresso posterior sobre a mesma instancia. Comprovar ausencia de segunda remocao, release ou transicao terminal.

### P5 — nenhum close antes de READY

Com DEFERRED e FAILED, comprovar zero chamadas ao destructive close root, zero release PST e zero close nativo.

### P6 — handle preservado

Com falha de remocao, comprovar que callback, contexto, adapter, processor, socket e membership permanecem alcancaveis e associados a mesma identidade.

### P7 — Connection preservada

Com pre-close nao pronto, comprovar que o manager nao limpa a entrada, nao decrementa a contagem e nao permite reutilizacao da Connection ou do candidato.

### P8 — nenhum double release

Executar falha seguida de recuperacao e sucesso. Comprovar um release PST, um provider destroy e nenhuma segunda liberacao.

### P9 — callback tardio

Entregar callback apos intencao terminal e apos close. Comprovar rejeicao por estado/identidade, nenhuma mutacao de outra geracao e nenhuma resurrection.

### P10 — Stop global

Executar Stop global durante pre-close pendente. Comprovar integracao com ADR-0030, deadline monotonico, causa preservada e ausencia de fallback destrutivo.

### P11 — reconciliacao apos sucesso

Comprovar que o ADR-0031 somente marca candidato `CLOSED` depois de Connection ausente, adapter nao registrado e fechado, processor sem connection, dispatch inativo e identidade correspondente.

### P12 — exactly-once nativo

Para recursos efetivamente adquiridos, comprovar exatamente um PST release e exatamente um native socket close. Recursos nao adquiridos permanecem fora do denominador.

## Consequencias e trabalho futuro

A implementacao exigira migracao explicita dos caminhos owner/manager que precisam observar conclusao. Manter um wrapper `void` pode preservar compatibilidade interna de chamada, mas ele somente pode registrar/encaminhar a intencao; nao pode esconder falha nem servir como prova de close concluido.

A aprovacao explicita do owner foi registrada antes da implementacao. P1-P12, C1-C12, S1-S12 e a qualificacao G2 permanecem nao executados ate que seus respectivos gates sejam cumpridos.