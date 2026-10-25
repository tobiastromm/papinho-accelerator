# Windows SCM lifecycle — closeout, 2026-10-22

main HEAD913c90cf2328f7939d8f5b927640601903ac6e91. Componentes independentes implementados e qualificados; objetivo completo NÃO concluído. Fonte/identidade operacional/credencialSCM e prontidão/quiescência runtime ainda não qualificadas: seção5/STOP preserva DENY. Detalhes em implementation-proof-and-handoff.md; não promover modelo a serviço real.

ConsultaSCM nativa implementada e observou serviço ausente no host. CLI aceita os sete comandos canônicos; install é parse/planejamento DATA sem CreateService. Branches start/stop/startup/remove estão gated e não executados. Host interno registra dispatcher/ServiceMain negado semruntime; não informaRUNNING nemSTOPPED com recursos protegidos retidos. Mesmo executable/main e Stopbridge, sem runtime/worker paralelo. Todos os comandos mutantes negam antes de aquisiçãoSCM. RemoverSCM nunca apagaCA/dados/CURRENT.

Quatro builds Selected/Legacy Debug/Release MSVC /W4 /WX semwarnings. 28 combinações de sete suítesPASS, mais quatro retestes após extensão de ServiceMain modelado; 36 casos nativos pelo executablePASS. Falhas/accessdenied/closeincerto/no-retry/Stop-before-running/retention/replay testados em modelos isolados e negativos reais. Nenhuma operação positiva/serviçoLAB criado ou UACexecutado.

Diagnóstico de sizes/asm inicialmente falhou porque /TC aplicava-se às.lib. Corrigido apenas na receita, mantendo histórico; quatro medições corrigidaspassam semwarnings. PDB temporário criado por essa primeira receita foi preservado no diretórioTEMP; nenhuma biblioteca/fonte foi sobrescrita. Build/testes do produto já tinham passado e não foram alterados pela correção diagnóstica.

Managedstatic novo116bytes; nomesRO38 e PE/code/stack medidos separadamente, não omitidos ou duplicados. Bytesnominais não autorizam efeito. CustosSCM/RPC/kernel/CRT/threads/fullstack/host e runtime operacional permanecemUNKNOWN. Quotas intocadas, ED11PARTIAL. Nenhuma mudança em PST/Browser/wire/produção0.6.4/ADRaccepted/formatos, sem clone/worktree/commit/push/tag/release. WIP verificado; somente hunks aprovados e append live/portability. QuatroLAB históricos não limpos/modificados. Não houve negativa oficial contornada.

CURRENT_TASK_COMPLETED=NO
WINDOWS_SCM_LIFECYCLE=PARTIAL_INDEPENDENT_COMPONENTS_QUALIFIED_PRODUCTIVE_BINDING_DENY
SERVICE_INSTALL=PARSE_DATA_ONLY_NO_CREATESERVICE_NATIVE_DENY
SERVICE_START=NATIVE_BRANCH_GATED_NOT_EXECUTED
SERVICE_STOP=DATA_AND_HANDLER_BRIDGE_QUALIFIED_NATIVE_STOP_NOT_EXECUTED
SERVICE_STATUS=NATIVE_READ_ONLY_QUERY_PASS_ABSENT_OBSERVED
SERVICE_STARTUP_MODE=AUTOMATIC_MANUAL_PARSE_NATIVE_BRANCH_GATED
SERVICE_REMOVE=NATIVE_BRANCH_GATED_NO_DATA_DELETION
OPERATIONAL_IDENTITY=NOT_QUALIFIED_NO_ACCOUNT_CREATED
ED11_TOTAL_STATUS=PARTIAL
BUILDS=SELECTED_LEGACY_DEBUG_RELEASE_MSVC_PASS
WARNINGS=0_FINAL_PRODUCT_AND_CORRECTED_DIAGNOSTIC_BUILDS
OFFLINE_TESTS=28_SUITE_CONFIGURATION_COMBINATIONS_PLUS_4_EXTENDED_MODEL_RETESTS_PASS
NATIVE_TESTS=36_CANONICAL_EXECUTABLE_READ_ONLY_NEGATIVE_CASES_PASS
NETWORK_TESTS=NOT_RUN
PRODUCT_SERVICE_INSTALL=NOT_RUN
LAB_SERVICE_CREATION=NOT_RUN
BOOTSTRAP_EXECUTION=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=VERIFIED_IN_VERIFICATION_JSON
FIRST_UNRESOLVED_DIVERGENCE=FACTUAL_INSTALLATION_ACCOUNT_ACL_CREDENTIAL_BINDING_ABSENT
NEXT=PERSONAL_MVP_WINDOWS_INSTALLATION_AND_CONFIGURATION

O próximo trabalho precisa fechar esse binding e os fatos de runtime/ED11 antes de qualquer instalação/start produtivos ou testeSCMpositivo; não se exige criar CA para testar apenas a infraestrutura administrativa independente. Não reabrir backuprecuperável como pré-requisito do MVPpessoal agoraaccepted.
