---
adr: ADR-0007
title: Identidade persistente de interface e resolução runtime de bind
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

# ADR-0007 — Identidade persistente de interface e resolução runtime de bind

## Contexto

O PapinhoAccelerator permite selecionar interfaces de rede para bind.

Essa seleção precisa sobreviver a mudanças de runtime sem confundir conceitos que possuem durações e finalidades diferentes:

```text
persistent interface identity
runtime snapshot identity
interface index
presentation name
IP address
```

Esses valores não são equivalentes.

Um índice de interface pode mudar entre boots ou alterações de adapter.

Um `interface_instance_id` serve apenas para agrupar registros dentro de um snapshot de discovery.

Um nome amigável serve para apresentação humana e pode mudar.

Um endereço IP pode mudar, desaparecer ou ser compartilhado em contextos distintos.

Mesmo assim, uma configuração administrativa como:

```text
"escute nesta interface"
```

deve poder referenciar a mesma interface posteriormente de forma mais estável que índice, nome ou endereço.

A Phase 1 consolidou um fluxo em que configuração persistida por interface é resolvida contra um snapshot canônico atual antes de produzir `PAPACC_BIND_SELECTION` e `PAPACC_BIND_TARGET`.

Esta decisão formaliza essa separação.

## Forças da decisão

- configuração persistente de bind;
- estabilidade entre discovery calls e reboots;
- múltiplas interfaces;
- IPv4/IPv6;
- nomes de interface mutáveis;
- índices runtime mutáveis;
- suporte futuro a backends não-Windows;
- separação entre configuração persistida e objetos runtime;
- ausência de identidade baseada em IP;
- previsibilidade de GUI/CLI;
- compatibilidade com appliance/headless.

## Alternativas consideradas

### Alternativa A — Persistir interface index

A configuração armazena o índice nativo/runtime da interface.

**Vantagens**

- simples;
- acesso direto ao backend.

**Desvantagens / trade-offs**

- índice pode mudar;
- semântica pode variar por família/protocolo;
- acopla configuração persistente a detalhe runtime;
- inadequado como identidade durável.

### Alternativa B — Persistir FriendlyName/presentation name

A configuração armazena o nome visível da interface.

**Vantagens**

- legível por humanos;
- fácil de exibir.

**Desvantagens / trade-offs**

- nome pode mudar;
- pode não ser único;
- pode não existir;
- localização/plataforma podem alterar apresentação;
- mistura UX com identidade.

### Alternativa C — Persistir IP address

A configuração identifica a interface pelo endereço IP selecionado.

**Vantagens**

- formato conhecido;
- diretamente relacionado ao bind.

**Desvantagens / trade-offs**

- DHCP pode alterar endereço;
- uma interface possui múltiplos endereços;
- IPv6 adiciona escopo;
- endereço não é identidade de dispositivo/interface;
- perde intenção administrativa quando o endereço muda.

### Alternativa D — Persistent ID opaco + resolução em snapshot atual

A configuração armazena um token persistente opaco quando disponível.

Durante startup/configuração, o token é resolvido contra um discovery snapshot atual e convertido para IDs runtime e bind targets atuais.

**Vantagens**

- separa persistência de runtime;
- nome/index/address podem mudar;
- backend pode fornecer estratégia própria;
- permite UI amigável sem usar nome como identidade;
- mantém bind baseado no estado real atual.

**Desvantagens / trade-offs**

- precisa de etapa explícita de resolução;
- persistent ID pode não estar disponível em todas as plataformas;
- desaparecimento/replacement de interface precisa de erro/policy definidos;
- token é machine-local, não universal.

## Decisão

O PapinhoAccelerator distingue explicitamente quatro identidades/representações de interface:

```text
Persistent ID
Snapshot Instance ID
Native/Runtime Interface Index
Presentation Name
```

e mantém IP address como fato de endereço separado.

### Persistent ID

`PAPACC_NETWORK_INTERFACE_PERSISTENT_ID` representa um token opaco, machine-local, destinado a reencontrar a mesma interface em snapshots posteriores.

Ele:

- pode ser persistido quando `is_valid = TRUE`;
- não é globalmente único;
- não é Wire Protocol identity;
- não é endereço IP;
- não é interface index;
- não é `interface_instance_id`;
- não é presentation name;
- pode ser indisponível em determinados backends.

A validade é determinada por:

```text
is_valid
```

e não pelo valor numérico isolado.

### Snapshot Instance ID

`interface_instance_id` existe para correlacionar interface e seus endereços dentro de **um único discovery snapshot**.

```text
interface_instance_id
→ snapshot-local grouping identity
```

Ele não deve ser persistido.

Não deve ser considerado estável através de:

- nova discovery call;
- reboot;
- mudança de topology;
- mudança de adapter;
- outra plataforma.

### Interface Index

`interface_index` representa informação runtime/nativa necessária a determinadas operações de networking.

Ela permanece separada da identidade persistente porque:

- pode mudar;
- possui significado técnico específico;
- IPv4/IPv6 podem ter índices distintos;
- não é identidade administrativa durável.

### Presentation Name

`presentation_name` é somente apresentação humana.

Pode ser usado em:

- CLI;
- GUI;
- logs;
- listagens.

Nunca deve ser usado como identidade canônica de configuração.

```text
presentation name != identity
```

### IP address não é interface identity

Um endereço pertence ao estado atual de networking.

A mesma interface pode ter múltiplos endereços e um endereço pode mudar ao longo do tempo.

Portanto:

```text
IP address != persistent interface identity
```

### Discovery Snapshot é canônico para uma resolução

Uma resolução de seleção deve usar um único snapshot lógico/coerente.

Fluxo:

```text
Persistent Configuration
        ↓
Persistent ID
        ↓
Canonical Discovery Snapshot
        ↓
current interface_instance_id
        ↓
PAPACC_BIND_SELECTION
        ↓
PAPACC_BIND_TARGET(s)
        ↓
platform listener creation
```

Não misturar registros provenientes de snapshots diferentes durante a mesma resolução.

### Bind Selection é runtime, não persistência

`PAPACC_BIND_SELECTION` representa a seleção já resolvida para o runtime atual.

Ele não é o formato persistente principal de identidade.

`interface_instance_id` dentro dessa seleção continua snapshot-local.

### Bind Target é resultado operacional

`PAPACC_BIND_TARGET` representa um alvo concreto de bind derivado do estado atual.

Ele pode conter:

```text
address
scope_id
interface_instance_id
```

Esses campos são runtime facts.

Um Bind Target não deve ser persistido como substituto da intenção administrativa original quando a configuração é “esta interface”.

### ALL interfaces é intenção administrativa

`--all-interfaces` / equivalente representa intenção:

```text
listen on all applicable interfaces
```

Não significa que `0.0.0.0` ou `::` sejam a configuração persistente conceitual.

Wildcard addresses surgem na resolução operacional.

```text
administrative intent: ALL
        ↓
runtime resolution
        ↓
0.0.0.0 / :: when appropriate
```

### FriendlyName não participa da resolução de identidade

A seleção por interface não deve procurar uma interface pelo nome amigável como mecanismo canônico.

O nome é exibido ao usuário, mas a persistência utiliza Persistent ID quando disponível.

### Interface ausente não deve fazer fallback silencioso

Se uma configuração persistente referencia uma interface que não pode ser resolvida no snapshot atual, o sistema não deve silenciosamente escolher outra interface apenas por:

- mesmo nome;
- mesmo índice;
- mesmo IP anterior;
- “primeira interface disponível”.

A falha deve ser explícita ou tratada por uma policy específica previamente definida.

Isso evita escutar acidentalmente em uma rede diferente da intenção administrativa.

### Persistência é separada do Configuration Model runtime

Em conformidade com `PapinhoEngineering/ADR-0004`, storage/source não define a semântica.

GUI, CLI ou arquivo podem representar a escolha persistente.

Todos devem convergir para a mesma semântica:

```text
configured interface identity
        ↓
resolved current runtime selection
```

A GUI futura não cria identidade própria baseada no texto exibido.

### Portabilidade de backend

Outros backends podem fornecer um token persistente compatível ou declarar:

```text
persistent_id.is_valid = FALSE
```

Este ADR não exige que todas as plataformas possuam o mesmo algoritmo ou namespace de Persistent ID.

Ele exige apenas que a distinção semântica seja preservada.

### Persistent ID não é identidade de rede global

O token é machine-local.

Não deve ser:

- transmitido como identidade de peer;
- usado para autenticação;
- comparado entre máquinas como device identity;
- usado como Wire interface identifier.

## Justificativa

Configuração persistente e estado runtime têm ciclos de vida diferentes.

Persistir um índice ou `interface_instance_id` congelaria um detalhe efêmero.

Persistir FriendlyName confundiria UX com identidade.

Persistir IP address confundiria endereço atual com intenção administrativa.

Um Persistent ID opaco permite preservar a intenção “a mesma interface desta máquina” enquanto o runtime continua trabalhando com índices, instance IDs e addresses atuais.

A etapa explícita de resolução também permite detectar de forma segura quando a interface configurada não existe mais.

## Consequências

### Positivas

- bind configuration sobrevive melhor a reboots/mudanças runtime;
- UI pode exibir nomes sem depender deles;
- índices continuam livres para significado técnico;
- IPv4/IPv6 podem compartilhar agrupamento de interface sem falsificar identidade;
- fallback perigoso é evitado;
- backends futuros podem definir sua própria estratégia;
- configuração persistente permanece separada de runtime objects.

### Negativas / trade-offs

- exige resolver Persistent ID em cada startup/reload;
- plataforma sem persistent identity precisa de comportamento explícito;
- interface substituída pode exigir intervenção administrativa;
- armazenamento precisa representar validade/token de maneira adequada.

### Neutras ou operacionais

- Win32 possui implementação atual;
- estratégia Linux/POSIX ainda pode ser definida no futuro;
- Persistent ID não precisa ter a mesma representação entre plataformas;
- `interface_instance_id` continua útil internamente;
- wildcard bind continua válido como resultado da seleção ALL.

## Regras derivadas

1. Persistent ID, instance ID, interface index e presentation name são conceitos distintos.
2. `interface_instance_id` é válido somente dentro do snapshot correspondente.
3. `interface_instance_id` não deve ser persistido.
4. `interface_index` não é identidade persistente.
5. Presentation name nunca é identidade canônica.
6. IP address não é interface identity.
7. Persistent ID é opaco e machine-local.
8. Somente `is_valid` define disponibilidade do Persistent ID.
9. Uma resolução usa um snapshot canônico único.
10. Configuração persistente deve ser resolvida para runtime selection.
11. Bind Selection representa runtime state, não persistent identity.
12. Bind Target representa resultado operacional, não configuração administrativa durável.
13. ALL interfaces é intenção administrativa; wildcard é resultado de resolução.
14. Interface persistida ausente não deve gerar fallback silencioso para outra interface.
15. Backends podem deixar Persistent ID indisponível.
16. Persistent ID não é Wire identity nem mecanismo de autenticação.
17. GUI/CLI devem compartilhar a mesma semântica de seleção de interface.

## Impacto

### Código

Network discovery deve manter separados:

- interface catalog;
- address records;
- persistent identity;
- runtime grouping/index;
- presentation metadata.

Resolution code deve traduzir configuração persistente em runtime Bind Selection/Targets.

### Build e toolchains

Nenhum impacto direto.

Backends de plataforma podem implementar estratégias diferentes de Persistent ID.

### Documentação

`docs/networking.md` permanece como visão operacional/técnica.

Este ADR passa a ser a decisão canônica sobre identidade e resolução de interface.

### Compatibilidade

A disponibilidade de Persistent ID pode variar por plataforma.

Essa diferença deve ser documentada factual e explicitamente.

### Testes

Testes devem verificar, conforme aplicável:

- instance IDs não assumidos estáveis;
- persistent ID equality;
- resolução de persistent ID no snapshot atual;
- interface não encontrada;
- presentation name não utilizado como identity;
- ALL → wildcard/runtime targets;
- atomicidade da resolução contra um snapshot;
- IPv4/IPv6 e scope handling;
- ausência de fallback silencioso.

## Verificação de conformidade

Pode-se verificar se:

- configurações persistidas não armazenam `interface_instance_id` como identidade durável;
- FriendlyName não é usado como chave canônica;
- interface index não substitui persistent ID;
- Bind Target não é persistido como identidade da interface;
- lookup ocorre no snapshot corrente;
- ausência da interface produz erro/policy explícita;
- backend sem token persistente não inventa estabilidade inexistente;
- UI apresenta nome separadamente da identidade.

## Relações

### ADRs relacionadas

- `PapinhoAccelerator/ADR-0002` — Core portável entre deployments e backends substituíveis.
- `PapinhoEngineering/ADR-0004` — Modelo compartilhado de configuração, fonte única de verdade e separação entre Core e Frontends.
- `PapinhoEngineering/ADR-0007` — Fronteiras portáveis entre core, plataforma e backends substituíveis.

### Capability Documents relacionadas

Nenhuma específica.

### Documentos relacionados

- `docs/networking.md`
- `docs/portability.md`
- `src/network/network_interface.h`
- `src/network/bind_selection.h`
- `src/network/bind_target.h`

## Princípio

**Persistimos a intenção de selecionar uma interface; resolvemos o estado concreto dessa interface no runtime atual.**

E:

```text
persistent identity != runtime instance
presentation != identity
address != interface
configuration intent != bind target
```

## Histórico de revisões

| Revisão | Data | Descrição |
|---:|---|---|
| 1 | 2026-09-06 | Formalização da separação entre Persistent ID, snapshot instance ID, interface index, presentation name e resolução runtime de bind. |
