---
adr: ADR-0005
title: Negociação de capabilities, disponibilidade de backend e autoridade de policy
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

# ADR-0005 — Negociação de capabilities, disponibilidade de backend e autoridade de policy

## Contexto

O PapinhoAccelerator foi concebido para oferecer capacidades independentes e extensíveis, como processamento de imagem, vídeo, áudio, layout, framebuffer e futuro `TLS_OFFLOAD`.

Nem toda instalação do Accelerator possuirá os mesmos backends, hardware, permissões, quotas ou configuração administrativa. Da mesma forma, clientes diferentes podem suportar subconjuntos distintos e solicitar preferências diferentes.

Por isso, três conceitos não podem ser confundidos:

```text
capability
backend availability
policy authority
```

Uma capability descreve uma função conceitual do produto.

Um backend determina se existe uma implementação capaz de executar essa função naquele deployment.

Policy/configuração/autorização determinam se a função pode ser concedida naquele contexto.

O simples fato de:

- o protocolo conhecer uma capability;
- um backend estar presente;
- o cliente solicitá-la;
- o cliente suportá-la;

não significa que ela esteja autorizada ou efetivamente ativa.

Também é necessário impedir que Capability Negotiation enfraqueça propriedades de infraestrutura, especialmente Transport Security.

## Forças da decisão

- deployments heterogêneos;
- backends opcionais;
- hardware futuro;
- autorização granular;
- quotas e resource policy;
- preferência do cliente;
- compatibilidade com clientes limitados;
- fallback local explícito;
- segurança fail-closed;
- independência entre capability conceitual e implementação concreta;
- separação entre compute e network egress;
- evolução/versionamento futuro das capabilities.

## Alternativas consideradas

### Alternativa A — Backend presente implica capability habilitada

Uma capability é oferecida sempre que existe implementação/backend disponível.

**Vantagens**

- negociação simples;
- pouca policy.

**Desvantagens / trade-offs**

- ignora configuração administrativa;
- ignora autorização por usuário/principal;
- ignora quotas;
- backend disponível vira autoridade indevida;
- dificulta isolamento entre clientes.

### Alternativa B — Pedido do cliente determina execução

O cliente anuncia suporte/preferência e o Accelerator atende quando tecnicamente possível.

**Vantagens**

- controle simples pelo cliente;
- pouca lógica de resolução.

**Desvantagens / trade-offs**

- cliente não confiável ganha autoridade sobre recursos;
- policy do servidor fica enfraquecida;
- pode conceder egress ou compute indevidamente;
- não funciona bem com quotas e permissões.

### Alternativa C — Effective capability resulta da interseção explícita

A capability só se torna efetiva quando suporte, disponibilidade, configuração, autorização e compatibilidade convergem.

**Vantagens**

- autoridade permanece no servidor/policy;
- backend não concede permissão;
- cliente não presume concessão;
- deployments parciais são naturais;
- permite quotas e parâmetros compatíveis;
- segurança de infraestrutura fica fora da negociação comum.

**Desvantagens / trade-offs**

- resolução é mais elaborada;
- exige estados/resultados explícitos;
- parâmetros e versões precisarão de contratos claros.

## Decisão

O PapinhoAccelerator adota uma resolução explícita de capabilities.

A baseline conceitual é:

```text
SERVER_SUPPORTED
        ∩ SERVER_ENABLED
        ∩ USER_ALLOWED
        ∩ CLIENT_SUPPORTED
        ∩ CLIENT_PREFERENCE
        = EFFECTIVE_CONFIGURATION
```

A interseção é avaliada por capability e, quando aplicável, por versões e parâmetros compatíveis.

Ausência, incompatibilidade ou decisão ambígua resulta em **não habilitação**, não em concessão implícita.

### Capability é identidade conceitual, não backend

Uma capability representa **o que** pode ser feito.

O backend representa **como** aquilo é executado.

Exemplo:

```text
VIDEO_DECODE
    ↓
CPU implementation
GPU implementation
library backend
FPGA
ASIC
future hardware backend
```

Trocar o backend não deve mudar a identidade conceitual da capability.

```text
capability != backend
```

### Backend availability não é autorização

`SERVER_SUPPORTED` pode derivar dos backends realmente disponíveis no deployment.

Isso responde:

> Existe implementação capaz de executar esta capability?

Não responde:

> Este cliente está autorizado a utilizá-la?

Portanto:

```text
backend present
    !=
capability authorized
```

### Configuração administrativa é autoridade separada

`SERVER_ENABLED` representa se o administrador habilitou a capability naquele servidor/deployment.

Uma capability tecnicamente suportada pode permanecer administrativamente desabilitada.

### Autorização é granular

`USER_ALLOWED` representa policy/autorização aplicável ao Principal/Session.

Autenticação responde:

```text
quem é?
```

Autorização responde:

```text
o que pode fazer?
```

Um Principal autenticado não recebe automaticamente todas as capabilities.

A policy pode permitir, por exemplo:

```text
IMAGE_DECODE = allowed
VIDEO_DECODE = allowed
NETWORK_EGRESS = denied
TLS_OFFLOAD = denied
```

### Cliente declara suporte, não autoridade

`CLIENT_SUPPORTED` informa o que o cliente consegue compreender/utilizar.

`CLIENT_PREFERENCE` informa o que ele deseja usar.

Nenhum dos dois concede autoridade.

O servidor devolve a configuração/resultado efetivo.

O cliente não deve inferir que recebeu uma capability apenas porque a solicitou.

### Local de execução é decisão distinta

Quando aplicável, uma capability pode ter execução:

```text
EXECUTION_LOCAL
EXECUTION_ACCELERATOR
```

A decisão pode ser independente por capability.

Exemplo:

```text
IMAGE_DECODE       → ACCELERATOR
VIDEO_DECODE       → ACCELERATOR
CSS_LAYOUT         → LOCAL
TLS_OFFLOAD        → DISABLED
```

Local de execução não é implementação concreta.

```text
execution location != backend implementation
```

### Fallback local é explícito e depende do cliente

Se execução remota não estiver disponível/autorizada, o cliente pode usar fallback local **somente se ele próprio declarar suporte a isso e a semântica da capability permitir**.

```text
remote denied/unavailable
        ↓
client supports local fallback?
        ├── yes → local execution may be selected
        └── no  → capability unavailable
```

Fallback não deve alterar silenciosamente propriedades de segurança.

### Compute não concede network egress

Executar trabalho remotamente no Accelerator não concede automaticamente o direito de utilizar o IP/rede do Accelerator para conexões externas.

```text
remote compute
        !=
network egress permission
```

Network egress possui policy própria.

### Transport Security não é capability negociável

Transport Security protege o hop PapinhoAccelerator Client–Server.

Ela é infraestrutura/policy de segurança e não uma capability de processamento normal.

```text
Transport Security != TLS_OFFLOAD
```

Capability Negotiation não pode:

- habilitar Transport Security quando policy exige outro estado;
- desabilitar Transport Security requerida;
- enfraquecer versão/propriedades obrigatórias;
- converter falha de segurança em fallback inseguro.

Uma configuração válida é:

```text
PapinhoAccelerator Transport Security = REQUIRED
TLS_OFFLOAD capability                = DISABLED
```

`TLS_OFFLOAD` refere-se a trabalho TLS para conexões externas do cliente e não à proteção do canal Client–Accelerator.

### Secure policy prevalece sobre preferência

Quando houver conflito:

```text
client preference
        vs
server security/policy
```

server security/policy prevalece.

A negociação não é um mecanismo de downgrade.

### Quotas e recursos são parte da concessão efetiva

Uma capability autorizada ainda pode estar sujeita a:

- CPU;
- RAM;
- bandwidth;
- streams simultâneos;
- jobs;
- tamanho/resolução;
- parâmetros específicos;
- capacidade do backend;
- carga atual.

Reserva e consumo devem ser contabilizados e liberados no cleanup.

Exceder limites deve produzir falha controlada no menor escopo seguro.

### Deployment parcial é válido

Conforme o ADR-0002 local, deployments podem oferecer subconjuntos.

Ausência de backend resulta em redução explícita de capabilities.

Isso não é falha arquitetural.

```text
deployment A → IMAGE + VIDEO
deployment B → IMAGE only
deployment C → COMPUTE backend subset
```

A negociação deve refletir o estado real.

### IDs e versões não são congelados por este ADR

Este ADR define semântica e autoridade.

Ele não congela:

- IDs numéricos de capabilities;
- wire messages;
- payload layout;
- versão inicial de cada capability;
- formato de parâmetros;
- mudança dinâmica durante Session.

Esses itens pertencem às specifications/decisões futuras correspondentes.

## Justificativa

O Accelerator precisa operar em ambientes onde capacidade técnica, política administrativa, autorização e preferência do cliente variam independentemente.

Se backend availability fosse tratada como autorização, adicionar hardware ou biblioteca poderia conceder funções sem intenção administrativa.

Se preferência do cliente fosse autoridade, um peer remoto poderia escolher recursos que não deveria possuir.

A interseção explícita mantém cada fonte de decisão em seu papel correto e produz uma Effective Configuration que pode ser auditada.

Separar capability de backend também permite que a mesma função seja executada por CPU, GPU, FPGA ou hardware futuro sem alterar o protocolo conceitual.

## Consequências

### Positivas

- autorização permanece separada de suporte técnico;
- backends podem ser opcionais;
- clientes limitados podem negociar subconjuntos;
- deployments parciais são suportados naturalmente;
- policy pode negar capabilities específicas;
- network egress permanece independente de compute;
- Transport Security não pode ser enfraquecida por negociação;
- implementação concreta pode evoluir sem renomear capability.

### Negativas / trade-offs

- negociação exige resolução de múltiplas dimensões;
- erros/resultados precisam distinguir unsupported, disabled e denied;
- quotas e parâmetros adicionam complexidade;
- capabilities versionadas precisarão de specifications próprias.

### Neutras ou operacionais

- os nomes atuais de capabilities continuam conceituais enquanto IDs não forem congelados;
- mudanças de capability durante uma Session permanecem decisão futura;
- nem toda capability precisa suportar fallback local;
- nem todo backend precisa existir em todo deployment.

## Regras derivadas

1. Capability e backend são conceitos distintos.
2. Backend disponível não concede autorização.
3. Capability suportada pode estar administrativamente desabilitada.
4. Cliente declara suporte/preferência, não autoridade.
5. Servidor determina e retorna o resultado efetivo.
6. Ausência/incompatibilidade/ambiguidade não habilita capability implicitamente.
7. Autenticação não concede automaticamente todas as capabilities.
8. Autorização deve poder ser granular.
9. Local de execução é distinto de backend de implementação.
10. Fallback local depende de suporte explícito do cliente.
11. Fallback nunca enfraquece propriedade de segurança.
12. Remote compute não concede network egress.
13. Transport Security não é capability negociável.
14. `TLS_OFFLOAD` é distinto de Transport Security.
15. Server security/policy prevalece sobre client preference.
16. Quotas/resource limits podem restringir uma capability autorizada.
17. Deployments podem anunciar subconjuntos factuais.
18. Capability IDs/wire formats não são definidos por este ADR.

## Impacto

### Código

O futuro Capability Framework deve manter separadas:

- discovery/backend support;
- configuração administrativa;
- autorização;
- client support;
- client preference;
- effective result.

Backends não devem modificar policy diretamente.

### Build e toolchains

Builds diferentes podem incluir backends diferentes.

Isso pode alterar `SERVER_SUPPORTED` sem alterar a identidade das capabilities.

### Documentação

`docs/capabilities.md` permanece como índice/visão geral conceitual.

Capability Documents específicos podem ser criados conforme capabilities individuais amadureçam.

Specifications futuras devem definir IDs/wire format sem redefinir a autoridade estabelecida aqui.

### Compatibilidade

Clientes e deployments podem suportar subconjuntos diferentes.

A negociação deve tratar isso explicitamente.

### Testes

Testes futuros devem cobrir combinações como:

- supported + enabled + allowed + client-supported + preferred → effective;
- backend ausente → unavailable;
- administratively disabled → not effective;
- unauthorized → denied;
- client unsupported → not effective;
- client preference absent → comportamento conforme regra da capability;
- remote unavailable + local supported → fallback explícito quando permitido;
- Transport Security requerida permanece independente de `TLS_OFFLOAD`.

## Verificação de conformidade

Pode-se verificar se:

- backend presence não concede capability sozinho;
- client request não concede capability sozinho;
- authorization é consultada separadamente;
- resultado efetivo é explícito;
- network egress não é inferido de compute;
- `TLS_OFFLOAD` não controla Transport Security;
- falha de negociação não produz downgrade de segurança;
- deployment anuncia apenas capabilities realmente suportadas;
- código não confunde capability ID com backend implementation.

## Relações

### ADRs relacionadas

- `PapinhoAccelerator/ADR-0001` — Secure Principal e Legacy Endpoint.
- `PapinhoAccelerator/ADR-0002` — Core portável entre deployments e backends substituíveis.
- `PapinhoAccelerator/ADR-0003` — Connection, Session e Channel — ownership e lifecycle.
- `PapinhoEngineering/ADR-0004` — Modelo compartilhado de configuração, fonte única de verdade e separação entre Core e Frontends.
- `PapinhoEngineering/ADR-0007` — Fronteiras portáveis entre core, plataforma e backends substituíveis.

### Capability Documents relacionadas

Capability Documents específicos ainda não são obrigatórios nesta fase.

### Documentos relacionados

- `docs/capabilities.md`
- `docs/architecture.md`
- `docs/protocol-overview.md`
- `docs/phase2-transport-session-design.md`
- `docs/phase3-security-architecture.md`

## Princípio

**Capability diz o que pode ser feito; backend diz como pode ser feito; policy decide se pode ser feito aqui e agora.**

E:

```text
supported != enabled != authorized != requested != effective
```

## Histórico de revisões

| Revisão | Data | Descrição |
|---:|---|---|
| 1 | 2026-09-06 | Formalização da separação entre capability, disponibilidade de backend, configuração, autorização, preferência do cliente e resultado efetivo. |
