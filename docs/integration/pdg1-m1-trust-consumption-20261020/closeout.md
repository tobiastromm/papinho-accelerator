# PDG1 M1 trust consumption — closeout parcial

Implementado consumo DATA PDG1 no adaptador M1 offline, com correlação pacote/SPKI, seleção após validação integral v3 e rejeições fail-closed. Mesmo DATA correspondente mantém SOURCE_ABSENT antes de KDF. Não há fonte factual CURRENT/pin/confirmation ligada nem qualificação positiva na sandbox. O objetivo factual completo permanece pendente.

Autoridades F1–F5/G0–G7, D3 e M0–M8 preservadas; nenhuma decisão normativa foi reaberta. HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a, main, WIP anterior preservado por baseline e preimagens. Executor aprovou patch exato14arquivos, correção de teste e append capability; sem negativa. Manifestos históricos não foram regravados.

Selected/Legacy Debug/Release: builds /W4 /WX finais sem warnings; 2/2 suítes pacote por configuração,146 asserções sintéticas de controle e1189 criptográficas reais offline;14/14 regressões root/issuer/ED11 por configuração. RSA nativo usa fixture TEST ONLY publicada; dados administrativos são sintéticos. Nenhuma prova positiva de CURRENT, pin, Browser ou commit v3 é inferida. Logs de build inicial com C4456 preservados, seguido de correção aprovada e rebuilds finais.

ED11: metadados/root máxima adicionam31192bytes ao cenário nomeado por operação; cenário conservador3625925 excedeWindows3MiB por480197 antes de custos desconhecidos. Ativação dependente DENY; nenhuma quota alterada. Provider/kernel UNKNOWN, total PARTIAL. Ver consumption-proof-and-ed11.md e ed11-census.json.

Browser import simples/avançada e fronteiras de confirmação/instalação estão especificadas em browser-import-handoff.md, sem alteração Browser e sem exigir root interna. Runtime/worker/CLI/formatos persistentes não mudaram. Nenhuma rede, emissão, assinatura produtiva, CA/chave/certificado real, publicação CURRENT, commit Git, tag ou release.

```ini
CURRENT_TASK_COMPLETED=NO
PDG1_M1_CONSUMPTION=IMPLEMENTED_OFFLINE_DATA_FILTER_NATIVE_DENY
FACTUAL_SIGNER_TRUST=DENY_UNTIL_QUALIFIED
M1_AUTHENTICATED_VALIDATION=DENY_SOURCE_ABSENT_BEFORE_KDF
BROWSER_HANDOFF=PREPARED_NO_BROWSER_CHANGE
BUILDS=SELECTED_LEGACY_DEBUG_RELEASE_PASS
WARNINGS=0_FINAL_BUILDS
OFFLINE_TESTS=2_PACKAGE_AND_14_ROOT_SUITES_PER_CONFIGURATION_PASS
NETWORK_TESTS=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=FACTUAL_CURRENT_POLICY_PIN_AND_INDEPENDENT_CONFIRMATION_SOURCE_UNBOUND
NEXT=MANUAL_PACKAGE_BROWSER_IMPORT_IMPLEMENTATION
```
