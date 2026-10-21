# Transação v3 — componente nativo concluído, integração administrativa parcial

CURRENT_TASK_COMPLETED=NO.

O mesmo worker agora discrimina persistência v3→v3 em packet privado11, com capacidade máxima30.592 em suas três imagens existentes. Guards verificam limites, integridade integral, CP3/base/R/A/P/I, predecessor e preservação exata do carrier/SNT/PDG. O CAS nativo compara TODOS os bytes anteriores e identidade do arquivo, usando a transação existente. COMMITTED não publica slot/CURRENT: previous e committed antigos permanecem intactos, o resultado bloqueia autoridade anterior e conserva dados até integração factual.

A entrada de teste é compilada somente no executável diagnóstico, sem declaração em header produtivo e sem BEGIN administrativo novo. Os oito casos nativos usam história e assinatura MODEL, sem autoridade positiva, CA/chave/certificado novo ou CURRENT real. O caminho normal v1 continua separado. Base v2 e alterações de issuer/delegação NÃO foram habilitadas por este slice.

**Bloqueio restante de implementação:** não há produtor canônico que prepare candidato v3 completo e valide kind8 com fontes factuais sob lifetime antes de publicar. O dispatch kind8 atual exige pin de backing já protegido; não é permitido fabricar pin de candidato ou CURRENT para contornar essa diferença. Também faltam base CURRENT/predecessor completo/história de aprovação factuais e sua integração à transação. A implementação dependente foi interrompida nesse gate; não foi classificada como simples falta de ambiente.

Quatro builds finais Debug/Release Selected/Legacy sem warnings;72 suítes offline e24 casos nativos locais passaram (8 v3,16 históricos). Rebuild limpo Selected necessário após mudança de layout. Logs das falhas iniciais de main duplicado, disco cheio e incremental com objetos antigos foram preservados. A única correção de fonte de build foi o define de fixture aprovado; nenhum predicado foi relaxado.

ED11 fixo Selected1.948.576 (+4.896); Legacy1.790.872 inalterado. Janela total operacional não qualificada; provider/kernel/stack transitivo UNKNOWN. Cenário conservador completo3.747.521 excede3MiB por601.793; não é pico medido nem reserva e a ativação permanece DENY.

[Prova e gate](transaction-proof-and-remaining-gate.md) · [Testes](validation.json) · [ED11](ed11-census.json) · [Hashes](final-manifest.json) · [Evidências/preimagens](evidence.zip)

```ini
V3_WORKER_TRANSACTION=NATIVE_PERSISTENCE_SLICE_IMPLEMENTED_NO_CANONICAL_PRODUCER
FULL_PREDECESSOR=FULL_BYTES_CAS_PRESERVED_FACTUAL_AUTHORITY_UNQUALIFIED
CAS_HANDLING=SUCCESS_CONFLICT_UNKNOWN_STOP_DISTINCT_NO_FALLBACK
SNAPSHOT_PIN_LIFETIME=EXISTING_G7_DATA_PRESERVED_CANDIDATE_PROTECTION_GATE_OPEN
CURRENT_PUBLICATION=DENY
BUILDS=4_FINAL_PASS
WARNINGS=0_FINAL_BUILDS
OFFLINE_TESTS=72_PASS
NATIVE_TESTS=24_PASS_SYNTHETIC_ADMIN_CONTEXT
NETWORK_TESTS=NOT_RUN
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
FIRST_UNRESOLVED_DIVERGENCE=CANONICAL_PREPUBLICATION_V3_PRODUCER_AND_FACTUAL_FULL_PREDECESSOR_LIFETIME_NOT_IMPLEMENTED
NEXT=AUTHORIZED_BOOTSTRAP_AND_V3_CURRENT_QUALIFICATION
```
