# PapinhoAccelerator

PapinhoAccelerator é um projeto independente para transferir tarefas computacionalmente pesadas de clientes para outro dispositivo. PapinhoBrowser será o primeiro cliente oficial, mas não define nem limita o protocolo, o servidor, os transports ou os backends.

## Decisões arquiteturais locais

- [ADR-0001 — Secure Principal e Legacy Endpoint](docs/adr/ADR-0001-secure-principal-e-legacy-endpoint.md)
- [ADR-0002 — Core portável entre deployments e backends substituíveis](docs/adr/ADR-0002-core-portavel-entre-deployments-e-backends-substituiveis.md)
- [ADR-0003 — Connection, Session e Channel — ownership e lifecycle](docs/adr/ADR-0003-connection-session-channel-ownership-e-lifecycle.md)
- [ADR-0004 — Escalonamento de I/O não bloqueante, limitado e justo](docs/adr/ADR-0004-escalonamento-de-io-nao-bloqueante-limitado-e-justo.md)
- [ADR-0005 — Negociação de capabilities, disponibilidade de backend e autoridade de policy](docs/adr/ADR-0005-negociacao-de-capabilities-disponibilidade-de-backend-e-autoridade-de-policy.md)
- [ADR-0006 — Perfil de Transport Security e credenciais do Secure Principal](docs/adr/ADR-0006-perfil-de-transport-security-e-credenciais-do-secure-principal.md)
- [ADR-0007 — Identidade persistente de interface e resolução runtime de bind](docs/adr/ADR-0007-identidade-persistente-de-interface-e-resolucao-runtime-de-bind.md)
- [ADR-0008 — Fixação de dependências Papinho por release](docs/adr/ADR-0008-fixacao-de-dependencias-papinho-por-release.md)

## Estado atual

As Phases 1 e 2 estão concluídas. A Phase 3 concluiu 3.A1, o perfil 3.A2A-R1 e
o closeout 3.A2B. O perfil normal é TLS 1.3 mTLS, com CA
privada/administrativa e certificado individual por dispositivo cliente. O
RetroZilla NSS/NSPR foi comprovado como backend TLS legado em VC6/Windows NT
4.0 SP6, inclusive com entropia segura normal e falha de entropia fail-closed.
PapinhoSecureTransport (PST) foi posteriormente implementado como biblioteca
independente e está pronto para ser consumido pelo Accelerator através de sua
API pública. A futura integração do Secure Principal usará PST; o Accelerator
não integrará NSS/NSPR diretamente. NSS/NSPR permanece um provider legado do
PST, não uma dependência direta do Core.

PST pronto e sua boundary privada de consumo no build não significam Transport Security integrada: autenticação,
autorização e Transport Security continuam não implementadas no Accelerator.
O Accelerator fixa e valida a release PST `v0.6.0` (API 2.1/SPI 3.0) por
manifesto de dependência e possui uma prova opt-in isolada de TLS 1.3 outbound
para `google.com:443`, migrada para role CLIENT explícito.
Essa prova reutiliza a mesma boundary CMake privada reservada às futuras
unidades de segurança, mas não implementa a capability geral `TLS_OFFLOAD`,
network egress de produção, proxy ou integração com Browser. A Phase 3.B
implementou a composition privada e failure-atomic de runtime, credencial,
trust e perfil Secure Principal SERVER. O adapter privado de logging PST também
está implementado. A boundary privada de scheduling usa o wait-set PST para
multiplexar conexões seguras, sources nativas borrowed, timeout e wake, com
trabalho limitado e fairness controlados pelo Accelerator; ela ainda não está
ligada ao servidor. A integração TLS no servidor permanece não implementada. A
baseline possui modelos portáteis em C99 e um
servidor Win32 estruturalmente operacional:

| Marco | Estado |
|---|---|
| PST release integration + private consumer build boundary | ✅ |
| Accelerator → PST → TLS 1.3 Internet proof | ✅ |
| Security Composition Lifecycle | ✅; ainda não ligada ao servidor |
| PST Logging Adapter | ✅; privado, síncrono e consumer-owned |
| PST readiness/scheduler integration | ✅; privada, opt-in e não ligada ao servidor |
| `TLS_OFFLOAD` general capability | 🟨 incompleta; veja o Capability Document |
| Network Egress production | ⬜ não implementado |
| Transport Security Browser ↔ Accelerator | ⬜ não implementado |
| Secure Principal | ⬜ não implementado |

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
- Secure Principal consumirá PapinhoSecureTransport; Legacy Endpoint permanece
  plaintext explicitamente configurado e não passa pelo PST.

## Uso atual no Windows

```text
papacc_server.exe --help
papacc_server --list-interfaces
papacc_server --port <porta> --all-interfaces
papacc_server --port <porta> --interface-id <persistent-id>
papacc_server --port <porta> --all-interfaces --log-level info
```

Não existe porta oficial ou default. O modo RUN exige `--port` e uma decisão explícita de bind. `--allow-network-egress` registra somente policy; egress ainda não foi implementado.

`--log-level` aceita `off`, `error`, `warn`, `info` (default), `debug` e `trace`. `off` desabilita somente toda saída do `PAPACC_LOGGER`; saídas funcionais de `--help` e `--list-interfaces` permanecem disponíveis. INFO registra lifecycle estrutural de listeners, Connections, Sessions, tickets e DATA attachment. Eventos possuem identificador estável, categoria, componente, operação, resultado normalizado e contexto seguro limitado. Tickets completos e payloads nunca são registrados, inclusive em TRACE. IDs mostrados são somente IDs runtime locais e não são serializados no Wire Protocol.

Consulte a [CLI Reference](docs/cli.md) para todas as opções implementadas,
regras de combinação, formato de inspeção e exemplos. Os valores usados nos
exemplos não são defaults oficiais.

## Validação com consumidor real

O primeiro consumidor real foi validado: PapinhoBrowser em Windows NT 4.0 acessou por LAN TCP o PapinhoAccelerator executado em Windows moderno e concluiu `CONTROL_OPEN` → `CONTROL_ACCEPT` → solicitação de ticket → segunda conexão TCP → `DATA_ATTACH` → `DATA_ACCEPT`. PapinhoBrowser permaneceu funcional e o Accelerator permaneceu opcional. Isto valida a integração estrutural da Phase 2; não inicia Phase 3 nem implica autenticação, Transport Security ou processamento de payload DATA.

## Documentação

- [CLI Reference](docs/cli.md)
- [Arquitetura](docs/architecture.md)
- [Visão do protocolo](docs/protocol-overview.md)
- [Capabilities](docs/capabilities.md)
- [Networking](docs/networking.md)
- [Modelo de mídia](docs/media-model.md)
- [Modelo de segurança](docs/security-model.md)
- [Checkpoint de arquitetura de segurança da Phase 3](docs/phase3-security-architecture.md)
- [Perfil inicial de Transport Security e credenciais](docs/phase3-transport-security-profile.md)
- [Spike de backend TLS e compatibilidade legada](docs/phase3-tls-backend-spike.md)
- [Prova final RetroZilla NSS mTLS/NT4](docs/phase3-nss-mtls-nt4-proof.md)
- [Decisão de perfis Secure Principal e Legacy Endpoint](docs/phase3-transport-profiles.md)
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

A aquisição reproduzível do SDK PST e a prova real, ambas opt-in, usam:

```text
cmake --build build\ninja --target papacc_acquire_pst
cmake -S . -B build\ninja -G Ninja -DCMAKE_BUILD_TYPE=Debug -DPAPACC_ENABLE_PST_OUTBOUND_PROOF=ON
cmake --build build\ninja
cmake --build build\ninja --target papacc_pst_tls13_google
cmake --build build\ninja --target papacc_pst_tls13_google_wrong_hostname
```

O pin canônico está em `dependencies/papinho-secure-transport.txt`. A prova
acessa a Internet e, deliberadamente, não integra o CTest regular.
O contrato de includes, link e runtime DLLs fica centralizado no target CMake
privado `papacc_pst_consumer`; consumidores não devem repetir paths ou listas
de bibliotecas. A boundary apenas descreve consumo do SDK staged e não cria
runtime PST nem adiciona TLS ao `papacc_server`. A composition PST-backed e seu
teste também permanecem opt-in; o build baseline não adquire dependências pela
rede.

Árvores `build*` são artefatos locais ignorados pelo Git. Não coloque fontes ou definições de protocolo necessárias dentro delas.

A versão `0.1.0` é a versão do software e não congela nem atribui versão, IDs ou layout ao Wire Protocol.
