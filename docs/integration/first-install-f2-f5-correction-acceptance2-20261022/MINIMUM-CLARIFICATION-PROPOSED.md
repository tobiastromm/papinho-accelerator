---
status: proposed
revision: 1
date: 2026-10-22
---
# Minimal F2/F3 clarification alongside the preserved F2/F5 correction

This addendum is PROPOSED, unapplied and subject to exact future acceptance. Read together with the original F0–F9 and the preserved F2/F5 correction. It does not alter accepted P1, select a source, add corporate prerequisites or qualify local installation. F0/F1/F4–F9 remain unchanged except for the already proposed F5 replacement; all effects stay denied.

## F2 — add after the proposed replacement paragraph

A projeção da prova deve distinguir campos efetivamente autenticados pela fonte de observações/associações do adapter. A fonte deve comprovar: (i) identidade do autorizador e sua competência sob política independente; (ii) identidade do operador autorizado ou predicado de operadores explicitamente permitido, cuja correspondência com o operador nativo o adapter deve verificar; (iii) alvo ou escopo de alvos autorizado; (iv) finalidade NEW_INSTALL e escopo/configurações permitidas; (v) origem, referência/revisão protegidas e condições de validade da decisão/política; (vi) status e resultado factual de revogação conforme o mecanismo da fonte. A fonte não é declarada autora da identidade nativa observada ou do vínculo local a nonce/invocação que não tenha autenticado. Aprovação genérica ou falta de uma condição obrigatória implica DENY.

Quando a fonte emitir aprovação individual, preservar seu identificador autenticado no namespace da própria fonte, separado de sua revisão. Quando a fonte fornecer somente política/delegação explícita que admita o perfil B, registrar APROVAÇÃO_INDIVIDUAL_NÃO_EMITIDA e a referência/revisão/regra protegida que foi efetivamente avaliada; não fabricar identificador externo a partir do nonce ou de contador local. O identificador de correlação do ato local não é identificador de aprovação da fonte. A ausência de aprovação individual só é admissível se a política permitir expressamente a avaliação local do pedido; se exigir aprovação por tentativa, sua ausência nega.

Revogação deve distinguir pelo menos REVOGADO, NÃO_REVOGADO_COMPROVADO e DESCONHECIDO. Uma fonte sem API de revogação pode fornecer prova equivalente de autorização corrente por mecanismo qualificado, mas não recebe por omissão o valor NÃO_REVOGADO_COMPROVADO. Resultado desconhecido, expiração, invalidação ou falta de capacidade de revalidação nega novos efeitos. Não se cria formato persistente, enum C, protocolo ou serviço de status por esta exigência semântica.

A decisão da fonte e a associação protegida não são a decisão final de admissibilidade. O Accelerator verifica cumulativamente a competência/escopo comprovados, correspondência de TODOS os elementos do pedido, integridade/ownership/lifetime da associação, exclusividade/custódia, Stop, recursos e demais contratos vigentes antes de cada efeito aplicável. Qualquer gate pode negar mesmo com fonte favorável. O adapter não pode converter associação correta ou aprovação externa em dispensa de P1, criação exclusiva, custódia, issuer ou ED11.

## F3 — replace only the sentence beginning “A mesma decisão vincula”

No perfil A de F2, a decisão autenticada da fonte vincula a configuração exata validada e suas referências protegidas; no perfil B, a associação local protegida vincula esses mesmos elementos à decisão/política independente, depois de comprovar que pertencem integralmente ao escopo autorizado. Em ambos os perfis o vínculo permanece distinto da decisão final de admissibilidade do Accelerator e não amplia a autoridade concedida pela fonte.

## Scenario boundary for exact acceptance

F1 não exige infraestrutura corporativa. Um perfil individual somente poderá ser qualificado quando uma cerimônia local concreta comprovar legitimidade/competência independente e atender F2/F3, sem usar token/posse/flag isolados como aprovação. Essa cerimônia ainda não é definida ou aceita por este addendum; o perfil continua negado. Um perfil institucional igualmente depende de fonte real, autenticada e qualificada, sem fornecedor obrigatório. A futura aceitação de F0–F9 deve registrar explicitamente essas limitações, sem transformar ausência de perfil local em obrigação de instalar um serviço corporativo.
