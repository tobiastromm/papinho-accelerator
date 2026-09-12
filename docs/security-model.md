# Modelo de segurança

As decisões arquiteturais canônicas vigentes estão registradas em:

- [ADR-0001 — Secure Principal e Legacy Endpoint](adr/ADR-0001-secure-principal-e-legacy-endpoint.md);
- [ADR-0005 — Negociação de capabilities, disponibilidade de backend e autoridade de policy](adr/ADR-0005-negociacao-de-capabilities-disponibilidade-de-backend-e-autoridade-de-policy.md);
- [ADR-0006 — Perfil de Transport Security e credenciais do Secure Principal](adr/ADR-0006-perfil-de-transport-security-e-credenciais-do-secure-principal.md).

O checkpoint histórico autoritativo para o threat model da Phase 3.A1 é
[Phase 3 Security Architecture and Threat Model](phase3-security-architecture.md).
Este resumo permanece alinhado com ele; detalhes de ameaças, fluxos, gates e
decisões então pendentes estão lá. As autoridades arquiteturais canônicas
posteriores são os ADRs listados acima.

Este documento originalmente registrou requisitos sem escolher mecanismo. O
ADR-0006 define o profile/policy do Accelerator. PapinhoSecureTransport (PST)
foi posteriormente implementado como biblioteca independente e é a abstração
Secure Transport pronta para consumo. Providers concretos permanecem
selecionáveis por target; nenhum provider é universalmente obrigatório.
Criptografia própria é proibida.

**Estado de implementação:** o controller Win32 seguro opt-in integra PST,
TLS 1.3 mTLS, AuthN/AuthZ, CONTROL e DATA. A prova 3.F usa um cliente de
referência em processo separado. O executável padrão ainda não possui fonte
operacional de credenciais/listener Secure, e `TLS_OFFLOAD` não foi
implementado. O serviço padrão não deve ser apresentado como seguro.

## Transport Security e TLS Offload

Transport Security é a segurança da comunicação `PapinhoAccelerator Client ↔ PapinhoAccelerator Server`. Ela pertence à infraestrutura e ao protocolo do PapinhoAccelerator e protege Control Channel, Data Channels, autenticação, comandos, payloads, credenciais e dados enviados para processamento. Não é uma capability de processamento normal.

`TLS_OFFLOAD` é um conceito diferente: uma capability negociável que poderá futuramente auxiliar ou executar operações TLS relacionadas às conexões do cliente com sites ou serviços externos. O nome e o ID definitivos dessa capability não estão congelados.

```text
Transport Security != TLS Offload

PapinhoAccelerator Transport Security = REQUIRED
TLS_OFFLOAD capability                = DISABLED
```

Essa combinação é válida. Desabilitar `TLS_OFFLOAD`, ou qualquer outra capability, nunca pode implicitamente desabilitar Transport Security. Capability Negotiation não pode enfraquecer propriedades de segurança exigidas para a Session. Quando política ou configuração exigir canal seguro, downgrade silencioso para transporte inseguro é proibido.

O perfil revisado foi congelado em [Phase 3 Transport Security and Credential
Profile](phase3-transport-security-profile.md): TLS 1.3 mTLS, CA
privada/administrativa, certificado individual por dispositivo cliente, sem
0-RTT, resumption ou fallback. A validação 3.A2B-R3 comprovou RetroZilla
NSS/NSPR como backend legado viável. Esse backend hoje pertence ao PST; o
Accelerator não integra NSS/NSPR diretamente. A Phase 3.B2 integrou PST; o pin
atual é PST 0.6.1 (API 2.1/SPI 3.0), na composição privada e opt-in do lifecycle de
segurança. A Phase 3.B3 adicionou somente a adaptação estruturada e secret-safe
dos eventos públicos PST para o logger do Accelerator; isso não torna o servidor
seguro. A Phase 3.B4 adicionou wait-set/readiness multiplexada privada,
incremental e limitada, também sem ligar PST ao accept loop real. Isso não torna o servidor
TLS-enabled.
PapinhoAccelerator não deve inventar um protocolo criptográfico próprio.

A Phase 3.C implementa somente a foundation privada: evidência PST copiada,
Principal opaco não persistido, resolver e authorization provider injetados e
contexts separados para conexão/Session. Fingerprint é chave de lookup, nunca
Principal. A 3.E integra essa foundation ao controller seguro opt-in e a 3.D
fornece o binding DATA autenticado/autorizado.
Essa foundation foi validada com TLS 1.3 mTLS real e ALPN `papacc/1` em
loopback. O Secure Principal mantém graceful shutdown como REQUIRED e, com PST
0.6.1, exige também `PST_CAP_GRACEFUL_SHUTDOWN` antes do binding. Os casos
NOT_ENROLLED, DENY e DISABLED não publicam Principal nem contexto autorizado.

A Phase 3.D adiciona a gate privada de Secure DATA. O ticket continua sendo
correlação estrutural, nunca identidade ou autorização bearer. Emissão exige
Session Security Context publicado e autorização DATA. Attach exige contexto
DATA autorizado, Session Security Context do target, igualdade pela primitive
de Principal e autorização DATA antes do commit one-time. Falhas de identidade
ou policy preservam um ticket estrutural ainda válido.

A Phase 3.E conecta essas boundaries ao accept e ao caminho CONTROL/DATA real
por meio de um controller Win32 opt-in. TLS 1.3 mTLS e ALPN `papacc/1`
terminam antes da publicação da Connection e antes do classifier. Falhas TLS,
de identidade, policy ou protocolo fecham somente o candidato; o listener não
muda de perfil e não existe fallback plaintext.

A Phase 3.F valida essa fronteira externamente com um processo cliente separado,
usando somente a API pública PST CLIENT e os bytes PACC já especificados. A
prova cobre CONTROL, DATA em TLS independente, replay, credential estrangeira,
negativos client-facing e close seguro, sem expor Principal interno no wire.

```text
PROFILE / POLICY
    → ADR-0006 e policy do Accelerator

SECURE TRANSPORT ABSTRACTION
    → PapinhoSecureTransport, existente e pronto para consumo

CONCRETE PST PROVIDER PER TARGET
    → selecionável/substituível; nenhum provider universal obrigatório

INTEGRATION INTO ACCELERATOR
    → controller Win32 seguro opt-in implementado; wiring operacional ausente
```

A decisão arquitetural posterior define os perfis futuros
[Secure Principal e Legacy Endpoint](phase3-transport-profiles.md). Legacy
Endpoint é plaintext, explicitamente habilitado e sem identidade criptográfica
forte; não é um modo seguro. Não existe fallback automático do perfil seguro
para o legado.

O provider de tickets da Phase 2.E3 é apenas um contador opaco determinístico.
Ele não é RNG, credencial, autenticação ou autorização e não oferece segredo ou
imprevisibilidade. Phase 3 poderá substituir o provider, adicionar autorização
de associação e inserir Transport Security sem derivar tickets de IPs ou IDs
runtime e sem mudar o campo opaco de 16 bytes, salvo revisão protocolar futura.

## Modos, autenticação e autorização

Os termos históricos `OPEN` e `AUTHENTICATED` descreviam possibilidades de
autenticação antes da formalização dos transport profiles. Eles não são
transport profiles concorrentes e não substituem `Secure Principal` ou
`Legacy Endpoint`, cuja semântica canônica pertence ao ADR-0001.

Na terminologia histórica, “open” significava somente ausência de autenticação
de cliente exigida, não ausência de política, validação ou limites. Essa
descrição não concede segurança ao canal, não caracteriza um Secure Principal
e não autoriza fallback para Legacy Endpoint. Possíveis mecanismos adicionais,
como usuário/senha, tokens ou allowlists, continuam trabalho futuro e não foram
decididos por este documento.

Autenticação estabelece uma identidade; autorização decide o que essa identidade pode fazer. São etapas independentes. Uma identidade pode, por exemplo, usar decode e framebuffer, mas não transcoding ou network egress. A autorização deve considerar capability, parâmetros, local de execução, egress e quotas.

A experiência futura de first-use trust e enrollment está registrada no
[Capability Document de Client Pairing](capabilities/client-pairing.md). Ele
traduz o perfil canônico do ADR-0006 para UX conceitual sem definir mensagens
PACC, GUI ou implementação. Código de pairing, nome e IP não são identidade do
servidor; private keys de cliente permanecem no cliente; enrollment aprovado
resulta em credencial/Principal submetido a policy, não em autorização
irrestrita.

## Propriedades obrigatórias

- Confidencialidade e integridade para controle e dados quando o cenário exigir transport seguro.
- Autenticação do servidor e proteção de credenciais.
- Proteção consistente de todos os Data Channels.
- Associação autenticada e íntegra entre Data Channel e Session.
- Prevenção de replay quando aplicável e IDs/tokens não previsíveis.
- Validação de estado, tipo, comprimento e limites antes de alocação/processamento.
- Limites de CPU, RAM, banda, Sessions, jobs, streams, tempo e filas.
- Cleanup seguro, cancelamento e isolamento de falhas entre Sessions.
- Erros e logs que sejam úteis sem vazar credenciais, tokens ou dados sensíveis.

Transport Security deverá fornecer essas propriedades no Accelerator por meio
do PST. A policy específica continua pertencendo ao Accelerator e sua
configuração permanece independente da negociação de `TLS_OFFLOAD`.

Secure Principal usa PST. Legacy Endpoint permanece Transport/PACC plaintext
explicitamente configurado, não passa pelo PST e não é representado como
`PST_BACKEND_NONE`, `PST_TLS_OFF` ou outro pseudo-provider.

## Data Channels e Sessions

Um Data Channel não deve ser aceito apenas por apresentar um Session ID. E1
deliberadamente não cria Wire Session ID: usa ticket temporário estrutural que
expira monotonicamente e é invalidado com CONTROL/Session. Isso ainda não prova
identidade ou autorização. Phase 3 deverá inserir validação autenticada e
autorizada entre ticket resolution e lifecycle/bind, limitar replay e impedir
vínculo cruzado entre Sessions.

## Fail-safe defaults

Capability, egress, destino, parâmetro ou versão desconhecidos são negados por padrão. Erros devem limitar o menor escopo seguro, mas encerrar a Session quando integridade, framing ou estado não puderem ser confiados. Degradação graciosa não permite enfraquecer autenticação, confidencialidade ou autorização.

O framework da Phase 4 adiciona autorização granular sem substituir AuthN/AuthZ.
Seu snapshot de Session é somente upper bound: membership não basta para uma
operação, e disable/revocation contextual pode negar imediatamente. Policy não
pode expandir silenciosamente o snapshot nem afetar Transport Security.

## Threat Model futuro

Antes de implementação pública, um Threat Model deverá cobrir peers maliciosos, mensagens fragmentadas/malformadas, exaustão de recursos, replay/hijacking, downgrade, abuso de egress/SSRF, resolução DNS e redirects, backends não confiáveis, isolamento de jobs, supply chain e exposição de dados. As fronteiras entre cliente, transport, parser, Session, Policy Engine e backend devem ser analisadas explicitamente.
