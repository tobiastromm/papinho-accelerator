# Próxima execução — ingresso nativo original no packet SCM vazio

Continuar main/HEAD/WIP existentes. Ler CLOSEOUT.md e EVIDENCE-MATRIX.csv desta pasta. Não refazer a composição scqa_start_* ou scqr_first_prepare já qualificada, nem registro/staging.

Primeiro delta concreto: ligar a aquisição original inicial do ServiceMain ao caller papacc_configuration_personal_scq_recipient_prepare, que agora cria receptor/profile/session a partir de originais verificados. Inspecionar a origem dos argumentos plan/originals/controller e a ramificação captured_empty em papacc_server_launcher_scq_closed_prepare. Não chamar um packet previamente populado de ingresso original.

Reutilizar exclusivamente o adapter Win32 SCQ e a fonte administrativa existentes. Separar seletores de operação de observações frescas. Não preencher personal_scq_registered_owner_decision/endpoint_decision com constantes, hashes, flags, callback permissivo ou arquivo de grants; não transformar o novo evento local OWNER_SCQ_DIAGNOSTIC_START em aprovação operacional implícita. Ato Start e confirmação SCQ são independentes; manter tomada única antes do efeito. Não serializar ponteiros nem usar numeric HANDLE recebido como autoridade; refs entregues exigem aquisição original e validação independente.

Implementar e testar offline o caller real de ingresso, se possível nos contratos SSO-B/OHI/B/TO0–TO9, usando packet inicialmente vazio e APIs SCM/endpoint/RNG simuladas. Testar falta de fonte original/escopo, par trocado, imagem/token incompletos, replay, Stop, fechamento e retenção. Não alterar readback/registro ou remover guards. Se a independência das fontes exigida pelo contrato impedir o primeiro ingresso, demonstrar precisamente a condição circular, o produtor ausente e a cláusula relevante; não criar outro mecanismo por inferência.

Integrar ao SCM owner o uso do ato original de Start sem estender sua autoridade a outras mutações. Antes de pedido real, fechar o plano de recursos específico e revalidar configuração/imagem/parado por originais sob gates frescos; não herdar R0–R9 ou B3. Não exigir PID/token futuro antes da operação. Manter bloqueado o Start que não tenha todas as provas.

Confrontar o delta com o runtime registrado c0f27f94…; nenhum rebuild do workspace atualiza essa imagem. Revisar STAGED-IMAGE-UPDATE-PROCEDURE.CANDIDATE.md e localizar o owner UPDATE aplicável antes de preparar qualquer atualização; stager exclusivo não faz overwrite. Não pedir seleção de artefato ainda mutável.

Executar apenas builds/regressões afetados. Atualizar hashes, matriz e capability. Não executar Start/Stop, endpoint, staging, SCM/ACL, CA/CURRENT, rede, TPM/NV/Q4, cleanup/refund; não criar ADR/owner/ledger/root/serviço paralelo. Não emitir comando que só encontre DENY ou com placeholders. Preservar UNKNOWN/quiescência OS, autorizações utilizadas, históricos, pins, reservas, chave fixture e PST0.6.4.
