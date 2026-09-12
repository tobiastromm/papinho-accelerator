# Arquitetura

## Decisões arquiteturais canônicas

Este documento oferece a visão corrente da arquitetura sem duplicar o contexto
e a justificativa preservados nos ADRs locais:

- [ADR-0001 — Secure Principal e Legacy Endpoint](adr/ADR-0001-secure-principal-e-legacy-endpoint.md);
- [ADR-0002 — Core portável entre deployments e backends substituíveis](adr/ADR-0002-core-portavel-entre-deployments-e-backends-substituiveis.md);
- [ADR-0003 — Connection, Session e Channel — ownership e lifecycle](adr/ADR-0003-connection-session-channel-ownership-e-lifecycle.md);
- [ADR-0004 — Escalonamento de I/O não bloqueante, limitado e justo](adr/ADR-0004-escalonamento-de-io-nao-bloqueante-limitado-e-justo.md);
- [ADR-0005 — Negociação de capabilities, disponibilidade de backend e autoridade de policy](adr/ADR-0005-negociacao-de-capabilities-disponibilidade-de-backend-e-autoridade-de-policy.md);
- [ADR-0006 — Perfil de Transport Security e credenciais do Secure Principal](adr/ADR-0006-perfil-de-transport-security-e-credenciais-do-secure-principal.md);
- [ADR-0007 — Identidade persistente de interface e resolução runtime de bind](adr/ADR-0007-identidade-persistente-de-interface-e-resolucao-runtime-de-bind.md);
- [ADR-0008 — Fixação de dependências Papinho por release](adr/ADR-0008-fixacao-de-dependencias-papinho-por-release.md);
- [ADR-0009 — Perfil de transporte explícito por listener](adr/ADR-0009-perfil-de-transporte-explicito-por-listener.md);
- [ADR-0010 — Resolução de configuração de segurança do servidor](adr/ADR-0010-resolucao-de-configuracao-de-seguranca-do-servidor.md).
- [ADR-0011 — Identidade de capability e modelo portátil limitado](adr/ADR-0011-identidade-de-capability-e-modelo-portatil-limitado.md);
- [ADR-0012 — Snapshot de capabilities e enforcement contextual](adr/ADR-0012-snapshot-de-capabilities-e-enforcement-contextual.md).

Para o threat model e o contexto histórico da inserção de Transport Security,
identidade e gates de autorização, consulte o checkpoint autoritativo de
Phase 3.A1 [Phase 3 Security Architecture and Threat Model](phase3-security-architecture.md).
As decisões arquiteturais canônicas posteriores permanecem nos ADRs acima.

## Estado de implementação

As Phases 1 e 2 implementam a Foundation portátil e, no Windows, discovery de interfaces, resolução persistente de bind, WinSock, listeners, aceitação não bloqueante, Sessions, Control/Data Channels estruturais, framing e os fluxos de estabelecimento CONTROL e associação DATA por ticket one-time. O executável integra esses componentes em um único loop `select()` e encerra de forma graciosa por Ctrl+C/Ctrl+Break.

PapinhoSecureTransport (PST) já existe como biblioteca independente. O pin
`v0.6.1` (API 2.1/SPI 3.0), a aquisição validada e uma boundary CMake privada centralizam seu
contrato de consumo. A composição privada de runtime, credenciais, trust e
perfil Secure Principal SERVER já existe de forma opt-in e failure-atomic.
Exposição operacional de Transport Security no executável,
Capability Negotiation, protocolo de aplicação pós-DATA e Compute Backends,
permanece trabalho futuro. Session
`ACTIVE` nesta baseline significa somente estabelecimento estrutural concluído;
não significa autenticada, autorizada, confiável ou segura.

Estado da integração PST:

| Camada | Estado |
|---|---|
| Release pin, aquisição e validação do SDK | implementado |
| Boundary privada de includes/link/runtime files | implementado |
| Private security runtime composition | implementado; usada pelo controller seguro opt-in |
| PST logging adapter | implementado; privado, síncrono e opt-in |
| Readiness/scheduler integration | implementado; usada pelo controller seguro opt-in |
| Authentication/Authorization foundation | implementado; Principal/resolver/policy e contexts privados, com prova mTLS real em loopback |
| Secure DATA association binding | implementado; integrada ao caminho DATA seguro opt-in |
| Transport Security no servidor | implementado na boundary Win32 opt-in da 3.E; wiring operacional/CLI permanece futuro |
| Secure reference client | prova externa process-isolated concluída na 3.F; não é SDK/produto |

`papacc_pst_consumer` é somente um target privado de build. Ele não constitui
API pública do Accelerator, não cria objetos PST e não deve ser ligado ao core
portátil ou às entidades Connection, Session e Channel.

`papacc_security_composition` é uma unidade privada consumidora do PST. Ela
possui o runtime, a credencial local, o trust dos peers, a seleção exata do
provider e a configuração TLS 1.3 mTLS SERVER do Secure Principal. Só publica
estado `READY` depois da composição completa; falhas liberam recursos parciais.
O objeto de composição não aceita transports nem executa handshake ou I/O por
si próprio; o controller seguro opt-in consome sua configuração e runtime para
essas operações. O executável `papacc_server` ainda não expõe configuração
operacional de credenciais/listener Secure.

O runtime dessa composição é criado com o canal público de logging do PST. Um
adapter privado traduz semanticamente `PST_LOG_EVENT` para
`PAPACC_LOG_RECORD`, preserva role/provider/fatos em contexto limitado e entrega
ao mesmo logger/sink controlado pelo consumidor. O contexto do callback vive
até depois de `pst_runtime_release()`; não existe logger ou sink global PST.

A boundary privada de secure scheduling possui o `pst_wait_set`, registra
conexões e external sources com tokens estáveis, recebe eventos em buffer de
capacidade fornecida pelo caller e oferece wake de shutdown. PST determina
readiness; a ordem de dispatch gira por slot e cada membro recebe no máximo uma
oportunidade por passagem. O listener permanece pertencente ao Accelerator.
Essa infraestrutura não altera `PAPACC_CONNECTION`, Session ou Channel e é
usada pelo controller seguro opt-in, sem tornar o executável padrão TLS-enabled.

A foundation privada de Authentication/Authorization copia evidência
normalizada do peer PST, resolve uma credencial para um Principal opaco e
estável e consulta policy separada para acesso/CONTROL. Contexts de conexão e
Session são objetos auxiliares copy-by-value; não alteram as entidades runtime,
wire ou storage. Ausência, desconhecimento, disable, DENY ou erro falham fechado.
O harness autônomo da 3.C usa PST 0.6.1 CLIENT/SERVER sobre TCP loopback e
prova credencial enrolled/ALLOW, TLS-valid/NOT_ENROLLED, policy DENY,
certificate rotation para o mesmo Principal e credential DISABLED. A 3.E
posteriormente ligou essa foundation ao controller seguro opt-in.

A boundary privada da 3.D resolve o ticket sem consumi-lo, consulta os
Security Contexts separados, compara Principals, aplica autorização DATA e só
então revalida e consome o ticket exato antes do bind de Channel. Mismatch,
DENY, erro de policy e contexts ausentes não consomem ticket válido. Hooks
opt-in nos processors preservam o caminho Phase 2 e são usados pelo controller
seguro opt-in da 3.E.

## Objetivos

O sistema deve permanecer independente de cliente, sistema operacional, transport e mecanismo de computação. Windows é a primeira plataforma implementada, não a arquitetura do produto. Extensões devem preservar compatibilidade, falhar de modo seguro e permitir degradação graciosa e fallback local pelo cliente.

PapinhoBrowser é um consumidor inicial e importante, mas não define sozinho a
identidade arquitetural do servidor ou do protocolo:

```text
PapinhoAccelerator
        ↓
public/client integration boundary futura
        ↓
consumers
        ├── PapinhoBrowser
        ├── outros clientes Papinho futuros
        ├── aplicações especializadas futuras
        └── futura client library / SDK
```

```text
PapinhoBrowser é um consumidor
        !=
PapinhoAccelerator é Browser-specific
```

Essa fronteira pública ainda não está implementada ou congelada e não declara
integração disponível para terceiros nem suporte a browsers externos. Algumas
capabilities poderão ser mais genéricas, como processamento de mídia, compute,
network egress e `TLS_OFFLOAD`; outras poderão exigir integração mais profunda,
como HTML/CSS/layout, display commands, framebuffer ou futura renderização web
remota. Esses exemplos não constituem uma matriz de suporte implementado.

## Camadas

Para o Secure Principal, a composição planejada é:

```text
Transport
    ↓
PapinhoSecureTransport (PST)
    ↓ TLS 1.3 mTLS configurado pela policy do Accelerator
authenticated peer result
    ↓
Accelerator Principal mapping / authorization
    ↓
PACC Framing
    ↓
CONTROL / DATA
```

PST implementa a fronteira Secure Transport consumida pelo Accelerator. O
Accelerator depende da API pública do PST, não diretamente de NSS/NSPR,
Schannel, OpenSSL ou tipos de provider. Providers permanecem substituíveis e
privados ao PST. Em sentido inverso, PST não conhece PACC, CONTROL, DATA,
Session, capabilities ou policy de produto do Accelerator.

```text
Clientes independentes (PapinhoBrowser é apenas o primeiro)
                         |
              Wire Protocol / Planes
                         |
+-------------------------------------------------------+
| Core                                                  |
| Sessions | Capabilities | Policy | Jobs | State       |
+--------------------+----------------+-----------------+
                     |                |
+--------------------v--+   +---------v-----------------+
| Transport Abstraction |   | Compute Backend          |
| TCP | LOCAL_* futuro   |   | CPU/GPU/hardware futuro |
+-----------------------+   +---------------------------+
             |                         |
+------------v-------------------------v----------------+
| Platform Abstraction Layer (PAL)                      |
| tempo, memória, sincronização, I/O e plataforma       |
+-------------------------------------------------------+
             |
 Windows/Win32 | Linux/POSIX | embedded | outros
```

### Core Layer

Contém regras portáveis de protocolo, estado de Session, negociação de capabilities, policy evaluation, lifecycle de jobs e tratamento abstrato de mensagens. Deve ser C portável e não pode expor ou depender diretamente de `HWND`, `HANDLE`, `SOCKET`, `CRITICAL_SECTION`, Win32, Winsock ou equivalentes de uma plataforma.

### Platform Abstraction Layer (PAL)

Fornece ao Core operações de plataforma por interfaces estreitas: memória, tempo monotônico, logging, sincronização, execução e I/O quando futuramente necessários. Implementações PAL encapsulam Win32, POSIX ou ambiente embarcado. A PAL não decide políticas de produto nem interpreta o protocolo.

### Transport Abstraction Layer

Oferece criação/aceitação abstrata de canais, envio/recepção, fechamento, identidade e propriedades do transport. O Core e as capabilities não devem distinguir Ethernet, Internet, PCI, ISA ou outro meio para executar sua lógica. TCP será o primeiro transport; `LOCAL_PCI`, `LOCAL_ISA` e outros são pontos de extensão, sem detalhes de hardware definidos nesta fase.

### Compute Backend

Descobre e executa operações oferecidas por implementações de computação. Uma capability descreve o que pode ser feito, não como: CPU, GPU, biblioteca de mídia, decoder físico, FPGA, ASIC ou outro dispositivo podem ser backends. Backend não concede autorização e não escolhe network egress.

### Capability Framework e Policy Engine

O framework mantém IDs/versionamento e ofertas independentes. O Policy Engine cruza suporte, configuração administrativa, autorização do usuário e pedido do cliente. Políticas são explícitas, com negação como default quando permissão ou compatibilidade estiver ausente.

A Phase 4 implementa a key `(numeric ID, major)`, registry e sets bounded em
storage do caller, avaliação dos cinco inputs e snapshot separado de Session.
O snapshot é upper bound imutável; enforcement contextual pode retirar acesso
imediatamente e reabilitar somente uma key já presente. Nova concessão exige
nova Session ou futura renegociação explícita. Nenhum wire ou workload foi
adicionado.

Transport Security não pertence ao catálogo comum de capabilities: é uma propriedade da infraestrutura e do protocolo que protege a comunicação entre PapinhoAccelerator Client e PapinhoAccelerator Server. `TLS_OFFLOAD`, por outro lado, é uma capability conceitual de processamento para auxiliar TLS em conexões do cliente com serviços externos. Portanto:

```text
Transport Security != TLS Offload
```

Capability Negotiation não pode remover nem enfraquecer propriedades de Transport Security exigidas para a Session. Desabilitar `TLS_OFFLOAD` não altera a segurança dos canais PapinhoAccelerator.

## Control Plane e Data Plane

O Control Plane estabelece e governa a Session: identificação, autenticação, versão, capabilities, configuração, comandos, status, heartbeat, PING/PONG, erros e encerramento. O Data Plane transfere imagens, áudio, vídeo, framebuffer e dados grandes de jobs.

Em TCP, uma Session usa conceitualmente um Control Channel e zero ou mais Data Channels. A associação Data Channel–Session deve ser autenticada, íntegra, resistente a associação indevida/replay quando aplicável e submetida aos mesmos limites e políticas. O perfil 3.A2A exige igualdade de principal mais ticket estrutural e autorização; a 3.D implementou a gate privada com commit revalidado.

Transport Security deve abranger tanto o Control Channel quanto todos os Data Channels quando a política/configuração da Session exigir canal seguro. O perfil revisado é TLS 1.3 mTLS, com CA privada/administrativa e certificado individual por dispositivo cliente, conforme [Phase 3 Transport Security and Credential Profile](phase3-transport-security-profile.md). O closeout 3.A2B-R3 comprovou RetroZilla NSS/NSPR como backend legado viável. PST foi posteriormente implementado e é a biblioteca escolhida para materializar a fronteira Secure Transport. A composição privada, conexão ao listener, handshake, I/O seguro e integração com Principal/Session estão implementados no controller Win32 opt-in da 3.E.

A direção posterior distingue [Secure Principal e Legacy Endpoint](phase3-transport-profiles.md). O primeiro exige TLS 1.3 mTLS; o segundo é plaintext explicitamente habilitado, desabilitado por padrão e sem identidade criptográfica forte. Listeners distintos são recomendados. Falha no perfil seguro nunca seleciona o perfil legado.

Cada conexão TCP CONTROL ou DATA estabelece proteção própria no controller
seguro opt-in. Como Transport Security fica abaixo de Framing, o pipeline é `accept ->
PST security establishment -> authenticated peer result -> Accelerator
Principal/authorization -> classifier -> framing`; contextos do PST ficam
separados das entidades portáteis `PAPACC_CONNECTION` e `PAPACC_SESSION`.

Readiness de secure transport não é presumida equivalente à readiness do
socket nativo. O scheduler consome o contrato público de readiness do PST,
que delega ao provider o mecanismo apropriado. A evidência NSS demonstrou
`PR_Poll` sobre o descriptor SSL como a representação correta de interesse TLS,
enquanto `select()` nativo observa somente o transporte. A integração deve
preservar nonblocking I/O, bounded work, fairness e `WOULD_BLOCK` normal do
ADR-0004.

Legacy Endpoint não usa PST, não é backend plaintext/none/off do PST e nunca é
selecionado após falha do Secure Principal.

### Ponte temporal

Uma direção futura do produto é permitir que clientes históricos aproveitem
capacidades e protocolos atuais por intermédio do Accelerator, sem precisar
implementar localmente toda a evolução do ecossistema externo:

```text
historical client
        ↓ Transport Security aplicável e policy do Secure Principal
Accelerator
        ↓ capabilities e conexões explicitamente autorizadas
serviços e protocolos externos atuais
```

Quando `TLS_OFFLOAD` e `NETWORK_EGRESS_ACCELERATOR` forem futuramente compostos,
o primeiro e o segundo hop serão conexões e relações de segurança independentes;
não uma TLS Session fim a fim nem tradução direta entre versões de TLS. A
evolução externa não autoriza downgrade do primeiro hop, fallback automático
para Legacy Endpoint ou criptografia própria. Este é um racional arquitetural
de longevidade, não promessa de compatibilidade eterna, suporte atual ou desenho
de protocolo. Os limites conceituais estão no [Capability Document de TLS
Offload](capabilities/tls-offload.md).

## Integração e validação concluídas

Esta sequência é plano de integração, não ADR nem definição de subfases:

1. PST standalone → PASS, concluído no projeto PST.
2. PapinhoAccelerator + PST → Secure Principal TLS 1.3 mTLS → PASS.
3. Secure reference client ↔ PapinhoAccelerator → Secure Principal CONTROL/DATA → PASS.
4. PapinhoBrowser integration → future work; not started by Phase 3.

As Phases 3.B–3.G consumiram a API pública PST e concluíram composição, policy
do Secure Principal, readiness, ownership/lifecycle, Principal mapping,
autorização, logging por adapter e os lifecycles CONTROL/DATA no controller
seguro opt-in. O [closeout 3.G](phase3-security-final-audit.md) contém a matriz
final e os limites factuais.

## Session

`PAPACC_SESSION` é a entidade interna que representa uma conexão lógica de cliente; “client” fica reservado para uma futura biblioteca cliente.

```text
UNINITIALIZED
    ↓
ESTABLISHING
    ↓
ACTIVE
    ↓
CLOSING
    ↓
CLOSED
```

Publicação ou estabelecimento estrutural inicial não torna a Session
automaticamente `ACTIVE`. A ativação ocorre somente depois dos gates exigidos
pelo protocolo, pelo transport profile e pela policy aplicável.

Durante `ESTABLISHING`, podem existir etapas internas que não são estados
canônicos congelados da Session:

```text
ESTABLISHING
    ├── internal security / peer-authentication gates
    ├── internal Principal mapping and authorization gates
    ├── internal protocol establishment
    └── internal capability / policy gates futuros
        ↓
ACTIVE somente quando todos os gates aplicáveis forem satisfeitos
```

No Secure Principal, esses gates incluem PST/TLS 1.3 mTLS, Principal
autenticado, autorização e estabelecimento aplicável. O Legacy Endpoint é um
transport profile plaintext explicitamente habilitado, desabilitado por padrão,
sem strong cryptographic identity, sujeito à própria policy e nunca escolhido
como fallback automático. Ele não implica autorização irrestrita e usa o mesmo
lifecycle conceitual da Session, com os gates aplicáveis ao seu profile/policy.

A decomposição interna pode evoluir sem criar uma segunda state machine
concorrente ao ADR-0003. A Session deverá futuramente possuir Session ID não
previsível, timeouts, heartbeat/PING/PONG, Data Channels associados, ownership
de jobs, cleanup idempotente e regras explícitas de recuperação/reconexão.
Reconexão não deve implicitamente herdar autoridade.

## Dependências permitidas

Na Foundation atual, a direção concreta é:

```text
papacc_core
    ↑
papacc_network (modelos portáteis)
    ↑
papacc_network_win32 / papacc_tcp_win32
    ↑
papacc_server_config
    ↑
papacc_server_cli / papacc_server_network
    ↑
papacc_server (composition root Win32)
```

`src/network` não depende da aplicação e nenhum modelo portátil inclui tipos Win32. `server_network` e o console são APIs privadas da aplicação Win32 nesta fase.

```text
Aplicação/host -> Core -> interfaces Transport/Compute/PAL
Implementação TCP -------------------------> PAL/OS
Compute Backend ---------------------------> PAL/driver/API própria
```

O Core depende somente de contratos abstratos. Implementações podem depender da PAL e de APIs específicas, mas não podem vazar seus tipos para interfaces portáveis. Transport não deve chamar capabilities; Compute Backend não deve controlar Session ou política; autenticação e autorização permanecem separadas.

## Regras de design

Separação de responsabilidades limita cada camada ao seu papel. Independências de plataforma e transport evitam acoplamento. Capabilities versionadas permitem evolução e compatibilidade retroativa. Política explícita e defaults seguros impedem concessões por omissão. Degradação graciosa rejeita apenas o recurso incompatível quando seguro; clientes podem usar fallback local quando o suportarem. Criptografia própria é proibida.
