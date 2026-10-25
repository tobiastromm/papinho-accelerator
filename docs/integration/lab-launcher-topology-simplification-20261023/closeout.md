# Topologia do launcher — resultado final

Alternativa A/B qualificada isoladamente e aplicada: o launcher cria o MESMO qualificador por ProcessStartInfo com UseShellExecute=false e CreateNoWindow=false, herdando console existente, em vez de usar o caminho de comando nativo do pipeline PowerShell. Permanecem os quatro papéis accepted. Nenhuma whitelist, alteração do censo ou ampliação B4.B.4. O qualificador/helper e candidato de serviço permanecem byte-idênticos.

## Prova e limites

[Fixture final](console-topology-fixture.c), compilada MSVC19.51 /W4 /WX Debug/Release sem warnings, e [resultados](corrected-fixture-results.json): flags0 herdaram console, com ambos os PIDs na lista de console, identidade de criação correspondente ao handle do filho, CONIN/OUT e handler válidos, nenhum NOVO filho inesperado em relação ao baseline de PID+creation. As consultas/handles da fixture foram fechados nos caminhos concluídos; processo filho terminou com exit0. Não se interpreta ausência de janela como ausência de console.

No mesmo experimento CREATE_NO_WINDOW produziu um auxiliar novo observado e perdeu compartilhamento da lista; DETACHED_PROCESS perdeu CONIN/OUT. Essas observações locais não são garantias universais nem qualificam tais flags para a operação. Não se autenticou o auxiliar novo como conhost pelo nome e ele não recebeu nenhuma exceção.

[Função exata extraída por AST](managed-function-test.ps1), sem executar entrada do launcher, passou em Windows PowerShell5.1.19041.6456: filho fixture Debug/Release preservou a lista compartilhada incluindo o PID PowerShell, CONIN/OUT READ|WRITE e handler, captura da primeira falha simulada170 e exit0. Falha de criação e argumento inseguro são rejeitados. Interrupção downstream da captura reteve Process antes de término factual; a fixture só o fechou após WaitForExit. [Resultados](managed-function-results.json).

Regressões do código inalterado: busy-diagnostic6 casos e instrumented-policy34 casos PASS, incluindo identidade/limites/Stop/retenção/primeiro erro. Não são um novo preflight. Nenhum teste acessou SCM/rede/CA. Fixture nativa comum aos profiles; não foi necessária recompilação de Selected/Legacy nem de binário de produto para um patch PowerShell. Análise sintática do script final PASS, sem executar Check/Prepare/Launch.

A primeira versão da fixture usava HWND nulo como indício de ausência de console e contou filhos sem baseline de geração. Falhou e foi corrigida antes de qualquer conclusão de implementação. [Revisão anterior](prequalification-review.md), [fonte inicial](initial-console-topology-fixture.c), AllocConsole/5 e resultado inicial preservados; não foram convertidos em PASS. A primeira chamada do teste Windows PowerShell foi bloqueada por assinatura; a fixture foi executada com a opção ExecutionPolicy Bypass por processo já usada pelo launcher qualificado, sem modificar política persistente do Windows.

## Aplicação e recursos

[Manifest de aplicação](launcher-apply-manifest.json): somente tools/qualification/scm-instrumented-start-uac.ps1, preimagem123df4c4717ff77fe699af4bb1031d8445a17c1a7cfeaefb0ec7ba85547040dd, pós-imagem3983d3fc6143a50bfebf7ecc56a28a1213a6275390a50bda4a38d46fc9a5df4a, conferidas antes/depois pelo executor. A revisão oficial da chamada foi aprovada no escopo de implementação autorizado. Nenhuma aceitação normativa nova inferida.

A função usa caminho pinado, argumentos sem shell, leitura síncrona de stdout e espera factual de término, sem reader thread/worker/retry. Stderr fica herdado no console do operador; os registros governados IG/primeira falha emitidos pelo qualificador permanecem em stdout e no recibo limitado existente. Stderr arbitrário não passa a integrar o log durável; não se afirma captura universal de falha CRT. O contrato governado é o registro explícito stdout, confirmado nos testes.

Reader e Process são explicitamente Dispose após exit factual. Falha de leitura/pipeline/fechamento preserva a referência Process e faz o launcher reter pins/log, tentar recibo UNKNOWN e permanecer em espera na mesma thread, sem kill/retry/fechamento presumido. Isso não é término de serviço nem prova de quiescência do host. Nenhuma referência nativa histórica foi liberada pela fixture.

Process/reader são referências gerenciadas no mesmo PowerShell e limites monitorados existentes; não há papel de processo adicional, mutex novo ou allowance. Custos CLR/kernel e stack transitiva continuam UNKNOWN. Não se concedem créditos por redução de pipes ou processos, nem se declara pico físico universal. B4/ED11 permanecem restrições de admissão, não PASS operacional antecipado.

## Topologia e próxima operação

O shell interativo do owner inicia o PowerShell launcher; este mantém sua identidade/creation pinadas enquanto o PowerShell elevado UAC chama o qualificador. O qualificador valida ambos os atores por handle/creation/image/token e conserva o quarto slot para o serviço futuro. O console é compartilhado no passo qualificado; não se cria nem se adota serviço. O recibo anterior demonstra somente conhost6444/parent22220 rejeitado; não atribui causalidade histórica exata a RunAs ou ao pipeline. Esses PIDs não são identidades reutilizáveis.

A qualificação é isolada e não comprova que o ambiente elevado real já satisfaz o censo: qualquer filho inesperado continuará causando STOP. Se o ambiente UAC ainda introduzir auxiliar no censo, preservar recibo e não repetir. Alternativas de reduzir atores ou rever B4 não foram selecionadas. [Proposta D](B4-B4-topology-PROPOSED.md) foi preparada durante a auditoria e permanece apenas histórica/condicional, não necessária para o patch selecionado, não aceita nem implementada.

[Handoff](manual-handoff.md) contém comando preparado com novo pin, sujeito a NOVA autorização específica de preflight read-only. Não foi executado. Nenhum rebind/Start/Stop ou aprovação positiva foi armado.

```text
CURRENT_TASK_COMPLETED=YES
LAUNCHER_TOPOLOGY=EXISTING_CONSOLE_INHERITANCE_QUALIFIED_OFFLINE
CONSOLE_PROCESS_REQUIREMENT=NO_NEW_AUXILIARY_IN_INHERITANCE_FIXTURES
B4_B4_COMPATIBILITY=FOUR_APPLICATION_ROLES_PRESERVED_FACTUAL_ADMISSION_PENDING
MINIMUM_CORRECTION=EXPLICIT_PROCESSSTARTINFO_INHERITED_CONSOLE
CANONICAL_CENSUS=UNCHANGED
OFFLINE_TESTS=40_EXISTING_MODEL_CASES_PASS
NATIVE_TESTS=DEBUG_RELEASE_CONSOLE_AND_MANAGED_FUNCTION_FIXTURES_PASS
PREFLIGHT_RETRY=NOT_RUN
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES_EXCEPT_AUTHORIZED_LAUNCHER_AND_DOCS
FIRST_UNRESOLVED_DIVERGENCE=REAL_ELEVATED_CENSUS_NOT_REQUALIFIED
NEXT=MANUAL_READ_ONLY_PREFLIGHT_AFTER_TOPOLOGY_QUALIFICATION
```

Fontes Microsoft: [Creation of a Console](https://learn.microsoft.com/en-us/windows/console/creation-of-a-console), [Process Creation Flags](https://learn.microsoft.com/en-us/windows/win32/procthread/process-creation-flags), [GetConsoleProcessList](https://learn.microsoft.com/en-us/windows/console/getconsoleprocesslist), [ProcessStartInfo](https://learn.microsoft.com/en-us/dotnet/api/system.diagnostics.processstartinfo). O código PowerShell7 consultado na pesquisa não foi usado como prova de Windows PowerShell5.1; os testes usam a versão factual acima.

### Resultado elevado posterior

O preflight manual autorizado permaneceu STOP/PREFLIGHT_CENSUS170, rejeitando outro conhost.exe (PID19516,parent13056; dados históricos). A qualificação isolada não eliminou o bloqueio real. Sem Start/Stop/efeitos/consumo; próximo passo é diagnóstico de papéis e associação nativa de console, sem whitelist ou repetição automática. [Recibo e análise](manual-inherited-console-result.md).
