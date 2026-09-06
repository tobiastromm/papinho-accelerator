# Networking e transports

Decisões arquiteturais canônicas relacionadas:

- [ADR-0002 — Core portável entre deployments e backends substituíveis](adr/ADR-0002-core-portavel-entre-deployments-e-backends-substituiveis.md);
- [ADR-0005 — Negociação de capabilities, disponibilidade de backend e autoridade de policy](adr/ADR-0005-negociacao-de-capabilities-disponibilidade-de-backend-e-autoridade-de-policy.md);
- [ADR-0007 — Identidade persistente de interface e resolução runtime de bind](adr/ADR-0007-identidade-persistente-de-interface-e-resolucao-runtime-de-bind.md).

## Estado implementado na Phase 1

O backend Win32 já implementa discovery, bind e listeners TCP. O fluxo operacional é:

```text
CLI Persistent Selection
          ↓
discovery snapshot canônico
          ↓
persistent ID -> interface_instance_id atual
          ↓
PAPACC_BIND_SELECTION runtime
          ↓
PAPACC_BIND_TARGET(s)
          ↓
WinSock + Multi-Listener Set
```

Comandos disponíveis:

```text
papacc_server --list-interfaces
papacc_server --port <porta> --all-interfaces
papacc_server --port <porta> --interface-id <persistent-id>
```

`--list-interfaces` é inspeção e não abre listeners. `--all-interfaces` é intenção administrativa; somente a resolução de targets produz `0.0.0.0` e `::`. A seleção persistente por interface é resolvida integralmente contra um único snapshot e não usa FriendlyName como identidade. O `control_port` é obrigatório e não possui default oficial.

## Future GUI — Listener Interface Selection

A futura GUI deve expressar a mesma intenção administrativa do Shared Server
Configuration Model. Ela não está implementada, e este exemplo é apenas uma
possível apresentação: valores, labels, porta e layout não estão congelados.

```text
┌──────────────────────────────────────────────┐
│ PapinhoAccelerator Server                    │
│                                              │
│ Porta:                                       │
│ [ 4433 ]                                     │
│                                              │
│ Escutar em:                                  │
│                                              │
│ ( ) Todas as interfaces                     │
│     IPv4 wildcard: 0.0.0.0                  │
│     IPv6 wildcard: ::                       │
│                                              │
│ (*) Interfaces selecionadas                 │
│                                              │
│ [x] Ethernet                                │
│     192.168.1.120                            │
│     fe80::...                                │
│                                              │
│ [x] Wi-Fi                                   │
│     192.168.0.15                             │
│                                              │
│ [ ] VPN                                     │
│     10.8.0.2                                 │
│                                              │
│ [ ] Loopback                                │
│     127.0.0.1                                │
│     ::1                                      │
│                                              │
│ [ Iniciar servidor ]                        │
└──────────────────────────────────────────────┘
```

### Intenção explícita e fail-safe default

`ALL_INTERFACES` não é o default implícito. O modelo inicializa em:

```text
UNSPECIFIED
```

Nesse estado, nenhuma interface foi escolhida, nenhum wildcard é presumido e
nenhum listener deve ser aberto apenas por default de configuração. A
configuração ainda não está operacionalmente completa para listening. O
operador deve escolher explicitamente `ALL_INTERFACES` ou
`SELECTED_INTERFACES`, pois escutar em todas as interfaces amplia a superfície
de exposição do serviço.

`ALL_INTERFACES` também não significa uma interface chamada `0.0.0.0`:

```text
ALL_INTERFACES                    intenção administrativa
        ↓ bind resolution
0.0.0.0 e ::                     wildcard targets runtime, quando aplicáveis
```

Wildcards não substituem nem identificam a intenção persistida de selecionar
todas as interfaces.

### Interfaces agrupam seus endereços

A GUI deve apresentar o catálogo por interface, não como lista plana de IPs:

```text
Interface
    ├── presentation name
    ├── status UP/DOWN, quando disponível
    ├── loopback metadata
    └── current addresses
         ├── IPv4
         └── IPv6
```

O usuário seleciona semanticamente interfaces; os endereços são fatos atuais
associados a elas. `SELECTED_INTERFACES` pode conter uma ou várias interfaces,
como Ethernet + Wi-Fi ou Ethernet + VPN. Selecionar uma interface considera os
endereços bindáveis IPv4/IPv6 atuais dela conforme o contrato vigente, e a
resolução produz todos os Bind Targets aplicáveis. Uma seleção explicitamente
configurada continua all-or-nothing quando não puder ser resolvida pelas regras
existentes; não há resultado parcial silencioso.

Discovery não escolhe a “melhor” interface:

```text
Discovery                       fornece fatos
Selection Policy / Config       expressa intenção do operador
Bind Resolution                 converte intenção em targets runtime
```

Não há heurística para preferir Ethernet ou Wi-Fi, ignorar VPN/loopback,
escolher o primeiro IP ou decidir um “melhor endereço”. Interfaces DOWN,
loopback, VPN, virtuais, IPv6-only ou com múltiplos endereços não devem ser
ocultadas automaticamente pelo discovery/catalog. A GUI poderá mostrar status,
riscos ou impossibilidade técnica e desabilitar escolhas inviáveis, mas essa UX
final ainda não está definida e não pode substituir silenciosamente a intenção
administrativa.

### Identidade e apresentação

```text
Persistent ID                    configuração/persistência
interface_instance_id            agrupamento no snapshot runtime
interface index                  dado técnico runtime
presentation/friendly name       apresentação para GUI/humano
IP addresses                     estado atual
```

O presentation name já existe no catálogo atual como metadata UTF-8 quando
disponível. A GUI poderá exibi-lo, mas a seleção persistível usa a identidade
persistente conforme o ADR-0007, nunca FriendlyName, endereço IP,
`interface_instance_id` ou índice runtime como identidade canônica.

### Fontes convergem para o mesmo modelo

Conforme `PapinhoEngineering/ADR-0004`, frontend e source não criam semânticas
paralelas:

```text
CLI parser ----------------┐
GUI futura ----------------┼──> Shared Server Configuration Model
Persisted source futura ---┘
                                  ↓
                              Validation
                                  ↓
                         Interface resolution
                                  ↓
                            Bind Targets
                                  ↓
                         Server composition
```

Não existem “GUI Server” e “CLI Server” distintos. A GUI não deve ter como
arquitetura principal montar uma command line e iniciar outra instância; cada
frontend adapta sua entrada ao mesmo modelo compartilhado. Depois da
normalização:

```text
CLI selection == GUI selection == future persisted selection
```

Hoje, a CLI expressa as escolhas por `--all-interfaces` ou uma ou mais
ocorrências de `--interface-id`; nenhuma sintaxe adicional é definida aqui.
Uma future Persisted Configuration Source poderá carregar/salvar a mesma
semântica, mas formato, extensão, path, Registry, precedência e auto-save não
estão decididos.

`PAPACC_SERVER_NETWORK` e o Listener Set permanecem infraestrutura de
listening: não possuem Sessions, processors de protocolo ou ownership das
Connections aceitas. Na composição atual do servidor, as camadas superiores
já aceitam Connections a partir desses listeners e processam os fluxos
estruturais CONTROL e DATA por meio do acceptor, I/O loop, processors e
managers apropriados.

## Abstração de transport

As camadas superiores trabalham com canais abstratos e não com sockets. A arquitetura reserva:

```text
Transport
├─ TCP                 listener foundation implementada no Win32
├─ LOCAL_PCI           futuro
├─ LOCAL_ISA           futuro
└─ outros              futuro
```

Essas entradas não prometem suporte e não pressupõem que transports locais imitem TCP. Semântica de ordenação, confiabilidade, MTU, descoberta e segurança deverá ser declarada por cada implementação sem contaminar o Core. Nenhum detalhe ISA/PCI é definido aqui.

## Topologia TCP planejada

```text
Session
├─ Control Channel
├─ Data Channel 1
├─ Data Channel 2
└─ Data Channel N
```

O Control Channel conduz negociação e lifecycle; Data Channels carregam volumes grandes. Criação, autenticação, vínculo à Session, limites, fechamento e comportamento após perda do controle precisam ser especificados antes da implementação. Um identificador sozinho não é prova suficiente de vínculo.

## Transport Security

Transport Security é a segurança da comunicação `PapinhoAccelerator Client ↔ PapinhoAccelerator Server`. Pertence à infraestrutura e ao protocolo de comunicação, protegendo Control Channel, Data Channels, autenticação, comandos, payloads, credenciais e dados enviados para processamento. Não é uma capability comum.

Quando política ou configuração exigir canal seguro, todos os canais relevantes da Session devem preservar esse requisito. Desabilitar qualquer capability — inclusive `TLS_OFFLOAD` — não pode desabilitar Transport Security, e downgrade silencioso para transporte inseguro é proibido.

O perfil concreto do `Secure Principal` é TLS 1.3 mTLS, conforme o
[ADR-0006](adr/ADR-0006-perfil-de-transport-security-e-credenciais-do-secure-principal.md).
PapinhoSecureTransport (PST) já existe e será a implementação da fronteira
Secure Transport consumida pelo Accelerator. Nenhum provider PST é universal:
o provider pode variar por target. RetroZilla NSS/NSPR foi comprovado
tecnicamente como provider legado, mas permanece encapsulado pelo PST e não é
integrado diretamente ao Core do Accelerator.

```text
Transport
    ↓
PST public API
    ↓
provider-specific secure transport
    ↓
decrypted/authenticated stream for Accelerator processing
```

O Transport genérico continua separado: Legacy Endpoint segue diretamente por
Transport/PACC plaintext explicitamente configurado e não usa PST, backend
plaintext, `PST_BACKEND_NONE` ou `PST_TLS_OFF`.

O scheduler do Accelerator deve integrar o contrato de readiness do PST em vez
de presumir equivalência com readiness do socket. No provider NSS, a evidência
comprovou `PR_Poll` sobre o descriptor SSL; outros providers podem usar seu
próprio mecanismo. A composição preservará os invariantes nonblocking, bounded
e fair do ADR-0004.

`TLS_OFFLOAD` trata separadamente de auxílio TLS para conexões do cliente com sites/serviços externos. Sua negociação não governa a proteção dos canais do PapinhoAccelerator.

## Compatibilidade com clientes extremamente antigos

O [ADR-0001](adr/ADR-0001-secure-principal-e-legacy-endpoint.md) preserva o
Legacy Endpoint como possibilidade futura para clientes incapazes de cumprir o
perfil Secure Principal. Um sistema da geração Windows 3.11 é um exemplo
concreto dessa classe de cliente extremamente antigo. Isso é somente um caso de
uso arquitetural: não declara suporte atual, compromisso de implementação nem
associa obrigatoriamente qualquer versão do Windows a um transport profile.
Windows 95/98 ou Windows NT 4.0, por exemplo, não são classificados
automaticamente como Legacy Endpoint, e toolchain não determina o profile.

```text
cliente extremamente antigo
        ↓
Legacy Endpoint
PACC plaintext em LAN/rede administrativamente isolada
        ↓
PapinhoAccelerator
        ↓
capabilities/protocolos modernos permitidos por policy
```

A finalidade é permitir que uma máquina muito antiga possa futuramente usar o
Accelerator como ponte para capacidades modernas sem precisar executar toda a
stack moderna localmente. Isso não torna seguro o hop entre o cliente legado e
o Accelerator: PACC permanece plaintext nesse trecho e pode ser observado ou
alterado por um atacante com acesso adequado à rede. Isolamento por listener,
porta, interface, rede ou VLAN reduz exposição operacional, mas não cria
confidencialidade, integridade ou identidade criptográfica.

O mesmo PACC é usado nos dois transport profiles; não existem variantes “PACC
seguro” e “PACC inseguro”. A arquitetura escolhe explicitamente entre Secure
Principal TLS 1.3-only e Legacy Endpoint reconhecidamente plaintext, em vez de
uma cadeia oportunista `TLS 1.3 → TLS 1.2 → plaintext`. Essa regra não afirma
que TLS 1.2 seja genericamente inseguro: apenas registra que ele não é fallback
do perfil Secure Principal definido para o PapinhoAccelerator.

Legacy Endpoint não concede Internet ou egress pelo Accelerator. Caso um fluxo
futuro precise de conexão externa, `NETWORK_EGRESS` continuará sendo autoridade
separada, dependente de configuração, autorização e policy. Compute remoto
também não concede egress, e nenhum desses comportamentos está implementado na
baseline atual.

## Compute offload versus network egress

Processamento no Accelerator e origem de conexões externas são decisões independentes:

- `NETWORK_EGRESS_CLIENT`: o cliente acessa a Internet; o destino vê o IP público do cliente. Dados podem ser enviados ao Accelerator para processamento quando o fluxo permitir.
- `NETWORK_EGRESS_ACCELERATOR`: o Accelerator abre a conexão externa; o destino vê o IP público do Accelerator.

Egress pelo Accelerator só pode ocorrer quando:

```text
SERVER_ALLOWS AND CLIENT_REQUESTS
```

A falta de qualquer condição nega o egress. O servidor pode oferecer CPU/GPU/RAM e proibir completamente o uso de seu IP. A preferência deve ser explícita e não herdada de outra capability.

## Requisitos futuros de segurança de egress

Egress remoto será uma autoridade privilegiada e deverá aplicar validação de destino em cada resolução/conexão, política de portas/protocolos, quotas, logging cuidadoso e resistência a DNS rebinding/redirecionamentos. Sem política explícita, não poderá alcançar localhost/loopback, interfaces administrativas, endereços privados/link-local, LAN ou recursos internos. Casos como IPv4/IPv6, nomes que resolvem para múltiplos endereços e redirects deverão fazer parte do Threat Model.

## UDP

UDP não será implementado inicialmente. Poderá ser avaliado no futuro para latência, após definição de segurança, congestion control, confiabilidade e sincronização apropriadas. Esta baseline não projeta mídia sobre UDP.
