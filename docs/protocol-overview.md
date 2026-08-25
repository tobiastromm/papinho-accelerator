# Visão geral do protocolo

Este documento define conceitos, não bytes definitivos. O protocolo deve servir clientes diversos e operar sobre a Transport Abstraction.

## Canais e fluxo conceitual

1. Um Control Channel cria uma Session.
2. As partes negociam versões, identidade/autenticação quando exigida e capabilities.
3. O servidor aplica autorização e política e retorna a configuração efetiva.
4. A Session entra em `READY`/`ACTIVE`; jobs usam controle no Control Plane e cargas grandes no Data Plane.
5. Data Channels adicionais são associados de forma segura à Session antes de transportar payload.
6. Erro, timeout ou encerramento conduzem a cleanup determinístico.

Mensagens de controle não devem ficar indefinidamente bloqueadas atrás de grandes payloads. Backpressure, limites e cancelamento devem valer por canal/job.

## Envelope futuro

O Wire Protocol deverá definir, no mínimo: Magic Number; Protocol Version; Message Type; Message Length; Session ID; Request/Job ID quando aplicável; Error Code; payload; tamanhos máximos; endianness única e explícita; e regras de compatibilidade entre versões.

Parsers deverão validar comprimentos antes de alocar ou ler payloads, rejeitar overflow, campos impossíveis e mensagens inválidas para o estado atual. Campos desconhecidos só podem ser ignorados quando a versão declarar isso seguro. IDs numéricos e layout binário não são definidos aqui.

## Vocabulário de mensagens não congelado

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
