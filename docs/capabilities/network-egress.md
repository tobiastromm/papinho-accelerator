---
capability: network-egress
last-updated: 2026-09-12
scope: project
status: concept
title: Network Egress
---

# Network Egress

## Objetivo

Documentar a autoridade/capability futura que permitirá controlar se o
PapinhoAccelerator pode abrir conexões externas em nome de um cliente,
sem confundir compute remoto com permissão para usar a rede ou o
endereço público do servidor.

Este documento é a **documentação viva factual da capability**.

Direções arquiteturais futuras ainda não congeladas por ADR ficam
separadas em:

``` text
docs/future/network-egress-policy.md
```

## Escopo

### Inclui

-   distinção entre egress realizado pelo cliente e pelo Accelerator;
-   configuração, policy e autorização explícitas;
-   validação futura de destinos, protocolos, portas e redirects;
-   limites, quotas, observabilidade e cleanup futuros;
-   relação com Capability Framework e Session Capability Snapshot.

### Não inclui

-   implementação atual de proxy ou conexão externa de produção;
-   concessão automática por outra capability;
-   Transport Security Client--Accelerator;
-   definição de DNS, HTTP ou TLS offload completos;
-   sintaxe wire ou API ainda não decidida;
-   firewall genérico do host/rede;
-   Web Proxy/Application Filtering;
-   TLS interception.

## Comportamento esperado

``` text
remote compute != network egress permission
```

Egress pelo Accelerator exige policy/autorização explícita. Backend
presente, pedido do cliente ou capability de compute autorizada não
concedem egress por si sós.

São fluxos distintos:

``` text
NETWORK_EGRESS_CLIENT
→ cliente abre a conexão externa

NETWORK_EGRESS_ACCELERATOR
→ Accelerator abre a conexão externa
```

A distinção descreve a origem real da conexão e da identidade de rede;
não congela IDs de capability ou wire format.

## Relação com a arquitetura de capabilities

A autoridade geral continua governada pelo ADR-0005:

``` text
SERVER_SUPPORTED
        ∩ SERVER_ENABLED
        ∩ PRINCIPAL_ALLOWED
        ∩ CLIENT_SUPPORTED
        ∩ CLIENT_PREFERENCE
        = EFFECTIVE_CONFIGURATION
```

Capability, backend e policy permanecem conceitos distintos.

O ADR-0011 define a identidade portátil de capabilities por key numérica
estável + semantic major version, mas **nenhum ID concreto para Network
Egress é atribuído por este documento**.

O ADR-0012 define o Session Capability Snapshot como upper bound
imutável.

Para Network Egress isso significa:

``` text
NETWORK_EGRESS ∈ snapshot
→ policy contextual pode negar imediatamente
→ policy contextual pode reabilitar depois
→ somente porque a capability já fazia parte do snapshot
```

Por outro lado:

``` text
NETWORK_EGRESS ∉ snapshot
→ policy não pode adicioná-la silenciosamente
→ nova Session
   OU
→ futura renegociação explícita
```

A renegociação futura permanece sem wire/protocolo definido.

## Arquitetura / fluxo atual conceitual

``` text
authenticated/authorized request
        ↓
server configuration + Principal policy + capability policy
        ↓
destination/protocol/port validation
        ↓
quota/resource admission
        ↓
external connection owned by an egress component
```

Falha ou ambiguidade deve negar egress. A conexão externa não pertence
implicitamente a um Compute Backend.

A arquitetura futura mais detalhada de Network Egress Policy e Egress
Profiles está preservada em:

``` text
docs/future/network-egress-policy.md
```

Esse documento futuro não declara implementação nem cria sozinho novas
decisões accepted.

## Exemplos de uso / configuração

O Configuration Model já contém o booleano `allow_network_egress`. Ele é
uma entrada administrativa parcial, não prova que egress esteja
implementado.

Exemplo conceitual:

``` text
allow_network_egress = FALSE
remote compute        = AVAILABLE
```

Essa combinação é válida.

## Estado atual

Status geral: `concept`.

### Resumo por área

  -------------------------------------------------------------------------------------------
  Área                     Estado                    Observação
  ------------------------ ------------------------- ----------------------------------------
  Capability Framework     implemented               Phase 4 fornece
                                                     key/registry/sets/snapshot/enforcement
                                                     genéricos; não implementa Network Egress

  Capability identity      implemented-generically   ADR-0011 define `(numeric ID, major)`;
                                                     nenhuma key concreta de Network Egress
                                                     foi atribuída

  Session Snapshot         implemented-generically   ADR-0012 define snapshot/enforcement;
                                                     Network Egress ainda não é workload de
                                                     produção

  Configuration Model      partial                   `allow_network_egress` existe e é
                                                     validado/copiado

  CLI source               partial                   opção existente compõe o campo de
                                                     configuração

  Policy por               framework-only            infraestrutura genérica Phase 4 existe;
  Principal/Session                                  policy concreta de Network Egress não

  External connection      experimental-proof        harness isolado abre somente
                                                     `google.com:443`; servidor não oferece
                                                     egress de produção

  Destination validation   not-implemented           sem regras concretas

  Egress Profiles          not-implemented           direção futura preservada em
                                                     `docs/future/network-egress-policy.md`

  DNS/redirect handling    not-implemented           sem contrato de produção

  Quotas/observabilidade   not-implemented           sem consumo real de egress

  Wire negotiation         not-implemented           Phase 4 não adicionou wire/workloads
  -------------------------------------------------------------------------------------------

## Implementado

-   campo portátil `allow_network_egress` no Server Configuration Model;
-   initializer, validation/copy e parsing CLI associados;
-   testes da composição/configuração desse campo;
-   harness opt-in isolado de DNS/TCP para `google.com:443`, usado
    somente pela prova PST/TLS 1.3;
-   Capability Framework genérico da Phase 4;
-   Capability Key, registry/sets bounded e identidade por `(id, major)`
    genéricos;
-   Session Capability Snapshot e enforcement contextual genéricos.

Esses itens **não implementam Network Egress de produção**.

## Parcialmente implementado

-   autoridade administrativa booleana de alto nível;
-   infraestrutura genérica de capability/policy sobre a qual Network
    Egress poderá ser construída futuramente.

## Não implementado

-   conexão externa no servidor/caminho de produção;
-   proxy;
-   destination policy concreta;
-   Network Egress Policy concreta;
-   Egress Profiles;
-   policy específica de egress por Principal/Session;
-   autorização de hostname/IP/subnet/porta/range;
-   resolução DNS de produção;
-   redirects;
-   quotas e contabilização de egress;
-   integração produtiva com `TLS_OFFLOAD`;
-   mensagens/IDs concretos de negociação para Network Egress;
-   Web Proxy/Application Filtering;
-   TLS interception.

## Configuração

`allow_network_egress` é `FALSE` por padrão. A existência do campo não
concede egress sem implementação, policy e autorização adicionais.

CLI e futuras interfaces devem convergir para o mesmo Configuration
Model, conforme a governança transversal de configuração.

O campo atual não substitui a futura Capability Key nem a policy
contextual.

## Compatibilidade

Nenhum target declara Network Egress funcional de produção.

O harness foi testado no host do target PST
`win32-x64-msvc-19.51-openssl3`; isso não constitui backend de egress
nem suporte da capability.

A Phase 4 é framework genérico e também não constitui suporte de Network
Egress.

## Limitações conhecidas

-   somente configuração parcial;
-   nenhuma conexão externa de produção;
-   nenhum contrato congelado de destination policy;
-   nenhum Egress Profile implementado;
-   nenhum modelo concreto de quotas/observabilidade de egress;
-   nenhum wire negotiation para a capability;
-   future architecture directions deliberadamente separadas da
    factualidade deste Capability Document.

## Pendências

-   [ ] Definir, na fase apropriada, Network Egress Policy concreta.
-   [ ] Definir Egress Profiles e autoridade de seleção.
-   [ ] Definir validação de hostname/endereço/subnet e porta/range.
-   [ ] Tratar localhost, loopback, private, link-local e redes de
    management.
-   [ ] Tratar DNS rebinding e mudanças de resolução.
-   [ ] Definir política de redirects e revalidação de cada destino.
-   [ ] Definir quotas de conexões, bytes, tempo, bandwidth e
    concorrência.
-   [ ] Definir ownership, cancellation e cleanup da conexão externa.
-   [ ] Definir observabilidade sem expor dados sensíveis.
-   [ ] Definir relação produtiva com `TLS_OFFLOAD` sem concessão
    implícita.
-   [ ] Definir a futura Capability Key concreta somente quando a
    capability amadurecer.
-   [ ] Definir wire negotiation somente por specification/decisão
    apropriada.
-   [ ] Avaliar ADRs futuras indicadas em
    `docs/future/network-egress-policy.md`.

## Ideias futuras / direções preservadas

As direções aprovadas mas ainda não congeladas como implementação estão
em:

``` text
docs/future/network-egress-policy.md
```

Entre elas:

-   Network Egress Policy contextual;
-   Egress Profiles;
-   destination policy;
-   no-silent link fallback;
-   SSRF/proxy-abuse protections;
-   fronteira contra firewall genérico;
-   separação de Web Proxy/Application Filtering;
-   separação de TLS interception.

O documento futuro também identifica quais pontos merecem futura
avaliação para ADR e quais devem permanecer decisões abertas.

## Questões em aberto

-   Quais protocolos externos serão permitidos?
-   Como nomes e endereços serão normalizados e revalidados?
-   Qual será a semântica concreta de DNS e DNS rebinding?
-   Redirects serão permitidos em quais condições?
-   Como Egress Profiles serão persistidos/configurados?
-   Como routing/failover será implementado?
-   Qual componente possuirá a conexão externa?
-   Qual será a Capability Key concreta?
-   Como será o wire negotiation?
-   Como a futura renegociação explícita do ADR-0012 será representada
    no protocolo?

## ADRs relacionadas

-   `PapinhoAccelerator/ADR-0002` --- Core portável entre deployments e
    backends substituíveis.
-   `PapinhoAccelerator/ADR-0005` --- Negociação de capabilities,
    disponibilidade de backend e autoridade de policy.
-   `PapinhoAccelerator/ADR-0006` --- Perfil de Transport Security e
    credenciais do Secure Principal.
-   `PapinhoAccelerator/ADR-0011` --- Identidade de capability e modelo
    portátil limitado.
-   `PapinhoAccelerator/ADR-0012` --- Snapshot de capabilities da
    Session e enforcement contextual.
-   `PapinhoEngineering/ADR-0004` --- Modelo compartilhado de
    configuração e separação entre Core e Frontends.
-   `PapinhoEngineering/ADR-0005` --- Governança da documentação viva de
    capabilities.

## Código relevante

-   `apps/server/server_config.h`
-   `apps/server/server_config.c`
-   `apps/server/server_cli.c`
-   `src/capability/`
-   `tests/pst_tls13_outbound_proof.c` --- prova isolada, fora do
    servidor.

## Testes / evidências

-   `tests/server_config_test.c` --- initializer, validação e cópia do
    campo;
-   `tests/server_cli_test.c` --- parsing e default da opção CLI;
-   `tests/server_network_test.c` --- composição aceita a configuração
    sem implementar egress;
-   testes da Phase 4 --- evidenciam o framework genérico de capability,
    snapshot e enforcement, não Network Egress de produção.

Code/Tests evidenciam somente os estados descritos acima.

O harness opt-in evidenciou DNS e TCP para destino fixo
`google.com:443`, sem aceitar destino arbitrário do cliente, sem proxy e
sem integrar um caminho Network Egress de produção.

## Documentos relacionados

-   `docs/capabilities.md`
-   `docs/capabilities/tls-offload.md`
-   `docs/future/network-egress-policy.md`
-   `docs/networking.md`
-   `docs/security-model.md`
-   `docs/phase4-capability-framework-policy-engine.md`

## Princípio

``` text
Capability Document
→ estado factual e vivo da capability

docs/future/
→ direção arquitetural adiada ainda não congelada como ADR

ADR
→ decisão durável aceita
```

## Histórico de mudanças

  -----------------------------------------------------------------------------
  Data                                Descrição
  ----------------------------------- -----------------------------------------
  2026-09-06                          Criação inicial do Capability Document
                                      com estado factual de configuração
                                      parcial e egress não implementado.

  2026-09-06                          Registrada prova outbound isolada para
                                      destino fixo, sem promover Network Egress
                                      de produção a implementado.

  2026-09-12                          Alinhado à Phase 4, ADR-0011/0012 e ao
                                      novo
                                      `docs/future/network-egress-policy.md`;
                                      preservado Network Egress como capability
                                      ainda não implementada em produção.
  -----------------------------------------------------------------------------
