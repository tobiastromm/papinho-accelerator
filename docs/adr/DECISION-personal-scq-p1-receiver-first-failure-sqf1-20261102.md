---
status: accepted
date: 2026-11-02
scope: P1 PERSONAL_LOCAL TRANSPORT_ONLY SQB3; offline implementation only
supersedes: []
---
# Decisão — retorno negativo SQF1 da primeira falha receptora

O proprietário aprovou explicitamente a proposta focal pelo SHA256 seguido de ok:
96b2d26f83cecc7688eeb9e3543a6b19d6660978b31b62ae175fe604f41a3643 ok.
Conferência byte-exata concluída antes da implementação.
Incorporam-se indivisivelmente B/F0–F7 de
[RECEIVER-FIRST-FAILURE-SAME-PIPE.CANDIDATE.md](../integration/phase6-server-update-after-utility-20261101/RECEIVER-FIRST-FAILURE-SAME-PIPE.CANDIDATE.md),
SHA25696b2d26f83cecc7688eeb9e3543a6b19d6660978b31b62ae175fe604f41a3643.

Especializa Q2 e a proibição de novo frame da errata Q6 apenas para negativo privado
SQF1/56bytes no MESMO pipe e tentativa. SQR1/SQC3/SCQ1/SQB1/SQB2/SQB3 permanecem
byte-exatos; SQF1 nunca é receipt/close/admissão/autoridade ou readiness.
OHI/SSO-B/P0–P7/G0–G9/G3/Q6/custódia/continuidade/retention e demais perfis
inalterados. Sem conflito residual nessa precedência focal; normas anteriores
não editadas. Send suprimido se inseguro/pending/Stop/deadline/resources UNKNOWN;
primeira falha original preservada, sem retry ou novo recurso pós-falha.

Autoriza somente registro e implementação/qualificação OFFLINE no owner existente.
Não autoriza coleta/UPDATE/disposição/E/Q/S/Start/Stop/kill/ACL/SCM/CA/CURRENT/rede/
TPM/NV/Q4/limpeza/refund/produtivo. Histórico e OS UNKNOWN/PST0.6.4 preservados.
