# Resultado manual BINARY_ONLY — reconciliação

Fonte: saída de console fornecida pelo proprietário nesta conversa. Não é captura assinada nem atestação de imagem mapeada. Timestamp da execução e SID não constam da saída: UNKNOWN.

Pedido aprovado: 3acf886784123d2f3523736077bcf03b728c56fc590c87edc0aea80d6ad158cf.
Registro original: e001fa0cf42d91b917c6b18f3168cff85ab4e077b0a4f822d64ce9032f0114cc.
Script: d1f72e217e90ee167267aada614c0bd1b04b12fce2ddc1de1c24cec12ce23c5f.
Executável: 4a68ce6bdb2737221824ba2f70930fbb8534ea5dfda3d9cfa460eaef22c546f9.
Esses quatro hashes foram reconferidos em disco durante a reconciliação. Isso não autentica retroativamente a imagem carregada ou o contexto da execução manual.

## Fatos reportados

| Propriedade | Resultado |
|---|---|
| Primeira falha | 1816, location 2, fase 6; sampler stage 9 |
| Entrada / query pós-frame | 109 / 109 handles; query bem-sucedida |
| Após layout binário, fase 7 | 132 handles; sample conhecido |
| Guard / cap | 124 / 128; excedidos em 8 / 4 |
| Aumento entre samples | 23 handles; não é atribuição interna por API |
| Private bytes | 2.826.240 → 3.551.232; aumento 724.992 |
| Working set | 6.582.272 → 8.192.000; aumento 1.609.728 |
| Memória física disponível mínima | 6.367.141.888 bytes |
| Commit disponível mínimo | 9.160.630.272 bytes |
| Volume / disco / arredondamento | UNKNOWN; não alcançados, não zero |
| Missing mask | 95 |
| Contexto nativo / token / SID | Não alcançados; predicate 0 não prova elegibilidade |
| Query pré-abertura | Não tentada; count_known=0 não significa contagem zero |
| Efeitos / RNG / confirmação | 0 / não chamado / não solicitada |
| Close controlado / retenção | known=1 / retained=0 |
| Quiescência interna OS | UNKNOWN |
| Exit nativo | 2; FAILED; DENY; NO_RETRY |

O consumed=0 reportado pertence ao ato de staging. A autorização da coleta manual foi utilizada segundo o resultado fornecido e não pode ser reutilizada. O registro original é preservado byte-exatamente; este documento é a reconciliação posterior, não uma edição da aprovação. Não há afirmação de ledger de consumo entre processos.

## Consequência técnica

A especialização BINARY_ONLY removeu a consulta desnecessária de ProgramData, mas não reduziu os handles observados neste ensaio: o resultado voltou a 132. O intervalo agora contém a resolução obrigatória de ProgramFiles por SHGetKnownFolderPath, validação/cópia/liberação e observação. Não demonstra leak nem identifica quais recursos internos causaram o aumento. O sampler registrou uma consulta válida, não uma falha de consulta com contagem desconhecida.

A API continua KF_FLAG_DONT_VERIFY e o caminho continua DATA, sem criação. CoTaskMemFree não fornece comprovação de fechamento de recursos internos. Não há referência controlada identificada que possa legitimamente ser fechada para eliminar os 23 handles. Não se altera cap/guard, não se desconta baseline, não se compensa com orçamento administrativo, não se troca backend e não se cria helper.

O pedido aprovado determina STOP se esse cold path continuar excedendo o limite. Consequentemente não se prepara pedido de staging protegido: capacidade, proteção original dos ancestrais, volume e retenção operacional ainda não demonstrados. Nenhuma coleta repetida idêntica é justificada por este resultado.

## Próximo gate concreto

A incompatibilidade factual atual é entre o caminho nativo observado e o envelope 124/128 vigente. Antes de outra execução, é necessário um fundamento técnico verificável para reduzir recursos dentro do mesmo contrato ou uma decisão explícita do proprietário para revisar o envelope aplicável. Esta reconciliação não propõe um número novo nem registra tal decisão. A adequação de eventual novo limite exigiria fundamentação própria; 132 observado não é teto ou reserva suficiente.

Não é necessário repetir a consulta para confirmar o mesmo bloqueio. Uma análise futura de alternativa de implementação deve demonstrar equivalência normativa e técnica antes de substituir APIs, autoridade ou processo. Nenhuma autorização operacional adicional é inferida.

Código inalterado; builds/testes anteriores não repetidos porque não houve correção técnica qualificada nesta reconciliação. Nenhuma operação protegida ou coleta foi executada pelo agente. HEAD/WIP, propostas e aprovações históricas preservados. Staging DENY; E21 PENDING; ED11/Phase 6 PARTIAL; PST 0.6.4 preservado.
