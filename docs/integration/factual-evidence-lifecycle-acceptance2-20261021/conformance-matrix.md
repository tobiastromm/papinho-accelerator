# Matriz de conformidade — correção E1/E6/E7 e E0–E8

Escopo: aceitação normativa da alternativa borrow-only exata apresentada ao owner. Sem implementação/qualificação de aquisição nativa. R1/R2 deixam de ser ambíguos nesse perfil restrito; o perfil de acquire que inicia trabalho assíncrono NÃO é aceito.

| Condição | Fonte e resolução | Disposição |
|---|---|---|
| Tentativa de acquire SDK | E1/E6: ligação síncrona sobre grafo já adquirido; retorno é seu resultado | CONFORME, sem pendência própria/ID novo |
| Preparação nativa anterior | E6/F4/G7: operação/lifetime do owner original, consulta factual precisa ser demonstrada antes do binding | CONFORME_COM_GATE; não é prova de fonte disponível |
| Sem lease devolvido | DENIED/INVALID não transferem/liberam nada; recursos anteriores conservados no owner original | CONFORME, ausência de lease não prova ausência de recursos |
| Aquisição pendente/retida | acquire nunca RETAINED; preparar pendência dentro dele é incompatível e deve negar | CONFORME_PERFIL_RESTRITO |
| Falha definitiva vs incerta | resultado do produtor consultado no contexto original; incerteza retém, falha não é quiescência | CONFORME_COM_PROVA_FUTURA |
| Stop e quiescência | E7/F4/Model E: Stop não cancela, não destrói, não devolve crédito por tempo | CONFORME |
| Release INVALID | sem tentativa/efeito parcial, owner e borrow previamente conhecido preservados | CONFORME |
| Lease reconhecido | precondição de borrow produtor válido; INVALID não valida ponteiro arbitrário | CONFORME, não criar registry de pointers |
| Estado depois de query INVALID | disposição anterior intacta, query-only continua depois de retenção | CONFORME |
| Primeira tentativa vs retry | corrigir precondição comprovadamente sem tentativa não é reemitir close; após tentativa somente query | CONFORME_EXACTLY_ONCE |
| Signer/purpose/issuer/CURRENT/status | E2–E5 byte-exatos, autoridade Accelerator F1/F2, independent Browser F3 | CONFORME_NORMATIVO; fatos positivos continuam DENY |
| Separação SDK/Browser/root | S0–S8/D3/F4/G6, sem codec root no Browser, sem autoridade nova | CONFORME |
| M1/segredos | TRUST antes KDF, CONTENT/recheck antes release; lifecycle error nunca APPLICABLE | CONFORME; máquina offline existente não mudou |
| C89/VC6/versionamento | opacos/enums/const por inspeção; revisão2 documental, sem wire/ABI pública nova | CONFORME_COM_TARGET_GATES; nenhum build/teste |
| ED11 | metadados/backing/refs/pins/retidos concretos ainda não qualificados, quotas intactas | CONFORME_COM_ACTIVATION_DENY |

Não se aceita uma nova fonte/handle de aquisição pendente. O procedimento de consulta da preparação nativa deverá ser ligado ao owner existente na implementação; sem isso não se inicia efeito dependente nem se cria contexto artificial. Essa limitação não é uma lacuna de resultado do SDK: acquire não pode iniciar a pendência. Factuais source/produção são gates diferentes da aceitação da interface restrita.

S0–S8, M0–M8, D3, F1–F5/G0–G7 e Model E/ED08/ED11 não são alterados. Cláusulas históricas PROPOSED no anexo só têm status normativo superado pelo novo registro; proibições de implementação permanecem. R1/R2 anteriores ficam encerrados apenas no escopo corrigido e permanecem históricos no relatório original.
