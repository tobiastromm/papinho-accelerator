# Reconciliação WS3M — acesso ao controlador

A consulta manual foi concluída com first=0, complete=True e fechamento do observador
conhecido. Sua autorização está consumida, sem retry. Saída original preservada em
OWNER-WS3M-CONTROLLER-QUERY-ACCESS-OUTPUT.txt; fatos em
WS3M-CONTROLLER-QUERY-ACCESS-RESULT.json.

O runtime observado não possui nenhum dos SIDs beneficiários das DACLs atuais do
processo/token do controlador. SeDebugPrivilege está ausente. O código exige reabrir
ambos em scqb_open_mode e reabre novamente o token em scqr_acquire. É uma barreira
concreta de compatibilidade atual, sem prova do erro nativo histórico da sessão.
Não atribuir o timeout exclusivamente a essa causa nem declarar bilateral PASS.

Menor solução proposta: duas referências originais com direitos somente query,
entregues pelo controlador já autenticado ao runtime por meio do owner existente.
Não modificar ACLs/privilégios. O direito TOKEN_QUERY não permite impersonação;
nenhum PROCESS_DUP_HANDLE será entregue ao serviço. Há especialização necessária
no formato privado, espera inicial e close. Não implementar por alteração silenciosa
de SQB2, cuja aprovação limitou o delta a8bytes de deadline.

Proposta única: CONTROLLER-QUERY-REFERENCES-SQB3.CANDIDATE.md
SHA256: 726e3f786b6ca187a0058734b6c9fd9e67d2d2b9d465f59bc9bd5313cbbe2c4c

Nenhum código operacional alterado. Builds/testes de produto não repetidos.
Comparação offline dos SIDs: PASS, interseção vazia; não equivale a AccessCheck.
Nenhuma coleta/UPDATE/E/Q/S/ACL/disposição foi executada pelo agente.

Primeiro gate: decisão focal sobre a proposta. Processos19860/19836 continuam
retidos; a observação não autoriza encerrá-los. Application close, histórico,
bilateral original e quiescência OS continuam UNKNOWN. Não reutilizar sessão.
