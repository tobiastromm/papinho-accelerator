# SSO-B — aceitação e integração parcial offline

## Decisão e escopo

Alternativa B/S0–S7 ACCEPTED em
`docs/adr/DECISION-personal-scq-closed-startup-s0-s7-20261030.md`, antes do código
dependente. SHA256 original confirmado:
`60cdd50be68fa15c1379c31f287edd697140cfc2bf39ff8b319b86f0e5f08fa3`.
A reconciliação anterior sem aprovação permanece histórica. Nenhum accepted
anterior ou proposta foi editado. Branch main e HEAD preservados.

## Código e evidência

`scqa_startup_original` verifica o original vivo do controller já consumido,
a tomada do perfil, referências originais revalidadas, contexto e lifetime.
Exige runtime ainda não vinculado, não utilizado e sem PID/criação fabricados.
Retorna borrow exclusivamente LOCAL: não atravessa processos, não admite
ServiceMain e não autoriza Start. `scqs_take` integra essa verificação quando
aguarda runtime. O owner canônico oferece consulta nativa de sua própria fonte,
sem setter ou argumento contendo prova/grant fornecido por caller.

`scqr_closed_ready_current` consulta evidência receptora: origem local adquirida,
ato recebido/consumido, entrega autenticada correlacionada, identidade atual,
referências originais, IO concluído e nenhuma perda/erro/Stop. Rejeita origem do
controller usada como ponteiro receptor, query-only, pending, peer perdido,
consumo ausente e outro owner. Receipt DATA posterior não equivale a essa evidência.
A consulta do owner não chama SetServiceStatus nem abre KSP, listener ou CA.

Não há novos owners, estruturas persistentes, handles ou grants. Layout das
estruturas permanece inalterado. Consultas usam plano temporário na stack;
pico transitivo/kernel/provider permanece UNKNOWN, sem crédito ou quota nova.
Limites124/128/1024 preservados. Relatórios antigos “contrato não designado”
foram corrigidos no comentário/bit privado: agora falta implementação/evidência
e entrega original, não uma segunda aceitação de S0–S7.

## Limite decisivo: não há cadeia completa

O resultado é PARCIAL. Não demonstrada a cadeia integrada
controller original -> admissão anterior ao ServiceMain -> bilateral -> RUNNING
fechado. As duas consultas qualificadas são componentes locais, não essa prova.

1. `personal_scq_registered_owner_decision`, endpoint e CNG continuam NULL:
nenhuma decisão operacional específica foi aprovada. A aprovação SSO-B não pode
preencher esses registros; nome/PID/SID/receipt/metadados não suprem a origem.
2. `main.c.api3.inc` despacha ServiceMain antes de `papacc_server_entry`;
`ps_main` ainda exige a fonte original antes de chamar o runtime. Não há referência
administrativa original recebida nesse ponto. Nenhum caminho positivo foi criado.
3. O perfil completo `scqp_peer`, `scqp_plan` e `scqs_runtime_snapshot` continuam
exigindo SERVICE_RUNNING. A consulta receptora atual depende desse perfil e do
host corrente. Portanto ela NÃO resolve a readiness anterior a RUNNING e não é
usada como atalho para reportá-lo. Os testes positivos receptores têm peer
RUNNING modelado, distinto do controller pré-runtime testado separadamente.

Não se provou impossibilidade arquitetural de SSO-B. Existe ainda lacuna de
implementação da preparação fechada e entrega original pré-readiness no mesmo
runtime. Seria incorreto atribuí-la apenas à falta de aprovação operacional ou
emitir pedido executável agora. Nenhuma nova ADR é necessária por inferência:
S0–S7 já autorizam a implementação offline restante. Uma aprovação operacional
isolada não faria funcionar a composição atual. Manter DENY.

## Qualificação e divergência corrigida

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings na campanha final.
Seis CTests afetados/configuração,24 execuções finais PASS. Fonte administrativa:
48 cenários/923 checks, incluindo15 pré-runtime. Receptor:31 cenários/757 checks.
Session driver e adapter são regressões afetadas pelo novo vínculo em take.
Três mutantes compilados e rejeitados: falta de tomada do perfil, reutilização
após consumo receptor e aceitação de query-only como readiness. Originais
restaurados e recompilados. Contagens são cobertura de modelos, não prova nativa.

Divergência de escopo: o teste herdado de adapter e seus includes utilizavam
BCryptGenRandom real para nome modelado. Foi alcançado nas execuções preliminares,
antes da correção. Não houve acesso KSP/chave, criação de endpoint ou alteração
persistente. Isso não atende à proibição CNG real deste pedido. O fixture foi
corrigido para RNG determinístico modelado, exclusivamente na TU de teste;
código de produto não recebeu fallback. A qualificação final foi repetida após
essa correção. Não alegar que nunca houve chamada CNG real nesta tarefa.

SID/security-descriptor helpers e threads Windows são reais; tokens, peers,
endpoints, conteúdo/proveniência de originais e lifecycle de serviço são modelos.
Não há autenticação bilateral nativa ou isolamento KSP real demonstrados. As
campanhas históricas inalteradas não foram repetidas como evidência nova.

## Estados obrigatórios

SSO_B_NORMATIVE_DECISION=ACCEPTED_OFFLINE_ONLY; decisão local referenciada acima.
ORIGINAL_STARTUP_SOURCE_IMPLEMENTED=PARTIAL_CONTROLLER_LOCAL; sem entrega/admissão ServiceMain.
CLOSED_DIAGNOSTIC_READINESS_IMPLEMENTED=PARTIAL_EVIDENCE_QUERY_ONLY; sem caminho pré-RUNNING.
BILATERAL_VALIDATION_STATUS=MODELED_COMPONENTS_ONLY; cadeia completa NOT_DEMONSTRATED.
SELECTED_DEBUG_RELEASE=PASS; logs SSB-Selected-*-build/ctest/LastTest.
LEGACY_DEBUG_RELEASE=PASS; logs SSB-Legacy-*-build/ctest/LastTest.
REGRESSIONS=24_FINAL_CTEST_EXECUTIONS_PASS;3 mutantes rejeitados.
NATIVE_EVIDENCE_STATUS=SID_SD_THREADS_ONLY; originais/peers/IO modelados, sem prova bilateral.
OPERATIONAL_AUTHORIZATION=NONE; autoridade produtiva DENY.
NEW_INSTALL_READINESS=NOT_READY; parent/SCM/token/CNG reais e composição fechada pendentes.
E21_STATUS=PENDING.
ED11_PHASE6_STATUS=PARTIAL.
NEXT_REAL_GATE=INTEGRATE_CLOSED_PREPARATION_AND_ORIGINAL_DELIVERY_BEFORE_RUNNING_OFFLINE;
depois bindings completos/pedidos operacionais separados. Não pedir outra aprovação SSO-B.

Hashes: SSB-SOURCE-HASHES.json, SSB-ARTIFACT-HASHES.json, SSB-QUALIFICATION.json.
Não há comando ou pedido operacional completo. PST0.6.4, WIP, chave fixture retida,
históricos, pins, UNKNOWN e reservas preservados. Nenhuma instalação, serviço,
endpoint, rede, TPM/Q4, CURRENT ou CA executados.
