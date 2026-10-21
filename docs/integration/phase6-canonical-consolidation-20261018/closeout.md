# Consolidação canônica Phase 6 — decisão registrada e integração interrompida por ownership

Data: 2026-10-18. HEAD: c50b8fb9a7e3293b5e6355fc08459bf85b56857a, main.

## Governança

A autorização direta PACC-PHASE6-CANONICAL-CONSOLIDATION-GOVERNANCE-ACCEPTANCE-1 foi submetida ao executor oficial. A criação foi aprovada e executada. A decisão accepted está em docs/adr/DECISION-phase6-canonical-consolidation-20261018.md, SHA-256 D32485FE3B0829A7B8BA6D7CE8DB12453610C4DA2B7C32E0085E703471336F65. O texto registra as dez condições do owner, sem substituir ADR accepted, ampliar qualificação ou autorizar rede/admissão.

## Integração factual realizada

Foram selecionados os componentes existentes de transporte, Connection, processor/scheduler/adapter PST, outbound TLS, network stream, Secure IO e IO loop. O build seleciona corpos privados API2 preservados ou API3 normal opt-in; os 18 corpos API2 da matriz model-e-components.json permanecem byte-exatos. O SDK normal API3 é separado de R2 TEST_ONLY. Nenhum Browser/PST foi alterado.

O produto papacc_server completo compilou e linkou em Debug após corrigir a dependência privada network_stream.try_close. A primeira tentativa falhou; model-e-normal-Debug-build1.log preserva o erro. model-e-normal-Debug-build2.log registra o linkage posterior sem warnings. Binário: build/phase6-canonical-consolidation-20261018/NormalDebug/papacc_server.exe; SHA-256 4901A30A5E97C1E629491C245EFF198CF754884CF8550740602108FDBC9926F3. Não foi executado.

Depois desse build foram iniciadas correções candidatas de retenção factual em close de socket, cleanup WinSock, listener e network, incluindo propagação ao main. Essas alterações posteriores NÃO foram compiladas nem testadas. O binário Debug não representa o estado final atual das fontes. Release e regressões offline desta consolidação não foram executados. As alterações posteriores de main/application_lifetime ainda exigem isolamento/verificação do target produtivo. Não se declara preservação comportamental comprovada de produção por build nesta revisão.

## Primeiro bloqueio factual — STOP de ownership

apps/server/server_secure_io_win32.c.api3.inc, função papacc_secure_accept: após criação bem-sucedida do wrapper, um erro de attach com ownership_accepted=FALSE chega ao fail, executa pst_transport_release(transport) e depois papacc_tcp_accepted_socket_win32_close(&accepted). O alias accepted.native_socket só é invalidado quando ownership_accepted=TRUE.

Na fonte PST do commit 4dbd2783ebaf135114da2b7247e36c0ad6bc550a que originou o SDK normal, src/platform/win32/pst_transport_win32.c, pst_win32_transport_destroy fecha o socket quando consumed=0 e libera o wrapper. Portanto, no caminho pré-consumo descrito, há uma segunda tentativa de close pelo alias local. É um defeito estático condicional comprovado, sem alegação de reprodução nativa. O retorno void da liberação do wrapper também não comprova fechamento nativo em falha.

A integração dependente foi interrompida conforme a condição de STOP para lifecycle inseguro. Não foi executado esse caminho, nem criado cleanup alternativo. O código candidato permanece WIP NÃO qualificado; não habilitar runtime. A solução deve distinguir ownership do wrapper e consumo PST, invalidar aliases na fronteira factual correta e manter wrapper/recursos/dependências duráveis em incerteza sem inventar disposition PST, repetir release ou alterar PST.

## Preservação verificada

- 453 caminhos da baseline continuam presentes; nenhum arquivo foi apagado.
- 18 preimagens produtivas da matriz verificadas: zero divergências de SHA-256.
- SDK normal: 46/46 entradas SHA256SUMS verificadas, zero divergências (47 arquivos incluindo o manifest de checksums).
- Arquivo de seis binários históricos COMPOSITE-BASELINE-BINARY-PREIMAGES.zip: SHA-256 336CA151941304952565DA538D5041DDFF59943405E0EDCE72B9A48D614062A1. Os seis primeiros membros correspondem byte-exatamente aos seis hashes esperados em C8-STORAGE-historical-preservation-20261015.json, verificados por stream sem extração.
- Pin produtivo preservado: dependencies/papinho-secure-transport.txt SHA-256 3D347A0606C7AD03D2ED02B593DABA8853A2C627F85A146F28FB90C94BA05C39.
- git diff --check: exit 0; avisos existentes de conversão LF/CRLF não são warnings de compilação.
- Nenhum processo papacc_server, cl, link, ninja ou ctest encontrado na consulta final focal.
- Os dois .map históricos continuam NOT_RECOVERED.

## Estado e limites

SOURCE_INTEGRATION_RESUMED=YES_STOPPED_AT_OWNERSHIP_DEFECT
FULL_CONSOLIDATION_COMPLETED=NO
CLIENT_ADMISSION=DISABLED_UNTIL_APPROVED
ED11_ADMISSION=DENY_UNTIL_APPROVED
ED11_COMPLETE_CENSUS=NOT_ESTABLISHED_FOR_CURRENT_GRAPH
PROVIDER_KERNEL=UNKNOWN
NETWORK_TESTS=NOT_RUN
OFFLINE_TESTS=NOT_RUN_FOR_THIS_CONSOLIDATION
C8_INTEGRATED_QUALIFIED=NO
E10_STATUS=PARTIAL
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION

A aceitação de governança está concluída. A tarefa principal não está concluída. Root/policy/epoch, C2/C3 dispatch e evidência conjunta não são declarados integrados ou qualificados por este build. Não houve commit, push, tag, release, rede ou promoção de versão.