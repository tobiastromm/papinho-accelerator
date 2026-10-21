# Root/C2/C3 canônico — closeout parcial, 2026-10-18

A transação administrativa dormente foi incorporada seletivamente ao canônico e validada offline com o SDK normal API3/SPI4. O objetivo integral de dispatch operacional C2/C3 permanece incompleto. Não houve rede, wait-set, handshake, autenticação ou admissão.

Autoridade: DECISION-phase6-canonical-consolidation-20261018 accepted, ADR-0044, ADR-0048, C2 revision 1.0 e direção explícita do owner sobre Site TLS. Nenhuma nova autoridade ou ADR foi aceita nesta implementação.

## Implementação efetiva

- Perfil opt-in PAPACC_ENABLE_CANONICAL_C8_ROOT, OFF por padrão, separado da produção API2 e do candidato normal sem root.
- Uma application root durável; o manager embutido no participante C2 canônico substitui o manager histórico duplicado. Sem novo listener ou owner paralelo.
- Fonte/material Secure, runtime e provider preparados factualmente (Connection PST não anexada), listener intent, capabilities e participante C2 real precedem apply dormente.
- BODY 0.4 explícito e persistência transacional existente: PREPARE → participantes → COMMIT factual → CURRENT/epoch emitido pelo mecanismo existente após commit. Epoch ausente antes do commit; não é revisão, ordinal ou valor fornecido pelo teste.
- APIs privadas de attachment ligam o owner C2 ao IO/Secure existentes; funções CONTROL/role conservam os fatos autenticados e os grants publicados. A chamada positiva de attachment não foi executada: cria wait-set/wake TCP, fora desta autorização sem rede.
- Proteções C8 nos caminhos genéricos de start/poll/accept e bind do listener pertencente à root. Helpers owned preservam aquisição para o ledger factual em falhas nativas.
- Teardown conserva try_shutdown, fechamento factual, retenção e dependências. Falha final de owner_finish também retém os pais. Retirement separado do rollback pré-consumo exige manager vazio, consumer fechado, ausência de Sessions/bridges/writes/referências observáveis.
- Nenhum callback de fixture usado como preparação positiva. Nenhuma Session, Principal autenticado ou grant foi fabricado.

A root expõe a transação administrativa e o attachment privado compilado. O fluxo de produto continua negando startup; não existe ativação operacional automaticamente concedida pelo novo target. O run não deve ser descrito como configuração operacional/dispatch integralmente qualificado.

## Validação

RootDebug e RootRelease: produto e testes focais compilados/linkados com MSVC 19.51 x64, /MD, SDK normal governado, zero warnings finais. Receitas e tentativas anteriores (incluindo falhas) preservadas. Houve correções de linkage dos testes: fixture de socket isolada antes da compilação; retenção e C2 vazio ligados à root real, sem stubs permissivos.

14 execuções offline finais, sete por configuração:
1. abort/rollback administrativo real, 30 assertions;
2. commit local nativo, CURRENT/epoch pós-commit e close, 40 assertions;
3. referência mantida, retenção da root e rejeição de reaquisição, 41 assertions;
4. sizeof/censo;
5. manager C2 vazio real e identidades inválidas;
6. 11 casos de ownership com retornos nativos explicitamente sintéticos;
7. entry points reais de Stop/shutdown/retention com registros sintéticos.

Logs finais: *-qualified-offline-*.log. Contagens de assertions não substituem cobertura. Commit TxF local é prova nativa de persistência neste host, não prova de rede, autenticação, Session, ED08 com dados, release sob carga ou falha nativa de closesocket. Retenção mantém deliberadamente o grafo; saída de processo não é prova de destruição.

Casos iniciais falharam por caminho relativo recusado pelo contrato de armazenamento; corrigido somente o teste para obter caminho absoluto. Foram preservados logs, sem tratar a tentativa falha como PASS.

## ED11

| Item | Bytes |
|---|---:|
| Root | 30256 |
| Participante (manager embutido) | 528728 |
| Owner C2 | 273728 |
| Uma bridge C3 | 132448 |
| Layout privado fixo (inclui itens; não somar novamente) | 1231336 |
| Arrays base | 276992 |
| Arrays Secure | 25088 |
| Scheduler | 4432 |
| Reserva bind/listener | 144 |
| Known managed declarado | 1537992 |
| Reserva combinada observada no caso offline | 1794899 |
| Limite inalterado | 2097152 |

A reserva offline inclui crédito adicional 131072 para scratch/stack coexistente, material 32768, runtime 3203 e demais créditos calculados pelo provider. Não é medição total de RAM; layout reserva também componentes ainda não ativados. PST/provider/kernel, allocator, demanda operacional e multiplicidade requerem qualificação própria. UNKNOWN permanece UNKNOWN. Admissão DENY.

## Site TLS — primeira dependência normativa

MODE_C_SITE_TLS_CATALOG=NOT_ESTABLISHED.

C2 §7, specification revision 1.0, linha 231: tls_profile_ref é catálogo opaco aprovado pelo servidor; zero exige default aprovado, não o cria. CA de mTLS e trust Site independentes.

ADR-0022 accepted estabelece SYSTEM/CUSTOM para Mode B e declara Mode C não implementado; não fornece catálogo operacional Mode C. A configuração atual associa outbound_tls_profile_ref a Network Egress; isso não concede autorização Site TLS.

Implementação histórica reutilizável:
- tests/c2_slices/c2_s6_server.c, c2_site_policy (linha 93) e configure_site (linha 199): política fixa diagnóstica;
- tests/c2_slices/c2_site_pump_peer.inc: template de fixture;
- apps/server/server_c2_owner_win32.c, configure_site/owner_activate (linhas 468/483): ponte para template e bridge existentes, sem catálogo governado.
Os escopos S6/HTTPS históricos permanecem válidos; não são promovidos a autoridade de configuração do produto.

Não foi criado catálogo, default implícito, CA, fallback ou permissão localhost. site_configured permanece falso na composição nova; política/capacidade operacional do participante permanece fechada.

Decisão normativa específica necessária: designar a fonte administrativa server-owned do perfil Mode C, o vínculo autorizado tls_profile_ref → perfil/trust/ALPN, sua aplicabilidade por root/policy e revogação. Reutilizar as implementações existentes após essa decisão; não converter o resolver Egress em autoridade.

## Preservação e limites

HEAD main c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Preimagens de arquivos preexistentes, proveniência das importações e hashes finais estão neste diretório. Uma tentativa adicional de copiar preimagem intermediária do novo run_root falhou por subdiretório inexistente; não se declara essa preimagem intermediária preservada. A fonte histórica original e seu hash permanecem disponíveis no import-provenance; não foi perdido WIP preexistente por esse erro.

Sem Browser/PST/wire/pin modificados. SDK normal e seis imagens históricas verificados. Outputs antigos preservados; novos builds RootDebug/RootRelease. Dois mapas históricos continuam NOT_RECOVERED. Não houve commit/push/release.

## Handoff mínimo

1. Revisar independentemente a transação e os guards importados, especialmente a aposentadoria pós-attachment ainda não exercitada.
2. Resolver somente a autoridade de perfil Site TLS acima; continuar trabalho independente sem inventar catálogo.
3. Integrar a ligação operacional e qualificar attachment/dispatch/listener sob autorização apropriada de wake/rede e ED11. Não executar nesta tarefa.
4. Somente depois qualificar Mode C HTTPS conjunto, grants, APPLICATION/RECORD, TLS_READY/FINAL, ED08 e Model E nativos.

CURRENT_TASK_COMPLETED=NO
SLICE_COMPLETED=YES
ROOT_C8_TRANSACTION=OFFLINE_VALIDATED_DORMANT
CONFIG_COMMITTED=NATIVE_LOCAL_OFFLINE_PASS
POLICY_CURRENT=POST_COMMIT_OFFLINE_PASS
EPOCH_FACTUAL=POST_COMMIT_OFFLINE_PASS
C2_C3_DISPATCH=PRIVATE_HOOKS_COMPILED_OPERATIONAL_WIRING_PARTIAL
LISTENER_SECURE=EXISTING_STARTUP_DENIED
GRANTS_BIND=AUTHENTICATED_HOOKS_COMPILED_NATIVE_PROOF_PENDING
APPLICATION_RECORD=COMPONENTS_COMPILED_NATIVE_PROOF_PENDING
TLS_READY_FINAL=COMPONENTS_COMPILED_SITE_PROFILE_AUTHORITY_MISSING
MODEL_E_RETENTION=OFFLINE_GRAPH_PRESERVED
ED08=SOURCE_PRESERVED_NATIVE_PROOF_PENDING
BUILDS=ROOT_DEBUG_RELEASE_PASS
WARNINGS=0_FINAL
OFFLINE_TESTS=14_OF_14_FINAL_EXECUTIONS_PASS
NETWORK_TESTS=NOT_RUN
ED11_TOTAL_STATUS=PARTIAL
CLIENT_ADMISSION=DISABLED
ED11_ADMISSION=DENY
MODE_C_SITE_TLS_CATALOG=NOT_ESTABLISHED
CATALOG_AUTHORITY=C2_SECTION7_REQUIRES_SERVER_APPROVAL_SOURCE_NOT_ESTABLISHED
REUSABLE_IMPLEMENTATION=EXISTING_C3_BRIDGE_AND_C2_SITE_TEMPLATE_ADAPTER
CANONICAL_INTEGRATION_REQUIRED=APPROVED_PROFILE_BINDING_AND_OPERATIONAL_DISPATCH
NORMATIVE_DECISION_REQUIRED=YES
ROOT_C2_INDEPENDENT_WORK=DORMANT_TRANSACTION_OFFLINE_VALIDATED
SITE_TLS_ADMISSION=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES
EXECUTOR_APPROVALS=APPROVED_FOR_EXECUTED_MUTATIONS
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=MODE_C_SITE_TLS_CATALOG_AUTHORITY_NOT_ESTABLISHED
NEXT=FOCUSED_REVIEW_AND_SITE_TLS_PROFILE_AUTHORITY_DECISION
