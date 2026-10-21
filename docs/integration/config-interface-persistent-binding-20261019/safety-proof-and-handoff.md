# Prova focal, hunks e handoff

Norma: docs/adr/DECISION-papacc-conf-interface-ref-persistent-id-20261019.md, ACCEPTED por autorização explícita do owner e escrita oficialmente aprovada. Proposta original preservada. Precedência limitada a interface_ref; provider não aceito por referência.

| Hunk | Arquivo/função | Efeito e prova |
|---|---|---|
| 0 | config_binding.h | entrada adicional de snapshot emprestado, const, sem ownership transferido; resolved nunca produzido pelo core |
| 1a | config_binding.c/persistent_decimal | decimal ASCII uint64 exato, precheck de overflow, zero com presença; sem IDs derivados |
| 1b | config_binding.c/bind_selection | compara tokens tipados/ordem com bind_selection administrativa já válida; snapshot único -> resolver persistente existente -> bind targets; erros limpam toda saída |
| 1c | pc_bind_existing_models_snapshot | preserva validação de modelo/policy/security e AuthN; só MODEL_BOUND; chama helper sem callbacks/syscalls/alocação |
| 1d | pc_bind_existing_models | wrapper compatível sem snapshot; SELECTED retorna UNSUPPORTED, impedindo caller nativo sem proveniência de obter RESOLVED |
| 2 | config_binding_test.c | fixtures sintéticas; sem root real, rede, provider ou grants reais |

Preimagens, pós-imagens e diffs exatos em operations.json, 0.diff/1.diff/2.diff e preimages/staged. Três arquivos funcionais/teste aprovados oficialmente antes de aplicação. Duas correções também aprovadas: test-fix.diff substituiu sprintf por composição bounded; test-name.diff renomeou parâmetro que ocultava global. Logs de falhas anteriores preservados; não houve supressão, troca de toolchain ou bypass. Appends de estado vivo/futuro têm preimagens e live-operations.json próprios. Nenhuma negativa do executor nesta tarefa.

## Lifetime e autoridade

Caller mantém PC_DOCUMENT, modelos, snapshot, interfaces/endereços/presentation e saída exclusivos, não sobrepostos e imutáveis durante empréstimo síncrono. Nenhum ponteiro de input escapa, nenhuma referência/pin é criada/liberada. A função core recebe valores e NÃO pode provar sua origem administrativa. Contar/modelar identidades não autentica caller. Portanto resolved=0 sempre; sem root, revision, A/P ou I inventados. Ao erro, memset integral da saída invalida até listeners anteriores; os modelos não mudam.

Nenhuma consulta Win32, callback, lock, espera ou thread nova. A função não chama autenticação nem Stop; pré-condição de lifetime é do caller. APIs antigas, wrapper CURRENT/Stop/source, modelo de Sessions/reaper e reservas não alterados. Não alegar controle de corrida com Stop sob pin real a partir de fixtures.

## Próxima integração concreta, ainda negada

A descoberta existente em server_network.c.api3.inc é temporária, alocada durante start e liberada antes do retorno. Não existe snapshot persistente pinado no owner que possa ser passado como autoridade por um cast. Não reutilizar buffers já liberados, não criar flag current no caller, não iniciar listener para obter snapshot e não chamar start como resolver.

Para SELECTED nativo, adaptar explicitamente a fronteira administrativa existente para executar descoberta pelo produtor canônico e manter seu snapshot completo sob lifetime/revisão factual da transação, fora de qualquer janela de cópia pura e sem bloquear Stop sem limite. Orçar os buffers/retidos e provar identidade do produtor, revisão, invalidation/Stop e revalidação CURRENT. Submeter esse patch específico quando a prova existir. Até lá a entrada nativa antiga usa NULL e nega SELECTED. Isso não exige novo catálogo de interfaces nem autoridade paralela.

Provider nomeado continua pendente: requisito CLOSED usuário/senha e vínculo de provisionamento automático preservados, sem escolher um callback de certificado ou OpenSSL como substituto. Nenhum código de provider foi implementado.

Codec: os campos propostos de PersistentID valid+uint64 permanecem coerentes sob ADR-0007; grafias decimais equivalentes têm mesmo token. Não persistir índice, instância runtime, endereço ou apresentação. A norma textual não aceita layout/integridade/envelope binário. BODY0.5/ROOT06 inalterados, codec PROPOSED.
