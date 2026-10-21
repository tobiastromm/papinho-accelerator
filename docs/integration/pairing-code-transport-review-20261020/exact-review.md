# Revisão exata P0–P9 e transporte inicial

Resultado: P0–P9 permanece PROPOSED; não há registro ACCEPTED. Revisão concluída, implementação não autorizada. HEAD main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a;710 hashes do manifesto anterior verificados; proposta original SHA25626762b4d8c888f1300775f2bddf3a1a76a5bbeaf99e6fece7eeafa76dbdee073 preservada.

## Matriz de revisão

| Cláusula | Verificação exata | Conclusão |
|---|---|---|
| P0 | Código separado de identidade, password, aprovação, emissão e grants; manual não rege chave automática | conforme em princípio; nenhum código impede relay sozinho |
| P1 | Alfabeto32sem duplicatas;10símbolos=50bits;7bytes fornecem56bits, descartar6não produz viés;10ou11bytes/hífenoffset5; sem aliases/locale | parâmetros explícitos, novos e ainda propostos; não é autenticação; origem/destino/entrega não definidos |
| P2 | CURRENT/R/A/P/I e contexto imutáveis; CSR posterior à autorização; revisão própria do issuer tratada em P6 | prova de continuidade entre conexão/contexto aprovado e CSR ainda ausente; nonce público sozinho não basta |
| P3 | 300000ms; start<=now<deadline; overflow/regressão negam; UTC distinto | explícito; falta timeout/deadline do handshake anterior à geração e alcance do prazo após handoff |
| P4 |5tentativas;2contextos incluindo retidos;2criações/10submissões por60000ms; anéis2/10 | limites exatos de código, não cobrem accepts/handshakes anônimos, bytes/CPU/filas antes de contexto; global é por processo contínuo |
| P5 | OPEN não exige login inicial; CLOSED valida username/password antes de aprovação; auto exige policy/manual exige ator | conforme, provider factual ainda PENDING; código não substitui password/trust |
| P6 | Consumo/submissão separados, UNKNOWN retém, CAS sem fallback, próprio PREPARE correlacionado | falta dizer que CONFIRMED rejeita qualquer segunda submissão enquanto aguarda aprovação, e definir quem apresenta/submete o código |
| P7 | Proíbe bootstrap inferido/novo wire e preserva mTLS | gate material declarado no próprio contrato; transporte não selecionado |
| P8 | CSR/PoP, reserva durável, renewal/revoke, Stop/retenção, Model E | preservado; não significa autoridade nativa ou política de revogação de Sessions ativas |
| P9 | custos completos, sem reservas presumidas,3MiB opt-in | correto como restrição, incompleto como orçamento de implementação/ativação |

Probabilidade meramente aritmética de5palpites uniformes contra50bits:5/2^50; não é prova de segurança de protocolo, origem, segredo da entrega, ausência de relay, pinning ou controle de concorrência. Não aceitar50bits como autenticação de servidor.

## Cláusulas materiais que impedem aceitação integral

1. P7: “APIs existentes não fornecem ainda transporte anterior à primeira credencial”. A decisão accepted provisioning18/10, Gates, exige decisão concreta própria; ADR-0006 define “TLS 1.3 com mutual TLS (mTLS)” e ADR-0009 fixa perfil antes do accept. A existência de server-auth no PST não especializa esses contratos do produto.
2. P1: “Aceitar como entrada local somente os10 bytes canônicos ou11 bytes com único hífen no offset5”. A frase define sintaxe local, mas não qual lado é verificador/submetente, qual ator recebe o segredo nem qual operação está sendo confirmada. P0–P9 não define uma cerimônia implementável de entrega do código.
3. P2: “um resultado imutável derivado associa a mesma solicitação/nonce ao CSR DER e SPKI DER exatos”. Sem continuidade de owner/conexão/transporte, uma correlação conhecida pode ser reapresentada por outro solicitante antes do CSR; PoP da nova chave prova essa chave, não a pessoa/instância aprovada. Não inferir canal binding, token bearer de retomada ou novo transcript. Somente aceitar vínculo por fonte factual/revisão/lifetime depois de contrato definido.
4. P6: “marca consumo do código irrevogável após aprovação factual do pareamento”. Não especifica o tratamento de uma segunda confirmação correta entre CONFIRMED e APPROVAL_GRANTED. Correção mínima: primeiro acerto faz claim irrevogável da tentativa para o request/owner, nenhuma segunda submissão é comparada; recusa/Stop/expiry não reabrem o código. Aprovação e autorização de emissão continuam separadas.
5. P4 limita geração/submissão, não handshakes pré-código. P3 começa após geração. Falta censo/admissão antes de criar conexão/handshake/CSR e respectivos deadlines/caps. Não inventar novos valores nesta revisão para parecer contrato fechado.

Essas são lacunas da proposta, não conflito entre dois ADRs accepted; nenhum ADR vigente será alterado. Não é seguro aceitar P0–P9 integralmente como independente de transporte. P1/P3 têm partes puramente locais aproveitáveis para uma futura aceitação recortada, mas isso não é autorização para aceitar o restante por referência; esta tarefa mantém o conjunto PROPOSED e registra uma única complementação proposta.

## Transporte protegido: mecanismo versus autoridade

PST public-api-abi.md/header separam Local Identity, Peer Authentication, trust e Expected Peer Name; fornecem CA customizada, peer snapshot/fingerprint, TLS1.3 e I/O incremental. O documento tls-runtime.md também contém evidência histórica de server-auth sem certificado de cliente; a versão de API desse relato é histórica, não prova do novo fluxo/API3/NT4. A matriz/provider deve satisfazer CLIENT e SERVER antes do binding; não há fallback pós-falha.

Menor mecanismo candidato: TLS1.3 autenticando o servidor com chave/certificado próprios do servidor, CA privada verificada por canal independente, hostname/SAN/SNI aprovados, certificado de cliente ausente apenas no propósito restrito de enrollment. Isso NÃO é o Secure Principal operacional aceito: requer especialização normativa explícita e discriminação de propósito antes do accept/handshake. Não se escolhe porta, ALPN, framing ou mensagem aqui. Sem regra de demultiplexação aprovada, não há rota que possa ser implementada.

Site TLS ADR-0027 autentica destinos outbound sob policy/Principal/Sessions já autorizados; decisão Site TLS18/10 reafirma trust server-owned e gate de egress. Não fornece acesso inbound ao novo Browser. Não encaminhar enrollment via Network Egress, C2 PAIR_OPEN, CONNECT, plaintext/STARTTLS ou HTTP por inferência. Não usar papacc/1 para conceder papel de enrollment sem decisão de protocolo. Reutilizar mecanismo PST não cria permissão de produto.

## Primeiro contato e portabilidade

O mecanismo conceitual mínimo é receber previamente por canal independente autenticado a identidade esperada do Accelerator, ExpectedServerName e fingerprint SHA256 do certificado de servidor/CA pública aplicável; conferir DER público antes de usá-lo como trust de tentativa. Certificado, chain, propósito serverAuth, SAN/nome e validade precisam ser verificados; fingerprint isolado não contorna esses checks. O conteúdo recebido pelo próprio peer não autentica a si mesmo. Falta de canal independente/estado/relógio/confiabilidade => DENY antes de password/código/CSR. Nenhuma CA externa ou trust store geral é necessária como requisito; a CA interna existente não é criada/qualificada por isto.

PST CUSTOM_CA_DER e peer_info são mecanismos reutilizáveis, não importer/cerimônia Browser prontos. Não afirmar que a importação avançada atual aceita CA-only nem que um pin efêmero já tem persistência/lifetime implementados. O Browser precisa de decisão local explícita para confiar nessa tentativa; a root do Accelerator não a concede. D3 do pacote manual serve de precedente de separação, não protocolo de enrollment.

Windows moderno pode selecionar backend elegível; box/ISA exigem equivalentes de CSPRNG, relógio, TLS/trust, owner/retenção e orçamento. Browser NT4/VC6 usa ABI portável/backend elegível, não CNG obrigatório. A existência do header C89 ou de provas históricas PST não qualifica esse fluxo ou seus perfis no NT4. Nenhum target foi testado nesta revisão.

## ED11 / Stop

Sem armazenamento funcional novo: delta aplicado0. Timestamps P4:96bytes de payload teórico (2+10)*8, não sizeof total. Somar apenas objetos realmente distintos. Censo de ativação faltante inclui accepts/handshakes não autenticados, state PST/provider, TLS record queues, certificado/chain/trust, peer snapshots, nonce/código, CSR/SPKI/DER, issuer/reserva/output, configuração/root/pins, contexto de aprovação, aliases/callbacks, stacks, concorrência e todos os retidos. Não criar Session operacional temporária: contexto de enrollment, se aprovado, será limitado e sem Principal/grants; ainda assim memória e conexão devem ser cobradas. A proposta não reserva pool nem créditos.

3MiB somente perfil Windows explícito; legacy2MiB, quotas individuais e crédito compartilhado intactos. Base2103847 não é pico de enrollment nem reserva de headroom. Custo de handshake/provider/kernel é UNKNOWN, não zero; ED11 PARTIAL, admissão DENY. O limiter deve proteger também trabalho pré-código antes dos efeitos; limites/timeout concretos e censo são gate pendente.

Stop não espera handshake/worker, nem prova cancelamento/destruição. Wait-set membership removida pelo owner antes do release; DESTROYED é factual, PRECONDITION_RETAINED preserva dependências, CLOSE_UNRESOLVED_RETAINED só permite consulta. Não remover leases/pins, limpar memória emprestada ou reusar slots por timeout, EOF/FINAL, processo encerrado ou falta de resposta. O CSR posterior exige a mesma continuidade, sem reconectar automaticamente com nonce antigo.
