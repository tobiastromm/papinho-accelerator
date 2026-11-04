# Próximo trabalho — contrato de elevação do runtime virtual

Ler RUNTIME-ELEVATION-SHAPE-V3-RECONCILIATION.md e RESULT.json e os accepted aplicáveis. Localizar exigências exatas de elevação, grupos, integridade e privilégios para o runtime virtual PERSONAL_LOCAL TRANSPORT_ONLY. Confrontar os dois callers: personal_runtime_identity_win32.inc e scqp_peer_state em personal_scq_profile_win32.inc.

A observação completa atual mostrou elevation1/typeDefault, Administrators ausente, High integrity e SeImpersonatePrivilege habilitado. Não normalizar ou ocultar esses campos. Distinguir identidade de serviço, elevação UAC e autoridade efetiva. Se houver correção já coberta, qualificá-la offline após cumprir a leitura de governança; se houver incompatibilidade normativa, apresentar uma única decisão focal, com cláusulas exatas e riscos. Não implementar relaxamento dependente de aprovação ainda ausente.

Nenhuma nova consulta, UPDATE, Start/Stop, endpoint, disposição ou limpeza autorizada. Processos6120/10080 não devem ser encerrados. Autorizações consumidas, UNKNOWN, retenção, G3 e DENY produtivo preservados.
