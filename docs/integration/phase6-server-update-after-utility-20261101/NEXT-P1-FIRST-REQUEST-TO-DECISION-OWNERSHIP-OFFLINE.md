# P1 — ownership entre primeira abertura e guarda da decisão

Continuar P1-CHAIN-SESSION-RECONCILIATION.md e resultado JSON.
Autorização608b6...4441 utilizada, sem retry. Dois originais novos preservados:
request417614100:1441792:542844 e decisão417614100:1179648:542846; coorte documental
fechada conhecida. Primeira NtCreateFile(request0) passou0/0/Information1/handle1.
Consulta seguinte pré-decisão(index1) contou125 e guard>=124 negou antes do open.
E/Q/S/Start/cadeia de imagem não alcançados. Não recriar decisões ou sessão.

Objetivo exclusivamente offline: inventariar APIs/handles/ownership/lifetime
do trecho entre as duas queries. Inclui primeiro NtCreateFile e informação,
GetSecurityInfo/SD, papacc_store_hash e seu close de hash/provider, LocalFree,
GetDriveType/QueryDosDevice do próximo arquivo. Referência original do pedido
deve continuar retida. Não presumir vazamento ou fechar internals Windows.

Identificar se há duplicação, retenção dispensável comprovada ou reorganização
legítima sem antecipar/ocultar guard, reduzir prova original ou liberar UNKNOWN.
Não importar113 da sessão antiga como baseline desta: count inicial exato não
foi preservado; sucesso da primeira abertura só demonstra<124 naquele corte.
Snapshot NT referência0 e count125 referência1 são fases diferentes.

Se houver defeito funcional demonstrado, corrigir e qualificar offline somente
as regressões afetadas, sem nova ADR/mecanismo/serviço/root/autoridade. Caso não,
delimitar a menor evidência faltante com custos do observador, sem executar coleta
ou preparar retry. Nenhuma observação adicional está autorizada por este prompt.

Preservar guard124/cap128/G3, originais/EXEs/retidos/reservas/atos consumidos,
HEAD/WIP/históricos/pins/fixture/PST0.6.4/DENY/UNKNOWN histórico e OS.
Não executar UPDATE/coleta/endpoint/SCQ/Start/Stop/SCM/ACL/CA/CURRENT/rede/TPM/NV/
Q4/limpeza/refund/recuperação ou propor aumento de quota por conveniência.
