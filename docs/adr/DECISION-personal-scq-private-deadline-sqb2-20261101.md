---
status: accepted
date: 2026-11-01
scope: PERSONAL_LOCAL TRANSPORT_ONLY private bootstrap deadline binding, offline only
supersedes: []
---
# Decisão — delta privado SQB2 de deadline

O proprietário autorizou explicitamente implementação/qualificação offline
do [pedido técnico exato](../integration/phase6-query-g3-accepted-20261031/PRIVATE-DEADLINE-DELTA.REQUEST.md)
em resposta atribuída e texto explícito subsequente. Conferência byte-exata:
SHA256 `62548c758282dd6b828b451a01a92bc4a369843ac55f7cc9c52f8c84a25ae11c`.
Aceita-se exclusivamente o corpo do pedido por incorporação, antes do código.
O original CANDIDATE e as rejeições anteriores ficam preservados como histórico.

Autoriza8bytes u64 little endian de deadline global no bootstrap PRIVADO SQB2,
total1084bytes e expansão correspondente de8bytes no buffer existente. Preservar
SCQ1(308bytes), SQB1(1076), SQR1/SQC1, N0/READ, ROOT/CURRENT/PACC e quaisquer
formatos públicos/persistidos. Não reinterpretar ou fazer downgrade de versões.
B3 rege representação privada; QUERY/G3 e G0–G9 continuam integrais, mesmos valores.
Não identificado conflito dentro desse delta explicitamente aprovado.

Valor recebido não renova janela original nem duração global. Comparar com
deadline original congelado pelo controlador, tentativa/política, metadata
candidato e relógio, prevalecendo o menor prazo aplicável. Provar origem e
autenticação bilateral antes da admissão; deadline/carrier/ACK não são autoridade.
Ausência, divergência, replay, Stop e close incerto conservam DENY/UNKNOWN.
Não substituir guarda pré-Start por constante permissiva: eventual integração
exige predicados originais/quantitativos e continuação receptora qualificados.

Somente código e testes offline. Não autoriza update staged, endpoint real,
Start/Stop, SCM/ACL, CA/CURRENT, rede operacional, TPM/Q4, limpeza/refund ou
recuperação. Preservar HEAD/WIP, históricos/pins, chave fixture, UNKNOWN/reservas,
quiescência OS UNKNOWN e PST0.6.4. E21 PENDING; ED11/Phase6 PARTIAL.
