# Fontes originais e cadeia de ingresso — estado factual

## Controlador

Há agora três atos separados no mesmo PAPACC_SCQ_ADMIN_SOURCE: SCQ, endpoint e
Start. Não existe novo owner, grant ou formato persistido. O endpoint tem fase,
primeira falha, confirmação atribuída e consumo próprios. Seu getter passou de
NULL constante a empréstimo condicionado ao original local e à confirmação
explícita OWNER_SCQ_PRIVATE_ENDPOINT. SCQ, Start, registro SCM consumido, carrier
e token elegível não fornecem essa confirmação.

Também foi corrigida uma incompatibilidade real entre callers: a função
session_acquire_originals termina com scqa_capture em fase1; first_controller
recusava qualquer fase diferente de zero. Agora ela continua EXCLUSIVAMENTE a
captura original não confirmada do mesmo owner/thread/seleção; revalida-a e não
repete capture. Confirmação, consumo, replay e fonte substituída são rejeitados.

A composição qualificada é:
profile_prepare_transport_data -> session_acquire_originals -> first_controller
-> confirmação SCQ -> confirmação endpoint -> confirmação Start -> session_prepare.
A entrada alternativa em inputs já capturados continua válida. As fases positivas
foram exercitadas com objetos nativos modelados, sem endpoint, RNG, KSP ou SCM.
Isso qualifica os mecanismos e a compatibilidade, não sua proveniência operacional.

**A aquisição do plano e dos handles pela entrada de processo frio ainda não
está ligada.** main chama service_cli antes de adquirir o owner; esse comando
não recebe a seleção original. Não foi acrescentado um loader de grants, campo
approved, manifesto fabricado ou escolha de números para disfarçar essa ausência.
Nenhum pedido/decisão operacional de primeiro Start foi criado em nome do owner.
O backend de captura existe; falta o producer/entrada original que o alimenta.

## Endpoint

O getter agora recebe o owner explicitamente e valida a fonte original local,
o propósito TO, os objetos/token/continuidade e o ato endpoint atribuído.
Antes de consumir/abrir endpoint nativo, transport_enter verifica a admissão de
recursos. Enquanto os termos TO7 são UNKNOWN, nega SEM consumir o ato endpoint
e SEM chamar o provider. O caminho já com runtime vinculado não recebe um atalho
para criar endpoint sem consumo. Conexão/entrega e namespace-current exigem o
consumo endpoint. Close incerto retém; read-only query de seleção não é consumo.
Request, decisão, READ, N0, owner guard e permissões de chaves permanecem estritos.

## Receptor do ServiceMain

O packet frio ainda recebe somente SCQ1 DATA. O getter da fonte administrativa
independente e o binding de endpoint nele continuam ausentes. Não se copiou o
ato local/ponteiro do controlador nem se promoveu SCQ1 a origem.
scq_ingress_continue nega antes de scqb_open. Depois desse corte, o adapter
existente contém captura kernel do peer/processo/token, recepção candidata,
conteúdo/SD/imagem, scqr_first_prepare, promoção, receipt e close; sua prova
positiva ainda usa uma seleção receptora explicitamente modelada, NÃO nativa.

O defeito de compatibilidade do controller e a fonte endpoint local foram
corrigidos. **Isso não conclui a cadeia fria bilateral.** A proposta de garantia
não supre esses producers e não autoriza I/O sem a origem exigida.

## Como fechar sem outra autoridade

O próximo patch de ingresso precisa escolher um caminho concreto de aquisição
candidata do plano e dos objetos aprovados para a entrada nativa, utilizando as
primitivas de F1–F6/OHI e mantendo apresentação/confirmação próprias. Um path
poderia ser somente locator; nunca é a autoridade. O código atual não deve ser
tratado como norma que proíbe todo dado de entrada, mas nenhuma representação
persistida de grant pode ser inventada. Não basta adicionar um getter positivo.

No receptor, a aquisição inicial tem de comprovar a origem do empréstimo do
controlador, com seleção/pedido/decisão originais e autenticação independente.
Se uma solução requer abertura candidata anterior a essa prova, identificar sua
admissão própria compatível com SSO-B S3/S4; não derivá-la de posse do endpoint ou
START_PENDING. Não foi demonstrada impossibilidade universal sob os accepted;
esta ausência de implementação não é automaticamente uma nova lacuna normativa.

Enquanto faltarem esses vínculos e a garantia específica, Start/update/endpoint
NOT_READY. Não há pedido operacional completo, comando executável, fonte
administrativa legítima fabricável pelo agente ou autorização reutilizável.
