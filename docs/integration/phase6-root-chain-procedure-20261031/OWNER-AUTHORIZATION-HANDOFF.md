# Autorização registrada e handoff manual

O proprietário respondeu literalmente `autorizo` à pergunta de aprovação de uma consulta manual do pedido SHA256 `df0e9ee77bf61aabf7535e2037fa5788dff6c1a052b0748949cf5cd76d0afa94`.

Registro separado: [OWNER-CHAIN-QUERY-AUTHORIZATION.json](OWNER-CHAIN-QUERY-AUTHORIZATION.json), SHA256 `3ed18cbd5ffd8d5f9a5b54940b528235f4c677cc49a7421d98947e1e803d9ec8`.

Foram reconferidos os bytes do pedido, escopo original, receita, comando e três arquivos do pacote: todos correspondem aos pins congelados. Nenhum rebuild, troca de artefato, alteração do pedido ou reuso de aprovação consumida. O estado NOT_AUTHORIZED nos documentos originais/pins permanece histórico; este ato posterior registra a autorização específica, sem reescrever a preparação.

## Única execução pelo proprietário

Usar Windows PowerShell 5.1 x64 já aberto como administrador no host/operador previstos. Colar o bloco completo de [MANUAL-COMMAND.txt](MANUAL-COMMAND.txt), SHA256 `b185035c0394c5f82d08565fbbc9a16efe13b70300f26d386e01a76bdc16b693`.

O bloco verifica pedido/receita/escopo/pacote antes da chamada nativa. Não chamar o .ps1 diretamente, mudar execution policy ou acrescentar retries. Identidade/token, known folder, objetos e demais gates frescos serão avaliados durante a única execução manual; conferência de hashes offline não os substitui.

Após saída, enviar o resultado integral. Em qualquer falha ou UNKNOWN, parar: não repetir o bloco, não ampliar direitos e não alterar ACLs. A submissão usa a autorização mesmo quando a coleta falha; falha anterior à submissão exige reconciliação antes de qualquer decisão de reuso. O registro não é grant técnico, ledger ou garantia de consumo entre invocações.

Consulta NOT_EXECUTED pelo agente. Consumo/sucesso/fechamento futuros ainda não observados. Staging, SCM/Start/Stop, alterações ACL, CA/CURRENT, endpoint, rede operacional, TPM/NV/Q4, cleanup/refund e recuperação continuam sem autorização. E21 PENDING; ED11/Phase6 PARTIAL; PST0.6.4 preservada.
