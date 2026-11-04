# Pedido técnico focal — vincular o deadline G3 no bootstrap privado

Status: CANDIDATE / NOT_APPROVED. Data de fechamento: 2026-11-01.
Não é autorização operacional ou nova família de ADRs. QUERY/G3 e seus valores
já estão accepted e selecionados; não se pede sua reaprovação.

## Bloqueio concreto

O metadata transitório de Start carrega o vencimento global calculado em READY,
mas o frame SQB1 atual só contém a janela original do plano (240000ms).
Essa janela é distinta do limite global de60000ms após READY. Não se pode
promover o vencimento recebido por SCM/carrier a prova de sua origem.
Sem um vínculo autenticado do prazo global, o receptor conserva DENY no caso
geral; coincidir com a expiração original é somente caso-limite qualificado,
não estratégia operacional de atrasar confirmações.

A revisão automática rejeitou a implementação SQB2 em duas submissões. Motivo:
considerou novo formato/payload ampliado além da autorização e incompatível
com a preservação de SCQ1/formatos. A segunda submissão incluiu o confronto com
B3 accepted (representação privada tipada) e mostrou SCQ1 separado/inalterado;
permaneceu rejeitada. A alteração NÃO foi aplicada. Não usar escrita indireta,
outro canal ou repurpose de campo para contornar a rejeição.

## Autorização de código solicitada, exata

Permitir SOMENTE implementar/qualificar offline a seguinte versão do frame
privado de bootstrap no MESMO adapter/pipe/owner, sem executar endpoint real:

| Offset | Bytes | SQB2 proposto |
|---:|---:|---|
| 0 | 4 | ASCII SQB2, distinto de SQB1 |
| 4 | 1048 | record SCQ existente, sem reinterpretar campos |
| 1052 | 24 | três IDs candidatos de destino, cada um u64 little endian, direitos mínimos existentes |
| 1076 | 8 | deadline global absoluto u64 little endian no domínio GetTickCount64 do mesmo boot |
| Total | 1084 | somente bootstrap privado, não protocolo público/persistido |

SCQ1 continua308 bytes e622 caracteres com NUL. SQB1 continua1076 bytes;
SQR1/SQC1 e seus códigos/receipts/close continuam inalterados. Não alterar
ROOT/CURRENT/PACC wire, N0/T0–T14, READ ou outro perfil. Não serializar ponteiros
como originais. Os IDs candidatos preservam a representação já qualificada;
deadline não é segredo, token ou autoridade administrativa reutilizável.

Aumentar somente o buffer de bootstrap/driver em8 bytes para comportar1084;
manter os tamanhos de receipt/close. Campos privados adicionais de custódia
devem ser incluídos no censo real/alinhamento; +8 de buffer não é máximo
transitivo nem prova de capacidade nativa. Não aumentar quotas.

Novo QUERY/G3 deverá exigir SQB2, sem downgrade para SQB1. Compatibilidade do
perfil antigo é explícita, nunca interpretar SQB1 como contendo um deadline
ausente. O escritor só pode emitir o valor congelado pelo ator G3 original,
depois dos três atos/consumo e validação do peer; revalidar antes de cada envio.
O receptor deve comparar o valor autenticado com o prazo candidato já congelado,
expiração original, tentativa/política e relógio. Divergência, renovação,
expiração ou ausência nega a promoção, conserva consumo/UNKNOWN e referências.

## Guardas preservadas

Não autoriza substituir a guarda pré-Start por constante permissiva. Após o
binding geral estar qualificado, eventual ligação dessa guarda deve exigir
fonte controladora original vigente, três atos independentes, política G3
exata, identidade/proteção da imagem correspondente e continuação receptora
implementada. Ausência de qualquer predicado conserva DENY. A autenticação
bilateral e receipt/close permanecem provas posteriores à criação do processo;
não inventar PID/token futuros nem anunciar readiness antecipadamente.

Qualificar prazo substituído/renovado, SQB1 recusado no novo QUERY, frames
parciais/pendentes, Stop, perda de peer, replay, direitos, close UNKNOWN e
readiness prematura, com APIs sensíveis simuladas, em quatro configurações.
Só depois estabilizar imagem e preparar pedidos separados de update, endpoint,
Start e eventual Stop. Nenhuma execução decorre desta decisão técnica.

## Decisão necessária

Autorizar explicitamente essa mudança privada de representação e buffer para
código/qualificação offline, ou conservar a implementação atual em DENY.
Registrar aprovação verificável antes de código dependente. Não inferir
aceitação por continuidade, builds ou pelos valores G3 já selecionados.
Sem update, endpoint, Start/Stop, SCM/ACL, CA/CURRENT, rede operacional,
TPM/Q4, limpeza/refund ou recuperação. Preservar HEAD/WIP/históricos/pins,
chave fixture, UNKNOWN/reservas, OS_QUIESCENCE UNKNOWN e PST0.6.4.
