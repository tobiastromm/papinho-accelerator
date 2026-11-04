---
status: accepted
date: 2026-11-02
scope: P1 PERSONAL_LOCAL TRANSPORT_ONLY; implementation and qualification offline only
supersedes: []
---
# Decisão — referências originais QUERY SQB3/SQC3

O proprietário aprovou explicitamente o documento-base por seu SHA256 seguido de
“ok” e, depois da identificação do conflito Q6, aprovou a errata pelo SHA256
seguido de “ok”. Ambos foram conferidos byte-exatamente antes do código.

Incorporar indivisivelmente os documentos preservados:
- [Base Q0–Q7](../integration/phase6-server-update-after-utility-20261101/CONTROLLER-QUERY-REFERENCES-SQB3.CANDIDATE.md), SHA256 `726e3f786b6ca187a0058734b6c9fd9e67d2d2b9d465f59bc9bd5313cbbe2c4c`.
- [Errata Q6](../integration/phase6-server-update-after-utility-20261101/CONTROLLER-QUERY-REFERENCES-Q6-CUSTODY.ERRATUM.CANDIDATE.md), SHA256 `b8d73c7541df2363e6f02489d5143a2becec78fdd315003806010a0303bbe810`.

Q6 aplica-se exclusivamente com a errata: fechamento dos cinco handles entregues
não afirma fechamento dos originais independentes de custódia. Estes permanecem
até o resultado terminal e a liberação legítima do empréstimo, sob OHI/SSO-B S5.
Resolve-se o conflito documentado sem alterar documentos accepted anteriores.

Reutilizar entrega cross-principal2/3/5/6/7, OHI, SSO-B, QUERY/G3, G0–G9 e P0–P7.
SQB3=1100/SQC3=1092; SQB1/SQB2/SQR1/SQC1 e formatos públicos/persistidos intactos.
Processo0x00101000 e tokenQUERY8 não impersonável; nenhum privilégio, ACL ou autoridade
administrativa transferida. Perfis READ/NEW_INSTALL/outros inalterados. G3-P1 e
PST0.6.4 preservados. Autoriza somente implementação/qualificação offline.

Não autoriza UPDATE, coleta, sessão E/Q/S, disposição, Start/Stop, SCM/ACL,
CA/CURRENT, rede, TPM/NV/Q4 ou limpeza/refund. Autorizações anteriores consumidas;
retenção histórica, application close e quiescência OS UNKNOWN.
