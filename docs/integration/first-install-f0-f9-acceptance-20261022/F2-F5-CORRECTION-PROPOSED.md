---
status: proposed
revision: 1
date: 2026-10-22
---
# Correção focal F2/F5 — origem externa e associação local

Somente os parágrafos abaixo são propostas de substituição. Não alteram a proposta original nem P1 accepted. Exigem revisão exata futura e não selecionam fornecedor, fonte, API, protocolo ou backend.

## F2 — substituir o parágrafo iniciado por “Esses campos”

Esses campos são requisitos semânticos, não um novo arquivo/token serializado, ABI ou protocolo. Admitir dois perfis explícitos de prova: (A) decisão externa autenticada já ligada ao pedido completo; (B) decisão/política externa autenticada que autorize explicitamente o operador e a operação NEW_INSTALL dentro de um escopo verificável, mais associação local protegida feita pelo adapter canônico. No perfil B, o adapter resolve competência, finalidade, alvo e configurações permitidas na fonte independente, verifica que TODOS os elementos do pedido específico pertencem ao escopo autorizado e vincula a mesma evidência/revisão protegida à identidade nativa do operador, parent retido, nome relativo, configuração exata, invocação e nonce32. Uma aprovação genérica de instalar uma aplicação, um nome de aplicação sem versão ou uma permissão de filesystem não bastam para inferir esse escopo. Ausência de qualquer condição, autenticidade, revisão, status, borrow ou revalidação implica DENY. A associação local não pode criar competência, substituir a decisão externa, ampliar seu escopo, nem emitir autorização reutilizável. Ambos os perfis exigem fonte resolvida fora do candidato por cadeia administrativa já estabelecida e adapter qualificado; não aceitar strings, flags, booleanos ou digest isolados como prova suficiente. Se a decisão externa não permitir a associação local, somente o perfil A pode ser usado. Não criar serviço/registry/formato persistente adicional para implementar qualquer perfil.

## F5 — substituir a frase iniciada por “A fonte deve vincular”

No perfil A de F2, a fonte autentica a decisão ligada ao contexto completo. No perfil B, o adapter deve provar a associação local protegida ao mesmo escopo, evidência/revisão, operador e pedido, conforme F2; essa correlação não representa nova competência administrativa. Um ID ou nonce esperado pelo caller, membership, callback booleano ou configuração candidata não prova nenhum dos perfis. A transição única AUTHORIZED -> CONSUMED, as revalidações e a ausência de rearmamento/retry permanecem integrais. Reutilizar uma política externa não cria automaticamente nova decisão para outra tentativa: é necessária nova avaliação explícita do pedido na mesma fonte e nova associação/invocação/nonce, depois da disposição factual da tentativa anterior. Se o escopo da fonte exigir aprovação humana individual por tentativa, uma avaliação automática da política não a substitui.

## Limites

A correção não declara que Configuration Manager ou outro mecanismo já satisfaça o perfil B. Seleção e qualificação exigem prova concreta do escopo de NEW_INSTALL, proveniência, autenticação, revalidação, configuração permitida, consumo/lifetime e ED11; sem isso, SOURCE_ABSENT/NOT_QUALIFIED/DENY. F0/F1/F3/F4/F6–F9, P1, issuer/custódia, Stop/retenção e todos os gates permanecem. Não ampliar o privilégio de administrador para aprovação implícita.
