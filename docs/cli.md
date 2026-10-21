# CLI Reference

Esta é a referência operacional canônica da Command-Line Interface atualmente
implementada pelo servidor Win32 do PapinhoAccelerator. O executável produzido
é `papacc_server.exe`; exemplos podem abreviar seu nome para `papacc_server`
quando a extensão estiver implícita no ambiente.

Esta página documenta o comportamento observado em `apps/server/main.c`,
`apps/server/server_cli.c`, `apps/server/server_cli_interfaces.c` e nos testes.
Ela não define uma API, wire format ou formato futuro de configuração.

## Visão geral

| Objetivo | Exemplo |
|---|---|
| Mostrar ajuda | `papacc_server.exe --help` |
| Listar interfaces | `papacc_server.exe --list-interfaces` |
| Escutar em todas as interfaces | `papacc_server.exe --port 4433 --all-interfaces` |
| Selecionar uma interface | `papacc_server.exe --port 4433 --interface-id 123` |
| Selecionar várias interfaces | `papacc_server.exe --port 4433 --interface-id 123 --interface-id 456` |
| Usar log DEBUG | `papacc_server.exe --port 4433 --all-interfaces --log-level debug` |
| Habilitar egress com policy/profile explícitos | `papacc_server.exe --port 4433 --all-interfaces --listener-profile secure-principal --security-configuration-ref production --allow-network-egress --egress-policy-ref controlled --egress-profile-ref default-route` |
| Iniciar Legacy Endpoint explicitamente | `papacc_server.exe --port 4433 --all-interfaces --listener-profile legacy-endpoint` |
| Iniciar Secure Principal | `papacc_server.exe --port 4433 --all-interfaces --listener-profile secure-principal --security-configuration-ref nt4-lab-secure` |

Todos os números dessa tabela são exemplos. `4433` não é porta default,
reservada ou oficialmente recomendada.

Uma invocação sem argumentos imprime o banner de Foundation e a versão do
software e encerra com sucesso. Ela não inicia o servidor em RUN mode.

## Management / Inspection Commands

### `--help`

```text
papacc_server.exe --help
```

Mostra a ajuda compilada no executável e encerra. O comando deve aparecer
sozinho; combiná-lo com qualquer outra opção é inválido.

### `--list-interfaces`

```text
papacc_server.exe --list-interfaces
```

`--list-interfaces` é um **Management / Inspection Command**, não uma Server
Configuration Source. Ele descobre o snapshot atual, agrupa os endereços por
interface e fornece o Persistent ID usado por `--interface-id`. Não abre
listeners, não entra em RUN mode e não altera configuração persistente.

O formato atual é:

```text
Interface: <presentation name ou (unnamed interface)>
  Runtime ID: <unsigned decimal>
  Persistent ID: <unsigned decimal ou unavailable>
  State: <UP ou DOWN>
  Loopback: <yes ou no>
  IPv4: <address>
  IPv6: <address>    Scope ID: <unsigned decimal, quando não zero>

Use an interface with:
  --interface-id <Persistent ID>

Use all interfaces with:
  --all-interfaces
```

Uma interface sem endereço imprime `Addresses: none`. As linhas IPv4/IPv6 se
repetem conforme os endereços atuais. Runtime ID, presentation name, endereço
IP e interface index não substituem o Persistent ID na seleção administrativa.

A saída de `--help` e `--list-interfaces` é saída funcional, não logging. Os
dois comandos continuam disponíveis independentemente do logger; pela
gramática atual, cada um deve ser invocado sozinho, sem `--log-level off`.

## RUN mode

O modo operacional exige simultaneamente:

```text
--port <port>
        +
exatamente uma intenção de bind:
    --all-interfaces
    OU
    um ou mais --interface-id <persistent-id>
```

Não existe porta default oficial nem bind default implícito. O Server
Configuration Model começa com bind `UNSPECIFIED` e `control_port = 0`; essa
configuração não é operacional para listening.

### `--port <port>`

Faixa aceita no RUN mode:

```text
--port <1..65535>
```

O valor é decimal unsigned composto apenas por dígitos. Zero é rejeitado pela
validação operacional, e valores acima de `65535` são rejeitados pelo parser.
`--port` pode aparecer uma única vez.

### `--all-interfaces`

```text
papacc_server.exe --port 4433 --all-interfaces
```

Esta opção produz a intenção administrativa `ALL_INTERFACES` no Shared Server
Configuration Model. Ela não armazena `0.0.0.0` ou `::` como identidade ou
configuração. Os wildcards surgem somente como Bind Targets durante a resolução
runtime, quando aplicáveis.

`--all-interfaces` pode aparecer uma única vez e é mutuamente exclusivo com
qualquer `--interface-id`.

### `--interface-id <persistent-id>`

```text
papacc_server.exe --port 4433 --interface-id 123
papacc_server.exe --port 4433 --interface-id 123 --interface-id 456
```

A opção pode ser repetida para selecionar múltiplas interfaces. Cada valor é a
representação textual atual da CLI para um unsigned 64-bit decimal, no intervalo
de `0` a `18446744073709551615`, inclusive. Somente dígitos decimais são
aceitos: sinal, hexadecimal e texto misto são inválidos. Zero é um Persistent
ID válido quando sua flag de validade também é verdadeira.

Valores duplicados são rejeitados, inclusive quando grafias decimais diferentes
representam o mesmo número. Essa representação pertence à CLI atual; não é wire
format nem formato universal de configuração ou persistência.

O ID é o Persistent ID local da interface, não `interface_instance_id`,
presentation/friendly name, endereço IP ou interface index. A seleção explícita
é resolvida contra o snapshot atual conforme o [modelo de
networking](networking.md).

### `--allow-network-egress`

```text
papacc_server.exe --port 4433 --all-interfaces --allow-network-egress
```

Esta opção configura `allow_network_egress = TRUE`. É apenas uma entrada
administrativa/policy flag no Configuration Model atual:

- não implementa network egress;
- não abre conexões externas;
- não concede egress por si só a clientes;
- não altera Transport Security;
- não habilita nem implica `TLS_OFFLOAD`.

A opção pode aparecer uma única vez. O estado e as limitações estão no
[Capability Document de Network Egress](capabilities/network-egress.md).

Ela somente é válida em `secure-principal` e exige simultaneamente
`--egress-policy-ref` e `--egress-profile-ref`. As referências são identidades
opacas do Configuration Model; precisam ser resolvidas antes de RUN e não
representam callbacks nem caminhos implícitos. Referência ausente ou desconhecida
faz o startup falhar fechado. `legacy-endpoint` continua sem Network Egress.

### `--egress-policy-ref` e `--egress-profile-ref`

```text
--egress-policy-ref <reference>
--egress-profile-ref <reference>
--outbound-tls-profile-ref <reference>
```

As opções selecionam, respectivamente, a Destination Policy e o Egress Profile.
Elas não são defaults implícitos e não podem ser usadas sem
`--allow-network-egress`. A source de laboratório Win32 resolve as referências
pelas variáveis `PAPACC_EGRESS_POLICY_REF` e `PAPACC_EGRESS_PROFILE_REF`; regras
exatas bounded são fornecidas por índices `PAPACC_EGRESS_RULE_<n>_HOST`,
`PAPACC_EGRESS_RULE_<n>_IPV4` e `PAPACC_EGRESS_RULE_<n>_PORT` (`0 <= n < 16`).
Esse formato é uma Configuration Source factual, não uma linguagem arquitetural
congelada.

`--outbound-tls-profile-ref` seleciona separadamente o profile de segurança TLS
CLIENT usado em `TLS_REQUIRED`. `system-tls13` usa o trust store do sistema. Um
profile lab privado deve coincidir com `PAPACC_OUTBOUND_TLS_PROFILE_REF` e usa
`PAPACC_OUTBOUND_TLS_CA_DER_B64` como trust anchor explícita. Esse material não
pertence à Destination Policy e nunca reutiliza implicitamente a CA inbound.

### `--listener-profile <profile>`

Valores implementados:

```text
legacy-endpoint
secure-principal
```

O default compatível é `legacy-endpoint`. A escolha é explícita e nunca é
inferida por porta, certificado ou bytes recebidos. Não existe fallback entre
perfis. Legacy é plaintext. Secure Principal exige PST, TLS 1.3, mTLS e ALPN
`papacc/1` antes de publicar uma Connection PACC.

### `--security-configuration-ref <reference>`

Obrigatória com `secure-principal` e proibida com `legacy-endpoint`. A
referência possui de 1 a 64 bytes e identifica inputs resolvidos por uma
Configuration Source application-owned; ela não contém certificado, private
key ou senha.

A fonte Win32 atualmente disponível é lab-only e resolve a referência e os
caminhos de credenciais por environment variables. Consulte
[Secure Principal listener — operação de laboratório](secure-principal-operation.md)
para os nomes exatos, provisioning e higiene de chaves.

### `--log-level <level>`

Valores implementados:

```text
off
error
warn
info
debug
trace
```

O default do RUN mode é `info`. A opção pode aparecer uma única vez e os nomes
são aceitos exatamente em minúsculas.

`--log-level off` desliga todos os eventos do `PAPACC_LOGGER`; não desliga
saída funcional nem a operação do servidor. Consulte [Logging e diagnóstico
operacional](capabilities/logging.md).

## Combinações inválidas

O parser e os testes atuais rejeitam:

- RUN mode sem `--port`;
- RUN mode sem `--all-interfaces` e sem `--interface-id`;
- `--all-interfaces` combinado com qualquer `--interface-id`;
- repetição de `--port`, `--all-interfaces`, `--allow-network-egress` ou
  `--log-level`;
- perfil desconhecido ou repetido;
- Secure Principal sem `--security-configuration-ref`;
- Legacy Endpoint com `--security-configuration-ref`;
- Persistent IDs numericamente duplicados;
- porta ausente, zero, não decimal ou fora de `1..65535`;
- Persistent ID ausente, com sinal, hexadecimal, não decimal ou maior que
  `UINT64_MAX`;
- valor de log diferente de `off`, `error`, `warn`, `info` ou `debug`;
- `--help` ou `--list-interfaces` combinado com outra opção;
- opções desconhecidas, opções curtas, argumentos posicionais e sintaxe
  de opção longa com valor unido por `=`.

Erros de parsing ou validação imprimem `Invalid server command: <RESULT>` em
stderr e encerram com status não zero.

## Configuration Source versus inspeção

As opções de RUN mode alimentam o modelo compartilhado:

```text
CLI arguments
      ↓
CLI parser
      ↓
PAPACC_SERVER_CONFIG
      ↓
validation / interface resolution
      ↓
server composition
```

O comando de inspeção segue outro fluxo:

```text
--list-interfaces
      ↓
management / inspection
      ↓
functional output
```

A CLI é a primeira Configuration Source concreta. GUI e persisted
configuration futuras devem convergir para o mesmo Shared Configuration Model.
Esta referência não define opção para carregar configuração, formato/path de
arquivo, Registry, precedência entre sources ou implementação de GUI.

## Sincronização com `--help`

O texto atual de ajuda cobre `--help`, `--list-interfaces`, `--port`,
`--all-interfaces`, `--interface-id`, `--listener-profile`,
`--security-configuration-ref`, `--allow-network-egress`,
`--egress-policy-ref`, `--egress-profile-ref`,
`--outbound-tls-profile-ref`, `--log-level`
e todos os níveis de log implementados. A sintaxe resumida também informa que
`--interface-id` pode ser repetido para selecionar múltiplas interfaces.

Não há `HELP OUTPUT GAP` conhecido entre as opções implementadas pelo parser e
o texto atual de `--help`.

## Documentos relacionados

- [Networking e transports](networking.md)
- [Logging e diagnóstico operacional](capabilities/logging.md)
- [Network Egress](capabilities/network-egress.md)
- [Arquitetura](architecture.md)
- [Secure Principal listener — operação de laboratório](secure-principal-operation.md)
