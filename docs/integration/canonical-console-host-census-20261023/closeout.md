# Console host census — revisão focal

Resultado: STOP; nenhuma correção funcional aplicada. A condição do owner (preservar DENY quando a identidade não puder ser comprovada) permanece aplicável.

## Evidência factual

HEAD main: 913c90cf2328f7939d8f5b927640601903ac6e91. WIP preexistente preservado.

[Recibo](../canonical-preflight-busy-diagnosis-20261023/manual-prepare-134372728231206156-final.json), SHA256 e45e9d2dd88cf70c69843e13cf5d56edf5bdfc687847be1c3a8132e983bd5795:

```text
FIRST_BUSY_STAGE=PREFLIGHT_CENSUS
FIRST_BUSY_PREDICATE=!participant && child
OBSERVED_PROCESS=conhost.exe
OBSERVED_PID=6444
OBSERVED_PARENT_PID=22220
child=1; project_prefix=0
first=170; close=0; starts=0; stops=0
intent_consumed=0; effect=0; diagnostic_created=0
```

O predicado real é !participant && (child || project_prefix); nesta amostra somente child foi verdadeiro. O recibo não contém associação nativa console/host, creation time, handle original, identidade nativa de imagem, token ou métricas desse host. Prova a causa da rejeição, ainda não prova que ignorá-la seria seguro. PIDs históricos não foram consultados como identidades atuais. A afirmação anterior de falso positivo fica condicionada a essas provas.

## APIs suportadas

- [GetConsoleWindow](https://learn.microsoft.com/en-us/windows/console/getconsolewindow): HWND do console atual ou NULL; em pseudoconsole pode ser apenas janela para fila de mensagens.
- [GetWindowThreadProcessId](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowthreadprocessid): processo/thread criador de um HWND.
- [GetConsoleProcessList](https://learn.microsoft.com/en-us/windows/console/getconsoleprocesslist): processos anexados ao console atual; não autentica imagem/geração nem documenta que o servidor conhost deva constar da lista.

Compor HWND do console e processo criador é um caminho candidato de correlação, não autenticação suficiente isolada. Exige observação na tentativa, handles e creation times estáveis, imagem protegida verificada, contexto de identidade e revalidação. Não generalizar para outro console, Windows Terminal ou pseudoconsole. Nome, parent PID e caminho textual isolados não bastam.

PRIMEIRA PROVA AUSENTE: associação nativa do console autorizado à instância exata rejeitada e sua identidade de criação/imagem. Não houve coleta por novo launcher, segundo censo ou processo auxiliar.

## Restrição ED11 independente

[B0–B6 accepted](../../adr/DECISION-ed11-single-lab-diagnostic-native-boundary-20261023.md), B4.B.4: no máximo quatro processos, papéis explícitos launcher PowerShell, PowerShell elevado, qualificador nativo e serviço. Infraestrutura de terminal/OS já existente permanece no baseline; filho adicional da operação é topologia não planejada => STOP. B4.B.1 exige métricas; B4.B.6 invalida mudança de topologia; B4.B.5 mantém NONE_GRANTED.

Implementação: IG_SAMPLE.process[4], arrays process/creation/pid[4], ig_metric rejeita count>4; slot3 reservado ao serviço. Três participantes existem antes de Start. Usar o quarto slot para conhost no preflight não comprova coexistência com o serviço. Nenhuma prova histórica permite atribuir esta instância ao baseline permitido.

GOVERNANCE CONFLICT seria introduzido por aceitar quinto processo operacional sem revisão de B4.B.4 ou por classificá-lo como baseline sem prova. Essa alteração não foi aplicada. Não há modificação silenciosa de contrato accepted.

Handles, threads, memória, stack observável, simultaneidade e lifetime do conhost permanecem UNKNOWN, não zero. close=0 é resultado de fechamento do qualificador, não término do console host. Nenhum crédito/quota/threshold alterado.

## Menor continuação

Preparar coleta focal no censo existente que correlacione console/host e identidade factual, sem aceitar automaticamente o resultado. A mesma geração deverá distinguir:

1. Infraestrutura já existente do console do owner, demonstradamente coberta pelo baseline, com custos observáveis explicitados e sem crédito de reutilização.
2. Auxiliar novo da operação: requer escolha explícita de topologia compatível com o limite ou revisão normativa específica antes de implementação. Não promovê-lo a baseline por conveniência.
3. Identidade indisponível/divergente, filho arbitrário ou concorrente: manter ERROR_BUSY/STOP.

Não se escolheu envelope numérico novo, nem ADR concorrente. Não existe patch de aceitação qualificado, portanto pins e launcher permanecem inalterados. Nenhum novo comando de preflight autorizado ou executado. Não se declara PASS de testes modelados como substituto de prova nativa.

Matriz necessária para eventual implementação: host legitimamente associado e autenticado; mesmo nome sem relação; imagem divergente; PID reutilizado; parent divergente; filho arbitrário; concorrência; identidade indisponível; limite excedido; Stop; retenção; ERROR_BUSY nos casos inseguros. Builds/testes não foram executados nesta revisão documental; resultados anteriores não foram reutilizados como nova qualificação.

## Resultado

```text
CURRENT_TASK_COMPLETED=NO
CONSOLE_HOST_CLASSIFICATION=DENY_MISSING_NATIVE_ASSOCIATION_AND_ACCOUNTING
CANONICAL_CENSUS=UNCHANGED_FAIL_CLOSED
CROSS_ADMISSION_EXCLUSIVITY=PRESERVED
CONHOST_RESOURCE_ACCOUNTING=UNKNOWN_NOT_ZERO
ED11_TOTAL_STATUS=PARTIAL
BUILDS=NOT_RUN_NO_FUNCTIONAL_CHANGE
WARNINGS=NOT_ASSESSED_THIS_TASK
OFFLINE_TESTS=NOT_RUN_NO_CORRECTION_APPLIED
NATIVE_TESTS=NOT_RUN
NETWORK_TESTS=NOT_RUN
PREFLIGHT_RETRY=NOT_RUN
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES_DOCUMENTATION_ONLY
FIRST_UNRESOLVED_DIVERGENCE=CONSOLE_HOST_NATIVE_ASSOCIATION_NOT_CAPTURED
NEXT=FOCUSED_CONSOLE_ASSOCIATION_EVIDENCE_AND_TOPOLOGY_REVIEW
```

Nenhum binário, fonte funcional, launcher, mutex, filtro ou contrato accepted alterado. LABs e recibos preservados. Sem clone/worktree/commit/push/tag/release.
