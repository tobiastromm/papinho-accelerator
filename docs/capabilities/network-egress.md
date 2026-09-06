---
capability: network-egress
title: Network Egress
status: concept
last-updated: 2026-09-06
scope: project
---

# Network Egress

## Objetivo

Documentar a autoridade/capability futura que permitirá controlar se o
Accelerator pode abrir conexões externas em nome de um cliente, sem confundir
compute remoto com permissão para usar a rede ou o endereço público do servidor.

## Escopo

### Inclui

- distinção entre egress realizado pelo cliente e pelo Accelerator;
- configuração, policy e autorização explícitas;
- validação futura de destinos, protocolos, portas e redirects;
- limites, quotas, observabilidade e cleanup futuros.

### Não inclui

- implementação atual de proxy ou conexão externa;
- concessão automática por outra capability;
- Transport Security Client–Accelerator;
- definição de DNS, HTTP ou TLS offload completos;
- sintaxe wire ou API ainda não decidida.

## Comportamento esperado

```text
remote compute != network egress permission
```

Egress pelo Accelerator exige policy/autorização explícita. Backend presente,
pedido do cliente ou capability de compute autorizada não concedem egress por
si sós.

São fluxos distintos:

```text
NETWORK_EGRESS_CLIENT
→ cliente abre a conexão externa

NETWORK_EGRESS_ACCELERATOR
→ Accelerator abre a conexão externa
```

A distinção descreve a origem real da conexão e da identidade de rede; não
congela IDs de capability ou wire format.

## Arquitetura / fluxo

Fluxo conceitual futuro:

```text
authenticated/authorized request
        ↓
server configuration + principal policy + capability policy
        ↓
destination/protocol/port validation
        ↓
quota/resource admission
        ↓
external connection owned by an egress component
```

Falha ou ambiguidade deve negar egress. A conexão externa não pertence
implicitamente a um Compute Backend.

## Exemplos de uso / configuração

O Configuration Model já contém o booleano `allow_network_egress`. Ele é uma
entrada administrativa parcial, não prova que egress esteja implementado.

Exemplo conceitual:

```text
allow_network_egress = FALSE
remote compute        = AVAILABLE
```

Essa combinação é válida.

## Estado atual

Status geral: `concept`.

### Resumo por área

| Área | Estado | Observação |
|---|---|---|
| Configuration Model | partial | `allow_network_egress` existe e é validado/copiado |
| CLI source | partial | opção existente compõe o campo de configuração |
| Policy por Principal/Session | not-implemented | autenticação/autorização ainda ausentes |
| External connection | experimental-proof | harness isolado abre somente google.com:443; servidor não abre conexões externas |
| Destination validation | not-implemented | sem regras concretas |
| DNS/redirect handling | not-implemented | sem resolver/seguir destinos |
| Quotas/observabilidade | not-implemented | sem consumo de egress |

## Implementado

- campo portátil `allow_network_egress` no Server Configuration Model;
- initializer, validation/copy e parsing CLI associados;
- testes da composição/configuração desse campo.
- harness opt-in isolado de DNS/TCP para `google.com:443`, usado somente pela
  prova PST/TLS 1.3.

Esses itens implementam configuração parcial, não a capability de egress.

## Parcialmente implementado

- autoridade administrativa booleana de alto nível.

## Não implementado

- conexão externa no servidor/caminho de produção;
- proxy;
- policy por Principal/Session;
- autorização de destino;
- resolução DNS;
- redirects;
- quotas e contabilização;
- integração com `TLS_OFFLOAD`;
- mensagens/IDs de negociação.

## Configuração

`allow_network_egress` é `FALSE` por padrão. A existência do campo não concede
egress sem implementação, policy e autorização adicionais.

CLI e futuras interfaces devem convergir para o mesmo Configuration Model,
conforme a governança transversal de configuração.

## Compatibilidade

Nenhum target declara network egress funcional. O harness foi testado no host
do target PST `win32-x64-msvc-19.51-openssl3`; isso não constitui backend de
egress nem suporte da capability.

## Limitações conhecidas

- somente configuração parcial;
- nenhuma conexão externa;
- nenhum contrato de destination policy;
- nenhum modelo de quotas ou observabilidade de egress.

## Pendências

- [ ] Definir policy e autorização por Principal/Session.
- [ ] Definir validação de hostname/endereço e porta/protocolo.
- [ ] Tratar localhost, loopback, private, link-local e redes LAN.
- [ ] Tratar DNS rebinding e mudanças de resolução.
- [ ] Definir política de redirects e revalidação de cada destino.
- [ ] Definir quotas de conexões, bytes, tempo, bandwidth e concorrência.
- [ ] Definir ownership, cancellation e cleanup.
- [ ] Definir observabilidade sem expor dados sensíveis.
- [ ] Definir relação com `TLS_OFFLOAD` sem concessão implícita.

## Ideias futuras

- policies por Principal, capability, destination class ou deployment;
- backends diferentes para proxying ou transports especializados.

Essas possibilidades não estão decididas.

## Questões em aberto

- Quais protocolos externos serão permitidos?
- Como nomes e endereços serão normalizados e revalidados?
- Redirects serão permitidos em quais condições?
- Como policy será composta com capability negotiation?
- Qual componente possuirá a conexão externa?

## ADRs relacionadas

- `PapinhoAccelerator/ADR-0002` — Core portável entre deployments e backends substituíveis.
- `PapinhoAccelerator/ADR-0005` — Negociação de capabilities, disponibilidade de backend e autoridade de policy.
- `PapinhoAccelerator/ADR-0006` — autenticação e autorização permanecem separadas no Secure Principal.
- `PapinhoEngineering/ADR-0004` — Modelo compartilhado de configuração e separação entre Core e Frontends.
- `PapinhoEngineering/ADR-0005` — Governança da documentação viva de capabilities.

## Código relevante

- `apps/server/server_config.h`
- `apps/server/server_config.c`
- `apps/server/server_cli.c`
- `tests/pst_tls13_outbound_proof.c` — prova isolada, fora do servidor.

## Testes / evidências

- `tests/server_config_test.c` — initializer, validação e cópia do campo.
- `tests/server_cli_test.c` — parsing e default da opção CLI.
- `tests/server_network_test.c` — composição aceita a configuração sem implementar egress.

Code/Tests evidenciam somente a configuração parcial descrita acima.
O harness opt-in evidenciou DNS e TCP para o destino fixo `google.com:443`,
sem aceitar destino do cliente, sem proxy e sem integrar o CTest regular.

## Documentos relacionados

- `docs/capabilities.md`
- `docs/capabilities/tls-offload.md`
- `docs/networking.md`
- `docs/security-model.md`

## Histórico de mudanças

| Data | Descrição |
|---|---|
| 2026-09-06 | Criação inicial do Capability Document com estado factual de configuração parcial e egress não implementado. |
| 2026-09-06 | Registrada prova outbound isolada para destino fixo, sem promover network egress de produção a implementado. |
