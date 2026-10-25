# B1 concluÃ­do; A1 em STOP por decisÃ£o explÃ­cita do owner

## Resultado e escopo

B1 Ã© a Ãºnica alteraÃ§Ã£o funcional: `psa_bd_current()` retorna o resultado de `psa_bd_aux_current()` no ramo N0. Fonte anterior estÃ¡ preservada em preimage; [patch focal](focal-changes.patch). A1 nÃ£o foi corrigido nem promovido: o owner escolheu STOP e solicitou a [revisÃ£o separada da reserva antecipada](A1-ownership-reservation-PROPOSED.md). Nenhuma execuÃ§Ã£o LAB, preflight, Start/Stop, SCM ou ACL nesta tarefa. RevisÃ£o de fontes pelo prÃ³prio agente, nÃ£o revisÃ£o externa independente.

HEAD/main permanece 913c90cf2328f7939d8f5b927640601903ac6e91. [Baseline](baseline.json) conserva hashes de 10080 arquivos versionados/WIP/nÃ£o ignorados antes das alteraÃ§Ãµes. As quatro preimages preservam integralmente os arquivos alterados. GovernanÃ§a/autoridades e auditoria anterior foram confrontadas, sem nova decisÃ£o accepted. PROJECT-CONTEXT.md nÃ£o existe; contexto/governanÃ§a jÃ¡ carregados nesta sessÃ£o contÃ­nua, incluindo ADRs aplicÃ¡veis Org/Papinho/locais, ED08/ED11, Model E e future C2 relevante. NÃ£o hÃ¡ mudanÃ§a arquitetural dependente de conflito. A1 Ã© nÃ£o conformidade de implementaÃ§Ã£o, nÃ£o licenÃ§a concedida por H0â€“H9 para descartar referÃªncia controlada.

## RevisÃ£o B1 e ponto de uso

Antes, sucesso N0 fazia `psa_bd_aux_current â†’ PSA.bd.check`, retornava e chamava o MESMO callback de novo. Agora o retorno N0 Ã© direto, sem trabalho intermediÃ¡rio entre a validaÃ§Ã£o protegida e o retorno ao caller. O caminho nÃ£o-N0 conserva seu Ãºnico `PSA.bd.check` no fim. `psa_bd_aux_current` permanece byte-exatamente inalterado e continua chamar o contexto para callers diretos via inventÃ¡rio/console. Nenhum token/SID, no-impersonation, identidade/creation/liveness/sessÃ£o, arquivo/RAW1/proveniÃªncia, contexto/plano, SCM, freshness, Stop, retenÃ§Ã£o ou primeiro erro foi retirado do callback; nÃ£o se introduziu cache nem uso de observaÃ§Ã£o histÃ³rica. Falhas retornam imediatamente, sem segunda invocaÃ§Ã£o mascarar o resultado. Chamadas posteriores continuam validar de novo e detectar mutaÃ§Ã£o entre fases.

Contagem estrutural no ramo de sucesso de UMA `psa_bd_current` N0: **2 â†’ 1 callbacks protegidos**. O novo fixture conta 1 apÃ³s correÃ§Ã£o; a asserÃ§Ã£o de 1 falhou na fonte anterior ([red-b1.log](red-b1.log)) e passou na fonte final ([green-b1.log](green-b1.log)). Isso nÃ£o mede duraÃ§Ã£o nem handles estÃ¡veis no LAB. Caller direto de aux continua 1, contexto negado executa 1 e preserva ACCESS_DENIED; falhas anteriores em token/criaÃ§Ã£o/arquivo/stale/Stop executam 0. RevalidaÃ§Ã£o seguinte apÃ³s mudanÃ§a de token nega. Nenhuma remoÃ§Ã£o B2 (por entrada do censo), B3 (runtime) ou C1 (cycle_live) aplicada.

Ownership/lifetimes B1: nenhum slot, HANDLE, borrow, aquisiÃ§Ã£o, fechamento, thread ou buffer adicionado/removido; storage do fixture BD permanece descriptor976/inventory208/state3128/admission3216/sample1416 bytes. As referÃªncias continuam nos mesmos owners; o callback duplicado removido nÃ£o era um owner separado. Nenhuma economia de handle ou alteraÃ§Ã£o do pico Ã© afirmada.

## QualificaÃ§Ã£o e correÃ§Ãµes do fixture

Quatro configuraÃ§Ãµes novas, separadas dos histÃ³ricos: Selected/Legacy Ã— Debug/Release, MSVC19.51 x64/Ninja, /W4 /WX, SDK isolado0.6.5 existente, produÃ§Ã£o0.6.4 preservada; opt-in read-only ON, positivo OFF, default de produto EMPTY_DENY inalterado. Quatro builds PASS, zero warnings. Scripts `build-*.cmd` e logs neste pacote. Nenhuma troca de toolchain/perfil para contornar falha. Esses binÃ¡rios de qualificaÃ§Ã£o NÃƒO foram colocados em launcher/pins nem autorizados para operaÃ§Ã£o; os hashes anteriores continuam histÃ³ricos.

11 fixtures por configuraÃ§Ã£o, **44/44 execuÃ§Ãµes PASS**, [resultados detalhados](test-results.json). BD94 casos, busy119 (incluindo 4 caracterizaÃ§Ãµes A1), source gate, polÃ­tica, runtime model/closed/file, cycle model, RAW1 model e pins default/optin. NÃ£o executado o entrypoint operacional do qualificador, serviÃ§o ou launcher. Testes nativos sÃ£o executÃ¡veis C compilados com APIs simuladas/modelos; **nÃ£o sÃ£o prova nativa factual de admissÃ£o, CloseHandle real ou reduÃ§Ã£o de recursos**. Sem testes de rede.

O teste de validaÃ§Ã£o lenta passou a injetar1400ms em UMA chamada, em lugar de700ms em duas. Preserva a condiÃ§Ã£o de validaÃ§Ã£o protegida >1s ANTES da nova amostra; limites1000/60000ms do cÃ³digo nÃ£o mudaram. Durante o desenvolvimento, um novo teste encadeou negativas sem encerrar seu owner sintÃ©tico, deixando actor_check_token ocupado apÃ³s negaÃ§Ã£o de token; o resultado seguinte corretamente foi INVALID_STATE. Corrigiu-se apenas isolamento do fixture, com finish factual do modelo e novo owner falso por caso. Nenhuma correÃ§Ã£o funcional adicional. Suite final4perfis passou apÃ³s essa correÃ§Ã£o.

A1 foi caracterizado em handles falsos: trÃªs falhas distintas, dois valores guardados; cleanup nega sem repetir close. O texto de saÃ­da marca explicitamente UNFIXED/STOP. O exit0 desse fixture significa reproduÃ§Ã£o esperada do defeito, **nÃ£o** cumprimento do contrato de retenÃ§Ã£o. A futura correÃ§Ã£o deve substituir essa caracterizaÃ§Ã£o por asserÃ§Ãµes de retenÃ§Ã£o segura; nÃ£o transformar a perda em comportamento aceitÃ¡vel. AquisiÃ§Ã£o pÃ³s-falha/no-lost-ref de A1 ainda NÃƒO qualificados e nÃ£o se afirma first-error/UNKNOWN corrigidos nesse helper.

## Hashes e preservaÃ§Ã£o

Fonte B1 SHA256 `90b0c771b3904b09db97d6c364f142ea2471d5547a4856910926cdc35ef15db7`.

Fixture BD SHA256 `ee57a285c08603fac7d83c13d35613c825fe93ea7d63a20e448c2479625cf37b`.

Fixture de caracterizaÃ§Ã£o/busy SHA256 `fa9c44bdcdfdb847f71bf5ffc516036833192c2e27d82e70a04dd4a7c3302d79`.

Novo qualificador Selected Release SHA256 `7370cda636ad279e5e35b4993ec6da67f688d9c80065ea6b26f6f14942d42be2`. NÃ£o substitui o hash operacional histÃ³rico e nÃ£o recebe autorizaÃ§Ã£o pela compilaÃ§Ã£o. [Hashes das quatro configuraÃ§Ãµes](artifact-hashes.json). Launcher, pins, dossiÃª, decisÃµes accepted, fontes do helper A1/ciclo/observer e binÃ¡rios histÃ³ricos preservados conforme checks finais. Nenhum comando positivo executÃ¡vel preparado.

## PrÃ³ximo gate

Revisar e autorizar separadamente a menor implementaÃ§Ã£o A1 com reserva prÃ©via nos mesmos dois slots, cobertura de TODOS os callers e gates pÃ³s-erro. A proposta declara seus invariantes ainda por provar e o custo de escalares por reconciliar; nÃ£o presume viabilidade completa nem pede aumento de quota. NÃ£o avanÃ§ar a qualificaÃ§Ã£o positiva de lifetime como se A1 estivesse resolvido. Resultado funcional B1 completo no escopo reduzido pelo owner; objetivo original A1+B1 permanece parcial por A1 STOP.

CURRENT_TASK_COMPLETED=YES_OWNER_RESCOPED_B1_AND_A1_PROPOSAL_ONLY
RETENTION_SATURATION=STOP_UNFIXED_PROPOSAL_ONLY
N0_DUPLICATE_CALLBACK=CORRECTED
OWNERSHIP=UNCHANGED_FOR_B1_A1_NOT_QUALIFIED
CALLBACK_REDUCTION=2_TO_1_PER_SUCCESSFUL_N0_CURRENT
NATIVE_HANDLE_REDUCTION=NOT_CLAIMED_WITHOUT_MEASUREMENT
BUILDS=4_PASS_SELECTED_LEGACY_DEBUG_RELEASE
WARNINGS=0
OFFLINE_TESTS=44_OF_44_PASS_A1_CHARACTERIZATION_NOT_SECURITY_PASS
NATIVE_TESTS=COMPILED_C_MODELS_ONLY_FACTUAL_LAB_NOT_RUN
NETWORK_TESTS=NOT_RUN
START_EXECUTED=NO
STOP_EXECUTED=NO
PREFLIGHT_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_10080_BASELINE_FILES_CHECKED_ONLY_4_INTENDED_DELTAS
FIRST_UNRESOLVED_DIVERGENCE=A1_RETENTION_OWNERSHIP_BEFORE_ACQUISITION_NOT_IMPLEMENTED
NEXT=A1_OWNERSHIP_RESERVATION_FOCUSED_REVIEW_BEFORE_NATIVE_C_POSITIVE_LIFETIME_ISOLATED_QUALIFICATION
