# Ligação pré-Start — autorização técnica específica solicitável

Status: CANDIDATE / NOT_APPROVED. Nenhuma execução operacional.
QUERY/G3 e SQB2 já estão autorizados para código offline; não se solicita
reaprovação de valores, formatos ou mecanismos já aceitos.

## Alteração exata pretendida

Em server_service_startup_win32.inc, manter o valor inicial
initial_receiver_source_unavailable=1 e todos os bits de negação.
Somente no ramo já condicionado a:

1. Sucesso de papacc_server_launcher_scq_start_carrier, que exige fonte
   administrativa original, referências/imagem/token/proteção/lifetime nativos
   vigentes no owner canônico, distinto de carrier recebido por SCM.
2. Sucesso de papacc_server_launcher_scq_start_resource_current, que exige o
   mesmo owner/invocação/thread, três atos atribuídos independentes SCQ/endpoint/
   Start, G3 exato selecionado, READY e deadline congelados e amostra fresca.
3. Continuação QUERY/SQB2 implementada e qualificada no mesmo artefato protegido.

Permitir que o relatório deixe de declarar a implementação receptora ausente.
Não se declara que o runtime futuro já foi autenticado. Só limpar o bit de
DELIVERY_CONTINUATION quando o namespace original também for revalidado por
papacc_server_launcher_scq_start_namespace_current. Preservar os campos
resource_pre_missing e later_obligations: não virar UNKNOWN em PASS.

Trecho exato a integrar no ramo condicional original/resource existente:

```c
  r.initial_receiver_source_unavailable=0U;
  if(!papacc_server_launcher_scq_start_namespace_current())
   r.missing_pre_start&=~PS_DIAGNOSTIC_START_NEEDS_DELIVERY_CONTINUATION;
```

Retirar a tentativa anterior de limpar esse bit FORA desse ramo; sem source
ou resource_current, availability permanece1 e Start permanece negado. Não
substituir a inicialização por0, instalar flag/callback permissivo ou aceitar
source a partir de metadata. O relatório indica disponibilidade e elegibilidade
PRÉ-Start limitada, nunca readiness ou capacidade integral.

## Fronteiras preservadas

PID/token futuros não são exigidos antes da criação. Depois do Start continuam
obrigatórias identidade/proveniência/auth bilateral, originais, validação G3 do
runtime, deadline SQB2, receipt e close antes de RUNNING diagnóstico. Nada disso
é substituído pelo relatório. Productive readiness, CA/CURRENT, KSP e rede negados.
Consumo precede efeitos; Stop, replay, perda do peer, falta de recursos,
expiração e close incerto conservam DENY/UNKNOWN e retenção.

Qualificar o relatório REAL com atores originais/quantitativos do modelo, sem
positive callback instalado no produto: falta de origem/G3/namespace nega;
três atos+G3 sem namespace não autorizam Start; namespace original válido permite
somente elegibilidade limitada com obrigações posteriores ainda abertas.
Fonte/identidade/CNG/SCM/endpoint/clock sensíveis continuam simulados nos testes.

## Por que a aprovação específica é pedida

A revisão automática rejeitou a remoção direta da guarda e, após SQB2 aprovado,
também o trecho condicional acima. Alegou possível Start/readiness antes de
provar a fonte receptora e que a aprovação SQB2 não cobria essa ligação.
O trecho não foi aplicado. Não se contorna a rejeição com outra ferramenta,
mudança de nomes ou realocação indireta do mesmo efeito.

Autorizar explicitamente APENAS essa ligação condicional e testes offline,
ou manter a guarda1. Não alterar SSO-B/G0/G3, quotas/valores ou autorizações
operacionais. Esta aprovação não é ordem de executar Start/Stop, endpoint,
update staged, SCM/ACL, CA/CURRENT, rede operacional, TPM/Q4, limpeza/refund ou
recuperação. Preservar HEAD/WIP/históricos/pins, UNKNOWN/reservas, chave fixture,
quiescência OS desconhecida e PST0.6.4.
