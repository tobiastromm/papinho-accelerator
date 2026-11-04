# Corte dos originais — alcance exato desta integração

Estado: PARTIAL. Não é decisão normativa, autorização ou pedido operacional.

## Callers efetivos

`main.c.api3.inc` chama `papacc_server_service_cli` antes de `papacc_server_entry`.
No Start fechado a CLI consulta o carrier original. Se ausente, chama agora
`papacc_server_launcher_scq_first_controller`, que exige o owner canônico ativo
na mesma invocação/thread e sem rede/publicação. Esse caller chama
`papacc_configuration_personal_scq_first_controller` no mesmo packet.

A composição nova exige perfil TO pré-runtime e as três referências nativas já
capturadas. Executa hashing/verificação existente quando necessário, constrói o
candidato query-only, captura a fonte com `scqa_capture`, apresenta/confirma SCQ,
apresenta/confirma o ato INDEPENDENTE de Start e prepara o session driver. Não
consome nenhum ato, não abre endpoint/SCM e não concede capacidade. Cada chamada
existente revalida suas identidades e Stop; não há callback positivo instalável.

Falha pré-efeito fecha somente o próprio perfil controlador ainda não consumido.
`papacc_server_launcher_scq_prestart_close` recusa liberar transporte, atos
consumidos ou UNKNOWN; usa o fechamento existente, remove o handler de console
e só então libera o owner. Falha de close retém. O fechamento terminal existente
também passa a remover o handler instalado por esta composição. Não se afirma
quiescência global do OS.

## O que NÃO foi integrado positivamente

1. **Controller frio:** a CLI não adquire hoje o plano operacional original e os
   três handles antes de adquirir o owner. O novo caller apenas compõe originais
   JÁ retidos; não os fabrica nem os abre a partir de argv/hash. A aquisição
   `session_acquire_originals` existente não ganhou um producer legítimo dessa
   seleção inicial. A entrada de processo frio continua DENY. Um teste positivo
   com referências modeladas não fecha esse corte.
2. **Receptor frio:** ServiceMain entrega SCQ1 DATA ao packet fechado. Em
   `scq_ingress_continue`, `registered_owner_decision` retorna NULL porque nem
   `scq_admin` nem `scq_recipient.registration` possuem original adquirido. O
   getter de decisão de endpoint também continua NULL. A função retorna antes
   de `scqb_open`. As aquisições de token/arquivos/conteúdo previstas APÓS essa
   abertura não fornecem a fonte anterior. Não houve ligação positiva nova aqui.
3. **Seleção independente de endpoint:** não existe registro original dessa
   decisão no packet. Confirmação SCQ/Start não é essa decisão. A aprovação
   normativa offline ou o registro SCM consumido tampouco a substituem.

O teste novo prova que um carrier completamente válido, gerado após a composição
controladora em modelo, continua sem produzir fonte num packet frio. Os testes
existentes do primeiro capture só completam o percurso com uma seleção receptora
INDEPENDENTE introduzida explicitamente pelo modelo. Não é autenticação nativa.

## Fronteira normativa precisa

OHI, seção Fonte proposta, exige objetos originais imutáveis/protegidos e afirma
que digest/caminho/elegibilidade/association DATA não bastam. B regra4 exige
pedido/decisão byte-exatos e confirmação atribuída; regra5 permite somente um
empréstimo da tentativa governada, não outra aprovação reconstruída no runtime.
SSO-B S1 exige a fonte original fornecida pelo controlador; S4 mantém a autorização
independente de endpoint/lifecycle. TO4–TO5 repetem origem, validação bilateral,
consumo e continuidade. Esses textos não foram alterados.

Trocar o getter por um candidato feito SÓ do carrier, de arquivos e de um token
elegível violaria essas condições. Compartilhar a seleção por ponteiro de fixture
violaria a separação entre processos. Portanto essas duas soluções foram
rejeitadas. **Isso não prova que toda solução técnica nos contratos seja
impossível, nem transforma a ordem atual do código em norma.** Demonstra a falta
de um producer nativo da primeira admissão e do vínculo independente de endpoint.
Uma solução deve especificar de onde vem o primeiro original e em qual fase ele
é legitimamente adquirido, antes de remover essa negação. Se optar por permitir
I/O candidato antes dessa origem, é necessário demonstrar sua autoridade própria
compatível com S4/OHI; não inferi-la de START_PENDING ou de SCQ1. Não se criou ADR,
store de grants, canal paralelo ou fonte administrativa reutilizável.

## Primeira lacuna / ação mínima

A primeira lacuna continua sendo a entrada legítima da seleção/pedido/decisão e
referências no controller frio, acompanhada da fonte inicial independente no
receptor/endpoint. A composição interna está ligada; a produção desses originais
não está. Não há hoje pedido de Start executável ou artefato final congelável.
Além desse corte, a política de suficiência exigida por TO7 permanece em DENY,
conforme RESOURCE-ADMISSION.md. Não solicitar amostras/retries para substituir
qualquer dessas origens ou provar ceilings opacos.
