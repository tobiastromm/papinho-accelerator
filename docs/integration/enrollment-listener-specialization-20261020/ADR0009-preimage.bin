---
adr: ADR-0009
title: Perfil de transporte explícito por listener
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

# ADR-0009 — Perfil de transporte explícito por listener

## Contexto

O ADR-0001 separa `SECURE_PRINCIPAL` e `LEGACY_ENDPOINT`, proíbe downgrade e
determina que a seleção ocorra por listener/configuração. Ele não congelou a
forma concreta do Shared Configuration Model.

O modelo anterior possuía um único `control_port` e uma única seleção de bind,
sem representar qual pipeline cada listener executava. Integrar Transport
Security ao servidor exige que essa escolha seja inequívoca antes de aceitar
conexões, sem sniffing, herança ambígua ou fallback causado por falha TLS.

## Forças da decisão

- fail-closed e resistência a downgrade;
- Shared Configuration Model independente de CLI, GUI e storage;
- coexistência futura de endpoints Secure e Legacy;
- múltiplas interfaces e portas;
- validação determinística de conflitos;
- perfil imutável durante o lifetime do listener;
- separação entre configuração do listener e material secreto.

## Alternativas consideradas

### Alternativa A — Perfil global/default com override por listener

**Vantagens**

- configuração compacta quando listeners são homogêneos.

**Desvantagens / trade-offs**

- cria herança e precedência;
- torna menos evidente se um valor foi resolvido ou herdado;
- aumenta o risco de interpretações diferentes entre Configuration Sources.

### Alternativa B — Perfil explícito em cada listener

**Vantagens**

- pipeline factual e inequívoco por listener;
- não depende de ausência, herança ou autodetecção;
- permite Secure e Legacy em endpoints distintos;
- mapeia diretamente o listener para seu accept pipeline.

**Desvantagens / trade-offs**

- exige migração explícita do modelo anterior;
- repete o perfil em configurações com muitos listeners homogêneos.

### Alternativa C — Coleções tipadas separadas

Manter `secure_listeners[]` e `legacy_listeners[]`.

**Vantagens**

- separação visual forte.

**Desvantagens / trade-offs**

- duplica o modelo de listener e sua validação;
- novos perfis exigiriam novas coleções;
- dificulta processamento uniforme e evolução do modelo.

## Decisão

O Shared Configuration Model representa listeners como uma coleção. Cada
listener normalizado contém explicitamente:

```text
Listener
├── bind intent / bind selection
├── port
├── transport profile
└── security configuration reference, quando aplicável
```

Os perfis iniciais são:

```text
SECURE_PRINCIPAL
LEGACY_ENDPOINT
```

Não existe perfil runtime `AUTO`, `DETECT`, `TRY_SECURE_THEN_LEGACY` ou
`UNSPECIFIED`. Uma Configuration Source pode apresentar Secure Principal como
default ao usuário, mas deve produzir um valor explicitamente resolvido no
modelo normalizado.

O perfil é fixado antes da publicação do listener e permanece imutável durante
seu lifetime. Alterá-lo exige reconfiguração e recriação explícitas.

`SECURE_PRINCIPAL` é o perfil normal de produção. Falhas de TLS, mTLS, trust,
ALPN, autenticação ou autorização encerram somente a conexão candidata e nunca
selecionam `LEGACY_ENDPOINT` nem entregam bytes ao classifier plaintext.

`LEGACY_ENDPOINT` exige opt-in administrativo explícito, não possui strong
cryptographic identity e nunca é inferido por tráfego, sniffing ou falha do
perfil Secure.

Listeners dos dois perfis podem coexistir em endpoints efetivos distintos. A
validação deve rejeitar conflitos de bind.

Um listener Secure referencia uma configuração de segurança normalizada. Essa
referência não contém private key raw, senha, trust DER nem define a
Configuration Source. A escolha de filesystem, certificate store, Registry,
formato, secret storage e enrollment permanece decisão separada.

## Justificativa

O perfil explícito no próprio listener torna a seleção do pipeline uma
propriedade factual do recurso que aceita a conexão. Isso implementa a
separação e a regra de downgrade do ADR-0001 sem introduzir herança implícita ou
coleções paralelas difíceis de evoluir.

## Consequências

### Positivas

- não existe ambiguidade entre listener Secure e Legacy;
- falha Secure não pode reinterpretar o listener;
- CLI, GUI, arquivos e APIs convergem para o mesmo modelo;
- múltiplos perfis e endpoints podem coexistir;
- configuração pode ser validada antes de entrar em RUN.

### Negativas / trade-offs

- a configuração anterior precisa de migração explícita;
- cada listener precisa declarar seu perfil;
- o modelo passa a representar uma coleção, mesmo em instalações simples.

### Neutras ou operacionais

- este ADR não implementa o Legacy Endpoint;
- este ADR não escolhe uma Configuration Source de credenciais;
- testes históricos podem continuar instanciando componentes plaintext
  isolados sem definir o default produtivo.

## Regras derivadas

1. Todo listener normalizado possui transport profile explícito e válido.
2. A seleção do profile ocorre antes do accept pipeline.
3. O profile não muda durante o lifetime do listener.
4. `SECURE_PRINCIPAL` e `LEGACY_ENDPOINT` são os perfis iniciais.
5. Não existe autodetecção, sniffing ou fallback entre perfis.
6. `SECURE_PRINCIPAL` é o perfil normal de produção.
7. `LEGACY_ENDPOINT` exige configuração administrativa explícita.
8. Configuração antiga sem profile não vira Legacy implicitamente.
9. Falta de configuração de segurança requerida impede o servidor de entrar em
   RUN.
10. Listeners de perfis diferentes podem coexistir somente em endpoints sem
    conflito.
11. O listener contém referência de segurança, não secrets ou trust material
    bruto.
12. CLI e GUI são Configuration Sources, não modelos concorrentes.

## Impacto

### Código

O Server Configuration Model deve evoluir para uma coleção de listeners. A
configuração normalizada controla qual accept pipeline é usado por cada
listener.

### Build e toolchains

Nenhum target, backend ou toolchain adicional é imposto.

### Documentação

Documentos de arquitetura, segurança, configuração e operação devem distinguir
explicitamente o profile de cada listener.

### Compatibilidade

Não existe conversão silenciosa do modelo antigo para plaintext Legacy. Uma
migração deve resolver explicitamente o profile e falhar fechado quando faltar
configuração requerida.

### Testes

Devem cobrir validação de profile, ausência de profile, imutabilidade runtime,
conflitos de endpoint, isolamento Secure/Legacy e ausência de downgrade.

## Verificação de conformidade

- nenhum listener publicado possui profile ausente;
- accept routing usa somente o profile fixado no listener;
- falha TLS nunca chama o classifier plaintext;
- nenhuma Configuration Source mantém semântica paralela;
- configuração Secure sem referência válida falha antes de RUN;
- secrets não aparecem no listener model;
- endpoints conflitantes são rejeitados.

## Relações

### ADRs relacionadas

- `PapinhoAccelerator/ADR-0001` — Secure Principal e Legacy Endpoint.
- `PapinhoAccelerator/ADR-0003` — Connection, Session e Channel — ownership e lifecycle.
- `PapinhoAccelerator/ADR-0004` — Escalonamento de I/O não bloqueante, limitado e justo.
- `PapinhoAccelerator/ADR-0006` — Perfil de Transport Security e credenciais do Secure Principal.
- `PapinhoEngineering/ADR-0004` — Shared Configuration Model e separação Core/Frontends.

### Capability Documents relacionadas

Nenhuma: Transport Security e listener profile não são capabilities negociáveis.

### Documentos relacionados

- `docs/phase3-transport-profiles.md`
- `docs/security-model.md`
- `docs/architecture.md`

## Histórico de revisões

| Revisão | Data | Descrição |
|---:|---|---|
| 1 | 2026-09-12 | Decisão aprovada de transport profile explícito e imutável por listener. |
