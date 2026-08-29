# PapinhoAccelerator

PapinhoAccelerator é um projeto independente para transferir tarefas computacionalmente pesadas de clientes para outro dispositivo. PapinhoBrowser será o primeiro cliente oficial, mas não define nem limita o protocolo, o servidor, os transports ou os backends.

## Estado atual

As Phases 1 e 2 estão concluídas. A Phase 3 iniciou apenas no estágio de
arquitetura e threat model; autenticação e Transport Security continuam não
implementadas. A baseline possui modelos portáteis em C99 e um servidor Win32
estruturalmente operacional:

- listeners TCP reais em um único `control_port` explícito;
- seleção de todas as interfaces ou de interfaces por identidade persistente local;
- catálogo de interfaces com identidade runtime, identidade persistente e nome de apresentação UTF-8;
- configuração por CLI e comando de inspeção `--list-interfaces`;
- composição transacional de discovery, resolução de bind, WinSock e listener set;
- aceitação não bloqueante, Sessions, um Control Channel e múltiplos Data Channels;
- Envelope 1.0 e os fluxos `CONTROL_OPEN`/`CONTROL_ACCEPT`, ticket DATA one-time e `DATA_ATTACH`/`DATA_ACCEPT`;
- encerramento gracioso por Ctrl+C e Ctrl+Break, inclusive com canais ativos.

Ainda não estão implementados:

- autenticação, autorização ou Transport Security;
- capabilities de computação, incluindo `TLS_OFFLOAD`;
- protocolo de aplicação ou processamento de payload após `DATA_ACCEPT`;
- network egress, proxy ou conexões externas;
- GUI, arquivo de configuração ou backend POSIX.

O protocolo entregue na Phase 2 é somente a base estrutural de transporte, framing, estabelecimento e associação. Session `ACTIVE` não significa autenticada, autorizada, confiável ou segura. A próxima fase deve introduzir autenticação, autorização e Transport Security sem alterar implicitamente esse significado por negociação de capability.

## Escopo arquitetural

- Core e modelos de rede portáteis sem tipos Win32/Winsock.
- PAL, backend de discovery e backend TCP Win32 como primeira implementação de plataforma.
- Control/Data Channels e Sessions estruturais entregues; capabilities e Compute Backends permanecem desenho futuro.
- Transport Security protege conceitualmente os canais do próprio PapinhoAccelerator e é independente da capability futura `TLS_OFFLOAD`, voltada a conexões externas do cliente.

## Uso atual no Windows

```text
papacc_server
papacc_server --list-interfaces
papacc_server --port <porta> --all-interfaces
papacc_server --port <porta> --interface-id <persistent-id>
papacc_server --port <porta> --all-interfaces --log-level info
```

Não existe porta oficial ou default. O modo RUN exige `--port` e uma decisão explícita de bind. `--allow-network-egress` registra somente policy; egress ainda não foi implementado.

`--log-level` aceita `off`, `error`, `warn`, `info` (default) e `debug`. `off` desabilita somente toda saída do `PAPACC_LOGGER`; saídas funcionais de `--help` e `--list-interfaces` permanecem disponíveis. INFO registra lifecycle estrutural de listeners, Connections, Sessions, tickets e DATA attachment. Tickets completos e payloads nunca são registrados. IDs mostrados são somente IDs runtime locais e não são serializados no Wire Protocol.

## Validação com consumidor real

O primeiro consumidor real foi validado: PapinhoBrowser em Windows NT 4.0 acessou por LAN TCP o PapinhoAccelerator executado em Windows moderno e concluiu `CONTROL_OPEN` → `CONTROL_ACCEPT` → solicitação de ticket → segunda conexão TCP → `DATA_ATTACH` → `DATA_ACCEPT`. PapinhoBrowser permaneceu funcional e o Accelerator permaneceu opcional. Isto valida a integração estrutural da Phase 2; não inicia Phase 3 nem implica autenticação, Transport Security ou processamento de payload DATA.

## Documentação

- [Arquitetura](docs/architecture.md)
- [Visão do protocolo](docs/protocol-overview.md)
- [Capabilities](docs/capabilities.md)
- [Networking](docs/networking.md)
- [Modelo de mídia](docs/media-model.md)
- [Modelo de segurança](docs/security-model.md)
- [Checkpoint de arquitetura de segurança da Phase 3](docs/phase3-security-architecture.md)
- [Portabilidade](docs/portability.md)
- [Auditoria final da Phase 1](docs/phase1-foundation-audit.md)
- [Auditoria final da Phase 2](docs/phase2-integration-audit.md)

## Build

Requer CMake 3.16 ou posterior e uma toolchain C99. No Windows, o build também produz o servidor operacional Win32.

Em Windows, abra o Developer PowerShell/Command Prompt do Visual Studio para disponibilizar CMake, Ninja e MSVC. A árvore canônica é `build/ninja/`, sempre out-of-source:

```text
cmake -S . -B build\ninja -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build\ninja
ctest --test-dir build\ninja --output-on-failure
```

Árvores `build*` são artefatos locais ignorados pelo Git. Não coloque fontes ou definições de protocolo necessárias dentro delas.

A versão `0.1.0` é a versão do software e não congela nem atribui versão, IDs ou layout ao Wire Protocol.
