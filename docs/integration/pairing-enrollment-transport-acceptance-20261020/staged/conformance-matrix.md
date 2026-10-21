# Revisão conjunta e matriz de conformidade

## Escopo

Revisão normativa de P0–P9 e T0–T7 Revision 2, sem implementação ou qualificação operacional. HEAD main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a; os 686 hashes do manifesto anterior foram verificados. Propostas e ADRs accepted anteriores permanecem intactos. O registro novo depende de aprovação oficial de seus hunks. Não foi identificado conflito material remanescente, considerando a precedência delimitada no registro.

## Pareamento e integração

| Requisito | Cláusulas | Conclusão |
|---|---|---|
| Formato e entropia | P1 | 10 símbolos ASCII de alfabeto com 32 entradas; 50 bits uniformes obtidos de 7 bytes CSPRNG, descartando 6 bits. Nonce de 32 bytes e correlação de 16 bytes. Entrada local admite hífen; wire usa exatamente 10 bytes. |
| Validade | P3/T6.1 | 300000 ms absolutos, relógio monotônico e aritmética verificada. Progresso e remaining_ms não renovam autoridade. Aplica-se o menor prazo vigente. |
| Tentativas e rates | P4/T2.5/T6 | Teto geral de cinco; perfil inicial exige uma CONFIRM e erro terminal. Código: duas criações e dez submissões por 60 s. Pré-handshake: seis tentativas globais e três por origem por 60 s. Capacidades incluem retidos. |
| Claim e replay | P6/T5 | Claim irrevogável no primeiro acerto, antes da aprovação; nenhuma segunda comparação durante espera. Consumo posterior e submissão ao issuer são etapas distintas. |
| Identidade do servidor | P0/P2/T1/T2 | Confirmação independente, CA pública autenticada, chain, nome/SAN, fingerprint e handshake antes de BEGIN, código, senha ou CSR. Código não autentica servidor; silent TOFU é proibido. |
| OPEN/CLOSED | P5/T5/T2.4 | OPEN dispensa somente login inicial. CLOSED exige provider governado e password sob TLS antes da aprovação. Provider ausente mantém DENY. |
| AUTO/MANUAL | P5/T3/T5 | Policy CURRENT ou ator/decisão administrativa factual, vinculados à solicitação exata. Nenhuma flag recebida cria aprovação. |
| Custódia e CSR | P2/P6/T4/T2.5 | Chave automática exclusivamente no Browser. CSR público após pareamento autorizado, mesma conexão/owner/request, DER/SPKI e PoP exatos. Nada concede assinatura ou Principal por decode. |
| Continuidade | P2/P6/T4 | Não há retomada por nonce público, IP ou token novo. Commits próprios do issuer são correlacionados por predecessor/CAS e autoridade CURRENT, sem pin histórico conceder novo efeito. |
| Emissão | P8/T2.5 | Reserva durável, assinatura e publicação factuais permanecem no issuer. CREDENTIALS não pode decorrer de SIGN_REQUESTED, write parcial ou CAS_UNKNOWN. |
| Stop e retenção | P8/T6.3/Model E | Nenhum cancelamento ou DESTROYED presumido. Referências, leases, buffers, resultados e créditos permanecem até quiescência/fechamento factuais. |
| Revogação/renovação | P8/issuer | Histórico, novos seriais e autorização preservados; política sobre Sessions ativas continua pendente. Não são criadas mensagens de renovação ou revogação. |

## Layout e estados

| Item | Conferência documental |
|---|---|
| Endpoint | Listener separado L0–L5 accepted, perfil pré-publicação imutável e coleção/root únicas. Capacidade insuficiente nega, sem truncamento. |
| Operacional | TLS 1.3/mTLS/papacc/1 preservados. Não selecionar auth pelo ALPN oferecido nem pela ausência de certificado. |
| ALPN | papacc-enroll/1, 15 bytes, hex 7061706163632d656e726f6c6c2f31. Configuração local com único item e REQUIRED; validar seleção negociada sem parser ClientHello. |
| Header | 16 bytes: magic 0–3, major 4, minor 5, kind 6, flags 7, payload length 8–11, sequence 12–15. Inteiros BE, sem sobreposição ou padding indefinido. |
| BEGIN | 32 + 32 + 2 + N = 66 + N; N de 1 a 253, máximo 319 bytes. |
| CONTEXT | 16 + 32 + 32 + 32 + 1 + 1 + 2 + 4 = 120 bytes. Offsets 0/16/48/80/112/113/114/116. |
| CONFIRM | 16 + 32 + 10 + 2 + 2 + U + P = 62 + U + P; máximo 318 bytes. OPEN tem 62; CLOSED tem 64–318. UTF-8 estrito e sem normalização. |
| STATUS | 16 + 1 + 3 = 20 bytes. WAIT opcional → APPROVED → PENDING opcional, uma ocorrência de cada e apenas S→C. |
| CSR | 16 + 32 + 4 + N = 52 + N; máximo 4148 bytes e um único CSR. |
| CREDENTIALS | 16 + 4 + 4 + L + C = 24 + L + C; máximo 8216 bytes. Uma leaf CLIENT e uma CA direta, sem chave privada ou grants. |
| CANCEL | 16 + 32 = 48 bytes, id e nonce correspondentes. |
| ERROR | 16 + 2 + 2 = 20 bytes, reasons 1–7; id zero somente antes de CONTEXT, um terminal sem texto arbitrário. |
| Parser | Sequence 1–8 por direção, oito frames e 32768 bytes por direção; body até 16384 e tamanho específico obrigatório. Rejeitar versão, direção, flags, tamanho, estado, trailing bytes, pipeline ou replay inválidos. |
| Fechamento | ERROR/CANCEL/CREDENTIALS são terminais. EOF, truncamento ou UNKNOWN não comprovam sucesso. Não há query, retry, fallback ou promoção para operação normal. |

O maior body definido pelos oito kinds tem 8216 bytes; 16384 é teto defensivo, não permissão para bytes extras. Fluxos válidos cabem em oito frames e 32768 bytes por direção, sem fragmentação de aplicação ou polling. Fragmentação/coalescência de TLS não altera as fronteiras: processar um frame autorizado por vez.

## Composição e autoridades

A decisão de provisionamento de 18/10 exige os dois modos, OPEN/CLOSED, aprovação e hot updates. A autorização atual aceita escolhas concretas antes abertas. Custódia cláusula 5 mantém CSR posterior ao pareamento e chave local. ADR-0028 separa Transport/AuthN/Admission/AuthZ; trust do servidor não vira Principal ou grant. ADR-0009 especializado L0–L5 autoriza endpoint separado, sem relaxar mTLS operacional. PST é mecanismo de TLS/peer info/ALPN/nonblocking/Model E, não fonte de policy ou confirmação independente.

T2/T6 fecha o transporte antes aberto em P7, somente no ENROLLMENT. T5 esclarece o claim antes de aprovação, sem substituir o consumo posterior de P6. Uma CONFIRM restringe especificamente o teto de P4. Expressões históricas de status nas fontes não invalidam a aceitação expressa; pré-condições factuais e gates de DENY permanecem. Não são aceitos por referência enums/codecs persistentes, directives, worker, runtime, fontes positivas, Browser ou patches funcionais.

## ED11 e portabilidade

Os tetos 262144 bytes por processo de enrollment e 98304 por lifetime são restrições, não reservas ou exceções ao orçamento existente. São duas conexões incluindo retidos e apenas um trabalho CSR/issuer de enrollment por vez. RX/TX de 16400 bytes cada, timestamps, backing, stack transitiva, provider, callbacks, issuer, root/pins e host headroom precisam de censo completo. Nenhuma Session operacional temporária é criada.

Windows 3 MiB somente explícito; Legacy 2 MiB intacto. Base selected 2103847 + 262144 = 2365991, margem aritmética 779737. Legacy 2067655 + 262144 = 2329799, excesso 232647: DENY nessa composição. Provider/kernel UNKNOWN, ED11 total PARTIAL. Não qualificar pelo cálculo.

Windows, box, ISA, NT4/VC6 e PST exigem capacidades, entropia, relógio, trust, TLS, ALPN e lifecycle factuais e execução real. Nenhum target é declarado compatível por documentação. Não houve build, teste, rede ou geração de material nesta tarefa.

## Gates restantes

Implementação continua NÃO autorizada. Uma tarefa futura precisa de hunks, preimagens e revisão oficial. Representação/codec versionados para ENROLLMENT e capacidade real de coleção/runtime ainda precisam contratos próprios, sem campos reservados ou aliases. CURRENT, admin positivo, policy/identidade/issuer/pins e provider CLOSED ausentes mantêm DENY. Censo operacional e qualificação de targets continuam abertos. Esses gates são limites explícitos da aceitação, não divergência normativa dentro dos intervalos revisados.
