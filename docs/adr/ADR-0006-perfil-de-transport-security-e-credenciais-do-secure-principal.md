---
adr: ADR-0006
title: Perfil de Transport Security e credenciais do Secure Principal
status: accepted
decision-date: 2026-09-06
last-revised: 2026-09-06
revision: 1
scope: project
decision-makers:
  - Tobias Tromm
supersedes: []
superseded-by: []
---

# ADR-0006 — Perfil de Transport Security e credenciais do Secure Principal

## Contexto

O `PapinhoAccelerator/ADR-0001` já define dois transport profiles distintos:

```text
Secure Principal
Legacy Endpoint
```

e estabelece que `Secure Principal` é o caminho normal/seguro, enquanto `Legacy Endpoint` é um perfil futuro, explícito e plaintext, sem downgrade automático.

Esse ADR-0001 responde **quais perfis existem e como eles se relacionam**.

Falta, porém, formalizar separadamente **qual é o perfil criptográfico e de credenciais do Secure Principal**.

A Phase 3 consolidou que o caminho seguro de produção deve utilizar protocolo criptográfico padrão, autenticação mútua, identidade por principal estável, trust anchors administrados, fail-closed e separação entre mecanismo criptográfico e policy.

Também houve revisão arquitetural importante: a escolha inicial de external PSK foi abandonada em favor de mTLS com certificados individuais por dispositivo.

Esta decisão registra esse perfil sem redefinir `Legacy Endpoint` e sem transformar detalhes de backend em identidade arquitetural.

## Forças da decisão

- confidencialidade;
- integridade;
- autenticação mútua;
- identidade estável;
- revogação;
- escopo limitado de comprometimento;
- compatibilidade com clientes legados;
- interoperabilidade entre backends;
- downgrade resistance;
- fail-closed;
- separação entre autenticação e autorização;
- não exposição de private keys ao core;
- independência entre policy e TLS backend.

## Alternativas consideradas

### Alternativa A — External PSK por cliente

Cada cliente possui um segredo simétrico compartilhado com o servidor.

**Vantagens**

- modelo conceitual simples;
- autenticação simétrica direta.

**Desvantagens / trade-offs**

- servidor armazena segredos de todos os clientes;
- comprometimento do servidor expõe relações simétricas;
- rotação/revogação exige lifecycle próprio;
- integração enterprise é menos natural;
- investigação do backend legado mostrou ausência da API pública necessária no NSS alvo;
- exigiria trabalho adicional de backport/API.

### Alternativa B — TLS apenas com autenticação do servidor

O servidor possui certificado, mas clientes não autenticam por certificado individual.

**Vantagens**

- provisioning mais simples para clientes;
- menor complexidade de PKI.

**Desvantagens / trade-offs**

- não fornece identidade criptográfica individual de cliente;
- autorização por dispositivo/principal fica enfraquecida;
- DATA association segura fica mais difícil;
- credenciais de aplicação separadas seriam necessárias.

### Alternativa C — TLS 1.3 com mTLS e certificados individuais

Servidor e clientes autenticam-se mutuamente através de certificados e private keys, com trust anchors administrados.

**Vantagens**

- identidade forte por dispositivo;
- private keys permanecem nos respectivos endpoints;
- revogação/renovação naturais;
- integração enterprise;
- mapeamento para Principal estável;
- backend comprometido no servidor não revela private keys dos clientes;
- compatível com backends TLS distintos desde que implementem o mesmo perfil.

**Desvantagens / trade-offs**

- exige enrollment/trust management;
- certificados dependem de wall clock confiável;
- pairing/bootstrap precisa de desenho cuidadoso;
- operação de CA exige disciplina.

## Decisão

O `Secure Principal` utiliza **TLS 1.3 com mutual TLS (mTLS)** como perfil normal de Transport Security do PapinhoAccelerator.

A relação conceitual é:

```text
TCP / Transport
    ↓
TLS 1.3 mTLS
    ↓
validated peer credential
    ↓
stable authenticated Principal
    ↓
PACC framing / protocol
    ↓
Session / Channel policy
```

### TLS 1.3 only

O perfil seguro utiliza TLS 1.3.

```text
TLS 1.2 fallback = forbidden
cleartext fallback = forbidden
```

Falha em estabelecer o perfil seguro encerra a tentativa.

Este ADR não redefine a existência de `Legacy Endpoint`; ele apenas define o Secure Principal.

A seleção entre Secure Principal e Legacy Endpoint continua governada pelo ADR-0001 e nunca ocorre como fallback de um handshake falho.

### Mutual TLS obrigatório

Servidor e cliente devem autenticar-se mutuamente.

Servidor:

```text
server certificate
+ server private key
```

Cliente:

```text
individual device certificate
+ client private key
```

Cada dispositivo cliente deve possuir sua própria credencial, salvo decisão futura explícita para outro modelo.

### Trust model

O trust anchor pode ser:

```text
Papinho private CA
ou
administrator-selected enterprise CA
```

Public Web PKI não é requisito do perfil.

A CA de assinatura não é uma runtime credential do Accelerator.

A private key da CA não deve ser distribuída ao servidor como requisito normal nem aos clientes.

### Private keys permanecem no endpoint proprietário

Client private key:

```text
generated/stored on client
```

Server private key:

```text
generated/imported/stored on server
```

O core não deve transportar raw private-key material entre camadas apenas para satisfazer um backend.

Backends devem trabalhar com referências/handles apropriados sempre que possível.

### Credencial não é Principal

Certificado, subject, fingerprint ou public key não são automaticamente a identidade interna final.

Mapeamento:

```text
validated trust chain
+ certificate policy
+ private-key proof
        ↓
credential/device record
        ↓
stable authenticated Principal
```

O Principal é semanticamente distinto de:

```text
certificate bytes
public-key bytes
subject string
serial number
fingerprint
IP address
Connection ID
Session ID
Channel ID
DATA ticket
```

Renovação de certificado pode preservar o mesmo Principal.

### Autenticação e autorização são separadas

```text
Authentication
→ quem é este peer?

Authorization
→ o que este Principal pode fazer?
```

mTLS autentica.

Policy autoriza.

Autenticação não concede automaticamente:

- criação de Session;
- DATA attachment;
- capability use;
- network egress;
- quotas;
- acesso administrativo.

### CONTROL e DATA autenticam independentemente

Cada conexão CONTROL ou DATA estabelece seu próprio Transport Security.

Uma CONTROL Connection segura não torna outra DATA Connection segura.

Baseline:

```text
CONTROL TCP
    → full TLS 1.3 mTLS
    → authenticated Principal
    → PACC
```

e:

```text
DATA TCP
    → independent full TLS 1.3 mTLS
    → authenticated Principal
    → PACC DATA_ATTACH
```

### DATA exige o mesmo Principal da Session CONTROL

Uma DATA attachment segura exige simultaneamente:

```text
authenticated DATA Principal
        ==
authenticated CONTROL Session Principal

AND valid structural ticket

AND DATA authorization

AND existing Session/CONTROL lifecycle invariants
```

Ticket estrutural não é identidade.

Principal mismatch não deve consumir um ticket legítimo.

### ALPN

O perfil protegido usa ALPN obrigatório com valor exato:

```text
papacc/1
```

ALPN identifica o application security profile.

Ele não substitui versionamento de:

- PACC;
- framing;
- messages;
- software;
- capabilities.

Missing/mismatched ALPN falha o handshake.

### 0-RTT

```text
0-RTT = disabled
```

Motivo: replay e ambiguidade de autorização.

### Resumption

```text
resumption = disabled/deferred
```

Até que principal binding, revogação, expiry e replay semantics sejam definidos.

CONTROL e DATA usam full handshakes na baseline atual.

### Forward secrecy

Forward secrecy é obrigatória através de ephemeral key exchange.

Perfil atual:

```text
preferred group candidate: X25519
compatibility candidate: P-256
```

A seleção final de grupos pode depender da evidência concreta de backend/target, mas não pode remover a propriedade de forward secrecy.

### Cipher suites

Perfil atual:

```text
TLS_CHACHA20_POLY1305_SHA256
→ required/preferred

TLS_AES_128_GCM_SHA256
→ allowed
```

Suites obsoletas são proibidas.

ChaCha20-Poly1305 é preferida por desempenho previsível em CPUs sem aceleração AES.

### Policy é superior ao backend

Backend implementa mecanismo.

Papinho define policy.

```text
backend
→ handshake mechanics
→ certificate operations
→ secure stream I/O

Papinho policy
→ TLS version
→ required auth model
→ trust rules
→ ALPN
→ allowed suites/groups
→ principal semantics
→ authorization gates
```

Um backend não pode silenciosamente:

- habilitar TLS 1.2;
- aceitar cleartext;
- remover client authentication;
- relaxar trust validation;
- aceitar outro ALPN;
- habilitar 0-RTT;
- enfraquecer downgrade policy.

### Backends podem variar

O perfil é independente da biblioteca concreta.

Backends distintos podem ser usados em plataformas diferentes, desde que implementem o mesmo contrato e policy.

A prova da Phase 3.A2B-R3 mostrou RetroZilla NSS/NSPR como backend legado tecnicamente viável para TLS 1.3 mTLS no target NT4/VC6, mas isso não transforma NSS em requisito arquitetural universal.

### Certificate validity e wall clock

Certificate validation exige wall clock plausível/confiável.

```text
monotonic clock
→ protocol/lifecycle durations

wall clock
→ certificate validity
```

Não desabilitar `notBefore`/`notAfter` para contornar relógio incorreto.

Clock inválido deve falhar fechado com diagnóstico explícito.

### Entropia

Material criptográfico exige CSPRNG/entropy source adequado.

Falha ou indisponibilidade:

```text
→ security failure
→ fail closed
```

Não utilizar fallback ambiental não criptográfico para:

- key generation;
- ephemeral keys;
- signatures;
- nonces;
- pairing secrets.

### Pairing e first-use trust

Não existe silent TOFU.

IP, DNS name, discovery result ou successful TCP connection não são identidade suficiente.

O bootstrap deve permitir verificação explícita da identidade do servidor.

Baseline conceitual inclui:

- server identity/trust domain;
- SHA-256 certificate fingerprint;
- explicit verification;
- independent trusted channel quando necessário.

Pairing code:

```text
!= reusable password
!= standalone identity
```

Deve ser:

- short-lived;
- single-use;
- rate-limited;
- bound to current pairing context;
- bound to expected server identity/fingerprint;
- bound to client-generated key/request.

Este ADR define arquitetura de trust/enrollment, não wire messages ou GUI final.

### Cert lifecycle

O design deve suportar:

- renewal;
- revocation;
- server credential rotation;
- CA rotation;
- compromised client credential handling;
- compromised server credential handling;
- compromised CA handling;
- malicious enrollment revocation.

Revogação é operação de trust/authorization, não mero delete de arquivo.

### Logging e secrets

Nenhum nível de log pode incluir raw secrets.

Não registrar:

- private keys;
- session keys;
- shared secrets;
- raw proofs;
- full security tokens;
- protected plaintext;
- credential material sensível.

`--log-level off` não é mecanismo de proteção de segredo; segredo não deve entrar no logger em nenhum nível.

## Justificativa

mTLS oferece identidade assimétrica individual por dispositivo, permite mapear credenciais renováveis para Principals estáveis e integra-se melhor a PKI privada ou enterprise.

TLS 1.3-only reduz ambiguidade de downgrade e mantém o perfil uniforme entre plataformas antigas e novas.

Separar policy de backend evita que limitações ou defaults de uma biblioteca alterem silenciosamente a postura de segurança do PapinhoAccelerator.

CONTROL e DATA autenticados independentemente evitam assumir que segurança de uma conexão se propaga para outra.

## Consequências

### Positivas

- confidencialidade e integridade fortes;
- autenticação mútua;
- identidade individual por dispositivo;
- revogação/renewal;
- server compromise não expõe client private keys;
- integração enterprise;
- downgrade resistance;
- DATA binding pode usar Principal equality;
- backends podem variar por plataforma;
- policy permanece uniforme.

### Negativas / trade-offs

- PKI/enrollment precisam ser operados;
- wall clock confiável é requisito;
- pairing/bootstrap é complexo;
- certificados e revogação exigem lifecycle;
- legacy targets exigem backend TLS capaz de atender o perfil.

### Neutras ou operacionais

- este ADR não implementa Transport Security;
- não congela storage format de certificados;
- não cria mensagens PACC de pairing;
- não define GUI de enrollment;
- não obriga NSS como backend universal;
- Legacy Endpoint continua governado separadamente pelo ADR-0001.

## Regras derivadas

1. Secure Principal usa TLS 1.3-only.
2. mTLS é obrigatório para Secure Principal.
3. Cada cliente usa credencial individual por dispositivo na baseline.
4. Public Web PKI não é requisito.
5. Private keys permanecem nos respectivos endpoints.
6. Credential não é Principal.
7. Principal é estável e distinto de certificate bytes/subject/fingerprint.
8. Authentication e authorization são separadas.
9. CONTROL e DATA fazem handshakes independentes.
10. DATA exige Principal igual ao CONTROL da Session.
11. Ticket estrutural não é identidade nem autoridade.
12. ALPN `papacc/1` é obrigatório.
13. 0-RTT é desabilitado.
14. Resumption é desabilitado/deferred.
15. Forward secrecy é obrigatória.
16. Backend não pode enfraquecer Papinho policy.
17. TLS backend pode variar por target.
18. Certificate validity usa wall clock confiável.
19. Lifecycle timeout usa monotonic clock, conforme decisões de runtime.
20. Entropy/CSPRNG failure é fail-closed.
21. Silent TOFU não é permitido.
22. Pairing code não é senha reutilizável nem identidade standalone.
23. Secrets não entram em logs em nenhum nível.
24. Este ADR não altera nem enfraquece `PapinhoAccelerator/ADR-0001`.

## Impacto

### Código

A futura integração de `PapinhoSecureTransport` deve expor mecanismos seguros sem vazar estruturas de backend ao core.

Session/Connection security context deve permanecer separado das entidades portáveis fundamentais quando apropriado.

### Build e toolchains

Targets podem utilizar backends TLS diferentes.

Todos devem implementar o mesmo perfil normativo quando declararem suporte a Secure Principal.

### Documentação

`docs/phase3-transport-security-profile.md` permanece como checkpoint técnico/histórico detalhado.

Este ADR passa a ser o registro canônico da decisão arquitetural do perfil seguro.

### Compatibilidade

Uma plataforma/backend incapaz de cumprir o perfil não deve receber um perfil criptográfico mais fraco com o mesmo nome.

```text
cannot satisfy Secure Principal
→ Secure Principal unavailable
```

Isso não autoriza downgrade automático para Legacy Endpoint.

### Testes

Testes futuros devem provar, conforme aplicável:

- TLS 1.3-only;
- mTLS;
- trust validation;
- required ALPN;
- no 0-RTT;
- no resumption na baseline;
- Principal extraction;
- independent CONTROL/DATA handshake;
- DATA Principal equality;
- certificate-time validation;
- entropy failure;
- no cleartext/TLS 1.2 fallback;
- secret-safe logging.

## Verificação de conformidade

Pode-se verificar se:

- Secure Principal não aceita TLS 1.2;
- client certificate é requerido;
- backend defaults não substituem Papinho policy;
- ALPN mismatch falha;
- CONTROL e DATA autenticam independentemente;
- ticket não é tratado como identidade;
- authenticated Principal é separado de certificate representation;
- private keys não atravessam o core como dados brutos sem necessidade;
- invalid clock falha certificate validation;
- entropy failure falha fechado;
- logs não contêm segredo;
- nenhum handshake failure seleciona Legacy Endpoint.

## Relações

### ADRs relacionadas

- `PapinhoAccelerator/ADR-0001` — Secure Principal e Legacy Endpoint.
- `PapinhoAccelerator/ADR-0003` — Connection, Session e Channel — ownership e lifecycle.
- `PapinhoAccelerator/ADR-0005` — Negociação de capabilities, disponibilidade de backend e autoridade de policy.
- `PapinhoEngineering/ADR-0007` — Fronteiras portáveis entre core, plataforma e backends substituíveis.
- `PapinhoEngineering/ADR-0003` — Contrato estruturado e desacoplado de logging.

### Capability Documents relacionadas

Nenhuma específica para Transport Security; Transport Security não é capability negociável.

### Documentos relacionados

- `docs/phase3-transport-security-profile.md`
- `docs/phase3-security-architecture.md`
- `docs/phase3-transport-profiles.md`
- `docs/phase3-nss-mtls-nt4-proof.md`

## Princípio

**Secure Principal é uma identidade autenticada por um perfil criptográfico forte e uniforme; não é apenas uma conexão TLS que “deu certo”.**

E:

```text
credential != Principal
authentication != authorization
backend != policy
TLS failure != legacy fallback
```

## Histórico de revisões

| Revisão | Data | Descrição |
|---:|---|---|
| 1 | 2026-09-06 | Formalização do perfil TLS 1.3 mTLS, trust/credential model, Principal semantics, CONTROL/DATA authentication e separação entre backend e policy. |
