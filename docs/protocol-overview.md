# Visão geral do protocolo

Phase 2.D3B integra `CONTROL_OPEN` -> `CONTROL_ACCEPT` ao RUN mode Win32 real.
Depois do estabelecimento, o processor permanece `ESTABLISHED` e não consome
novas mensagens. DATA association, autenticação, Transport Security,
capabilities e o dispatcher Control geral continuam não implementados.

Este documento define a visão conceitual das futuras mensagens. O envelope comum de bytes foi congelado separadamente em [Protocol Framing](protocol-framing.md), sem atribuir tipos numéricos de mensagens ou payloads.

**Estado de implementação:** existem foundations runtime de Connection, Session
e Channel, encoder/parser, Framed Reader e Framed Writer portáteis. O executable
processa apenas as duas mensagens de estabelecimento pelo I/O Loop Win32
combinado. A arquitetura normativa está em
[Connection I/O Scheduling](connection-io-scheduling.md).
Os nomes de mensagens abaixo continuam não congelados, exceto pelos primeiros
tipos normativos `CONTROL_OPEN` (`0x0001`) e `CONTROL_ACCEPT` (`0x0002`),
definidos em [Control Establishment Protocol](control-establishment-protocol.md).

## Canais e fluxo conceitual

1. Um Control Channel cria uma Session.
2. As partes negociam versões, identidade/autenticação quando exigida e capabilities.
3. O servidor aplica autorização e política e retorna a configuração efetiva.
4. A Session entra em `READY`/`ACTIVE`; jobs usam controle no Control Plane e cargas grandes no Data Plane.
5. Data Channels adicionais são associados de forma segura à Session antes de transportar payload.
6. Erro, timeout ou encerramento conduzem a cleanup determinístico.

Mensagens de controle não devem ficar indefinidamente bloqueadas atrás de grandes payloads. Backpressure, limites e cancelamento devem valer por canal/job.

## Envelope comum congelado

Envelope 1.0 possui header fixo de 16 bytes, magic `PACC`, versão de envelope 1.0, Header Length, Message Type U16, Flags U16 e Payload Length U32, com inteiros multi-byte big-endian. IDs de Session, Channel, Connection e correlação não pertencem ao header universal; metadata futura pertence à semântica de mensagens específicas.

O contrato normativo completo, exemplos golden e integrações portáteis de
stream estão em [Protocol Framing](protocol-framing.md). Nenhum Message Type
real é atribuído por este overview.

## Vocabulário de mensagens não congelado

`CONTROL_OPEN` e `CONTROL_ACCEPT` não fazem parte da lista especulativa abaixo;
seus IDs, direções e payloads 1.0 já estão congelados no documento normativo.

```text
HELLO / WELCOME
AUTH / AUTH_RESULT
CAPABILITIES / CAPABILITY_CONFIG
PING / PONG
DATA_CHANNEL_REQUEST / DATA_CHANNEL_ACCEPT
REQUEST / RESPONSE / PROGRESS / CANCEL / COMPLETE
ERROR / DISCONNECT
```

`HELLO/WELCOME` negociam compatibilidade, não presumem sucesso. `CAPABILITIES` anuncia suporte; `CAPABILITY_CONFIG` expressa o resultado permitido. Operações longas precisam de correlação, progresso, cancelamento e conclusão inequívoca. `ERROR` deve distinguir falha de mensagem, job, capability, canal e Session sem revelar segredos.

## Compatibilidade

Versão e capability são eixos distintos. Uma versão de protocolo compatível não implica suporte a toda capability. Novos peers devem negociar um subconjunto comum; caso não exista base segura, devem encerrar com erro explícito. Extensões deverão ser delimitadas e versionadas, preservando parsers antigos. Não há garantia de retomada de Session nesta baseline.

## TCP e futuro UDP

TCP será o primeiro transport e suportará Control Channel e Data Channels. UDP permanece opcional para avaliação futura de casos sensíveis a latência. Nenhum protocolo de mídia UDP, porta ou mecanismo de confiabilidade é especificado nesta fase.
