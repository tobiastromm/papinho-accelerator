# Reconciliação da sessão G3-P1 runtime SID

Autorização manual UTILIZADA, SEM RETRY. Evidência preservada em OWNER-G3-P1-RUNTIME-SID-SESSION-OUTPUT.txt; SHA256: `b4b6ea4b8bba1a0ed87bf2706fb4064c506cfa8a4a7c636c02e47a68ccbc46df`.

As duas cópias documentais foram criadas e verificadas, com fechamento conhecido: pedido `417614100:11599872:11740`, decisão `417614100:1048576:290736`. Não constituem confirmações E/Q/S.

A validação final passou static, runtime unbound e janela: passed_mask=7, rejected=0. Janela original 12426578–12666578 ms; consulta em 12426593 ms (15 ms decorridos). A correção SID passou nessa execução; isso não prova causas históricas. Contagens observadas: inicial 113, última pré-abertura 118, guard188/cap192; não demonstram pico nem capacidade de toda a sessão.

A falha WIN32=5 ocorreu na projeção do plano SCQ, depois de BINARIES e antes de OWNER_SCQ. Em `server_configuration_personal_native_win32.inc`, scqa_console_projection percorre binary/parent/data e retorna zero para qualquer comprimento zero. Em `personal_install_paths_win32.inc`, papacc_personal_paths_selected zera a estrutura e preenche apenas binary; parent/data permanecem vazios por desenho. Logo o presenter exige dados não aplicáveis ao percurso P1. O retorno é mapeado para ERROR_ACCESS_DENIED pelo confirmador/controller; não é evidência de ACL inadequada. Não há diagnóstico separado de cada escrita do console, portanto uma falha concorrente de output não pode ser excluída apenas pelo log.

Nenhuma confirmação SCQ, endpoint ou Start foi alcançada no fluxo; nenhum Start foi chamado. Estado SCM atual não foi consultado. Fechamento das cópias conhecido não prova fechamento nativo integral. Fechamento nativo, retenção histórica e quiescência OS permanecem UNKNOWN; readiness produtiva DENY.

Primeiro gate: corrigir e qualificar offline somente a projeção TRANSPORT_ONLY para campos DATA não aplicáveis, preservando validação estrita dos demais perfis, escopo original e três confirmações. Não preencher caminhos fictícios nem descobrir ProgramData para satisfazer o presenter. Não preparar nova sessão/UPDATE antes da qualificação e dos bindings completos. Nenhum código operacional alterado nesta reconciliação; campanhas anteriores não repetidas.

Próximo prompt: NEXT-G3-P1-TRANSPORT-PRESENTER-CLOSURE-OFFLINE.md. Documentos anteriores EXECUTION_PENDING permanecem históricos; esta reconciliação registra o resultado efetivo.
