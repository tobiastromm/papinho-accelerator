# V3 CURRENT / E0 — conclusão parcial

CURRENT_TASK_COMPLETED=NO

Implementadas correlação DATA CP3/revisão/predecessor/A/P/I sob o mesmo pin e consulta negativa de prontidão na root canônica. O preflight usa os guards existentes de CURRENT; mesmo se a correlação DATA passar, retorna NOT_SUPPORTED. Não produz evidência, lease, confiança ou publicação.

A qualificação positiva permanece PENDING_EXTERNAL_ENVIRONMENT, conforme decisão do owner: não existe instalação CURRENT v3 qualificada disponível. Também faltam integração nativa de commit/publicação CP3, predecessor completo protegido e proveniência factual de aprovação/status da delegação. Digest e metadados de fixture não suprem essas obrigações.

Seis builds finais MSVC Debug/Release: Selected e Legacy mais E0/M1 C90 isolado, sem warnings. 72 suítes principais + 2 isoladas e 16 casos nativos locais passaram. Os casos positivos DATA são sintéticos; casos nativos de filesystem/worker não qualificam CURRENT v3. Duas falhas iniciais Selected por ordem Windows/Winsock dos includes foram preservadas e corrigidas mediante aprovação exata.

Fixo Selected 1.943.680 bytes; Legacy 1.790.872, inalterados. Nenhum campo/buffer fixo, heap, thread, handle ou pin adicionado. Stack transitivo/provider/kernel continuam UNKNOWN. Cenário conservador anterior completo 3.742.625 excede 3 MiB; não é pico medido nem reserva. ED11 PARTIAL; ativação DENY.

[Prova e handoff](current-and-source-proof.md) · [Validação](validation.json) · [ED11](ed11-census.json) · [Hashes](final-manifest.json) · [Evidências](evidence.zip)

```ini
V3_CURRENT_BINDING=DATA_CORRELATION_AND_NEGATIVE_PREFLIGHT_ONLY
E0_E8_FACTUAL_SOURCE=DENY
KIND8_DISPATCH=IMPLEMENTED_PUBLIC_DATA
G7_LIFETIME=PASS_SCOPED_DATA
M1_TRUST=DENY_UNTIL_FACTUAL_SOURCE
PAPACC_INSTALL=DENY
AUTOMATIC_ENROLLMENT=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
NETWORK_TESTS=NOT_RUN
BROWSER_CHANGED=NO
PST_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=NATIVE_V3_COMMIT_CP3_PUBLICATION_AND_PROTECTED_FULL_PREDECESSOR_SOURCE_UNQUALIFIED
NEXT=MANUAL_PACKAGE_FACTUAL_TRUST_AND_END_TO_END_QUALIFICATION
```
