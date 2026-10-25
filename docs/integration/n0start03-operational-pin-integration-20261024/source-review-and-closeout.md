# N0START03 — pins opt-in integrados e qualificados offline

## Resultado

Mesma branch main/HEAD913c90cf2328f7939d8f5b927640601903ac6e91. Decisão N7/planoED11 SHA c17aa105580aa7680e8d97042abc9daa32640dcf1e69dcdce49c59b8462109ac, L/H/L1/P/P3/N/RAW1/B/ModelE e governança accepted aplicável lidos/conferidos na baseline; ED08/Future Direction PACC revisados como limites sem alterar protocolo/produção0.6.4. Revisão de fonte pelo agente executante, não parecer externo independente. Preimagens/diffs preservados.

Implementada apenas integração03 e gates necessários no owner/launcher existentes, sem novo serviço/owner/registry/recorder/thread. Nenhum preflight, Start/Stop, Place, consulta SCM ou alteração ACL executado. Objetos protegidos não foram copiados/regenerados; recibo/readback históricos continuam fatos distintos de métricas frescas.

## Perfis e autorização

CMake PAPACC_ENABLE_N0START03_READONLY_PINS=OFF por padrão. Header padrão permanece hashesvazios/IDszero e ig_n0_inputs_ready nega antes de aquisição. Build ON habilita somente os IDs/hashes recebidos e a rota read-only; combinação com PAPACC_ENABLE_N0_POSITIVE_LAB causa FATAL_ERROR, e combinação de macros também #error. Build opt-in usa PAPACC_N0_READONLY_PINNED_LAB, bloqueando Cycle/legacy/observer na entrada. Nenhuma rota positiva habilitada neste slice.

Launcher mantém arquivo canônico e PowerShell único; parâmetro Profile padrão EMPTY_DENY rejeita antes de lançar processo. Somente N0START03ReadOnly é candidato; Cycle rejeitado explicitamente. Prepare exige ExactApprovalSha25664hex, fixa novo qualificador, inputs protegidos e decisãoN7/ED11, operador/token/console e confirmações existentes. Native argc9 --prepare-n0 exige hash64hex extra; argc8 sem aprovação não serve ao opt-in. Não reutilizar o comando histórico de -02 ou prepare8.

Reutilizado ig_approval/ig_approval_file/cleanup/hash/originalhandle do ciclo existente, agora com caminho fixo opt-in docs/integration/n0start03-operational-pin-integration-20261024/EXACT-READONLY-APPROVED.json. Nenhum arquivo com esse nome foi criado. Sem registro/hash aprovado e revalidações, DENY. Não é uma autoridade nova: é o mesmo mecanismo de aprovação exata, restrito à operação read-only. Não se confunde com o arquivo protegido .approved, que continua dadoV2. Futuro registro só após aprovação explícita do owner, não deriva da aprovação de implementação.

Plano nativo existente agora inclui N7_ED11=c17... e o hash do registro de aprovação mesmo na rota read-only; identidades/hostboot/políticas/operador/token/desafio/arquivoOS/inputIDs já ligados permanecem. Conteúdo do arquivo de aprovação original é revalidado na fase protegida com o mesmo hash. Native source continua exigir identidade, proveniência de arquivo e N2/RAW1/freshness/lifetime, sem autenticação inventada de console/imagem mapeada. Launcher fixa/retém também a decisão documental original de N7/ED11. Gate de política de pins rejeita hashN7 vazio/divergente, sem aceitar booleano de prova externa.

O registro de aprovação é identificado pelo hash exato selecionado na autorização humana; a rotina existente não interpreta semanticamente JSON arbitrário como autoridade. Aprovação específica/handoff final devem conferir o conteúdo e vincular este pacote/artefatos. Não registrar aceitação operacional só porque o parser aceitou64hex.

## Pins factuais e artefatos

Dossiê V2: volume417614100/high1966080/low2672522/397bytes/SHA5087db7a1e02e3334e827360a464c25b3edc1b0059d0a01d506cf17da6ec4050. Evidência: volume417614100/high2883584/low2416818/7573bytes/SHA15b643829189855149826779e9b6a54c1650ed9df6360de63e90cd35bf4887d7. RAW1 dos dois0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814, originalhandle/mask7/owner/group/DACL/links/type/noreparse/currenthash revalidados futuramente. Não há SDDL/.NET reserializado como RAW1 nem mudanças de ACE/ACL.

Qualificador opt-in SelectedRelease: SHA 4e7f9cd090fb132d741d57cbb1754a410474b897375a272d8708d51439903407 em build/n0start03-operational-pins-20261024/Selected/Release/papacc_scm_instrumented_lab.exe.

Launcher canônico: SHA a96ad14d8ded2d9033599579462af154046fdee738e2d804512e6c40779ec009. O encaminhamento [Console]::Out.WriteLine($line) e todas as Write-Host administrativas permanecem iguais à preimagem. Runtime/recorder de serviço não alterados; hash runtime44d8597469bdd4f40b317f467029ee96b3ad69fdc270bcdbf5e126788f623e5c preservado. Não houve instalação/cópia de imagem operacional.

## Recursos

Nenhum cap ou crédito alterado. Quatro papéis,128handles nativos/1024admincompartilhados,memória/stack/threads,60s1s/headroom5/9 e4/8,UNKNOWN/H/P3/Stop/retention preservados. Static pins não concedem orçamento/reserva/refund. ig_preflight ainda exige observações completas; ausência de orçamento factual impede consume. Nenhuma avaliação de capacidade real nesta tarefa.

Sem estado permanente/thread/logger novos. Header contém constantes condicionais; plano usa buffer2304 existente com novo campo de decisão. O perfil read-only passa a manter a referência ig_approval_file do owner existente (antes usada na rota positiva); launcher retém referências de aprovação e decisãoN7. Esses custos NÃO são zero nem crédito: futuros handles/memória/stack e margens devem ser medidos junto aos participantes reais. Nenhum teto de handles ou melhora LAB garantidos. Não fechar referências obrigatórias para passar. Sem expansão da estrutura de contexto, serviço ou recorder.

## Builds e testes

Builds novos separados de históricos, mesma MSVC19.51x64/Ninja/SDKisolado0.6.5/perfilmanagedSelected3MiB vsLegacy2MiB; produção0.6.4 inalterada. Dois perfis(default/optin) x Selected/Legacy x Debug/Release =8 builds PASS, /W4/WX,0warnings. A qualificação recompila o serviço componente vinculado ao qualificador; runtimeexe histórico não foi alterado porque sua fonte/perfil não mudou. Logs finais refletem as fontes finais e dependências explícitas do novo teste de pins, sem confiar em showIncludes localizado. Ajustes durante qualificação foram seguidos de rebuild final8perfis.

88suítes/modelos PASS:11porconfig/perfil. Modelos antigos preservam N2ausente/defaultdeny/replay/V1antigo/identidadehashSD/token/boot/substituição/freshness/Stop/UNKNOWN/close; novos testes exigem pins positivos somente opt-in, IDsdivergentes/arquivoantigo/links/reparse/hashN7ausente/divergente/formatoaprovaçãoausente e guardsdefault/opt-in. Modelos B+D cobrem quinto processo/1025admin/129nativo; policytest acrescenta amostraausente/consumo sem admissão negados. Não são fatos nativos de orçamento ou prova de approval humano.

PowerShell5.1:9casos usando SOMENTE funções e guardas AST originais, mais2negações na função Pin para hashN7divergente e aprovaçãoexataausente. Launcher entry não foi executada. Fixture precisou import explícito Utility do PSHOME para contexto Python herdado, conforme launcher já fazia; correção só do harness, sem trocar toolchain. Compilefail esperado confirmou #error se combinados opt-in e positivo. Não confundir esse teste negativo esperado com falha dos8builds qualificados.

NATIVE_TESTS=executáveis nativos de MODELO, sem entrada real/SCM/serviço. Nenhuma execução de --prepare/--cycle/launcher ou captura ETW. Concorrência/Stop/retention canônicos continuam modelo/qualificação isolada histórica; não declaramos nova prova de mutex ou simultaneidade real desta tentativa.

## Handoff manual candidato — NÃO EXECUTAR AINDA

Launcher candidato é o arquivo canônico acima, ModePrepare/ProfileN0START03ReadOnly. READONLY-OPERATION-REQUEST.DRAFT.json descreve efeitos e argumentos; ExactApprovalSha256 é null deliberado, não autoridade pendente preenchida automaticamente. Não fornecemos comando aparentemente executável com hash fictício. Após aprovação específica de UMA execução read-only, registrar o conteúdo exato em EXACT-READONLY-APPROVED.json, conferir seu hash e emitir comando manual com TODOS os hashes literais. A aprovação atual não o autoriza.

Futura operação limita-se a consultas read-only de operador/token/hostboot/serviço/recovery/SCM/inputs/auxiliar, mutex temporário canônico, censo/métricas, confirmações, saída/fechamento/retention. Não consumo/rearmeintent/Start/Stop/rebind/imagem/sink/ACL/inputs/retry. Um único PowerShell5.1x64 já elevado e interativo, script pinado; auxiliarPID/criação e desafios confirmados no contexto real, sem valores históricos. Gates divergentes/ausentes/excesso/stale/UNKNOWN => DENY sem efeitos; se references uncertain preservarowner sem kill/forcedmutex/cleanup. F10 somente após retorno/fechamento factual.

CURRENT_TASK_COMPLETED=YES
OPERATIONAL_PINS=INTEGRATED_OPTIN_READONLY_ONLY
DEFAULT_PROFILE=EMPTY_DENY
READONLY_ROUTE=OFFLINE_QUALIFIED_EXACT_MANUAL_APPROVAL_REQUIRED
POSITIVE_ROUTE=DENY
BUILDS=8_DEFAULT_OPTIN_SELECTED_LEGACY_DEBUG_RELEASE_PASS
WARNINGS=0
OFFLINE_TESTS=88_MODEL_RUNS_PLUS11_PS_CASES_AND_FORBIDDEN_PROFILE_DENIAL_PASS
NATIVE_TESTS=MODELS_ONLY_NO_FACTUAL_ADMISSION
PREFLIGHT_EXECUTED=NO
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_EXCEPT_AUTHORIZED_PATCH
FIRST_UNRESOLVED_DIVERGENCE=EXACT_READONLY_OPERATION_APPROVAL_MISSING_FRESH_BUDGET_NOT_MEASURED
NEXT=N0START03_MANUAL_READONLY_PREFLIGHT_APPROVAL
