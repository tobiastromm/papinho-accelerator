# U0-U7 — aceitação suspensa por R1 em U6

CURRENT_TASK_COMPLETED=NO. Revisão detectou ambiguidade material em U6: “não precisa ser serializada para conceder validade operacional após restart” pode sugerir validade pós-restart que D3.7/G5 não concedem. A cláusula também nega recuperação, tornando a leitura conflitante. GOVERNANCE CONFLICT registrado; nenhuma decisão ACCEPTED criada.

Preparada correção de um único trecho e Revision2 PROPOSED separada. Proposta original/ADRs/formatos preservados, sem patch aplicado ou autoridade nova. Matriz cobre ato explícito versus identidade, candidata/predecessor/CP2-CP3, SNT/PDG/issuer/policy/status/UTC, Stop/lifetime/CAS e ED11.

HEAD inicial Accelerator main913c90cf2328f7939d8f5b927640601903ac6e91, working tree limpa; Browser main1eb028554f622672377953225e162ed9b6564a40 com123 linhas de WIP preservadas. O commit do owner foi observado antes da base desta tarefa; nenhum commit executado pelo agente. Quatro LAB não acessados.

[Revisão e matriz](conformance-review.md) · [Correção exata](exact-u6-correction-PROPOSED.patch) · [Revision2 proposta](U0-U7-REVISION2-PROPOSED.md) · [Preimagens](exact-correction-and-preimages.json) · [Auditoria](audit.json) · [Hashes](final-manifest.json)

```ini
CURRENT_TASK_COMPLETED=NO
UPDATE_APPROVAL_CONTRACT=PROPOSED
ADMINISTRATIVE_ACT=DEFINED_PROPOSED_EXPLICIT_ACT_NOT_TOKEN_ALONE
CANDIDATE_BINDING=DEFINED_PROPOSED_EXACT_OPERATION_IMAGE_PREDECESSOR_TUPLES
STATUS_SOURCE=ISSUER_POLICY_FACTUAL_SOURCES_REQUIRED
UTC_QUALIFICATION=REQUIRED_NOT_ESTABLISHED
IMPLEMENTATION_AUTHORIZED=NO
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
FIRST_UNRESOLVED_DIVERGENCE=U6_POST_RESTART_OPERATIONAL_VALIDITY_AMBIGUITY
NEXT=V3_UPDATE_APPROVAL_PRODUCER_IMPLEMENTATION
```

NEXT somente após revisão/aceitação específica da correção; nenhuma autorização funcional por referência. Não houve negativa oficial do executor: o impedimento de aceitação vem da condição de conformidade integral da seção9, não de auto-review.

## Verificação final e alteração externa

[Verificação final](final-verification.json):5895 arquivos Accelerator e2163 Browser confrontados com a baseline. Somente client-pairing.md alterado pelo agente entre os arquivos preexistentes do Accelerator; fontes funcionais/ADRs/proposta original e HEADs preservados. Browser não recebeu escrita pelo agente. Seu .gitignore mudou externamente durante a tarefa (hashes before/now na verificação); preservado sem restaurar. WIP_PRESERVED refere-se à preservação pelo agente, não à igualdade integral do Browser diante dessa alteração externa. Nenhum LAB acessado.
