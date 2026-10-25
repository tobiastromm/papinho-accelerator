# A1 — viabilidade da reserva antecipada em dois slots

## Decisao e limite da qualificacao

**VIAVEL NO MODELO ISOLADO, CONDICIONADA A INTEGRACAO FUTURA QUALIFICADA.** Nenhum terceiro slot auxiliar necessario foi identificado nos dez schedules atuais revisados: confirmacoes exigem dois; os demais usam no maximo um auxiliar de cada vez. Reentrancia que precisaria de outra aquisicao e negada antes da API, mesmo se houver slot livre enquanto outra API esta em andamento. Nao se promete continuar com sucesso um callback que exija capacidade adicional; o comportamento e STOP, nunca terceiro handle sem owner.

O helper funcional `lab_close_aux` e TODOS os callers operacionais continuam inalterados. A1 permanece defeito funcional conhecido/STOP. O [patch candidato](A1-functional-candidate.NOT-APPLIED.patch) e o [bridge proposto](candidate-bridge.NOT-APPLIED.inc) NAO foram aplicados, incluidos no produto ou compilados como qualificador. Foram compilados/executados somente modelo e fault fixtures desta pasta, em processos sem autoridade LAB. Nenhuma identidade operacional, intent, imagem, sink, dossier ou reserva ED11 foi criada. Nao ha comando positivo executavel nesta entrega.

## Fontes e preservacao

HEAD/main 913c90cf2328f7939d8f5b927640601903ac6e91. Baseline desta tarefa registra10149 arquivos existentes nao ignorados; preimages preservam seis fontes de integracao e a live documentation antes do append. A proposta A1 anterior, closeout B1, ownership matrix, call graph e codigo atual foram lidos/confrontados. Model E/ADR0033, N0-N9, B0-B6/ED11, L/H/P e Stop/retencao mantem autoridade. Governanca Org/Papinho/local accepted ja carregada na sessao continua; nenhuma decisao accepted foi reescrita. PROJECT-CONTEXT.md continua ausente. Future C2 relevante continua direcao, sem mudanca causada por este modelo.

O callback B1 consolidado permanece byte-exato ao baseline (SHA90b0c771b3904b09db97d6c364f142ea2471d5547a4856910926cdc35ef15db7). O helper antigo permanece SHA c3cb9a3274b51c7e6bf6e3eb74708309bda8ad353d3b662dd9f52c5209f019f1 do arquivo scm-registration-lab.c. Checks finais verificam todos os hashes de baseline, pins/launcher e binarios historicos. Sem commit/push/tag/release, clones ou worktrees.

## Maquina de estados demonstrada

- Exatamente `HANDLE handle[2]`. FREE -> RESERVED antes de chamar adapter; reserva e ownership logico mesmo antes de existir um handle. O fake API verifica independentemente que recebeu o endereco do slot RESERVED persistente e a marca BUSY.
- Reserva de par e indivisivel no controle sincrono: ambos FREE e geracoes disponiveis, ou falha antes da primeira aquisicao. Dois slots ocupados fazem a terceira reserva negar sem API. Se um so estiver disponivel, confirmacao que exige dois tambem nega sem abrir IN.
- Adapter escreve diretamente no slot. Sucesso com handle valido -> OWNED; erro inequivoco sem referencia -> FREE. ERROR_NO_TOKEN so e ausencia esperada no thread-token caller; nao mascara ACCESS_DENIED ou outro erro. Falha com handle nao nulo ou sucesso com NULL/sentinel -> UNKNOWN + congelamento, preservando o valor observado e a incerteza. UNKNOWN(NULL) nao e prova de referencia fisica viva, mas tambem nao libera automaticamente o slot.
- O borrow e um lease (indice + geracao), nao novo owner. Begin/End mantem BUSY durante TODO o uso nativo, com begin de par unico para IN/OUT; nao se libera BUSY apenas por copiar aliases. Close/aquisicao reentrantes negam antes da API enquanto o borrow esta ativo. End precede qualquer close e propaga a primeira causa. Cada uso/close valida estado e geracao. Reuso de valor/slot nao valida lease antigo; overflow de geracao nega antes da API. Leases sao privados do unico owner e thread de controle; nao sao mecanismo de autorizacao, segredo ou capacidade exportavel.
- Close confirmado libera somente o slot correspondente. Primeiro close incerto -> UNKNOWN, valor preservado, Stop/retained/congelamento. Outro OWNED ja adquirido continua retido e rastreavel. Nem o slot UNKNOWN nem o outro OWNED recebe tentativa de close adicional depois de congelar.
- Apos freeze: zero novas chamadas simuladas de aquisicao, uso ou fechamento, valores dos slots e primeira causa imutaveis. Finish aux nega; dependencias/borrows/mutex nao sao liberados. Retencao conhecida externa tambem congela, inclusive com slot RESERVED ainda sem API.
- Stop antes de uma aquisicao a impede; Stop sem close incerto permite somente fechamento checado de referencias conhecidas. Finish do auxiliar significa apenas ausencia dessas referencias, nao quiescencia global, refund, fim de servico ou disposicao de efeitos persistentes. Nao existe reset operacional, retry, rollback, ampliacao de array ou credito.

Primeiro erro global e primeira falha CloseHandle sao escalares separados. O teste injeta1816 anterior ao close:1816 permanece primeira causa e ERROR_INVALID_HANDLE fica como primeira falha de close. Reentrancia negada durante API nao e substituida por sucesso da API externa; sua causa continua preservada. APIs retornam resultados locais; o bridge candidato deve preservar tambem o erro anterior do caller no retorno/recibo existente, sem criar logger.

## Cobertura factual e indireta

[10 callers](caller-coverage.csv) e [14 grupos de fronteira indireta](indirect-boundaries.csv). Os testes parametrizados reproduzem schedules de ownership, NAO executam os corpos reais dos callers nem qualificam suas autorizacoes/ACLs/identidades no Windows.

`lab_operator`: o token primario base continua no global lab_token; nao e um terceiro auxiliar. `lab_operator_current`: ausencia de thread token libera a reserva antes do token primario temporario. Confirmacoes: IN/OUT ocupam os dois slots, sem chamada explicita ao produtor N0 enquanto o par auxiliar esta aberto; os console handlers apenas sinalizam Stop. `cycle_live`: processos/token-base/access/snapshots continuam nos globals existentes; apenas o thread token transitorio usa o auxiliar e erro nesse ramo retorna antes da proxima thread. `ig_actor` conserva seu processo no observer e so empresta o token auxiliar. Intent legado e extract mantem o arquivo auxiliar ate close confirmado, sem ativar tais rotas neste teste.

A confirmacao canonica BD tem seus proprios input/output e owners; nao deve ser migrada arbitrariamente para estes dois slots nem somada como se fossem referencias perdidas pelo helper. Ela termina antes da revalidacao subsequente no caminho atual. Custos globais/referencias obrigatorias existentes continuam contabilizados; capacidade de dois auxiliares nao e limite total de handles do qualificador nem prova de folga128.

As entradas que precisam de gate ANTES de callbacks/aquisicoes estao no candidato: bind/current/aux_current canonicos em hook compilado somente pelo qualificador LAB; produtor/binding/roots/SD N0; census antes da revalidacao; observer antes de revalidacao e PSS; bind de processo; copy/intent/sink; extract e caminho done. Guardar somente lab_operator_current ou o final do census seria insuficiente. As protecoes originais ficam depois do novo gate no caminho nao congelado; nenhuma validacao e retirada ou substituida por flags externos.

## Reentrancia, Stop e retencao

O primeiro bridge candidato liberava BUSY apos copiar o alias; a revisao detectou a lacuna antes de qualquer aplicacao. O candidato final mantem um escopo de borrow completo, encerrado uma unica vez antes do close. Um mutante sem esse escopo foi rejeitado pelo teste de reclose durante uso.

BUSY protege aquisicao/uso/close sincronos contra reentrada que tente reservar, usar ou fechar. Essa reentrada nega e sinaliza Stop sem executar uma segunda API. Modelo e candidato exigem unico thread de controle; handlers externos apenas publicam Stop atomico pelo mecanismo existente. Nao e algoritmo lock-free/thread-safe para callers arbitrarios. Stop e observado em boundaries sincronos, sem promessa de interromper uma API em curso. Concorrencia real, excecoes nativas e callbacks Windows ainda exigem qualificacao de integracao; nao se conclui reentrancia impossivel.

O candidato suprime observacoes novas e desvia para a conducao existente de Stop/retencao. Nao terminaliza PSA.phase prematuramente por A1 antes da unica possibilidade de Stop ja autorizada. A chamada canonica admission_stop deve passar, ig_stop deve aceitar e SCM deve indicar estado/controles compativeis; a sugestao corrige no candidato o descarte desse retorno no ramo de Stop. Se owner ja esta terminal, nao ha nova autoridade. START_PENDING sem aceitar Stop resulta retencao; RUNNING ou START_PENDING com aceite sao apenas elegiveis a solicitar. Pedido, aceite, conclusao e quiescencia continuam separados. Nada e executado contra SCM aqui.

O modelo da fronteira de Stop e deliberadamente abstrato e nao prova retorno ControlService real nem estado futuro. Depois de close incerto, nao tentar fechar recursos dependentes para obter sucesso de cleanup. Model E query-only e N5/H7 preservam UNKNOWN; a unica excecao de acao de seguranca e o Stop que ja era autorizado, quando seus gates existentes permitem. Perda de console e timeout nao liberam ownership.

## Testes e limites de prova

Quatro builds do modelo: Selected/Legacy x Debug/Release, MSVC x64 da toolchain existente, /std:c11 /W4 /WX, zero warnings. Debug usa /Od /Zi /MDd; Release /O2 /DNDEBUG /MD. A1_SELECTED=1/0 identifica os dois perfis no fixture. Esses builds standalone NAO compilam o owner operacional ou exercitam os allocators/perfis managed do produto; nao foram trocados targets/backends do produto. Os scripts de build estao neste pacote e apontam somente para o novo build/a1-ownership-reservation-feasibility-20261024.

Por configuracao: **89 casos dirigidos PASS**, incluindo60 schedules parametrizados dos dez callers (falha da segunda aquisicao se aplica somente aos pares; nos singles esse parametro repete caminho simples), reserva1/2/3, par indivisivel, falha/ausencia/contradicao, dois OWNED, primeira e segunda posicao de close falho, tentativa posterior, reentrada/reserva e close, lease forjado/antigo, double-close, overflow, Stop, dependencias retidas, primeira causa, UNKNOWN e finish negado. [Resultados](test-results.json) e logs.

Exploracao finita de **271453 nos por configuracao**, profundidade5/alfabeto12, incluindo sequencias repetidas/estados equivalentes. Nao sao271453 estados unicos nem prova formal de toda sequencia possivel/Windows. Resets e copias de estados entre casos/ramos sao SOMENTE de handles falsos e nao representam fechamento nativo ou autorizacao para resetar um owner real. Oracle separado em bitmask acompanha aquisicoes falsas vivas/fechadas: cada fake handle vivo deve ter exatamente um slot persistente; nenhum slot FREE/RESERVED pode guardar handle apos retorno; UNKNOWN exige freeze; nenhum callback pode receber API nova depois do freeze. A exploracao compara imutabilidade dos valores/counters/primeira causa depois de congelar.

Quatro mutantes isolados compilados com /W4/WX foram detectados em execucao: descarte do valor no close incerto, bypass do gate de aquisicao congelada, sobrescrita do primeiro erro e borrow de par sem escopo BUSY. Cada mutante retornou1 como esperado; nao e falha dos modelos finais nem evidencia nativa de leak. [Resultados de mutacao](mutation-results.json). Nenhuma API real de aquisicao/CloseHandle e chamada pelo algoritmo de teste; processos/CRT do fixture nao constituem medicao de recursos operacionais.

## Storage medido x nao medido

No x64, em todas as quatro configuracoes:

| Parcela | Bytes |
|---|---:|
| Os dois HANDLEs |16|
| Bitfield de estados/flags (substitui count de4bytes)|4|
| Primeira causa |4|
| Primeiro erro CloseHandle |4|
| Duas geracoes escalares de64bits |16|
| Campos novos somados, antes do padding |44|
| sizeof(A1_OWNER), incluindo padding |48|
| Agregado equivalente antigo HANDLE[2]+unsigned, incluindo padding |24|
| Delta dos campos e dos agregados comparados |+24|
| Lease local, sem ownership proprio |16|
| Dois leases na confirmacao, sem otimizar lifetimes |32|
| Flag local de escopo de borrow da confirmacao (int x64/MSVC) |4|
| Shape do argumento do adapter no stack |40|

Os globals antigos separados somam20bytes declarados; seu padding/linker real nao foi medido como agregado nativo. Os48bytes sao tamanho do tipo candidato, nao heap/kernel/Private Bytes adicionais medidos. A1_OWNER e apenas o nome do estado auxiliar privado do mesmo owner existente, nao outro owner de admissao, servico, autoridade ou lifecycle. Frames, sobreposicao de lifetimes, stack reservado/commit e custo de codigo dependem da integracao/compilador: UNKNOWN. Nao somar os32+4+40 com memoria de processo como se fossem categorias disjuntas nem afirmar economia de handles estaveis. O delta deve entrar no ledger de storage na implementacao futura, sem aumentar quotas ou conceder creditos. HOST_CREDITS=NONE_GRANTED.

## Candidato e gate seguinte

O gerador make-candidate-patch.py le as fontes, altera somente strings em memoria e escreve o patch nesta pasta. Nao executa git apply, modifica fontes ou prepara lancamento positivo. O nucleo adicionado pelo patch corresponde ao modelo final testado; o bridge e os hunks dos callers sao proposta de integracao com os limites abaixo.

Pendente/inconclusivo por escopo: compilar/qualificar bridge e corpos reais junto ao owner; provar afinidade de thread e reentrancia no ponto de uso; adaptar o fixture antigo que escreve lab_aux_count e espera o defeito; adicionar dependencias explicitas das novas includes no build; reconciliar storage completo/stack/lifetimes/borrows/Stop; testar falhas em todas as entradas reais e macro/hook default ausente fora do LAB. Os wrappers reais de API ainda nao foram executados. [Contratos primarios e limites](api-contract-notes.md). Nenhum conhecimento sintetico substitui identidade/token/RAW1/SCM/freshness/autorizacao obrigatorios.

Nao existe terceiro auxiliar necessario no call graph sincrono revisado; se integracao futura demonstrar tal necessidade ou callback obrigatorio durante par ocupado, STOP antes de alterar codigo funcional, sem aumentar array. A menor proxima autorizacao e implementar/qualificar este slice focal no owner existente, preservando default EMPTY_DENY, positivo DENY e demais contratos. Nao aprovar LAB, preflight, intent ou Start a partir destes testes.

CURRENT_TASK_COMPLETED=YES
A1_RESERVATION_FEASIBILITY=VIABLE_IN_ISOLATED_MODEL_CONDITIONAL_ON_INTEGRATION
TWO_SLOT_CAPACITY=SUFFICIENT_FOR_REVIEWED_SCHEDULES_THIRD_REQUEST_DENIED
CALLER_COVERAGE=10_STATIC_AND_SYNTHETIC_SCHEDULES_NOT_REAL_BODIES
REENTRANCY=DENY_BEFORE_NESTED_API_SINGLE_CONTROL_THREAD_MODEL
FIRST_ERROR=PRESERVED_SEPARATE_FIRST_CLOSE_ERROR
RETENTION=FROZEN_REFERENCES_PRESERVED_IN_MODEL
STORAGE_COST=48_BYTES_OWNER_VS24_EQUIVALENT_AGGREGATE_PLUS24_LEASE16_REQUEST40
FUNCTIONAL_IMPLEMENTATION=NOT_AUTHORIZED_NOT_APPLIED
BUILDS=4_MODEL_PASS
WARNINGS=0
OFFLINE_TESTS=89_DIRECTED_AND271453_NODES_PER_CONFIGURATION_PASS
NATIVE_TESTS=NO_FACTUAL_WINDOWS_OWNERSHIP_PROOF
NETWORK_TESTS=NOT_RUN
START_EXECUTED=NO
STOP_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
PREFLIGHT_EXECUTED=NO
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_10149_BASELINE_FILES_CHECKED_ONLY_LIVE_DOC_APPEND
FIRST_UNRESOLVED_DIVERGENCE=FUNCTIONAL_A1_STILL_UNFIXED_INTEGRATION_NOT_AUTHORIZED
NEXT=A1_FOCUSED_IMPLEMENTATION_APPROVAL_OR_ALTERNATIVE
