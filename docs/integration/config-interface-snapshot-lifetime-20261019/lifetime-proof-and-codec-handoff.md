# Prova por função e handoff — lifetime do snapshot administrativo

## Escopo e separação de autoridade

O storage é um membro embutido do participante da root já existente; não é catálogo paralelo, registry, owner ou novo sistema de pins. A API de cópia não atesta origem nativa, CURRENT, epoch, policy ou grants. Ela resolve a perda de lifetime do empréstimo anterior. Snapshot preparado não equivale a topologia vigente para uma operação futura. A entrada nativa SELECTED continua DENY enquanto não houver associação factual pós-commit e revalidação aplicável; não ligar essa entrada a um view de preparação pending.

A decisão accepted de PersistentID e ADR-0007 permitem resolver identidade no snapshot aplicável; ADR-0009 mantém perfil/listener explícitos. Model E/ED08/ED11 limitam retenção e liberação. Nenhuma regra accepted foi alterada e nenhuma política quantitativa/freshness foi criada.

## Prova por função

| Função/objeto | Owner/entrada | Aquisição/uso/publicação | Invalidação/liberação/Stop |
|---|---|---|---|
| papacc_interface_snapshot_copy_prepare | output exclusivo zero-inicializado; input snapshot completo imutável e disjunto, propriedade do caller durante chamada | valida capacidades32/128, IDs, duplicação, flags, famílias e associação; copia valores; remove presentation pointers; present somente por último | não aloca, chama callback ou consulta SO; falhas anteriores à escrita não publicam parcial; recusa replacement de present/retained |
| papacc_interface_snapshot_copy_view | caller já possui storage e lifetime do owner | confere invocation/revisão de preparação, presence e !retained; view síncrona legacy, contratualmente const; nenhum ponteiro externo | não é lease/pin novo, não pode escapar teardown; storage/owner não podem mudar durante empréstimo; revisão/invocation divergentes retornam view vazio |
| prepare_listener | owner canônico/thread existentes via participant_candidate; source temporária imutável durante chamada | next local recebe cópia; seleção/targets usam view de next; checa orçamento e Stop/candidato antes/depois; transfere por valor para mesmo participante | view de next não escapa; arrays embutidos sem self-pointers; Stop observado antes da publicação descarta next; nenhuma consulta Win32 de recursos/enumeração no owner |
| participante.interface_snapshot | única root embutida e única invocation | mantém fatos de interface/endereço sem string de apresentação; preparação é write-once | repetição da preparação é negada; reaper/slots de Session não liberam este membro; sem old/new snapshots coexistindo por reload |
| preparation_retain | owner/thread válidos; source em estado permitido | marca copy retained antes da tentativa de reter source, inclusive se ela falhar | bytes/crédito permanecem na root, view e replacement negados; não limpar em falha incerta |
| configuration_participants_rollback / cleanup existentes | guards de zero-child, source borrow, quiescência e resultado factual de release de provider existentes | somente após teardown comprovado, caminho existente zera participante e sua cópia | falha anterior retém graph e copy; nenhuma free/CloseHandle adicionada, nenhum cleanup alternativo |

## Revisão e ausência explícita

invocation vem de o->invocation real; preparation_revision vem de t->model.revision real. Na primeira preparação, R=0 significa candidato ainda sem revisão committed, explicitamente pending; não CURRENT, não epoch e não autoridade zero. Os testes da root mostram esse estado sem inventar R/A/P. Um novo CURRENT não pode ser inferido dessa cópia; associação à revisão committed precisa usar a transação futura e seus fatos existentes, sem alterar o snapshot ou aceitar revisão obsoleta.

Origem de dados é a source emprestada da preparação existente; esta função não autentica sua origem. No teste nativo da root a source foi efetivamente produzida pelo backend canônico, com PersistentIDs reais. Isso não qualifica uma source externa arbitrária nem amplia os contratos de admissão. Emissor-only não concede automaticamente freshness de topologia.

## Concorrência e TOCTOU

A enumeração termina antes da cópia. Durante a cópia os buffers pertencem exclusivamente ao caller e não podem ser modificados; concorrência nos mesmos buffers viola o contrato de C/ownership e não é corrigida por flags ou por repetir leitura. Depois do retorno o caller pode substituir/liberar seus buffers: a cópia independe deles, comprovado por overwrite nos testes. Uma enumeração em outros buffers não altera a cópia. Tentativa em thread não proprietária é realmente executada e negada antes de publicar participante. Nenhum lock/thread/espera foi acrescentado ao produto.

Stop é conferido no início e depois da cópia/resolução bounded; não há chamadas de enumeração no trecho. O console pode sinalizar Stop; nenhum callback do provider ocorre na cópia. O teste Stop ocorre durante o pipeline de preparação, antes de prepare_listener; não afirmar injeção determinística no meio de memcpy. Um Stop posterior invalida utilização futura pelos guards existentes; sucesso não promete autoridade perpétua. Consulta nativa lenta continua fora deste trecho, sem nova alegação de latência garantida do SO.

## Validação final e limites

Debug/Release finais build3: zero warnings, linkage completo; sem troca de toolchain/SDK.9CTests +3modos root por configuração =24/24 invocações finais PASS. Testes anteriores preservados, não somados como novos cenários. Native discovery enumera4interfaces/8endereços/4PersistentIDs válidos e bindáveis; copy real usa dados nativos, metadata de invocation/revisão do teste simples são fixtures. Modos root usam única root real dormente, fixtures TLS já existentes, sem handshake/autenticação ou nova CA.

| Requisito | Evidência | Limite |
|---|---|---|
| Criação/publicação da cópia | root snapshot-copy prepara participante real com enumeração local; copy5928bytes | publicação de participante pending, nunca configuração CURRENT |
| Liberação dos buffers da source | overwrite do caller após copy preserva IDs/endpoints próprios | alteração de bytes locais, não interface do SO |
| Interface removida/alterada/ambígua/renumerada | fixtures anteriores dos resolvers e bridge; malformed ownership copy rejeitado | nenhum adapter físico/remoto alterado |
| Revisão obsoleta/substituição | view revision1 contra captura pending0 nega; segundo prepare_listener e prepare sobre present negam | não houve publicação live ou snapshot de topologia revogado pelo SO |
| Concorrência | thread diagnóstica não proprietária faz tentativa e recebe erro, sem copy publicada; handles de thread fechados factualmente | não injeta data race nos buffers que devem ser exclusivos |
| Stop | root já com source/runtime/provider preparados recebe Stop; listener/copy permanecem ausentes e rollback normal passa | Stop no meio do memcpy não instrumentado |
| Falha de cópia | excesso de capacidade e associação inválida rejeitam sem present; raiz preserva participante vazio | não simula access violation de ponteiro inválido |
| Retenção/falha de liberação | preparation_retain real conserva copy e impede view; owner_finish retorna erro/RETAINED e bytes permanecem | retenção explícita, não falha nativa de pst_connection_try_release induzida |
| Teardown | rollback antes de commit fecha provider/source pelos caminhos existentes e zera cópia; owner_finish normal passa | não certificado de destruição de Sessions autenticadas |
| CURRENT/epoch/pin | ausentes de forma factual, não fabricados; entrada antiga conserva guards | positivo SELECTED/RESOLVED sob CURRENT real NÃO executado |

Nenhum modo snapshot alcança prepare_store/commit/publish após o branch; retorna antes. Executáveis de testes foram executados apenas nesses três modos. Registros anteriores de transaction-test não foram reexecutados. Sem rede, listener, credenciais/certificados/CA novos, grants ou admissão.

## Handoff para codec/aplicação

Codec segue PROPOSED; não serializar este struct nem ponteiros como BODY0.5/ROOT06. Layout privado de storage não é formato de arquivo. Antes de levar SELECTED a RESOLVED, vincular a captura à revisão factual aplicável na transação existente, exigir source/target/identidade e contexto CURRENT/epoch/pin e revalidar Stop. Revisão pending deve continuar negada. Não inferir topologia atual de bytes históricos nem criar freshness arbitrária; source nativa/atualização de topologia continuam requisitos distintos de ownership. Provider nomeado permanece PENDING; CLOSED e provisionamento automático preservados.
