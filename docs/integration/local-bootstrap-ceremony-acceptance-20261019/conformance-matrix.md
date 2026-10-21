# Matriz de conformidade focal

| Condição | Cláusulas | Resultado e limite |
|---|---|---|
| Prova local, sem negativa global | P0/P2/P3/P4 | Conforme; especialização explícita D2/D3.6, não alteração silenciosa |
| Ator autorizado e ação explícita | P1/P2 | Autorização fora do candidato; token não prova presença humana; sem bootstrap automático |
| Contêiner, criação exclusiva e custódia | P3/P7 | Todos necessários; ausência/CREATE_NEW/file ID isolados insuficientes; existente mesmo vazio negado |
| Histórico incerto/corrupção/restauração | P2/P4/P7 | DENY; não reset, adoption, migração ou recriação automática |
| Mesma root, transação e commit | P5/P6 | Sem CURRENT anterior fabricado; commit confirmado e fechamento antes de publicação; A/P/I não inventados |
| Update/CAS | P6 | CURRENT/base/pin/referências/CAS; conflict/UNKNOWN sem fallback |
| Concorrência e falhas | P3/P7/P8 | Perdedor não adota contêiner; custódia perdida, Stop e resultado incerto negam efeitos novos; retenção obrigatória |
| Autoridade operacional | P5/P9 | Nenhum grant, provider, listener, TLS ou admissão implícitos |
| Portabilidade | P1/P3 e delimitação do registro | Perfil Windows restrito ao adaptador; núcleo independente; box/ISA não qualificadas |
| Model E/ED08 | P7/P8 | Sem liberação por timeout/process exit/FINAL; sem retry destrutivo |
| ED11 | P9 | Limite 2097152; ALL anterior 2089223; margem 7929 não é aprovação; provider/kernel UNKNOWN |
| Histórico e layouts | P0/P5 | D3/LST1/BODY0.5/ROOT06 preservados; nenhuma migração aceita |

A aceitação é normativa, não prova nativa. Não foram executados builds, testes ou rede. A cerimônia ainda exige implementação e prova concreta de autorização, namespace/custódia e Stop. O subconjunto lógico estimado de 100 bytes não inclui todos os custos de token, handles, buffers, caminhos, auditoria e retenção; não constitui reserva. Censo compilado e coexistência são gates futuros.

Fontes integrais: os três documentos de initial-bootstrap-proof-review-20261019; decisões accepted D2/D4 e D3; contratos root/C8, Model E, ED08/ED11 conforme referências preservadas nesses documentos. Sem conflito material na especialização expressamente autorizada. Nenhuma regra de ausência global, freshness ou fencing é aceita.
