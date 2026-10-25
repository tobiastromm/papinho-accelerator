# UTC Q0-Q7 corrigido — aceitação normativa delimitada

CURRENT_TASK_COMPLETED=YES. Correções exatas Q2/Q3 e sua preservação dependente em Q1 resolvem R1/R2. rho tem direção UTC real/PAL e E cobre erro aditivo qualificado dos endpoints; propagação usa arredondamento para fora, checked arithmetic e invalidação. Nenhuma fonte UTC real/rollback operacional é qualificada por este registro.

[Decisão ACCEPTED](../../adr/DECISION-qualified-utc-q0-q7-corrected-20261021.md), após aprovação oficial e pós-hashes da operação. Anexo congelado13076bytes SHA256 82c97ae14c82622b86095b701f0af9de191cfe6b39e371e63501dfe02f26b746; proposta original, patch, Revision2 PROPOSED e STOP anterior preservados. Nenhum patch funcional aplicado/aceito por referência.

[Cláusulas congeladas](Q0-Q7-corrected-frozen-clause-bytes.txt) · [Conformidade](conformance-matrix.md) · [Incorporação/preimagens](incorporation-and-preimages.json) · [Auditoria](audit.json) · [Hashes](final-manifest.json).

```ini
CURRENT_TASK_COMPLETED=YES
QUALIFIED_UTC_CONTRACT=ACCEPTED
Q2_RHO_DIRECTION=REAL_UTC_ADVANCE_PER_PAL_ADVANCE
Q3_PAL_ERROR=QUALIFIED_ADDITIVE_E_OUTWARD_ROUNDING_CHECKED_ARITHMETIC
UTC_SOURCE=NOT_QUALIFIED_UNTIL_IMPLEMENTED
ROLLBACK_PROTECTION=NORMATIVE_INVALIDATION_NOT_IMPLEMENTED_OR_QUALIFIED
IMPLEMENTATION_AUTHORIZED=NO
ISSUER_STATUS_BINDING=DENY
UPDATE_U0_U7_BINDING=DENY
V3_COMMIT=DENY
CURRENT_PUBLICATION=DENY
E0_E8_FACTUAL_SOURCE=DENY
M1_TRUST=DENY
PAPACC_INSTALL=DENY
AUTOMATIC_ENROLLMENT=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
BUILDS=NOT_RUN
TESTS=NOT_RUN
NETWORK_TESTS=NOT_RUN
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=NONE_IN_NORMATIVE_SCOPE
NEXT=QUALIFIED_UTC_SOURCE_IMPLEMENTATION
```

Baseline Accelerator main913c90cf2328f7939d8f5b927640601903ac6e91 com14 linhas de WIP; Browser main96f6cd2f9ec4f834999828fa13bb7d170db66a98 limpo. Preservar WIP/.gitignore Browser e quatro LAB; nenhuma escrita no Browser, fonte funcional ou ADR accepted anterior. ED11 runtime delta0; limites/quotas e provider/kernel UNKNOWN mantidos. Não houve builds/testes/rede/relógio/ACL/token, chaves/certificados ou commit/push/tag/release.
