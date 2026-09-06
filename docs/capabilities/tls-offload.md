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
- relação futura, ainda não desenhada, com network egress.

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
ADR-0005. A relação entre essa capability e quem abre a conexão externa ainda
precisa de desenho explícito: TLS offload não concede network egress.

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
| Network egress | not-implemented | Relação ainda precisa de desenho |
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
- dependência funcional de fluxos externos ainda não definida;
- nenhum protocolo, API, backend, armazenamento ou UI congelado.

## Pendências

- [ ] Definir o contrato funcional da capability.
- [ ] Definir relação explícita com network egress.
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
