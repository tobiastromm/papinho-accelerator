# Phase 6 — decisão focal e integração do ack/custódia existente

Ler o closeout/QUALIFICATION deste pacote. Não refazer CAS, A/P privado, consulta de status, cohort M7 ou handoff CA: já estão implementados e qualificados nos seus escopos. Não iniciar outra auditoria/TPM ou família de ADRs.

1. Conferir a proposta focal `PUBLISHED-ACK-FOCAL-DECISION.PROPOSED.md`, SHA256 `3fcce389e58299aaa161779b9c7976f00f8e79d76dc5622d40d60b091e34f684`. Registrar aceitação somente após decisão explícita. Sem aprovação, preservar formatos/PUBLISHED-DENY e continuar apenas trabalho técnico independente autorizado.
2. Com decisão válida, documentar e revisar o encoding versionado exato da intenção e do ack na mesma root/história/worker. Reutilizar campos existentes, distinguir A/P originais e atuais, manter compatibilidade explícita e bounds. Não reinterpretar v1, usar A futuro ou criar ack do ack infinito. Integrar/qualificar dentro das cláusulas aceitas, sem converter DATA em autoridade.
3. Conectar a aquisição original CA/key/cert/generation/custódia ao handoff tipado já existente. A borrow da cerimônia passa sem gap ao backend; leases de assinatura não devolvem a custódia. Não usar keyname/fixture/flag como prova NORMAL. Sem fonte original disponível, preparar apenas a autorização específica dos objetos protegidos que faltarem; não os materializar automaticamente e não reutilizar N0/RECOVERY.
4. Exercitar a composição positiva e o wrapper M7 somente com fonte e publicação/revogação canônicas legítimas. Reutilizar cohort e receipts existentes; verificar Stop/invalidation e A/R/P sem alegar quiescência/close. Nunca repetir assinatura/CAS após UNKNOWN.
5. Builds/regressões afetadas, hashes, capability/matriz e relatório único. Preservar HEAD/WIP, históricos, recursos/limites, Selected/Legacy, versões API/PST e produção 0.6.4. Sem fechamentos ED11/Phase 6 por contagem de testes.

Nenhuma autorização operacional é inferida da aceitação normativa. Não executar CURRENT/root reais, bootstrap, assinatura produtiva, Start/Stop real, SCM/ACL, rede operacional, TPM/NV/Q4/ownerAuth, cleanup/refund, publicação PST ou recuperação autoritativa. Não solicitar segredos.
