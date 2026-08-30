# Visão geral do protocolo

A arquitetura de segurança que futuramente envolverá estes bytes está congelada
em [Phase 3 Security Architecture and Threat Model](phase3-security-architecture.md).
A Phase 3.A1 não altera o registry nem implementa segurança.
O perfil revisado da 3.A2A-R1 seleciona TLS 1.3 mTLS com CA
privada/administrativa e certificado individual por dispositivo cliente abaixo
do framing, sem introduzir mensagens PACC; veja [Phase 3 Transport Security and
Credential Profile](phase3-transport-security-profile.md).

Phase 2.D3B integra `CONTROL_OPEN` -> `CONTROL_ACCEPT` ao RUN mode Win32 real.
Phase 2.E1 congela, sem implementar, associação estrutural DATA por ticket
opaco one-time. Autenticação, Transport Security, capabilities e protocolo de
payload DATA continuam não implementados.

Phase 2.E2A implementa apenas registry/codecs, Ticket Value Object e Association
Manager portáteis. O generator continua injetado; post-Control processor, DATA
Attach Processor, classifier e integração do servidor permanecem para E2B/E3.

Phase 2.E2B1 adiciona o processador CONTROL pós-estabelecimento portátil para a
troca sequencial `DATA_TICKET_REQUEST` -> `DATA_TICKET`. DATA_ATTACH, classifier
e integração Win32 do servidor continuam não implementados.

Phase 2.E2B2B completa o caminho portátil estrutural de DATA_ATTACH: consumo
one-time do ticket, bind DATA pelo Channel Manager e emissão de DATA_ACCEPT.
Na conclusão portátil de E2B2B, integração Win32 e protocolo de aplicação
pós-DATA ainda permaneciam ausentes.

Phase 2.E3 integra o fluxo estrutural completo no servidor real: classificação,
CONTROL, solicitação de ticket, DATA_ATTACH e DATA_ACCEPT. Não há protocolo de
aplicação pós-DATA nem monitoramento improvisado de EOF para DATA estabelecido.

Este documento resume a visão conceitual. O envelope comum está em [Protocol
Framing](protocol-framing.md); os únicos tipos numéricos atribuídos são o
registry normativo `0x0001`..`0x0006`.

**Estado de implementação:** a Phase 2 está concluída no escopo estrutural.
Existem runtimes de Connection, Session e Channel, encoder/parser, Framed Reader
e Framed Writer portáteis. O executable processa as seis mensagens normativas
de estabelecimento CONTROL e associação DATA pelo I/O Loop Win32 combinado.
Não processa protocolo de aplicação após `DATA_ACCEPT`. A arquitetura normativa está em
[Connection I/O Scheduling](connection-io-scheduling.md).
Os IDs normativos `0x0001`..`0x0006` estão definidos em
[Control Establishment Protocol](control-establishment-protocol.md) e
[Data Association Protocol](data-association-protocol.md).

## Canais e fluxo conceitual

1. Um Control Channel cria uma Session.
2. Após Transport Security exigida, as partes estabelecem identidade/autenticação e negociam versões e capabilities sem permitir downgrade.
3. O servidor aplica autorização e política e retorna a configuração efetiva.
4. Na direção recomendada para a Phase 3, a Session só entra em estado utilizável/`ACTIVE` após os gates exigidos de autenticação e autorização; jobs usam controle no Control Plane e cargas grandes no Data Plane.
5. Data Channels adicionais são associados de forma segura à Session antes de transportar payload.
6. Erro, timeout ou encerramento conduzem a cleanup determinístico.

Mensagens de controle não devem ficar indefinidamente bloqueadas atrás de grandes payloads. Backpressure, limites e cancelamento devem valer por canal/job.

## Envelope comum congelado

Envelope 1.0 possui header fixo de 16 bytes, magic `PACC`, versão de envelope 1.0, Header Length, Message Type U16, Flags U16 e Payload Length U32, com inteiros multi-byte big-endian. IDs de Session, Channel, Connection e correlação não pertencem ao header universal; metadata futura pertence à semântica de mensagens específicas.

O contrato normativo do envelope e das integrações portáteis de stream está em
[Protocol Framing](protocol-framing.md). Este overview não atribui IDs extras.

## Vocabulário de mensagens não congelado

`CONTROL_OPEN`, `CONTROL_ACCEPT`, `DATA_TICKET_REQUEST`, `DATA_TICKET`,
`DATA_ATTACH` e `DATA_ACCEPT` não fazem parte da lista especulativa abaixo;
seus IDs, direções e payloads estão congelados nos documentos normativos.

```text
HELLO / WELCOME
AUTH / AUTH_RESULT
CAPABILITIES / CAPABILITY_CONFIG
PING / PONG
REQUEST / RESPONSE / PROGRESS / CANCEL / COMPLETE
ERROR / DISCONNECT
```

`HELLO/WELCOME` negociam compatibilidade, não presumem sucesso. `CAPABILITIES` anuncia suporte; `CAPABILITY_CONFIG` expressa o resultado permitido. Operações longas precisam de correlação, progresso, cancelamento e conclusão inequívoca. `ERROR` deve distinguir falha de mensagem, job, capability, canal e Session sem revelar segredos.

## Compatibilidade

Versão e capability são eixos distintos. Uma versão de protocolo compatível não implica suporte a toda capability. Novos peers devem negociar um subconjunto comum; caso não exista base segura, devem encerrar com erro explícito. Extensões deverão ser delimitadas e versionadas, preservando parsers antigos. Não há garantia de retomada de Session nesta baseline.

## TCP e futuro UDP

TCP será o primeiro transport e suportará Control Channel e Data Channels. UDP permanece opcional para avaliação futura de casos sensíveis a latência. Nenhum protocolo de mídia UDP, porta ou mecanismo de confiabilidade é especificado nesta fase.
