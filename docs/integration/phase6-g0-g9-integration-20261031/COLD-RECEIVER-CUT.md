# Corte original do receptor — sem concessão por DATA

G0 accepted não altera a origem administrativa: o preâmbulo exclui essa
redefinição e G2 diz expressamente que os producers incompletos não são
resolvidos/dispensados pela especialização de recursos. B4 exige decisão original
byte-exata e confirmação atribuída; B5 permite empréstimo restrito, não segunda
aprovação ou restauração por arquivo. S3 permite metadados/referências permitidos;
S4 conserva autorização independente de endpoint.

## Dependências factuais no produto

1. ServiceMain recebe SCQ1 como DATA; carrier_capture conserva seletores,
   não instala decisão/endpoint. START_PENDING não autoriza por si I/O privado.
2. ingress_continue exige registered_owner_decision e endpoint_decision exatos
   antes de scqb_open. No owner frio não há scq_admin confirmado nem empréstimo
   receptor autenticado; os getters corretamente negam.
3. scqb_open obtém pipe, processo/token e SQB1; candidates verifica direitos e
   identidades; inputs verifica descritores/conteúdo e imagem; first_prepare
   constrói somente QUERY; promote/admit_offer exige seleção independente e
   originalidade antes da admissão. Receipt/close ainda são posteriores.
4. Portanto usar o próprio SQB1 para preencher previamente registered_owner_decision
   seria construir a origem a partir do objeto cujo recebimento depende dela.
   Copiar a seleção controladora por ponteiro num modelo mascara esse corte.

Não há API de aquisição original anterior ao pipe que forneça ao receptor esse
empréstimo e a autoridade específica da primeira conexão. Isso é uma ausência
do producer/caller, **não prova de impossibilidade de qualquer implementação**.
Sem demonstração da proveniência anterior, uma correção local que remova a
pré-condição ou derive uma aprovação de carrier/token/imagem viola G2/B4.

## O que não é solução

O mesmo SID administrativo, imagem com mesmo nome/ID/hash ou peer vivo não prova
que os três atos foram confirmados/consumidos. OHI não aceita arquivo de grant;
SCQ1/SQB1/ack não são autoridade. Não usar registro SCM consumido, helper novo,
grant persistido, impersonation ou estado RUNNING para fechar o ciclo.

## Próxima integração delimitada

Demonstrar uma fonte original anterior à primeira conexão, no mecanismo privado
existente, OU demonstrar a cláusula já accepted que autoriza uma captura QUERY
bounded antes da admissão, distinguindo a autoridade de conexão da autoridade
emprestada depois. Esse percurso precisa de G3 próprio, identidade/continuidade
originais e validação independente; não basta renomear o estado para QUERY.
Somente então implementar os callers e negativos de ausência/substituição/replay,
com APIs simuladas, sem nova aprovação G0. Se depender de nova autoridade não
coberta, apresentar exatamente essa fronteira ao proprietário antes do código.

Nenhum pedido operacional pronto, comando com placeholders ou nova ADR criado.
O guard explícito de indisponibilidade no relatório impede que a nova entrada
controladora fria abra namespace ou chame Start enquanto esse producer faltar.
