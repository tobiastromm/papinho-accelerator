# Disposição dos participantes da etapa212 — pronta para decisão específica

## Evidência reconciliada

Consulta e21756e47387d4e1500d03f32d4142ad97ba4c76f1d97e844ba9bf9d7f748e65 utilizada sem retry. Saída first0/completeTrue/observer_close_knownTrue. Controlador7580/criação134381210166495538/token126164448:0/auth570063:0/modified126164115:0/groups17. RuntimeSCM18000/criação134381210451301160/token126200549:0/auth126195148:0/modified126195209:0/groups10. SCM antes/depois START_PENDING/PID18000 pelo caminho qualificado. Correlação atual; original histórico do runtime/bilateral/close/quiescência continuam UNKNOWN. Nenhuma nova consulta executada pelo agente.

## Menor disposição proposta

Mesmo adaptador transitório e primitivas da disposição anterior; dois objetos fixos revalidados por handles/criação/token, imagem original retida por identidade/hash e configuração SCM. Não basta PID, string ou hash. Runtime primeiro; somente saída observada/código excepcional exato e readback STOPPED/PID0 permitem encerrar o controlador. No máximo uma submissão por alvo, sem fallback/retry. Se o SCM ainda não refletir STOPPED, interromper parcial, sem polling para obter PASS. Nenhuma alteração do serviço, ACL ou arquivos.

Ausência de ações de recuperação SCM é gate adicional obrigatório, antes da primeira submissão e antes do segundo alvo. Terminar serviço sem reportar STOPPED pode disparar recuperação; não desabilitá-la para passar. Docs Microsoft: https://learn.microsoft.com/en-us/windows/win32/api/winsvc/ns-winsvc-service_failure_actionsw . Não há garantia contra outro agente alterando configuração/relançando processos.

TerminateProcess é abrupto e assíncrono; somente retorno não prova saída. Cada submissão recebe uma espera60000ms e GetExitCodeProcess do mesmo handle. Esse prazo é tolerância operacional solicitada, não máximo nativo/G3. I/O, DLLs, referências externas, reservas e quiescência podem permanecer desconhecidos. Docs: https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-terminateprocess . Não chamar encerramento de SCQ close, limpeza ou recuperação.

A legitimidade é a decisão excepcional posterior específica prevista em G0–G9 G5; não há autoridade implícita nas consultas/autorizações antigas. G7 preserva UNKNOWN/reservas; G9 exige decisões separadas. Não modifica ADR accepted nem cria novo mecanismo. A sessão falha fica abandonada, nunca PASS; UPDATE e nova sessão requerem escopos próprios e gates frescos depois da reconciliação.

## Qualificação

Candidato real compilado em PowerShell5.1, sem invocação. Modelo substitui17 APIs e a fronteira aquisição/hash da imagem;52 cenários PASS. Core de orquestração/guards/ordem/fechamento equivalente após remover somente essas fronteiras e controles de teste. Modelos incluem PID/criação/token/path/configuração divergentes, recuperação presente, cancelamento/replay, falhas das submissões, esperas/exitcodes, SCM ainda pendente, alteração após saída do runtime e fechamento UNKNOWN. No máximo uma chamada por alvo e nenhuma segunda submissão após falha intermediária.

Wrapper AST PASS, uma invocação futura fixa, sem respostas automáticas. Fechamento apenas das referências próprias do adaptador. Até7 referências explícitas e buffers8192/512bytes; custos internos opacos. Sem transportar G3/B/R/U. Não repetir builds/testes do produto: código do produto não mudou nesta preparação. Logs/hashes em STAGE212-DISPOSITION-OFFLINE-QUALIFICATION.json. Nenhuma chamada real de SCM/processo/token/terminate/UPDATE executada nesta qualificação.

## Primeiro gate

Pedido ONE-STAGE212-PARTICIPANTS-DISPOSITION.REQUEST.CANDIDATE.md
SHA25675ce698ed1846c25d40390f01fa89c93b3ace0e2e199b6ab9bd6b5807f728494.

Aguarda aprovação explícita dessa operação excepcional com seus riscos. Não há decisão de disposição emitida. Após aprovação, registrar os bytes efetivamente emitidos, seu hash real e preparar o comando manual; não executar automaticamente. Nenhum comando com placeholder foi emitido.

Depois da única execução autorizada: reconciliar cada submissão/saída/primeira falha, SCM e fechamento. Se houver base suficiente e serviço STOPPED, preparar UPDATE U1–U7/U2 do candidato diagnóstico212797696/bf7d0793... com utilitário existente e autorização própria; depois nova sessão E/Q/S com novos originais/atos. Preservar históricos, EXEs, DLLs, ProgramData, reservas, PST0.6.4 e todos os UNKNOWN residuais.
