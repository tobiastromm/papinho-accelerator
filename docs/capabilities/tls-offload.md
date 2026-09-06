---
capability: tls-offload
title: TLS Offload para conexões externas
status: concept
last-updated: 2026-09-06
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

### Duas conexões e relações de segurança independentes

No cenário futuro em que o Accelerator seja autorizado a abrir uma conexão
externa e `TLS_OFFLOAD` esteja efetivo, existem duas conexões independentes:

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
habilita processamento TLS. A composição acima também depende da policy da
Session, do destino e do protocolo, além de implementação futura.

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
| Processamento TLS externo | not-implemented | Nenhum fluxo externo implementado |
| Network egress | not-implemented | Autoridade separada; composição conceitual documentada |
| Policy/autorização específica | not-implemented | Regras específicas pendentes |
| Backend | not-implemented | Nenhuma biblioteca selecionada para esta capability |
| Fallback | unknown | Sem decisão específica além de não enfraquecer segurança |

## Implementado

- somente a separação documental e arquitetural entre `TLS_OFFLOAD` e
  Transport Security.

## Parcialmente implementado

- nenhuma execução parcial da capability;
- a infraestrutura de configuração possui conceitos gerais de policy/egress,
  mas não configura `TLS_OFFLOAD`.

## Não implementado

- negociação;
- wire IDs/messages;
- TLS de conexões externas;
- backend;
- proxy/network flow;
- policy e quotas específicas;
- fallback local ou remoto específico.

## Configuração

Não existe configuração específica de `TLS_OFFLOAD`. Opções futuras devem
convergir para o mesmo Configuration Model e não podem controlar Transport
Security por efeito colateral.

## Compatibilidade

Nenhum target declara suporte implementado ou testado para esta capability.

## Limitações conhecidas

- conceito sem implementação;
- composição concreta, ownership e lifecycle dos fluxos externos ainda não definidos;
- nenhum protocolo, API, backend, armazenamento ou UI congelado.

## Pendências

- [ ] Definir o contrato funcional da capability.
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

- Quem abre e possui a conexão externa?
- Quais dados atravessam CONTROL/DATA?
- Quais destinos, protocolos e redirects são permitidos?
- Existe fallback local e em quais condições?
- Quais backends e perfis externos serão suportados?

## ADRs relacionadas

- `PapinhoAccelerator/ADR-0001` — Secure Principal e Legacy Endpoint.
- `PapinhoAccelerator/ADR-0005` — Negociação de capabilities, disponibilidade de backend e autoridade de policy.
- `PapinhoAccelerator/ADR-0006` — Perfil de Transport Security e credenciais do Secure Principal.
- `PapinhoEngineering/ADR-0005` — Governança da documentação viva de capabilities.

## Código relevante

Nenhum código implementa `TLS_OFFLOAD` atualmente.

## Testes / evidências

Não existem testes funcionais da capability. As auditorias documentais e a
ausência de implementação no source tree sustentam o status `concept`.

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
