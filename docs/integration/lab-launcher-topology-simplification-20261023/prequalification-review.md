# Auditoria da topologia do launcher LAB

Nenhum patch funcional aplicado. Nenhuma alternativa foi qualificada como substituta do launcher. DENY permanece. HEAD main 913c90cf2328f7939d8f5b927640601903ac6e91.

## Topologia comprovada pelo código e pelo recibo

[Launcher](../../../tools/qualification/scm-instrumented-start-uac.ps1): o shell original do usuário executa uma instância Windows PowerShell com -File (papel launcher). Essa instância permanece em Wait enquanto Start-Process -Verb RunAs inicia outra instância Windows PowerShell elevada. A instância elevada chama o qualificador nativo por `& $Exe @nativeArgs 2>&1 | ForEach-Object`, preservando saída limitada/recibos. O serviço é o quarto papel futuro, ainda não iniciado. O shell interativo externo não deve ser confundido com a instância -File observada pelo owner.

[Qualificador](../../../tools/qualification/scm-instrumented-cycle-lab.inc): ig_actor abre os dois processos PowerShell por handles, verifica creation time, caminho e SID do token; portanto papéis administrativos não são inferidos apenas pelo parent PID. ig_actor rejeita papéis repetidos/self. O quarto slot do observer fica para o serviço. A confirmação positiva usa CONIN$/CONOUT$ e o Stop usa o console handler existente.

[Recibo factual](../canonical-preflight-busy-diagnosis-20261023/manual-prepare-134372728231206156-final.json): qualificador PID18716/creation31286088:3470518135, filho rejeitado conhost.exe PID6444/parent22220, child=1/project_prefix=0, primeiro170 no censo. O recibo não registra os handles/creation times completos do console host nem prova a qual dos dois consoles ele pertence. Não se reutilizaram PIDs históricos. O registro de criação dos consoles não está disponível nesse recibo: não se afirma que RunAs ou o pipeline tenham criado especificamente aquele conhost.

## Alternativas, na ordem solicitada

A. Herança de console existente: Windows documenta herança por padrão na criação de filho console. CREATE_NEW_CONSOLE e DETACHED_PROCESS mudam essa relação. É a alternativa preferencial de menor alteração, mas sua compatibilidade com o fluxo Windows PowerShell5.1/RunAs/redirecionamento pinado não foi demonstrada pela fixture atual. Requer prova no ponto de criação e preservação de saída, CONIN/OUT e handler; não basta janela visível ou nome de processo.

B. Flags: CREATE_NO_WINDOW e DETACHED_PROCESS não constituem uma qualificação automática. A documentação prevê diferença de console e pode comprometer a confirmação atual. A fixture nesta sessão não estabeleceu a separação esperada entre janela, associação e handles; nenhum flag foi aplicado ao launcher. Ocultar janela não elimina processo nem dispensa censo/accounting.

C. Menos intermediários: usar diretamente uma única instância PowerShell elevada pode reduzir um processo lógico, mas exige revisar binding de dois atores, argumento de criação, owner do log/retained record, observação e lifetime. Passar o mesmo PID para os dois papéis viola o contrato atual e a validação. A implementação não foi reduzida silenciosamente; esta alternativa não é mero ajuste de censo.

D. Revisão normativa: somente se A não puder ser qualificada e C não for selecionada. Foi preparado um texto focal PROPOSED, não aplicado, sem afirmar inevitabilidade de conhost adicional. B4.B.4 accepted permanece intacto. Não há whitelist.

## Experimentos isolados e limites

Fixture C99 compilada com MSVC19.51, /W4 /WX, Debug e Release, sem warnings. Nenhuma biblioteca SCM, rede ou CA vinculada; criação de filho por CreateProcessW e pipe compartilhado stdout/stderr, conferência de creation time em handle, espera de término e fechamento verificado nos caminhos concluídos. Fontes e hashes em evidence-manifest.json; scratch externo governado preservado.

1. A tentativa inicial da fixture tratou GetConsoleWindow()==NULL como necessidade de AllocConsole. AllocConsole falhou com Win32 5, exit40. HWND nulo isolado não é prova suficiente de falta de associação ao console. Essa premissa não foi aplicada ao launcher. A execução com diagnóstico focal preservou exatamente o código5, sem alteração de permissões ou políticas.
2. Em uma fixture independente sem alocação de console, o ramo CREATE_NO_WINDOW retornou exit33. Saída factual: same_console=1 (comparação de HWNDs; não prova compartilhamento quando ambos são NULL), CONIN=1, CONOUT=1, handler=1, unexpected_children=2, process_exit=0, checked_close=1. Não se autenticaram os dois processos adicionais; nem se prova que ambos foram novos, pois a fixture não continha baseline de gerações dos filhos. O campo identity_bound=1 da saída era uma etiqueta estática do desenho; não substitui o resultado FAIL do conjunto de verificações. O ramo DETACHED_PROCESS e a execução Release não foram atingidos após FAIL. Não atribuir o resultado ao launcher ou à conta virtual.
3. Modelos existentes Selected Release: busy diagnostic6 casos PASS e política instrumentada34 casos PASS, incluindo limites/identidade/Stop/retenção. São regressões do código INALTERADO; não qualificam uma topologia nova.

As falhas da fixture não provam impossibilidade universal de A/B, nem justificam alterar o host para fazer o teste passar. Não houve retry de preflight, launcher, UAC operacional, Start/StopService, mudança de ACL/SCM, liberação de handles da tentativa histórica ou criação de arquivo operacional.

## Teste discriminante mínimo ainda necessário

Corrigir somente a fixture: detectar associação também por GetConsoleProcessList e GetConsoleMode dos handles, sem usar HWND nulo como prova de ausência; não chamar AllocConsole para recuperar resultado incerto. Capturar baseline e delta de filhos com PID+creation identity e handles, incluindo nomes apenas como diagnóstico; provar console compartilhado por membros da mesma lista e geração. Criar um filho fixture com flags0 e pipe único, verificando confirmação/handler, saída limitada, ausência de auxiliar NOVO e término/fechamento. Flags alternativas devem produzir observações, não pressupostos de sucesso. Primeiro executar sem elevação e sem SCM; se o ambiente não suportar console interativo factual, preparar fixture manual isolada separada, não reutilizar o preflight como teste.

Esse teste não foi executado aqui e não exige rever thresholds B4. Não existe ainda patch do launcher ou novo pin qualificado para entregar como comando de preflight.

## Resultado

```text
CURRENT_TASK_COMPLETED=NO
LAUNCHER_TOPOLOGY=AUDITED_FOUR_APPLICATION_ROLES_PLUS_UNCLASSIFIED_CONSOLE
CONSOLE_PROCESS_REQUIREMENT=NOT_PROVEN_INEVITABLE_OR_REMOVABLE
B4_B4_COMPATIBILITY=UNCHANGED_NEW_TOPOLOGY_NOT_QUALIFIED
MINIMUM_CORRECTION=FIX_ISOLATED_CONSOLE_PROBE_BEFORE_SELECTING_A
CANONICAL_CENSUS=UNCHANGED_UNTIL_QUALIFIED
ISOLATED_BUILDS=Debug/Release_PASS_WARNINGS_0
ISOLATED_TOPOLOGY_TESTS=FAILED_INCONCLUSIVE_NOT_PROMOTED_TO_PASS
UNCHANGED_POLICY_REGRESSIONS=40_CASES_PASS
PREFLIGHT_RETRY=NOT_RUN
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_NO_FUNCTIONAL_OR_PIN_CHANGE
FIRST_UNRESOLVED_DIVERGENCE=NATIVE_CONSOLE_ASSOCIATION_AND_CHILD_GENERATION_PROBE_INSUFFICIENT
NEXT=ISOLATED_CONSOLE_INHERITANCE_PROBE_BEFORE_PREFLIGHT
```

PST/Browser/PACC wire/produção0.6.4 e quotas preservados. Sem clone/worktree/commit/push/tag/release.

Fontes primárias: [Creation of a Console](https://learn.microsoft.com/en-us/windows/console/creation-of-a-console), [Process Creation Flags](https://learn.microsoft.com/en-us/windows/win32/procthread/process-creation-flags), [GetConsoleProcessList](https://learn.microsoft.com/en-us/windows/console/getconsoleprocesslist), [GetConsoleWindow](https://learn.microsoft.com/en-us/windows/console/getconsolewindow), [ProcessStartInfo](https://learn.microsoft.com/en-us/dotnet/api/system.diagnostics.processstartinfo). Código atual de PowerShell7/master foi consultado apenas como referência de pesquisa; NÃO é prova do comportamento do Windows PowerShell5.1 pinado.
