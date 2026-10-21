---
status: proposed
revision: 1
scope: complementary normative requirements; no wire selection
---

# Complementação mínima candidata T0–T7

Não ACCEPTED. Não modifica o P0–P9 original, ADR-0006/0009/0028, BODY/root/issuer, PST ou wire. Define o recorte de decisão necessário; os itens explicitamente pendentes continuam bloqueios de implementação. Aceitar requisitos de segurança não aceita uma porta/perfil/mensagem ausente.

## T0 — Especialização de propósito a decidir

O owner deve decidir explicitamente se autoriza um propósito restrito ENROLLMENT_INITIAL sobre mecanismo PST TLS1.3 server-auth, antes de existir certificado de cliente. Essa seria especialização apenas para bootstrap, não alteração de Secure Principal, que permanece mTLS/papacc/1. Nenhum listener atual recebe a exceção silenciosamente. Sem especialização accepted, INITIAL_TRANSPORT=DENY.

## T1 — Autenticação e primeiro contato candidatos

Antes de liberar password, código ou CSR, Browser deve possuir confirmação administrativa independente/autenticada, vinculada à tentativa e à identidade esperada do Accelerator: ExpectedServerName e fingerprint SHA256 dos bytes DER do certificado servidor aprovado; CA/chain públicas necessárias devem ser obtidas com autenticidade independente, nunca confiadas só porque chegaram do peer. Confirmar fingerprint da CA/identidade do certificado quando utilizada como trust material. A confirmação do código não substitui essa verificação. Nenhuma CA externa obrigatória, CA nova, certificado fictício ou silent TOFU.

Usar o trust customizado do PST por tentativa, Expected Peer Name/SAN, SNI aprovado, serverAuth e validade/UTC confiável; provar propriedade da chave pelo handshake. Rejeitar divergência/nome ausente/cadeia inválida/validade/status não demonstrado, conforme contrato a ser qualificado. Material/quadro de confirmação é público mas não ganha autoridade por arquivo/hash. Não desativar chain/name verification para comparar fingerprint depois. Administração/root fornece identidade pública aprovada; Browser decide trust local específico, sem PapinhoTrustStore, sem assumir APIs de importação ainda inexistentes.

TLS1.3 obrigatório, sem0-RTT/resumption/downgrade, sem fallback para outro provider pós-falha. CLIENT e SERVER precisam de capacidades factuais elegíveis; peer-auth no Browser exige certificado servidor, servidor apresenta Local Identity própria. Ausência de certificado do cliente somente é candidata nesse propósito restrito, nunca em conexões operacionais.

## T2 — Isolamento, sem protocolo inferido

Conexão de enrollment jamais vira CONTROL Session, DATA association, Network Egress ou capability normal. Decisão server-side não depende de flags/strings/ALPN recebidos sem um discriminator aprovado antes de escolher política de handshake. Reutilizar owner/listener/worker/root existentes apenas se houver forma factual e normativamente aceita de discriminação. Não criar segundo listener ou servidor HTTP para suprir ausência de rota.

PENDENTE: identificar regra de seleção/demultiplexação, perfil de aplicação, mensagens e framing compatíveis com protocolo já aprovado. Nada encontrado autoriza enviar código/password/CSR no PACC atual. Não alocar ALPN, opcode, URL, porta, pacote, STARTTLS ou protocolo novo nesta proposta. Se não houver extensão existente aceita, pedir decisão explícita de escopo/protocolo em vez de implementar. T0–T7 não autoriza esse desenvolvimento. Site TLS é outbound e não é rota de bootstrap; não habilitar egress para matricular cliente.

## T3 — Cerimônia do código candidata, para revisão

Direção mínima a decidir explicitamente: servidor gera o código P1 dentro da preparação administrativa; somente operador administrativo autorizado recebe sua apresentação protegida. Esse operador entrega o código ao destinatário pretendido por canal independente autenticado, junto à identificação inequívoca da solicitação/servidor. Destinatário verifica T1, confirma a solicitação e submete o código ao mesmo contexto pelo canal protegido restrito. Não enviar o código ao Browser solicitante pelo mesmo canal e chamá-lo de prova independente; não logar, auto-confirmar ou usar senha como identidade. Código comprova apenas confirmação contextual, não identidade civil, usuário, grants ou posse de chave.

Quem/como apresenta e recebe o segredo são escolhas candidatas desta T3; não são fatos já aprovados. Nenhum encoding de rede é definido. Sem operador/canal independente factual, DENY. P1 fornece formato local; T2 precisa existir antes de implementar submissão.

## T4 — Continuidade e CSR posteriores

O código/confirmação fica vinculado a uma única conexão TLS estabelecida, owner/operação/geração e contexto P2, pelo lifetime real de objetos já existentes. Não basta repetir request nonce público, ID, digest ou IP em outra conexão. A mesma conexão deve transportar todas as etapas e o CSR posterior; queda/invalidação/Stop não permite retomar pela apresentação do código antigo. Nenhum token bearer ou protocolo de retomada é criado. Não inventar TLS exporter/transcript hash próprio.

Após pareamento verificado e autorizado (incluindo OPEN/CLOSED e aprovação), Browser gera/mantém chave local e envia somente CSR/PoP público. Owner valida perfil/PoP e associa CSR/SPKI imutáveis à solicitação aprovada, ainda sob a mesma continuidade. Troca de bytes, nonce, listener/security_ref/identidade servidor ou policy/CURRENT antes do handoff invalida; depois do handoff correlacionar os commits próprios do issuer conforme P6. Conexão autenticada do servidor não autentica usuário/cliente e não concede emissão.

Perda após submissão/efeito possivelmente durável: consulta/reconciliação interna pelo issuer exato quando autorizada, não retry pela rede, nova reserva ou assinatura. Como nenhuma mensagem de consulta está definida, cliente mantém resultado desconhecido e DENY; operador não apaga histórico para tentar de novo.

## T5 — Correção candidata de P6 sem alterar original

Primeiro código correto faz transição única PREPARED_CONTEXT→CONFIRMED com claim irrevogável para owner/request/conexão. De CONFIRMED em diante rejeitar toda nova submissão antes de comparação/efeitos, inclusive quando aguarda autenticação/aprovação. Claim não é APPROVAL_GRANTED nem consumo para emissão. Recusa, falha, expiry ou Stop não reabrem código ou saldo. A marca PAIRING_CONSUMED posterior conserva esse claim e autoriza somente o próximo teste do workflow, não assinatura. Resultado incerto retém e não reverte.

AUTO_APPROVAL precisa de policy CURRENT factual e todas as verificações; MANUAL_APPROVAL precisa de ator/decisão vinculados. CLOSED exige username/password governado antes de ambas; OPEN dispensa só esse login inicial. Provider atual não comprovado mantém CLOSED DENY. Decisão de emissão/serial/reserva é independente no issuer. Nenhum parâmetro P1/P3/P4 é aceito automaticamente por T5.

## T6 — Recursos, deadlines e Model E

Antes de qualquer ativação, aceitar e qualificar caps/deadlines para accepts e handshakes pré-código, concorrência de conexões/contextos/approvals/issuer, bytes por fase, CSR e saídas, taxa de tentativas antes de trabalho caro. Estes valores ainda NÃO ESTÃO FECHADOS. P3/P4 continuam parâmetros candidatos locais;5min do código não limita handshake prévio, nem10submissões/min limita sockets anônimos. Sem limites concretos/ED11, DENY.

Não criar Session operacional para manter estado de enrollment. Owner existente conserva conexão/geração, trust/credential leases, configuração/pins, aprovações, buffers e resultados retidos. Remove membership e realiza release factual Model E; nenhuma finalização por Stop/timeout/saída de processo. Nunca compartilhar workspace/slot retido.3MiB apenas Windows explicitamente selecionado, quotas individuais e legacy2MiB intactos; provider/kernel UNKNOWN, total PARTIAL.

## T7 — Registro e gates restantes

T0–T7 e correção de P6 são PROPOSED, sujeitos a revisão/aceitação específica do owner e executor. Nenhuma fonte funcional, API, wire, codec, identidade, epoch, registry, CA ou listener é criado por esse texto. A revisão de P0–P9 deve permanecer pendente enquanto segurança depender de T1/T3/T4/T5 e transporte T2/limites T6 ainda abertos. Não registrar aceitação integral por silêncio.

INITIAL_TRANSPORT=TLS13_SERVER_AUTH_CANDIDATE_DENIED_PENDING_PRODUCT_CONTRACT
PAIRING_CODE_CONTRACT=PROPOSED
FIRST_CONTACT_TRUST=INDEPENDENT_AUTHENTICATED_CONFIRMATION_PROPOSED
AUTOMATIC_ENROLLMENT=DENY
IMPLEMENTATION_AUTHORIZED=NO
