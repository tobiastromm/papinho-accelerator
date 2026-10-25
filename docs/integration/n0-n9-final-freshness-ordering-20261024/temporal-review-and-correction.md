# N0–N9 — revisão temporal e correção focal

TASK=PACC-PHASE6-N0-N9-FINAL-FRESHNESS-ORDERING-1. Revisão de fonte do agente executor, não revisor externo. main/HEAD913c90cf2328f7939d8f5b927640601903ac6e91. Autoridade desta tarefa: correção e qualificação offline; nenhuma nova execução. N0–N9, RAW1, B+D, B0–B6 revisão4, Model E/ED08 e fonte da integração positiva confrontados. Nenhum ADR accepted foi modificado; direção futura de listeners revisada e permanece não implementada. Não há nova arquitetura, owner, mutex, serviço, registry, worker ou recorder.

## Evidência histórica e alcance da causa

owner-N0START-20261024-01-result.txt é cópia byte-exata do anexo do owner, com proveniência/hash em historical-result.json. Não é observação independente do executor. CONSUME_INTENT/1460, close0/exit2, consumed0/effect0/diagnostic_created0/starts0/stops0. 122handles no qualificador e742+271=1013 administrativos são amostras históricas, não nova admissão/peaks.

O IG_HOST reportou tick78387718. Na fonte então executada, esse sample.oldest era atribuído ANTES da revalidação protegida e dos hashes no início do observer. Ao final da coleta bd_metrics revalidava novamente; depois havia CONFIRM_REVALIDATION com lab_bound/dependências/censo, seguida por n0_arm com outra revalidação e teste metrics_at<=1s. Essa ordem defeituosa é comprovada pela fonte. O contador de leitura3128662684 bytes no qualificador é agregado; não permite atribuir latência por chamada nem localizar em que subfase foi feita cada leitura.

A falha precede o ig_consume local (que marcaria consumed1 no recibo): n0_arm ou sua cadeia de callbacks é o limite focal. NÃO se prova qual callback/API ou gate gerou1460. O transcript não contém tick do arm, metrics_at interno impresso ou duração das chamadas. A amostra identificável é a geração reportada com oldest78387718; sua idade exata no gate e a observação efetivamente stale continuam UNKNOWN. Não atribuir retroativamente a uma API, a um dossiê vencido ou exclusivamente à idade de métricas.

## Mapa de timestamps e gerações

| Campo | Significado e regra |
|---|---|
| context.began | Início original da aquisição do dossiê N0; nunca reiniciado pela correção;60s continua aplicado também após validação completa |
| inventory.console.observed | Observação original do tuple/prova auxiliar;60s independente, não substitui relógio da capacidade |
| context.validated_at (novo) | Conclusão de verificação completa dos conteúdos/identidades protegidos; não é métrica de capacidade nem crédito; +8bytes no layout medido |
| sample.oldest | Agora capturado imediatamente ANTES da primeira API de métricas, DEPOIS das validações completas, sem subtrair duração real da coleta |
| bd.metrics_at | Copia exatamente sample.oldest; não substitui por horário de término e não renova automaticamente |
| ig_gate.preflight | Mesma primeira geração aceita;60s em paralelo ao início original do dossiê, que pode ser mais antigo e negar antes |
| arm/consume/effect now | Query de tick depois da continuidade obrigatória; idade<=1000ms, inclusive1000;1001 nega sem refresh |
| ig_start now | Continua<=1s da geração final mais antiga e<=60s do preflight; nenhuma espera manual depois |
| ig_gate.last | Intervalo de observação em execução continua com limite1s; alvo100ms, sem promessa de observar todos os picos |

Continuam somente DUAS gerações pré-efeito: preflight antes do consumo e final depois de preparação/rebind e antes do Start. Não foi acrescentada uma terceira medição, retry ou loop de renovação. Nenhuma amostra histórica é reutilizada.

## Ordem corrigida

1. Mesmo owner/mutex exclusivo e binding protegido, operador/token/identidades originais e confirmações. Confirmação positiva e censo/revalidações potencialmente demoradas ocorrem antes da primeira coleta.
2. A cada chamada existente do observer: invalida metrics_valid e fecha a janela anterior; executa validação COMPLETA dos conteúdos, IDs, ACL/RAW1, políticas, configuração/binário/dependências/registro de aprovação/SCM. Só depois abre a janela interna e fixa sample.oldest para coletar host/processos.
3. Após a coleta e antes de arm/consume/effect, a MESMA cadeia canônica revalida token/ModifiedId/liveness/thread/identidades, plano, handles originais, volume/ancestrais, owner/group/DACL/RAW1, arquivo aberto, sessão/clientes, SCM/recovery/status e inventário. A janela evita somente repetir as leituras integrais de conteúdo já validadas e retidas durante essa geração. Não evita checagens de segurança, estado ou freshness.
4. Arm/consume/effect mantêm a comparação original1s, agora com diagnóstico explícito do gate e dos timestamps. Consumo continua uma única criação exclusiva no owner e precede efeitos. Rebind e preparação podem consumir a tentativa sem alcançar Start; os mesmos limites podem negar nesse intervalo.
5. A geração final fecha a janela anterior, exige nova validação completa e coleta novos valores antes do Start. Depois há somente continuidade/gates; coleta lenta ou API de continuidade que demore demais ainda causa DENY. Não há garantia de PASS real.

## Base da continuidade protegida

O modo completo permanece default em lab_bound e cycle_dependencies_current, e no read-only. As variantes privadas *_ex podem omitir somente os READs integrais de conteúdo na janela do N0 positivo. Identidades e descritores originais continuam consultados. O contexto só habilita a janela depois do callback completo no owner já adquirido. Stamp ausente/anterior a began/fora60s, dossiê stale, janela sem estado preparado, plano divergente, token/ID/SD diferente ou referência perdida negam. Não existe parâmetro argv/hash ou booleano externo que substitua a prova.

Os objetos originais ficam abertos sem compartilhamento de escrita/exclusão; não há reabertura/adopção pelo nome. O conteúdo é comparado aos pins na validação completa e sua continuidade é suportada pelas referências retidas/proteção de acesso, não por presumir que PID/nome garante integridade. ACL/RAW1 e identidades são verificados na continuidade. Ameaça de administrador equivalente/Windows comprometido permanece fora da garantia aceita; mapped-image provenance e associação autenticada do console continuam UNPROVEN/N7. Nenhuma nova propriedade de autenticação ou limite universal é afirmada. Mudança de política de proteção seria outra decisão, não parte desta correção.

## Diagnóstico, recursos e qualificação

O mesmo stdout/diagnóstico existente agora emite IG_FRESHNESS quando o gate de métricas nega: ARM/CONSUME/EFFECT, now, metrics_at, valid/age_known, age_ms, dossier_began e validated_at. A primeira falha é preservada; métricas ausentes/clock regressivo são marcadas com age_known0, não convertidas em observação0 válida. Não é recorder/logger adicional; não há novo arquivo operacional ou captura. O histórico não é reescrito com esses campos futuros.

PS_BD_CONTEXT_STORAGE1576 vs1568, BD_STATE3128 vs3120, admission3216 vs3208 nos modelos: +8bytes de estado fixo, sem handles, buffers grandes ou threads novos. Nenhuma quota/128handles/orçamento administrativo/headroom/reserva/credits foi aumentada. Stack/transitive/kernel/cache permanecem UNKNOWN; redução de leitura é estrutural, não economia nativa medida nem crédito.

Quatro novos builds Selected/Legacy Debug/Release /W4 /WX zero warnings.36 execuções de nove suítes PASS: BD83 (inclui relógio injetado, validação anterior>1s/amostra nova,1000/1001, token/file-ID alterados, falha de escrita do intent após consumo, replay/Stop/retention/close incerto), busy92 (inclui fase de conteúdo completo/continuidade, stamp ausente, stale60s/clock regressivo e pending-window), SD13, policy34, sourcegate6, runtime/closed/file e ciclo. Falha após consumo usa CreateFile/WriteFile mocks, sem arquivo ou serviço real; referências falsas são explicitamente fixtures. Casos com relógio controlado NÃO provam duração das APIs no host.

16 invocações reais do binário usam somente argc2 rejeitado antes de aquisição. Nenhum --prepare-n0/--cycle-n0 completo foi executado. Launcher AST/Review, hash divergente, ausência de aprovação e loop de saída real com StringReader PASS; nenhuma admissão positiva. Não houve novo preflight, Start/Stop, SCM/ACL, ETW, CA/cert/bootstrap/CURRENT ou rede. Modelos positivos não qualificam o cenário nativo.

## Disposição e próxima operação

INTENT_DISPOSITION=NOT_CONSUMED_HISTORICAL. Não existe prova atual independente de ausência do claim nem autorização reutilizável. N0START-01 terminou com falha, portanto NÃO repetir o comando nem reaproveitar seu registro de aprovação. A nova operação candidata N0START-20261024-02 continua usando a tentativa normativa exata75330d11cc7e4123ae62eadc9a59c2f5 SOMENTE se o owner aprovar especificamente essa elegibilidade e os gates canônicos frescos comprovarem ausência do intent nos originais sob mutex. Nenhum rearm/delete/overwrite/disposição de recursos por inferência. Se existir claim ou qualquer divergência, DENY.

Script e binário apontam para um NOVO caminho de registro EXACT-OPERATION-APPROVED.json neste pacote. Ele NÃO foi criado. O registro anterior fica byte-exato e não será lido pelo sucessor. Alteração dos hashes/identidade da operação invalida aprovação anterior. [Solicitação](operation-request.json) e [handoff candidato](manual-handoff.md) exigem nova aprovação manual específica; execução automática permanece DENY.
