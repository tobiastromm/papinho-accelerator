# PapinhoSecureTransport 0.6.4 — handoff do consumer

## Baseline consumida

```text
release=v0.6.4
release_commit=9e32c84dad6d8a84d6f1119ddde88ffe57583c89
api=2.1.0
spi=3.0
target=win32-x64-msvc-19.51-openssl3
asset_sha256=f1af50c077b10fecf099ea112c4f02f5eb761e6befb0578cc4f3061f23d3630c
```

O Accelerator consome exclusivamente o SDK publicado. A aquisição validou o
hash do ZIP e todos os hashes internos do pacote. Nenhum artefato de checkout
do PST, header interno de provider ou dependência VC6/NSS foi introduzido.

## Mudança relevante ao consumer

O PST 0.6.4 corrige a sinalização de liveness no wait-set para conexão TLS já
estabelecida e ociosa. O contrato observado pelo Accelerator é:

- conexão saudável e ociosa retorna a bloqueio após eventual progresso TLS;
- FIN remoto acorda o wait-set e preserva o token da conexão;
- uma operação PST posterior classifica o encerramento;
- `pst_wait_set_wake` continua sendo o mecanismo de wake/cancel local.

Não há polling periódico, keepalive, probe de aplicação ou inspeção do socket
nativo no scheduler de produção.

## Ownership preservada

PST fornece readiness e liveness backend-neutral. O Accelerator conserva
fairness, deadlines, ordem de dispatch, stop e orchestration. O backend OpenSSL
continua selecionado pelo contrato público do pacote; detalhes internos de NSS
não fazem parte desta integração.

## Validação executada

- aquisição pública e hashes externos/internos: PASS;
- configure MSVC 19.51 x64/OpenSSL: PASS;
- teste do scheduler com TLS 1.3 estabelecido: idle bloqueia por timeout
  mensurado, sem loop de wake: PASS;
- FIN remoto em conexão estabelecida ociosa: wait acorda, token correto e
  classificação PST `CLOSED`/`FAILED`: PASS;
- wake local, double wake e stop já cobertos pelo mesmo teste: PASS;
- build completo e CTest completo: registrados no relatório da execução.

O cenário explícito de RST estabelecido no wait-set não recebeu um harness
novo nesta revisão; a matriz existente continua cobrindo reset no lifecycle
TLS outbound. Evidência NT4/NSS publicada pelo PST não é tratada como teste do
Accelerator.

## Limites

Esta atualização não altera wire protocol, capability policy, seleção de
provider, produção do servidor ou a separação entre Transport Security e
TLS_OFFLOAD. Nenhuma nova decisão arquitetural foi introduzida.
