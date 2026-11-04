# SQB2 — deadline autenticado integrado offline

Continuação da task PACC-PHASE6-QUERY-G3-ACCEPTANCE-AND-COLD-CHAIN e da aprovação
explícita do delta SQB2. Data2026-11-01, branch main, HEAD
f341985924d29870d8ff627ed37c8a0874ad6469. WIP/históricos preservados.
**SQB2 e QUERY/G3 qualificados em modelo; ligação canônica pré-Start pendente.**

## Decisões e autorização

QUERY/G3 accepted continua integral, mesmos oito parâmetros. O proprietário
aprovou especificamente o pedido SQB2 SHA256
62548c758282dd6b828b451a01a92bc4a369843ac55f7cc9c52f8c84a25ae11c, por resposta
atribuída e texto explícito subsequente. Conferência integral/byte-exata feita;
[registro accepted](../../adr/DECISION-personal-scq-private-deadline-sqb2-20261101.md)
precedeu o código. A proposta original CANDIDATE permanece imutável e seu estado
não aprovado é histórico. Não há autorização operacional.

## Código efetivamente ligado

O mesmo owner/controlador usa os três atos originais, G3 congelado e o adapter
existentes. scqb_write emite SQB2 quando o deadline do ator original está anexado;
nega se o valor difere do ator. scqt_start_io revalida header, record, três IDs e
deadline antes de cada escrita, inclusive continuação parcial. Fonte/Start
consumidos, peer original e recursos continuam obrigatórios.

O receptor ServiceMain/ingress usa a QUERY já integrada. scqb_open_query fixa
metadata candidato, identidade/processo/token e G3 local; SQB2 é obrigatório
nesse caminho. scqb_candidates rejeita downgrade/prazo diferente antes de admitir
referências. Após verificação nativa independente de direitos/IDs/SD/conteúdo,
SCM/runtime/imagens e proveniência, a scope local congela uma vez. O deadline
autenticado precisa coincidir com o metadata já congelado, estar dentro da janela
original e continuar vivo antes de first_prepare/promote. Valores não são grants.

Receipt e close originais continuam etapas separadas anteriores ao predicado de
RUNNING diagnóstico. Não se introduziu readiness produtiva, KSP, CA/CURRENT,
serviço/root/owner/ledger/canal genérico. Source/controller não são ponteiros
transportados; referências continuam candidatas mínimas até validação.

## Formatos e limites preservados

| Elemento | Bytes |
|---|---:|
| SCQ1 codificado |308;622 caracteres com NUL|
| Record SCQ |1048|
| SQB1 |1076, preservado no perfil antigo|
| SQR1 |1052, inalterado|
| SQC1 |1076, inalterado|
| SQB2 privado |1084 = SQB1+8; magic próprio|

O campo extra ocupa1076..1083, u64 little endian GetTickCount64 do mesmo boot.
Buffer existente expandido em8bytes; campo de custódia adicional contado no
censo. SQB1 não é reinterpretado como tendo prazo, nem aceito como downgrade da
nova QUERY. N0/READ e wire público/ROOT/CURRENT/persistência permanecem intactos.
Não houve aumento das quotas124/128/1024.

## G3 e esperas

Seleção imutável: handles128/guard124, crescimento privado e working set1994944
bytes POR participante, pisos físico7239172096 e commit9823326208 bytes,
freshness100ms, janela original240000ms, duração global60000ms após READY.

ps_scq_remaining_ms centraliza o menor prazo original/global sem renovação;
controller wait, espera terminal e receiver wait passaram a usá-lo, incluindo
revalidação após ResetEvent. Ausência, expiração ou contador inválido nega.
Antes de StartService há nova validação fresca DEPOIS da conferência de imagem
e do consumo; falha não rearma o ato. Antes de I/O/aquisições, o helper G3 mede
idade até o efeito, contando todos os handles e preservando Stop/UNKNOWN.

Isso não garante cancelamento/retorno de chamadas nativas no deadline. Custos
opacos, picos entre amostras e quiescência OS continuam UNKNOWN sob G0–G9.
resource_pre_missing63 e later_obligations15 não foram limpos como prova física.

## Qualificação final

Selected/Legacy Debug/Release: quatro builds PASS, zero warnings finais.
28 CTests afetados por configuração: **112 pares teste/configuração PASS**.
Não somar reruns e campanhas históricas. [Logs/manifesto](EVIDENCE.json),
[procedimento offline](qualify.ps1).

QUERY_COLD_CHAIN:27 cenários/1430 checks por configuração, sem seleção receptora
pré-preenchida. Fonte original dos três atos, QUERY, identidades/direitos/imagem,
policy, admissão, receipt/close e prazo G3 independente são exercitados em código
real com APIs protegidas modeladas. Seis caminhos atingem o predicado de readiness
antes do fault; cinco fecham normalmente, um injeta close incerto e retém.
Inclui caso normal anterior à expiração original, coexpiração legítima,
deadline substituído/renovado, writer divergente, downgrade SQB1, partial/pending,
peer perdido, Stop, falta de receipt/close e rejeição de readiness prematura.

G0:33 cenários+15 casos QUERY de recursos e8 mutações de campos,555 checks.
Carrier:97 checks, sete casos de metadata do ServiceMain e aritmética de prazo,
incluindo expiração, excesso, formato e carry u64. Metadata válido continua DATA
e não autoriza admissão/readiness. Teste inicial novo usou prazo incorreto da
fixture; corrigido só o modelo, log de falha preservado, nenhum guard afrouxado.

SID/SD/thread helpers reais não são prova de fonte administrativa nativa.
Token/processo/SCM/endpoint/RNG/KSP/CNG dos positivos são simulados. Nenhum
endpoint ou Start real, nenhum RUNNING produtivo. Autenticação bilateral nativa
continua NOT_DEMONSTRATED. Não se declara a cadeia canônica completa pelo build.

## Primeiro bloqueio restante: ligação pré-Start

O antigo initial_receiver_source_unavailable=1 ainda impede namespace/Start no
caller canônico. A revisão automática rejeitou a ligação CONDICIONAL desse
relatório à origem controladora real, três atos/G3 e namespace originais. Alegou
possível relaxamento pré-Start fora da aprovação SQB2. O trecho não foi aplicado;
também não foi substituído por constante permissiva ou caminho indireto.

O problema de formato do deadline já está resolvido por autorização explícita.
Agora falta essa ligação, não outra seleção G3 ou novo producer QUERY.
[Pedido técnico exato](PRESTART-CONTINUATION-BINDING.REQUEST.md), SHA256
9637246ad9ad6de34ff9eb5cb9440e628a14909ea7f95cb6a2ba2b7265287f68:
resposta atribuída aprovou o hash da pergunta, mas o texto explícito subsequente
informou9637246ad9ad6e34ff9eb5cb9440e628a14909ea7f95cb6a2ba2b7265287f687,
que diverge dos bytes atuais. Reconfirmação de identidade solicitada, pendente;
nenhum registro accepted dessa ligação ou código positivo aplicado. Não é
reaprovação QUERY/G3/SQB2. Preserva identidade/bilateralidade
do runtime como obrigação posterior à criação e só indica disponibilidade da
continuação/eligibilidade limitada antes de Start, sem inventar PID/token futuros.

## Recursos, imagem e caminho mínimo

Censo Selected: ROOT367944, FIXED_GRAPH1569552, KNOWN1994992 bytes,
PROCESS_LIMIT3145728. Delta+16 no KNOWN desta etapa (buffer/custódia), sem
alterar orçamento escolhido1994944 ou crédito compartilhado. Legacy KNOWN1826424,
PROCESS_LIMIT2097152. São tamanhos controlados, não footprint/ceiling nativo.

Selected/Release workspace:710144 bytes, SHA256
b911c7d476774c3f41841fc55989a4327c612c7980b2289f70d1888b46f06b2d.
Delta+1536 contra snapshot anterior708608/139a9d87…, preservado antes do build;
+138752 contra staged histórico571392/c0f27f94…. DLLs sem delta: ssl1321472
SHA2563fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed;
crypto7217664 SHA25609eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.
Bundle candidato9249280bytes. Não é seleção operacional ou imagem instalada;
não congelar para update enquanto a ligação funcional estiver pendente.

Após ligação: qualificar o caller canônico; preparar UMA atualização governada
para o EXE existente reutilizando STAGED-IMAGE-UPDATE-PROCEDURE.CANDIDATE.md de
phase6-original-first-delivery-20261031. Stager exclusivo não autoriza overwrite
e sua autorização está consumida. A primitiva UPDATE de imagem existente ainda
precisa ser identificada/qualificada; não improvisar cópia/rename/rollback.
Só depois pedidos operacionais separados de update, endpoint/Start/eventual Stop.
Sem comando com placeholders, sem Start antes da imagem correspondente.

Autoridade DENY; pedido operacional NOT_READY; E21 PENDING;
ED11/Phase6 PARTIAL; OS_QUIESCENCE UNKNOWN. Staged/SCM não foram reconsultados ou
alterados. Preservados HEAD/WIP, históricos/pins, autorizações consumidas,
chave fixture, UNKNOWN/reservas e PST0.6.4. Nenhum update, endpoint, Start/Stop,
SCM/ACL, CA/CURRENT, rede operacional, TPM/Q4, cleanup/refund ou recuperação.

[Próximo prompt executável](NEXT-PRESTART-CONTINUATION-AND-STAGED-UPDATE.md).
# Atualização posterior — ligação pré-Start resolvida

O proprietário confirmou o hash real f68. A ligação foi aceita, integrada e
qualificada em quatro builds/112 pares PASS. Ver [fechamento sucessor](../phase6-prestart-continuation-20261101/CLOSEOUT.md).
O conteúdo abaixo é o estado histórico anterior; o próximo gate corrente é
UPDATE governado da imagem staged, não reaprovação da ligação. DENY operacional
e UNKNOWN permanecem.

