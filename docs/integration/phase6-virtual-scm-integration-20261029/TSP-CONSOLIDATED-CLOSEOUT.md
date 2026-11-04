# Primeira sessão TRANSPORT_ONLY — continuação e plano factual

Task PACC-PHASE6-TO9-EXACT-TRANSPORT-SESSION-READINESS, 2026-10-30.
Branch main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preservados.

## Resultado e fronteira

A continuação nativa foi integrada e qualificada offline no mesmo owner,
sem novo serviço, root, endpoint, fonte de aprovação ou IPC. O plano está
concretizado em TSP-SESSION-PLAN.json e TSP-ORIGINAL-BINDINGS.csv, mas NÃO é
pedido operacional: faltam limites quantitativos nativos justificados e os
originais operacionais. Não foram inventados valores, PID/token futuros,
manifestos ou autoridade. Não há comando executável com placeholders.

TO0–TO9, OHI, B e SSO-B continuam accepted e imutáveis. N0/T0–T14 não foram
alterados nem reutilizados. Nenhuma nova ADR ou seleção foi solicitada.

## Integração funcional

scqr_continue_transport encadeia as primitivas existentes depois da aquisição
original e conexão legítimas, sem construí-las por inferência. Usa somente
estados já retidos no driver/transport; faz no máximo uma submissão/poll de I/O
por chamada, sem loop, sleep, retry ou callback permissivo. Trata avanço parcial,
pending/completion, origem, destinatário, consumo/replay, Stop e retenção. Falha
ou Stop com I/O pendente conserva OVERLAPPED/buffer/referências e pede cancelamento,
sem afirmar conclusão ou devolver créditos.

papacc_configuration_personal_scq_transport_continue liga essa sequência ao
owner canônico: exige a decisão original de endpoint coincidir com a decisão
SCQ e purpose2, referências/peer atuais e o mesmo packet/thread. Os três getters
de decisões operacionais continuam NULL. Perda de fonte/contexto com pending
solicita cancelamento e preserva UNKNOWN. READ não adquire este caminho.

O launcher/ServiceMain fechado pode realizar uma continuação limitada no mesmo
packet já adquirido; um owner vazio continua a encerrar sem efeitos. Não foi
introduzido um executor que cria fonte/endpoint ou faz polling sem política:
pendência conserva o owner sob os contratos existentes. A disponibilização real
do par, a aquisição/admissão original e a cadência/lifecycle operacional ainda
não foram ativadas/provadas. Não confundir uma API ligada com prova nativa.

## Correção de suficiência de readiness

Foi reportado GOVERNANCE CONFLICT entre a suficiência anterior do código e TO6:
close produzido/escrito no receptor não demonstra que o controlador já aplicou
e aceitou o SQC1 original. Os modelos anteriores coordenavam ambas as pontas,
mas isso não era um binding de retorno entre processos. O problema foi corrigido
no código sob a norma existente, sem modificar sua decisão.

O mesmo transporte duplex retorna o MESMO SQC1 após o controlador confrontar
seus IDs originais de entrega. O receptor confronta esse retorno com seus IDs
realmente fechados e com o controlador original, além de todas as verificações
independentes de fonte/token/imagem/pedido/plano/peer. Operações privadas locais
16 (escrita pelo controlador) e32 (recepção pelo runtime) são restritas a
TRANSPORT_ONLY. Não são novos campos wire/persistidos, não transferem autoridade,
não mudam SQR1 nem o corpo SQC1, não se aplicam a READ ou a N0. O novo campo
close_confirmed é estado do consumidor validado, não aprovação ou setter.

Readiness fechada agora exige o retorno validado. Um eco DATA, escrita concluída,
identidade de endpoint isolada ou campo preenchido pelo caller não bastam. O
produtor só envia o original já aceito e o consumidor exige o seu próprio close
anterior, tentativa/record integral, IDs, continuidade e peer. Não há downgrade
ou interoperabilidade presumida com imagens anteriores; imagem/callers devem
estar vinculados aos artefatos finais selecionados. Resultado terminal de uma
continuação é local, não readiness produtiva ou quiescência global.

Um negativo novo reproduziu substituição do buffer antes da escrita de retorno:
o código detectava a divergência após WriteFile. A validação foi antecipada para
antes de cada submissão de retorno, incluindo prefixo/record/IDs originais. O
regression log preserva a falha; a versão final exige zero chamadas de escrita
nesse caso. Não foi enviado conteúdo por endpoint real em qualquer teste.

## Evidências novas e reutilizadas

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings finais.
Nove CTests afetados/configuração,36 execuções finais PASS. Não repetidas as
campanhas criptográficas, TPM, Browser, issuer ou outros testes independentes.

Novo componente:22 cenários/1290 verificações por configuração,5 conclusões
locais e17 retenções modeladas. Cobre fonte ausente, processo/token/propósito
substituídos, peer perdido, concorrência, I/O parcial/pending, Stop com pending,
referências ausentes, close inválido/incerto, retorno com IDs alterados, peer
incorreto, READ tentando retorno, replay e substituição antes da escrita.

A cadeia ServiceMain tem47 cenários,26 TRANSPORT_ONLY,2019 verificações/config,
8 readiness fechadas e39 retenções. Agora passa pelo retorno original antes de
Running. O componente READ/adapter mantém53 cenários/1131 verificações. Três
mutantes da continuação foram compilados e rejeitados: receipt DATA tratado
como close, resubmissão de I/O pending e readiness sem retorno do controlador.
Fontes restaurados e executáveis finais recompilados. Logs TSP-* e
TSP-QUALIFICATION.json contêm os resultados. Correções do harness (literal de
pipe e uso do wait nativo no teste de thread) foram feitas antes da evidência
final; nenhuma falha de compilação é contabilizada como mutant rejeitado.

Peers/originais/decisões/token/SCM/IO/RNG/KSP permanecem modelos explícitos;
SID/SD/thread e censo compilado são helpers nativos, não prova do par original.
Zero endpoints reais, Start/Stop, SCM/ACL, sessões, KSP/CNG real, CA/CURRENT,
rede, TPM/Q4, instalação ou operações protegidas. Autenticação bilateral nativa
NOT_DEMONSTRATED. Nenhuma aprovação operacional foi criada ou consumida.

## Plano quantitativo fundamentado versus desconhecido

Ver TSP-QUANTITATIVE-PLAN.md/JSON. O protocolo tem4 mensagens de payload:
1048 offer +1052 SQR1 +1076 SQC1 +1076 retorno original =4252 bytes de wire;
a soma de leituras/escritas dos dois atores é8504, não uma reserva duplicada.
scqt_io_result exige progresso positivo e limitado: no máximo4252 conclusões
com bytes positivos por ator para esse fluxo fixo. Isso NÃO limita polls
pendentes, latência, queries, aquisição independente ou duração global. O1024
do harness não é política operacional.

Record1048, SQR11052, SQC11076, buffer1076 permanecem. Transporte728 e driver1888
não cresceram: o campo privado de4 bytes cabe no alinhamento nativo existente;
nenhum byte reservado de formato persistido foi utilizado. Censo managed
Selected1979784/3145728 e Legacy1811216/2097152; SCM separado12924 e somas
estáticas1992708/1824140, sem aumento de quota ou crédito por key vazio.
A nova continuação tem snapshot local de plano1048, aninhado a verificações
existentes; o censo de structs não mede stack transitivo, físico/commit ou picos
provider/kernel. Estes custos continuam UNKNOWN, não zero.

Validade por contexto/evento e consumo permanecem. scqp_window exige janela
finita e corrente sob o plano original, mas os valores de validade, idade/duração
de coleta, freshness, duração global, encerramento, margens físico/commit e
custos ativos ainda não têm fundamento nativo SCQ suficiente. Todos permanecem
null no plano, com fontes, fórmulas e gates definidos. Expiração pede Stop e
nega efeitos, não prova término ou quiescência. Não importados números LAB,
Browser/D4–D5 ou amostras históricas TPM; nenhum teto universal foi inferido de
fixtures ou baixa utilização observada em outro ensaio.

## Originais, artefato e primeira evidência faltante

Fontes e pontos de aquisição estão mapeados: pedido/decisão protegidos e
confirmação atribuída no controlador; alvo absoluto congelado por known folders
e identidade original; runtime SCM/TokenUser/grupos/restrições/imagem/processo
por referências e observações frescas; endpoint exclusivo; entrega mínima e
receipts de cada ator. Não há uma fonte operacional nova preenchida: os três
registros permanecem NULL e os originais não foram adquiridos nesta tarefa.
Campos observáveis somente durante a sessão não são exigidos antecipadamente.

O primeiro fundamento faltante é o envelope nativo SCQ e seus limites de
coleta/tempo/headroom, com origem suficiente para compor uma política concreta.
O plano também precisa do pedido/decisão originais e disponibilidade legítima
do par SCM; instalação, criação do parent ou Start não são efeitos implícitos.
A continuação limitada não cria nem entrega tais originais por metadados.
Sem esse conjunto, não existe pedido operacional exato completo a aprovar.

O SHA e013dd67... era candidato anterior. TSP-WORKING-ARTIFACTS.json contém os
hashes reais atuais, classificados WORKING/NOT_FROZEN/NOT_SELECTED. Não foi
solicitada nova seleção enquanto faltam fundamentos técnicos/factuais. O pin
selecionado90123fb... e sua cópia histórica ficam preservados. Somente quando o
plano estiver completo haverá congelamento e UMA seleção final, seguida de
pedido e aprovação operacionais próprios, nunca execução automática.

## Caminho até NEW_INSTALL

Concluir a política fundamentada e aquisição original sob os owners/contratos
existentes; congelar/selecionar o artefato final; então delimitar em um pedido
os efeitos realmente necessários de endpoint, lifecycle SCM/StartStop, imagem
e eventual parent. Não agregar instalação ou KSP por inferência. Após aprovação
própria, obter prova bilateral nativa/close/readiness e as observações necessárias.
Isolamento CNG MACHINE e cerimônia NEW_INSTALL/CA_INITIALIZE/publicação ficam
em escopos separados. Não se exige CA prévia para testar transporte.

Próximo prompt: NEXT-TSP-QUANTITATIVE-AND-ORIGINAL-PLAN-GATE.md. Não reabre TO0–TO9,
SSO-B, OHI ou N0. Pedido NOT_READY; E21 PENDING; ED11/Phase6 PARTIAL; DENY.
HEAD/WIP/históricos/chave fixture/UNKNOWN/reservas e PST0.6.4 preservados.
