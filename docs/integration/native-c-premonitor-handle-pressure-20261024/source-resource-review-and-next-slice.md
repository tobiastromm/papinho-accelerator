# Pressão de handles antes do primeiro monitoramento

Foi identificada uma candidata de redução de UMA referência estável antes do monitoramento: o token próprio do qualificador em `lab_token`, após a validação inicial bem-sucedida. O patch está preparado e NÃO APLICADO, restrito ao ramo positivo N0. B2 não foi qualificada: a consolidação proposta reduz chamadas, mas mantém o mesmo pico transitório e tem contraexemplo de segurança. O orçamento operacional permanece DENY.

## Estado e fontes

main / HEAD `913c90cf2328f7939d8f5b927640601903ac6e91`. Baseline de10.510 arquivos comparada integralmente, sem mudança ou ausência antes do append da live doc. Preimagens de header, owner BD, contexto, ciclo/registration e documentação foram preservadas. Nenhum código funcional, launcher, pin, accepted ADR, objeto LAB ou evidência anterior mudou.

Autoridades e evidências consideradas: Model E, ED11 B0–B6 revisão4, N0–N9 (especialmente N4/N5), L0–L9, H0–H9, auditoria A–J, A1 funcional, B1, revisão N0START03 e experimento nativo anterior. A auditoria anterior descreve retenção até cleanup e recomendava não antecipar tokens sem prova; sua contagem é estado factual histórico, não proibição normativa de fechar uma referência após último uso demonstrado. B1 permanece uma validação protegida por current N0; A1 permanece exatamente dois slots, sem nova autoridade. A direção futura C2 não é alterada.

126/128 é observação histórica; +3 intent/imagem/processo é simultaneidade conhecida. 129 continua uma projeção CONDICIONAL, nunca pico positivo medido. Não há admissão fresca, novo orçamento ou geração de tentativa.

## A — referência própria sem uso posterior: lab_token

Busca completa em tools/qualification e apps/server identificou apenas: declaração; OpenProcessToken(self) e TokenUser/Elevation/ElevationType/Statistics em lab_operator; closes de cleanup. Não há passagem desse handle ao descriptor, canonical BD, observer ou callback. lab_operator_current usa SOMENTE `lab_stats`, reabre o token corrente e compara TokenId, AuthenticationId e ModifiedId; essa função foi extraída sem alteração textual (exceto fins de linha) para o fixture. `source-review-pins.json` fixa a conferência.

O token em questão é do PRÓPRIO qualificador. NÃO é o token do PowerShell elevado, o token auxiliar, o actor_token de aquisição, nem os anchors canonical BD. Os dois tokens canônicos e todos os processos/arquivos/contextos exigidos por N4 permanecem vivos e revalidados. Os cinco originais transitórios do handoff já fecham no código atual; não constituem economia nova.

Último uso de lab_token: GetTokenInformation(TokenStatistics) depois de todas as verificações iniciais bem-sucedidas. O retorno síncrono não deixa borrow desse handle. As estatísticas são valores copiados, não ponteiros dentro do token. Fechar uma referência de consulta não substitui nem modifica o token primário do processo e não declara destruição física desse token. Revalidações futuras continuam abrindo o token ATUAL e comparando os mesmos identificadores; falhas, impersonation, mudança ou Stop continuam DENY.

Fundamento das identidades: [TOKEN_STATISTICS](https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-token_statistics) define TokenId como identificador da instância e ModifiedId como identificador alterado a cada modificação; AuthenticationId identifica a sessão de logon. [LUID local](https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-allocatelocallyuniqueid) tem escopo local e de boot. Não se presume TokenId igual a SID, nem se revalida apenas um token velho. O boot continua bound pelo owner. Isso não autentica console/imagem nem substitui autorização administrativa.

## Patch candidato e falhas

`premonitor-self-token.NOT-APPLIED.patch`, SHA256 `7b1dc4b012c5d1495290e38630d899e8e94941f7ecb6ea17e5b9e455db7fd4d1`. Conferência git apply --check passou; o patch NÃO foi aplicado. Modifica somente o ciclo instrumentado: depois de lab_operator e boot terem passado, e antes de PROTECTED_BINDING, fecha lab_token APENAS se positive && n0. Não muda lab_operator, rotas read-only/legadas, predicates, launcher ou pins.

Close bem-sucedido precede atribuição NULL; cleanup posterior não repete close. Falha captura imediatamente o código nativo, mantém o handle global no mesmo owner, invoca freeze/Stop/retenção A1 e segue done sem novas aquisições. Não há novo slot, mutex, array, autoridade ou retry. A chamada ocorre antes do mutex/consumo/efeitos; não libera dependências retidas. Caso uma verificação inicial falhe, o bloco nem é alcançado, preservando a primeira falha e o caminho anterior.

A compatibilidade proposta com Model E/N4 decorre do último uso e ausência de borrow do token próprio, não de relaxamento de identidade. A revisão funcional oficial ainda deverá confirmar essa fronteira no executável completo e testar os callers/cleanup, inclusive os modos não alterados. Também deverá reconciliar o armazenamento compilado do novo rótulo de estágio; ausência de novo HANDLE não significa custo total de memória zero. O fixture não executa a validação administrativa inicial real nem concede sua autoridade. Nenhum ACCEPTED foi modificado e nenhum orçamento deriva deste patch.

## Outros lifetimes: preservar

`premonitor-ownership-review.csv` detalha17 grupos. N0 input roots[5] são revalidados pelos mesmos handles; dossier/evidence continuam ligados ao plano/RAW1; policy[4] e approval mantêm originais e compartilhamento que impede novas escritas/deletes. A supressão de hash na metric_window NÃO prova que seja seguro fechar esses handles e perder a proteção de lifetime. Nenhuma redução desses arquivos foi selecionada.

lab_parent/config/native.image/dependencies e cycle_source_file continuam em identidade/SD/hash/locks/borrows. A imagem de origem não é apenas entrada de cópia: o BD ainda a usa. Intent é necessário a admission_effect, cycle_file é revalidado e usado, cycle_process sustenta identidade/liveness/Stop/quiescência. Nenhum dos três é fechado antecipadamente. Observer usa aliases do mesmo processo; não há duplicata estável dispensável demonstrada. Censo/snapshots e threads já fecham antes da próxima fase no caminho limpo. PSS permanece obrigatório e separado. Recursos posteriores de cycle_live não foram otimizados.

## B2 — proposta comparada, equivalência rejeitada

Fonte atual após B1: ig_project_census faz um current completo (um callback de contexto), um inventory (um callback), depois ps_bd_census → ps_bd_inventory → ps_bd_console → validação auxiliar/contexto PARA CADA entrada. Em N entradas aceitas são N+2 callbacks, e não o N+3 do relatório histórico anterior a B1. Essas chamadas são sequenciais; seu número não se soma como handles simultâneos.

O comparador usa o header ps_bd_census/ps_bd_inventory REAL. A candidata extrai exatamente o sufixo de membership para processar a MESMA sequência sem repetir inventário por entrada. Preserva verificações iniciais e acrescenta current/inventory de encerramento, totalizando4 callbacks. Não há cache global: proposta de geração local isolada. A prova N0 usa IDs sintéticos e callback de teste, não o owner operacional. O callback executa APIs reais de token próprio, identidade de arquivo e bytes RAW/ReadFile no próprio executável já existente; não lê dossiê nem consulta SCM.

Nos caminhos limpos, quatro builds /W4 /WX sem warnings e212 execuções de testes passaram. Isso inclui testes que ESPERAM a falha de equivalência: exit0 do teste não significa que B2 seja segura. Há três réplicas por método/perfil/configuração para32/256/1024 entradas. Exemplo Selected Release, medianas:

| N | callbacks atuais/propostos | tempo atual/proposto (ms) | pico transitório sobre antes |
|---:|---:|---:|---:|
|32|34 /4|0,7101 /0,1404|+1 /+1|
|256|258 /4|5,9857 /0,1445|+1 /+1|
|1024|1026 /4|23,0257 /0,1412|+1 /+1|

Todos os perfis tiveram +1 em ambos os métodos, devido ao token de revalidação emprestado/fechado por A1. São tempos do callback substituto, não desempenho do contexto protegido completo, censo real ou futuro LAB. ReadFile lê256 bytes; não reproduz todos os hashes/RAW1/SCM/signature. Diferenças absolutas de baseline entre processos não são economia atribuída; comparação de pico é normalizada ao próprio antes.

Contraexemplo determinístico: uma observação obrigatória reversível torna-se inválida na entrada intermediária e volta ao valor inicial antes do final. O atual detecta ERROR_ACCESS_DENIED5 naquele ponto; os endpoints da proposta passam. O sinal injetado modela uma observação como client context, que pode mudar e retornar; não simula ABA impossível de TokenId/ModifiedId, não é prova de ataque nativo real, e não afirma que o código atual detecta todas as mudanças entre amostras. Demonstra perda de UMA observação que o código atual efetivamente faria.

Segundo contraexemplo: divergência de contexto precede um filho arbitrário. Atual registra primeiro erro5; proposta só observa o filho e registra170. Mesmo quando ambos negam, primeira falha/ordem não são equivalentes. Mutações persistentes de token/identidade/RAW/generation, filho arbitrário ou prefixo de projeto, Stop, stale60s/1s e UNKNOWN são negadas nos cenários respectivos; isso não elimina o contraexemplo transitório.

Challenge da tentativa e timestamp local não são versão nativa de TODAS as propriedades voláteis. Para reconsiderar B2 seria necessário demonstrar uma fronteira factual válida para a geração ou manter os checks voláteis em seus pontos atuais e qualificar separadamente o agrupamento de trabalho estável. Não inventar tal API/fence, omitir revalidação obrigatória ou promover um contador local a prova. No caminho positivo metric_window já suprime hashes; B2 não garante economia de handles e não é a solução escolhida para pressão pré-monitoramento. Nenhum patch B2 operacional é entregue.

## Evidência nativa e limites

O fixture não cria arquivos de dados, intent, imagem, sink ou processo de serviço. Abre seu próprio executável SOMENTE por leitura e cria handles de token/Toolhelp/duplicatas de self isolados. Os três positivos adicionais são três DuplicateHandle(self), explicitamente substitutos de contagem, não os objetos reais de intent/imagem/serviço. Não há SCM/ACL setter, console adicional, listener, PSS substituto ou novo recorder operacional.

Comparação de anchor em cada configuração: caminho atual conserva1; proposta fecha1, com diferença real before−stable=1, e64 revalidações reais passam com as estatísticas salvas. Divergência de TokenId/ModifiedId é INJETADA nos valores esperados e produz negação; não se alterou o token real do operador. Stop nega novas aquisições. No caso close-unknown, CloseHandle NÃO é chamado (erro5 injetado); o handle real e a dependência file permanecem retidos, A1 congela e o subprocesso retorna2 como contenção. Não é prova de falha real de CloseHandle, close bem-sucedido ou refund; não há segundo close do handle incerto.

GetProcessHandleCount é amostral. A instrumentação acrescenta consultas sem criar handle próprio e preserva GetLastError. Não foram atribuídos custos de kernel/heap/cache, picos não amostrados ou custos do PowerShell/auxiliar. As obrigações UNKNOWN históricas/H0–H9 permanecem. A nova candidata altera uma referência controlada, não compra crédito contra essas lacunas.

## Alternativas e gate

A/C selecionada como CANDIDATA: encerrar somente a referência própria após último uso, mantendo prova em valores e a reabertura atual do token, com freeze após close incerto. Redução estrutural e nativa isolada de1, ainda sem implementação funcional.

B não qualificada: menos callbacks e tempo no fixture, mesmo pico de1, perda de observação e de primeira falha. D permanece para ADMISSÃO OPERACIONAL: nenhum PASS antecipado.

Se a base126 fosse idêntica e contivesse essa referência, uma redução1 produziria125+3=128 estáveis no primeiro monitoramento. Isso NÃO é métrica fresca nem garante capacidade: PSS no experimento anterior adicionou até2 nos pontos amostrados, referências transitórias ainda existem, e o binário/contexto podem mudar. Não elevar128, não converter essa subtração em crédito, e não chamar128 de margem suficiente. Qualquer implementação deverá reconciliar o conjunto inteiro antes do consumo e manter1024 administrativos, quatro papéis,60s/1s, headroom e NONE_GRANTED.

Solicitar aprovação funcional SEPARADA somente para o patch candidato exato e seus testes de integração nas quatro configurações. Sem nova tentativa, pins operacionais, preflight ou comando positivo. Este pacote conclui a investigação e a preparação autorizadas; orçamento/admissão permanecem não demonstrados. Não iniciar otimização posterior como desvio desse gate.

CURRENT_TASK_COMPLETED=YES
PREMONITOR_HANDLE_PRESSURE=ONE_REFERENCE_REDUCTION_CANDIDATE_TOTAL_BUDGET_UNPROVEN
SAFE_REDUCTION=ONE_QUALIFIER_SELF_TOKEN_REFERENCE_CANDIDATE_NOT_APPLIED
B2_CONSOLIDATION=NOT_QUALIFIED_COUNTEREXAMPLE_AND_NO_PEAK_HANDLE_REDUCTION
OWNED_LIFETIMES=LAST_USE_BORROW_AND_RETENTION_REVIEWED
NATIVE_RESOURCE_EVIDENCE=FOUR_ZERO_WARNING_BUILDS_212_EXPECTED_OUTCOMES_REAL_SELF_TOKEN_COUNTS_PLUS_SYNTHETIC_CONTEXT_MUTATIONS
START_EXECUTED=NO
STOP_SERVICE_EXECUTED=NO
PREFLIGHT_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_10510_BASELINE_FILES_UNCHANGED_BEFORE_LIVE_APPEND
FIRST_UNRESOLVED_DIVERGENCE=TOTAL_FRESH_PREMONITOR_AND_PSS_CAPACITY_NOT_DEMONSTRATED_FUNCTIONAL_PATCH_PENDING_APPROVAL
NEXT=NATIVE_C_PREMONITOR_CORRECTION_OR_OPERATION_REVIEW
