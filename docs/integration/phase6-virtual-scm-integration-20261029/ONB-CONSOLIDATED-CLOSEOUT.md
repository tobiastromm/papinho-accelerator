# Originais da primeira sessÃ£o SCQ â€” bindings e fechamento

TASK PACC-PHASE6-SPC-ONE-NATIVE-ORIGINAL-BINDING. Branch main e HEAD
f341985924d29870d8ff627ed37c8a0874ad6469 preservados; mesmo owner/runtime.
SSO-B/S0â€“S7/OHI/cross-principal B continuam accepted e imutÃ¡veis. Sem nova ADR.

## Trabalho tÃ©cnico concluÃ­do nesta tarefa

A revisÃ£o encontrou um problema concreto no binding de lifecycle: o modelo SPC
reportava Running antes de receipt/close; os wrappers nativos observam SCM de
novo e comparariam Running atual ao record Pending original. NÃ£o foi introduzido
fallback de estado, ediÃ§Ã£o do binding, setter ou uma nova representaÃ§Ã£o.

O perfil fechado agora exige completar a troca original existente ANTES da
readiness: offer -> aquisiÃ§Ã£o local independente -> SQR1 receipt -> ack no
controller -> produtor SQC1 fecha os handles locais efetivamente entregues ->
write completo do close -> controller valida IDs contra seu DuplicateHandle
original -> Running fechado. O guard receptor exige fase4/ack e prova de close
produzida+entregue. SQR1 DATA ou offer isolado nÃ£o satisfazem essa condiÃ§Ã£o.
O caminho legado da consulta nÃ£o fechada mantÃ©m seu contrato anterior.

Os leitores independentes dos originais permanecem vivos durante a operaÃ§Ã£o;
fechar referÃªncias entregues nÃ£o fecha os leitores/provas por alias. Valores
fechados do SQC1 sÃ£o identificadores de tabela do destinatÃ¡rio, comparados ao
resultado da entrega original; nunca sÃ£o convertidos em HANDLE para fechar algo.
O controller e o receptor tÃªm receipts separados. O modelo sÃ³ anuncia Running
quando as duas metades correlacionadas passaram; falha/substituiÃ§Ã£o/replay/
fechamento incerto preservam UNKNOWN e consumo, nÃ£o dÃ£o quiescÃªncia global.

MudanÃ§as de cÃ³digo: personal_scq_recipient_win32.inc e
scq_pre_readiness_chain_test.c. Sem crescimento de structs/pools/buffers, alteraÃ§Ã£o
wire/ROOT/CURRENT ou quota. O censo SPC existente permanece aplicÃ¡vel ao layout;
custos/picos nativos e headroom reais continuam UNKNOWN. Sem novas mediÃ§Ãµes
empÃ­ricas ou ceilings. Limites124/128/1024 preservados.

## QualificaÃ§Ã£o nova, sem reabrir campanhas histÃ³ricas

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings. Cinco CTests
afetados/configuraÃ§Ã£o,20 execuÃ§Ãµes PASS. O novo fluxo possui21 cenÃ¡rios/916
verificaÃ§Ãµes por configuraÃ§Ã£o;4 readiness confirmadas antes dos fechamentos
finais e17 retenÃ§Ãµes de host modeladas. Inclui falha no produtor de close antes
de readiness, substituiÃ§Ã£o de IDs no close, receipt ausente, perda da prova de
close e entrega nÃ£o comprovada, alÃ©m de identidade, origem ausente, replay,
Stop/perda do peer e UNKNOWN. Mantidos RNG/KSP/endpoints e mutaÃ§Ãµes simulados.

Dois mutantes novos rejeitados: retirar guard de close produzido e retirar guard
de close entregue. Originais restaurados e artefatos finais recompilados. NÃ£o
repetidos os quatro mutantes histÃ³ricos SPC, inputs com CNG real ou campanhas
criptogrÃ¡ficas inalteradas. Logs/hashes ONB-* sÃ£o as evidÃªncias desta tarefa.
Nenhuma prova bilateral nativa, chamada KSP/CNG real ou operaÃ§Ã£o protegida executada.

## Bindings prÃ©vios versus frescos

A matriz ONB-ORIGINAL-BINDINGS.csv identifica16 bindings, fontes e gates. Nomes de
serviÃ§o/conta/layout sÃ£o seleÃ§Ãµes normativas, nÃ£o objetos adquiridos. HÃ¡ inventÃ¡rio
SHA256 real dos quatro executÃ¡veis compilados em ONB-CANDIDATE-ARTIFACTS.json;
ele representa bytes no workspace, nÃ£o imagem instalada/aprovada ou atestaÃ§Ã£o
mapped-image. Nenhum desses artefatos foi selecionado operacionalmente por inferÃªncia.

PID, criaÃ§Ã£o, TokenUser/estatÃ­sticas/grupos/restriÃ§Ãµes, SID resolvido, boot/contexto
atual, handles originais, nonce e endpoint sÃ£o adquiridos/revalidados na sessÃ£o
nos pontos indicados. NÃ£o se exige PID/token futuro no pedido e nÃ£o se reutilizam
SID1108, boot ou PIDs histÃ³ricos. NÃ£o foi executada coleta administrativa extra.

A prova de fechamento Ã© binding fresco original: nÃ£o Ã© um receipt DATA genÃ©rico,
hash, nome ou suposiÃ§Ã£o de quiescÃªncia. O novo ordenamento permite manter Pending
factual durante a troca, sem reescrever o record original para obter PASS.

Fontes anteriores confrontadas: BPR-BINDING-MATRIX.csv e BPR-CONSOLIDATED-CLOSEOUT.md
já registravam a política de budgets/window incompleta e o erro1060 observado
naquela ocasião. Isso foi reutilizado como histórico, não como nova consulta SCM
ou prova de ausência atual. Não existe valor SCQ aprovado nesses registros que
possa preencher automaticamente os campos; não foi solicitada coleta redundante.

## Pedido operacional e primeira lacuna

OPERATIONAL_REQUEST=NOT_READY. NÃ£o hÃ¡ pedido executÃ¡vel ou comando com placeholders.
A primeira dependÃªncia Ã© o PLANO OPERACIONAL ORIGINAL, byte-exato e independente,
ainda nÃ£o fixado, e sua aquisiÃ§Ã£o/registro legÃ­timos no owner. Os trÃªs seletores
nativos continuam NULL. Este prompt Ã© preparaÃ§Ã£o, nÃ£o a decisÃ£o operacional que
OHI exige. Um arquivo escrito pelo agente, presenÃ§a de metadados, confirmaÃ§Ã£o
NEW_INSTALL ou aceitaÃ§Ã£o SSO-B nÃ£o podem preencher essa autoridade.

Campos prÃ©vios nÃ£o fundamentados no plano: seleÃ§Ã£o do artefato/perfil exato;
composiÃ§Ã£o de polÃ­tica e valores justificados de validade/headroom/duraÃ§Ã£o; e
seletor `key` exigido pelo schema/contrato atual. O cÃ³digo exige valid_from/until
e scqp_key; nÃ£o hÃ¡ default autorizado. NÃ£o foram copiados valores do LAB nem
nomes de fixtures. Um seletor Ã© intenÃ§Ã£o administrativa, nÃ£o prova de objeto KSP:
nÃ£o se exige criar CA ou chave MACHINE para provar o transporte, nem se alega
que a fixture CurrentUser demonstra MACHINE. Sem um seletor legÃ­timo designado,
este campo permanece UNBOUND; nÃ£o se altera silenciosamente o schema para omiti-lo.

A aquisiÃ§Ã£o real do pedido/decisÃ£o e a continuaÃ§Ã£o nativa do runner ainda nÃ£o
foram ativadas/provadas. O runner fecha fonte vazia e retÃ©m provas vivas sem
continuaÃ§Ã£o governada; aprovaÃ§Ã£o isolada de endpoint nÃ£o o torna executor.
Isso Ã© parte explÃ­cita da fronteira, nÃ£o um problema de descobrir PID antecipado.

NÃ£o encontrado conflito entre accepted aplicÃ¡veis. NÃ£o se declarou impossibilidade
arquitetural nem foi proposta outra famÃ­lia de ADRs/autoridades para preencher
os campos. A menor decisÃ£o real Ã© fixar aquele Ãºnico plano com valores e origens
legÃ­timas e aprovÃ¡-lo operacionalmente sÃ³ quando estiver completo. A aprovaÃ§Ã£o
normativa SSO-B nÃ£o precisa ser repetida. Originais nativos continuam nÃ£o demonstrados.

## Efeitos e caminho atÃ© NEW_INSTALL

Separar no pedido futuro: endpoint privado; lifecycle SCM/Start/Stop; SCM/CNG e
parent. A primeira prova pode excluir probe KSP/CA/CURRENT, mas nÃ£o ignora campos
obrigatÃ³rios do plano. CriaÃ§Ã£o/instalaÃ§Ã£o/cÃ³pia/proteÃ§Ã£o do alvo nÃ£o sÃ£o efeitos
implÃ­citos. ExistÃªncia de diretÃ³rio nÃ£o prova autoridade; N0 nÃ£o Ã© C8 e nenhuma
root de teste Ã© promovida. A necessidade factual de parent/serviÃ§o/imagem deve
ser observada pelos gates, sem inferir ausÃªncia atual de registros histÃ³ricos.

ApÃ³s plano completo e autorizaÃ§Ã£o especÃ­fica: adquirir originais protegidos,
qualificar par/entrega/close nativos e lifecycle; qualificar CNG MACHINE em escopo
prÃ³prio; entÃ£o parent/NEW_INSTALL/CA_INITIALIZE e publicaÃ§Ã£o sob seus gates.
Sem TPM/Q4 por padrÃ£o, rede operacional, novas roots ou grant persistido.
E17/E18 recebem apenas nova evidÃªncia offline; demais gates nÃ£o sÃ£o promovidos.

BINDING_RULES=PRESESSION_AND_FRESH_SEPARATED;
ORIGINAL_CLOSE_BINDING=INTEGRATED_OFFLINE; ORIGINAL_NATIVE_PAIR=NOT_DEMONSTRATED;
OPERATIONAL_REQUEST=NOT_READY; AUTHORITY=DENY; E21=PENDING; ED11_PHASE6=PARTIAL.
HEAD/WIP/histÃ³ricos/pins/chave fixture/UNKNOWN/reservas/PST0.6.4 preservados.
PrÃ³xima continuaÃ§Ã£o: NEXT-ONB-EXACT-SESSION-PLAN.md.
