---
capability: tls-offload
title: TLS Offload para conexões externas
status: concept
last-updated: 2026-09-07
scope: project
---

# TLS Offload para conexões externas

## Objetivo

Registrar o conceito de uma capability futura que poderá auxiliar ou executar
operações TLS relacionadas a conexões do cliente com sites ou serviços
externos.

## Escopo

### Inclui

- TLS associado ao fluxo externo cliente → site/service;
- possível execução ou auxílio pelo Accelerator;
- futura negociação, autorização, policy, limites e seleção de backend;
- composição futura com network egress, mantendo permissões independentes.

### Não inclui

- Transport Security do hop PapinhoAccelerator Client ↔ Server;
- definição de um backend ou biblioteca TLS;
- implementação de proxy, egress, HTTP ou transporte seguro;
- IDs, mensagens ou payloads PACC;
- fallback automático ou downgrade de segurança.

## Comportamento esperado

`TLS_OFFLOAD` é o nome conceitual atual. Ele não implica nome definitivo nem ID
wire congelado.

```text
Transport Security != TLS_OFFLOAD
```

Transport Security protege CONTROL e DATA entre cliente e Accelerator. Esta
capability trata de TLS de uma conexão externa. Habilitar, desabilitar, negar
ou não suportar `TLS_OFFLOAD` não pode alterar o transport profile da Session
nem enfraquecer o Secure Principal.

## Arquitetura / fluxo

Fluxo puramente conceitual, sem congelar protocolo ou implementação:

```text
client request for an external service
        ↓
Capability Negotiation + authorization + policy
        ↓
TLS_OFFLOAD effective?
        ↓
future external-TLS processing path
```

Capability, backend availability, policy e configuração efetiva seguem o
ADR-0005.

### Modelos conceituais de ownership do transport externo

Existem pelo menos dois modelos possíveis para uma implementação futura. Esta
baseline não escolhe entre eles.

#### Modelo A — client-owned external transport

```text
external service
      ↕
external transport originado e possuído pelo cliente
      ↕
cliente Papinho
      ↕ fluxo seguro / processamento TLS assistido
PapinhoAccelerator
```

Nesse modelo conceitual, o destino externo continua vendo a origem de rede do
cliente enquanto o Accelerator auxilia ou realiza parte do processamento
seguro. `client-owned external transport` ainda precisa ser desenhado. Não se
presume viabilidade com API ou wire atuais, retransmissão de TLS records,
ownership de socket específico, suporte do PST ou qualquer outro mecanismo.

#### Modelo B — Accelerator-owned external transport

```text
cliente Papinho
      ↓ solicitação autorizada
PapinhoAccelerator
      ↓ conexão externa originada pelo Accelerator
external service
```

Esse modelo exige autoridade e policy de `NETWORK_EGRESS_ACCELERATOR` e pode
também usar `TLS_OFFLOAD` ou outro processamento seguro externo. Quando ambos
forem efetivos, existem duas conexões e relações de segurança independentes:

```text
CONEXÃO 1 — Transport Security do PapinhoAccelerator
client -> Secure Principal / Transport Security aplicável -> Accelerator

CONEXÃO 2 — protocolo seguro externo
Accelerator -> protocolo seguro exigido pelo serviço -> external service
```

Não existe uma única TLS Session contínua atravessando
client → Accelerator → external service. O Accelerator termina a primeira
relação de segurança e, quando autorizado, estabelece outra conexão e outra
relação de segurança com o serviço externo. Isso não é tradução direta de TLS
antigo para TLS novo.

O protocolo e o perfil da segunda conexão podem evoluir independentemente do
primeiro hop. Uma versão futura de TLS ou um protocolo seguro sucessor é apenas
uma possibilidade ilustrativa, não uma versão declarada, requisito ou suporte
atual.

### Composição com Network Egress

```text
TLS_OFFLOAD != permissão de NETWORK_EGRESS
NETWORK_EGRESS_ACCELERATOR != TLS_OFFLOAD habilitado

NETWORK_EGRESS_ACCELERATOR autorizado
        +
TLS_OFFLOAD efetivo
        ↓
possível conexão segura externa pelo Accelerator no futuro
```

Cada autoridade deve ser concedida e avaliada separadamente. `TLS_OFFLOAD` não
permite que o Accelerator abra conexões externas, e a permissão de egress não
habilita processamento TLS. Portanto, são possibilidades futuras distintas:

```text
A. TLS_OFFLOAD + client-owned external transport
B. TLS_OFFLOAD + Accelerator-owned external transport
```

No modelo B, a composição também depende da policy da Session, do destino e do
protocolo, além de implementação futura.

Essa composição não enfraquece o primeiro hop. Se o Secure Principal não puder
atender ao perfil de Transport Security vigente, ele permanece indisponível ou
falha explicitamente até que uma decisão futura altere o perfil ou a seleção de
provider. Não há downgrade silencioso, uso continuado de protocolo quebrado por
estar em uma LAN nem fallback automático para Legacy Endpoint.

PST permanece a API provider-neutral de Secure Transport. NSS/NSPR é um
provider substituível, não a identidade de PST, e a policy do Secure Principal
continua pertencendo ao Accelerator.

### Racional de ponte temporal

Esta capability pode futuramente ajudar clientes históricos a acessar
capacidades e protocolos atuais sem implementar localmente toda a evolução do
ecossistema externo. O Accelerator funciona, nesse sentido, como uma ponte
temporal entre capacidades distintas dos dois lados.

Esse racional de longevidade não promete compatibilidade eterna, não declara
suporte a uma plataforma histórica específica e não torna disponível hoje
qualquer fluxo externo.

## Exemplos de uso / configuração

Exemplo conceitual válido:

```text
PapinhoAccelerator Transport Security = REQUIRED
TLS_OFFLOAD capability                = DISABLED
```

Isso preserva o canal seguro Client–Accelerator e apenas desabilita o auxílio
TLS para conexões externas.

## Estado atual

Status geral: `concept`.

### Resumo por área

| Área | Estado | Observação |
|---|---|---|
| Nome conceitual | concept | `TLS_OFFLOAD`; nome/ID definitivo não congelado |
| Capability Negotiation | not-implemented | Framework e wire ainda ausentes |
| Processamento TLS externo | experimental-proof | Harness isolado comprova Accelerator → PST → google.com:443; sem fluxo de produção |
| External transport ownership | concept | Modelos client-owned e Accelerator-owned ainda não escolhidos |
| Network egress | experimental-proof | Somente harness explícito; autoridade e fluxo de produção continuam ausentes |
| Policy/autorização específica | not-implemented | Regras específicas pendentes |
| Backend | experimental-proof | PST v0.6.1/API 2.1/OpenSSL 3 no target validado; sem seleção de backend da capability de produção |
| Fallback | unknown | Sem decisão específica além de não enfraquecer segurança |

## Implementado

- somente a separação documental e arquitetural entre `TLS_OFFLOAD` e
  Transport Security;
- pin e aquisição reproduzível da release PST `v0.6.1`;
- boundary privada e reutilizável do contrato de consumo do SDK PST no build;
- prova vertical opt-in e isolada de TLS 1.3 outbound para `google.com:443`,
  com validação de cadeia, hostname e resposta HTTPS.

## Parcialmente implementado

- a prova exercita somente a mecânica PST/TLS outbound e não constitui a
  capability negociável nem um caminho servido ao cliente;
- a infraestrutura de configuração possui conceitos gerais de policy/egress,
  mas não configura `TLS_OFFLOAD`.

## Não implementado

- negociação;
- wire IDs/messages;
- TLS de conexões externas no servidor/caminho de produção;
- seleção e lifecycle do backend da capability de produção;
- proxy/network flow;
- policy e quotas específicas;
- fallback local ou remoto específico.

## Configuração

Não existe configuração específica de `TLS_OFFLOAD`. Opções futuras devem
convergir para o mesmo Configuration Model e não podem controlar Transport
Security por efeito colateral.

## Compatibilidade

O target factual `win32-x64-msvc-19.51-openssl3` da release PST `v0.6.1` foi
testado no harness. Isso não declara suporte implementado à capability geral.

## Limitações conhecidas

- conceito sem implementação de produção, com prova vertical experimental;
- composição concreta, ownership e lifecycle dos fluxos externos ainda não definidos;
- nenhum protocolo, API, backend, armazenamento ou UI congelado.

## Pendências

- [ ] Definir o contrato funcional da capability.
- [ ] Escolher e definir modelo(s) de ownership do transport externo.
- [ ] Definir contrato concreto de composição com network egress.
- [ ] Definir policy, autorização, quotas e destination handling.
- [ ] Definir fallback sem downgrade de segurança.
- [ ] Definir IDs/wire somente em uma fase de protocolo apropriada.
- [ ] Implementar e validar antes de declarar suporte.

## Ideias futuras

- execução TLS no Accelerator para clientes incapazes de usar protocolos
  modernos diretamente;
- backends distintos por deployment.

Essas ideias não são decisões nem suporte atual.

## Questões em aberto

- Quais modelos de ownership serão suportados e quem possui cada conexão?
- Quais dados atravessam CONTROL/DATA?
- Quais destinos, protocolos e redirects são permitidos?
- Como DNS, proxy semantics, cancellation e quotas se compõem em cada modelo?
- PST ou outro backend poderá participar do modelo client-owned e por qual contrato?
- Existe fallback local e em quais condições?
- Quais backends e perfis externos serão suportados?

## ADRs relacionadas

- `PapinhoAccelerator/ADR-0001` — Secure Principal e Legacy Endpoint.
- `PapinhoAccelerator/ADR-0005` — Negociação de capabilities, disponibilidade de backend e autoridade de policy.
- `PapinhoAccelerator/ADR-0006` — Perfil de Transport Security e credenciais do Secure Principal.
- `PapinhoEngineering/ADR-0005` — Governança da documentação viva de capabilities.

## Código relevante

- `dependencies/papinho-secure-transport.txt`
- `tools/acquire-pst-release.ps1`
- `tests/pst_tls13_outbound_proof.c`
- `CMakeLists.txt` — targets opt-in de aquisição, build e provas reais.

## Testes / evidências

Em 2026-09-06, o harness opt-in comprovou com a API pública PST:

- DNS e TCP para `google.com:443`;
- TLS 1.3 (`PST_TLS_VERSION_1_3`, wire `0x0304`);
- certificado presente, cadeia do servidor e hostname validados;
- resposta HTTPS `HTTP/1.1 301 Moved Permanently` recebida;
- rejeição de `wrong-hostname.invalid` como `hostname mismatch`;
- zero tentativas de porta plaintext nos dois cenários.

O harness não pertence ao CTest regular porque depende de Internet e trust
store do host. A regressão offline permaneceu em 41/41 testes.

## Documentos relacionados

- `docs/capabilities.md`
- `docs/capabilities/network-egress.md`
- `docs/security-model.md`
- `docs/networking.md`
- `docs/phase3-transport-security-profile.md`

## Histórico de mudanças

| Data | Descrição |
|---|---|
| 2026-09-06 | Criação inicial do Capability Document após a migração das decisões arquiteturais para ADRs. |
| 2026-09-06 | Registrado o racional de ponte temporal, as duas relações de segurança independentes e a separação de autoridade entre TLS offload e network egress. |
| 2026-09-06 | Preservados, sem escolha arquitetural, os modelos conceituais client-owned e Accelerator-owned para o transport externo. |
| 2026-09-06 | Registrada a prova vertical experimental PST v0.4.0/TLS 1.3 outbound, sem promover a capability geral a implementada. |
| 2026-09-07 | Centralizado o contrato privado de consumo do SDK PST, sem composição runtime ou promoção da capability. |
| 2026-09-11 | Migrado o pin para PST 0.6.0/API 2.1 e revalidada a boundary; a integração de readiness da Phase 3.B4 pertence a Transport Security e não promove `TLS_OFFLOAD`. |
