# PUBLISHED A/P — fronteira concreta de representação

Estado: análise funcional / proposta focal, **não accepted**, sem novo formato aplicado.

Norma vigente: DECISION-issuer-c2-epoch-authority-relation-20261019, P1.1/P1.2/P3.1–P3.5. A é confirmação administrativa emitida pela mesma root após commit; P identifica a autorização operacional vigente. I/R/A/P não são intercambiáveis. Nova representação privada requer revisão versionada própria. A autorização técnica desta tarefa não modifica essas cláusulas.

## Contradição que a implementação precisa resolver

1. O core propõe ISSUED_UNPUBLISHED → PUBLISHED com published_epoch ainda zero.
2. PACIS601 versão 1 codifica published_epoch como u64 no payload do registro, offset 260. A função transition rejeita PUBLISHED com esse campo zero **antes** do append/commit. O novo teste comprova que a imagem permanece inalterada nessa rejeição.
3. A função nativa issuer_publish06 só gera A depois que o worker confirmou o commit. Escolher P, revisão, PID ou um valor futuro para preencher o campo não resolve essa diferença de fases.
4. O core ainda compara o published_epoch retornado pelo adapter com after.epoch. No root live, epoch é P. Essa igualdade não é prova de A; a ligação de metadados preservou conscientemente publish=1 como UNSUPPORTED.
5. ROOT07 permite registrar A/P após publicação, mas seu link não codifica por si só um ato específico de publicação de uma credencial. Transformar todo ISSUED_UNPUBLISHED com checkpoint em PUBLISHED concederia um efeito que o ato não demonstrou.

Não há conflito entre ADRs accepted a resolver por reinterpretação. Há uma lacuna da implementação/representação atual. Nenhum número foi fabricado e nenhuma transição passou a retornar sucesso.

## Menor decisão/patch focal a preparar

Recomenda-se revisar, **no mesmo codec/root**, uma representação versionada que separe:

- intenção autorizada de publicação, vinculada a request/credencial/CSR/serial/high-water, predecessor R e P efetivo;
- commit factual dessa intenção sem concessão de autoridade à credencial;
- seleção da root e emissão de A pós-commit;
- ack específico da intenção, vinculado aos bytes originais, R/I/A/P e ao proprietário da transação;
- releitura e confirmação final antes de projetar PUBLISHED para admissão;
- ausência/contradição/Stop/UNKNOWN: credencial não admitida, referências e serial retidos.

O ack não pode ser um bool, callback permissivo ou setter. Antes de aplicar bytes novos, definir a representação exata, compatibilidade com PACIS601/ROOT06/ROOT07 e preservação dos históricos; obter a revisão específica exigida por P1.2. Não se propõe nova root, ledger, serviço ou família de ADRs. Não se aceita automaticamente uma revisão do codec.

Alternativa: manter a representação atual e manter PUBLISHED/DENY. Ela preserva segurança, mas não satisfaz o caminho NORMAL completo. Nenhum novo ensaio de CAS, TPM ou consulta ownerAuth responde a essa lacuna.
