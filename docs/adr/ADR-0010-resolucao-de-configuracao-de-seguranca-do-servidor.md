---
adr: ADR-0010
title: Referência opaca e resolução de configuração de segurança do servidor
status: accepted
decision-date: 2026-09-12
last-revised: 2026-09-12
revision: 1
scope: project
decision-makers:
  - Tobias Tromm
supersedes: []
superseded-by: []
---

# ADR-0010 — Referência opaca e resolução de configuração de segurança do servidor

## Contexto

O ADR-0009 exige que cada listener `SECURE_PRINCIPAL` referencie configuração
de segurança, sem inserir secrets no modelo de listener. O projeto precisa
resolver essa referência para Local Identity, Peer Trust, Principal Resolver e
Authorization Provider sem transformar filesystem, Windows Certificate Store,
Registry, PST ou outro mecanismo atual na identidade permanente da
configuração.

## Forças da decisão

- Shared Configuration Model independente das Configuration Sources;
- portabilidade entre plataformas e backends;
- secrets fora de CLI, listener model, logs e PACC;
- resolução fail-closed antes de RUN;
- ownership e cleanup explícitos;
- testabilidade com provisionamento programático;
- evolução futura de storage sem alterar listeners.

## Alternativas consideradas

### Alternativa A — Arquivos como modelo canônico

É simples inicialmente, mas acopla o domínio a paths, formatos, permissões,
password handling e filesystem.

### Alternativa B — Windows Certificate Store como modelo canônico

Integra-se ao Windows, mas não é portável e faria semântica Win32 atravessar o
Shared Configuration Model.

### Alternativa C — Referência opaca e resolver controlado pela aplicação

O listener mantém uma referência estável e não secreta. Um resolver da
composition root produz inputs normalizados e com lifetime explícito.

## Decisão

Cada listener `SECURE_PRINCIPAL` possui `security_configuration_ref`
explicitamente resolvida no Shared Configuration Model. A referência é
bounded, copiável, comparável, não secreta, platform-neutral e não codifica
filesystem, certificate store, Registry, PST ou backend TLS.

```text
Configuration Source
    → Shared Configuration Model
    → security_configuration_ref
    → application-owned Security Configuration Resolver
    → Normalized Server Security Configuration
       ├── Local Identity
       ├── Peer Trust
       ├── Principal Resolver
       └── Authorization Provider
    → Security Composition
```

O resolver opera durante bootstrap, não no hot path de conexões. Todos os
listeners Secure são resolvidos e validados antes da publicação dos listeners
e antes de RUN. `NOT_FOUND`, configuração inválida, indisponibilidade e erro
interno falham fechado e nunca selecionam `LEGACY_ENDPOINT`.

Os inputs resolvidos formam snapshot com lifetime e cleanup explícitos. O
server/security composition os possui ou retém até todas as conexões
dependentes terminarem. Não são aceitos borrowed pointers cuja origem possa
desaparecer durante esse lifetime.

Listeners Secure distintos podem compartilhar a mesma referência. Deduplicar
snapshots ou criar snapshots separados é detalhe de implementação, desde que
ownership permaneça correto.

`LEGACY_ENDPOINT` não usa PST nem configuração Secure Principal. Uma referência
Secure presente nesse perfil é configuração incoerente e deve ser rejeitada.

A Phase 3.E pode usar resolver programático e PKI autônoma exclusivamente em
testes. O executável produtivo sem resolver/configuração operacional válida
falha antes de RUN; ele não usa fixtures, não gera identidade efêmera e não
executa plaintext por omissão.

## Justificativa

A referência opaca preserva uma semântica única e portável, enquanto adapters
operacionais podem evoluir independentemente. A resolução antecipada torna a
ausência de credenciais um erro de configuração, não uma falha tardia após
aceitar tráfego.

## Consequências

### Positivas

- sources e storages permanecem substituíveis;
- secrets não contaminam configuração genérica;
- testes exercitam o caminho real por injeção explícita;
- listeners são publicados somente após bootstrap seguro completo.

### Negativas / trade-offs

- exige boundary, outcomes e lifecycle próprios;
- uma Configuration Source operacional ainda precisará ser implementada;
- referências inválidas impedem startup.

### Neutras ou operacionais

- hot reload não é exigido;
- snapshots permanecem estáveis durante seu runtime lifetime;
- nenhuma sintaxe administrativa de certificados é definida.

## Regras derivadas

1. `security_configuration_ref` é referência, não secret nem source.
2. Somente a composition root conhece o resolver.
3. Listener, PACC, Session, Channel e processors não conhecem storage concreto.
4. Resolução ocorre antes da publicação de listeners e de RUN.
5. Falha de resolução Secure é fail-closed.
6. Nenhuma falha seleciona Legacy ou plaintext.
7. Inputs resolvidos possuem ownership e cleanup explícitos.
8. Principal Resolver e Authorization Provider fazem parte das dependências
   normalizadas, sem serem embutidos no listener model.
9. Test fixtures não são defaults nem artefatos produtivos.
10. O executável não finge possuir configuração produtiva quando ela falta.

## Impacto

### Código

É necessária uma boundary portátil privada de resolver e um aggregate de
configuração resolvida reutilizando as APIs das Phases 3.B–3.D.

### Build e toolchains

Nenhum provider, source ou storage concreto é imposto.

### Documentação

Documentar separadamente referência, resolução, sources disponíveis e estado
factual do executável.

### Compatibilidade

Filesystem, Windows Store, Registry, firmware, HSM e provisionamento
programático podem ser adapters futuros sem alterar o listener model.

### Testes

Cobrir resolução, falhas, validação antes de RUN, sharing, ownership, cleanup e
ausência de fixtures no pacote produtivo.

## Verificação de conformidade

- generic server config não contém secret bytes ou provider handles;
- CLI não recebe private key/password/PIN;
- resolução ocorre fora do hot path;
- listeners não são publicados quando resolução falha;
- PST permanece atrás da security composition;
- cleanup ocorre após todas as conexões dependentes;
- production package não contém test PKI.

## Relações

### ADRs relacionadas

- `PapinhoAccelerator/ADR-0006` — Perfil de Transport Security e credenciais.
- `PapinhoAccelerator/ADR-0009` — Perfil de transporte explícito por listener.
- `PapinhoEngineering/ADR-0004` — Shared Configuration Model.
- `PapinhoEngineering/ADR-0009` — Identidade, autenticação, trust e nome do peer.

### Capability Documents relacionadas

Nenhuma: Transport Security não é capability negociável.

### Documentos relacionados

- `docs/security-model.md`
- `docs/phase3-authentication-authorization.md`
- `docs/integration/papinho-secure-transport-0.6.1.md`

## Histórico de revisões

| Revisão | Data | Descrição |
|---:|---|---|
| 1 | 2026-09-12 | Aprovação da referência opaca e do resolver de configuração de segurança controlado pela aplicação. |
