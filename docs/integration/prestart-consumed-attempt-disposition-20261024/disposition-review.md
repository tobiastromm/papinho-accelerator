# Disposição de tentativa consumida antes de Start — revisão factual e normativa

## Decisão da revisão

Há um caminho atual para **registrar o encerramento das referências próprias comprovadas**, preservando intent, imagem, arm e ImagePath. Esse caminho já foi exercido pelo owner e pelo launcher na execução histórica. Não há, nos contratos atuais, autorização suficiente para transformar esse encerramento limitado em refund da reserva/cargas UNKNOWN ou em permissão para sucessora. Recomenda-se B: revisão normativa focal, ainda PROPOSED, para disposição pré-Start por categoria. Até aceitação e prova correspondente, D: STOP para nova admissão. Nenhuma disposição executável ou ACCEPTED é criada aqui.

Não confundir duas perguntas: o serviço desta tentativa teve processo ativo? A evidência sustenta que não houve chamada StartService. Todas as obrigações e custos da tentativa desapareceram? A evidência não sustenta essa conclusão. QUIESCENCE=UNPROVEN permanece intacto, sem inventar um serviço a parar ou exigir sua execução para descobrir se precisa ser parado.

## Autoridades e escopo de Model E

- [B0–B6 accepted](../../adr/DECISION-ed11-single-lab-diagnostic-native-boundary-20261023.md): B2 separa fechamento síncrono de referência própria de liberação de kernel/cache; B4.B.5 exige lifetime E exclusividade para crédito/refund e mantém cargas conhecidas retidas e a allowance integral indisponíveis até fechamento factual; B4.B.6 mantém consumo, identidade e não repetição; B4.C/D preservam incerteza, B5/B6 não autorizam nova admissão por silêncio.
- [N0–N9 accepted](../../adr/DECISION-first-native-lab-diagnostic-boundary-20261024.md): N0 fixa uma tentativa; N4 exige identidade/lifetime; N5 distingue Stop, quiescência e fechamento, preserva UNKNOWN e declara que eventual disposição separada não é fornecida por esta decisão; N6 mantém limites e NONE_GRANTED; N8 proíbe apagar/rearmar/reutilizar o intent.
- [Interpretação ED11 incremental](../../adr/DECISION-ed11-preserve-incremental-resource-envelopes-20261012.md): lifetimes retidos mantêm quotas; atribuição não observada não é zero.
- [Especificação1.0](../../protocol/pacc-c2-specification-revision-1.0.md), §11.2/11.3, especialmente linhas535/586/588: retenção terminal query-only, lifetimes separados, refund somente da capacidade correspondente ao lifetime encerrado. Esses mecanismos de C2/PST não são uma API administrativa de recuperação do LAB.
- ADR0030/31/32 mantêm factualidade de pre-close, identidade e exactly-once. [ADR0034](../../adr/ADR-0034-pacc-c2-revision3-contract-and-separated-gates.md) preserva os gates separados. ADR0033 foi lido na cópia isolada build/phase6-c2-c3-20261012/accelerator/docs/adr/ADR-0033-model-e-consumer-close-completion.md: escopo ISOLATED, não transplantado como nova norma main. PRECONDITION_RETAINED e CLOSE_UNRESOLVED_RETAINED são disposições de referências PST com contratos próprios; não atribuir esses enums automaticamente a arquivos ou a toda tentativa LAB.

Uma carga retida é obrigação de manter contabilização/capacidade indisponível enquanto um recurso, borrower, dependência ou lifetime necessário permanece não reconciliado. Não significa que uma reserva de1GiB é uma alocação física viva, nem que todo arquivo persistente mantém um handle aberto. A allowance selecionada é política operacional de risco, não memória Windows reservada. HOST_CREDITS já era NONE_GRANTED: não há crédito alocado que esta revisão possa simplesmente devolver. A proibição de reutilização da allowance é distinta do saldo atual de handles e da existência física dos objetos.

## Fatos independentes e força da evidência

| Fato | Evidência | Limite |
|---|---|---|
| Intent consumido |Recibo consumed1; registro103 bytes e ID/hash confirmados pela revisão anterior|Irreversível para replay; não é recibo de cleanup|
| StartService não chamado |Recibo starts0 e ramo real: ig_start incrementa starts ANTES de atribuir cycle_start_requested e chamar StartServiceW|Prova da execução instrumentada sob código/pins confiáveis; não auditoria global do SCM ou de outros administradores|
| StopService não chamado |Recibo stops0; ig_stop precede ControlService no ramo pós-Start|Nenhuma necessidade de mandar Stop para fabricar encerramento|
| SCM STOPPED/PID0 |Consulta anterior e consulta read-only atual,1077/specific0|Snapshot não atesta lifetime ou quiescência histórica|
| Referências próprias do qualificador fechadas |IG_FINAL close0 só depois de ig_cleanup bem-sucedido; retorno e launcher final|Recibo agregado; não extinção de kernel/cache nem de todos os processos auxiliares|
| Referências próprias do launcher |WaitForExit e Dispose/CloseHandle precedem F10/mensagem final; N0_CYCLE_EXIT=2 registrado sem CLOSE_UNKNOWN|Fechamento de seus pins/pipe/process-object/locks conhecido; não término do próprio PowerShell ou conhost|
| Imagem persistente |434688 bytes/ID0a00000024dfd7/hasheda84c...361b confirmados na revisão anterior|Arquivo preservado, não adopted/new nem mapped-image attestation|
| Sink/arm persistente |1024 bytes/ID1000000024e0ad/hash240d65...1e0 confirmado|Arm sem append final; nenhum NTSTATUS psr_prepare obtido|
| ImagePath alterado |Readback aponta para imagem instrumentada antiga|Efeito de configuração permanente sem rollback|
| Quiescência histórica |quiescent0 no recibo|UNPROVEN; não reescrever como1|
| PIDs históricos |20740/21328/488 não encontrados em consulta atual; qualifier creation e actor/aux creation no recibo|Ausência atual não é wait do handle original, não prova data/motivo/término histórico|

Recibo e consultas anteriores são evidência histórica explicitamente datada; as consultas desta tarefa estão em current-*.txt e não constituem nova admissão. Nenhum arquivo protegido foi aberto para escrita ou recriado. Não houve consulta invasiva de handles, driver, trace ou captura nova.

## Caminho real de fechamento e operações pendentes

[ig_retire](../../../tools/qualification/scm-instrumented-policy.h:59) permite retirar referências locais se não há retenção e (`!starts || quiescent`). Isto permite pré-Start sem alegar quiescência de um serviço inexistente nesta execução. Não retorna crédito de host.

[ig_cleanup](../../../tools/qualification/scm-instrumented-cycle-lab.inc:205) rejeita flags de retenção; fecha snapshots, encerra borrows/aposenta amostra B+D, fecha eventos, desbloqueia configuração, fecha aprovação/contexto N0, chama cycle_cleanup e termina o owner. [cycle_cleanup](../../../tools/qualification/scm-runtime-cycle-lab.inc:234) declara expressamente que handles podem fechar enquanto efeitos persistem; verifica retenção/aux/pending_sd, fecha referências originais e ps_close. [admission_finish](../../../apps/server/server_service_admission_win32.inc:156) exige contexto sem pending, thread/owner corretos e flag de prontidão local; fecha intent e referências, ReleaseMutex e fecha mutex. O registro persistente nunca é apagado e permanece guard contra replay.

O `finish(1)` desse ramo é precondição de fechamento das referências sob o critério pré-Start, não prova do campo IG_FINAL quiescent nem autorização para liberar cargas históricas. Não classificar essa distinção como defeito de produção ou como novo procedimento de recuperação.

Cópia usa ReadFile/WriteFile síncronos com OVERLAPPED NULL e FlushFileBuffers; consumo idem; chamadas SCM são síncronas; LockFileEx usa FAIL_IMMEDIATELY e seu unlock é conferido. A estrutura OVERLAPPED do lock não prova I/O assíncrona pendente. Cleanup verifica snapshots/pending_sd/borrows e propaga falha de close; o caminho com referência incerta não emite IG_FINAL close0 e retém. Isso sustenta que não havia pendência **conhecida sob o controle desses owners** no retorno. Não prova fim de cache/writeback/kernel transitive, operação de outro processo, thread interna do console ou todos os borrowers não observáveis do Windows. Nenhum enum terminal PST foi observado nesta tentativa sem serviço iniciado.

## Ledger de disposição

| Categoria | Lifetime/owner | Disposição permitida agora | Reserva/refund |
|---|---|---|---|
| Handles/snapshots/borrows do qualificador, contexto N0, pins e locks |Originais do owner/launcher|Registrar CLOSED conforme recibos e código; não fechar novamente|Nenhum crédito novo concedido; não somar referências fechadas como handles vivos comprovados|
| Serviço/processo da operação |Start não chamado por esta execução|Registrar NOT_CREATED_BY_THIS_ATTEMPT dentro do alcance da evidência; service quiescence global continua UNPROVEN|Não inventar custo medido de serviço, PID ou token; não usar isso para zerar allowance|
| PowerShell/auxiliar |Processos administrativos distintos de suas referências emprestadas|Término de ambos não provado por original handles; preservar evidência e UNKNOWN, sem kill|1022→1032 histórico, não saldo vivo atual nem crédito8/10handles|
| Intent |Tombstone persistente|CONSUMED_PRESERVED definitivo; manter identidade/conteúdo|Nunca rearmar/refund de autorização|
| Imagem e sink arm |Objetos de arquivo persistentes, handles próprios fechados|PRESERVED_IN_PLACE; histórico não é novo recurso da sucessora|Armazenamento/efeitos mantidos; sem zero de kernel/cache|
| SCM ImagePath |Configuração do serviço existente|REBIND_PRESERVED; consultar somente|Não restaurar/reutilizar implicitamente|
| Proveniência/dossiê protegido |Objetos estáticos originais|Preservados; não emitir autorização de outra tentativa|Não são reserva consumível de substituição|
| Custos transitivos/intervalos não observados/allowance |Política ED11 e UNKNOWN|RETAINED_UNAVAILABLE_FOR_REUSE|Nenhum refund ou extinção factual demonstrado|

Não existe no código consultado um ledger persistente de crédito multi-tentativa reconciliável nem uma API para importar/encerrar a reserva histórica após morte do owner. Estado local de PSA e mutex não é tal ledger. Não inventar saldo, valor de kernel ou transição administrativa. Quantificar apenas o conhecido; preservação UNKNOWN não afirma leak infinito nem todas as quotas fisicamente ocupadas.

## Comparação A–D e recomendação única

| Alternativa | Contrato/evidência | Decisão |
|---|---|---|
| A: disposição administrativa atual |Permite documentar fechamento factual das referências locais e efeitos persistentes; não oferece waiver/refund de UNKNOWN ou nova autoridade após owner encerrado|PARCIAL: registrar o ledger acima; reserva não liberada|
| B: revisão focal pré-Start |Pode definir alcance de encerramento administrativo pré-Start e tratamento explícito da allowance sem apagar efeitos nem declarar quiescência histórica|RECOMENDADA COMO PROPOSTA, não aceitação; exige owner e revisão oficial|
| C: coexistir com cargas antigas |B4.B.5 exige não reutilizar allowance e exclusividade; N0/B0 só uma tentativa. Não há orçamento nativo atual, quantificação do UNKNOWN ou owner qualificado para duas admissões/reservas|NOT_QUALIFIED; mais RAM ou1024handles por si não autoriza coexistência; não criar reserva paralela|
| D: STOP |Aplica-se enquanto B não aprovado/provado ou A insuficiente|ESTADO EXECUTIVO ATUAL|

**Menor revisão B a preparar para decisão do owner:** definir disposição administrativa separada para CONSUMED_BEFORE_START com prefixo de execução comprovado por recibo+pins; comprovar fechamento dos recursos próprios e ausência de pendências conhecidas; inventariar cada objeto persistente sem limpar, adotar ou rearmar; distinguir processos compartilhados e suas referências; manter cada custo realmente incerto e sua razão. Delimitar expressamente em B4.B.5/N5/N8 se a allowance única pode deixar de bloquear uma futura identidade quando não houve Start e os refs próprios fecharam, sem representar isso como liberação física/credito ou quiescência histórica. Se o owner pretender dispensar alguma prova atualmente obrigatória, o texto deve nomear a dispensa e o risco residual; não chamar essa decisão de refund factualmente comprovado. Não se pode retirar a carga de um recurso cuja existência/lifetime continua UNKNOWN por mera renomeação.

Esta tarefa entrega requisitos dessa revisão, sem publicar ADR candidate normativo concorrente ou registrar ACCEPTED. A futura revisão deve preservar a causa1816, consumed1/effect2/arm1/starts0/stops0/close0/quiescent0, tombstone, locks/fechamento exatamente uma vez, objetos protegidos/SCM, limite1024/128/quatro papéis/freshness/NONE_GRANTED. Deve separar autorização normativa, implementação se necessária e disposição factual; nenhuma cria automaticamente sucessora. Se não houver disposição explícita coerente sem violar essas condições, D permanece.

## Propriedade faltante e riscos

Primeira propriedade não demonstrada para **liberação integral sob o contrato atual**: fechamento factual de todas as obrigações de lifetime/UNKNOWN às quais B4.B.5 vincula a allowance e sua exclusividade. Fechamento dos refs locais está comprovado no alcance dos recibos; quiescência/terminação histórica completa de participantes e custos transitivos não está. Não é necessária outra investigação de conhost para autenticar relação impossível; essa tarefa não exige nem fabrica essa propriedade.

Riscos: recibo agregado não é trace completo de todos os eventos; processo não iniciado pelo qualifier não exclui ato de outro administrador; kernel/cache e console permanecem além da observabilidade; estado SCM pode mudar entre consultas; revisão da allowance seria mudança de garantia que exige decisão explícita. Preservar fatos e risco sem afirmar ataque, leak ou recuperação concluída.

CURRENT_TASK_COMPLETED=YES (revisão concluída; liberação integral NÃO demonstrada).
HISTORICAL_ATTEMPT=CONSUMED_PRESERVED
START_SERVICE_CALLED=NO
STOP_SERVICE_CALLED=NO
QUIESCENCE=UNPROVEN
RETAINED_CHARGES=KNOWN_REFERENCES_CLOSED_PERSISTENT_EFFECTS_PRESERVED_UNKNOWN_ALLOWANCE_NOT_RELEASED
MODEL_E_DISPOSITION=LOCAL_CLOSE_EVIDENCED_NO_GLOBAL_REFUND_ADMINISTRATIVE_REVISION_REQUIRED
REFUND_AUTHORIZED=NO_UNLESS_FACTUALLY_PROVEN
SUCCESSOR_ATTEMPT=NOT_CREATED
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
START_EXECUTED=NO; STOP_EXECUTED=NO; SCM_MUTATIONS=NONE; ACL_MUTATIONS=NONE; PROTECTED_WRITES=NONE
NEXT=SUCCESSOR_SCOPE_DECISION_AFTER_RETAINED_CHARGE_DISPOSITION
