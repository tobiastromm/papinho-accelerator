# L5/P5 — análise de cobertura histórica para uma sucessora

## Conclusão e alternativas

**Cobertura quantitativa independente não demonstrada.** A revisão permite propor B, não escolhê-la como autorização: H0–H9 identifica precisamente a garantia que faltaria se o owner optasse pela avaliação empírica de uma sucessora. A continua preferível se houver prova suficiente; C é obrigatório enquanto B não for aprovado ou houver obrigação controlada incompatível. Não há budget PASS, identidade nova ou admissão.

A justificativa restrita para submeter B é a conjunção P3: execução consumida ANTES da chamada Start, referências e borrows próprios fechados, ausência de pendências conhecidas e efeitos inventariados/preservados. Isso distingue resíduo opaco de OS de uma operação de serviço ainda pendente. Não prova que todo custo físico terminou. A continuidade proposta aceita uma limitação de atribuição retrospectiva, não uma liberação de recursos. Se a lacuna atingir uma referência/reserva controlada, essa justificativa deixa de valer e C prevalece.

[Candidate H0–H9](../../adr/ADR-CANDIDATE-l5-p5-target-specific-historical-resource-coverage-20261024.md), revisão1, permanece PROPOSED. Não modifica accepted, código, launcher ou pins.

## Autoridades, integridade e alcance da revisão

[Preparação de binding anterior](../independent-lab-successor-exact-binding-20261024/binding-and-resource-eligibility-review.md); L0–L9; P0–P9/P3; N0–N9; B0–B6 revisão4/B+D; ED11 incremental; especificação1.0 §§11.2/11.3; ADR0030–32; Model E ADR0033 somente no escopo isolado. Esses documentos distinguem normas, fechamento local, retained ownership e incerteza empírica; não há autorização para transpor uma exceção de LAB à produção. Future Directions de PACC permanecem propostas de contrato, sem sobrepor essas decisões ou demonstrar implementação.

Baseline registra HEAD/main913c90cf2328f7939d8f5b927640601903ac6e91 e hashes/preimagem do WIP. Reconciliados8980 arquivos do baseline histórico com overlays qualificados e exceções conhecidas de capability/launcher; manifesto do pacote imediatamente anterior reconferido. Os hashes da política/owner/launcher permanecem os do baseline anterior. Nenhuma consulta nova ao SCM ou aos objetos protegidos: evidências abaixo são históricas/sequenciais, não snapshot atual nem admissão fresca. Não se executou build/teste nativo, ETW, fixture, preflight ou aquisição do mutex; revisão documental não precisa desses efeitos.

## Matriz A–F: origem, identidade, lifetime e obrigação

Origem comum: [recibo original](../n0start02-posteffect-reconciliation-20261024/owner-N0START-20261024-02-result.txt), SHA25630889b7ab36e08e5c8ad57ee7d0ac2c7ec118566d6972aaa504de12e9017ed26; [P2 qualificado](../p35-authorized-manual-readonly-result-20261024/p2-inventory-QUALIFIED-FOR-REVIEW.json), SHA2565e911ac326d58fc431c611d29327aed658b25a500f67c61e94ba2be042ed4eb0; [P3 aplicado](../prestart-administrative-disposition-acceptance-20261024/P3-EXACT-DISPOSITION-RECORDED.md), SHA256892d51b6a0033b3211c6483adb56d03d728c5f8285dfc8520cf73574c3ee5fad. A matriz não reescreve os rótulos anteriores à aplicação P3 nos arquivos históricos.

| Categoria/owner | Evidência/identidade | Lifetime comprovado e limite | Obrigação/cobertura futura |
|---|---|---|---|
| A — qualifier/launcher/owner e borrows controlados | Recibo final close0, retorno do launcher, cleanup vinculado às fontes/pins/P3.3–4 | Próprias refs/snapshots/locks fechados no alcance comprovado. Não é extinção kernel/cache. | Não repetir close nem contabilizar handle sabidamente fechado como vivo. Sem refund transitive; contradição futura bloqueia. |
| B — serviço da execução antiga | starts0 e ramo1816 antes de ig_start que incrementaria antes de StartServiceW; P3.2 | NOT_STARTED_BY_THIS_ATTEMPT. STOPPED/PID0 não é única prova nem quiescência histórica. | Não inventar processo/custo observado do serviço antigo; não requerer Stop de processo não iniciado. Atos de terceiros não universalmente auditados. |
| C — participantes administrativos | PowerShell21328/criação134373363180662661; auxiliar488/criação134373363180848380; qualifier20740/criação31286236:3190032344, apenas identidades HISTÓRICAS | Retorno do qualifier provado pelo recibo; término global de PS/auxiliar não demonstrado pelos handles originais. Admin1022→1032, qualifier124/128 são amostras antigas. | Censo futuro resolve identidades atuais e sobreposição; nada de nome/PID antigo como autorização, quinto papel ou baseline confiável. Medir PS+aux juntos<=1024, sem somar qualifier nesse teto. |
| D — intent, imagem e sink/arm | Intent native417614100:2097152:2358097/103bytes; imagem417614100:655360:2416599/434688bytes; sink417614100:1048576:2416813/1024bytes; hashes abaixo/P2 | Persistentes, refs próprias fechadas. Sink só arm sem NTSTATUS final. Persistência não demonstra handle vivo. | Preservar conteúdo/IDs/SD/tombstone. Não adotar como novos. Custos armazenamento/metadados não são zero. Sem overwrite/append/delete/rearm. |
| D — ImagePath/config/dossiê | SCM aponta à imagem histórica; config116bytes/hash a44b0e...89a44; RAW1 antigo5358/326bytes/IDs no P2 | Readbacks sequenciais históricos; não prova de estado fresco nem autorização nova. | ImagePath como pré-estado explícito; rebind somente por nova autorização. Novos claims e binding protegido; não relabelar arquivos. |
| E — OS kernel/cache/writeback/stack transitivo/console e lacunas entre amostras | P2 E, B2/B4.D, recibos agregados; owner/lifetime individual incompleto | UNKNOWN_RETAINED. Sem teto cumulativo por tentativa demonstrado. Picos ausentes não são consumo atual calculável. | H2/H3 propostos para esse resíduo apenas. Registrar observável e incerteza sem zero/refund/valor artificial. Reserva/borrow controlado não pode migrar para E. |
| F — marcador administrativo P5 | P3 registrado com norma P0–P9 | Disposto DOCUMENTALMENTE; permite avaliar, não reservar/consumir. | Sem crédito1GiB. Reservas REAIS, se materializadas, mantêm owner/lifetime/indisponibilidade até prova. Ausência de enumeração não prova inexistência. |

Hashes históricos de conteúdo: intent9e54d105d484c2201b5b280f9354038e9373f355f137c7e8a0debbca1ca45895; imagemeda84c0cf91ad1266cee462e62c89b84ffa5873853a0122215d15d666701361b; sink240d658c0cd8fdc2fdd6980f81c3a6419b1c8427876b766a664d686ebdd661e0. Os três somam435815bytes LÓGICOS; isso NÃO é tamanho alocado em disco, custo físico/kernel, reserva ou budget de handles. Outros arquivos/config/dossiê/metadata não estão nessa soma.

## Cobertura independente: seis grandezas distintas

| Grandeza | O que pode demonstrar | O que não demonstra / condição pendente |
|---|---|---|
| Capacidade física/commit atual | Elegibilidade agregada no instante das amostras frescas5/9GiB | Não atribui resíduo histórico, não garante futuro, nenhuma amostra nova feita aqui. |
| Reserva lógica controlada | Limite/capacidade atribuída a owner/lifetime específico quando houver prova | PS_ADMISSION/PS_BD_STATE mantêm refs/costs/tempo, não ledger persistente de refund nem alocação física1GiB. Falta enumeração não permite declarar todas reservas inexistentes. |
| Allowance administrativa | P5 distingue marcador disposto de recurso real | Não é bytes retornados, crédito ou direito de usar reserva velha. |
| Objetos persistentes | IDs/hashes/tamanhos/obrigações documentados | Não provam handles vivos, clusters físicos ou cache/stack agregado; não se adotam como novos. |
| Resíduo transitivo histórico | Parcela observável pode ser registrada; resto permanece E | Sem teto cumulativo comprovado. H3 é mudança de garantia explícita, não bound disfarçado. |
| Novos custos previstos | Caps controlados, guards por papel, limites do recorder e obrigações de refs conhecidos | Não são máximos medidos do OS nem garantia de handle saving. Demandam novo binding/implementação qualificada/admissão factual. |

Não existe equação factual `UNKNOWN <=1GiB`, nem `headroom - guards = crédito`. Também não se exige fabricar uma segunda alocação1GiB para provar independência de allowance que não foi alocada. A exigência é novo orçamento conhecido sem reutilização de recurso retido, plus decisão separada sobre a incerteza que não tem limite retrospectivo disponível. Medir host e participantes continua obrigatório; isoladamente não resolve H2.

## Comparação e justificativa da proposta

**A — prova quantitativa independente:** plenamente compatível e preferível se demonstrável. Os dados não contêm atribuição/upper bound histórico de E. Contagens1022→1032 não atribuem objeto/lifetime nem provam leak; fixtures do patch não qualificam capacidade real. Uma nova captura de LAB não é necessária nem autorizada para concluir essa lacuna.

**B — decisão de risco target-specific:** tecnicamente justificado SUBMETER à escolha do owner, pois P3 provou ausência de Start e fechamento controlado sem pendências conhecidas; só o resíduo opaco pode ser dispensado de atribuição/teto retrospectivo. Não pode ser apresentado como orçamento provado. H3 explicita precisamente a garantia relaxada e possibilidade de exaustão/overshoot. Exposição delimitada a um alvo/boot e UMA sucessora, sem retry e sem rede operacional/listener/CA/certificados/bootstrap/CURRENT. N7 separado. Sem aceitação expressa, B não opera.

**C — STOP:** vigente agora. Também obrigatório se faltar prova de referência/reserva controlada, se houver incompatibilidade histórica, impossibilidade de censo/métrica exigida, limite excedido, owner abandonado ou recusa do risco. Não se cria tentativa para escapar desse resultado. A aprovação H3 não poderia dispensar essas condições.

## Garantias preservadas, riscos e próximos gates

Quatro papéis,128nativos/1024admin e outros caps N6/B+D; freshness60s/1s; headroom5/9GiB/pisos4/8; NONE_GRANTED; sameowner/mutex/exclusividade; consumo único; N2/RAW1; Stop/UNKNOWN/exactly-once. Não relaxar assinatura/identidade/proveniência obrigatória como se fossem riscos contábeis. Dependência de OS confiável, ausência de atribuição universal, execução de terceiros e picos/gaps permanecem limitações expressas, não alegações de segurança.

Menor caminho: (1) revisar/aprovar ou recusar H0–H9, especificamente H3; (2) autorizar separadamente seleção documental e anexo exato de UMA identidade, sem GUID automático nesta tarefa; (3) reconciliar obrigações/reservas e vincular nova tupla/N7; (4) autorizar slice exato no owner/contexto/recorder/launcher existentes, mantendo entrada positiva fechada; (5) qualificar offline e produzir hashes reais; (6) preparação protegida/qualificação factual/aprovação manual exata próprias. Sem novo serviço/registry/credit authority/logger ou código para simular refund. Nenhum comando operacional é entregue aqui.

CURRENT_TASK_COMPLETED=YES
HISTORICAL_COVERAGE=PARTIAL_DOCUMENTED_OPAQUE_CUMULATIVE_BOUND_NOT_DEMONSTRATED
INDEPENDENT_BUDGET=REQUIRED_NOT_GRANTED
UNKNOWN_OBLIGATIONS=RETAINED
TARGET_SPECIFIC_DECISION=PROPOSED
SUCCESSOR_ADMISSION=DENY
NEW_ATTEMPT=NOT_CREATED
START_EXECUTED=NO
STOP_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
FIRST_UNRESOLVED_DIVERGENCE=OPAQUE_HISTORICAL_CUMULATIVE_COVERAGE_HAS_NO_BOUND_OR_SPECIFIC_RISK_APPROVAL
NEXT=TARGET_SPECIFIC_RESOURCE_DECISION_OR_SUCCESSOR_BINDING
