# Correção mínima candidata — E1/E6/E7

STATUS=PROPOSED. Não modifica a proposta original nem contrato accepted. Patch exato anexo é somente material para revisão, não uma alteração aplicada. Nenhuma implementação/aceitação foi autorizada por esta preparação.

A alternativa proposta é restringir acquire ao empréstimo síncrono de um grafo já preparado e protegido pelo owner existente. O SDK não inicia aquisição nativa nem precisa recuperar aquisição parcial. Se isso não atender o owner factual, não simular sucesso: propor operação de aquisição explicitamente alcançável no mesmo owner, sem registry paralelo. Essa escolha exige decisão específica antes de aceitar os bytes corrigidos.

Substituições exatas propostas:

## E1 — imediatamente antes de sp_trust_acquire

```c
/* acquire never returns RETAINED; revalidate may retain an existing lease. */
```

## E6 — substituir o primeiro parágrafo

acquire SHALL ser uma ligação síncrona e all-or-nothing a referências já factualmente adquiridas, protegidas e contabilizadas pelo owner da operação existente ANTES da chamada. Não inicia aquisição nativa, I/O, trabalho assíncrono, nova alocação ou aquisição parcial de ref/pin/lease; não publica borrow parcial. Se não for possível comprovar essas precondições, nega sem iniciar o trabalho dependente. Sucesso exige E2–E5 integrais, referências transitivamente protegidas pelos owners reais e correlação à mesma operação existente; não exige que root, issuer e destinatário se tornem um único owner.

Resultados permitidos de acquire são SOMENTE SP_TRUST_APPLICABLE, SP_TRUST_DENIED ou SP_TRUST_INVALID; acquire NÃO retorna SP_TRUST_RETAINED. APPLICABLE escreve o único borrow completo no output válido/disjunto; DENIED/INVALID deixam o output intacto e não transferem, liberam ou criam ownership. Referências pré-adquiridas e qualquer trabalho anterior retido continuam alcançáveis pelo owner existente e seguem seu lifecycle próprio, nunca são descartados por esse retorno. O lease é metadado público transitório já reservado dentro desse owner, não token bearer copiável ou capacidade de outra operação; sua dimensão e custo ainda exigem censo de implementação. Core recebe borrow somente após aquisição factual; dados candidatos permanecem DATA.

Em revalidate, APPLICABLE confirma apenas a fase solicitada no mesmo lifetime. DENIED/INVALID não liberam o lease ou suas dependências; cessam o avanço M1. RETAINED de revalidate pressupõe lease já conhecido, conserva esse lease e toda a operação, proíbe KDF/release de conteúdo e remete ao encerramento governado do mesmo owner. Nenhum resultado de revalidate autoriza retry destrutivo, fallback, novo pacote ou reconstrução de evidência.

## E7 — substituir o início do segundo parágrafo

SP_TRUST_RELEASE_INVALID significa SOMENTE precondição inválida, sem executar tentativa de liberação. Para um lease previamente válido e conhecido pelo seu owner, conserva o mesmo borrow/ownership, dependências, créditos e estado de tentativa anterior; não significa RELEASED nem autoriza limpar/reutilizar a memória. Corrigir uma precondição somente quando o owner comprovar que a tentativa factual ainda não ocorreu não é repetir uma tentativa. Depois de uma tentativa efetivamente iniciada, o resultado do encerramento deve ser RELEASED ou RELEASE_RETAINED, nunca INVALID para ocultar incerteza. Query com precondição inválida não muda disposição ou ownership e não reabre direito a release. Não invocar a API com ponteiro/lifetime desconhecido ou já invalidado.

release autoriza uma única tentativa efetiva governada pelo owner;

O restante de E7 continua exatamente como na fonte. A expressão existente “Repetição de release/destrutivo negada” passa a referir a tentativa efetiva, não uma chamada cuja precondição impediu qualquer tentativa. Nenhum retry é autorizado se o owner não comprovar ausência de tentativa; incerteza mantém retenção.

Demais E0–E8 não são reabertos. A correção não aprova backend, fonte factual, CURRENT, confirmação Browser, key/signature, armazenamento, rede, reserva ED11 ou migração. Não é nova ABI/wire ou novo pin system. Submeter a opção e os hunks à decisão do owner e revisão oficial antes de qualquer registro ACCEPTED.
