# Integração SQB3/SQC3 e custódia Q6 — conclusão offline

Estado: OFFLINE_QUALIFIED; operação real DENY. Data2026-11-02.
Branch main/HEAD f341985924d29870d8ff627ed37c8a0874ad6469, WIP preservado.

## Autoridade normativa

Base726e3f786b6ca187a0058734b6c9fd9e67d2d2b9d465f59bc9bd5313cbbe2c4c e
errata b8d73c7541df2363e6f02489d5143a2becec78fdd315003806010a0303bbe810 foram
aprovadas separadamente pelo proprietário, conferidas e incorporadas em
[decisão accepted](../../adr/DECISION-personal-scq-p1-query-references-sqb3-20261102.md)
ANTES do código. Originais e decisões anteriores byte-exatos, sem reescrita.
Conflito Q6 resolvido: close da entrega não fecha nem substitui a custódia original.
OHI/SSO-B/QUERY/G3/entrega cross-principal continuam aplicáveis.

## Código integrado no mesmo owner

personal_scq_transport_win32.inc: o controlador já autenticado duplica seus
originais administrativos com processo0x00101000/tokenQUERY8 para o runtime
original validado. Cinco efeitos de entrega, cada um com guards frescos de origem
e destino; consumo anterior obrigatório. PROCESS_DUP_HANDLE permanece no remetente.

personal_scq_bootstrap_win32.inc: o receptor P1 abre somente o endpoint previsto e
recebe SQB3 como QUERY candidata. Não reabre o administrador com OpenProcess ou
OpenProcessToken. Valida direitos exatos, tipo kernel, não herança, aliases, PID/
criação/liveness contra peer kernel e plano, token completo e originais protegidos.
Nenhum token/PID/frame/hash confere autoridade. Binding/admissão bilateral só após
conteúdo/SD/proveniência/imagem/seleção e confirmações originais.

personal_scq_recipient_win32.inc: captura as cópias de custódia local de processo
e token, com guarda fresca antes de CADA duplicação. A segunda reabertura de token
foi substituída somente no percurso SQB3. Referências de custódia permanecem até
resultado terminal/liberação do empréstimo; o caminho estrito anterior permanece.

server_launcher_owner_win32.c e predicado interno do configuration owner: somente
a PRIMEIRA leitura QUERY pendente pode esperar sem referência original do peer.
Evento/pipe originais, Stop, runtime local e menor deadline permanecem obrigatórios.
Não inventa liveness; após binding, volta à espera com peer original retido.

Após a última validação com as referências recebidas, fecha-as uma vez e guarda
somente seus IDs em closed_query. As cópias do recipient continuam vivas. SQC3
compara os cinco IDs da entrega e o plano original; fechamento, retorno atribuído
e readiness continuam estágios distintos. Falhas parciais não rearmam tentativa.

SCQ1/SQB1/SQB2/SQR1/SQC1 mantêm bytes e tamanhos. SQB3 selecionado somente pela
política local P1, não pelo magic remoto; sem downgrade/fallback. READ, NEW_INSTALL
e demais perfis sem ampliação. Não houve alteração de CLI, serviço ou autoridade.

## Gramática privada exata

| Frame | Segmentos | Total |
|---|---|---:|
| SQB3 | magic[0..3], record[4..1051], arquivos3xu64LE[1052..1075], deadlineu64LE[1076..1083], processo/token2xu64LE[1084..1099] |1100|
| SQC3 | magic[0..3], record[4..1051], arquivos fechados3xu64LE[1052..1075], consultas entregues fechadas2xu64LE[1076..1091] |1092|

IDs são valores na tabela do destinatário, nunca handles locais do remetente.
Não efetuar DUPLICATE_CLOSE_SOURCE nem operação contra IDs remotos fechados.
SQC3 não afirma close dos readers de conteúdo/SD nem das duas cópias originais de
custódia; estes seguem o owner até fechamento terminal comprovado.

## Qualificação final

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings. Dezenove regressões
afetadas em cada configuração:76pares PASS, zero falhas. Logs sqb3-*-build.log e
sqb3-*-tests.log, fontes/pins/contagens em SQB3-OFFLINE-QUALIFICATION.json.
A suíte QUERY agora tem71cenários/4913asserções por configuração, com APIs protegidas
simuladas. O modelo usa IDs distintos para entrega e custódia e torna os handles
fechados indisponíveis: comprova custódia viva após SQC3 e close terminal separado.

Inclui reaberturas administrativas negadas, cinco pontos de entrega parcial,
direitos excedentes/tipos/aliases errados, token/identidade divergentes, replay,
segunda entrega, Stop, deadline/freshness/capacidade que vencem entre as duplicações
locais, seleção incompleta, SQC1 insuficiente, SQC3/retorno divergentes, close UNKNOWN
e readiness prematura. Negativos antigos e perfis privados anteriores preservados.
Não executar coleta nativa como teste: endpoints/SCM/processos/token/I/O mutantes
são modelos. O censo executado sem argumentos calcula somente sizeof, não coleta.

## Recursos preservados

Transport1112->1152bytes (+40 de bookkeeping); driver2280->2336 (+56), incluindo
buffer1084->1100 (+16). Root/managed +56bytes. Slots controlados continuam44;
referências recebidas substituem as duas aberturas e a custódia usa seus dois slots
já contabilizados. Não converter isso em máximo nativo de handles.

Selected: ROOT375168, KNOWN2002216, limite3145728bytes.
Legacy: ROOT206600, KNOWN1833648, limite2097152bytes.
Novo buffer temporário NtQueryObject512bytes na stack, com comprimentos/ponteiros
limitados e direitos/tipo exatos. Não é alocação de subsistema nem teto de stack
nativa; custos kernel/provider/CRT/IO e latências permanecem UNKNOWN.

Entrada128/guard188/cap192, parcelas44/16/4; commit3221225472; físico7239172096;
privado1994944; working3145728; freshness100ms, janela240000ms e global60000ms
inalterados. Policy0b260c85dfceec5e6e861bb998e4504723785b927d875d113d41a405e1ce1b60.
Prazo mínimo sem renovação, consumo único, Stop, UNKNOWN e DENY produtivo mantidos.

## Imagem e próximo gate

Novo Selected/Release804352bytes:
SHA25611cbef6508b1b06fb56031b22b93dccc6ae3e05df22528190f3cf7215636d815.
Congelado em papacc_server.sqb3-query-804352.exe, idêntico ao build; delta+5632bytes.
DLLs de build idênticas aos pins anteriores; updater não alterado. O último canônico
reconciliado era798720bytes/ID417614100:4128768:327703/pin6ad6e378…; não foi atualizado
nem consultado novamente nesta tarefa. Não presumir readback fresco do instalado.

Preparado ONE-SQB3-IMAGE-UPDATE-REQUEST.CANDIDATE.md e wrapper reutilizado, AST22bindings/
16argv qualificados offline, sem execução. O pedido é CONDICIONAL, não executável
agora: os participantes WS3M estavam retidos no último fato observado. Não existe
OWNER-SQB3-UPDATE-APPROVAL.txt emitida nem aprovação operacional implícita.

Primeiro gate concreto: disposição legitimamente autorizada dos participantes
WS3M19860/criação134381311017119087 e19836/criação134381311235391902, com identidade,
criação/token/imagem frescos e autoridade G5 específica se término excepcional for
necessário. Nenhuma disposição anterior serve. Último SCM START_PENDING, não STOPPED.
Esta integração NÃO permite TerminateProcess ou Stop. Não repetir a sessão consumida.

Após disposição reconciliada e readback fresco STOPPED: autorização específica de
UPDATE U1–U7/U2, execução manual única/reconciliação, depois novos originais e nova
sessão E/Q/S independente. PID/token futuros não são requisitos anteriores ao
processo, mas bilateral/receipt/close/RUNNING diagnóstico exigem provas reais.

A barreira DACL observada permanece fato atual da consulta, não erro histórico
retroativamente provado. Modelo não prova native bilateral, capacidade ou sucesso
da próxima sessão. Application close, retenção histórica e quiescência OS UNKNOWN.
Nenhuma coleta/UPDATE/disposição/endpoint/SCQ/Start/Stop/SCM-ACL/CA-CURRENT/rede/
TPM-NV-Q4/limpeza/refund executada. HEAD/WIP/históricos/pins/EXEs/PST0.6.4 preservados.
