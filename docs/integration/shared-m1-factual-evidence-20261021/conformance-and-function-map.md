# Conformidade e mapa da extração

HEAD/main Accelerator c50b8fb9a7e3293b5e6355fc08459bf85b56857a; Browser1eb028554f622672377953225e162ed9b6564a40/main. Baseline atual5619/2114 arquivos Git-visible não-build, não assume ausência de WIP. Todos hashes anteriores e decisões accepted ficam preservados, exceto hunks exatos da tarefa. Não acessar quatro LAB.

| Responsabilidade anterior | Destino factual | Invariante |
|---|---|---|
| failure/abort_operation/stage e sequência run | shared_structure/m1.c | primeiro erro, ordem fixa, terminal, sem skip/reset/retry |
| STRUCTURE + argumentos/budget | adapter manual_package_validation.c | parsers/bounds/aliases/custo root privados preservados |
| pré-parse CONTENT | evento TCB interno CONTENT_PARSE, stage permanece CONTENT | mesma prioridade de falha/parser/Stop; não é wire ou nova etapa de autoridade |
| contadores e state/stage | flow SP_M1_STATE embutido no owner existente | sem segundo estado mantido em paralelo; metadata não concede trust |
| wipe | adapter/port existentes | limpa key/q/staging próprios somente quiescentes |
| publicação única | core decide ordem; adapter owner_publish copia output | somente depois CONTENT/recheck/release/quiescence/Stop |
| finish | sp_m1_finish; wrapper existente | mesmo owner/binding retido, query-only até quiescência; ABORTED sem late publish |
| PDG1/CURRENT/issuer/material/profile | adapter nativo existente | SOURCE_ABSENT antes KDF; nenhuma nova fonte artificial |
| test port | binário isolado MP_PACKAGE_VALIDATION_OFFLINE_TEST | SYNTHETIC_ONLY; ausente do binding nativo |

S0–S8: núcleo somente C89/enum/size_t, sem Browser/root/EVP/CNG/NSS/PST. Source canônica complementar; PACC protocol-only preservado. Sem máquina agendadora duplicada no adapter, que contém somente ponte/preflight/ownership. API M1 privada é TCB de integração, não API factual de evidências nem callback de confiança exposto ao caller.

M0–M8/D3/F1–F5/G0–G7: algoritmos/layouts/serial/policy intactos, confiança não decorre de criptografia ou PDG DATA. Recheck adicional preservado. API E0–E8 identifica decisões ainda propostas para aquisição/captura/revalidação/release, sem registrar ACCEPTED ou criar headers/implementação factual. A origem administrativa positiva e confirmação independente continuam sem binding factual. Não existe conflito accepted resolvido por inferência.

Model E/ED08: retenção sobrevive retorno initiating-call, mesmo owner/metadados/backing/provider; Stop/timeout/exit não finalizam. O novo controle guarda identidade de owner/binding para finish. Funções privadas síncronas, port static const e contexto/input preservados pelo caller; não chamar KDF pelo caminho Stop. Nenhum worker, root, allocator, pin system ou contexto de confiança paralelo.

Prova de ligação Selected/Debug: dumpbin mostra sp_m1_execute/sp_m1_finish definidos e referenciados na package_validation.lib; não contém mp_auth_offline_test_run. Ambos os targets native/model compilam a mesma fonte comum; somente macro/hook de modelo é separado. Sem exports de backend ou trust source no header m1.h. O antigo scheduler completo está preservado só na preimagem histórica, não no produto.

Oito builds finais /WX sem warnings: standalone204 checks por VC6 real/MSVC Debug/Release; integração146 controles+1189 crypto por Selected/Legacy Debug/Release. Alterações nos testes antigos somente acesso flow, expectativas mantidas. Controle positivo é sintético, incluindo ausência/CURRENT/pin/revogação; RSA nativo sobre fixtures públicas comprova DENY no trust e zeroKDF, não autoridade positiva. Retenção de envelope/payload e descartar resultado incerto continuam cobertos. Não declarar testes no NT4/box/ISA, pins factuais ou trust positivo.

Compatibilidade de fonte privada: callers/tests locais atualizados ao membro flow; não altera ABI público PapinhoPACC/wire nem snapshot Browser. Browser não migrado, simples/avançado/provenancev2/writer intactos. Sem retomar fixture negada, builds GUI ou testes de rede.
