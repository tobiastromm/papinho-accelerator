# Handoff após aceitação normativa

Autoridade designada: docs/adr/DECISION-pairing-enrollment-transport-20261020.md. O registro incorpora os intervalos exatos com precedência; as propostas históricas preservadas não são uma segunda autoridade. Arquitetura separada L0–L5 continua accepted. Aceitação não comprova implementação e não autoriza código nesta tarefa.

Próximo slice sugerido para futura autorização: biblioteca portável privada de framing PAE1/1.0, campos byte-exatos, UTF-8, comprimentos verificados, direção/estado e lógica local de código/claim/relógio/limiter. Sem sockets, CA, provider fictício ou CURRENT sintético com autoridade. Preparar patch exato, medir estruturas, capacidades e stack, e separar fixtures de controle de evidência criptográfica ou factual nativa.

Antes de persistir ou ativar ENROLLMENT, obter extensão versionada aceita do modelo/codec. Não usar enum SECURE_PRINCIPAL/LEGACY, bytes reservados ou alterar BODY0.5/ROOT06/D3/LST1/ROOT08 por inferência. Reutilizar coleção, participantes, worker, root e CAS; capacidade insuficiente nega em vez de truncar a coleção. Essa dependência não impede um futuro slice local independente expressamente autorizado, mas bloqueia ativação.

Resolver fontes factuais de listener/security_ref/ExpectedServerName/trust/issuer/policy/CURRENT sob pins e lifetime. Ausência mantém DENY; não publicar ou inicializar CURRENT para demonstrar sucesso. CLOSED continua exigindo provider governado de usuário/senha, não certificado, código ou boolean. Confirmação independente e autorização local do Browser precisam de fronteiras reais. CSR posterior é público; nenhuma chave privada automática pertence ao Accelerator.

Provar capacidade do owner/scheduler/PST para dois lifetimes incluindo retidos, seleção anterior ao handshake e capacidades de cada role/backend. Aplicar o ALPN de enrollment somente no novo endpoint. Não modificar PST nem papacc/1. Sublimites não são pools ou reservas: custo transitivo, coexistência e recursos opacos precisam de censo. Legacy mantém DENY sob o adicional conhecido; Windows não é qualificado por soma aritmética.

Preservar créditos e workspaces em resultados incertos. Deadline/Stop não são cancelamento ou DESTROYED. Reuso depende de quiescência factual, exatamente um close/refund e ausência de borrowers. Reserva/assinatura/publicação pertencem ao issuer existente; sem autoridade, checkpoint/fencing ou admin exigidos, DENY. Nenhuma rede, assinatura, CA, certificado, importação Browser ou release está autorizada.

Provas futuras: oito mensagens/offsets, limites, versão/flags/sequence/trailing/estado/direção; charset/CSPRNG/TTL e falhas de código; uma CONFIRM e claim sem replay; OPEN/CLOSED e AUTO/MANUAL ausentes negados; CSR precoce/trocado e canal/nonce/request divergentes; Stop, timers, rates, pressão de dois retidos e ED11; nenhum grant implícito. Nenhum desses testes foi executado nesta aceitação.
