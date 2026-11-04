# Continuação técnica — primeira entrega original no packet fechado

Projeto/branch/owner atuais. Ler CLOSEOUT.md desta pasta e o prompt original NEXT-FIRST-DIAGNOSTIC-START-READINESS.md. Esta continuação não substitui nem conclui o objetivo técnico anterior.

Implementar offline a ligação ainda ausente entre scqa_startup_original controller-local e a recepção inicial no packet existente de papacc_server_launcher_scq_closed_prepare. Usar o adapter SCQ, autenticação bilateral e referências tipadas accepted; não serializar ponteiros ou criar grants/owner/IPC paralelo. Compor ato lifecycle atribuído independente da confirmação SCQ, consumo antes do efeito modelado e retenção até fechamento original. Manter selection operacional ausente sem ato original válido.

Qualificar a cadeia completa com APIs SCM/RNG/endpoint simuladas: original válido, ausente/substituído, ato lifecycle ausente, destinatário divergente, restart/replay, Stop, peer perdido, fechamento UNKNOWN, nenhum RUNNING antes do bilateral, nenhuma chamada produtiva. Não confundir modelo com autenticação Windows real. Testar runtime inicialmente vazio, não apenas packet previamente populado. Reutilizar fontes/tests existentes e não refazer registro/staging.

Confrontar o runtime staged c0f27f94… com os novos callers. Se troca for necessária, demonstrar necessidade e preparar efeito separado; não executar. Fechar plano próprio de recursos startup/SCQ sem importar Q/R0–R9, B3 ou custos opacos aceitos de outros efeitos. Não exigir PID/token futuro antes do Start.

Somente após conclusão técnica, preparar pedido operacional byte-exato se todos os bindings estiverem completos. Não executar Start/Stop, endpoint, SCM/ACL, staging, CA/CURRENT, rede, TPM/NV/Q4, limpeza/refund. Preservar UNKNOWN, históricos, autorizações utilizadas, HEAD/WIP e PST0.6.4. Não criar ADR por conveniência nem solicitar reaprovação SSO-B.
