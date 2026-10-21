# Cohort UPDATE v3 — implementação delimitada e qualificação parcial

CURRENT_TASK_COMPLETED=NO.

Implementado um cohort de80 bytes no owner existente, sem segunda root/produtor/pin table. O produtor canônico pode manter a geração anterior protegida enquanto copia bytes candidatos para um slot G7 livre, com serial0, sem pin do candidato. t.model/source publicados não viram scratch. O pin predecessor real é movido para o cohort e a source publicada usa somente seu mecanismo de borrow existente. Staging não significa RESOLVED/VALIDATED.

A entrada é UPDATE dormente, distinta de bootstrap/restore; nega runtime ativo, attachment/listener/Session pins e operações pendentes. O primeiro subconjunto usa predecessor v3 protegido; base v2 permanece não integrada. Sem CURRENT factual, nega sem criar cohort. Validar e commitar permanecem negados: fontes de aprovação/referências do candidato e predecessor factual não foram qualificadas; packet11 não é iniciado pelo cohort não resolvido.

Abort próprio verifica quiescência, identidade/serial de slot e ausência de worker/handles/resultado incerto antes de unborrow/unpin. Stop não limpa nem libera; os mutadores históricos não consomem o candidato. Incerteza mantém ambas as gerações e crédito alcançáveis no mesmo owner.

Gate pré-patch: esquema80 confirmado por probe MSVC/W4/WX antes dos hunks, prova/hash congelados. Censo compilado final Selected1.948.656 (+80), Legacy1.790.872 inalterado. Cenário completo conservador3.747.601 excede3MiB em601.873 e não autoriza ativação. ED11 totalPARTIAL/provider-kernel-stack UNKNOWN; nenhum aumento de quota.

Quatro builds finais sem warnings;74 suítes offline e24 casos nativos locais passaram. Positivo de lifetime é CURRENT MODEL com SHA/CA pública e tickets reais do componente; o singleton real foi exercitado somente em negativos sem CURRENT. Não há qualificação positiva de CURRENT, source/commit do cohort ou E0. Falhas iniciais e reparos exatos preservados.

[Ownership/lifecycle e gate restante](ownership-lifetime-and-gates.md) · [Gate pré-patch](prepatch-lifetime-capacity-proof.json) · [ED11](ed11-census.json) · [Testes](validation.json) · [Hashes](final-manifest.json) · [Evidências](evidence.zip)

```ini
V3_UPDATE_COHORT=IMPLEMENTED_DORMANT_UNRESOLVED_STAGING_V3_BASE_SUBSET
CURRENT_IMMUTABILITY=PASS_MODEL_NATIVE_PIN_CODE_REAL_ROOT_NEGATIVES
CANDIDATE_LIFETIME=OWNED_G7_SLOT_NO_CANDIDATE_PIN_ABORT_AFTER_QUIESCENCE
FULL_PREDECESSOR=COMPLETE_BYTES_AND_PIN_HELD_FACTUAL_ROOT_QUALIFICATION_PENDING
ED11_RECONCILIATION=80_FIXED_BYTES_ADDED_NO_QUOTA_CHANGE_FULL_ACTIVATION_DENY
BUILDS=4_FINAL_PASS
WARNINGS=0_FINAL_BUILDS
OFFLINE_TESTS=74_PASS
NATIVE_TESTS=24_PASS_SYNTHETIC_ADMIN_CONTEXT
NETWORK_TESTS=NOT_RUN
CURRENT_PUBLICATION=DENY
E0_E8_FACTUAL_SOURCE=DENY
M1_TRUST=DENY
PAPACC_INSTALL=DENY
AUTOMATIC_ENROLLMENT=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
ED11_TOTAL_STATUS=PARTIAL
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=FACTUAL_CANDIDATE_REFERENCE_APPROVAL_AND_COMPLETE_CURRENT_SOURCE_UNQUALIFIED_VALIDATE_COMMIT_DENIED
NEXT=AUTHORIZED_V3_UPDATE_PREPARE_VALIDATE_QUALIFICATION
```
