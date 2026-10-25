# Resolução de capacidade positiva — evidência, limites e bloqueio factual

## Estado e escopo

Branch main, HEAD913c90cf2328f7939d8f5b927640601903ac6e91 preservado. Nenhum fonte funcional, launcher, pin ou artefato protegido foi modificado. Nenhum Prepare, SCM, Start/Stop, intent, imagem ou sink operacional foi executado/criado. A autonomia técnica do documento foi adotada pela instrução posterior do owner para executar documentos de tarefa enviados. Isso não concede autoridade operacional.

A revisão não demonstrou impossibilidade universal sob os contratos. Também não demonstrou capacidade positiva. Portanto o resultado correto é BLOCKED por propriedade factual ainda ausente, não uma decisão arquitetural inventada ou um PASS offline fictício.

## Lifetime e projeção confrontados com fonte

lifetime-and-overlap-review.csv revisa as28 categorias originais e distingue aquisição original/duplicada, borrow, último uso, fechamento e UNKNOWN. Não somar categorias sobrepostas nem tabelas de processos distintos.

scm-instrumented-cycle-lab.inc fecha lab_token somente positive&&n0 após lab_operator/ig_boot e antes de PROTECTED_BINDING; Prepare conserva o handle. ig_n0_bind já fecha actor_token/control/actor/auxiliary/osfile após handoff bem-sucedido ao ownerBD. Não há cinco referências esquecidas a fechar. As confirmações e o censo também fecham seus temporários antes da janela seguinte.

ig_n0_current revalida roots[5],dossiê,evidência,policies[4],aprovação,configuração,imagem fonte e dependências; o content-phase limita conteúdo sem remover identidade/RAW1. Fechar/reabrir os originais quebraria proteção de substituição/lifetime e exigiria nova prova; cache de hash não substitui essa identidade. Os tokens-base actor/auxiliary e cópiasBD pertencem ao owner e não podem ser descartados como se fossem seus check-tokens temporários. B1 já removeu duplicação do callback dentro do ramoN0; B2 não prova redução de pico e altera observações de segurança.

Na sequência positiva, papacc_server_service_admission_consume adquire o intent; cycle_copy adquire cycle_file e troca writer por reader após fechamento comprovado, sem coexistência dupla. Ambos permanecem usados por efeitos/revalidações/fechamento. O sink preparador fecha antes da amostra final; arquivo persistente não é handle vivo por definição. O rebind mantém os SC_HANDLE existentes, sem equivalência automática com dois kernel handles; RPC/transitivos continuam UNKNOWN.

Somente depois da cópia/sink/rebind ocorre ig_observe final antes de Start. Na observação do serviço, cycle_process é aberto e emprestado ao mesmo inventory/observer, sem cobrança duplicada. PSS snapshot e thread original coexistem durante identidade/liveness/VirtualQueryEx; já fecham antes do participante seguinte. Retirar a thread antecipadamente perde a prova contemporânea. Liberar o snapshot exigiria copiar e revalidar outro formato de evidência e não é um fechamento de último uso demonstrado no código atual.

Projeção condicional validada:126(read-only)−1(self-token)+1(intent)+1(imagem)+2(sobrecusto amostradoPSS/thread)=129 pré-Start; +1(process)=130 primeiro monitoramento. São projeções de referências sobre um contexto histórico, não picos nativos positivos medidos, garantia de custo Windows ou prova de impossibilidade. O ramo positivo também executa provas/SCM diferentes e pode ter custos internos distintos.

C1 encurtaria após-retorno de cycle_live os lifetimes de token/access/thread-snapshot; não reduz o pico dentro dessa função nem resolve pré-Start. Fechar referências obrigatórias, transferir custos ao administrador ou omitir observação não são soluções. Não implementado.

## Experimento novo e discriminante: herança do filho

O teste reutiliza o executável service_observer_stack_va_isolated_test já qualificado, chamando somente --isolated-observer-self clean. O processo PowerShell do fixture reproduz UseShellExecute=false,CreateNoWindow=false,RedirectStandardOutput=true e stdin/stderr não redirecionados. Não chama o launcher nem o main operacional. Cria exclusivamente oito eventos anônimos sintéticos com CreateEventW; SetHandleInformation só altera esses eventos do teste. Nenhum handle do operador/LAB teve flags alteradas. O filho termina normalmente; stdout/Process e eventos próprios são fechados com retorno conferido. Não há driver, ETW, serviço ou owner adicional.

| Configuração existente | Baseline |8 eventos não herdáveis|8 eventos herdáveis|Baseline posterior|
|---|---:|---:|---:|---:|
| Selected Debug |58|58|66|58|
| Selected Release |42|42|50|42|

Oito execuções nativas esperadas passaram, exit0 e contagem final igual à inicial em cada filho. Picos amostrados foram baseline+2 em cada variante. O flag herdável dos mesmos oito objetos é a variável controlada; o acréscimo de oito demonstra a propagação no mecanismo testado. Logs brutos, script e checks permanecem neste subdiretório do pacote canônico. A primeira apresentação Format-Table de hashtables saiu vazia; os JSON/logs estavam completos e foram conferidos, e somente a apresentação do script foi corrigida, sem repetir os testes.

Isso NÃO atribui oito,80 ou qualquer parcela dos126 históricos à herança. Não sabemos quais handles do PowerShell real estavam herdáveis, quais chegaram ao filho ou quais custos surgiram em SCM/RPC/inicialização. O mecanismo demonstrado abre uma hipótese distinta das anteriores, mas não autoriza fechar handles desconhecidos, alterar suas flags globais no PowerShell ou lançar outro processo de autoridade.

## Candidatos e condição de decisão

1. Fechar temporários de binding/console/censo: já ocorre, sem redução nova.
2. Fechar fonte/políticas/roots/intent/imagem: último uso ainda ocorre; rejeitado sem nova arquitetura de prova e retenção.
3. Reordenar/remover PSS/thread/RAW1: não demonstrado equivalente; rejeitado como patch focal.
4. C1 após cycle_live: redução posterior candidata, não solução deste gate.
5. Restringir herança na criação do filho: mecanismo testado demonstra benefício quando há referências não necessárias herdáveis. Aplicabilidade/custo no LAB ainda não atribuído. Não aplicar silenciosamente troca de Process.Start, native STARTUPINFOEX ou alteração de flags; exigiria prova de stdio/console/processo original/Stop/fechamento e do conjunto real a excluir. Uma nova representação de lançamento só é justificável se a atribuição factual identificar essa causa. Não criar ADR accepted por inferência.

Nenhuma correção funcional melhor foi demonstrada com a evidência atual. Isso não é prova de impossibilidade; não há base para declarar uma alteração arquitetural inevitável. Alterar o launcher agora apenas apostaria na hipótese de herança.

## Próxima observação mínima e caminho crítico

Primeiro dado ausente: decomposição da tabela do qualificador real entre referências herdadas no nascimento, referências controladas adquiridas e handles internos de inicialização/SCM. A observação read-only anterior não registrou essa decomposição e sua aprovação está consumida. Não repetir Prepare para obter o mesmo número.

O próximo gate concreto é aprovar somente um desenho de atribuição nativa discriminante antes de consumo: tabela do filho no início e após binding/SCM, correlacionada com referências explicitamente passadas pelo supervisor e suas flags, preservando os originais e a identidade PID/criação. O mecanismo de observação deve ter suporte e overhead qualificados; nenhum censo por nome ou reutilização numérica de handle é identidade de objeto. Esta tarefa não fabrica prova de objeto nem instala ferramenta dentro do qualificador para substituir autoridade. Um pedido executável não pode ter hash/resultado futuro inventado.

Após atribuição, selecionar redução por herança restrita se houver referências realmente dispensáveis; caso contrário atacar somente a primeira aquisição/lifetime responsável demonstrada. Qualificar com stdout/confirmacão/Stop/UNKNOWN/fechamento e falhas usando o harness existente. Só depois preparar autorização positiva separada, com nova admissão fresca e capacidade128/1024. B4.C/NTSTATUS/serviço/Stop/quiescência seguem como provas da futura tentativa autorizada; não podem ser provados por read-only ou impostos circularmente antes do serviço existir.

## Evidências preservadas e estados

Builds Debug/Release zero warnings e14/30/2 testes do pacote anterior foram reaproveitados; não houve fonte nativa alterada que justificasse rebuild ou regressões equivalentes. Este teste novo executou os dois binários isolados existentes, oito casosPASS. Qualificador635eaa3b55be85f7c21ac3f38084c304bcd527d39a0e6ad39fda3bf3d68283f5 e launcher690ee3b3d2346902765259e533ac304bf2101a513d7a3bf7618ebc7d3dad43e3 permanecem iguais. Histórico, aprovação de uso único, H/N7/P3, reservas e UNKNOWN não foram alterados. HEAD/WIP preservados no alcance dos hashes verificados.

CURRENT_TASK_COMPLETED=NO
POSITIVE_CAPACITY_RESOLUTION=BLOCKED
LIFETIME_TABLE=lifetime-and-overlap-review.csv
PRESTART_SIMULTANEOUS_CAPACITY=NOT_DEMONSTRATED_CONDITIONAL129
FIRST_MONITOR_CAPACITY=NOT_DEMONSTRATED_CONDITIONAL130
UNSAMPLED_PEAK=UNKNOWN
SOURCE_CHANGES=NONE_FUNCTIONAL_ISOLATED_SCRIPT_ONLY
BUILD_DEBUG_RELEASE=REUSED_EXISTING_QUALIFIED_BINARIES_NO_NATIVE_SOURCE_CHANGE
REGRESSIONS=EXISTING_RESULTS_PRESERVED_NEW8_NATIVE_EXPECTED_OUTCOMES_PASS
NATIVE_POSITIVE_EXECUTION=NOT_AUTHORIZED_NOT_RUN
ED11_TOTAL_STATUS=PARTIAL
PHASE6_TOTAL_STATUS=PARTIAL
FIRST_UNRESOLVED_GATE=ACTUAL_STARTUP_INHERITED_VERSUS_CONTROLLED_AND_INTERNAL_HANDLES_ATTRIBUTION
NEXT=DISCRIMINATING_NATIVE_STARTUP_ATTRIBUTION_DESIGN_NO_PREPARE_REPLAY
