# Arquitetura

## Estado de implementação

As Phases 1 e 2 implementam a Foundation portátil e, no Windows, discovery de interfaces, resolução persistente de bind, WinSock, listeners, aceitação não bloqueante, Sessions, Control/Data Channels estruturais, framing e os fluxos de estabelecimento CONTROL e associação DATA por ticket one-time. O executável integra esses componentes em um único loop `select()` e encerra de forma graciosa por Ctrl+C/Ctrl+Break.

Autenticação, autorização, Transport Security, Capability Negotiation, protocolo de aplicação pós-DATA e Compute Backends permanecem trabalho futuro. Session `ACTIVE` nesta baseline significa somente estabelecimento estrutural concluído; não significa autenticada, autorizada, confiável ou segura.

## Objetivos

O sistema deve permanecer independente de cliente, sistema operacional, transport e mecanismo de computação. Windows é a primeira plataforma implementada, não a arquitetura do produto. Extensões devem preservar compatibilidade, falhar de modo seguro e permitir degradação graciosa e fallback local pelo cliente.

## Camadas

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

Transport Security não pertence ao catálogo comum de capabilities: é uma propriedade da infraestrutura e do protocolo que protege a comunicação entre PapinhoAccelerator Client e PapinhoAccelerator Server. `TLS_OFFLOAD`, por outro lado, é uma capability conceitual de processamento para auxiliar TLS em conexões do cliente com serviços externos. Portanto:

```text
Transport Security != TLS Offload
```

Capability Negotiation não pode remover nem enfraquecer propriedades de Transport Security exigidas para a Session. Desabilitar `TLS_OFFLOAD` não altera a segurança dos canais PapinhoAccelerator.

## Control Plane e Data Plane

O Control Plane estabelece e governa a Session: identificação, autenticação, versão, capabilities, configuração, comandos, status, heartbeat, PING/PONG, erros e encerramento. O Data Plane transfere imagens, áudio, vídeo, framebuffer e dados grandes de jobs.

Em TCP, uma Session usará conceitualmente um Control Channel e zero ou mais Data Channels. A associação Data Channel–Session deve ser autenticada, íntegra, resistente a associação indevida/replay quando aplicável e submetida aos mesmos limites e políticas. O mecanismo não está definido nesta fase.

Transport Security deve abranger tanto o Control Channel quanto todos os Data Channels quando a política/configuração da Session exigir canal seguro. Seu mecanismo concreto continua indefinido.

## Session

`PAPACC_SESSION` é a entidade interna que representa uma conexão lógica de cliente; “client” fica reservado para uma futura biblioteca cliente.

```text
CONNECTED -> NEGOTIATING -> AUTHENTICATING -> READY -> ACTIVE
                                                       |
                      CLOSED <- CLOSING <---------------+
```

Servidores abertos podem atravessar `AUTHENTICATING` sem credencial, mantendo uma decisão explícita de modo/política. Nomes e transições ainda não estão congelados. A Session deverá futuramente possuir Session ID não previsível, timeouts, heartbeat/PING/PONG, Data Channels associados, ownership de jobs, cleanup idempotente e regras explícitas de recuperação/reconexão. Reconexão não deve implicitamente herdar autoridade.

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
