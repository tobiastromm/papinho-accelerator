# SCQ — origem receptora, entrada por papel e fechamento separado

TASK: PACC-PHASE6-SCQ-NATIVE-ROLE-ENTRY-AND-CLOSE-PROOF-OFFLINE
2026-10-30. main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preservado.
Resultado: mecanismos novos QUALIFIED_OFFLINE. Startup SCM positivo, fonte
operacional real e autenticação bilateral nativa NÃO demonstrados/concluídos.
ED11/Phase6 PARTIAL; E21 PENDING; autoridade DENY.

## Implementações

`personal_scq_recipient_win32.h/.inc`: origem LOCAL do receptor no packet canônico.
Não copiar `PAPACC_PERSONAL_SCQ_ORIGINAL *` do processo controlador. A referência
controller aponta para backing local de observação do peer, não para outro owner
nem autoridade nova. Captura somente PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE
e TOKEN_QUERY; sem token duplicável/impersonável, segredo ou chave.

`scqr_acquire/current` verificam registro original independente, papel2, perfil,
processo/criação, TokenUser por original_refs, TokenId/AuthenticationId/ModifiedId,
grupos/restrições/elevação/sessão, runtime SCM e binding de chave/nome/geração do
plano. Rejeitam replay, cópia do backing, troca de peer/token e janela inválida.
CNG de conteúdo/SD e custódia dos originais permanecem no owner de inputs existente.
Agora os três digests de SD calculados são retidos (+96 bytes); objeto, conteúdo
e SD devem corresponder ao MESMO plano, não só content_verified de outra coleta.
Não se concede autoridade por hash, caminho, SID resolvido ou esse flag interno.

Estado1 é query-only: controller_consumed/consumed/borrowed continuam0. Abertura
anônima do cliente exige esse contexto validado e aprovação independente de
endpoint no root. Nessa fase o adapter permite apenas receber uma projeção limitada;
nega escrita, uso, duplicação de referências e aplicação ordinária como ato.
`scqr_admit_offer` exige decisão e inputs locais independentes/revalidados, peer
kernel correlacionado aos processos/token originais, janela e leitura exata da
projeção aprovada no contexto de origem. Só então vincula o offer de uso único
à origem local e admite o receptor existente. O produtor controlador continua
obrigado a take-before-offer. Bytes/receipt isolados nunca realizam essa transição.
Falha do caminho query-only preserva UNKNOWN sem inventar consumo local.

O positivo novo demonstra dois packets e duas origens locais DIFERENTES em modelo;
o receptor usa sua própria origem, não &controller.source.value. Não é prova de
consumo durável nem autenticação cross-principal real. Não há loader de aprovação,
setter de grant ou callback permissivo no produto.

## Callers e host existentes

No owner: `papacc_configuration_personal_scq_recipient_prepare/acquire` capturam
runtime próprio, inputs originais, CNG, imagem e origem query-only; não usam
personal_token administrativo como fallback para o runtime. `transport_enter`
compõe create/connect/connected do controlador e open/admit do receptor.
`transport_data/poll/stop` e o driver existentes governam IO e lifetime.

`papacc_server_service_scq_host_current` consulta o próprio contexto privado do
ServiceMain: callback ativo, thread original, PS_RUNNING, sem Stop ou receipt
terminal. LAB retorna sempre0. O receptor é ligado a essa evidência por chamada
direta, sem novo serviço/owner ou callback de autoridade. A identidade SCM/token
é revalidada pelo observer existente; Stop global fecha o gate antes de nova IO.
Builds sem esse bridge negam a entrada, não usam fixture como fallback.

Designações de efeito são privadas, imutáveis e permanecem TODAS NULL: original
SCQ, endpoint e probe CNG. São escopos independentes do mecanismo existente, não
novo ledger/registry persistente nem parser CLI/arquivo. Endpoint não habilita
KSP; erro anterior de proveniência não é ocultado pelo gate posterior de efeito.
NORMAL/RECOVERY, SAM console, CA_INITIALIZE e NEW_INSTALL não são novas fontes SCQ.

## Fechamento

`scqt_adopt_delivery` move para o adapter as referências locais originalmente
entregues, só após validação; não toma os readers da origem nem aliases de suas
próprias duplicatas. Esses readers continuam sob inputs até o fim da validação.
`scqt_produce_close` chama CloseHandle nas duplicatas e referências entregues,
limpa somente handles comprovadamente fechados e preserva UNKNOWN no primeiro
close incerto, sem retry/double-close. Não produz prova após falha.

Nova etapa PRIVADA SQC1 + record1048 + três identificadores de referência fechada
(24 bytes), total1076. O buffer cresce de1052 para1076; SQR1 permanece1052.
É distinta de SQR1 DATA, que fica byte/semanticamente inalterado. Permitida por B
regra3 (representação privada controlada) e regra6 (receipts/fechamento separados),
sem mudar PACC wire, ROOT/CURRENT, API pública ou formatos persistidos.

Pernas internas4/8 só são aceitas no papel/fase corretos, uma vez, após o receipt
DATA e as conclusões de IO. Validação do close repete identidade nativa do peer,
origem, record completo e as referências exatas na tabela do destinatário:
SQR1, outra tentativa, outro token/peer, outro handle do MESMO arquivo ou replay
negam. Os valores recebidos são comparados somente com os resultados originais
de DuplicateHandle; nunca são convertidos em handles usados em uma API.
O controlador pode finalizar a custódia remota pelo close separado correlacionado,
sem exigir término/Stop do serviço; término original observado continua alternativa.
Nunca executar DUPLICATE_CLOSE_SOURCE contra números remotos reutilizáveis.

Esses resultados de fechamento são MODELADOS, mesmo usando implementações Win32
reais compiladas. Não são certificação criptográfica, close de SCM real, fencing
ou quiescência de todo o owner. O close cobre somente as referências governadas;
source/query/inputs/IO/host têm seus próprios fechamentos. Ack DATA e retorno0 do
processo não se tornam prova de liberação. Pending/uncertain retém backing.

## Qualificação final

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings. Allowlist16
CTests por configuração:64 execuções PASS, sem suíte completa operacional.
Nova suíte recipient_close:31 cenários/739 checks por configuração;19 fixtures
retidos com backing estático/SIDs preservados até o fim do processo.
Regressões adapter53/1131, driver35/711, inputs41/248 e fontes/identity/dispatch/
perfil/N0 scope/roots/presenter permanecem PASS. Dois testes host product/lab
qualificam o bridge, Stop, thread e proibição de herdar LAB. As falhas intermediárias
foram corrigidas; os logs finais correspondem às fontes finais.

Reais: compilação/link Win32, helpers SID/SD, CNG hash/random e thread nos testes
correspondentes. Modelados: peers, token/SCM positivos, original op decision,
entrega/endpoint, IO e CloseHandle. Todas as chamadas de criação/conexão de
endpoint são substituídas nos testes. Nenhum endpoint real, Start/Stop, sessão
SCM/CNG, chave persistente ou instalação executados. Chave fixture anterior não
aberta; receipt histórico preservado. `git diff --check` sem erros de whitespace.

Recursos compilados: origem receptora760, transporte728 (antes664), driver1888,
inputs512 (antes416). Crescimento do packet944 bytes. Buffer compartilhado1076, com lifetime de IO.
Managed Selected1979760/3145728; Legacy1811192/2097152. Isso não comprova pico
nativo, headroom físico/commit ou custos provider/kernel. Sem novos créditos,
quotas, prazos ou valores de headroom;124/128/1024 intactos.

## Estado que NÃO foi fechado

O bridge do host não é um caminho de startup positivo. `ps_binding_ready()` segue
literal0 e ServiceMain preserva o denied-start antes do callback normal. Este
pacote NÃO completou a fonte original de admissão do startup. Instalar o serviço
por fora, alterar argc ou simplesmente aprovar endpoint não resolve essa lacuna.
Não trocar esse guard por presença de SID/registro/fixture ou por true.

A observação BPR1060 é histórica desta sequência; não repetida nem apresentada
como consulta fresca. Não foi adquirido par SCM real neste pacote. Os três
registros de operação estão vazios; target original, fonte startup e política
quantitativa específica ainda não estão completos. PID/token continuam gates
frescos, nunca identidades fictícias exigidas antes da sessão.

Pedido operacional: NOT_READY/NOT_EMITTED; nenhum comando com placeholders.
Primeiro gate restante: fonte factual de instalação/admissão no ServiceMain do
mesmo owner, com seleção/decisão originais e readiness correspondente. A ligação
positiva dessa fonte precisa ser concluída antes de pedir execução. Não declarar
que falta apenas uma aprovação. Não herdar o startup LAB/qualifier, chamar outro
serviço, abrir rede/KSP para satisfazer readiness ou exigir CA anterior ao bootstrap.

Não foi identificada necessidade demonstrada de nova família de ADR; decisões
B/OHI/M5/P3 permanecem. Se o binding de startup precisar definir nova semântica de
readiness diagnóstica, demonstrar a cláusula exata antes de ampliá-la. Executar só
o que já estiver coberto; UNKNOWN continua DENY.

Caminho: fonte startup/seleção/política -> pacote de efeitos separados -> par real
capturado fresco -> única prova bilateral autorizada -> sessão SCM/CNG própria ->
custódia MACHINE/NEW_INSTALL e publicação próprias. Parent, Start/Stop, endpoint
e KSP não são concessões implícitas uns dos outros. Sem TPM/Q4 por inferência.

Fontes, binários, logs e hashes: RCE-QUALIFICATION.json, RCE-SOURCE-HASHES.json e
RCE-ARTIFACT-HASHES.json. Próxima continuação: NEXT-RCE-SERVICE-STARTUP-ORIGINAL-SOURCE.md.
