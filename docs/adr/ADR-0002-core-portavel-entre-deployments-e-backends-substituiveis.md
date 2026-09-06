---
adr: ADR-0002
title: Core portável entre deployments e backends substituíveis
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

# ADR-0002 — Core portável entre deployments e backends substituíveis

## Contexto

O PapinhoAccelerator não é concebido apenas como um servidor Windows convencional.

A arquitetura deve preservar a possibilidade de diferentes formas de deployment ao longo da evolução do produto, incluindo, conforme viabilidade futura:

```text
Windows / Win32
Linux / POSIX
Raspberry Pi / SBC
Embedded
Dedicated Hardware
PCI / PCIe
ISA
outros appliances ou dispositivos especializados
```

Essa lista representa possibilidades arquiteturais preservadas, não suporte atual nem compromisso de implementação uniforme.

Alguns deployments poderão hospedar o servidor completo. Outros poderão disponibilizar apenas capacidades específicas, transports locais, Compute Backends ou integração com hardware.

O Windows/Win32 é a primeira implementação concreta disponível, mas não deve determinar a semântica pública, tipos, ownership, lifecycle ou estrutura de domínio do produto.

Esta decisão especializa para o PapinhoAccelerator a regra transversal definida por `PapinhoEngineering/ADR-0007`.

## Forças da decisão

- possibilidade de deployments heterogêneos;
- evolução futura para appliance e hardware dedicado;
- suporte potencial a SBCs e ambientes embarcados;
- possibilidade de placas PCI/PCIe/ISA ou dispositivos especializados;
- reaproveitamento do core entre plataformas;
- substituibilidade de PAL, transports e Compute Backends;
- isolamento de detalhes Win32/POSIX/hardware;
- preservação de PACC, Session, Channel, policy e capabilities;
- longevidade;
- redução de forks por plataforma;
- capacidade de operar com subconjuntos de funcionalidades.

## Alternativas consideradas

### Alternativa A — Tratar Windows como arquitetura definitiva

O Accelerator é estruturado em torno de Win32/WinSock e demais mecanismos nativos, com portabilidade tratada posteriormente.

**Vantagens**

- menor esforço inicial;
- integração direta com a primeira plataforma.

**Desvantagens / trade-offs**

- tipos e semântica Windows poderiam contaminar o core;
- portabilidade futura exigiria refatorações amplas;
- appliances e hardware dedicado poderiam demandar forks;
- backend e domínio tenderiam a se confundir.

### Alternativa B — Criar implementações completas independentes por deployment

Cada ambiente recebe sua própria implementação principal do Accelerator.

**Vantagens**

- otimização livre por plataforma;
- poucas abstrações compartilhadas.

**Desvantagens / trade-offs**

- duplicação de PACC, Session, Channel, policy e lifecycle;
- risco de divergência funcional;
- manutenção multiplicada;
- correções e segurança precisariam ser reproduzidas;
- evolução do protocolo poderia divergir por plataforma.

### Alternativa C — Core portátil com deployments e backends substituíveis

O Accelerator mantém um núcleo compartilhado e contratos portáveis, enquanto mecanismos concretos são fornecidos por PAL, Transport, Secure Transport, Compute Backend e integrações específicas.

**Vantagens**

- preserva lógica comum;
- permite trocar plataforma sem reescrever o produto;
- suporta deployments parciais/especializados;
- mantém tipos nativos confinados;
- favorece testes e evolução incremental.

**Desvantagens / trade-offs**

- exige interfaces explícitas;
- aumenta o trabalho inicial de arquitetura;
- alguns deployments terão capabilities diferentes;
- hardware especializado poderá exigir backends próprios.

## Decisão

O PapinhoAccelerator será estruturado para preservar um **core portável entre deployments**, com mecanismos específicos fornecidos por backends substituíveis.

A organização conceitual é:

```text
PapinhoAccelerator Portable Core
│
├── PACC / protocol processing
├── Connection / Session / Channel models
├── capability model
├── policy
├── routing / orchestration
└── portable lifecycle/state
        │
        ▼
Portable Contracts
        │
        ├── PAL
        ├── Transport
        ├── Secure Transport
        ├── Compute Backend
        ├── Storage / device services
        └── outras fronteiras justificadas
                │
                ▼
Concrete Implementations
        ├── Win32
        ├── POSIX/Linux
        ├── SBC / embedded
        ├── appliance
        ├── dedicated hardware
        └── PCI/PCIe/ISA or device-specific implementations
```

### Windows é implementação inicial, não modelo do produto

Win32/WinSock podem implementar backends concretos, mas não definem a identidade do core.

Tipos como:

```text
SOCKET
HANDLE
HWND
CRITICAL_SECTION
```

não devem atravessar fronteiras portáveis do Accelerator quando puderem ser encapsulados.

### PACC, Session e policy não pertencem à plataforma

PACC, Session, Channel, capability model, policy e demais modelos portáveis não devem ser duplicados apenas porque um deployment utiliza outra plataforma ou mecanismo de transporte.

```text
deployment diferente
        !=
PACC diferente
        !=
Session diferente
        !=
core diferente
```

Divergências locais devem permanecer em backends ou adapters sempre que tecnicamente possível.

### PAL, Transport e Compute são mecanismos substituíveis

A PAL encapsula primitivas de plataforma.

Transport encapsula canais de comunicação.

Compute Backend encapsula mecanismos de execução/offload.

Essas fronteiras podem possuir implementações distintas por deployment sem transferir detalhes nativos ao core.

### Secure Transport permanece fronteira separada

Transport Security não deve ser embutida de forma inseparável em um backend de networking específico.

O core deve consumir uma fronteira adequada de Secure Transport quando o perfil exigir segurança.

Isso permite que backends diferentes de secure transport existam sem alterar PACC ou Session.

### Deployment pode oferecer apenas subconjunto

Portabilidade não implica equivalência de capabilities.

Um deployment limitado pode disponibilizar:

```text
server completo
apenas Compute Backend
apenas transport especializado
subconjunto de media capabilities
subconjunto de networking
outro papel explicitamente definido
```

Ausência de um backend/capability deve resultar em disponibilidade reduzida e explícita, não em comportamento presumido.

### Hardware especializado é possibilidade, não implementação atual

PCI, PCIe, ISA, embedded e hardware dedicado são possibilidades arquiteturais preservadas.

Este ADR não define:

- protocolo elétrico;
- driver;
- DMA;
- memória compartilhada;
- bus mastering;
- firmware;
- interface de dispositivo;
- mecanismo de descoberta;
- wire format específico de hardware.

Essas decisões pertencem a trabalhos futuros quando houver hardware concreto.

### Build target não define core

A existência de builds distintas por plataforma/toolchain não implica forks do core.

Os targets seguem `PapinhoEngineering/ADR-0002`.

## Justificativa

O PapinhoAccelerator existe justamente para deslocar trabalho de máquinas limitadas para recursos externos ou especializados.

Isso torna plausível que o próprio Accelerator seja executado em contextos muito diferentes: um PC Windows, um servidor Linux, uma pequena appliance, um SBC ou hardware dedicado.

Se sua arquitetura central depender da primeira plataforma disponível, essa evolução exigirá reescrever partes fundamentais do produto.

Ao manter PACC, Session, Channel, policy e demais modelos em um core portátil, a evolução ocorre pela introdução de backends concretos.

A arquitetura preserva possibilidades futuras sem implementar antecipadamente plataformas que talvez nunca sejam necessárias.

## Consequências

### Positivas

- o core pode ser reutilizado entre deployments;
- Win32 não contamina contratos portáveis;
- Linux/appliance/SBC/hardware futuro podem reutilizar PACC e runtime lógico;
- backends podem ser desenvolvidos incrementalmente;
- deployments limitados podem oferecer subconjuntos explícitos;
- reduz risco de forks completos;
- facilita testes de contratos independentes de plataforma.

### Negativas / trade-offs

- interfaces de backend precisam ser cuidadosamente projetadas;
- haverá custo de adapters e composition roots específicos;
- determinados recursos poderão exigir exceções/localizações específicas;
- alguns backends terão capacidades e desempenho diferentes.

### Neutras ou operacionais

- somente Win32 está implementado atualmente em várias áreas;
- nenhum backend Linux/embedded/PCI/ISA é criado por este ADR;
- não há obrigação de suportar todas as plataformas listadas;
- novas fronteiras devem ser adicionadas apenas quando uma necessidade real surgir.

## Regras derivadas

1. Win32 é uma implementação concreta, não a semântica do produto.
2. O core portátil não deve expor tipos Win32/POSIX/hardware quando puderem ser encapsulados.
3. PACC, Session, Channel e policy não devem ser duplicados por deployment.
4. PAL deve encapsular primitivas de plataforma.
5. Transport deve encapsular mecanismos de comunicação.
6. Secure Transport deve permanecer uma fronteira separada quando requerido.
7. Compute Backend deve encapsular mecanismos de execução/offload.
8. Backends diferentes não implicam core diferente.
9. Deployments podem oferecer subconjuntos explícitos de capabilities.
10. Ausência de backend/capability não deve ser tratada como suporte implícito.
11. PCI/PCIe/ISA/hardware dedicado permanecem possibilidades, não suporte atual.
12. Detalhes de hardware não devem ser inventados antes de requisitos concretos.
13. Novos backends devem preservar ownership e lifecycle explícitos.
14. Builds/targets diferentes não justificam forks do core.
15. Esta decisão especializa, mas não substitui, `PapinhoEngineering/ADR-0007`.

## Impacto

### Código

O core e APIs portáveis devem permanecer livres de dependências nativas desnecessárias.

Composition roots e backends podem ser específicos de plataforma.

### Build e toolchains

Targets distintos podem selecionar backends distintos, mantendo o máximo possível do core compartilhado.

### Documentação

Documentos de arquitetura e portabilidade devem referenciar este ADR e `PapinhoEngineering/ADR-0007`.

A lista de plataformas deve continuar sendo tratada como possibilidade arquitetural quando não houver suporte factual.

### Compatibilidade

Suporte real continua dependente de Target Matrix e validação factual.

Este ADR não declara suporte a Linux, Raspberry Pi, embedded, PCI, PCIe ou ISA.

### Testes

Contratos portáveis devem, quando possível, possuir testes independentes do backend.

Backends devem provar conformidade com contratos e comportamento específico da plataforma.

## Verificação de conformidade

Pode-se verificar se:

- headers do core não expõem tipos nativos desnecessariamente;
- Win32-specific code permanece em backends/composition apropriados;
- PACC/Session/Channel não são duplicados por plataforma;
- novos backends implementam contratos em vez de copiar o core;
- documentação distingue possibilidade arquitetural de suporte real;
- nenhuma plataforma futura é declarada suportada sem implementação/validação;
- builds diferentes reutilizam o core quando tecnicamente possível.

## Relações

### ADRs relacionadas

- `PapinhoEngineering/ADR-0002` — Identificação factual e durável de build targets.
- `PapinhoEngineering/ADR-0004` — Modelo compartilhado de configuração e separação entre Core e Frontends.
- `PapinhoEngineering/ADR-0007` — Fronteiras portáveis entre core, plataforma e backends substituíveis.

### Capability Documents relacionadas

Nenhuma específica.

### Documentos relacionados

- `docs/architecture.md`
- `docs/portability.md`
- `docs/networking.md`
- `docs/phase2-transport-session-design.md`
- `docs/protocol-framing.md`
- `docs/phase3-security-architecture.md`

## Princípio

**O PapinhoAccelerator deve poder mudar radicalmente de deployment sem precisar mudar de identidade arquitetural.**

E:

```text
Windows hoje
Linux amanhã
appliance ou hardware depois
        ↓
o core continua sendo PapinhoAccelerator
```

## Histórico de revisões

| Revisão | Data | Descrição |
|---:|---|---|
| 1 | 2026-09-06 | Formalização da especialização local de portabilidade do PapinhoAccelerator para deployments heterogêneos e backends substituíveis. |
