# Pedido — uma consulta manual read-only de estado após timeout WS3M

CANDIDATE / NOT_AUTHORIZED / NOT_EXECUTED. Uma execução manual, sem retry. Não executa nova E/Q/S nem reutiliza consultas/disposições consumidas. Finalidade exclusiva: localizar o estado SCM atual e correlacionar a vivacidade do controlador original da tentativae10c1dd5d934a23d5e17d7c5aa93aac75e2dddf99f3892e97bb725d65cdf37f7.

Evidência: OWNER-WS3M-SESSION-PARTIAL-OUTPUT.txt e continuação do proprietário: WAIT_TIMEOUT1460/step5/stage212/start_submission_phase1/retained1/service_close5023. Start foi submetido; estado atual do runtime, operação de transporte pendente e close são UNKNOWN. Não atribuir causa ao token/recursos nem aumentar deadline por esse timeout.

Procedimento observe-ws3m-status.MANUAL.CANDIDATE.ps1, SHA256 c4d2e98a3c5a58a1f97656d4f2bac89983b9fdeef62ea1706a3969bf00c2f9d2, pasta canônica C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101. Reutiliza observe-stage212-disposition.MANUAL.CANDIDATE.ps1 alterando SOMENTE nome do pedido/confirmação e PID/criação fixos. Nenhum novo owner, serviço, canal ou infraestrutura.

Contexto: host TCORP-TOBIAS, Windows PowerShell Desktop x64 interativo sem redirecionamento, operador SID S-1-5-21-785699550-1440458426-1682014700-1108. Conferir pins do procedimento/pedido; sem autoelevação, ACL override ou ajuste de privilégio. Após aprovação específica e confirmação manual OBSERVE_WS3M_STATUS_ONCE, uma chamada cada a C:\Windows\System32\sc.exe queryex PapinhoAccelerator, qc PapinhoAccelerator e qsidtype PapinhoAccelerator, nessa ordem. Somente consultas, sem ControlService/Start/Stop/reconfiguração. Falha interrompe sem retry.

Depois Get-Process -Id19860 somente como locator. Conferir StartTime convertido para FILETIME134381311017119087 (high31288087/low597716335) antes de atribuir ao controlador original; imprimir HasExited e liberar uma vez o objeto de consulta próprio. PID reutilizado/tempo divergente/consulta indisponível implica UNKNOWN/DENY, sem perseguir outro processo. Não aguardar/terminar/controlar o processo, não abrir runtime por PID. O PID do runtime mostrado por SCM é somente locator atual, não identidade original comprovada.

Não consulta tokens, memória, lista de handles ou arquivos protegidos. Essa correlação não atesta TokenId original, retenção interna, imagem mapeada, bilateral, receipt/close, negação KSP/CA ou prontidão. Estado observado é DATA, não autoridade para terminar, atualizar ou reiniciar.

Custos: três sc.exe sequenciais e uma consulta de processo, referências/transitórios/PowerShell/SCM-RPC opacos; não importar envelopes G3/U/B/R nem prometer teto nativo, duração máxima ou capacidade. Não escreve/cria/copia/renomeia/exclui arquivos de instalação; pins leem somente arquivos do procedimento/pedido. Fecha somente referências próprias do observador, não recursos da aplicação. Exit0 não prova close SCQ nem quiescência OS.

Proibidos: Start/Stop/kill, E/Q/S ou endpoint adicionais, UPDATE, SCM/ACL mutantes, enumeração genérica, memória/debug/impersonação, CA/CURRENT/KSP, rede operacional, TPM/NV/Q4, limpeza/refund/rollback/restauração/recuperação ou produção. Preservar originais/EXEs/históricos/pins/UNKNOWN/reservas/PST0.6.4. Disposições anteriores não autorizam novos alvos.

Qualificação offline: transformação exata do procedimento existente, AST/constantes/comandos sem executar wrapper ou consulta. Evidência WS3M-READONLY-STATUS-OFFLINE-QUALIFICATION.json. Após única execução manual autorizada, reconciliar saída integral/primeira falha/códigos/status e manter UNKNOWN onde não houver prova. Qualquer disposição exige decisão própria G5. O pedido atual não autoriza a si mesmo nem dispõe de aprovação emitida.
