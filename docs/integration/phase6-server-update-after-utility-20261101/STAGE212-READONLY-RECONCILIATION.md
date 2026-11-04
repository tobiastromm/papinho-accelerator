# Consulta etapa 212 reconciliada

Fonte: saída manual integral enviada pelo proprietário nesta retomada; STAGE212-READONLY-RESULT.json é transcrição estruturada, não cópia byte-exata. Pedido 4c05b5043668d01976853e74cf8ca5af714ae1c078614a54c7c781227bff1edd utilizado, sem retry. Nenhuma consulta executada pelo agente.

queryex/qc/qsidtype retornaram zero. Na consulta: START_PENDING, PID SCM 18000, códigos de saída zero, checkpoint e wait hint zero. Configuração: OWN_PROCESS, DEMAND_START, conta virtual NT SERVICE\PapinhoAccelerator, SID UNRESTRICTED e comando diagnóstico exato esperado. PID SCM não é prova original do token/identidade do runtime.

Controlador 7580 corresponde ao FILETIME original 134381210166495538 e HasExited=False. Não atesta token, referências internas ou close. Estado é observação daquele momento, não lease fresco permanente.

Código offline: ps_main publica START_PENDING antes do callback e só publica STOPPED mediante terminal_receipt=1/PS_STOPPED. ps_park_retained fixa PS_RETAINED e Sleep(INFINITE), deliberadamente sem reportar STOPPED. O controlador também retém quando start_effect=1. Isso explica por que esperar não garante conclusão; a consulta não distingue runtime executando ingresso de runtime já estacionado em retenção. Não atribuir um desses ramos como causa factual.

A origem exata do erro 5 de progress/wait continua UNKNOWN. Não há prova suficiente para corrigir token, ACL, transporte ou deadlines. A configuração SCM observada não identifica esse subpredicado. Novo lançamento CLI não reproduz esse processo e agravaria a ambiguidade.

Nenhum Stop/kill/UPDATE/retry autorizado por esta consulta. As disposições anteriores de outros PIDs não se aplicam. Não limpar originais ou retidos. Não transformar prazo expirado em autorização de liberação.

Próximo trabalho técnico: fechar a distinção offline entre retorno de start_progress e start_wait e os predicados de runtime_bind/offer/transport, projetando somente evidência já capturada no owner. Não reamostrar nem modificar gates para diagnosticar. Se necessário alterar a imagem, qualificar primeiro e separar UPDATE de nova sessão. A sessão atual não pode ser reutilizada. Qualquer disposição do controlador E do possível runtime exige escopo próprio com identidade suficiente, sem inferir quiescência/close.

Código operacional inalterado; builds e campanhas anteriores não repetidos. Preservados G3-P1, identidade, autoridade, bilateral, receipt/close, UNKNOWN e DENY produtivo.
