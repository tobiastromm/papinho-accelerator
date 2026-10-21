---
adr: ADR-0023
title: TLS_REQUIRED v1 não suporta NETWORK_HALF_CLOSE direcional
status: accepted
date: 2026-09-12
---

# Contexto

ADR-0017 define `NETWORK_HALF_CLOSE` como fechamento apenas da direção do
emissor. Isso possui correspondência direta em TCP, mas o contrato público do
PapinhoSecureTransport 0.6.1 oferece shutdown TLS recíproco e terminal por
`close_notify`, não uma meia-fechada direcional de application data.

# Decisão

`TCP_PLAIN` preserva `NETWORK_HALF_CLOSE` como verdadeiro half-close TCP: a
direção emissora é encerrada e a direção reversa continua utilizável.

`TLS_REQUIRED` v1 não suporta `NETWORK_HALF_CLOSE`. O servidor rejeita a
operação deterministicamente pelo caminho terminal de erro do Network Stream.
Ele não chama `pst_connection_shutdown()` para emular half-close, não converte
silenciosamente o verbo em `NETWORK_CLOSE` e não continua como se uma
meia-fechada TLS existisse.

Para `TLS_REQUIRED`, `NETWORK_CLOSE` inicia shutdown PST incremental e limitado,
seguido do fechamento do transporte e cleanup terminal. EOF sem `close_notify`
é unclean/truncated, nunca clean EOF. `NETWORK_ABORT` realiza teardown terminal
imediato e não é reportado como encerramento gracioso.

## Precedencia de terminais duplicados ou concorrentes

O primeiro terminal valido efetivamente aceito pelo owner da operacao torna-se
definitivo. A validacao anterior a aceitacao verifica identidade da operacao,
geracao, estado, autoridade do evento, compatibilidade contratual e causa
factual. Primeiro evento recebido nao significa primeiro terminal aceito.

Depois da aceitacao, tipo e causa sao imutaveis. Eventos posteriores nao
substituem o terminal, nao produzem segunda publicacao ou liberacao e nao
ressuscitam a operacao. O owner serializa a decisao no modelo de ownership e
wait-set existentes, sem exigir locking novo. Um evento interno redundante pode
ser suprimido, mas frame PACC remoto duplicado ou invalido continua sujeito ao
diagnostico e tratamento de violacao de protocolo. Terminal tardio depois do
teardown e rejeitado sem acessar objetos liberados.

Para a matriz G: CLOSE/CLOSE preserva um CLOSE; ABORT/ABORT preserva um ABORT;
CLOSE/ABORT preserva o CLOSE ja aceito e registra o ABORT tardio; ABORT/CLOSE
preserva o ABORT ja aceito e registra o CLOSE tardio; concorrencia e serializada
pelo owner para uma unica transicao efetiva. Shutdown global pode continuar sem
frame terminal PACC, deadline pode produzir NETWORK_ABORT, perda de CONTROL
mantem sua cascade propria e TLS_REQUIRED continua sem NETWORK_HALF_CLOSE
direcional.

# Matriz normativa de qualificacao A-I

A matriz abaixo registra o gate de regressao recuperado da investigacao
`PAPINHOACCELERATOR-TLS-REQUIRED-PEER-CLEAN-CLOSE-LIFECYCLE-FIX-1`.
Ela define contrato, cobertura existente, lacuna e evidencia requerida, mas nao
afirma implementacao, execucao ou aprovacao. Cada caso permanece
`NOT_QUALIFIED` ate possuir uma execucao isolada e identificada; a tabela
registra o estado factual individual.

| Caso | Contrato | Teste/cobertura existente | Lacuna | Criterio de PASS | Status |
|---|---|---|---|---|---|
| A | Peer `close_notify` inicia shutdown PST reciproco e produz exatamente um `NETWORK_CLOSE`, sem `NETWORK_HALF_CLOSE`. | `papacc.fixture_lifetime.A`, `papacc.browser_fixture.tls_clean_close` e cenario `NETWORK_CLOSE` da integracao server. | Qualificado no build isolado PST 0.6.5: shutdown `COMPLETE`, fixture graceful e terminal `NETWORK_CLOSE`. | Trace PST/Stream/PACC prova shutdown `COMPLETE`, um `NETWORK_CLOSE`, zero half-close e teardown bounded. | QUALIFIED_PST_0.6.5 |
| B | EOF sem `close_notify` resulta em truncation/`TLS_TRUNCATED`, nunca fechamento limpo. | `papacc.browser_fixture.tls_terminal_matrix` e `papacc.phase6.reference.tls_truncation_matrix`. | Qualificado no build isolado PST 0.6.5 pela matriz fragmentada `REFERENCE_CLIENT_TLS_TRUNCATED_HTTP_V2`. | Resultado PST truncado, razao terminal PACC correta, zero clean/half-close e cleanup completo. | QUALIFIED_PST_0.6.5 |
| C | TCP reset preserva erro factual de transporte; nao e inferido como fechamento TLS limpo. | `papacc.phase6.reference.tls_required_tcp_reset` estabelece PST/TLS_REQUIRED, envia um byte de application data e injeta RST por `SO_LINGER {1,0}`. | Qualificado no PST 0.6.5 com observador test-only nao invasivo que preserva `io.context` e o callback original. | `TLS_TRUNCATED`, zero falso close limpo e PST/socket/Stream/DATA com acquired=1, release attempt=1, release success=1 e referencia remanescente=0. | QUALIFIED_PST_0.6.5 |
| D | Cancelamento durante shutdown preserva a causa e nao produz falso sucesso. | `papacc.phase6.reference.tls_required_shutdown_cancel` usa PST 0.6.5 test-only/OpenSSL real e barreira BIO de escrita. | Qualificado com `PST_CLOSE_CLEAN`, shutdown reciproco `NEED_WRITE`/`INTEREST_WRITE`, `request_stop` posterior a pendencia e owner `STOPPED`. O shutdown global nao publica terminal PACC: o DATA transport fecha sem frame terminal. | Stop posterior a pendencia real, causa `SERVER_STOP_REQUESTED`, zero falso clean success, zero terminal PACC no fio e exactly-once teardown de PST/socket/Stream/DATA. | QUALIFIED_PST_0.6.5_TEST_ONLY_GATE |
| E | Deadline durante shutdown encerra de forma limitada e nao produz falso sucesso. | `papacc.phase6.reference.tls_required_shutdown_deadline` usa PST 0.6.5 test-only/OpenSSL real e barreira BIO de escrita. | Qualificado com `PST_CLOSE_CLEAN`, shutdown reciproco `NEED_WRITE`/`INTEREST_WRITE`, deadline monotonico de 5000 ms observado pelo owner e causa `SHUTDOWN_DEADLINE`. | Um `NETWORK_ABORT` generico no PACC, causa interna `SHUTDOWN_DEADLINE`, zero falso close limpo e exactly-once teardown de PST/socket/Stream/DATA. | QUALIFIED_PST_0.6.5_TEST_ONLY_GATE |
| F | Perda de CONTROL encerra Jobs, Streams e DATA dependentes conforme ownership da Session. | `papacc.phase6.reference.tls_required_control_loss` usa PST 0.6.5 test-only/OpenSSL real e barreira BIO de escrita. | Qualificado com `PST_CLOSE_CLEAN`, shutdown reciproco `NEED_WRITE`/`INTEREST_WRITE` e perda factual de CONTROL posterior a pendencia. | Owner observa `CONTROL_TRANSPORT_LOSS`, cascade DATA unica, zero recurso orfao ou resurrection e exactly-once teardown de PST/socket/Stream/DATA. | QUALIFIED_PST_0.6.5_TEST_ONLY_GATE |
| G | O primeiro terminal valido aceito pelo owner vence; terminais repetidos, conflitantes, concorrentes ou tardios nao substituem tipo/causa, duplicam publicacao/liberacao nem ressuscitam a operacao. | O selftest sintetico de `papacc_network_stream_ai_injection_test` cobre apenas o oracle local e nao qualifica G. | Executar G1 CLOSE/CLOSE, G2 ABORT/ABORT, G3 CLOSE/ABORT, G4 ABORT/CLOSE, G5 concorrente com shutdown PST realmente pendente e G6 tardio apos teardown no Stream/server I/O loop reais. | Identidade e geracao validadas; uma transicao terminal efetiva; primeiro terminal aceito preservado; tardios registrados/rejeitados conforme origem; no maximo uma publicacao contratual; PST/socket/Stream/DATA liberados exatamente uma vez quando adquiridos; zero resurrection ou acesso apos release. | NOT_QUALIFIED |
| H | DATA tardio apos terminal e rejeitado sem resurrection do Stream. | Validacoes genericas de estado existem, sem caso focal A-I identificado. | Criar regressao test-only de DATA apos terminal TLS/PACC. | Rejeicao deterministica; Stream permanece terminal; nenhum novo I/O PST/socket. | NOT_QUALIFIED |
| I | PST connection, socket, Network Stream e DATA sao liberados exatamente uma vez. | Cleanup aparece distribuido nos testes de lifecycle e integracao. | Falta prova agregada exactly-once no caso A e nos terminais B-H. | Instrumentacao test-only por recurso, baseline/final iguais e zero leak/double release. | NOT_QUALIFIED |

## Estado factual dos testes preparados

O target `papacc_network_stream_ai_injection_test` implementa modos explicitos
D-I sobre as transicoes reais de `papacc_network_stream_*`. D/E/F injetam,
respectivamente, cancelamento, deadline e perda de CONTROL enquanto o shutdown
esta pendente; G verifica rejeicao de terminal/close duplicado; H verifica
rejeicao de DATA no estado `CLOSING`; I observa o callback real de fechamento
e conta somente recursos marcados como adquiridos. O modo `selftest` valida
apenas o oracle e a instrumentacao; ele nao qualifica nenhum caso A-I.

A-F usam os fixtures reais e integrados indicados na tabela. G-I ainda exigem
execucao isolada com o pacote PST 0.6.5 governado e correlacao com os testes
integrados do server. G-I permanecem `NOT_QUALIFIED`.
## Execucao isolada PST 0.6.5

A execucao sequencial qualificou A e B no pacote OpenSSL x64 de SHA-256
`0c134e0fc842e22e80dd6b35f6e7f207a7859f097f0be5b7d0be9c08d9d05c67`.
C foi qualificado pelo teste focal
`papacc.phase6.reference.tls_required_tcp_reset`. A execucao estabeleceu
TLS_REQUIRED/PST, confirmou um byte de application data, aplicou RST por
`SO_LINGER {1,0}`, observou `TLS_TRUNCATED` e rejeitou falso fechamento
TLS limpo/`NETWORK_CLOSE` gracioso. O observador test-only preservou
`stream->io.context` e todos os callbacks exceto `io.close`; o wrapper chamou
exatamente uma vez o callback original com o contexto original. PST connection,
site socket sob ownership transferido ao PST, Network Stream e DATA attachment
registraram acquired=1, release attempt=1, release success=1 e nenhuma
referencia remanescente. D foi qualificado com o gate test-only de shutdown:
peer clean close, shutdown reciproco em `NEED_WRITE`/`INTEREST_WRITE`, Stop
posterior a pendencia e zero terminal PACC no fio durante shutdown global. E
foi qualificado com o mesmo SDK test-only: deadline monotonico de 5000 ms,
budget total do reference client de 7000 ms, expiracao observada pelo owner,
causa interna `SHUTDOWN_DEADLINE`, exatamente um `NETWORK_ABORT` generico e
uma liberacao de cada recurso adquirido. Os quatro registros de recursos
derivam a causa factual do observador e registraram `SHUTDOWN_DEADLINE`.
A-F estao qualificados no PST 0.6.5. G1 foi qualificado isoladamente no
PST 0.6.5 test-only: o primeiro `NETWORK_CLOSE` foi aceito pelo owner, DATA
passou a `TERMINAL` sem segundo dispatch wire, e a duplicata injetada na mesma
fronteira privada foi suprimida sem alterar terminal ou causa. A unica tentativa
de publicacao posterior pertenceu ao clean close do site apos shutdown PST
concluido; DATA ja estava `TERMINAL`, o framed writer retornou
`PAPACC_RESULT_INVALID_STATE` e nenhuma publicacao PACC foi efetivada. O oracle
rejeitou 9/9 variantes invalidas e PST/socket/Stream/DATA foram liberados uma vez.
G permanece `NOT_QUALIFIED` ate G2-G6; H-I permanecem `NOT_QUALIFIED`.

O valor de shutdown ainda reutiliza o timeout de establishment neste slice.
Um timeout dedicado de shutdown permanece divida explicita; a qualificacao de
E nao congela nem altera a duracao de producao.
O oracle nao aceita `NETWORK_HALF_CLOSE` para `TLS_REQUIRED`. A ordem de
qualificacao e:

```text
PST 0.6.5 qualificado
-> Accelerator integra e qualifica A-I
-> Browser requalifica Mode B
-> C0 closeout
-> C1, sob autorizacao separada
```

# Relações

Esta decisão complementa ADR-0017 sem substituir sua semântica direcional para
`TCP_PLAIN`, e especializa o profile Mode B de ADR-0022. Mode D permanece
preservado porque utiliza `TCP_PLAIN` como túnel de bytes opacos.

# Consequências

- o mesmo verbo nunca muda silenciosamente de significado conforme o mode;
- clientes Mode B v1 usam `NETWORK_CLOSE` ou `NETWORK_ABORT` para terminar;
- uma futura meia-fechada TLS real depende de contrato PST compatível e será
  tratada como PST extension candidate na fase apropriada;
- PapinhoSecureTransport 0.6.1 não é modificado por esta decisão.
