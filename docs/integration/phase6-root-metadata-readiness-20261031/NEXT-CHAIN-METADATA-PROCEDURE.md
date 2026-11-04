# Continuação — procedimento mínimo read-only da cadeia

Ler CONSOLIDATED-CLOSEOUT.md e READONLY-CHAIN-REQUEST.CANDIDATE.md deste pacote, especificação/replay anterior e contratos accepted aplicáveis. Não executar consulta/staging/retry.

Concluir a preparação do procedimento no adapter/owner existente: somente raiz e componentes existentes, leitura OWNER/DACL/identidade, referências originais relativas ao parent, revalidação/close. Parar no primeiro ausente; não listar irmãos nem utilizar preflight que solicita criação. Auditar reutilização antes de acrescentar código; sem owner/helper/serviço/IPC novo.

Se implementação for indispensável, cumprir leitura de governança/build, apresentar fatos/plano e qualificar somente alterações afetadas com consultas nativas modeladas, sem coleta live. Direitos mínimos congelados, nenhum ajuste de privilégio ou concessão de autoridade. Não confundir classificação estrutural com AccessCheck geral.

Congelar procedimento/hashes reais, revisar o pedido candidato preservando esta versão histórica e apresentar SHA256 final para autorização específica de uma consulta manual. Não fabricar pins ou emitir comando não qualificado. Não alterar guarda antes da análise factual por aresta. Preservar seleção B3 e autorizações consumidas.

Sem coleta automática, ACL/SCM, diretórios/contas/CA, Start/Stop, endpoint, CURRENT, rede operacional, TPM/NV/Q4, cleanup/refund ou recuperação. Preservar DENY/UNKNOWN, HEAD/WIP/históricos/pins/reservas/PST0.6.4.
