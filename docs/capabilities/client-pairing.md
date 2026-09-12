---
capability: client-pairing
title: Client Pairing e Enrollment
status: concept
last-updated: 2026-09-06
scope: project
---

# Client Pairing e Enrollment

## Objetivo

Registrar a experiência futura de pareamento e enrollment entre um cliente
Papinho, especialmente o PapinhoBrowser, e o PapinhoAccelerator. Para o usuário
comum, a experiência deve usar conceitos como **Parear com Accelerator**,
**Novo dispositivo**, **Autorizar**, **Recusar** e **Dispositivo autorizado**.

X.509, CSR, CA privada, certificate chain e provider TLS permanecem abaixo
dessa experiência. Este documento descreve uma capacidade futura de produto;
não define uma capability de processamento negociável nem congela protocolo,
API, armazenamento ou GUI.

## Escopo

### Inclui

- first-use trust verificável do Accelerator;
- enrollment de uma credencial individual de dispositivo;
- geração e ownership local da private key do cliente;
- aprovação ou recusa explícita do contexto de pairing;
- mapeamento posterior da credencial para Principal estável e policy;
- experiências conceituais para usuário final, administrador e enterprise;
- reconexão automática após enrollment bem-sucedido.

### Não inclui

- mensagens, IDs, payloads ou encoding PACC de pairing;
- GUI, CLI, API, formato de armazenamento ou fluxo de renewal congelados;
- implementação de CA, issuer, enrollment ou gerenciamento de dispositivos;
- external PSK como perfil atual;
- confiança baseada apenas em código, nome, IP ou proximidade de rede;
- geração ou armazenamento normal da private key do cliente pelo servidor;
- autorização irrestrita decorrente do enrollment;
- alteração do perfil criptográfico normativo do ADR-0006.

## Comportamento esperado

Regra fundamental:

```text
pairing code != server identity
```

O cliente não pode confiar automaticamente em um peer apenas porque conectou a
um endereço e recebeu um código. No primeiro pareamento, deve existir uma ação
explícita para verificar a identidade apresentada pelo Accelerator. Conforme
disponível, a experiência pode mostrar:

- nome de apresentação do servidor;
- network endpoint;
- identidade do servidor e trust domain;
- fingerprint SHA-256 do certificado;
- código de pairing/verificação;
- estado de trust;
- ação explícita de confirmação.

Nome e endpoint são presentation/diagnostic metadata, não autenticam servidor
ou cliente. O código também não é uma senha reutilizável nem identidade
standalone. Ele deve permanecer vinculado ao servidor/certificado esperado, à
fingerprint, ao pairing session/transcript atual, ao material público gerado
pelo cliente e ao propósito de enrollment. Os requisitos criptográficos
canônicos pertencem ao ADR-0006.

## Arquitetura / fluxo

### Primeiro pareamento

```text
PapinhoBrowser
    ↓
descobre ou recebe o endereço do Accelerator
    ↓
obtém a identidade apresentada
    ↓
usuário verifica identity / fingerprint / code
    ↓
cliente gera private key LOCALMENTE
    ↓
private key nunca sai do cliente
    ↓
client-generated public key / enrollment request
    ↓
Accelerator mostra solicitação pendente
    ↓
administrador Autoriza ou Recusa
    ↓
issuer autorizado emite/assina device credential
    ↓
credential mapeada para stable Principal + assigned policy
    ↓
cliente armazena private key, device credential
e expected trust anchor / server identity
```

A aprovação deve estar vinculada ao mesmo pairing context verificado pelo
cliente. O servidor recebe somente o material público necessário ao enrollment.

### Ownership da private key

```text
Client private key
    → generated on client
    → stored on client
    → NEVER transmitted to Accelerator
```

No fluxo normal, o Accelerator não gera nem armazena a private key do cliente.
Essa fronteira deriva do ADR-0006 e não depende do frontend usado.

### Enrollment e autorização

```text
approved enrollment != unrestricted authorization

credential / device
        ↓
stable Principal
        ↓
assigned policy
```

Uma credencial autenticada não concede tudo implicitamente. Policy poderá
controlar Session, DATA attachment, capabilities, network egress, quotas e
outros privilégios de forma separada.

### Conexões futuras

```text
PapinhoBrowser
      ↓ automatic mTLS with enrolled device credential
PapinhoAccelerator
```

O pairing não deve ser repetido a cada conexão. Novo pairing ou retrust pode
ser necessário após remoção da credencial, revocation, expiry ou falha de
renewal, mudança de server identity/trust domain ou ação explícita de
forget/unpair. A UX e as regras finais de renewal permanecem futuras.

## Experiências conceituais

Os exemplos a seguir são **possíveis implementações/UI**. Valores, texto,
layout e fluxo visual são ilustrativos e podem mudar sem alterar a semântica.

### PapinhoBrowser — usuário verifica o Accelerator

```text
┌────────────────────────────────────────────┐
│ Parear com PapinhoAccelerator              │
│                                            │
│ Servidor: Accelerator-Sala                 │
│ Endereço: 192.168.1.50                     │
│                                            │
│ Identidade apresentada                     │
│ SHA-256: AB:CD:EF:...                      │
│                                            │
│ Código de verificação                      │
│ 482 731                                    │
│                                            │
│ Confirme que estes dados correspondem      │
│ aos exibidos no PapinhoAccelerator.        │
│                                            │
│ [ Cancelar ]              [ Confirmar ]    │
└────────────────────────────────────────────┘
```

### PapinhoAccelerator — administrador decide

```text
PapinhoBrowser solicita enrollment
            ↓

┌────────────────────────────────────┐
│ Solicitação de novo dispositivo    │
│                                    │
│ Nome: Tobias-PC                    │
│ Endpoint: 192.168.1.35             │
│ Código: 482 731                    │
│ Fingerprint: AB:CD:...             │
│                                    │
│ [ Autorizar ]       [ Recusar ]    │
└────────────────────────────────────┘
```

Nome e IP ajudam apresentação e diagnóstico, mas não autenticam o cliente. A
identidade real resulta do enrollment, da credencial validada e do mapeamento
para Principal.

## Modos de operação conceituais

### End-user pairing

Para pequeno ambiente privado: o Browser verifica a identidade, o usuário
confirma e o restante do enrollment pode ser automatizado.

### Server-admin approval

O dispositivo aparece como pending; o administrador vê o contexto vinculado,
decide **Autorizar** ou **Recusar** e poderá atribuir Principal/policy.

### Enterprise enrollment

Provisioning, PKI ou management externo podem realizar enrollment sem GUI
interativa. Os detalhes permanecem futuros.

Os modos podem usar frontends diferentes, mas devem convergir para o mesmo
modelo de enrollment, trust, credencial, Principal e policy.

## Estados conceituais

Nomes úteis para raciocínio, sem congelar API ou wire:

```text
UNPAIRED
PAIRING
PENDING_APPROVAL
AUTHORIZED
REJECTED
REVOKED / EXPIRED
```

Os nomes e transições finais podem mudar. Um cliente não pareado não recebe
normal CONTROL Session, DATA attachment, capabilities ou network egress.
Ausência, ambiguidade ou falha de verificação deve falhar fechada.

`Recusar` encerra o pairing atual sem emitir credencial. Uma solicitação
rejeitada não é reaprovada automaticamente. Rate limiting, expiry e resistência
a replay devem respeitar o security profile. Blacklist, bloqueio permanente e
retry policy exigem decisão posterior.

## Estado atual

Status geral: `concept`.

### Resumo por área

| Área | Estado | Observação |
|---|---|---|
| UX de pairing | concept | Sem GUI ou textos finais congelados |
| First-use trust | concept | Requisitos governados pelo ADR-0006 |
| Enrollment | not-implemented | Sem issuer ou fluxo operacional |
| Device credential | not-implemented | Sem provisionamento ou armazenamento |
| Principal/policy mapping | partial | Runtime AuthN/AuthZ existe; enrollment e storage não |
| Reconexão mTLS | not-implemented | Transport Security server-side existe; Browser/enrollment não |
| Enterprise enrollment | concept | Modelo e frontend futuros |
| Wire/API | not-implemented | Nenhuma mensagem, ID, payload ou API definida |

## Implementado

- nenhuma parte funcional de client pairing/enrollment.

## Parcialmente implementado

- nenhuma implementação parcial foi identificada em código ou testes.

## Não implementado

- telas de cliente e administração;
- discovery orientado a pairing;
- verificação e binding do pairing context;
- geração/invocação de enrollment request;
- aprovação, recusa, emissão, armazenamento, renewal e revocation;
- mapeamento de credencial para Principal/policy;
- mensagens ou APIs de pairing;
- integração com PST ou com a Phase 3.B.

## Configuração

Nenhuma opção de configuração de pairing/enrollment existe. Fontes futuras
devem convergir para o Configuration Model compartilhado e preservar policy e
defaults fail-closed.

## Compatibilidade

Nenhum target, cliente ou frontend declara suporte implementado ou testado.
PapinhoBrowser é o exemplo principal de UX, não o único cliente possível.

## Limitações conhecidas

- conceito sem implementação;
- UX, wire, API, storage e operação do issuer indefinidos;
- nenhum lifecycle concreto de retry, renewal ou revocation;
- nenhuma integração enterprise definida.

## Pendências

- [ ] Definir threat model e ceremony operacional detalhados antes de implementar.
- [ ] Definir ownership, lifetime e persistência dos registros públicos.
- [ ] Definir issuer e fluxos end-user, admin e enterprise.
- [ ] Definir vínculo verificável do contexto e resistência a relay/replay.
- [ ] Definir rate limits, expiração e auditoria sem vazar material sensível.
- [ ] Definir renewal, revocation, forget/unpair e retrust.
- [ ] Definir wire/API somente em fase apropriada.
- [ ] Implementar e validar antes de declarar suporte.

## Ideias futuras

- frontend simples que esconda terminologia de PKI do usuário comum;
- automação enterprise por tooling de gestão externo;
- atribuição de policy durante aprovação administrativa.

Essas ideias não congelam UX, integração ou comportamento implementado.

## Questões em aberto

- Qual componente e autoridade operarão o issuer em cada deployment?
- Como o canal independente de verificação será apresentado em cada frontend?
- Como pedidos pendentes serão persistidos, expirados e auditados?
- Como renewal, revocation e mudança de trust domain serão apresentados?
- Quais operações administrativas serão separadas por policy?

## ADRs relacionadas

- `PapinhoAccelerator/ADR-0001` — Secure Principal e Legacy Endpoint.
- `PapinhoAccelerator/ADR-0005` — capability, backend e autoridade de policy.
- `PapinhoAccelerator/ADR-0006` — perfil de Transport Security, credenciais, first-use trust e pairing.
- `PapinhoEngineering/ADR-0005` — governança da documentação viva de capabilities.

## Código relevante

Nenhum código implementa client pairing/enrollment atualmente.

## Testes / evidências

Não existem testes funcionais de pairing/enrollment. A busca no source tree e
nos testes não encontrou implementação; o ADR-0006 e a documentação histórica
fornecem somente a baseline arquitetural/conceitual.

## Documentos relacionados

- `docs/security-model.md`
- `docs/phase3-transport-security-profile.md`
- `docs/phase3-security-architecture.md`
- `docs/architecture.md`

## Histórico de mudanças

| Data | Descrição |
|---|---|
| 2026-09-06 | Criação inicial do Capability Document para a experiência conceitual de client pairing/enrollment. |
