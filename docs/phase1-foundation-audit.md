# Phase 1 Final Foundation Audit

## Resultado

**PHASE 1 READY.** A Foundation está coerente para iniciar o design checkpoint da Phase 2. A auditoria não encontrou vazamento de recursos conhecido, ciclo de dependência, quebra de failure atomicity ou funcionalidade prematura de protocolo. Dois problemas objetivos foram corrigidos: documentação que ainda descrevia o servidor como mero esqueleto e criação incondicional do executable Win32 em builds não-Windows.

Este resultado não declara o produto pronto para clientes. Os listeners atuais não aceitam conexões e não implementam Session, protocolo, autenticação ou Transport Security.

## Escopo auditado

- tipos, resultados, tempo monotônico, logging e runtime;
- modelos portáteis de IP, interface, identidades e views;
- discovery/catálogo Win32 e apresentação UTF-8;
- Persistent/Runtime Bind Selection e Bind Target Resolution;
- WinSock, socket/bind/listen e Multi-Listener Set;
- Server Config, CLI, inspeção, Server Network e console lifecycle;
- CMake, 19 testes e documentação da Foundation.

## Arquitetura, boundaries e dependências

A direção observada é Foundation/Core → modelos portáteis de Network → backends Win32 → modelos da aplicação → CLI/composição. `src/network` não inclui nem referencia `apps/server`. Tipos nativos (`SOCKET`, `DWORD`, `LONG`, `sockaddr`, `IP_ADAPTER_ADDRESSES` e headers Windows) ficam em arquivos explicitamente Win32; `LONG` existe ainda no bridge de console privado da aplicação. Nenhum tipo nativo escapou para os modelos portáteis.

APIs portáteis reutilizáveis ficam em `include`, `src/network`, `src/runtime`, `src/platform` e `src/transports`. Server Config e parser CLI são application models portáteis. Discovery, TCP, Server Network e console são implementações/application composition Win32. `server_network.h` expõe tipos do backend Win32, deliberadamente, pois não é API portátil nem pública do produto.

Não foram encontrados ciclos no grafo CMake. Bibliotecas de sistema permanecem privadas aos backends: `ws2_32` no TCP Win32 e `iphlpapi` no discovery Win32. O executable operacional passou a existir somente dentro de `if(WIN32)`, evitando uma dependência Win32 inválida em builds portáteis.

## Ownership e lifetime

| Objeto/view | Owner | Borrower e lifetime | Cleanup |
|---|---|---|---|
| `PAPACC_RUNTIME` | caller | runtime usa logger emprestado até shutdown | `papacc_runtime_shutdown` não destrói logger |
| logger callback/context | caller | `PAPACC_LOGGER` mantém ponteiros; textos do record valem só durante callback | owner externo |
| `PAPACC_SERVER_CONFIG` | caller/config source | copia escalares; Persistent ID array é view enquanto config é consultado | nenhuma destruição própria |
| `PAPACC_PERSISTENT_BIND_SELECTION` | caller | array de Persistent IDs emprestado | nenhuma destruição |
| `PAPACC_BIND_SELECTION` | caller/resolver | array de runtime IDs emprestado | nenhuma destruição |
| discovery snapshot | caller | view sobre arrays de interface/address e arena de apresentação | caller libera os três storages |
| `presentation_name` | arena do snapshot | UTF-8 read-only, NUL-terminated, válida enquanto a arena existir | caller libera a arena |
| Bind Targets | caller | Listener Set copia target para suas entries | caller pode liberar após start |
| TCP Platform | caller | sockets/listener usam contexto inicializado | `papacc_tcp_platform_shutdown` |
| TCP Socket | caller/Listener Set | handle nativo privado ao backend | close idempotente |
| Listener Entry storage | caller ou Server Network | Listener Set referencia enquanto ativo e possui os sockets ali | Listener Set fecha; owner libera storage |
| Server Network | caller | possui listener storage, Listener Set e WinSock após publicação | shutdown idempotente |
| CLI Persistent ID storage | application layer | config referencia até `server_network_start` terminar | liberado imediatamente após startup |
| console state | main thread | handler publica somente flag process-local | uninstall determinístico |

Não existem destructors artificiais para views. O Server Network não retém config, Persistent IDs, snapshot, presentation arena, runtime IDs ou Bind Targets. Ele publica somente plataforma, Listener Set e storage de entries depois do startup integral; portanto todos os temporários são liberados com segurança após sucesso.

## Invariantes confirmados

### Inicialização e defaults

Todos os objetos stateful auditados possuem initializer determinístico: runtime, IP, interface/catalog/address, Persistent ID, presentation name, snapshots, selections, targets, TCP Platform/Socket/Listener Set, Server Config, Server Network, CLI request e console. Server Config inicia com `control_port = 0`, egress negado e bind `UNSPECIFIED`; esse estado não abre portas e não é operacionalmente válido.

### Autoridades de validação

- Persistent Selection valida sua forma e IDs.
- Server Config valida boolean, seleção e porta operacional.
- Persistent resolver é a única tradução de Persistent ID para runtime ID.
- Bind Target Resolver valida interfaces/addresses e cria targets.
- backends TCP controlam socket/listener behavior.
- Server Network apenas compõe essas autoridades e não contém `argc`, `argv` ou strings de opções.

`--list-interfaces` permanece management/inspection e não integra Server Config.

### Identidades e discovery

`interface_instance_id` é efêmero por snapshot; `persistent_id` é identidade administrativa local à máquina; `presentation_name` é somente exibição. FriendlyName não participa de igualdade, seleção, persistência ou protocolo. Address records contêm apenas IP, instance association, family-specific index e scope.

O backend realiza uma chamada lógica de `GetAdaptersAddresses` por API de discovery, preserva interfaces sem endereço, DOWN e loopback, e não filtra administrativamente. LUID e FriendlyName são protegidos por `Length`. IPv4 usa `IfIndex`; IPv6 usa `Ipv6IfIndex` e preserva `sin6_scope_id`. FriendlyName é convertido de UTF-16 para arena UTF-8 variável, com comprimento em bytes, NUL e degradação para indisponível.

`is_valid` é a única autoridade da identidade persistente: `{ TRUE, 0 }` é válido e coberto por testes. Não existe proxy `value != 0` no código de produção.

A API address-only permanece como facade legacy/internal útil para testes/callers. Ela reutiliza os mesmos helpers de enumeração, contagem e conversão; não há uma segunda implementação de discovery.

### Bind pipeline

Os modos são `UNSPECIFIED`, `ALL_INTERFACES` e `SELECTED_INTERFACES`. ALL não armazena wildcard na configuração: a resolução transforma intenção administrativa em runtime ALL e então em `0.0.0.0`/`::`. Seleção explícita é all-or-nothing tanto para Persistent IDs ausentes quanto para interfaces sem endereço bindável. Não há fallback por nome/endereço nem resultado parcial.

Dedup usa address + scope; endereços IPv6 iguais com scopes distintos continuam targets distintos, preservando a primeira ocorrência. IPv6 mantém 16 bytes em network order, scope separado, sockaddr correto e output visual `[IPv6]:port`.

### Portas e TCP

Porta zero é válida no primitive TCP para ephemeral bind, mas inválida no Server Config operacional. A nomenclatura administrativa é exclusivamente `control_port`; não há `listen_port` ou `server_port` no código.

WinSock solicita 2.2, valida a versão retornada, limpa falhas, rejeita double init, aceita contexts independentes e permite reuse após shutdown. Socket usa handle local até bind bem-sucedido, `SO_EXCLUSIVEADDRUSE`, nunca `SO_REUSEADDR`, sockaddr IPv4/IPv6 correto e rollback em falha. Close é idempotente. O primitive mantém um socket bound válido se `listen()` falhar.

O Listener Set mantém uma porta lógica, reutiliza a porta ephemeral resolvida, degrada somente families `NOT_SUPPORTED`, compacta entries ativas, desfaz outros erros em ordem reversa e suporta shutdown idempotente/reuse.

### Server Network, CLI e console

Server Network executa: config validation → discovery → persistent resolution → target resolution → WinSock → Listener Set → publicação. WinSock só é adquirido depois de todas as etapas lógicas. Falhas fazem rollback integral.

A CLI faz parsing decimal independente de locale, detecta overflow até `U64_MAX`, aceita ID zero, rejeita duplicatas/conflitos, implementa sizing atômico e não esconde heap dentro do config. Heap no `main` é caller storage e possui multiplicação protegida.

O handler de console trata Ctrl+C/Ctrl+Break somente com publicação interlocked e retorno. Não imprime, aloca, libera ou executa cleanup. CLOSE/LOGOFF/SHUTDOWN retornam `FALSE`, sem promessa falsa. O bridge global mínimo é necessário porque `SetConsoleCtrlHandler` não recebe contexto; config, network, runtime e sockets não são globais. O único wait permanente é o polling de 50 ms no executable; `server_network_start` permanece não bloqueante.

### Segurança, egress e Phase 2

`allow_network_egress` é policy configurada, não funcionalidade. Não existem proxy, outbound connection, TLS relay ou HTTP fetch. `Transport Security != TLS Offload`; nenhum dos dois está implementado. Não existem `accept`, `WSAAccept`, `AcceptEx`, `send`, `recv`, connection objects, Session, handshake, framing ou protocolo no código da Foundation.

## Failure atomicity, erros e tamanhos

Discovery, persistent resolution, target resolution, TCP bind, Listener Set, CLI parse e Server Network inicializam outputs/estado de publicação somente depois de validações e desfazem recursos locais nas falhas. A taxonomia observada segue: malformed model → `INVALID_ARGUMENT`; impossibilidade no estado/topologia → `INVALID_STATE`; capacity/limite → `LIMIT_EXCEEDED`; alocação → `OUT_OF_MEMORY`; family/operação ausente → `NOT_SUPPORTED`; falha nativa inesperada → `INTERNAL_ERROR`. Não existe `PAPACC_RESULT_LIMIT` transitório.

Multiplicações de arrays de interface, address, runtime ID, target, listener entry e CLI ID possuem guards por `SIZE_MAX`. Acúmulo da arena verifica soma; instance IDs limitam interface count a `UINT32_MAX`; parsing decimal verifica overflow antes de multiplicar; tempo monotônico evita overflow no produto. As APIs fundamentais continuam caller-storage oriented; heap existe nos backends/Application Layer com ownership explícito.

## Achados classificados

### FIXED NOW

1. README e documentos descreviam o estado como apenas baseline/smoke ou TCP ainda futuro. Foram alinhados aos listeners reais e às ausências da Phase 2.
2. `papacc_server` era criado fora do conditional Win32 embora seu composition root dependesse de Winsock/console. O target foi limitado a Windows.
3. Ownership/lifetime de temporários do Server Network e estado de console estavam corretos no código, mas pouco explícitos nos headers; os contratos foram documentados.

### ACCEPTED FOR PHASE 1

- Resolução/dedup e detecção de duplicatas usam algoritmos quadráticos; os conjuntos de interfaces são pequenos e não justificam estrutura adicional nesta fase.
- Polling de console em 50 ms é simples, não faz busy-spin e está restrito ao lifecycle do executable.
- Bridge process-global de duas flags interlocked para um runner por processo.
- API legacy address-only permanece por compatibilidade e compartilha implementação.
- `server_network` é application-private e explicitamente Win32; uma composição portable ainda não é necessária.

### DEFERRED TO PHASE 2

- accept e connection ownership;
- framing/parser e limites de mensagens;
- Session, Control/Data Channels e associação segura;
- autenticação/autorização, heartbeat, timeouts e cancellation;
- desenho e implementação de Transport Security;
- negotiation/IDs/versionamento de capabilities e jobs.

Nenhum message ID, capability numeric ID, magic number ou wire layout foi congelado.

### DEFERRED TO LATER PHASE

- backends POSIX/Linux e outros transports;
- GUI/Management Plane;
- arquivo/Registry e formato de configuração;
- network egress/proxy/outbound e `TLS_OFFLOAD`;
- Compute Backends e processamento de mídia;
- política de release/versionamento e porta oficial;
- tratamento ampliado de CTRL_CLOSE/LOGOFF/SHUTDOWN.

## Compatibilidade Win32 e C99

Não existem defines novos de `WINVER`, `_WIN32_WINNT` ou `NTDDI_VERSION`. As APIs usadas são compatíveis com o baseline histórico pretendido nesta camada. LUID é lido somente quando `IP_ADAPTER_ADDRESSES.Length` cobre o membro, sem impor LUID ao modelo portátil. Todos os targets configurados usam C99, extensions OFF e warnings `/W4`; não há C11 atomics, C++ ou supressão global de warnings.

## Inventário final de testes

1. `papacc.version`: versão do software.
2. `papacc.types`: larguras e tipos foundation.
3. `papacc.server.configuration`: defaults, cópia e validação do config.
4. `papacc.server.cli`: parsing, sizing, conflitos, overflow e IDs.
5. `papacc.server.cli_interfaces`: apresentação do catálogo/snapshot.
6. `papacc.server.network`: composição real, rollback, ALL, SELECTED e reuse.
7. `papacc.server.console_win32`: install/uninstall, exclusividade e stop flag.
8. `papacc.network.ip_address`: construção, igualdade e formatting IPv4/IPv6.
9. `papacc.network.bind_selection`: formas e validação contra snapshot.
10. `papacc.network.persistent_bind_selection`: validade, zero válido e resolução atômica.
11. `papacc.network.bind_target`: ALL, selected, dedup, scope e all-or-nothing.
12. `papacc.network.interface_model`: initializers e identidades/views.
13. `papacc.transport.tcp_platform.win32`: WSA lifecycle.
14. `papacc.transport.tcp_socket_bind.win32`: bind, exclusividade, rollback e close.
15. `papacc.transport.tcp_listener_set.win32`: porta compartilhada, family degradation e rollback.
16. `papacc.network.discovery.win32`: captura/consistência real sem topologia hardcoded.
17. `papacc.pal.monotonic_time`: monotonicidade e contratos de tempo.
18. `papacc.runtime.logging`: callback, filtros e timestamp.
19. `papacc.runtime.lifecycle`: init/shutdown e logger emprestado.

Os testes de ambiente real não fixam FriendlyName, Persistent ID, Wi-Fi ou disponibilidade universal de IPv6. Skips são permitidos apenas quando a topologia necessária não existe. Não há sleeps arbitrários em testes de networking; o polling de 50 ms pertence somente ao runtime console. CTest permanece o gate e não depende de automação GUI.

## Validação final

- clean configure: aprovado;
- build completo: aprovado, zero warnings;
- CTest: 19/19 aprovado;
- smoke sem argumentos: exit 0, versão 0.1.0;
- `--list-interfaces`: exit 0, catálogo real completo;
- RUN ALL: listeners wildcard reais e shutdown Ctrl+C com exit 0;
- RUN SELECTED: loopback real selecionado e shutdown Ctrl+C com exit 0;
- portas dos testes fechadas após shutdown;
- `git diff --check`: aprovado;
- nenhum helper temporário deixado no repositório.

## Phase 2 readiness checklist

- [x] portable result/types stable enough
- [x] network interface model stable
- [x] server lifecycle stable
- [x] TCP listener ownership stable
- [x] shutdown lifecycle stable
- [x] no known resource leaks
- [x] no known layering violations
- [x] no known failure-atomicity violations
- [x] tests green
- [x] docs aligned

O próximo passo permitido é um design checkpoint próprio da Phase 2. Este audit não autoriza inferir ou congelar o Wire Protocol.
