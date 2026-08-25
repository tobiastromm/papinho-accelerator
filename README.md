# PapinhoAccelerator

PapinhoAccelerator é um projeto independente para transferir tarefas computacionalmente pesadas de clientes para outro dispositivo. PapinhoBrowser será o primeiro cliente oficial, mas não define nem limita o protocolo, o servidor, os transports ou os backends.

Esta revisão contém somente a baseline arquitetural e documental. Ela não oferece servidor funcional, protocolo binário congelado ou processamento de mídia.

## Escopo arquitetural

- Core portável em C, sem tipos ou APIs específicas de Win32/Winsock.
- Platform Abstraction Layer (PAL), abstração de transport e abstração de Compute Backend.
- Control Plane separado de um ou mais Data Channels.
- Sessions, capabilities, políticas, autorização e execução local/remota como conceitos explícitos.
- TCP como primeiro transport planejado; outros transports permanecem extensões futuras.
- Transport Security protege os canais do próprio PapinhoAccelerator e é independente da capability conceitual `TLS_OFFLOAD`, voltada a conexões externas do cliente.

## Documentação

- [Arquitetura](docs/architecture.md): camadas, responsabilidades, dependências e lifecycle.
- [Visão do protocolo](docs/protocol-overview.md): planes, framing conceitual e mensagens futuras.
- [Capabilities](docs/capabilities.md): negociação, localização da execução e políticas.
- [Networking](docs/networking.md): transports, canais e network egress.
- [Modelo de mídia](docs/media-model.md): streaming assist, transcoding e remote frames.
- [Modelo de segurança](docs/security-model.md): autenticação, autorização e requisitos de proteção.
- [Portabilidade](docs/portability.md): plataformas-alvo e limites das abstrações.

## Princípios

Separação de responsabilidades; independência de plataforma e transport; design por capabilities; política explícita; compatibilidade retroativa; defaults seguros; nenhuma criptografia própria; degradação graciosa; e fallback local quando suportado pelo cliente.

## Estado

Baseline de arquitetura/especificação com esqueleto compilável da fundação. Nenhuma plataforma, transport ou capability está declarada como implementada ou suportada.

## Build da fundação

Requer CMake 3.16 ou posterior e uma toolchain C moderna.

```text
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

O target `papacc_core` é uma biblioteca estática C99. `papacc_server` é somente um smoke test do bootstrap: imprime a identificação da fundação e termina, sem inicializar serviços ou abrir portas. A versão exposta é a versão do software e não define uma versão do Wire Protocol.
