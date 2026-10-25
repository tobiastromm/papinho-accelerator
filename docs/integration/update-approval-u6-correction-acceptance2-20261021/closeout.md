# U0-U7 corrigido — aceitação normativa delimitada

CURRENT_TASK_COMPLETED=YES. Correção U6 conforme D3.7/G5 e recuperação issuer: histórico pode persistir pelos contratos existentes; restart não restaura ato UPDATE, CURRENT ou autoridade operacional. R1 encerrado somente no escopo normativo. U0-U5/U7 permanecem byte-exatos.

Registro [ACCEPTED](../../adr/DECISION-v3-update-approval-u0-u7-u6-correction-20261021.md) condicionado à aprovação oficial e verificação dos hashes desta operação; anexo congelado9786bytes SHA256 4597b5e3ddda96db039025dc0c4eb898e08a1a6fa799e7f4e0d444e39f2842ec. Proposta original, Revision2 PROPOSED, patch e STOP anterior preservados. Nenhum patch funcional é aceito ou aplicado por referência.

[Conformidade](conformance-matrix.md) · [Incorporação/preimagens](incorporation-and-preimages.json) · [Cláusulas congeladas](U0-U7-corrected-frozen-clause-bytes.txt) · [Auditoria](audit.json) · [Hashes](final-manifest.json).

Baseline Accelerator main913c90cf2328f7939d8f5b927640601903ac6e91 com2 linhas de WIP; Browser main96f6cd2f9ec4f834999828fa13bb7d170db66a98 limpo. Baseline atual preserva .gitignore Browser alterado externamente e agora incorporado pelo owner; nenhuma restauração/escrita no Browser. Quatro LAB não acessados.

```ini
CURRENT_TASK_COMPLETED=YES
UPDATE_APPROVAL_CONTRACT=ACCEPTED
U6_RESTART_SEMANTICS=NO_AUTOMATIC_CURRENT_AUTHORITY_OR_APPROVAL_RESTORATION
HISTORICAL_PERSISTENCE=PER_ACCEPTED_CONTRACTS_ONLY
OPERATIONAL_AUTHORITY_AFTER_RESTART=DENY
AUTHORITATIVE_RECOVERY=DENY
IMPLEMENTATION_AUTHORIZED=NO
V3_COMMIT=DENY
CURRENT_PUBLICATION=DENY
E0_E8_FACTUAL_SOURCE=DENY
M1_TRUST=DENY
PAPACC_INSTALL=DENY
AUTOMATIC_ENROLLMENT=DENY
SIGNATURE_AUTHORITY=DENY
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
NEXT=V3_UPDATE_APPROVAL_PRODUCER_IMPLEMENTATION
```

CURRENT externo, token/UTC positivos, produtor funcional e ED11 de ativação não foram qualificados. O término desta tarefa é somente normativo; gates operacionais continuam negados. Não houve negativa oficial do executor.
