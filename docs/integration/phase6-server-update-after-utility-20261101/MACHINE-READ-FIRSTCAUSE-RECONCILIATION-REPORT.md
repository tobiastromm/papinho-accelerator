# READ FIRSTCAUSE: rejeição no ramo READ após resultado correlacionado

Saída integral preservada, duas cópias verificadas/fechadas, confirmação E/Q/S/P e Start submetido. Sessão usada, sem retry. Resultado factual em MACHINE-READ-FIRSTCAUSE-SESSION-RESULT.json. Não executar outra sessão/UPDATE/disposição automaticamente.

Primeiro retorno exportado: branch9/error5, role1/sessionphase4/transportphase1/legs5275. Bits1,2,8,16,128,1024,4096. No código atual SCQK_P_SEND128 e SCQK_RESULT_READ1024 indicam passagem pelo envio P e aceitação do resultado correlacionado; não revelam classificação/status do probe. Fase4 e bit16 representam avanço no caminho DATA/close do controlador, não application-close ou quiescência global.

Análise focal: scqkm_continue_session salta as etapas READY/P/RESULT já marcadas e chama scqr_continue_transport. Este pode devolver erro original de validação nativa ou zero após close. Quando devolve zero, scqkm_continue_session exige scqk_result(probe,fixture->key.diagnostic_control_confirmed)==1; caso contrário retorna ERROR_ACCESS_DENIED. A saída atual não distingue esses dois retornos. Nenhuma prova de ACL incorreta, vazamento, quota baixa ou abertura CNG bem-sucedida pode ser inferida.

Primeira lacuna concreta: retorno interno da continuação de close versus rejeição da classificação final, e snapshots já existentes do probe/controle positivo. Investigar atribuição/preservação de runtime_cng_probe e diagnostic_control_confirmed, critérios scqk_result e decoder, sem consulta ou nova operação. Guard phase3 em scqkr_current não pode ser acusado apenas porque a fase observada final é4: com os bits atuais a chamada não está nesse trecho.

Código não modificado; builds/testes inalterados não repetidos. Retained1/CLOSE5023: aplicação e runtime permanecem inconclusivos; controller9004 é apenas locator com criação original registrada, runtimePID não exportado. Nenhum G5 antigo autoriza encerramento. Não repetir comando ou matar processos. Retenção histórica/quiescência UNKNOWN, produção DENY/PST0.6.4 preservados.
