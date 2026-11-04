# Errata focal Q6 — fechamento de entrega e custódia original

Status: CANDIDATE; exige decisão explícita. Não altera o original aprovado.
Data: 2026-11-02. Escopo exclusivo P1 PERSONAL_LOCAL TRANSPORT_ONLY.

Documento-base aprovado pelo proprietário por hash seguido de “ok”:
CONTROLLER-QUERY-REFERENCES-SQB3.CANDIDATE.md, SHA256
726e3f786b6ca187a0058734b6c9fd9e67d2d2b9d465f59bc9bd5313cbbe2c4c.
A aprovação está registrada; a compatibilidade normativa de Q6 impediu a aceitação.

## Conflito preciso

GOVERNANCE CONFLICT: Q6 atribui ao SQC3 fechamento das referências de consulta
“incluindo suas cópias locais conhecidas” antes de RUNNING diagnóstico.
OHI/Handoff: “O mesmo fluxo governado mantém os originais desde a confirmação
até o resultado terminal.” SSO-B S5 preserva lifetime de originais e validação
bilateral independente. A proposta também exige essas garantias em Q4/Q7.

As duas cópias em recipient.value.operator_process/operator_token são custódia
original usada por scqr_current, scqs_current, scqt_current e pela transição
scqr_closed_ready_current -> scqr_arm_closed_running. Fechá-las em produce_close
faria falhar essas revalidações. Ignorar as verificações ou usar somente PID/digest
não é uma correção permitida. scqr_close já exige ausência de empréstimo ativo.

## Única correção normativa solicitada

Substituir exclusivamente a expressão de Q6 “incluindo suas cópias locais conhecidas”
pela distinção abaixo, aplicável à produção/validação de SQC3 e readiness:

SQC3 comprova fechamento dos três handles de arquivos originalmente entregues e
dos dois handles de consulta originalmente entregues, identificados pelos cinco
IDs destinatários da entrega. As cópias auxiliares desses handles destinadas ao
transporte devem também estar fechadas antes da declaração de fechamento.
As referências originais independentes de custódia do recipient (processo/token),
assim como os readers originais de conteúdo/SD dos arquivos, permanecem retidas
no owner existente até o resultado terminal e a liberação legítima do empréstimo.
Elas NÃO estão cobertas pelo fechamento SQC3, nem são declaradas fechadas por ele.
Fechamento terminal dessas referências é outra evidência obrigatória; falha mantém
UNKNOWN e retenção. Nenhuma referência é fechada antes da última validação exigida.

Os dois handles de consulta recebidos alimentam as cópias de custódia verificadas;
após a transferência local, seus IDs fechados ficam em bookkeeping não utilizável.
As cópias de custódia não podem ser substituídas, reabertas por PID ou adquiridas
com direitos excedentes. SQC3 continua fechado em cinco IDs, mesma versão/tamanho,
mesmo pipe, tentativa e consumo; nenhum novo frame ou canal será criado.

Todo o restante de Q0–Q7 permanece como aprovado, inclusive SQB3=1100bytes,
SQC3=1092bytes, direitos mínimos0x00101000/TOKEN_QUERY8, ausência de impersonação,
proteção, autenticação bilateral, quotas G3-P1, Stop, deadlines e DENY produtivo.
O censo44 já distingue capture_controller/capture_token dos2slots de recipient;
não se adicionam handles nem se aumenta quota por esta errata.

## Decisão e próximo passo

Aprovar esta errata autoriza registrar a decisão do documento-base COM esta
correção indivisível e concluir implementação/qualificação offline das duas
pontas. Não reabrir outras escolhas aprovadas, nem editar silenciosamente o base.
Não concede UPDATE, nova sessão, coleta, ACL/SCM ou disposição dos processos retidos.
Sem aprovação, manter STOP para implementação dependente e DENY operacional.

A falha de redação é da proposta preparada pelo agente; não constitui vazamento
ou novo defeito demonstrado no produto. A observação anterior permanece consumida.
Preservar UNKNOWN, históricos, pins, retenção, quiescência OS e PST0.6.4.
