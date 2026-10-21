---
status: proposed
revision: 1
task: PACC-PHASE6-AUTOMATIC-ENROLLMENT-READINESS-1
scope: pairing-code-only; no network protocol or authority activation
---

# Proposta focal Ãºnica â€” cÃ³digo de pareamento automÃ¡tico

Este texto P0â€“P9 Ã© candidato normativo para revisÃ£o oficial, nÃ£o ACCEPTED. O owner autorizou preparar parÃ¢metros exatos apÃ³s confirmar que nÃ£o hÃ¡ contrato accepted adicional. NÃ£o altera ADR-0006, ADR-0028, provisionamento de18/10, CA/profile1 ou qualquer wire. ImplementaÃ§Ã£o dependente permanece proibida atÃ© aceitaÃ§Ã£o especÃ­fica. NÃ£o aceita um novo transporte por referÃªncia.

## P0 â€” Alcance e fronteiras

O cÃ³digo Ã© confirmaÃ§Ã£o contextual de pareamento, nunca identidade do servidor, senha do usuÃ¡rio, aprovaÃ§Ã£o administrativa, prova de posse, autorizaÃ§Ã£o de emissÃ£o, Principal ou grant. Quatro decisÃµes distintas sÃ£o exigidas: identidade/trust do servidor verificados; autenticaÃ§Ã£o inicial conforme OPEN/CLOSED; decisÃ£o AUTO_APPROVAL/MANUAL_APPROVAL na policy administrativa vigente; autorizaÃ§Ã£o factual do issuer para a solicitaÃ§Ã£o exata. Nenhuma etapa dispensa as outras. ConfirmaÃ§Ã£o humana deve distinguir destinatÃ¡rio/servidor e solicitaÃ§Ã£o; um cÃ³digo retransmitido nÃ£o impede phishing/relay por si sÃ³.

MANUAL .papacc permanece independente. ExceÃ§Ã£o manual de chave nÃ£o se aplica: chave privada automÃ¡tica Ã© gerada e mantida exclusivamente no cliente; Accelerator recebe somente CSR pÃºblico/prova de posse apÃ³s pareamento verificado. Nada exige geraÃ§Ã£o de fixtures .papacc.

## P1 â€” Formato e entropia candidatos

CÃ³digo canÃ´nico: exatamente10 bytes ASCII do alfabeto ordenado `0123456789ABCDEFGHJKMNPQRSTVWXYZ` (32 sÃ­mbolos), total50bits uniformes. ApresentaÃ§Ã£o local opcional:5 sÃ­mbolos, hÃ­fen ASCII0x2D,5 sÃ­mbolos. Aceitar como entrada local somente os10 bytes canÃ´nicos ou11 bytes com Ãºnico hÃ­fen no offset5; remover somente esse hÃ­fen. Rejeitar minÃºsculas, aliases I/L/O/U, espaÃ§os, Unicode, terminadores embutidos, trailing bytes e qualquer outro tamanho; nÃ£o normalizar por locale. Strings de exibiÃ§Ã£o nÃ£o sÃ£o formato de rede.

Gerar7 bytes novos por CSPRNG qualificado, ler os primeiros50bits em ordem MSB-first e mapear10 grupos consecutivos de5bits aos Ã­ndices0..31. Ignorar os6bits finais; nÃ£o aplicar mÃ³dulo decimal, hash de hostname, relÃ³gio, contador, GUID ou fallback pseudoaleatÃ³rio. Cada geraÃ§Ã£o usa nova amostra; falha da fonte nega, sem segundo provider implÃ­cito. ComparaÃ§Ã£o do cÃ³digo canÃ´nico usa mecanismo constante no tempo revisado, sem primitiva criptogrÃ¡fica prÃ³pria. A representaÃ§Ã£o nÃ£o autentica a conexÃ£o.

Gerar ainda nonce servidor32bytes e correlaÃ§Ã£o administrativa16bytes, independentes do cÃ³digo. Nonce cliente32bytes vem do CSPRNG local do Browser; nenhum segredo privado Ã© transferido. ColisÃ£o de correlaÃ§Ã£o/nonce com contexto ainda vivo ou retido nega a tentativa; nÃ£o sobrescrever nem reusar slot. SeleÃ§Ã£o/revisÃ£o de um adapter de entropia Ã© prÃ©-condiÃ§Ã£o factual, nÃ£o autorizaÃ§Ã£o para usar tokens C2 como cÃ³digo. APIs CSPRNG do Windows jÃ¡ usadas no projeto sÃ£o mecanismos candidatos de plataforma, nÃ£o fonte de aprovaÃ§Ã£o.

## P2 â€” Contexto imutÃ¡vel obrigatÃ³rio

O owner administrativo existente deve possuir uma Ãºnica preparaÃ§Ã£o imutÃ¡vel vinculando: root identity; CURRENT/R/A/P/I factuais e pin/lifetime; listener/security_ref/perfil TLS e ExpectedServerName aprovado; fingerprint SHA256 do certificado de servidor efetivamente verificado; issuer/geraÃ§Ã£o aplicÃ¡veis; correlaÃ§Ã£o administrativa; nonce cliente; nonce servidor; identidade da solicitação pública (nonce cliente já indicado, nunca um segredo privado); identidade de dispositivo proposta e sua aprovaÃ§Ã£o administrativa separada; modo OPEN/CLOSED; modo AUTO/MANUAL; identidade/revisÃ£o da policy; inÃ­cio/deadline monotÃ´nicos; contadores e estado de consumo.

Esses sÃ£o requisitos de comparaÃ§Ã£o/lifetime, nÃ£o novos campos persistentes aceitos ou um transcript criptogrÃ¡fico de rede. NÃ£o se define concatenaÃ§Ã£o assinada/hash/MAC novo. CSR subject, IP, nome de apresentaÃ§Ã£o, cÃ³digo ou request ID nÃ£o viram identidade estÃ¡vel/Principal. Request de emissÃ£o somente utiliza identidade aprovada na mesma root; se a fonte nÃ£o existe, DENY. Nenhum CURRENT, epoch, identidade ou pin vem de flags do solicitante ou de decode.

Ao verificar cÃ³digo/aprovar/consumir/autorizar reserva, comparar o contexto inteiro com as fontes factuais sob lifetime protegido. Qualquer troca de nonce, destino, certificado, listener, identidade, policy ou CURRENT/R/A/P/I invalida a tentativa; nÃ£o atualizar contexto em lugar. Mesmo issuer-only que preserve P nÃ£o preserva automaticamente a autorizaÃ§Ã£o do pareamento. Nova tentativa exige nova confirmaÃ§Ã£o e novo cÃ³digo, sem afetar Sessions antigas nem seus snapshots.

## P3 â€” Validade e relÃ³gios

Prazo do contexto inteiro:300000ms, absoluto, a partir da conclusÃ£o factual da geraÃ§Ã£o local sob owner; inclui espera de confirmaÃ§Ã£o, autenticaÃ§Ã£o e aprovaÃ§Ã£o manual. VÃ¡lido somente se `start <= now < deadline`, com soma e diferenÃ§a verificadas. Igualdade ao deadline Ã© expirada. RelÃ³gio regressivo, invÃ¡lido, indisponÃ­vel ou overflow: DENY. NÃ£o prolongar ao errar cÃ³digo, reconectar, consultar ou alternar aprovaÃ§Ã£o. Nenhuma duraÃ§Ã£o Ã© inferida de timestamps de peers.

MonotÃ´nico local rege cÃ³digo e limites; UTC confiÃ¡vel rege certificados/status, sem desabilitar validade. Contexto nÃ£o sobrevive a restart. NÃ£o reconstruir saldo, cÃ³digos, aprovaÃ§Ã£o ou autoridade com arquivo ausente, relÃ³gio remoto ou digest. Nova operaÃ§Ã£o deve cumprir de novo todas as provas; incerteza de emissÃ£o jÃ¡ iniciada continua retida no issuer, nÃ£o vira novo request automÃ¡tico.

## P4 â€” Tentativas e limites candidatos

No mÃ¡ximo5 submissÃµes de cÃ³digo por contexto. Toda submissÃ£o vinculada a contexto existente consome uma tentativa ANTES da comparaÃ§Ã£o, inclusive formato invÃ¡lido ou erro de backend; uma tentativa correta tambÃ©m consome. O quinto erro encerra o contexto como DENIED; bloqueado/expirado/consumido nunca compara novamente. Falha de confianÃ§a/autenticaÃ§Ã£o nÃ£o libera tentativas.

Por processo/owner administrativo, no mÃ¡ximo2 contextos simultaneamente vivos OU retidos; nenhuma parcela Ã© por cliente e nÃ£o hÃ¡ novo pool/worker. Contexto retido ocupa capacidade atÃ© quiescÃªncia factual, mesmo apÃ³s expiraÃ§Ã£o. NÃ£o apropriar slots, reservas ou tokens de Sessions/C2 para contornar quotas.

Limite global de criaÃ§Ã£o:2 tentativas em qualquer janela mÃ³vel de60000ms. Limite global de submissÃ£o:10 tentativas em qualquer janela mÃ³vel de60000ms. Registrar evento admitido antes de geraÃ§Ã£o/comparaÃ§Ã£o; eventos com idade exatamente60000ms deixam a janela. Rejeitar ao atingir limite, sem dormir/bloquear Stop, gerar segredo, comparar cÃ³digo ou abrir nova operaÃ§Ã£o. SubmissÃ£o cujo contexto Ã© inexistente tambÃ©m cobra a janela global quando admitida, mas nÃ£o cria registro individual. ExposiÃ§Ã£o externa de erros nÃ£o pode revelar se cÃ³digo, usuÃ¡rio ou identidade existem; diagnÃ³stico interno sem secrets identifica a fase.

AnÃ©is bounded de2 e10 timestamps sÃ£o suficientes para as janelas locais, alÃ©m dos contadores por contexto; contabilizar estrutura/alinhamento antes de admitir. NÃ£o sÃ£o catÃ¡logo de clientes ou persistÃªncia de trust. Alcance explicitamente por processo contÃ­nuo: nÃ£o prova limitaÃ§Ã£o global entre instalaÃ§Ãµes/restauraÃ§Ãµes/processos. Antes de qualquer ativaÃ§Ã£o com mÃºltiplos processos, comprovar exclusividade administrativa na mesma root ou manter DENY. Restart cancela todos os contextos, nÃ£o recupera pareamento; nÃ£o Ã© garantia contra DoS nem substitui polÃ­tica de acesso. Nenhum limite aceito de Session/Connection/C2 Ã© aumentado por estes nÃºmeros propostos.

## P5 â€” OPEN/CLOSED e aprovaÃ§Ã£o

OPEN dispensa username/password inicial somente para solicitar enrollment. CLOSED exige resultado factual de validaÃ§Ã£o username/password por mecanismo governado, dentro de canal aprovado, antes de AUTO_APPROVAL ou MANUAL_APPROVAL. NÃ£o registrar senha, inventar provider, usar certificado ou cÃ³digo como substituto da senha CLOSED. Provider nomeado atual PENDING => CLOSED DENY.

AUTO_APPROVAL exige decisÃ£o explÃ­cita da policy CURRENT para a solicitaÃ§Ã£o e todas as provas de pareamento/AuthN; a autorização posterior para emissão exige ainda CSR/PoP e sua correlação; nÃ£o Ã© flag enviada pelo cliente. MANUAL_APPROVAL exige ator administrativo autenticado/autorizado, aÃ§Ã£o explÃ­cita e vÃ­nculo Ã  preparaÃ§Ã£o imutÃ¡vel exata. A troca de modo nÃ£o reaproveita tentativa aprovada. AusÃªncia, recusa, revogaÃ§Ã£o, ambiguidade ou revisÃ£o obsoleta negam. Pending nÃ£o recebe CONTROL Session, DATA, capabilities, egress ou permissÃµes administrativas. Aprovar pareamento nÃ£o emite certificado nem concede grants.

## P6 â€” Consumo Ãºnico e resultado incerto

Estados lÃ³gicos candidatos, nÃ£o novo codec/API: PREPARED_CONTEXT â†’ CONFIRMED â†’ ACCESS_AUTHENTICATED_OR_OPEN â†’ APPROVAL_GRANTED â†’ CONSUMED_FOR_EXACT_REQUEST. Cada transiÃ§Ã£o depende da prova factual correspondente; negativas sÃ£o DENIED/EXPIRED/STOPPED, incerteza RETAINED_UNKNOWN. Esses estados nÃ£o sÃ£o estados de emissÃ£o do issuer.

Owner serializa e marca consumo irrevogÃ¡vel ANTES de submeter a mesma correlaÃ§Ã£o de request Ã  transaÃ§Ã£o canÃ´nica; a marca nÃ£o concede autoridade. Somente a confirmaÃ§Ã£o factual na root da mesma solicitaÃ§Ã£o pode avanÃ§ar preparaÃ§Ã£o/reserva de serial conforme contratos issuer. Se houver reentrÃ¢ncia, concorrÃªncia ou resultado de consumo/commit incerto, reter contexto e DENY novos efeitos, sem restaurar saldo/cÃ³digo, fallback ou reemitir. Uma marca volÃ¡til nÃ£o prova commit durÃ¡vel; antes de restart nenhum contexto Ã© recuperÃ¡vel, e o issuer mantÃ©m serial/record/high-water/reconciliaÃ§Ã£o prÃ³prios.

ApÃ³s consumo, uma consulta somente observa a correlaÃ§Ã£o existente; replay da confirmaÃ§Ã£o/cÃ³digo nunca chama emissÃ£o/reserva outra vez, mesmo com bytes idÃªnticos. NÃ£o hÃ¡ mensagem de consulta definida aqui. CAS_CONFLICT e CAS_UNKNOWN nÃ£o sÃ£o sucesso nem autorizaÃ§Ã£o de retry destrutivo; nenhuma tentativa cria novo request automaticamente. AtÃ© haver binding concreto aprovado de consumo/owner/root, o caminho permanece DENY.

## P7 â€” Transporte inicial explicitamente nÃ£o selecionado

Reutilizar apenas o listener e capacidades aceitas. O Secure Principal operacional continua TLS1.3/mTLS/ALPN papacc/1, sem0-RTT/resumption/downgrade. Um Browser novo sem credencial NÃƒO pode fazer esse handshake por uma exceÃ§Ã£o implÃ­cita. OPEN nÃ£o torna client certificate opcional e nÃ£o cria endpoint HTTP/server-auth/plaintext. D3 de pacote manual nÃ£o Ã© protocolo de bootstrap automÃ¡tico. C2 PAIR_OPEN e seus tokens sÃ£o associaÃ§Ã£o de offload pÃ³s-Session, nÃ£o enrollment.

Nenhuma mensagem PACC, extensÃ£o TLS, ALPN, porta, ID, perfil ou exchange de cÃ³digo/CSR Ã© definido/implementado por P0â€“P9. APIs existentes nÃ£o fornecem ainda transporte anterior Ã  primeira credencial; esse caminho fica `INITIAL_ENROLLMENT_TRANSPORT=DENY_UNSPECIFIED`. Caso de cliente jÃ¡ matriculado mantÃ©m autenticaÃ§Ã£o normal, mas nÃ£o ganha mensagem de renewal nÃ£o existente. NÃ£o declarar bootstrap implementÃ¡vel enquanto essa lacuna persistir. Escolha futura que exija ampliar protocolos/perfis depende de autorizaÃ§Ã£o e decisÃ£o prÃ³prias, fora desta proposta de cÃ³digo.

## P8 â€” Issuer, lifecycle e recuperaÃ§Ã£o

ApÃ³s aprovaÃ§Ã£o completa, convergir ao mesmo PAPACC_ISSUER_REQUEST/backend/root e PREPARE â†’ VALIDATE â†’ durable COMMIT â†’ CURRENT â†’ EPOCH existentes. CSR exige perfil accepted/prova de posse validada, digest exato e identidade aprovada; dados pÃºblicos validados nÃ£o autorizam assinatura. Serial durÃ¡vel reservado antes de qualquer assinatura, high-water/history preservados; renovaÃ§Ã£o Ã© novo request/key/serial vinculado ao predecessor autorizado, nunca reciclagem de cÃ³digo/serial. RevogaÃ§Ã£o persiste e nega novos efeitos conforme publicaÃ§Ã£o; polÃ­tica de Sessions ativas continua pendente.

NÃ£o transferir chave privada automÃ¡tica; nÃ£o aplicar custÃ³dia manual. NÃ£o gerar CA/chave/certificado ou instalar trust automaticamente. Manter SIGNATURE_AUTHORITY e AUTHORITATIVE_RECOVERY DENY atÃ© gates prÃ³prios.

Stop apenas impede novos efeitos, nÃ£o cancela I/O/CNG nem implica destruiÃ§Ã£o. Owner conserva contexto, snapshots, pins, callbacks, workspaces, resultados incertos e crÃ©ditos atÃ© fechamento/quiescÃªncia factuais. ExpiraÃ§Ã£o lÃ³gica nÃ£o autoriza eviction, wipe prematuro ou reutilizaÃ§Ã£o de retidos. Limpeza posterior usa mecanismos revisados; cÃ³digo/nonce/senha/rawproof/chaves nunca em logs. Auditoria bounded: correlaÃ§Ã£o administrativa pÃºblica/revisÃ£o/fase/motivo/ator aprovado, sem cÃ³digo ou segredo. NÃ£o criar worker, registry ou log de autoridade paralelo.

## P9 â€” ED11 e aceitaÃ§Ã£o

Nenhum orÃ§amento novo Ã© aprovado. Windows3145728 somente opt-in; legacy2097152 e quotas individuais intactos. Antes de implementar/ativar, medir sizeof dos contextos e anÃ©is, CSR/SPKI/CA/DER/TBS, root snapshots/pins, issuer, verificador, Sessions existentes, heap/stack/provider, callbacks, coexistÃªncia e retenÃ§Ã£o. Contextos cancelados/incertos continuam cobrados. Reuso exige prova de exclusividade; nÃ£o excluir custo por estar no caller/stack/outro backend. Provider/kernel UNKNOWN nÃ£o Ã© zero. Base selected conhecida2103847 nÃ£o mede pico de enrollment; nenhuma margem dela autoriza buffers. CenÃ¡rio de pacote manual3625925 nÃ£o deve ser somado como se todo enrollment o instanciasse: automÃ¡tico nÃ£o consome KDF/ZIP/chave manual, mas suas coexistÃªncias possÃ­veis exigem censo.

P0â€“P9 necessitam aceitaÃ§Ã£o humana/normativa especÃ­fica e revisÃ£o oficial do executor antes de cÃ³digo dependente. Proposta nÃ£o qualifica transporte, fonte de autorizaÃ§Ã£o positiva/CURRENT, provider CLOSED, perfil Browser/NT4, reserva ED11 ou emissÃ£o. Tudo permanece DENY. RevisÃ£o deve decidir os parÃ¢metros P1/P3/P4, fontes P2/P5, consumo P6 e gate de transporte P7 explicitamente; nÃ£o aceitar patches funcionais por referÃªncia.
