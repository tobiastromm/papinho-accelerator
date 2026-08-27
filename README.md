# PapinhoAccelerator

PapinhoAccelerator é um projeto independente para transferir tarefas computacionalmente pesadas de clientes para outro dispositivo. PapinhoBrowser será o primeiro cliente oficial, mas não define nem limita o protocolo, o servidor, os transports ou os backends.

## Estado atual

A Foundation da Phase 1 possui modelos portáteis em C99 e um backend servidor Win32 operacional no nível de infraestrutura:

- listeners TCP reais em um único `control_port` explícito;
- seleção de todas as interfaces ou de interfaces por identidade persistente local;
- catálogo de interfaces com identidade runtime, identidade persistente e nome de apresentação UTF-8;
- configuração por CLI e comando de inspeção `--list-interfaces`;
- composição transacional de discovery, resolução de bind, WinSock e listener set;
- encerramento gracioso por Ctrl+C e Ctrl+Break.

Ainda não estão implementados:

- aceitação ou processamento de clientes;
- Sessions, Control Channel, Data Channels ou Wire Protocol;
- autenticação, autorização ou Transport Security;
- capabilities de computação, incluindo `TLS_OFFLOAD`;
- network egress, proxy ou conexões externas;
- GUI, arquivo de configuração ou backend POSIX.

Os listeners atuais apenas permanecem abertos até a solicitação de parada. Não existe `accept()`, envio, recepção ou protocolo nesta fase.

## Escopo arquitetural

- Core e modelos de rede portáteis sem tipos Win32/Winsock.
- PAL, backend de discovery e backend TCP Win32 como primeira implementação de plataforma.
- Control/Data Channels, Sessions, capabilities e Compute Backends preservados como desenho futuro, não como funcionalidade entregue.
- Transport Security protege conceitualmente os canais do próprio PapinhoAccelerator e é independente da capability futura `TLS_OFFLOAD`, voltada a conexões externas do cliente.

## Uso atual no Windows

```text
papacc_server
papacc_server --list-interfaces
papacc_server --port <porta> --all-interfaces
papacc_server --port <porta> --interface-id <persistent-id>
```

Não existe porta oficial ou default. O modo RUN exige `--port` e uma decisão explícita de bind. `--allow-network-egress` registra somente policy; egress ainda não foi implementado.

## Documentação

- [Arquitetura](docs/architecture.md)
- [Visão do protocolo](docs/protocol-overview.md)
- [Capabilities](docs/capabilities.md)
- [Networking](docs/networking.md)
- [Modelo de mídia](docs/media-model.md)
- [Modelo de segurança](docs/security-model.md)
- [Portabilidade](docs/portability.md)
- [Auditoria final da Phase 1](docs/phase1-foundation-audit.md)

## Build

Requer CMake 3.16 ou posterior e uma toolchain C99. No Windows, o build também produz o servidor operacional Win32.

```text
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

A versão `0.1.0` é a versão do software e não congela nem atribui versão, IDs ou layout ao Wire Protocol.
