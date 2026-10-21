# Prova focal — snapshot nativo e vínculo administrativo

## Conclusão e STOP

Não existe no estado canônico um snapshot de discovery retido/pinado pela root administrativa. Existe produtor nativo e existe participante administrativo que consome temporariamente um snapshot emprestado; eles não são o mesmo contrato de lifetime. A integração funcional foi interrompida pelos itens9 da tarefa: snapshot sem provenance administrativa/pin/lifetime comprováveis. Não houve negativa do executor nem conflito de norma. O patch oficial aprovado nesta tarefa modifica somente o teste local existente.

## Cadeia factual por função

| Objeto/fronteira | Owner e lifetime factual | Identidade/revisão/epoch/pin | Resultado |
|---|---|---|---|
| network_interfaces_win32.c / papacc_network_discover_local_snapshot | caller possui arrays de interfaces/endereços/presentation; produtor usa buffer privado GetAdaptersAddresses e o libera ao fim | Luid machine-local quando campo disponível, instanceIDs só locais; não contém root/R/A/P nem ticket | snapshot de descoberta nativa, não snapshot administrativo CURRENT |
| server_network.c.api3.inc / papacc_server_network_snapshot_discover | storage local malloc, sizing/retry limitado em número; free antes do retorno de network_start | nenhum pin CURRENT vinculado à origem da enumeração | não reutilizar após free; chamar start abriria sockets, proibido |
| server_cli_interfaces.c / papacc_server_cli_list_interfaces | buffers locais à listagem e liberação ao retornar | representação/observação do CLI; sem pin/revisão administrativa | não é catálogo nem lease da root |
| server_configuration_participants_win32.inc / prepare_listener | catalog é empréstimo do caller apenas durante a chamada; copia listener, runtime_ids, targets e invocation | participante não retém snapshot, identidade do produtor, revisão da enumeração nem handle/lease correspondente | readiness do listener não comprova snapshot CURRENT |
| PAPACC_ROOT_TRANSACTION_PARTICIPANTS | embutido no owner canônico | contém targets/IDs runtime/invocation, não catálogo completo de interfaces nem sua geração pinada | reconstruir catálogo a partir desses dados fabricaria fatos |
| root_snapshot_held/current_pin/current | pin_read valida ticket/slot e rootdomain/R/A/P/I e projeção; slots retêm configuração | proteção factual do snapshot de configuração, não dos buffers externos de discovery | não ampliar implicitamente o alcance do pin |
| config_source_binding_win32 / resolve | caller possui pin/source durante chamada síncrona; revalida CURRENT/thread/revisão/Stop/source | usa API core sem discovery snapshot | SELECTED continua UNSUPPORTED; nenhum resolved positivo espúrio |

O participante prepara listener antes do commit/publicação. Um catalog recebido então não contém automaticamente identidade da revisão CURRENT publicada depois. Reter só targets não retém origem/instância do snapshot. Uma nova revisão issuer-only e seu epoch separado não podem ser usados para dar freshness de topologia a bytes antigos. Hash, cópia ou campo current do caller também não criam essa relação.

## Stop, concorrência e retenção

O produtor chama GetAdaptersAddresses síncrono; quatro tentativas limitam número de chamadas/alocações, não a duração de cada chamada. Ele não aceita cancellation/deadline ou root Stop. Copiá-lo para o owner e checar Stop somente depois não prova resposta limitada do owner. O precedente focal active-root-safe-observation-boundary-20261018 documenta a mesma distinção entre revalidação posterior e latência de consulta; esta tarefa não reabre scheduling nem cria worker.

Não foi adicionada thread, lock, callback, coleta ou empréstimo no owner. Não há ponteiro novo para sobreviver a unpin/teardown. Stop/CURRENT/retained da entrada antiga preservados. Sem snapshot administrativo factual, os testes não simulam que existe pin nativo válido. Sem mudança nativa de interface, nenhuma afirmação de remoção/renumeração do SO.

## Matriz de validação real

| Requisito | Evidência desta composição | Limite |
|---|---|---|
| Snapshot nativo factual | teste Windows real enumera4 interfaces/8 endereços;4 PersistentIDs válidos/4 bindáveis em ambas configurações | catálogo local no teste; não CURRENT administrativo |
| PersistentID válido | cada token retornado pelo produtor resolve exatamente para sua instanceID e tem endpoints na consulta de tamanho | sem socket, configuração ou Session real |
| PersistentID inválido | cópia do ID real com is_valid=FALSE retorna INVALID_ARGUMENT e seleção vazia | negativo tipado, não remoção física |
| Remoção/ambiguidade/identidade alterada/renumeração | bridge e resolver tipado usam fixtures sintéticas existentes, inclusive instanceID19 | não topologia nativa alterada |
| Endpoint excedente | bridge sintética rejeita terceiro alvo sem truncar | conflito entre listeners factuais não exercitado |
| Pin ausente | entrypoint nativo real rejeita NULL/fake owner/ticket | não é corrida sob CURRENT real |
| Stale revision/Stop/retenção | testes antigos negam na primeira identidade root inválida; Stop interlocked próprio efetivamente solicitado | ramos internos sob pin CURRENT real NÃO comprovados |
| Snapshot substituído/concorrência durante resolução | nenhuma proteção nova implementada; sem snapshot administrativo | NÃO EXECUTADO/negação preservada, não declarar cobertura |
| Ausência de fallback | negativos sintéticos de token/modelo/snapshot e inspeção dos resolvers; native lookup usa token exato | sem teste de reconfiguração de SO |

9 CTests por configuração,18/18PASS; não18cenários independentes. Modelo54 verificações, parser121 incluem asserts auxiliares. Debug/Release build2 final /WX passa, servidor normal API3 e componentes linkados. Build inicial é preservado, mas o teste foi recompilado depois do patch; somente build2 é usado como evidência do estado final. Teste novo não cria root, não faz commit/CURRENT, não autentica cliente, não abre listener nem envia rede. Outputs não publicam IDs de interface, nomes ou endereços, somente contagens.

## Menor dependência de integração

Antes de chamar a API core com snapshot não-NULL na entrada nativa, a fronteira administrativa existente precisa possuir uma captura completa feita pelo produtor canônico, sob empréstimo/pin verificável e identidade da máquina/root e da revisão aplicável. Deve preservar buffers e presentation até concluir uso, invalidar geração substituída, revalidar CURRENT e Stop e negar alterações incertas. O pin de configuração e o lease de discovery precisam de relação explícita; não são intercambiáveis.

Isso deve ser implementado na preparação/ownership existentes, sem novo catálogo/registry/autoridade e com custo/resposta de Stop comprovados. A fonte atual não fornece essa captura pinada. Não inventar aqui interface de registro de snapshots externos nem flag de confiança para aceitar bytes do caller. A alternativa síncrona ingênua no owner não tem prova de latência; alternativa fora do owner exige demonstrar o mecanismo existente e seu ownership, não criar silenciosamente novo coletor/worker. Nenhuma decisão de arquitetura adicional é inferida como accepted.

Codec/aplicação: layout continua PROPOSED; nenhum BODY0.5/ROOT06 ou CURRENT alterado. Resolver token decimal aceito não aceita layout binário. Provider nomeado PENDING/rejeitado, CLOSED e provisionamento automático preservados sem implementação nova.
