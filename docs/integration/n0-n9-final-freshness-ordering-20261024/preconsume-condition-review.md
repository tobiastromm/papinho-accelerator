# N0START-02 — condição pré-consumo ausente e aprovação condicionada

A decisão real do owner está registrada em OWNER-APPROVAL-CONDITIONAL-N0START-20261024-02.json, SHA256 a203e711ea4bed5d04a2d471ca8225b3fcf65788d729f62a7fa469651a91cec2.22 pins atuais conferem; nenhuma fonte/binário foi alterado nesta revisão. main/HEAD913c90cf2328f7939d8f5b927640601903ac6e91 preservados. Nenhuma execução/preflight/SCM/ACL/input ou intent foi efetuado.

## Divergência comprovada

O handoff manual anterior afirma que o owner nega se intent/imagem/sink existirem antes de efeitos. O código implementa ausência do intent/launch em psa_absent (server_service_admission_win32.inc:14-20), com exclusividade e arm/consumo posteriores. SCM inicial/status/recovery são lidos em scm-instrumented-cycle-lab.inc:279-282. Porém:

- Consumo local e canônico ocorre em scm-instrumented-cycle-lab.inc:305-306.
- cycle_copy é chamado em :310, depois do consumo; cycle_open_relative(create=1) usa FILE_CREATE em scm-runtime-cycle-lab.inc:37-40. Colisão só então retorna FILE_EXISTS.
- ig_create_diagnostic é chamado em scm-instrumented-cycle-lab.inc:314, depois do consumo; o psrf_create usa CREATE_NEW e só então rejeita sink existente.

CREATE_NEW protege contra overwrite, mas NÃO comprova ausência antes de consumir. A condição expressa do owner exige essa comprovação antes de qualquer efeito. O handoff preparado pelo agente foi impreciso nesse ponto; não se trata de uma ausência factual já provada ou de autorização para ignorá-la. Não há mudança normativa nem necessidade de novo owner.

## Menor correção proposta — não aplicada

No ciclo/owner existentes, depois de validar/adquirir o pai original e exclusividade, antes da primeira geração de métricas e antes de qualquer consumo:

1. Consultar os dois nomes fixos de imagem e sink relativos ao HANDLE do pai nativo original, cuja identidade417614100:1966080:79426 já é vinculada e revalidada. Não usar somente Test-Path/caminho textual como prova canônica.
2. Reutilizar a superfície NtCreateFile/RootDirectory/OBJ_DONT_REPARSE existente, somente FILE_OPEN/query sem Create/OpenIf/overwrite. Somente STATUS_OBJECT_NAME_NOT_FOUND com ausência de handle retornado é prova elegível de ausência; acesso negado, sharing, path-not-found, reparse, tipo inesperado, API indisponível ou resultado incerto causam STOP.
3. Objeto existente é colisão, nunca adoção. Qualquer handle de consulta adquirido deve ficar no armazenamento nativo já pertencente ao owner e ter fechamento verificado; falha retém e bloqueia consumo. Reconciliar um handle de consulta transitório dentro de128, sem novo orçamento/owner/recorder.
4. Preservar os CREATE_NEW finais: uma consulta anterior não é reserva permanente do nome. A exclusividade canônica exclui outra admissão do projeto; proteção do pai exclui escrita não autorizada. Um administrador/OS equivalente pode ainda alterar estado entre consulta e criação; não prometer ausência atômica universal. Uma falha posterior conserva intent/efeitos parciais, sem retry/rollback/overwrite.
5. Executar essas consultas antes da coleta para não recolocar I/O potencialmente lento após a amostra; manter duas gerações,60s/1s, token/plan/RAW1, censo, recovery e Stop.

Testes mínimos: imagem existente; sink existente; ambos ausentes; namespace/pai/file-ID divergente; access/sharing/reparse/resultado inconclusivo; fechamento incerto; Stop; concorrência; criação de colisão após consulta; consumed0/effect0 nos casos prévios; retenção após efeitos. Quatro builds zero warnings e negativos offline, sem operação nativa/Start. Atualizar pins e submeter a operação exata corrigida ao owner. Esses testes NÃO foram executados nesta tarefa documental; fonte não foi modificada.

## Disposição e bloqueio

O recibo01 continua provando historicamente consumed0/effect0/close0/starts0/stops0. Não concede autorização reutilizável nem prova ausência atual do claim ou de conflitos. As checagens canônicas frescas permanecem obrigatórias, e nenhum objeto será apagado/rearmado para permitir a02.

EXACT-OPERATION-APPROVED.json esperado pelo launcher permanece AUSENTE. O registro condicional é deliberadamente outro arquivo, porque o binário só confere hash e não interpreta um campo de bloqueio JSON; colocá-lo no caminho executável poderia aparentar habilitação sem cumprir a condição. Não usar o hash condicional em -ExactApprovalSha256 e não copiar/renomear o registro para o caminho do launcher.

Não há comando positivo final qualificado sob a aprovação condicionada atual. Nenhuma autorização foi recusada ou presumida: o requisito factual pedido pelo owner não está implementado na ordem exigida. A preparação termina em STOP até correção focal revisada e novos pins. ED11 PARTIAL; CURRENT, assinatura e recuperação autoritativa DENY.
