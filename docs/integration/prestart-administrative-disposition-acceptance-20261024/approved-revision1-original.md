---
status: proposed
revision: 1
date: 2026-10-24
scope-level: project
scope-target: administrative disposition of consumed pre-Start LAB operation N0START-20261024-02 only
supersedes: []
---
# Candidate — disposição administrativa pré-Start com efeitos retidos

## Status, autoridade e alternativas

TASK=PACC-PHASE6-PRESTART-CONSUMED-ATTEMPT-NORMATIVE-REVISION-1 autoriza somente preparar esta revisão. P0–P9 são PROPOSED, sem aprovação presumida, implementação ou efeito. Este documento é a única proposta normativa desta tarefa; a matriz de revisão vinculada é evidência analítica, não outra autoridade.

| Alternativa | Resultado |
|---|---|
| A — manter STOP sob contrato atual |Seguro enquanto a prova exigida faltar; não há regra atual para retirar administrativamente o bloqueio integral da allowance. Não se presume STOP eterno: novas provas ou decisão podem alterar o gate.|
| B — fechamento administrativo pré-Start |Selecionada: reconhece fechamento local comprovado, preserva obrigações e permite revisão posterior de elegibilidade sem refund fictício.|
| C — nova admissão independente |Somente avaliação futura subordinada a B e a decisão exata própria; não é autorização derivada nem coexistência automática.|

## P0 — fronteira exata e precedência condicional

Se e somente se explicitamente aprovada pelo owner e registrada em decisão normativa designada, esta revisão permite avaliar a disposição de **N0START-20261024-02**, tentativa `75330d11cc7e4123ae62eadc9a59c2f5`, serviço existente `PapinhoAcceleratorLab20261023`, no host/boot/configuração/binário fixados em N0/B0. Não é política automática para todo LAB, todo erro pré-Start ou novas repetições. O boot histórico236441 não se torna observação atual por constar deste texto.

A especialização focal atinge apenas: B4.B.5 quanto ao bloqueio categórico da allowance após disposição pré-Start; N5 quanto à separação entre fechamento local e disposição documental; N8 quanto ao caráter irreversível da autorização antiga e possibilidade de submeter outra identidade à revisão. Não altera os bytes das decisões accepted, a verdade histórica, o predicado de fechamento de qualquer referência nem Model E/ED08. B2, B4.B.1–4/6, B4.C/D, B5/B6 e N0–N4/N6/N7/N9 permanecem vigentes nos aspectos não expressamente especializados. N0/B0 ainda exigirão decisão exata de escopo para qualquer sucessora.

Não amplia a exceção N7 para outra tupla, não autoriza segunda admissão simultânea, outro owner/registry/serviço/logger/credit manager ou mecanismo geral de recuperação. O efeito desta revisão é documental e de política; nenhuma nova capacidade de execução decorre dele.

## P1 — fatos históricos imutáveis e evidência mínima

Preservar separadamente: INTENT=CONSUMED; STARTS=0; STOPS=0; LOCAL_CLOSE=PROVEN no alcance dos recibos; QUIESCENCE=UNPROVEN; IMAGE=PRESERVED; SINK=PRESERVED; SCM_IMAGEPATH=MODIFIED_PRESERVED; FIRST_ERROR=1816. O recibo original, seus hashes, origem fornecida pelo owner, aprovação histórica e fontes/binários correspondentes permanecem byte-exatos. Documento de revisão não substitui o recibo original nem vira prova nativa nova.

A ausência de chamada StartService deve ser demonstrada pelo prefixo de execução e vínculo exato ao código/binário aprovado, não somente pelo contador textual ou por estado SCM. Para esta execução, ig_start incrementa starts antes da chamada e o ramo de erro1816 antecede esse ponto; starts0 é coerente com esse caminho. Registrar limites de confiança nos recibos/código/OS e a ausência de auditoria universal de atos de outros administradores. Ausência de PID, arquivo ou evento isolado não basta.

Não exigir Stop nem quiescência de processo de serviço que não foi iniciado pela tentativa. Registrar NOT_STARTED_BY_THIS_ATTEMPT, sem fabricar PID, token de serviço ou custo observado. Isso não afirma ausência universal de qualquer execução do serviço por terceiros nem quiescência global/histórica.

## P2 — categorias e obrigações independentes

| Categoria | Disposição exigida | Consequência contábil/operacional |
|---|---|---|
| A — referências próprias |CLOSED_EVIDENCED somente para handles, snapshots, pins, locks e borrows originais cujo fechamento foi comprovado; close-failed ou perdido permanece UNKNOWN|Não fechar novamente; não manter contagem fictícia de handles sabidamente fechados; nenhum refund transitive decorre disso.|
| B — processo do serviço |NOT_STARTED_BY_THIS_ATTEMPT conforme P1|Não executar Start/Stop para provar cleanup; quiescência global continua UNPROVEN.|
| C — processos administrativos compartilhados |Distinguir processo de suas referências emprestadas; registrar identidade/criação, métricas históricas e lifetime efetivamente conhecido|Fechamento do handle não prova término de PowerShell/auxiliar; não reclassificar operação anterior como baseline confiável por nome/idade.|
| D — objetos persistentes |Intent tombstone, imagem, sink/arm, ImagePath e dossiê preservados com IDs/hashes/estado e obrigações de acesso|Persistência não implica handle aberto; arquivo fechado não implica exclusão. Sem adoção, overwrite, rollback, rearm ou autoridade da sucessora.|
| E — custos/lifetimes desconhecidos |UNKNOWN_RETAINED, com owner/dependência conhecida, razão, alcance e evidência ausente|Não preencher zero, marcar DESTROYED ou retirar quota realmente retida; apenas consulta segura quando Model E exigir.|
| F — allowance ED11 |Separar marcador de exclusão administrativa, reserva lógica controlada real e margem operacional de risco1GiB|Não são uma alocação física única. Aplicar P5 a cada componente; nunca emitir crédito1GiB nem saldo reutilizável por diferença de métricas.|

Cada registro deve conter identidade da tentativa/objeto, categoria, evidência de origem e hash, owner/lifetime/dependências conhecidos, estado, limite de prova e obrigação remanescente. Não reduzir o conjunto a `closed=true`. Categorias C/E podem permanecer UNKNOWN mesmo com A fechado e B não iniciado.

## P3 — transição documental com prova conjunta

A transição proposta é:

`CONSUMED_BEFORE_START → ADMINISTRATIVELY_CLOSED_WITH_RETAINED_EFFECTS`.

Requer simultaneamente:

1. Origem e integridade reconciliadas dos recibos, aprovação histórica, binários/configuração/política e identidade exata; divergência de hash/geração ou lacuna de origem invalida a prova.
2. Prova de nenhuma chamada StartService por essa execução, sem qualquer Start tentado cuja conclusão seja incerta. Chamada que falhou ainda é chamada e fica fora desta regra.
3. Fechamento comprovado de TODAS as referências próprias abrangidas pelo owner/launcher e seus empréstimos controlados; final close0 coerente com fontes e retorno, sem ramo de retenção/close-failure. Recibo final ausente, perda abrupta ou borrower pendente nega a transição.
4. Ausência de pendências conhecidas sob controle desses owners: I/O/callback/snapshot/lock/buffer emprestado e operação SCM em curso reconciliados segundo os contratos existentes. Retorno síncrono prova somente seu alcance; kernel/cache transitive continua em E.
5. Ledger P2 completo, efeitos preservados e reconciliados por consultas adequadas de identidade/conteúdo/configuração, sem colisão com a história; acesso negado ou substituição de objeto obrigatório mantém disposição pendente.
6. Revisão oficial da aplicação exata e aprovação específica do owner para registrar a disposição. Aprovar esta regra não aplica automaticamente a transição ao caso concreto. Estado administrativo deve referenciar os recibos originais e a decisão que o autorizou, sem reescrevê-los.

O registro deve ser exclusivo, não sobrescrever o original e não fazer nova chamada de close/release. Sua eventual colocação/preparação é outra operação autorizada; este candidate não implementa formato, parser ou arquivo consumível pelo runtime. Não criar segunda autoridade: o registro documenta a decisão; não substitui admissão do owner canônico.

## P4 — alcance e proibições da disposição

A disposição encerra administrativamente a execução consumida no alcance A/B e registra C/D/E/F preservados. Não declara sucesso diagnóstico, leak-free, STOP_COMPLETED, QUIESCENCE_PROVED, NATIVE_DESTROYED, refund, ausência de custos de OS ou retorno do sistema ao estado anterior.

Não muda consumed1, first1816, effect2, diagnostic_created1, close0 ou quiescent0; não limpa tombstones, flags, locks ou mutex por força. Não permite retomar a tentativa, completar seu Start, reabrir o sink para append ou usar sua aprovação novamente. Sem novo FINAL nativo ou correção retroativa do primeiro erro.

Se houver PRECONDITION_RETAINED ou CLOSE_UNRESOLVED_RETAINED real, sua autoridade e restrições específicas permanecem. P3 não resolve nem contorna esses estados; referência incerta controlada impede P3. Não importar enums de PST para declarar que arquivo/SCM foi destruído.

## P5 — allowance: exceção delimitada, sem refund

Sob B4.B.5 atual, a allowance integral permanece indisponível até fechamento factual. **A mudança de garantia proposta** é permitir que, após P3 aplicado por decisão exata, o marcador administrativo da allowance da execução pré-Start deixe de ser, sozinho, impedimento absoluto a uma análise de nova admissão. Não se infere extinção física do risco/custo antigo. O owner deve aceitar expressamente essa separação e o risco residual P7; o mero nome novo não a produz.

Essa especialização NÃO permite reutilizar reserva controlada, crédito alocado, quota de recurso vivo/retido, dependência ou capacidade com lifetime incerto. Cada reserva real preserva sua identidade e indisponibilidade até fechamento factual pelo contrato correspondente. E permanece UNKNOWN_RETAINED; sua obrigação não some da avaliação futura. Se não for possível distinguir a allowance de uma reserva real ou determinar sua obrigação, permanece bloqueada.

Nenhum crédito1GiB é concedido, transferido ou reembolsado. HOST_CREDITS=NONE_GRANTED e reusable/refund credit0 permanecem. Uma nova admissão não pode ser financiada pela allowance antiga, por2–10handles históricos, pelo patch de console ou por memória aparentemente livre. A margem física/commit nova deve ser medida integralmente conforme política então aprovada; estoque de arquivos, reservas controladas e custos compartilhados devem ser reconciliados sem dupla contagem nem omissão.

Custos UNKNOWN obrigatórios não ficam satisfeitos por medição livre do host. A análise futura deve demonstrar que as obrigações conhecidas/retidas e o novo orçamento podem coexistir sem reutilização, ou identificar explicitamente a categoria residual não mensurável e obter decisão target-specific separada, sem dispensar limite rígido ou criar crédito. Esta revisão não estende automaticamente B4.D/N7 à sobreposição de outra tentativa. Se essa demonstração/decisão não existir, DENY. P5 autoriza avaliar, não consumir orçamento nem iniciar coexistência.

## P6 — gate independente para eventual sucessora

Nenhuma identidade, GUID, intent, arquivo, dossiê ou aprovação sucessora é criado. Uma futura proposta deve conter nova identidade, novos nomes exclusivos de intent/imagem/sink/arm, dossiê vinculado, binários/configuração/políticas e pins reais, e decisão exata de escopo N0/B0. Novos IDs de arquivos/PIDs/tokens/métricas devem ser factuais, nunca preenchidos antecipadamente. Manter os objetos históricos em sua própria identidade; fontes de evidência estática só podem ser referenciadas após revisão de aplicabilidade, não adotadas como nova autorização.

Antes de qualquer efeito futuro: disposição P3 aprovada e aplicada; ledger histórico integral; ausência de obrigação incompatível, pendência ou autoridade anterior ainda ativa; exclusividade pelo MESMO owner/mutex qualificada; orçamento independente sob P5; quatro papéis reais no máximo, incluindo processo administrativo anterior ainda relevante caso exista;128handles por nativo/1024administrativos compartilhados e todos os demais limites;freshness60s/1s, headroom5/9GiB e pisos4/8GiB; N2/RAW1 e confirmação exata; risco N7 reavaliado e explicitamente aceito para a nova tupla. Não somar limites de duas tentativas para criar teto maior ou ocultar quinto processo.

Uma lacuna de lifetime não é resolvida por PID ausente, novo boot, abandono de mutex ou process disappearance. Não tomar posse automaticamente de mutex abandonado ou forçar liberação. Qualificação futura precisa distinguir exclusividade atual de exclusividade histórica; a decisão administrativa não prova ambas.

ImagePath preservado continua efeito anterior; eventual mudança requer revisão/autoridade exata. Ausência de nomes deve ser provada canonicamente antes do consumo, com CREATE_NEW/FILE_CREATE finais; consulta prévia não é reserva atômica. Sem overwrite, retry, rollback ou cleanup automático. Aprovação normativa, implementação/qualificação, disposição histórica e aprovação operacional manual são gates distintos. Nenhum comando positivo deriva deste documento.

## P7 — riscos residuais e preservação dos contratos

O risco específico novo é considerar encerrada a obrigação de exclusão administrativa de uma allowance pré-Start sem afirmar liberação física de todos os custos transitivos ou quiescência global. Os custos permanecem registrados; medição futura pode não atribuir completamente kernel/cache/console, e registros agregados dependem do código/pins e OS confiáveis. Não é prova de ausência de efeitos de terceiros, ausência de vulnerabilidade ou ausência de leak.

O owner deve aceitar esse risco para este caso exato, além dos riscos já aceitos, sem converter isso em dispensa de A/P3 ou de fato obrigatório N2/ED11. Falha de integridade, close incerto, pendência conhecida, Start possivelmente chamado ou reserva real ainda disputada não são riscos dispensáveis por P7.

ED08/Model E/ADR0030–32 mantêm precondições e exactly-once; nenhuma ordem de fechamento, callback, Session, provider, release ou wire muda. Scope isolado ADR0033 permanece isolado. Stop solicitado/aceito/concluído, quiescência e referência fechada continuam independentes. Não parar/matar processos para completar dossier; perda de console não é cleanup. Limites, quotas, quatro papéis, N7, NONE_GRANTED, produção0.6.4, CURRENT/signature/recovery DENY e ED11 PARTIAL permanecem.

## P8 — exemplos normativos de elegibilidade, não testes executados

| Caso | Resultado exigido |
|---|---|
| Recibos íntegros+fontes/pins; starts0 antes da chamada; refs/borrows fechados; sem pending conhecido; objetos intactos; C/E UNKNOWN explicitados; aplicação exata aprovada |PASS somente para P3 administrativo; quiescent0 permanece, refund0, sucessora ainda DENY até P6.|
| P3 aprovado; orçamento futuro independente completo; retenções compatíveis; novo escopo/risco/implementação e operação aprovados separadamente |Elegível à admissão canônica futura fresca; não PASS antecipado de Start.|
| starts0 sem recibo final/sem vínculo ao binário ou fonte |DENY P3; contador isolado insuficiente.|
| StartService chamado e falhou, retornou timeout ou conclusão incerta |DENY: fora da fronteira pré-chamada.|
| CloseHandle falhou, callback/borrow/IO controlado pendente ou owner perdido antes de recibo |DENY P3; preservar referências/UNKNOWN sem cleanup alternativo.|
| STOPPED/PID0 ou PIDs antigos ausentes como única prova |DENY; sem quiescência/lifetime retroativos.|
| Imagem/sink substituídos, intent apagado, hash/ID divergente |DENY; não adotar ou restaurar para fabricar prova.|
| P3 aplicado, mas custos obrigatórios UNKNOWN sem cobertura/decisão delimitada |P3 não é refund; DENY sucessora por P5/P6.|
| Novo GUID usa intent/sink/arm/approval antigo ou orçamento desconta1GiB/8handles |DENY replay/adoption/fictional credit.|
|129handles nativos,1025administrativos, quinto processo, freshness expirada ou obrigação anterior incompatível |DENY futura admissão; limites não se acumulam entre tentativas.|

## P9 — aprovação, preservação e próximos gates

Manter PROPOSED até revisão oficial integral e aprovação humana do conteúdo/hash completos P0–P9, incluindo expressamente a exceção P5 e seu risco P7. A aprovação desta norma não autoriza registrar o caso como ADMINISTRATIVELY_CLOSED nem implementar ou executar. Registrar ACCEPTED somente pelo processo autorizado, preservando proposta original/hashes, contratos anteriores, recibos, WIP e objetos históricos. Nenhum ADR accepted é editado nesta tarefa.

Após eventual aceitação, apresentar aplicação exata documental da disposição com evidências P3/P2 e autoridade específica, sem refund. Somente depois avaliar escopo sucessor; não criar automaticamente tentativa nem arquitetura. Qualquer integração funcional exige autorização e qualificação próprias, e qualquer efeito manual exige pacote exato aprovado separadamente.

PRESTART_DISPOSITION=PROPOSED
ADMINISTRATIVE_TRANSITION_APPLIED=NO
IMPLEMENTATION_AUTHORIZED=NO
REFUND_AUTHORIZED=NO
SUCCESSOR_AUTHORITY=DENY_UNTIL_SEPARATE_APPROVAL
START_EXECUTED=NO
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY

## Referências

- [Revisão factual pré-Start](../integration/prestart-consumed-attempt-disposition-20261024/disposition-review.md).
- [B0–B6 revisão4 accepted](DECISION-ed11-single-lab-diagnostic-native-boundary-20261023.md).
- [N0–N9 accepted](DECISION-first-native-lab-diagnostic-boundary-20261024.md).
- [ED11 incremental](DECISION-ed11-preserve-incremental-resource-envelopes-20261012.md), [ADR0034](ADR-0034-pacc-c2-revision3-contract-and-separated-gates.md) e [especificação1.0 §11.2/11.3](../protocol/pacc-c2-specification-revision-1.0.md).
- [Matriz de revisão e critérios](../integration/prestart-consumed-attempt-normative-revision-20261024/review-matrix-and-closeout.md).
