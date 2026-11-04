# Console herdado: reconciliação factual e decisão de binding

Data2026-10-28. Continuação autorizada do TASK PACC-PHASE6-ED11-REAL-CONSOLE-ROLE-BINDING. [Transcrição exata recebida](OWNER-TRANSCRIPT.txt), [JSON do proprietário](OWNER-MANUAL-OUTPUT.json), [revisão DATA](CONTEXT-REVIEW.json), [ledger por contexto](CURRENT-CONTEXT-LEDGER.csv).

## Resultado

A coleta nativa terminou first=NONE/erro0, pico próprio79, referências originais finais0/retainedfalse/close_error0. O executável impresso438735bb... corresponde byte a byte ao qualificado. Nenhum TPM/Q4 foi executado. O modo --observe-inherited NÃO chama FreeConsole/AttachConsole; attached=false/error0 e os campos detach default não representam nova falha de associação.

A evidência agora contém a lista de clientes e auxiliares do console em que a coleta foi executada. Esse console é DIFERENTE do seed17204 fornecido no comando. first=NONE significa coleta sem falha controlada, NÃO binding/admissão PASS. A saída já traz unique_seed_and_class49_candidate_sum_known=false; respeitamos esse resultado.

## Identidades e contextos separados

| Papel corrente observado | PID | Criação FILETIME | Handles |
|---|---:|---:|---:|
| Observador nativo |20952|134376368586345654|79|
| Pai real do observador / PowerShell / HWND owner |20976|134376368499650721|712|
| Auxiliar conhost do console atual |13944|134376368499761309|270|
| Seed do comando, outro PowerShell |17204|134376272948693901|618|
| Auxiliar do seed antigo |20144|134376272949369467|266|
| Explorer/ancestral desktop observado |7200|134375241536646020|3862|

O handle original do observador informa parent_metadata20976. Os clientes são exatamente[20952,20976], count2. Os três processos correntes estão vivos/estáveis nas observações, sessão1, processo com TokenUser...1108 e elevado. O token da thread, LUID continuity e ausência de impersonation NÃO foram coletados; esses gates continuam obrigatórios no host futuro, não inferidos daqui.

Observer e pai20976 retornam class49 raw13945/0x3679, NTSTATUS0/8bytes, valores repetidos iguais. Candidate13944 deriva do perfil forward flag1 e tem handle/criação/token/sessão/arquivo/metrics próprios, distintos de pai e observador. Seu raw20978/0x51f2 é backlink com flag2 para20976; não é novo host. Arquivo conhost volume417614100/FileID327680:1591236/hashb02ee54f... observado; igualdade desse arquivo compartilhado entre duas instâncias NÃO une os processos20144 e13944. O mesmo vale para os dois PowerShell com mesmo hash e criações diferentes. Proveniência mapeada/autenticação completa da associação permanecem UNPROVEN.

O seed17204 continua ligado ao seu host20144 e não pertence à lista de clientes do console atual. Sua presença no comando não o transforma no operador/pai atual. Não atualizar silenciosamente um pin operacional de17204 para20976: estes são fatos de diagnóstico para futura decisão e binding explícitos. Expectativa de pai17204 sob esse recibo deve NEGAR por mudança de contexto; o reviewer offline confirma isso.

Não afirmamos por que o console mudou nem atribuímos isso a uma ação específica do proprietário. O recibo demonstra objetivamente os dois contextos e suas identidades. Não há necessidade de outra coleta equivalente para identificar os participantes desta saída.

## Recursos: soma correta e limites

O subtotal do par administrativo CANDIDATO atual é712+270=982handles,42 abaixo de1024 naquele instante. O nativo79 é observado SEPARADAMENTE sob128/guard124; não o transferimos para o orçamento administrativo. PrivateBytes atuais do par=70803456bytes e WorkingSet=105115648bytes, somas de amostras por processo com lifetime sobreposto, não picos atômicos/universais. Threads/stack/kernel/transitivos e recursos futuros TBS não foram provados.

O campo1154 é a soma exploratória ligada ao seed618+antigo auxiliar266+novo auxiliar270; exclui o pai atual712 e mistura contextos. Foi reportado como NÃO QUALIFICADO pelo próprio coletor. Não é o orçamento administrativo atual, não prova estouro de1024 e não deve ser corrigido subtraindo custos arbitrariamente. Tampouco somamos os dois pares884+982 como se fossem uma única operação.

Os processos do console antigo e Explorer foram observados explicitamente, não apagados ou tratados como zero. São externos ao conjunto de clientes/pai/helper deste console; qualquer classificação futura de custo realmente relacionado à operação deve ser preservada pela política e pelo censo fresco. Este reviewer DATA não concede exclusão geral de processos, crédito ou capacidade. Toda observação é histórica; não vira orçamento fresco para uma futura prova.

## Trabalho técnico realizado sem novos efeitos

Nenhum binário, fonte nativa, launcher, aprovação, pedido consumido ou pin operacional foi alterado. Não foi necessário rebuild. O coletor já expôs a divergência por identidades e marcou o subtotal como inválido. Não há defeito funcional demonstrado que justifique relaxar o guard TPM.

Implementado [reviewer offline](review_context.py), que deriva o pai pelo próprio nó do observador, confronta lista exata de clientes, originais/criação/lifetime, consultas forward repetidas, SID/sessão/elevated de processo e evidência por arquivo. O subtotal corrente é calculado por identidades únicas, ignorando rótulos de papel que não definem o contexto. Isso é análise de DATA relatada pelo proprietário, não autenticação ou novo owner/autoridade/caminho de admissão; saída sempre admission=DENY.

[Testes](OFFLINE-REVIEW-TESTS.log):9 métodos PASS, incluindo matriz de20 negativos (28cenários ao expandi-la). Cobrem pai esperado antigo/criação divergente, duplicatePID, cliente inesperado/duplicado, referências/lifetime/consulta/width/mudança de associação, token/sessão/evidência ausentes,1025admin,125reserve e retenção. Caso real982/79 passa somente como contexto diagnóstico coerente; expectativa17204 nega. Nenhuma consulta nativa ou TPM foi repetida; regressões nativas anteriores foram reutilizadas.

## Decisão normativa realmente pendente

O requisito antigo HWND PID distinto continua negando, pois o HWND é do pai atual20976. O auxiliar REAL existe,13944. A mudança necessária continua ser o locator/binding explícito, não criar processo artificial nem aumentar quotas. A lista/clientes/contexto elevado, antes pendente, agora foi OBSERVADA neste alcance diagnóstico. Autenticação completa e mapped image seguem UNPROVEN; proteção/file provenance e demais requisitos obrigatórios de admissão não são substituídos por este JSON.

A [proposta C0–C9](../../../adr/ADR-CANDIDATE-tpm-proof-explicit-console-host-binding-20261027.md) permanece PROPOSED, original imutável SHA256 `7dee709121cc9b56633efb6d968bd0935b81bc6c18b000226ed35cf0a179f985`. Seu texto de contexto/pending anterior fica preservado como histórico; esta reconciliação resolve apenas a pendência factual de leitura ali citada, não altera os termos normativos. C4 depende de aceitação explícita do risco residual estreito; histórico N7 não é reutilizado. C7 proíbe integração funcional antes dessa aceitação.

Próxima decisão: aceitar ou rejeitar C0–C9 exata para o binding da prova matemática standalone. Se aceita, registrar primeiro normativamente, integrar e qualificar tecnicamente no host canônico, medir custos e atualizar hashes prospectivos; somente depois preparar novo pedido TPM e solicitar autorização operacional própria. Não emitir agora outro comando de coleta nem pedido TPM prematuro. Se o risco residual não for aceito, manter DENY até vínculo autenticado mais forte.

## ED11 e preservação

E21=PENDING; posse TPM não demonstrada. Revogação/custódia/NV/ledger/Q4 continuam independentes. E17/E18 e E19/E20 não foram promovidos; E22 depende deles.4demonstrados/12limitados/6pendentes. ED11/Phase6 PARTIAL.

Aprovação TPM4a5f permanece usada/não reutilizável. A coleta read-only não consumiu nem restaurou essa autorização. HEAD/main/WIP/históricos/UNKNOWN/reservas preservados; sem TPM, Q4, NV, Start/Stop, SCM/ACL, limpeza/refund, CURRENT, assinatura ou recuperação autoritativa.

READONLY_RESULT=COMPLETE_CONTROLLED_COLLECTION_WITH_CROSS_CONSOLE_SEED_DETECTED
CURRENT_CONTEXT=20952_NATIVE_20976_PARENT_13944_AUXILIARY
CURRENT_ADMIN_HANDLES=982_OBSERVED_NOT_GRANTED
NATIVE_PEAK=79_OBSERVED
MIXED_SEED_SUBTOTAL=1154_NOT_A_VALID_ADMIN_BUDGET
CONTROLLED_REFS=0
RETENTION=false
C0_C9=PROPOSED_PENDING_EXPLICIT_OWNER_ACCEPTANCE
NEW_TPM_REQUEST=NOT_READY_NOT_CREATED
NEXT=C0_C9_NORMATIVE_DECISION_THEN_TECHNICAL_INTEGRATION
