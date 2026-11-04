# Pré-Start retido — diagnóstico focal e correção offline

Branch main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469. WIP preservado.
Base: G3-P1-3GIB-SESSION-RETAINED-RECONCILIATION.md e consulta posterior
G3-P1-3GIB-RETAINED-READONLY-RECONCILIATION.md. Autorizações utilizadas sem retry.
SCM observado STOPPED/PID0 e configuração correta; controlador original148 vivo.
Nenhuma nova consulta, endpoint, Start/Stop, UPDATE, ACL ou encerramento executado.

## Estado factual e causa

A causa histórica exata do ERROR5 permanece UNKNOWN. LIMITED_RISK_ELIGIBLE não
prova todas as revalidações posteriores. CLOSE0 com ERROR5 no ramo qualificado
exclui start_effect1, que seria marcado ANTES da chamada StartServiceW e forçaria
ps_close a retornar ERROR_INVALID_STATE. A consulta SCM é corroborativa do estado
parado observado, não snapshot de efeitos internos, prova de fechamento ou lease.
Namespace/referências podem ter sido retidos antes desse ponto. Não inferir
consumo de cada ato a partir de sua confirmação textual.

A revisão local identificou um defeito independente, demonstrado no modelo:
ps_image_open inicializa e=ERROR_INVALID_DATA, mas o resolvedor do alvo retorna0
para e. Se uma verificação posterior de hashing/identidade/comando salta para
"done", o valor0 podia escapar sem image_ready. Correção mínima: restaurar
ERROR_INVALID_DATA após sucesso da resolução, antes das verificações seguintes.
Não altera caminho, ACL, critérios, ordem, access/share, retenção ou consumo.
O teste injeta identidade substituída APÓS resolução bem-sucedida e exige13,
estágio111 e image_ready falso. Isso fecharia uma falha de retorno permissivo,
mas não identifica retroativamente o predicado da sessão5.

## Diagnóstico integrado

Mesmo owner; dois DWORDs locais adicionais na CLI (última etapa e resultado do
prestart_close), ponteiro local entre helpers e atualizações escalares de etapa.
Nenhum estado persistente de owner/packet, handle, consulta, amostra ou relógio
adicional. A emissão SCQ_START_FIRST_CAUSE é apenas após erro e ps_close, antes
do estacionamento retido original; não cria efeito após uma falha nem altera
freshness de admissão aprovada. Contém etapa/error/start_submission_phase/
prestart_close/service_close/retained; sem conteúdo de documentos ou referências.
É última operação tentada, não tracing de cada chamada interna. Etapa0 não
prova operação ausente ou sucesso de algo não medido.

A legenda completa está no JSON de qualificação. 101–112 distinguem aquisição
alvo/abertura/hash/identidade/comando; 301–303 distinguem conexão SCM, serviço e
status; 201–208 distinguem predicados/readback/consumo/freshness antes de Start;
209 marca submissão, 210–215 distinguem percurso posterior. Subpredicados internos
podem ainda precisar de delimitação se a etapa não os desambiguar. Nenhum
NTSTATUS é inventado: helpers Win32 preservam seu retorno atual.

Rejeições e perfil produtivo não ganham autorização; READ/NEW_INSTALL/registro
mantêm semântica original e recebem ponteiro de diagnóstico NULL. Runtime LAB
usa mesma assinatura com NULL. Sem alteração pública, wire ou formato persistido.
G3-P1 3GiB/128-188-192/físico/growth/freshness/prazos intactos; sem fallback.
O armazenamento persistente e censo de owner permanecem inalterados; custos
nativos/stack total não viram máximos provados. Nenhuma nova decisão normativa.

A chamada Shell de scm_registration_target no caminho de Start permanece
identificada como hipótese de simplificação: NÃO foi alterada nem declarada
causa. Removê-la sem prova de binding substituto seria alteração distinta.

## Qualificação

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings, toolchain
MSVC14.51 x64/Ninja /W4 /WX e configurações existentes preservadas, sem reconfigurar
SDK ou backend. 20 testes/configuração =80 pares PASS. Campanha repetida somente
porque houve nova correção após primeira validação do diagnóstico.
Modelos de APIs SCM/arquivo mutantes; nenhum SCM/endpoint/RNG operacional.
Negativos focais: falha CreateFile5/etapa103, falha ReadFile30/etapa107,
resolvedor3/etapa101, identidade alterada13/etapa111 e impossibilidade de
CLOSE0 com start_effect1. Regressões originais de origem/recursos/fechamento,
perfil, autenticação, Stop, UNKNOWN, retenção e censo incluídas.
Logs e hashes: G3-P1-RETAINED-FIRST-CAUSE-QUALIFICATION.json.

Selected/Release final: 787968bytes, SHA256
a2626bf759cfe5e10f7add6515be2e2b8523c8cdc86bf6d49773010a44bf8a94.
Cópia congelada no workspace: papacc_server.retained-first-cause-787968.exe.
Instalado continua786944/ID417614100:4718592:287758/pinbf2f8c67... sem alteração.
DLLs e configuração SCM não foram modificadas. Novo candidato não é executor
administrativo, não é autoridade e ainda não está instalado.

## Primeiro gate restante e caminho mínimo

A disposição do controlador148 retido não está resolvida. STOPPED/PID0 e
Dispose do objeto do observador não liberam suas referências internas. Não há
comando autorizado de kill/Ctrl+C/fechar console/recovery/limpeza. Não preparar
retry desta sessão ou UPDATE concorrente como se a retenção tivesse desaparecido.
Primeiro é necessário delimitar, nos contratos de retenção e na coorte atual,
a disposição legítima do processo e das referências; qualquer ação operacional
exige pedido específico com efeitos/riscos e aprovação, sem ocultar UNKNOWN.
Só depois, se admissível, UPDATE U1–U7/U2 do candidato e nova sessão E/Q/S
independente, cada qual com autorização própria. Não repetir coletas já usadas.
Não há pedido operacional de UPDATE completo nesta entrega: retenção é impedimento
factual aberto, não motivo para reconstruir infraestrutura.
