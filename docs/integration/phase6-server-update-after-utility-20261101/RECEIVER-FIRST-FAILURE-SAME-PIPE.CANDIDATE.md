# Proposta focal — retorno negativo da primeira falha receptora

CANDIDATE / NÃO ACCEPTED / SEM IMPLEMENTAÇÃO DEPENDENTE / SEM AUTORIDADE OPERACIONAL.
Data2026-11-02. Exclusivamente P1 PERSONAL_LOCAL TRANSPORT_ONLY SQB3, diagnóstico fechado.

## Necessidade factual e fronteira de prova
A sessão db62ad040b42e49a568fcdd91a816abc2c29a97ed324a93e0935ab75d2587a2f,
tentativa39b3840843f4fcec9aee3935d6a2bd533f4056bbe8c2c1abf4e1a75107bfe85f,
foi consumida: documentos criados/close conhecido, três confirmações e Start submetido.
Controlador7400/criação134381413716812022 aguardou receipt1052/operation2/pending1/
offset0, timeout1460/212/retained1. Não capturou fase/erro original do receptor.
PID runtime não impresso; estado SCM atual não consultado. Não presumir término.
A correção P1 instalada174db00a… não resolveu o timeout; não prova causa anterior.

No código, ingress_continue faz goto failed preservando primeiro n->error,
o->unresolved e transporte falho. O ator termina em service_runtime_retained e
ps_park_retained/Sleep(INFINITE), sem reportar STOPPED. Console do controlador
imprime apenas seu pacote, não esse estado do outro processo. Modelos substituem
captura nativa por capture_input_model, incluindo digests copiados do plano.
O PASS dessa fronteira não demonstra sucesso de SD/hash/cadeia/runtime nativos.
Nenhuma hipótese de chamada específica foi provada por esse timeout.

## Alternativas e decisão indispensável
A manter DENY/UNKNOWN sem novo ensaio. B recomendada: retorno NEGATIVO mínimo pelo
mesmo pipe original, sem outro serviço/canal/owner/protocolo administrativo.
Não repetir Start às cegas, aumentar quotas ou alterar ACL/conta do serviço.
Não recomendar dump amplo, VM_READ/debugger/impersonação, arquivo de log protegido,
novo evento/registry/event source ou memória compartilhada para extrair um DWORD.
Essas alternativas adicionariam acesso/efeitos não cobertos; não foram executadas.
Inventário offline do Release não encontrou papacc_server.pdb/map que permita
assumir localização simbólica segura de root. Não afirma inexistência global.

A implementação B conflitaria, sem decisão focal, com Q2 de SQB3 (SQR1 preservado
byte-exato), e com a errata Q6: "nenhum novo frame ou canal será criado".
Não se altera SQR1 ou SQC3; pede-se somente EXCEÇÃO delimitada à proibição de novo
frame, para resposta negativa privada SQF1. Q3 (fase QUERY não emite receipt),
OHI/SSO-B/G0–G9/G3/P0–P7 continuam vigentes: SQF1 NÃO é receipt ou close.
Aprovar B deve registrar norma designada antes de qualquer implementação dependente.
Nenhum accepted anterior é editado silenciosamente. Sem aprovação, A/DENY.

## Regras indivisíveis propostas F0–F7
F0. Mesmo owner, ServiceMain, pipe/local namespace, tentativa e três atos E/Q/S
originais; apenas P1 TRANSPORT_ONLY explicitamente selecionado em ambas pontas.
Sem efeito produtivo, novo endpoint, reconexão/retry, owner/serviço/root ou fonte
de autoridade. READ/NEW_INSTALL/SQB1/SQB2 e demais perfis inalterados.

F1. Congelar a PRIMEIRA falha receptora antes de retenção alterar fases. Catálogo
finito de etapas do ingress real (captura runtime/policy/resources/open/read/
candidates/paths/input-capture/content/runtime-observation/image/bind/deadline/
first-prepare/promote/continuation/receipt), cada mapeamento documentado e testado.
Código Win32 do retorno original; native_kind0UNKNOWN/1NTSTATUS/2WIN32 e raw somente
quando capturados da MESMA chamada original. Não consultar GetLastError depois de
outras chamadas para inventar causa. Chamadas de validação podem ter erro composto;
nesse caso native_kind0/raw0 e código original, não diagnóstico nativo fictício.
Stage positivo não concede identidade/bilateral ou comprova causa histórica.

F2. SQF1 privado56bytes, canônico little-endian sem padding:
0..3 magic ASCII SQF1;4..7 tamanho56;8..11 stage;12..15 win32;
16..19 native_kind;20..23 raw;24..55 attempt32bytes da associação original.
Stage/erro positivos e kind válido obrigatórios; UNKNOWN exige raw0. Não transportar
path/SID/token/HANDLE/ponteiro/nonce novo/payload/descritor/conteúdo de arquivo.
Attempt já pertence à associação SCQ privada, não é grant ou segredo novo.
SQF1 não imita SQR1/SQC3, não inclui plano/refs e não altera nenhum byte de formato
legado/público/persistido. O receptor do negativo não pode chamar ack/close/admit/ready.

F3. Emitir no máximo UM SQF1, apenas no pipe original já adquirido e identificado
pelo kernel, com runtime local original e tentativa/carrier/versão escopados.
O controlador aceita negativo somente no mesmo pipe/tentativa, com identidade de
runtime e associação previamente validadas/retidas pelo seu fluxo original.
Uma indicação do peer não é verdade nativa atestada pelo controlador: rotular
RECEIVER_REPORTED_FIRST_FAILURE, não ORIGINAL_CONTROLLER_NATIVE_ERROR.
Negativo serve somente para DENY e retenção; nunca autoriza novo efeito/grant.
Não exigir concluir a validação que justamente falhou, mas não dispensar as
provas mínimas independentes de posse/identidade do pipe/runtime e scope da tentativa.
Sem essas provas, suprimir envio e preservar UNKNOWN. Falha antes do pipe pode
continuar produzindo somente timeout; essa limitação é deliberada.

F4. Antes do envio, Stop/deadline expirado/recursos desconhecidos/insuficientes,
I/O pendente ou completion incerta IMPEDEM nova escrita. Não cancelar e presumir
completion, fechar referências para obter margem ou renovar prazos. Não sobrepor
buffer/OVERLAPPED ainda em uso. Reservar buffer56 e bookkeeping limitado no mesmo
pacote existente, contabilizar tamanho/alinhamento/censo antes dos builds. Reusar
pipe/evento somente com ownership/lifetime comprovados; sem handles novos.
Se não for possível provar reutilização segura, relatório antes de implementar
outro mecanismo. Nenhuma quota aumenta. Preservar entrada128/guard188/cap192,
privado1994944/working3145728/físico7239172096/commit3221225472/freshness100ms/
janela240000/global60000/menor deadline/política vigentes e contagem completa.

F5. Consumo do envio negativo antes da única submissão; preservar TODOS os objetos,
originais/borrows/IDs/bytes de bootstrap/backing retidos. O buffer de diagnóstico
não sobrescreve frame de entrega ou evidência parcial. Erro durante envio é
secundário: jamais substitui a primeira falha congelada nem rearma a tentativa.
Uma única WriteFile para56bytes; polls observam a MESMA submissão. Short write,
completion incerta ou erro suprimem continuação/reenvio e conservam UNKNOWN;
não enviar restante, reiniciar frame ou produzir segundo SQF1.
Nenhum novo recurso pode ser adquirido depois da falha para fazer diagnóstico passar.

F6. Controlador distingue magic limitado antes de exigir completar1052bytes de
SQR1, inclusive leitura fragmentada. SQF1 exige exatamente56bytes para validação;
limitar leitura, rejeitar extra/truncado/versão/attempt divergente/replay/tipo
inválido. Não interpretar primeira leitura parcial como receipt/close ou sucesso.
Reusar o mesmo estado I/O com limites explícitos, preservando buffer até completion
conhecida. Ao receber negativo válido, imprimir uma vez stage/code/kind/raw e
encerrar progressão com DENY/retention original. Nenhuma reconexão, retorno/ack,
readiness, liberação de empréstimo, close ou cleanup é inferida pelo frame.
Com EOF/timeout/Stop/UNKNOWN manter a primeira falha local e registrar peer negativo
somente se completo/correlacionado; não substituir prioridade de falhas por conveniência.

F7. Disposição/close permanecem independentes. Enviar/receber SQF1 ou observar
saída não comprova SCQ close/Q6/release/refund/quiescência OS. SQC3 mantém cinco IDs,
originais independentes de custódia permanecem até resultado terminal/liberação
legítima, sem fechar antes da última validação. Se falha impede close, reter UNKNOWN.
Históricos/pins/EXEs/previous/originais/reservas/PST0.6.4/DENY produtivo preservados.

## Qualificação exigida depois de aprovação
Testar composição REAL ingress/continuation até receipt com fronteiras nativas
mockadas explicitamente, sem substituir pipeline inteiro por digests aprovados.
Injetar falha em cada etapa, preservar a primeira, native-known/UNKNOWN e campos
zero legítimos; erro secundário de envio não a substitui. Testar frame56/truncado/
extra/fragmentos/attempt errado/replay/versão/kind, confusão com SQR1/SQC3, pending
antes de envio, completion UNKNOWN/short-write/Stop/deadline/resourceDENY; ausência
de pipe/prova local não envia. Confirmar ausência de handle novo e footprint real.
Receber negativo nunca passa ack/admit/bilateral/close/RUNNING. Caminho positivo,
formatos e perfis legados preservados. Quatro builds e regressões afetadas; não
repetir campanhas inalteradas. Modelos não são prova nativa do ensaio histórico.

## Riscos e limites da decisão
Metadado remoto pode ser incorreto; só conduz DENY. Envio pode ser suprimido por
Stop/recursos/deadline/pipe/completion e timeout continuar sem causa remota.
AsyncIO/latência/picos internos não demonstrados. Não garantir que a próxima sessão
completará ou que captura stage elucidará toda causa nativa. Não converter UNKNOWN
em zero/PASS. Risco adicional limitado:56bytes e bookkeeping/ramo negativo no pipe
já existente; censo exato deve preceder qualificação e congelamento.

## Decisão solicitada e operações separadas
Aprovar B e F0–F7 SOMENTE para registro normativo e implementação/qualificação
offline. Não aprova imagem, UPDATE, coleta, disposição, E/Q/S, Start/Stop/kill,
ACL/SCM/CA/CURRENT/rede/TPM/NV/Q4/limpeza/refund/produtivo.
Processos atuais podem continuar retidos; nenhuma consulta foi executada para
identificá-los nesta tarefa. Autorizações anteriores consumidas não se reutilizam.
Depois de código e testes completos, preparar disposição atual e eventual UPDATE
U1–U7/U2 e nova sessão com pedidos próprios. Não criar infraestrutura paralela.
