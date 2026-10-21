# Prova delimitada e handoff

## Estado factual

HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a, main. O patch de oito arquivos foi submetido ao executor e aprovado antes da aplicação; source-operations.json registra preimagens e hashes. baseline-verification.json verifica 645 arquivos anteriores: somente CMakeLists.txt mudou, por acréscimo aprovado. Nenhum formato persistente, fonte de produção, Browser, PST ou wire operacional mudou.

## Implementação por função e lifetime

- ep_frame_encode/decode: PAE1/v1.0, oito tipos, direções, tamanhos, UTF-8 e campos reservados. Decode retorna uma view emprestada do buffer do caller, sem autoridade.
- ep_reader_push: montagem incremental limitada a 16400 bytes, deadline absoluto de frame 5s, sem pipeline oculto. Caller mantém RX estável enquanto houver view; reader pronto não pode ser reutilizado. Stop invalida sem liberar storage.
- ep_trace_observe: sequência/correlação, até oito frames e 32768 bytes por direção; OPEN/CLOSED e AUTO/MANUAL representam somente estados declarados. APPROVED observado não comprova autorização. CSR é copiado para 4096 bytes próprios, mantidos em Stop; nenhuma emissão é chamada. CSR shape não comprova assinatura/PoP.
- ep_pair_prepare_data/claim_data: dados próprios, código Crockford de 50 bits, TTL 300s, colisões negadas, comparação constante existente, uma submissão/claim por conexão/geração local. Não equivale a PAIRING_CONSUMED, policy, Principal ou autorização. Owner futuro mantém contextos inválidos/retidos e limpa segredos somente após quiescência factual.
- ep_pair_generate_windows: BCryptGenRandom para código/correlação/nonce, scratch 55 bytes limpo com SecureZeroMemory. Prova nativa restrita ao RNG, sem chave ou certificado.
- ep_limiter_admit: seis admissões globais/minuto, três por origem/minuto, dezesseis entradas, dois lifetimes inclusive retidos. ep_origin_set normaliza IPv4-mapped. Caller deve usar origem canônica da PAL, nunca headers remotos. ep_limiter_release_data contabiliza ticket local após fechamento factual pelo owner futuro; não fecha handles nem devolve créditos nativos. Stop não libera tickets.
- ep_tls_profile_prepare: projeção imutável de configuração PST com credencial emprestada, TLS1.3/server-only/papacc-enroll/1. Caller mantém credencial, lease e endereço do perfil durante todo uso/retenção. Nenhuma chamada PST ou listener foi ativado. Teste do builder operacional mantém certificado de cliente REQUIRED e papacc/1.
- ep_enrollment_quote: soma conservadora, UNKNOWN nega; não reserva créditos ou qualifica memória nativa.

## Limites da entrega e gates

O propósito ENROLLMENT ainda não tem representação persistente versionada aceita no modelo/root. Não usar enum secure/legacy nem bytes reservados. Runtime singular não pode truncar listeners[]. Ligação produtiva fica DENY. Fonte factual de CURRENT, identidade do servidor e autorização administrativa não estão disponíveis; provider CLOSED permanece PENDING. Não se cria autoridade sintética para suprir essas fontes.

Ainda faltam limites globais P4 de geração 2/min e submissão 10/min, aplicação dos deadlines de conexão/handshake/primeira mensagem/escrita/vida total pelo owner nativo, continuidade factual de aprovação/consumo/CSR até issuer e aquisição supervisionada dos créditos. O limiter atual implementa somente T6 pré-TLS. Não registrar cobertura integral de P0–P9 ou T0–T7.

## Validação

Quatro configurações: Selected e Legacy, Debug e Release, MSVC /W4 /WX, dez suítes por configuração, zero warnings. Logs preservados e validation-results.json. São 1369 assertions DATA_MODEL e 13 PROFILE_DATA por configuração; CSR/frames/approval são fixtures sintéticas sem autoridade. BCrypt RNG é a única evidência nativa nova. Não há handshake, socket, rede, assinatura privada ou prova positiva CURRENT. NT4/VC6, box e ISA não foram qualificados.

## ED11

ed11-census.json separa tamanhos compilados de reserva operacional. Delta fixo no servidor zero: bibliotecas opt-in não ligadas a papacc_server. Exemplo de dois conjuntos reader/trace/pair/perfil, RX/TX e limiter: 76280 bytes; RNG scratch 55 bytes. Essa soma NÃO inclui toda stack transitiva, buffers de encode/decode adicionais, PST/handshake, issuer, retenção criptográfica e custos opacos. Não é pico nem reserva aprovada.

Subteto enrollment 262144 permanece dentro do limite selecionado. Cenário normativo base Windows 2103847 + subteto = 2365991, margem nominal 779737; Legacy 2067655 + subteto = 2329799, excede 2097152 em 232647: ativação negada. UNKNOWN de provider/kernel e composição de ativação impedem qualificação total. Quotas individuais e seleção explícita Windows3MiB preservadas.

## Handoff Browser e integração

Browser mantém chave automática exclusivamente local. Confirmar identidade do servidor por canal independente autenticado antes dos segredos; código não autentica servidor. Usar endpoint ENROLLMENT separado, TLS1.3 e ALPN exato; rejeitar downgrade/papacc/1 e mensagens operacionais. Mesma conexão/owner/geração/request/nonces até CSR; sem resume por token. CLOSED exige provider factual; código, autenticação, aprovação e emissão são gates distintos. Cancelamento/Stop não provam cancelamento de I/O ou liberação de recursos. Não modificar Browser nesta entrega.

Próximo patch depende primeiro do contrato persistente do propósito, owner/runtime de coleção e censo completo. Qualificação positiva externa permanece necessária. CURRENT, assinatura, recuperação e admissão continuam DENY.
