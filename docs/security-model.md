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

Este documento originalmente registrou requisitos sem escolher mecanismo. A
3.A2A agora seleciona o perfil e o modelo de credencial iniciais, mas ainda não
escolhe biblioteca TLS, backend nem implementação criptográfica. Criptografia
própria é proibida.

**Estado de implementação:** não existem autenticação, Transport Security nem
`TLS_OFFLOAD`. O servidor processa Control establishment; E1 apenas congela um
ticket opaco one-time para futura associação estrutural DATA. Esse ticket não é
credencial, autenticação nem autorização segura. O serviço não deve ser
apresentado como seguro nesta fase.

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
NSS/NSPR como backend legado viável; ele ainda não está integrado ao produto.
PapinhoAccelerator não deve inventar um protocolo criptográfico próprio.

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

Transport Security deverá fornecer essas propriedades por mecanismo futuro, sem biblioteca fixada nesta baseline. Sua configuração é independente da negociação de `TLS_OFFLOAD`.

## Data Channels e Sessions

Um Data Channel não deve ser aceito apenas por apresentar um Session ID. E1
deliberadamente não cria Wire Session ID: usa ticket temporário estrutural que
expira monotonicamente e é invalidado com CONTROL/Session. Isso ainda não prova
identidade ou autorização. Phase 3 deverá inserir validação autenticada e
autorizada entre ticket resolution e lifecycle/bind, limitar replay e impedir
vínculo cruzado entre Sessions.

## Fail-safe defaults

Capability, egress, destino, parâmetro ou versão desconhecidos são negados por padrão. Erros devem limitar o menor escopo seguro, mas encerrar a Session quando integridade, framing ou estado não puderem ser confiados. Degradação graciosa não permite enfraquecer autenticação, confidencialidade ou autorização.

## Threat Model futuro

Antes de implementação pública, um Threat Model deverá cobrir peers maliciosos, mensagens fragmentadas/malformadas, exaustão de recursos, replay/hijacking, downgrade, abuso de egress/SSRF, resolução DNS e redirects, backends não confiáveis, isolamento de jobs, supply chain e exposição de dados. As fronteiras entre cliente, transport, parser, Session, Policy Engine e backend devem ser analisadas explicitamente.
