# N0START03 — viabilidade positiva não demonstrada; correção/fixture focal antes de pacote positivo

## Conclusão

Preflight read-only PASS NA OBSERVAÇÃO: admin1002/1024 (731PowerShell+271auxiliar), qualificador126/128, first0/close0/intent0/effect0/starts0/stops0/exit0. Não são admissão fresca, teto ou reserva. Global quiescence continua não demonstrada (quiescent0); fechamento local foi factual. O recibo original desta operação é a saída de console fornecida pelo owner; o launcher não grava arquivo de resultado read-only. A transcrição estruturada neste pacote é evidência documental derivada, NÃO novo recibo nativo de execução ou colocação.

Primeiro bloqueio técnico: não está demonstrada capacidade do QUALIFICADOR para manter o conjunto de referências do ciclo positivo sob128. O código adiciona pelo menos intent+imagem até a amostra final, mais processo de serviço após Start. Mantida a base126, os saldos condicionados são128 antes de Start,129 no primeiro monitoramento e132 após cycle_live bem-sucedido. Não são observações positivas nem picos oficiais; diferenças entre builds, lifetimes/OS/CRT podem alterar a base. Não executar LAB para descobrir um cruzamento já previsto condicionalmente.

Além disso, os artefatos atuais foram deliberadamente compilados read-only; launcher/CMake/entrada positiva permanecem DENY. Não reabilitar Cycle nesta revisão. Não foi produzido pacote operacional positivo/registro de autorização/novo intent ou efeitos. A etapa seguinte mínima é qualificação isolada da pressão e revisão focal de lifetimes, não Start.

## Mapa de operações e ownership

| Operação/fonte | Referência própria do qualificador | Lifetime/overlap | Classificação |
|---|---|---|---|
| Confirmação positiva, scm-instrumented-cycle-lab.inc:169 | in/out de CONIN/CONOUT, dois handles | Ambos fechados por lab_close_aux antes de preflightmetrics | Transitórios necessários, não +2 persistentes na amostra final |
| Ausência pré-consumo, mesma fonte:35 | cycle_file na consulta relativa ao pai original | Sem handle em ausência; colisão fecha referência ou retém UNKNOWN; consulta não reserva nome | Sem crédito; transient/failure retained |
| Arm/revalidação | Handles originais de contexto/processos/inputs/mutex já presentes | Não duplicar aliases nem soltar provas obrigatórias | Base existente, lifetimes obrigatórios |
| Consume, server_service_admission_win32.inc:128–144 | PSA.intent, CREATE_NEW/write/flush | Permanece obrigatório para admission_effect; fechado apenas finish factual, linhas179–181 | +1 próprio persistente, não dispensável só para passar |
| Cópia, scm-runtime-cycle-lab.inc:129–146 | cycle_file destino; cycle_source_file já aberto no preflight | Criação exclusiva, hash/readback; fecha handle de escrita e reabre read-only SEQUENCIALMENTE, mantendo cycle_file até cleanup | +1 persistente líquido, NÃO contar escrita+reopen como2simultâneos |
| Sink/arm, scm-instrumented-cycle-lab.inc:188 e psr-file-capture-lab.inc:53–68 | ig_diag.handle | psrf_create escreve/flush/verifica e fecha no sucesso antes do rebind/final sample; em falha retém | +1 transitório, não persistente no caminho limpo |
| Rebind/SCMreadbacks | SC_MANAGER/SERVICE já abertos; ChangeServiceConfig/readback | Mesmos objetos SCM; direitos positivos mudam, não se infere quantidade kernel a partir de SC_HANDLE | Sem novo handle explícito nomeado nessas chamadas; RPC/SCM interno UNKNOWN |
| Censo final, scm-instrumented-cycle-lab.inc:139–161 | ig_guard_snapshot | Snapshot explícito fechado antes do final ig_observe; close falho retém | +1 transitório, não quitar guardas |
| Métricas/PSS, scm-instrumented-observer.inc:23–61 | snapshot/marker/thread | Cada medição exige ausência de snapshot anterior; cleanup checado antes de próximo participante. GetProcessHandleCount precede o PSS do participante | Custo real interno/temporário UNKNOWN; HPSS/marker não somados ingenuamente como kernel handles |
| StartService, ciclo:410–417 | Nenhum novo HANDLE explícito retornado por StartServiceW | Cria processo real do serviço; biblioteca/SCM/OS custos desconhecidos | Não zero; serviço tem tabela/limite próprio, não somar seus handles aos126 do qualificador |
| Bindserviço, ciclo:253–265 | cycle_process=OpenProcess | Handle próprio retido até cleanup; mesmo valor emprestado a observer/inventory | +1 persistente; borrow não adiciona segunda entrada de tabela |
| Validação RUNNING, runtime:188–220 | cycle_token e cycle_access (DuplicateToken) | São mantidos depois do retorno; cleanup236 fecha | +2 persistentes atualmente; uso posterior deve ser auditado antes de encurtar lifetime |
| Enumeração de threads, runtime:221–231 | cycle_threads snapshot + cycle_thread por thread | Cada cycle_thread fecha; snapshot threads não fecha na saída de sucesso, só cleanup236 | +1 persistente atualmente; snapshot de módulos anterior é fechado204 e não acumula com ele |
| Stop seguro | Usa native.service existente e estados legais | solicitado/aceito/concluído/quiescência/close distintos; sem kill e sem retry | APIs/OS unknown, referências obrigatórias mantidas |
| Extração diagnóstico, ciclo:199–209 | Handle local da leitura do sink | Efetuada após término/erro, por identidade; fechamento checado | +1 temporário; não causa amostra pré-Start |

SDs de ConvertStringSecurityDescriptor/GetSecurityInfo são buffers e LocalFree, não handles de tabela somados à quota. Handle lógico de SCM/HPSS não implica custo kernel fixo. Processos administrativos e serviço têm budgets distintos; alias/borrow preservados sem double count.

## Ordem temporal que torna a margem relevante

O preflight positivo executa confirmação/ausência/validações completas ANTES da primeira amostra; depois consume, copy, sink, rebind e readbacks, censo, finalsample, Start. Assim, intent+cycle_file são simultâneos à amostra final por contrato e código; nenhum fechamento entre essas etapas elimina ambos.

Após Start, o loop liga cycle_process e chama censo/ig_observe/ig_monitor ANTES de cycle_live. Condicionalmente126+1intent+1image+1process=129, portanto pode negar no primeiro monitoramento, antes de obter a validação RUNNING desejada. Só quando ciclo/estados permitirem vem cycle_live. Seu sucesso conserva token+access+threadsnapshot; total de seis referências adicionais sobre a base,132condicionado, sem contar picos de API. Como guarda é amostral, transientes podem existir antes da próxima amostra sem terem sido medidos; não afirmar que passou o ciclo porque uma leitura final é128.

Os tokens/cycle_threads parecem candidatos a fechamento local APÓS sua última operação de verificação, mas esta revisão não autoriza patch ou supressão da prova. O snapshot não fechado no retorno é referência conhecida retida pelo código, NÃO leak comprovado: cleanup tem fechamento e lifetimes em falhas devem ser preservados. Mesmo que se encurtem esses três, intent/imagem/processo somam3 e a base126 ainda daria129. Portanto fechar apenas tokens/snapshot não demonstra solução suficiente.

Intent é necessário a admission_effect. A imagem é usada em cycle_files/AccessCheck/identidade e segue retida. Handle do serviço prova identidade/lifetime/quiescência/Stop. Não propor fechar qualquer um só para fazer quota PASS; exigir demonstração de segurança de eventual transformação.

## Administração e outros recursos

PowerShell731+auxiliar271=1002, margem22. Encaminhamento stdout/reader/processo supervisionado/pins/cadeia/helper já existiam na coleta. Nenhum custo de handle do qualificador deve ser somado a1002. Abertura CONIN/OUT no qualificador pertence à sua tabela; efeitos indiretos no auxiliar/console e InformationStream/CLR não foram quantificados nesta execução. Saída positiva mais longa, supervisão, coleta/Stop e comportamento PSS podem afetar processos/OS; ausência de criação explícita no launcher não garante delta0. Não atribuir economia ou22handles ao patch de console: comparação controlada no LAB não existe.

PSS é custo de observação obrigatório atualmente; GetProcessHandleCount é lido antes da captura PSS desse participante. Para o próprio qualificador, seus handles internos criados depois da leitura não estão necessariamente naquela126. Snapshots de outros participantes também possuem custo no coletor; não presumir que são criação na tabela do alvo. Sem novos traces ou recorder, a menor medição isolada deve mapear isso na tabela real do supervisor.

Host físico6304161792 e commitavailable15185559552 passaram5/9GiB naquela coleta, não agora. Native memory/thread/stack observados passaram guards; serviço real/custos positivos/transitivos/quotas independentes futuros continuam UNKNOWN. H0–H9 dispensa teto retrospectivo universal SOMENTE resíduo opaco histórico; não dispensa esse conjunto conhecido de referências controladas novas. P3/histórico/intentantigo/imagem/sink/ImagePath/UNKNOWN/reservas reais indisponíveis permanecem. Nenhum crédito ou refund.

## Menor próximo passo proposto, sem execução

Uma fixture isolada do MESMO owner/ciclo com APIs de efeitos substituídas (como testes existentes) deve registrar aquisições/fechamentos/saldos por fase, preservar borrows/firstfailure/Stop/retention e não tocar SCM/LAB/intent/imagem/sink. Incluir tabela por owner, geração antes/depois PSS e simulação de base126; projetar consumo/imagem/processo/token/access/snapshots sem Start real. Se necessária medição de handles reais, usar duplicatas/fixtures sem efeitos apenas em experimento autorizado, ferramentas oficiais já disponíveis; sem recorder dentro do produto e sem trace LAB novo.

A qualificação deve demonstrar consumo simultâneo sob128 também depois do Start simulado/Stop/erro, sem fechar provas obrigatórias. Medir/admitir antes de consumir com capacidade para efeitos subsequentes; não conceder128 no limite e ignorar novas refs conhecidas. Se uma correção de lifetimes for comprovada suficiente, solicitar autorização de patch focal+offline antes de nova preparação positiva; este trabalho não modifica fonte nem quotas. Não basta reprovar após intent consumido. Se não há redução segura suficiente, manter DENY e apresentar custo mínimo/decisão necessária, sem aumentar128 automaticamente.

Critérios DENY: margem insuficiente para referências conhecidas, PSSobrigatório não qualificado, processo adicional, métrica obrigatória desconhecida, lifetime controlado incerto, identidade/input/token/boot/política divergente, freshness60s1s expirada, headroom insuficiente, obrigação incompatível. Preservar1024admin/128nativo/quatro papéis/NONE_GRANTED/exclusividade/Stop/UNKNOWN/exactly-once/N7 específico. Não reabrir associação de conhost nem exigir teto universal H2 como condição nova.

CURRENT_TASK_COMPLETED=YES
READONLY_PREFLIGHT=PASS_HISTORICAL
POSITIVE_CYCLE_VIABILITY=NOT_DEMONSTRATED_CONTROLLED_HANDLE_OVERLAP_BLOCKER
ADMIN_HEADROOM=22_OBSERVED
QUALIFIER_HEADROOM=2_OBSERVED
ADDITIONAL_HANDLE_COSTS=2_PRESTART_3_POSTSTART_6_POSTLIVE_PERSISTENT_NAMED_REFERENCES_CONDITIONAL_BASE
FIRST_UNRESOLVED_DIVERGENCE=QUALIFIER_HAS_NO_DEMONSTRATED_CAPACITY_FOR_MANDATORY_POSITIVE_REFERENCES_AND_OBSERVER_OVERHEAD
INTENT_CONSUMED=NO
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_NO_FUNCTIONAL_SOURCE_CHANGES
NEXT=N0START03_POSITIVE_OPERATION_PREPARATION_OR_TARGETED_RESOURCE_CORRECTION
