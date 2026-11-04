# Primeiro ingresso nativo — fronteira comprovada, integração incompleta

Task PACC-PHASE6-NATIVE-FIRST-INGRESS-AND-SCM-ACT-BINDING. Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preservados. Nenhum efeito operacional, consulta SCM, Start/Stop, endpoint, atualização de imagem ou alteração de autoridade.

## Resultado

O critério de sucesso NÃO foi atingido nesta execução. Não há nova ligação positiva no código de produção. Acrescentada uma regressão focal que demonstra mecanicamente a lacuna, sem usar source/receipt de fixture como autoridade nativa. A integração remanescente é técnica; não foi demonstrada impossibilidade nos contratos accepted nem necessidade de reaprovar SSO-B ou criar ADR. A ausência de aprovação operacional não dispensa implementar os callers offline.

## Cortes exatos na cadeia

1. personal_scq_admin_win32.inc: scqa_start_take consegue tomar o ato independente antes de existir PID/token do runtime. Isso é um ato no controlador. Não muda o estado do módulo SCM, não entrega referências remotamente e não autoriza endpoint.
2. server_service_win32.c: ps_binding_ready permanece gate antes de OpenSCManager/StartServiceW. Não há caller que associe o ato original Start ao SCM owner com revalidação do serviço/imagem/comando/parado e consumo no efeito. O novo teste confirma que um Start tomado em outro packet modelado não limpa missing_pre_start=15.
3. personal_scq_transport_win32.inc: scqt_create exige driver controlador phase3, que só vem depois de scqs_offer e observação original do runtime. Antes disso, não se cria o nome efêmero do endpoint. O teste toma o Start antes de runtime e verifica que create falha antes de event/pipe/RNG.
4. server_service_win32.c/ps_main: o perfil closed aceita exatamente argc1, apenas o nome SCM. Não existe seletor transitório que entregue o nome efêmero ao callback. O teste fornece args adicionais e comprova rejeição anterior a registrar handler/anunciar status/executar runtime. O caminho persistido service host scq-closed sozinho não contém os originais.
5. personal_scq_transport_win32.inc/scqt_open_local: exige session/profile e original local com refs válidas antes de abrir. Sem original, não chama CreateFile/CreateEvent. scqr_open ainda exige scqr_current, que exige os originais locais independentes.
6. scqt_deliver_refs: o controlador só duplica as três referências depois de scqt_current validar o endpoint e o par runtime original. Os valores resultantes são custódia no processo destino; não há produtor que os disponibilize ao receptor inicial. Não são um canal implícito de descoberta.
7. personal_scq_recipient_win32.inc/scqr_first_prepare: exige inputs content_verified e leitores originais antes de criar receptor. O teste observa o runtime (em modelo) e passa packet sem inputs: rejeição sem aquisição de endpoint. Portanto o primeiro preparo acrescentado anteriormente NÃO resolve a entrega que teria de produzir esses inputs.
8. server_launcher_owner_win32.c/captured_empty: fecha somente o conjunto vazio. Remover essa guarda, aguardar indefinidamente ou anunciar RUNNING não cria fonte. Nenhuma dessas alterações foi feita.

A cadeia é: originais locais -> abrir/validar endpoint -> entregar refs -> disponibilizar originais locais. Além disso, nome criado depois do Start não é entregue por nenhum ingresso ao callback atual. Esta é uma circularidade de implementação, não uma obrigação normativa de ter PID futuro ou CA anterior.

## Normas

B regras2/3 permitem implementar uma representação privada restrita para empréstimo entre os dois principals, com originais/PID/criação/token/grupos/imagem/serviço e direitos mínimos. Regra4/OHI exige origem e confirmação atribuída, não DATA como aprovação. Regra5 mantém consumo no controlador antes dos efeitos; regra6 preserva lifetime/UNKNOWN; regra7 mantém recursos. SSO-B S1/S2/S3 permite preparação e binding tardio, S4 separa scopes operacionais, S5/S6 impedem replay/ponteiros/segredos e S7 exige código/provas antes de operar. TO0–TO9 dispensa somente key/KSP nesse perfil.

Não há conflito normativo identificado. Implementar um ingresso tipado privado nesse escopo não exige por si nova ADR, mas precisa validação efetiva por ambas as pontas. Não se pode resolver a circularidade com setter, callback positivo, getter NULL substituído por grant, argv como aprovação, cast de número remoto como referência original ou espera/RUNNING sem evidência.

## Código e testes desta execução

Novo tests/scq_native_first_ingress_boundary_test.c usa os corpos reais das guardas e ServiceMain com APIs de processo/token/SCM/endpoint/RNG simuladas pela fixture existente. CMakeLists.txt integra um target/CTest. O teste pre_readiness recebeu somente um macro de nome de entry para composição dos ensaios, sem alterar sua semântica. Código de produção permaneceu intacto.

Quatro compilações do target Selected/Legacy Debug/Release PASS, zero warnings finais; quatro pares CTest/configuração novos PASS, 28 verificações por configuração. Não chamar isso de quatro rebuilds completos do produto ou de prova bilateral. A primeira compilação do novo teste encontrou colisão de macro main no harness; corrigida pela entry macro específica antes das quatro compilações finais. Não repetida campanha histórica de 57 cenários. Seus resultados anteriores permanecem válidos apenas no alcance já declarado.

O teste mantém o ato consumido e encerra seu source como retido/UNKNOWN; nenhum fechamento de recursos modelados foi promovido a quiescência OS. SID helpers reais não são prova de contexto SCM real.

## Imagens e pedido

EXE staged histórico permanece c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf/571392 bytes. Não foi reaberto nem atualizado nesta task. Workspace Selected/Release continua 890dd9276993f8cf80b089f7dd33d5feb7ac71d0e8af45d2ef4740e067fb22db/582144 bytes, conferido somente no workspace. Nenhum binário produtivo mudou nesta execução; mudanças são no harness/CMake/documentação. O delta de código runtime da task anterior continua a exigir atualização para ser qualificado no processo real, mas o candidato ainda não é final.

Não congelar/selecionar imagem nem preparar update executável enquanto falta o ingresso. Procedimento candidato anterior continua delimitação, não autorização de overwrite. O stager exclusivo não é updater de arquivo existente. Nenhuma autorização utilizada foi reaproveitada.

Pedido Start NOT_READY. Primeira pendência técnica é o carrier inicial, com captura não autorizadora e validação original anterior ao consumo/uso. Em seguida ligar ato/target ao SCM owner e completar recursos próprios; só então estabilizar artefato e preparar pedidos separados de atualização, endpoint e lifecycle. Ver NEXT-PRIVATE-BOOTSTRAP-CARRIER-IMPLEMENTATION.md. Não há comando operacional nesta entrega.

E21 PENDING; ED11/Phase6 PARTIAL; OS quiescence UNKNOWN; PST0.6.4, WIP, históricos, pins, reservas e chave fixture preservados.
