# Socket ownership corrigido e consolidação canônica retomada

2026-10-18. HEAD main c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Autorizações: decisão accepted de consolidação e tarefa direta PACC-PHASE6-CANONICAL-SOCKET-OWNERSHIP-REMEDIATION-AND-CONSOLIDATION-RESUME-1. Nenhuma ADR accepted foi alterada. A decisão canônica mantém SHA-256 D32485FE3B0829A7B8BA6D7CE8DB12453610C4DA2B7C32E0085E703471336F65.

## Transferência factual e correção

1. Socket criado/aceito pertence ao owner nativo existente.
2. Falha antes de criar wrapper mantém o alias nativo. Close consulta o resultado real de closesocket; erro preserva handle, marca incerteza sticky e impede retry/reuso.
3. Create do wrapper bem-sucedido transfere o socket para o wrapper. O mecanismo privado papacc_pst_socket_attach invalida imediatamente o alias, antes de attach. Elegibilidade/resultado do attach não são usados para inferir essa transferência.
4. Attach com consumption factual pertence ao child PST, mesmo se o resultado é erro. O consumer não fecha o socket transferido; preserva Connection e aplica Model E.
5. Attach rejeitado sem consumo mantém o wrapper no owner durável existente. Não chama release void nem closesocket através do alias invalidado. A inexistência de retorno factual de close nessa API impede declarar cleanup concluído. A contenção é explícita; não é DESTROYED nem prova de close nativo.
6. Secure candidate, outbound TLS e C3 bridge compartilham esse mecanismo, cada um com armazenamento próprio no owner já existente. Nenhum owner, listener, resolver ou Authorization Provider paralelo foi criado.

O defeito original foi confirmado também na ponte C3 importada; foi corrigido antes de instalar dispatch. Não há native close após transferência. No erro pré-wrapper, accepted/acceptor/connector/listener/network preservam aquisição e dependências. A root candidata não libera arrays/provider/pais quando IO loop ou acceptor não confirmam conclusão. Os corpos API2 ficam selecionados separadamente e preservados; main/run/application-lifetime foram separados para não propagar a correção candidata inadvertidamente à produção.

## Validação efetivamente executada

- Produto papacc_server normal API3: Debug e Release compilados/linkados com zero warnings finais.
- Produto PST0.6.4/API2: Debug e Release compilados/linkados em outputs novos, com SDK local de archive SHA-256 F1AF50C077B10FECF099EA112C4F02F5EB761E6BEFB0578CC4F3061F23D3630C. Zero warnings finais. Nenhum produto foi executado.
- Dez testes offline por configuração normal:20/20 execuções finais passaram. Lista: wire_golden, phase6.wire_runtime, connection.lifecycle, pst.api3_offline_contract, application.lifetime, application.lifetime_retained, application.graph_census, socket.ownership_offline, socket.retention_offline e c2.core_empty_offline.
- socket.ownership_offline:11 casos sintéticos usando o mecanismo real de handoff e os caminhos reais de close com PAL/PST fixos substituídos no target de teste. Cobre attach sucesso, erro após consumo, erro pré-consumo, retenção sem overwrite/retry, create failure, retorno incoerente OK sem consumo, close sucesso/exactly-once, close falho sticky, cancel/Stop repetidos de connector e WSACleanup sucesso/falha. Não cria sockets/runtime/wait-set e não comprova falha WinSock nativa.
- socket.retention_offline: entry points reais Secure Stop/try_shutdown, outbound try_close e C3 release/close/reset com registros sintéticos retidos. Não houve chamada de cleanup nativo ou fabricação de disposition PST. A prova permanece sintética.
- c2.core_empty_offline: manager real vazio, init/reinit, negação de Session/OPEN/BIND sem identidade, tick, Stop repetido e rejeição de release de slot vazio. Nenhuma Session autenticada, grant ou epoch foi fabricada.
- Os testes de lifetime verificam que registro, retenção e selagem não concedem admissão.
- Um erro de linkage do teste (nome papacc_transports.lib inexistente) foi preservado em Debug-build3.log e corrigido para papacc_connection. Não foi suprimido warning. As falhas de âncoras de edição foram locais; não houve negativa do executor.

Final-validation.log registra o último build/test dos quatro produtos e dez testes por configuração. C3-build1.log e C2-core-build1.log registram compilação dos componentes novos. processor-symbols.txt comprova referência UNDEF pst_connection_try_release no objeto candidato e pst_connection_release no objeto produtivo. Final-admission-linkage.log registra os imports OpenSSL da composição Release normal após a boundary negativa ser separada do main; a negação constante inline intermediária eliminava a composição pelo otimizador e não é a versão final. Não usar o binário intermediário como prova de linkage atual.

## Consolidação retomada: composição e pendências

| Componente | Estado factual |
|---|---|
| Owners nativos e Model E | Integração candidata e regressões offline acima; prova nativa pendente |
| Listener Secure existente | Reutilizado nas fontes, sem novo listener; candidato nega startup antes de native effects |
| C2 core | Cinco fontes existentes incorporadas com hashes de proveniência; manager/codec/session/tokens compilados |
| C3 bridge | Componente existente incorporado, handoff corrigido, métricas/controles diagnósticos isolados do normal; compilado |
| Root C8/config/policy/epoch | Transação factual ainda NÃO wired no produto; não há CURRENT/epoch/grant operacional |
| Participantes fechados existentes | Preservados; não foram instalados como segundo owner nem substituídos por callback sintético |
| C2/C3 dispatch | NÃO instalado/ativado na root factual. Bibliotecas compiladas não equivalem a componentes vivos no executável |
| C7/E10/v5, E5/E11, ED08 | Evidências anteriores preservadas; não requalificadas nem promovidas por este slice. Integração restante pendente |
| Admissão | Boundary privada retorna DENY por ausência de root committed/policy CURRENT/epoch e gate ED11 aprovado. Sem setter, fallback ou autoridade positiva |

O reparo focal e a retomada efetiva de fontes/builds/testes estão concluídos. A tarefa principal FULL-CONSOLIDATION permanece PARTIAL; não se declara seu encerramento. A primeira pendência de implementação continua sendo conectar os participantes reais à única root factual antes de permitir dispatch. Não é apresentada como conflito normativo, necessidade de nova ADR ou autorização para rede. A decisão accepted continua autoridade para o trabalho restante.

## ED11

O censo foi corrigido para usar o layout normal realmente compilado, sem o define privado C8 intermediário. Debug/Release:873176 bytes para a soma declarada de root/arrays/participantes/crédito; esse número inclui participantes declarados e não prova que estejam ativos/alocados no produto. O resultado intermediário875312 correspondia a outro layout e foi substituído somente para este censo normal.

C3 bridge sizeof=132448; duas instâncias futuras=264896. Essas instâncias não estão alocadas na root atual: projeção separada, sem reserva/admissão implícita. Não cobrar novamente o manager C2 embutido no participante. Limite2097152 inalterado. Faltam composição root completa, discovery/snapshots dinâmicos, multiplicidade/retention completos, stack/allocator e custos PST/provider/kernel. UNKNOWN permanece UNKNOWN e ED11 total PARTIAL. O mecanismo de handoff adiciona apenas slots de referência nos owners existentes, sem alocação própria, I/O ou espera. A alocação do wrapper já existente continua no SDK.

## Preservação

WIP anterior foi preservado em corpos API2/preimagens/hash manifests; nenhuma árvore foi substituída/resetada. Três preimagens complementares foram materializadas por inversão exata das edições ou fonte histórica idêntica e verificadas contra os hashes do manifest anterior antes da escrita: tcp_socket.api3, tcp_connection_transport.api3 e server_io_loop.api3. Não se presume identidade por texto semelhante.

SDK normal, SDK produtivo/pin e seis binários históricos devem ser conferidos no final-manifest.json desta pasta. Os dois .map históricos permanecem NOT_RECOVERED. Nenhum Browser/PST/wire/pin foi alterado; nenhuma rede, produção, commit/push/tag/release ou promoção0.6.5 foi executada. Todas as escritas escaladas desta retomada foram oficialmente aprovadas; erros de âncora/linkage não são negativas do executor.

CURRENT_TASK_COMPLETED=YES_FOCUSED_REMEDIATION_AND_RESUMPTION
SLICE_COMPLETED=YES
MAIN_FULL_CONSOLIDATION_COMPLETED=NO
SOCKET_ATTACH_OWNERSHIP=CORRECTED
DOUBLE_CLOSE=REMOVED_FROM_THREE_IDENTIFIED_CONSUMERS
CLOSESOCKET_RESULT=FACTUAL_IN_SOURCE_SYNTHETIC_OFFLINE_VALIDATED
MODEL_E_RETENTION=SOURCE_AND_OFFLINE_SYNTHETIC
MODEL_E_EXACTLY_ONCE=OFFLINE_SYNTHETIC_NOT_NATIVE_QUALIFIED
CANONICAL_CONSOLIDATION=RESUMED_PARTIAL
ROOT_C8=TRANSACTION_NOT_WIRED
C2_C3=COMPONENTS_COMPILED_DISPATCH_NOT_INSTALLED
LISTENER_SECURE=EXISTING_REUSED_STARTUP_DENIED
PST065_NORMAL=COMPILED_DEBUG_RELEASE_NOT_PUBLISHED
BUILDS=FOUR_PRODUCT_CONFIGURATIONS_PASS
WARNINGS=0_FINAL
OFFLINE_TESTS=20_OF_20_FINAL_PASS
NETWORK_TESTS=NOT_RUN
ED11_TOTAL_STATUS=PARTIAL_UNTIL_QUALIFIED
CLIENT_ADMISSION=DISABLED_UNTIL_APPROVED
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
FIRST_UNRESOLVED_DIVERGENCE=FACTUAL_ROOT_CONFIGURATION_POLICY_EPOCH_TRANSACTION_NOT_WIRED
NEXT=CONTINUE_EXISTING_ROOT_TRANSACTION_AND_C2_C3_WIRING_WITH_ADMISSION_DENIED