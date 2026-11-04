# Observação dos participantes reconciliada

Fonte: saída integral fornecida pelo proprietário; JSON é transcrição estruturada, não captura byte-exata. Pedido e21756e4... utilizado sem retry. first0/consumedTrue/completeTrue/observer_close_knownTrue/wrapper0. Pelo caminho qualificado, as duas consultas SCM mantiveram START_PENDING/PID18000 e a revalidação usou os mesmos handles originais adquiridos pelo observador.

Controlador7580/criação134381210166495538/token126164448:0/auth570063:0/modified126164115:0/groups17 e SID original correspondem à sessão. Runtime18000/criação134381210451301160/token126200549:0/auth126195148:0/modified126195209:0/groups10 e SID virtual original foram observados e revalidados agora. Nenhum token ou handle impersonável foi exposto.

É correlação atual SCM/processo/token, não aquisição histórica do original interno do controller, autenticação bilateral, receipt/close, readiness, autorização de disposição ou prova da causa212. Fechamento conhecido é somente do observador; application close/retidos/quiescência OS permanecem UNKNOWN.

O código estaciona o controlador com start_effect1 e pode estacionar o ServiceMain por ps_park_retained sem reportar STOPPED. O serviço não anuncia SERVICE_ACCEPT_STOP em START_PENDING. Portanto não há procedimento de Stop normal demonstrado para liberar esta sessão. Não enviar ControlService nem executar a CLI do host.

Próximo gate: preparar uma decisão excepcional separada para os dois objetos fixos, com revalidação por handles/criação/token, configuração SCM e imagem esperadas. Não aplicar permissões consumidas dos PIDs148/15744. Somente após autorização explícita, efeitos/saída e readback suficientes poderão permitir UPDATE. Não inferir limpeza, rollback, refund ou quiescência global.
