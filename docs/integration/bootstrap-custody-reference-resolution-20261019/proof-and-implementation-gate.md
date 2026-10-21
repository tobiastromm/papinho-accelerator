# Prova focal e condição de STOP

A tarefa de implementação não foi concluída. Nenhum patch funcional foi preparado/aplicado: os dois elos de prova exigidos continuam ausentes. Não houve negativa do executor nem conflito normativo novo; trata-se de prova de implementação não estabelecida. A decisão de cerimônia já aceita não precisa ser aceita outra vez.

| Ponto real | Fato de código | Limite |
|---|---|---|
| server_configuration_commit_win32.c/path_ok | Consulta GetFileAttributesA dos componentes do caminho; comentário exclui mutação privilegiada concorrente do fault model diagnóstico | Não mantém handles dos ancestrais/contêiner; não protege o namespace até consumo |
| commit_init | Verifica arquivo ausente e inicializa domínio/owner | Ausência não prova novo contêiner nem autoridade; não usar como cerimônia |
| server_configuration_pending_win32.inc/pending_worker | CreateFileTransactedA(p->path,...,OPEN_EXISTING,...); valida identidade e bytes do predecessor | Update de objeto existente; não criação inicial sob custódia. Identidade do arquivo após abrir não prova continuidade do caminho anterior |
| PAPACC_NORMAL_RESTART_WITNESS / recovery_guard03 | Domínio/digest/revisão/file ID de estado anterior no escopo supervisionado de restart | Não é witness de primeira criação, autorização bootstrap, freshness global ou fencing |
| server_config_source_binding_win32.c/papacc_server_config_source_resolve | Exige configuration_current(o,pin), revisão exata, source publicado e validado, owner thread, Stop, reserva; depois usa pc_bind_existing_models | Resolver de update, não de bootstrap. Remover guard ou fabricar pin não cria autoridade |
| server_security_source_win32.c/source_prepare / source_resolve | Reutiliza loader de material/leases e publicação local do source; referência vinculada ao ambiente | Estado PUBLISHED do SOURCE não é CURRENT da root nem autorização administrativa da cerimônia |
| server_security_source_win32.h | Declara source lab-only e seus states/borrowers | Material carregável e callbacks existentes não comprovam aprovação administrativa. Não assumir fixtures como autoridade; tampouco inferir que todo material externo é fixture |
| PAPACC_ROOT_CONFIGURATION_PREPARATION | Já mantém source, modelo, policy, capabilities, participantes, invocation, retenção/reserva | É o owner a reutilizar; não contém atualmente prova de criação/custódia da cerimônia |

## Obrigações exatas

P3 accepted exige que a MESMA preparação observe criação exclusiva de novo contêiner, sua identidade e custódia contínua e comprove proteção contra namespace substitution. A sequência atual path_ok -> abertura posterior não demonstra isso. CREATE_NEW isolado, abrir depois de CreateDirectory ou comparar identidade tardiamente não substituem a prova. Pelo STOP seção9, não iniciar criação nem bootstrap nessa condição.

D2.1 exige autorização e identidade/integridade/lifetime factuais dos mecanismos iniciais. O resolver disponível conserva corretamente CURRENT para update; não pode ser reaproveitado no bootstrap passando pin sintético. É necessário um caminho inicial dos mesmos participantes vinculado ao ator e à custódia, sem inventar root CURRENT. Provider nomeado PENDING e SELECTED DENY permanecem.

## Próxima alteração técnica necessária

1. No adaptador existente, demonstrar uma operação que crie e adquira o contêiner protegido no mesmo namespace sob parent autorizado; manter os objetos de custódia no owner da preparação. Provar proteção de ancestrais, concorrência/substituição e objeto exato consumido pelo worker. Nenhuma API nova é escolhida ou qualificada neste relatório.
2. Verificar token efetivo e autorização administrativa antes de efeitos; vincular ação explícita/ator/invocação/candidato. SID textual, flag ou referência de configuração não bastam.
3. Reutilizar source_validate/borrow/unborrow e os validadores de policy/capabilities/binding no caminho inicial da preparação, sem alterar o resolver CURRENT de update. Só marcar RESOLVED depois de comprovar autoridade inicial e lifetime, não por callback sintético ou MODEL_BOUND.
4. Reutilizar o worker, acrescentando modo inicial explícito/predecessor ABSENT e consumo da mesma custódia; resultado durável exportável como COMMITTED_PENDING, sem CURRENT/A/P/I ou admissão. Stop não espera consultas nativas; UNKNOWN/close falho retêm ownership e crédito.
5. Submeter esse patch exato com prova e censo antes de aplicação. O presente relatório não aprova hunks, nova autoridade ou nova arquitetura. A prova nativa administrativa permanece pendente; testes sintéticos de binding não a substituem.

ED11: zero armazenamento/código acrescido; cenário anterior ALL2089223/limite2097152 é preservado, sem qualificá-lo novamente. Handles, estados, buffers e retenções futuros precisam de censo compilado;7929 bytes não são autorização. Provider/kernel UNKNOWN.
