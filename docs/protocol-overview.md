# Visão geral do protocolo

A implementação pública portátil é [PapinhoPACC](integration/papinhopacc.md),
governada por [ADR-0025](adr/ADR-0025-contrato-publico-portavel-papinhopacc.md).
Sua API 1.0.0 e a geração externa de contrato wire 1 são metadata distintas
dos campos wire Envelope/CONTROL 1.0; a extração não adiciona bytes ao wire.

## Estado factual após a Phase 6

O registry atual termina em `0x000E`. `0x0007/0x0008` implementam negociação
inicial de capabilities e `0x0009..0x000E` implementam Network Offload após
`DATA_ACCEPT`. Os IDs históricos `0x0001..0x0006` não mudaram. O DATA Channel
dedicado identifica implicitamente o Network Stream; nenhum ID runtime,
Principal interno, backend, profile, source, trust ou SNI independente é
serializado. Veja [Phase 6 — Network & TLS Offload](phase6-network-tls-offload.md).

As decisões arquiteturais de runtime, scheduling, capabilities e segurança
estão registradas nos [ADRs locais](adr/). As specifications referenciadas
neste documento continuam sendo a autoridade normativa para bytes, frames,
mensagens e comportamento wire.

A arquitetura e o perfil de segurança que protegerão estes bytes estão
decididos nos ADRs locais e detalhados em [Phase 3 Security Architecture and
Threat Model](phase3-security-architecture.md). A Phase 3.A1 não alterou o
registry nem implementou segurança.
O perfil revisado da 3.A2A-R1 seleciona TLS 1.3 mTLS com CA
privada/administrativa e certificado individual por dispositivo cliente abaixo
do framing, sem introduzir mensagens PACC; veja [Phase 3 Transport Security and
Credential Profile](phase3-transport-security-profile.md).

PapinhoSecureTransport (PST) é consumido abaixo do framing pelo controller
Secure Principal opt-in. As Phases 3.B–3.F integraram Transport Security,
AuthN/AuthZ e CONTROL/DATA seguros sem alterar os bytes PACC. A configuração
operacional no executável padrão permanece futura. Legacy Endpoint não passa
pelo PST.

Historicamente, Phase 2.D3B integrou `CONTROL_OPEN` -> `CONTROL_ACCEPT` ao RUN mode Win32 real.
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
Framing](protocol-framing.md). O registry factual atual contém `0x0001` até
`0x000E`: os seis tipos históricos de CONTROL/DATA, a negociação inicial de
capabilities (`0x0007`/`0x0008`) e o protocolo Network Offload em DATA dedicado
(`0x0009`..`0x000E`), conforme ADR-0016 e ADR-0017.

**Estado de implementação:** a Phase 2 está concluída no escopo estrutural.
Existem runtimes de Connection, Session e Channel, encoder/parser, Framed Reader
e Framed Writer portáteis. O executável processa as seis mensagens normativas
de estabelecimento CONTROL e associação DATA, negociação inicial de
capabilities e Network Offload após `DATA_ACCEPT` pelo I/O Loop Win32 combinado.
A decisão arquitetural
canônica de scheduling está no
[ADR-0004](adr/ADR-0004-escalonamento-de-io-nao-bloqueante-limitado-e-justo.md),
enquanto [Connection I/O Scheduling](connection-io-scheduling.md) preserva a
explicação técnica detalhada.
Os IDs históricos `0x0001`..`0x0006` estão definidos em
[Control Establishment Protocol](control-establishment-protocol.md) e
[Data Association Protocol](data-association-protocol.md). Os IDs
`0x0007`..`0x000E` e suas semânticas são governados pelos ADRs locais 0016,
0017 e 0023 e refletidos nos Capability Documents de Network Egress e
TLS Offload.

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
[Protocol Framing](protocol-framing.md). Este overview apenas reflete o registry
governado; não atribui IDs extras.

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
