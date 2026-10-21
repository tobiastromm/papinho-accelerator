# Matriz de conformidade, segurança e ED11

## Estado e precedência

HEAD main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a. A decisão explícita atual do owner resolve a escolha do endpoint separado. DECISION-enrollment-listener-adr0009-specialization-20261020 registra somente L0–L5 accepted após aprovação oficial de escrita. Nenhum parâmetro novo do candidato aplicativo é accepted por inferência. ADR-0009/0006 e propostas históricas ficam byte-exatas. T2/T6 Revision2 e ajustes mínimos de coerência T0/T3/T7 estão em cópia nova e patch exato; P0–P9 original não muda.

| Requisito | Fonte/restrição | Resultado |
|---|---|---|
| Listener explícito/imutável, coleção | ADR-0009 decisão/regras1–3/10; owner decisão atual | L0/L1 mantém; novo propósito ENROLLMENT só em endpoint separado |
| mTLS operacional TLS1.3/papacc/1 | ADR-0006; security_composition.c | L2 não muda; impossível selecionar auth por oferta ALPN/ausência de cert no operacional |
| Nova exceção restrita antes de client cert | decisão atual perfis ENROLLMENT | L2/L5 especializa somente novo listener, sem substituir o ADR inteiro |
| ALPN/framing | PST public config/negotiated ALPN | T2 usa TLS normal e novo perfil aplicativo PROPOSED, sem parser TLS ou mudança PST |
| Wire PACC | contrato Secure Principal/C2 existente | nenhum byte/ID de papacc/1 muda; novo perfil exclusivo candidato não entra no dispatcher operacional |
| Trust inicial | ADR-0006 Pairing; T1; S1–S5/profile1 | CA/servidor/nome/fingerprint e confirmação independente antes de BEGIN/segredos; CA externa/TOFU não pressupostos |
| Site TLS | ADR-0027 e decisão Site TLS18/10 | não reutilizado; sem Network Egress |
| OPEN/CLOSED/approval | provisioning18/10 itens3–5, ADR-0028 | CONFIRM distinções exatas; provider CLOSED ainda sem fonte => DENY |
| Claim único/continuidade | T4/T5 e P6 | mesma conexão/owner/request; primeiro acerto bloqueia replay; uma submissão no novo perfil |
| CSR/chave local | custódia5/profile1 | somente depois de approval; CSR/PoP exato; nenhuma private key no wire |
| Serial/CURRENT/CAS | issuer/profile1/root/C8/issuer-C2 | nenhum valor recebido vira autoridade; handoff idempotente; UNKNOWN sem retry/fallback |
| Lifecycle | Model E/ED08 | slots/leases/segredos/creditos retidos até fato; graceful deadline não é DESTROYED |
| Quotas | ED11/W1–W6 | novos subtetos são PROPOSED dentro do processo; nenhuma reserva ou aumento automático |
| Persistência/target | LST1/D3/ROOT08 e runtime singular atuais | ENROLLMENT não tem enum/codec aceito; gate de extensão versionada permanece explícito |

## Decisões novas que precisam de aceitação exata

ALPN papacc-enroll/1 (15bytes), PAE1/v1.0,8kinds/tabelas de payloads/sequence/caps, confidencialidade de CONFIRM, uma submissão (restrição do tetoP4), limites temporais/rates/memória e todos os parâmetros T2/T6 são candidatos. Autorização arquitetural L0–L5 não os aceita. COMPLEMENT-REVISION2-PROPOSED.md é a cópia composta candidata, não uma alteração do histórico. acceptance-scope-draft.md é minuta para a próxima revisão, não registro vigente.

Não foram alocados enum de listener em codecs, campos de BODY/root, parser directives, portas padrão ou IDs PACC. O novo ALPN e framing são proposta explícita autorizada para o serviço separado, não protocolo de produção criado por inferência. Não podem ser implementados antes da aceitação normativa específica e aprovação de patch.

## Análise de ataques e falhas

| Risco | Controle candidato | Limite factual |
|---|---|---|
| Downgrade por ALPN/SNI | endpoint escolhe perfil antes do handshake; configuração ALPN única e REQUIRED | runtime precisa suportar listener adicional sem truncamento, ainda não qualificado |
| Falso servidor/TOFU | material/nome/fingerprint independentes, chain/name/validade+handshake | fonte pública/Browser ceremony positiva ainda gate externo |
| Relay/troca de solicitante | código entregue independente a destinatário, mesmo TLS/owner/nonce/request até CSR | código não prova pessoa nem impede engenharia social de operador comprometido |
| Replay/segunda submissão | seqpor direção, estado fechado, claim único, uma CONFIRM; sem retomada | seq não é primitiva criptográfica; TLS e owner são obrigatórios |
| Swap CSR/key | CSR posterior autorizado, DER/SPKI exatos e PoP; clientAuth profile | CSR público validado não concede assinatura ou Principal |
| Brute force/login oracle | pré-handshake global6/origem3 por60s; P4geração2/min; uma CONFIRM, erroDENIED genérico | origemIP não identidade, NAT pode sofrer negação; canal não implementa provider |
| Slowloris/payload hostil | header16 antes debody, body<=16KiB,5s porframe parcial,8frames/32KiB por direção | fornecedorTLS/OS tem recursos opacos; não se declara hard bound nativo |
| Flood de origens/retidos |2lifetimes incluindo retidos,16origens, tabela cheia nega | limites podem reduzir disponibilidade; não faz evict ou delega a outro processo |
| Queda após possível emissão | RESULT_UNKNOWN/ISSUER_PENDING e retenção; sem retry privado/root fallback | cliente não recebe query novo; resultado precisa reconciliação interna autorizada |
| Stop/timeout/close falho | interrompe efeitos novos, release Model E factual, dependências/creditos preservados |2000ms não cancela provider nem prova fechamento; zero garantias inventadas |

## ED11: valores propostos não são capacidade demonstrada

| Parcela | Bytes/limite |
|---|---:|
| Base selected conhecida anterior, não pico |2103847|
| Sublimite managed enrollment inteiro proposto |262144|
| Cenário conservador adicional selected |2365991|
| Limite Windows explicitamente selecionado |3145728|
| Margem aritmética antes de custos não qualificados |779737|
| Base legacy conhecida anterior |2067655|
| Mesmo adicional hipotético no Legacy |2329799|
| Limite Legacy/outros perfis inalterado |2097152|
| Excesso hipotético Legacy |232647|
| RX+TX por conexão (16400*2) |32800|
| RX+TX para2conexões, incluído no sublimit |65600|
| Teto completo por conexão/lifetime |98304|
| Dois tetos de conexão |196608|
| Residual dentro de262144 para global/listener/stack/scratch atribuível |65536|
| Timestamps de código P4 |96|
| Timestamps preTLS (6+16*3)*8, sem metadata |432|
| Delta runtime aplicado nesta tarefa |0|

Não somar timestamps/filas de novo quando já embutidos no orçamento de262144/98304. São tetos candidatos, não alocações ou reservas prontas. Rate table16origens precisa metadata/alinhamento/lifetime; codecontext/privatecredentials/CSR/DER/backing/trust/peerinfo/callbacks/transitive stack e armazenamento issuer/worker/coexistência reais precisam sizeof/multiplicidade. Entrada/listener/OS backlog2 não mede kernel. CNG/PST/provider/kernel UNKNOWN não é zero. O mesmo processo pode ter operação/C2/C3/Site TLS/pacote manual coexistentes/retidos; censo completo não pode omiti-los. Não somar cegamente cenário manual como se todo enrollment o usasse, nem pressupor exclusividade.

Sob a base atual, Legacy não comporta o adicional hipotético: manter enrollment DENY nesse perfil, sem aplicar3MiB automaticamente ou apagar reservas. Windows apenas passa aritmética conhecida; todo custo requerido deve caber em subtetos e no processo, com crédito factual/host headroom antes de efeitos. Se a implementação não couber, DENY e devolver decisão para revisão; números propostos não provam ativação. ED11_TOTAL_STATUS=PARTIAL. Não houve build, teste, medição operacional ou código nesta tarefa.

## Portabilidade e handoff

Config/header BE/framing não dependem de Win32/CNG; PST permanece substituível por capabilities de role, TLS1.3, nome/trust, ALPN, nonblocking e Model E. C89/VC6 pode representar os bytes sem struct-packing; monotônico/range/UTF8 precisam implementação governada. Windows/box/ISA/NT4 não são qualificados por documentação. Entropia/relógio/limiter/hash/CSR/provider devem responder UNSUPPORTED/DENY se faltar semântica, sem downgrade ou fallback. Thumbprint não é CertificateIdentity/Principal; identidade TLS de servidor/cliente e autenticação de usuário continuam separadas.
