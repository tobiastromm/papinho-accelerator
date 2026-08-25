# Networking e transports

## Abstração de transport

As camadas superiores trabalham com canais abstratos e não com sockets. A arquitetura reserva:

```text
Transport
├─ TCP                 primeiro a ser implementado
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

Quando política ou configuração exigir canal seguro, todos os canais relevantes da Session devem preservar esse requisito. Desabilitar qualquer capability — inclusive `TLS_OFFLOAD` — não pode desabilitar Transport Security, e downgrade silencioso para transporte inseguro é proibido. O mecanismo concreto e a biblioteca permanecem indefinidos.

`TLS_OFFLOAD` trata separadamente de auxílio TLS para conexões do cliente com sites/serviços externos. Sua negociação não governa a proteção dos canais do PapinhoAccelerator.

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
