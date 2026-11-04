# Sessão READ final-I/O — reconciliação

2026-11-03. SESSION_CLOSED_INCONCLUSIVE / ISOLATION_DENY / NO_PRODUCTIVE_PROMOTION.

A tentativa2a2db773415e5b78cf4b165e5bbf91ae78f918ee8bb64acf030d0692aab5095b concluiu e retornou ao prompt. Quatro confirmações E/Q/S/P atribuídas à mesma tentativa estão presentes. O nativo reportou CONSUMED=1 e CONTROLLED_CLOSE=KNOWN, terminal1168/CLI3/RUNNING=NOT_REPORTED. A autorização desta sessão está utilizada, sem retry; não é necessário preparar disposição excepcional a partir desta evidência.

Duas cópias exclusivas: request417614100:2555904:107563 e decisão417614100:3866624:108666, created_mask3/verified_mask3/close_knownTrue/firstNONE. Plano apresenta hashes exatos do pedido55ffe9a68db126733d5aa3b85e805c035e57f4a12505055b901838b265be8ce9 e decisão241f3179c9ffe5ae92d4916706536d80b81244b8e6f8bcb0c88fdd268fbf6354; imagem882688/ID417614100:1966080:412413/SHAe3a433c4a6edb565c783fee672bfc82a5794ba44988b123e7a38eb4ec5c9d6b7. Originais preservados, não reutilizar.

O branch11 retornou1168 com legs5275, em vez do branch6/error50/legs5259 da tentativa anterior. Isso demonstra a passagem nativa pelo percurso final-I/O corrigido nesta sessão. A correção removeu o impedimento de conclusão local; não transforma a negação inconclusiva em isolamento qualificado.

Conferência de código: server_service_win32.c só atribui start_effect3 depois do resultado tipado PAPACC_SCQ_PROGRESS_CLOSED_INCONCLUSIVE, espera terminal do processo original, GetExitCodeProcess1168, QueryServiceStatusEx/ps_scq_terminal_status, readback virtual e scq_start_finish sem erro. A mensagem SESSION_CLOSED_INCONCLUSIVE/CONTROLLED_CLOSE=KNOWN deriva desse caminho. A linha intermediária SERVICE=REQUEST_SUBMITTED;COMPLETION=NOT_PROVEN é genérica; a mensagem terminal posterior é específica. Não se tomou exit3 isolado como prova. Nenhuma nova consulta SCM/processo/token/KSP foi executada pelo agente.

Limites: não há dump separado de runtime PID/token, status bruto CNG, todos os consumos individuais ou handles de close nesta saída. As provas internas do caminho tipado são correlacionadas ao código qualificado; não fabricar campos ausentes ou observação corrente. Estado SCM terminal é validado pelo caminho nativo original; não é lease atual. Histórico de tentativas anteriores, application-close anterior, retenção/reservas e quiescência global OS permanecem UNKNOWN. Este fechamento conhecido vale para a sessão presente, não encerra retrospectivamente outras coortes.

Marco alcançado: lifecycle diagnóstico READ executado e encerrado de forma controlada, com resultado explicitamente inconclusivo. Primeiro requisito restante para READ qualificado: demonstrar negação NTE_PERM+NULL sob os contratos K5, com mesmo controle positivo/identidade/proteção e todas as provas. BADKEYSET não satisfaz esse requisito. Nenhum RUNNING diagnóstico ou readiness produtiva é inferido.

Próximo trabalho: análise offline focal do resultado KSP inconclusivo e dos requisitos remanescentes, reutilizando evidências existentes e sem nova sessão/coleta/UPDATE por conveniência. Preservar a fixture, seus descritores, originais, autorizações consumidas e PST0.6.4. Não alterar K5, ACL, key ou critérios para obter PASS.
