---
capability: client-pairing
title: Client Pairing e Enrollment
status: concept
last-updated: 2026-10-18
scope: project
---

# Client Pairing e Enrollment

## Consolidação de produção — 2026-10-18

O status `concept` descreve pairing/enrollment ainda não implementado, sem
rebaixar a importação granular já existente no Browser. Dois modos permanentes
são obrigatórios antes do fechamento integral Phase6:
[decisão explícita do owner](../adr/DECISION-production-client-provisioning-20261018.md)
e [seis gates OPEN](../phase6-network-tls-offload.md#production-client-provisioning--phase-6-gates).
A [matriz atual](../integration/production-client-provisioning-consolidation-20261018/RECONCILIATION.md)
separa contrato/mecanismo existente/lacuna. Nenhum pacote ou enrollment produtivo
foi implementado por esta atualização documental.

- MANUAL: preservar importação granular avançada; adicionar Export new client
  no admin e importação verificada/transacional de pacote no Browser. Versão,
  integridade, destinatário/servidor e limites obrigatórios; formato/issuer/CSR
  concretos e proteção de eventuais segredos ainda não congelados. Não há
  autorização para servidor exportar chave privada do cliente.
- AUTOMATIC: endereço, servidor verificável, confirmação por código e políticas
  AUTO_APPROVAL/MANUAL_APPROVAL. Chave normal no cliente; confirmação/AuthZ/
  expiry/anti-replay obrigatórios. Cerimônia, bootstrap anterior à primeira
  credencial e wire permanecem em gate próprio.
- OPEN dispensa login inicial para pedir enrollment. CLOSED no enrollment valida
  username/password antes de aprovação. Isso preserva ADR-0028 para o serviço
  geral, transporte/AuthN/AuthZ separados. Password nunca plaintext;
  pending/unapproved sem CONTROL/DATA/egress operacional.
- Ambos convergem no mesmo cadastro/issuer/Principal-policy/grants e no mesmo
  CredentialGeneration/Reference/store/leases. Add/approve/renew/revoke sem
  restart é obrigatório na mesma root PREPARE -> COMMIT -> CURRENT -> EPOCH.
  Entry point atual de primeiro commit dormente não é hot reload.
- Revoke persistente/deny novas conexões são obrigatórios; política/propagação
  sobre sessões ativas exige decisão concreta. Local unpair não revoga servidor.
  Preservar Model E, retenção e safe retirement.

### Mecanismos reutilizáveis e limites

Browser ADR-0007 e roadmap, seção Production Secure Provisioning (07/10),
qualificam Manual R1/current-host: import DER/raw Base64/PKCS8 não cifrado,
validator/store_prepare, commit ActiveConfiguration e leases/retirement. Não
qualificam NT4 real, pacote, issuer, pairing ou revoke remoto. Pontos reais:
src/papinho_credential_options.inc, papinho_credential_import.inc,
papinho_credential_store.h/.c, papinho_accelerator_config_generation.inc e
papinho_credential_lifetime.inc, somente lidos nesta tarefa.

No Accelerator, apps/server/server_security_source_win32.h/.c continua fonte
lab de credenciais/single leaf digest; não é cadastro multi-cliente nem issuer
de produção. Root e BODY0.5 existem, mas
server_configuration_root_transaction_win32.inc restringe prepare/publish ao
primeiro commit dormente, sem atualização live. APIs privadas não comprovam
CLI/GUI administrativa instalada. Ver
[handoff administrativo](../integration/mode-c-administrative-tls-input-binding-20261018/provisioning-handoff.md).

Limitações/pendências abaixo referem-se ao enrollment e não negam essas bases
parciais. A [policy futura](../future/network-egress-policy.md) permanece separada
da emissão/enrollment. Parâmetros concretos pendentes constam da matriz;
nenhum formato, quota ou protocolo é escolhido aqui.
CLIENT_ADMISSION=DISABLED; SITE_TLS_ADMISSION=DENY; ED11_ADMISSION=DENY.

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
| Reconexão mTLS | partial | Listener real e provisioning manual lab-only existem; pairing/enrollment e persistência não |
| Enterprise enrollment | concept | Modelo e frontend futuros |
| Wire/API | not-implemented | Nenhuma mensagem, ID, payload ou API definida |

## Implementado

- provisioning manual e descartável de PKI para laboratório, fora do checkout;
- listener real aceita uma credencial previamente matriculada por fingerprint
  através de uma Configuration Source lab-only.

Esses itens são infraestrutura de validação e não implementam client
pairing/enrollment.

## Parcialmente implementado

- reconexão mTLS é tecnicamente possível com credencial manualmente
  provisionada, sem UX, emissão, armazenamento ou lifecycle de enrollment.

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

Nenhuma opção de configuração de pairing/enrollment existe. A fonte lab-only
do listener Secure Principal não é uma fonte de pairing. Fontes futuras
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
- `docs/secure-principal-operation.md`

## Histórico de mudanças

| Data | Descrição |
|---|---|
| 2026-09-06 | Criação inicial do Capability Document para a experiência conceitual de client pairing/enrollment. |
| 2026-09-29 | Registrado o provisioning manual lab-only e explicitado que ele não constitui pairing/enrollment. |
| 2026-10-18 | Reconciliados modos permanentes/gates Phase6 e mecanismos Browser R1/root; pacote/enrollment/hot-update permanecem pendentes. |

## Backend CA privado — diagnóstico de 2026-10-19

O backend parcial `server_ca_key_win32` registra agora a primeira falha por
função, etapa e origem LOCAL_GUARD/NATIVE_NCRYPT, preservando o código original
e o ownership. A comparação sintética com a preimagem cobriu 19 cenários;
Debug/Release focais passaram sem warnings. Isso não implementa enrollment,
nem qualifica emissão nativa. A origem histórica de NTE_PERM e o estado da
chave diagnóstica anterior continuam UNKNOWN. Nenhuma chave foi criada ou
excluída nesta instrumentação. Ver [closeout focal](../integration/cng-failure-origin-instrumentation-20261019/closeout.md).
As decisões posteriores de issuer/custódia e perfil técnico constam em
[issuer e custódia](../adr/DECISION-internal-client-issuer-and-manual-custody-20261018.md)
e [perfil técnico](../adr/DECISION-internal-ca-technical-profile-20261018.md);
essas autoridades delimitam os trechos conceituais históricos acima.
### Diagnóstico de chave somente-leitura — 2026-10-19

O antigo `--native-key` foi removido do harness atual. O diagnóstico separado
exige nome/IDs/SIDs explícitos e só abre/valida/fecha a chave; criação/setters
ficam excluídos por compilação, sem exportação privada ou exclusão. Testes
focais são sintéticos; nenhuma operação nativa de chave foi executada. A chave
anterior permanece UNKNOWN. Ver [handoff de segurança/portabilidade](../integration/cng-diagnostic-safety-portability-20261019/safety-identity-portability-handoff.md).
### Núcleo de issuer portável — 2026-10-19

O núcleo comum privado passou a implementar estados, serial/reserva,
idempotência, consulta, revogação/renovação e retenção com interfaces de backend
substituíveis. Testes são offline/sintéticos, sem CA ou certificados reais.
Persistência na root real, publicação live e backend CNG permanecem pendentes;
isso não implementa pairing/enrollment nem libera admissão. Ver
[contrato de integração](../integration/portable-issuer-core-20261019/implementation-and-root-handoff.md).
### Persistência issuer na root canônica — 2026-10-19

Codec privado e adapter dormente integrados à única root; serial, histórico e revogação com fixtures foram persistidos em arquivos nativos isolados. A composição produtiva nega publicação ROOT06 e assinatura sem CURRENT/EPOCH. Recuperação é somente observacional; emissão real, CNG e publicação live continuam pendentes. Ver [closeout](../integration/issuer-root-persistence-20261019/closeout.md) e [handoff](../integration/issuer-root-persistence-20261019/implementation-and-live-root-handoff.md). Admissão permanece DENY.

### Lifetime de snapshots issuer — 2026-10-19

Três gerações de dados issuer podem coexistir nos slots protegidos da única root. Tickets inválidos, slot esgotado e retenção negam reuso. O guard dormente da aplicação e a negação de publicação foram preservados: Sessions operacionais entre revisões ainda não comprovadas; recuperação autoritativa exige checkpoint/fencing. Ver [closeout parcial](../integration/issuer-live-root-recovery-20261019/closeout.md). Nenhuma emissão/admissão foi habilitada.

### Persistência issuer pendente — 2026-10-19

A mesma root possui agora um pacote de I/O e um worker Windows sem autoridade operacional. Begin/poll não esperam pela persistência; Stop preserva buffers e handles até conclusão factual. CAS conflitante ou incerto não publica CURRENT nem reutiliza autoridade antiga. O contrato comum de resultados não inclui Win32/CNG e compila em C estrito. Debug/Release e regressões offline passaram; publicação live/Session, recuperação autoritativa, CNG e box/ISA continuam pendentes. Admissão permanece DENY. Ver [prova e handoff](../integration/issuer-live-transaction-safety-20261019/safety-proof-and-handoff.md).

### Session e publicação administrativa issuer — 2026-10-19

A integração canônica vincula cada Session à revisão admitida por pin e mantém R/I/A/P distintos. A publicação de metadados issuer usa o worker pendente existente, confirma commit, revalida a projeção operacional e publica CURRENT/A sem alterar P em issuer-only. O reaper preserva quiescência e retenção. Debug/Release e 46 regressões offline passaram; metadados de Sessions nos testes são REVOKED sintéticos, sem autenticação ou tráfego real. Censo com ABI outbound correto: 1.790.912 bytes fixos; reserva base 1.957.939, attachment fechado 1.962.035. Recuperação autoritativa, CNG e credenciais reais continuam pendentes; admissão DENY, box/ISA não qualificadas. Os parágrafos anteriores registram slices históricos. Ver [prova, limites e handoff](../integration/issuer-session-live-publication-20261019/integration-proof-and-handoff.md).
