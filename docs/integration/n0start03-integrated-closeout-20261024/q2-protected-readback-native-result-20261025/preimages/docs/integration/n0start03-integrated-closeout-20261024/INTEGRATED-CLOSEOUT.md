# N0START-03 — conclusão integrada e limites da prova

Estado: preparação técnica concluída; observação operacional pendente de aprovação específica. ED11 e Phase 6 permanecem PARTIAL. Não houve Prepare, preflight, Start/Stop, consulta SCM, alteração ACL ou criação de objeto operacional nesta tarefa.

## Correção e alcance

O observador real recebeu instrumentação mínima opt-in para separar a contagem do processo observado da tabela de handles do qualificador durante PSS e stack-VA. A opção CMake permanece OFF por padrão, exige pins read-only N0START03 e rejeita configuração positiva. Nenhum handle, owner, thread, logger ou campo persistente de ownership foi acrescentado. O estado local compilado tem 40 bytes; isso não representa o custo total de stack ou memória.

IG_PROCESS continua representando a contagem pré-PSS do processo alvo. IG_OBSERVER_WINDOW representa exclusivamente a tabela do coletor. Seus pontos amostrados não demonstram pico universal e não devem ser somados às tabelas de PowerShell/auxiliar. Falha na consulta causa erro, preservação da primeira falha e fechamento/retenção pelo mecanismo existente. Recursos internos não observados continuam UNKNOWN; não há crédito ED11.

O teste isolado chama ig_measure_process real sobre o próprio processo, incluindo PSS e VirtualQueryEx, sem executar o main operacional. Os cenários UNKNOWN injetados preservam a referência original e terminam por contenção do processo de teste; não demonstram fechamento factual nem quiescência global. A comparação de linhas não vazias após remover exclusivamente os blocos opt-in preservou o fonte anterior; isso não constitui prova de equivalência binária.

## Qualificação

- Selected Debug e Release: PASS, zero warnings.
- 14 resultados esperados do observador nativo isolado, incluindo negativos e contenção UNKNOWN.
- 30 regressões PASS (15 em cada configuração).
- Duas configurações CMake proibidas rejeitadas; parser do launcher sem erros.
- Caso limpo Debug: 56 → 57 → pico amostrado 58 → 56 após fechamento.
- Caso limpo Release: 40 → 41 → pico amostrado 42 → 40 após fechamento.

Essas contagens pertencem aos fixtures. Não demonstram margem LAB, economia operacional ou orçamento concedido. elapsed_ms zero pode decorrer da resolução do relógio. Não houve qualificação Legacy nova nem execução do qualificador operacional.

## Artefatos atuais

| Artefato | Bytes | SHA256 |
|---|---:|---|
| Qualificador Selected Release | 92672 | 635eaa3b55be85f7c21ac3f38084c304bcd527d39a0e6ad39fda3bf3d68283f5 |
| Launcher canônico | 11846 | 690ee3b3d2346902765259e533ac304bf2101a513d7a3bf7618ebc7d3dad43e3 |

Pedido candidato: READONLY-OPERATION-REQUEST.DRAFT.json, SHA256 2913091b3392cb24631565695b429a3b06153a4d8fa0c7b1facebb3cc9defcba. Não é aprovação executável. O candidato de registro mantém owner_decision=null e execution_authorized=false. Somente caminho e hash derivados do qualificador mudaram no launcher; semântica de confirmação, console, exclusividade e default EMPTY_DENY foram preservadas. Rota positiva DENY.

Os hashes anteriores deeff3b9…/daf6c078… e o pedido 63550997… são históricos, não pins do futuro procedimento atual. Preimagens byte-exatas estão em ed11-closure-drive. Relatórios anteriores de consistência e unchanged-operational-artifacts.json descrevem etapas anteriores; a evidência corrente é final-integration-checks.json e artifact-hashes.json.

## Gates restantes

ED11_GATE_MATRIX.csv contém 22 gates com fonte, alcance e pendência. PHASE6_GATE_SEPARATION.csv separa oito classes: o resultado deste LAB não conclui C2/C3, Browser, ED08/Model E, lifecycle Windows ou revogação end-to-end. Backup/recuperação institucional e sincronização bilateral deferidos pelo MVP pessoal não foram inventados como bloqueios novos.

Primeira propriedade ausente: observação fresca do contexto nativo real N0START03 — operador/token/boot, dossiê, censo, SCM read-only e recursos efetivos de qualificador, PowerShell e auxiliar. A observação histórica 126/128 não é orçamento fresco. O resumo opt-in apenas acrescenta amostras do coletor; pico não amostrado continua UNKNOWN e qualquer amostra acima de 128 impede conclusão favorável. Permanecem 128/1024, quatro papéis, freshness 60s/1s, headroom e NONE_GRANTED.

Mesmo uma observação read-only favorável não prova o pico positivo ou o serviço. B4.C prevê medição factual de serviço na futura tentativa diagnóstica; não se exige fabricar essa prova antes do primeiro Start. H0–H9 limita a exigência de teto retrospectivo de resíduos opacos históricos; referências controladas incertas continuam DENY, sem refund. Objetos históricos, P3, N7 específico e UNKNOWN permanecem preservados.

## Próxima ação concreta

O owner deve revisar e aprovar especificamente o pedido read-only atual acima, com os novos hashes. Somente depois poderá ser registrado o arquivo de aprovação real e fornecido seu SHA256 literal no comando manual. O procedimento manual candidato já está integralmente preparado em manual-procedure.NOT-AUTHORIZED.md; seu campo de aprovação futura é deliberadamente pendente. O registro executável permanece ausente. Não se reutiliza a aprovação histórica ea272d2a… nem se executa automaticamente.

Evidências: ed11-closure-drive/actual-observer-trace-results.json, regression-results.json, optin-guard-negative-results.json, optin-native-window-summary.json, trace-off-source-equivalence.json, final-integration-checks.json e preservation-final.json. Logs e preimagens permanecem no mesmo pacote canônico, sem criar pacote concorrente.

CURRENT_TASK_COMPLETED=YES
TECHNICAL_WORK_DONE=OPTIN_OBSERVER_QUALIFIED_AND_CANONICAL_PACKAGE_UPDATED
READONLY_OBSERVATION_APPROVAL_REQUIRED=YES
FIRST_UNRESOLVED_FACTUAL_GATE=FRESH_ACTUAL_N0START03_NATIVE_ADMISSION_AND_ROLE_RESOURCES
OPERATIONAL_PINS=PREPARED_NOT_AUTHORIZED
INDEPENDENT_BUDGET=NOT_GRANTED
POSITIVE_PATH_QUALIFICATION=NOT_DEMONSTRATED
ED11_TOTAL_STATUS=PARTIAL
PHASE6_TOTAL_STATUS=PARTIAL
PREPARE_EXECUTED=NO
START_EXECUTED=NO
STOP_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=OWNER_SPECIFIC_APPROVAL_ONE_MANUAL_FRESH_READONLY_OBSERVATION


## Atualização após resultado manual

O gate de aprovação/observação anteriormente pendente foi cumprido uma única vez pelo owner. Estado corrente e decisão subsequente: [MANUAL-RESULT-AND-NEXT-GATE.md](MANUAL-RESULT-AND-NEXT-GATE.md). Administração1002/1024; qualificador126 pré-PSS e128 amostrados, sem margem nesse ponto. Sem efeitos. ED11/Phase6 PARTIAL e positivo DENY; a pendência atual é capacidade simultânea positiva, não repetir a aprovação read-only histórica.


## Resolução de capacidade positiva

Revisão e novo experimento discriminante: [positive-capacity-resolution/CLOSEOUT.md](positive-capacity-resolution/CLOSEOUT.md). O mecanismo de ProcessStartInfo propagou oito eventos sintéticos herdáveis e zero não herdáveis, em Debug/Release; não há atribuição desse efeito aos126 reais. Sem redução funcional especulativa ou prova de impossibilidade. Capacidade positiva permanece bloqueada na atribuição startup/herança versus ownership e custos internos. Nenhuma execução operacional.


## Estado corrente: observação discriminante de tabela preparada

Ver [handle-attribution/CLOSEOUT.md](handle-attribution/CLOSEOUT.md). Método PSS suportado enumerou840 entradas em fixture e confirmou herança de eventos por objeto compartilhado. Opt-in no mesmo qualificador/owner implementado, Debug/Release zero warnings,32 resultados esperados+30 regressões+8 casos de mapa. Dependências incrementais corrigidas e artefatos finais recompilados. Pedido atualSHA256 243a8a74ce5346998f4f75b656fa028064793f3f082dd621043f1d358e5cb03e. Aprovação nova N0HT01 AUSENTE; registro anterior é histórico. Origem dos126 reais e capacidade positiva permanecem não demonstradas. ED11/Phase6 PARTIAL. Não executar automaticamente.


## Atualização: aprovação específica N0HT01 registrada

O owner concedeu autorização explícita para UMA observação manual discriminante vinculada ao pedido243a8a74ce5346998f4f75b656fa028064793f3f082dd621043f1d358e5cb03e. Registro exclusivo: handle-attribution/EXACT-HANDLE-READONLY-APPROVED.json, SHA256 56aa3bea6981a16151b116a313f6ee6c192a74dcb5f919791ca599778e6c0ef7. Artefatos e11 verificações de pins/dependências conferidos; registro histórico57610fea preservado. Comando completo: handle-attribution/OWNER-MANUAL-COMMAND.md. Execução manual ainda pendente; nenhum Prepare foi iniciado pelo Codex, nenhum teste/build repetido. A autorização não concede Start/Stop, efeitos positivos ou orçamento. ED11/Phase6 PARTIAL; próximo gate é receber o resultado manual real.


## Atualização: resultado discriminante recebido — conclusão B

Ver [MANUAL-RESULT-AND-ARCHITECTURAL-DECISION.md](handle-attribution/MANUAL-RESULT-AND-ARCHITECTURAL-DECISION.md).71→105→126 handles,1→9→27 slots controlados,984 administrativos;126/128pré-PSS e128 amostrados. FlagINHERIT em12 entradas,9não stdio ainda não provadas dispensáveis. Sem efeitos. Não repetir N0HT01. Proposta arquiteturalI0–I6 de herança explícita PROPOSED, sem implementação ativa ou alteração de accepted; capacidade positiva não demonstrada, ED11/Phase6 PARTIAL.


### Integração de herança explícita — impedimento reproduzido (2026-10-25)

Fixture adversarial nativo provou três transfers privados herdados por criador global concorrente; controle sem herança0. Console nativo real isolado/mesmo HWND/confirmação/Escape/F10 PASS.62 regressões+36 casos do mecanismo+6 cenários novos (18 iterações) PASS conforme resultados esperados, Debug/Release zero warnings. Resultado negativo impede integração canônica; launcher/pins intactos. ADR revisão3 PROPOSED compara coordenação global, helper incompatível com quatro papéis e bootstrap sem herança que exige revisão de transporte. Nenhuma nova observação LAB é qualificada; positivo DENY, ED11/Phase6 PARTIAL. Relatório: docs/integration/n0start03-integrated-closeout-20261024/explicit-inheritance-integration/CLOSEOUT.md.


### Transporte local sem herança — protótipo qualificado (2026-10-25)

Named pipe local exclusivo/DACL explícita, criação do filho sem herança e autenticação recíproca implementados em teste nativo.24 cenários/120 iterações+62 regressões PASS, Debug/Release zero warnings. Zero objetos privados herdados no concorrente do pai e no descendente do filho depois do binding CRT (_open_osfhandle/_fdopen, sem _dup2). Filho52→54→53; supervisor com custos de inicialização e estado144 bytes além de buffers/transitivos, sem economia operacional concedida. Normativa N5 ainda veda novo IPC: candidate T0–T9 aguarda decisão; launcher/pins intactos e nenhum LAB. ED11/Phase6 PARTIAL. Relatório: docs/integration/n0start03-integrated-closeout-20261024/noninherited-transport/CLOSEOUT.md.


### T0–T9 revisão2 — emenda exata e plano condicionado (2026-10-25)

N5 accepted permanece byte-exato. Emenda proposta restringe a exceção ao stdout local efêmero do qualificador03, sem comandos/control plane/daemon/outros consumers. Protótipo recebeu guardas de namespace/IDs e impersonation real, replay/lifetime e cleanup de ack corrigido:36 cenários/180 iterações +40 verificações puras PASS; Debug/Release zero warnings.62 regressões funcionais anteriores não repetidas sem mudança. Plano de integração fechado no mesmo launcher/owner e default EMPTY_DENY; pins operacionais intactos. Decisão normativa exata do owner é o único gate para começar a integração técnica já autorizada; nenhuma observação operacional aprovada. Ver t0-t9-normative-resolution/CLOSEOUT.md e conditional-integration-plan.md no pacote integrado. ED11/Phase6 PARTIAL, positivo DENY.


### Estado corrente — T0–T9 aceita e transporte integrado (2026-10-25)

Após aprovação normativa explícita, T0–T9 revisão2 foi registrada ACCEPTED por decisão específica, sem editar N5 original. O launcher canônico agora usa transporte stdout privado sem herança, opt-in read-only, default EMPTY_DENY/positivo DENY. Quatro builds zero warnings;60 regressões,52 resultados nativos e28 casos PowerShell/console PASS. Comparação no MESMO fixture PowerShell: filho64 inicial na rota anterior versus52 antes do bootstrap/53 conectado na nova, oito objetos privados antes versus zero depois; não é economia LAB ou grant. Supervisor tem novos custos; contexto nativo4464 bytes mais managed/CRT/stack/módulos/transitivos. Preservados RAW1/N7/H/P3/UNKNOWN e128/1024/60s1s/quatro papéis/NONE_GRANTED. Nenhum Prepare/Start/Stop/intent/SCM/ACL operacional executado. Gate restante: aprovação específica do novo pedido read-only; capacidade positiva não demonstrada, ED11/Phase6 PARTIAL e autoridades DENY. As descrições anteriores de decisão T0 pendente e launcher intacto são históricas, sucedidas por este estado.

Relatório consolidado: docs/integration/n0start03-integrated-closeout-20261024/t0-t9-acceptance-and-integration/CLOSEOUT.md.


## Observação manual do transporte privado — 2026-10-25

Read-only exit0/first0/close0, sem efeitos; qualificador118 antes do PSS, pico amostrado120; administração1005/1024. Aprovação específica usada, sem reutilização. Capacidade positiva e ED11/Phase6 permanecem PARTIAL/DENY. Reconciliação: `docs/integration/n0start03-integrated-closeout-20261024/private-stdout-native-result-20261025/RECONCILIATION-AND-NEXT-GATE.md`. Nenhuma nova execução ou alteração funcional nesta reconciliação.


## Composição positiva isolada — 2026-10-25

Dispatcher canônico antecipado e C1 last-use closes aplicados somente sob guards; quatro builds produto/quatro fixture zero warnings,68 resultados nativos isolados e60 regressões PASS. Pico limpo99 no fixture, não capacidade LAB; três refs live fechadas no ramo N0 após último uso, sem reduzir pico anterior. Sete pins históricos preservados; launcher operacional inalterado. Extensão positiva do transporte depende da proposta T10–T14, pois T0/T8 accepted mantêm positiva DENY. Relatório: `docs/integration/n0start03-integrated-closeout-20261024/positive-composition-closure/CONSOLIDATED-CLOSEOUT.md`. ED11/Phase6 PARTIAL; sem Prepare/Start/Stop/SCM/ACL ou consumo operacional.


## T10–T14 accepted e integração positiva — 2026-10-25

Decisão específica registrada após aprovação normativa explícita. Entrada positiva separada, transporte/launcher e pins derivados integrados; default EMPTY_DENY. Quatro builds zero warnings,72 regressões,48 fixtures PowerShell,4 bootstraps nativos sintéticos e27 gates puros PASS. Aprovação positiva original ainda AUSENTE; nenhuma execução operacional. Pedido SHA256 `218f012c98d1f65e6216fb21023113d266d252a0acd5cd17cee201509ee44efb`. Relatório: `docs/integration/n0start03-integrated-closeout-20261024/t10-t14-positive-integration/CONSOLIDATED-CLOSEOUT.md`. Registros anteriores descrevem seus respectivos momentos históricos; T10 não concede Start/admissão. ED11/Phase6 PARTIAL, custos operacionais/picos não amostrados UNKNOWN; 128/1024/NONE_GRANTED preservados. Próximo gate é aprovação específica da única operação manual positiva, seguida dos gates frescos, sem repetição read-only equivalente.


## Résultat positif manuel03 — échec pré-consommation (2026-10-25)

Owner output: exit2/first1460 PRE_METRIC_REVALIDATION/bd_census_predicate, starts0/stops0/intent0/effect0/diag0/close0; transport closed/retained0. Sem métricas finais ou efeitos. Aprovação usada sem retry; orçamento/capacidade não demonstrados. Cadeia por entrada do censo repete validação protegida completa; exact timestamp/observação expirada não registrado. Reconciliação: `docs/integration/n0start03-integrated-closeout-20261024/positive-native-result-20261025/RECONCILIATION-AND-FAILURE-ANALYSIS.md`. Primeiro trabalho técnico restante: equivalência segura de censo por geração limitada, sem aumentar60s/1s ou omitir provas. ED11/Phase6 PARTIAL, autoridades produtivas DENY.


## PRE_METRIC correction — 2026-10-25

Redundância de conteúdo corrigida somente no censo positivo pré-métricas: originais read/share-read retidos, escopo síncrono limitado, checks voláteis por entrada preservados, duas fronteiras completas sem renovar relógios60s/1s. Quatro builds sem warnings/76 regressões/27 gates PASS;18amostras nativas de callback sintético, não LAB. Fonte exata de expiração histórica não recuperável. Pins novos e endereço de aprovação separado; pedido NÃO aprovado SHA256 `64f20a0fcbe94782b3178c9da27ce0c76a989e478802d7e2262d3cef04d0e18b`. Relatório `docs/integration/n0start03-integrated-closeout-20261024/pre-metric-timeout-resolution-20261025/CONSOLIDATED-CLOSEOUT.md`. Sem operação/SCM/ACL/consumo; ED11/Phase6 PARTIAL. Próximo gate: aprovação manual positiva exata, depois gates frescos.


## Resultado nativo corrigido03 — 2026-10-25

Censos390/453ms first0, idade6.25/13.125s. Em PREFLIGHT_METRICS houve1816: administração767+270=1037/1024 (excesso13), qualificador112amostrados/128. Intent/effects/diag/starts/stops0, close0 e transporte fechado/retained0. Aprovação utilizada sem retry. Não há pico total/IG_HOST/serviço ou capacidade positiva; censo corrigido passou somente no alcance observado. Próximo bloqueio é atribuição de referências administrativas antes de qualquer nova operação, sem aumentar quotas. Relatório `docs/integration/n0start03-integrated-closeout-20261024/corrected-positive-native-result-20261025/RECONCILIATION-AND-NEXT-GATE.md`. ED11/Phase6 PARTIAL, autoridades DENY.


## Administrative handle correction, 2026-10-25

Five Cycle-only console emissions qualified offline; normalized27 fewer handles in exact-fragment fixtures, not LAB capacity. Four product builds zero warnings,76 regressions,52 PowerShell forwarding cases,20 native adversarial/fault outcomes,64 approval-reader cases and27 launcher gates PASS. Historical1037/1024 remains factual. Fresh positive admission and service lifecycle remain unproved; ED11/Phase6 PARTIAL, no credits. See admin-handle-budget-resolution-20261025/CONSOLIDATED-CLOSEOUT.md. No operational execution or new approval.


## N0START03 manual positive result, 2026-10-25

Administrative gate passed at993/1024 then995/1024; qualifier112/114/115 samples. Intent consumed, image/sink and rebind retained, one Start, no Stop. First qualifier error13 at OBSERVE_FROM_START_PENDING/n0_original_roles_missing; separate service psr_prepare predicate token.specific_group.enabled returned5. Own/transport close confirmed; original service handle termination evidenced quiescent1, not global quiescence/refund. No RUNNING or complete service/four-role metrics. Approval used;03 replay DENY. ED11/Phase6 PARTIAL. Evidence: docs/integration/n0start03-integrated-closeout-20261024/admin-budget-positive-native-result-20261025/RECONCILIATION-AND-NEXT-GATE.md. Next is token-contract diagnosis and consumed03 disposition, not another equivalent handle observation.


## Service token/startup diagnostic qualification, 2026-10-25

Existing first-failure observed_a/b now carry exact specific-SID group count/raw OR attributes on the group predicate; census denial emits expected/seen masks. Security guards unchanged. Four product builds zero warnings,76 regressions,32 actual-census-body simulated cases and4 isolated native restricted-token runs PASS. New candidates/pins NOT_ACTIVATED; consumed03 image/dossier preserved. Historical attributes and missing-role mask remain UNKNOWN. P3 pre-Start disposition cannot apply to03 starts1. No operational retry or new identity; ED11/Phase6 PARTIAL. Current evidence: docs/integration/n0start03-integrated-closeout-20261024/service-token-startup-resolution-20261025/CONSOLIDATED-CLOSEOUT.md.


## Post-Start03 disposition and identity coverage, 2026-10-25

Original local close/service termination evidenced; global quiescence/refund not proved. P3 excludes03 Start1; H/L grant no further retry. Canonical intent inventory corrected: `.intent` under installation exists103bytes (content denied), not unused docs manual-native-intent path. Old request/approval preserved; path discrepancy requires explicit review. Isolated borrowed-token/restriction/original-process correlation candidate:4 zero-warning builds,96 token/40 role models,4 own-token native reads/8 lifetime checks; NOT_INTEGRATED, no product source/pin/LAB effects changed. Q0–Q9 normative candidate PROPOSED only. See docs/integration/n0start03-integrated-closeout-20261024/poststart-disposition-and-token-diagnosis-20261025/CONSOLIDATED-CLOSEOUT.md. ED11/Phase6 PARTIAL; no new identity or authority.


## Q0–Q9 acceptance and diagnostic integration, 2026-10-25

Q0–Q9 revision1 was accepted against exact candidate SHA2567eab14040096173ccf28a56c818737115046c94ba657cac51920b36754e9656f; the original proposal remains unchanged. Bounded token metadata and original-process census correlation are now integrated and qualified offline. Four Selected/Legacy Debug/Release product builds,76 regressions,96 token model outcomes,40 role model outcomes,32 actual-census-body cases and12 isolated native checks passed with zero final build warnings. The original recorder/sink and quotas remain unchanged; incomplete prepare-token observation denies readiness. This is not new service-token evidence or operational admission. Historical03 intent/Start/effects remain preserved; Q2 NOT_APPLIED, global quiescence UNPROVEN, UNKNOWN retained. No new identity, intent, LAB execution, SCM/ACL mutation or operational pin activation. Next: exact protected historical inventory/Q2 evidence review and owner-selected Q4 identity annex, before any independently approved live observation. See docs/integration/n0start03-integrated-closeout-20261024/q0-q9-acceptance-and-integration-20261025/CONSOLIDATED-CLOSEOUT.md. ED11/Phase6 PARTIAL; CURRENT/signature/recovery DENY.


## Q2/Q4 protected readback preparation, 2026-10-25

Exact historical hashes verified; read-only SCM still shows preserved03 ImagePath/STOPPED PID0/Win325/no recovery actions or triggers. The current sandbox SID is not the authorized elevated operator. A fixed original-parent NtCreateFile/read-only inspection reusing pinned native RAW1 and the unchanged03 dossier reader is qualified: PowerShell5.1 compile warning level4/as-errors,30 models, native synthetic same-handle/sharing/absence/Stop/close checks and three pin/confirmation negatives PASS. Protected Verify NOT_EXECUTED; no new protected denial probe, LAB effect, product/launcher/pin change or Q2 application. Historical image/intent IDs unprinted remain distinct from future current observations; no hash-to-identity substitution. Q4 has no new selected identity. Next is exact owner manual read-only approval and factual output, followed by Q2 evidence review; ED11/Phase6 PARTIAL. See docs/integration/n0start03-integrated-closeout-20261024/q2-q4-protected-readback-and-disposition-20261025/CONSOLIDATED-REVIEW.md.
