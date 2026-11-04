# Especificação focal da evidência de atribuição — NÃO AUTORIZA EXECUÇÃO

Continuação de NEXT-HANDLE-ORIGIN-EVIDENCE.md. Esta é uma especificação técnica, não ADR, implementação qualificada, pedido operacional pronto ou aprovação. Não altera124/128/1024.

## Resultado da busca documental

Foram pesquisados docs/integration, tools/qualification e tests por evidências de handles/tracing e pelos QPC4098010899467/4098010991760 da execução reportada. Não foi localizado registro de criação/fechamento correspondente à execução BINARY_ONLY. Não foram localizados ETL/WPRP/WPAX dentro de docs/integration nessa busca; não é alegação de ausência em todo o equipamento.

Existe análise histórica em wpr-handle-attribution-20261023/manual-result-03.md e existing-etl-wpa-handle-analysis-20261023/closeout.md. A captura corresponde ao qualifier PID444/criação134372785867046628, outro boot/artefato/operação; ETL externo de66.060.288bytes, hashc5e4bb7cdce671cb5f8bc2e42f61313a78553da815c68a5f9234013b27bb2679, conforme relatórios históricos, não recalculado nesta tarefa. Esse material descreve453 linhas de lifetime e modelo condicional167/168, não o sample132 atual. Não se transpõem ownership, quantitativos, autorização ou conclusões.

O histórico documenta que Handle.Light não estabeleceu stacks e que header PID não identifica necessariamente a tabela destinatária. Há duplicações/transferências que exigem payload decodificado e amostra exata. Logo copiar o launcher histórico não resolve a questão.

## Mecanismos existentes e limites

1. tools/qualification/wpr-handle-prepare-manual.ps1 é coordenador histórico externo, com boot236441, pins, launcher LAB e diretório exclusivos antigos. Faz efeitos próprios de tracing/arquivos/processos e invoca Prepare. Não é autorizado, compatível ou reutilizável automaticamente para esta tarefa. NÃO executar nem repinar mecanicamente.
2. tools/qualification/scm-readonly-handle-table.inc usa PSS no próprio qualifier histórico. Captura snapshot/marker, adiciona referências e pode chamar hashing para nomes. Não é observador sem custo nem prova de callsite. Não importar para o stager antes do guard: overhead desconhecido, risco de alterar justamente o fenômeno e limites.
3. O sampler atual registra contagens e QPC ao redor do conjunto de queries, não o instante exato de cada aquisição. Seus frames não são diário de handles. Imports de DLL e nomes/tipos não dão ownership transferido.

## Menor prova capaz de responder à pergunta

Usar ferramenta Windows existente de atribuição como observador externo, sem serviço auxiliar, sem substituir o processo canônico e sem deslocar seus recursos. Antes de escolher perfil/comando, demonstrar documentalmente e por qualificação offline que o mecanismo disponível pode produzir TODOS os campos abaixo. A disponibilidade histórica de WPA não prova a disponibilidade/configuração atual ou stacks suficientes.

| Evidência | Requisito de aceitação |
|---|---|
| Identidade do sujeito | PID + criação + imagem/pin + mesma tentativa; não PID isolado |
| Estado inicial | Entradas herdadas/duplicadas e aquisições desde início ou rundown consistente antes do intervalo |
| Eventos | Create/duplicate/close, tabela origem/destino, valor handle, identidade de objeto quando disponível, tempo e ordem |
| Callsite | Stack de aquisição ou evidência equivalente que identifique componente/caminho chamador; nome/tipo sozinho insuficiente |
| Fronteiras | Query pós-frame e query do sample7 delimitadas no domínio de relógio do trace com incerteza explícita |
| Integridade | Esquema/versionamento decodificado, perdas/buffers e gaps declarados; ausência de perda não prova cobertura |
| Observer | Processos/recursos próprios discriminados e overhead/kernel/arquivos/retidos explicitamente contabilizados |
| Disposição | Estado final da sessão de tracing e fechamento dos objetos controlados; não afirmar quiescência global |

Reconstruir tabela por identidade do processo e lifetime da entrada, não por valor isolado reutilizável. Duplicação entre processos é transferência/criação conforme schema, não soma cega por headerPID. Excluir handles kernel apenas com critério comprovado; preservar dúvidas. Não parear eventos de mesmo timestamp por ordem inventada.

Resultado procurado: lista das aquisições líquidas no intervalo, respectivas stacks/componentes e lifetime, com balanço comparável às queries. Se só houver janelas temporais, produzir conjunto de estados candidatos e não uma partição exata. O intervalo de QPC atual contém várias APIs; não escolher um instante que convenientemente produza132. Atribuição a shell/COM ainda não está demonstrada.

## Correção e limites de decisão

Mesmo com stack, um recurso interno não passa a pertencer ao stager. Só corrigir aquisição desnecessária controlada, duplicação comprovada ou contrato de API com liberação correspondente segura. Não CloseHandle por varredura, FreeLibrary de módulo emprestado, fechamento de STD, cancelamento de cache do OS, desconto do baseline ou aumento de quota. Contrato known-folder/proteção e referências originais permanecem obrigatórios.

Não avançar além da falha existente124/128: o sujeito conserva STOP. O observador deve poder registrar o caminho que termina em DENY, sem autorizar continuação após132, token/volume/staging ou efeitos produtivos. A prova de atribuição não pode ser classificada PASS de capacidade por ignorar o guard; ocorrência132 continua falha.

## Overhead e autorização ainda não resolvidos

Não há perfil comprovado nesta tarefa que entregue stacks/identidades/fronteiras com overhead conhecido. Nenhum ceiling, duração ou reserva é copiado do histórico LAB. O trace histórico63MiB é custo daquela captura, não limite admissível futuro. Sem fundamento para tempo/armazenamento/recursos/privacidade e confirmação de capacidade do instrumento, não há pedido byte-exato executável.

Eventual coleta pode conter nomes de objetos, caminhos e dados de outros processos: minimizar projeção, restringir acesso e finalidade, não publicar bruto nem presumir ausência de segredos. Não instalar ferramenta, alterar política/ACL ou habilitar sessão kernel automaticamente. Qualquer gravação, tracing, processo e invocação futura requer escopo explícito próprio; autorizações BINARY_ONLY/full-layout continuam consumidas.

## Entrega desta continuação

Busca e especificação concluídas; causa interna dos23 handles permanece UNKNOWN. Sem evidência existente específica, nenhuma alteração de código segura foi identificada. Não há rebuild ou campanha de teste porque código/binário não mudou. Sem coleta, trace, consulta SCM, endpoint, KSP, CA, CURRENT, TPM ou escrita protegida. Próximo trabalho técnico é verificar offline a capacidade de atribuição do instrumento existente contra esta especificação, sem executar tracing; não outra coleta agregada. Se a ferramenta não suportar os campos, registrar precisamente a limitação antes de preparar qualquer pedido.

Staging DENY; E21 PENDING; ED11/Phase6 PARTIAL; HEAD/WIP/pins/históricos e PST0.6.4 preservados.
