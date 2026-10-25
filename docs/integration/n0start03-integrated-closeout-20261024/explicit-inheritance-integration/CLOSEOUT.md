# ED11 — conclusão da tentativa de integração de herança explícita

2026-10-25 — PACC-PHASE6-ED11-EXPLICIT-INHERITANCE-INTEGRATION.
Branch main / HEAD913c90cf2328f7939d8f5b927640601903ac6e91 preservados. Baseline e preimagens locais foram capturados antes das edições. A autorização desta tarefa cobre implementação/testes técnicos, não execução operacional ou aceitação normativa inferida.

## Resultado dirigido

A integração canônica foi impedida por uma falha de segurança reproduzida, não por falta de autorização para editar ou reconstruir: a preparação privada de HANDLE_LIST expõe stdin, escritor de stdout e stderr a um CreateProcess global concorrente no mesmo supervisor. O teste retorna0 quando reproduz corretamente essa exposição; isso NÃO significa aprovação do gate de segurança.

O novo cenário invoca o código C real NX_Start com um hook compilado somente para o fixture. Após preparar os transfers e atributos e antes da criação pretendida, um segundo thread executa CreateProcessW com bInheritHandles=TRUE, sem HANDLE_LIST. O competidor é criado suspenso para estabilizar a observação; snapshot completo da tabela e referências importadas por DuplicateHandle são comparados aos três objetos originais com CompareObjectHandles. Resultado: três correspondências exatas. Controle com herança FALSE: zero. O hook alonga deterministicamente uma janela possível, não demonstra que um concorrente existiu no LAB nem estima probabilidade.

Os dois controles passaram em Debug/Release, três repetições por configuração. Um lock exclusivo do nosso helper não impede uma chamada global que não participe desse lock. A lista restringe o filho pretendido; não restringe outro criador. Não alterar flags de handles do operador, suspender threads alheios, reduzir a janela ou criar um quinto processo resolve isso dentro das regras atuais.

Referências: [contrato oficial HANDLE_LIST](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-updateprocthreadattribute), [análise oficial da herança inversa concorrente](https://devblogs.microsoft.com/oldnewthing/20260511-00/?p=112313/). O requisito de referências herdáveis é a razão da janela. A garantia de I3 permanece NÃO SATISFEITA; N7 aceito não autoriza ampliar risco para exposição dos canais administrativos.

## Gates e evidências

| Gate | Resultado | Alcance |
|---|---|---|
| Filtragem do filho pretendido | PASS isolado | Oito eventos sintéticos excluídos por identidade; não prova dispensabilidade dos nove handles reais |
| Transferência frente a criador global concorrente | FAIL, reproduzido | Três objetos privados herdados pelo competidor, em todas as repetições |
| Controle de concorrente sem herança | PASS | Zero dos três objetos herdados |
| Console nativo interativo | PASS isolado | Console real oculto, mesmo HWND pai/filho, ReadConsoleW, WriteConsoleW, confirmação, Escape, F10 e término |
| Semântica canônica PowerShell/console loss | NÃO QUALIFICADA | Read-Host/Console.ReadKey, notificações de perda de console e retenção do launcher real não foram promovidos por testes C |
| Identidade/supervisão | PASS no escopo testado | Handle original de CreateProcess, PID/criação, Wait/exit-code; não reabre PID para supervisão |
| Ownership/UNKNOWN | PASS nos fixtures cobertos | Close incerto, lease inválido, erro de saída, freeze, primeira falha, zero novas aquisições; retenção sem retry/kill |
| Integração operacional/pins | BLOQUEADA | Launcher e pacote operacional históricos intactos |
| Capacidade positiva128/ED11 total | NÃO DEMONSTRADA | Nenhuma nova observação LAB |

Console: o processo de teste nasce com CREATE_NEW_CONSOLE e SW_HIDE, sem interação com o console do operador. O filho compartilha esse console. O pai abre CONIN$/CONOUT$ próprios do fixture; o helper duplica privadamente esses canais e redireciona stdout ao pipe. Teclas são injetadas com WriteConsoleInputW somente nesse console isolado. A confirmação é lida pelo filho; Escape solicita Stop sintético e F10 é lido pelo pai depois do término. WriteConsoleW verifica stderr; protocolo de pipe/stdin/stderr também permanece coberto pelos testes anteriores. Não há serviço, StopService ou autenticação nova. Ausência de janela visível é uma propriedade do fixture, não bypass do censo operacional.

## Código e fronteiras

- Novo tests/explicit_inheritance_boundary_test.c reutiliza o helper C existente em execução isolada; nenhuma ligação ao launcher ou serviço.
- NX_BEFORE_CREATE é um hook de compilação do fixture, com implementação default no-op; não altera a política de criação nem autoriza o qualificador LAB.
- O fixture de observador ganhou somente modo console-child. Rotas operacionais não são chamadas.
- CMake adiciona alvo Windows moderno /W4 /WX. Vista+/Windows10/PSS permanecem fora dos componentes NT4/VC6. Não houve mudança de toolchain/backend para contornar falha.
- Falha inesperada de fechamento, PSS free, ResumeThread ou wait no novo cenário retém referências originais no processo/thread do teste, registra UNKNOWN e espera; o runner registra PID e não mata em timeout. Em execução limpa o competidor suspenso é retomado uma vez e termina naturalmente. Não há kill, rollback ou liberação forçada.
- As falhas injetadas do helper experimental continuam cobertas pelos testes de seu contexto. A política A1 de dois slots do produto, B1, self-token e o owner canônico não foram modificados. Não atribuir esse contexto ao orçamento do serviço.

A condição “integrar após qualificar gates” não foi alcançada. Não foi inserido código incompleto ou opt-in capaz de contornar esse gate no launcher canônico.

## Builds, regressões e lifetimes

Selected Debug/Release PASS; zero warnings, /W4 /WX. Logs de build inicial, final e qualified preservados. Os targets compilam o qualificador como dependência de qualificação offline, mas NÃO o executam.

Resultados finais:62 regressões (32 cenários de observador/tabela e30 do owner/recursos),36 casos de herança/protocolo/fault injection e6 cenários novos de fronteira PASS conforme resultados esperados. Os6 cenários executam18 iterações internas (global/none/console × Debug/Release ×3), incluindo o resultado NEGATIVO que bloqueia integração. JSON finais identificam os binários e logs dessa árvore de build; evidências anteriores não foram sobrescritas.

Novos cenários: referências controladas terminam0. Contagem total no processo de fronteira40→52 na primeira execução e52→52 nas duas execuções aquecidas, em ambos os controles e console, nas duas configurações. O aumento inicial12 não foi atribuído a objetos; não é declarado leak nem custo zero. Estabilidade amostrada/fechamento de slots não provam ausência universal de vazamentos, resíduos Windows ou picos não amostrados. O overhead do segundo thread, snapshot e competidor é do experimento adversarial, não custo projetado de admissão.

O helper mantém contexto4408 bytes, atributos lógicos48, pico controlado6 e estável2. A simulação de close incerto retém6 referências; lease/erro de saída retêm2. Essas são referências do supervisor de teste. Custos managed/heap/loader/kernel/stack não se reduzem a esses tamanhos. Nenhum crédito é concedido.

## Economia e ED11

Repetição da comparação serial: Debug68→56 e Release52→40; oito exclusões com identidade provada. Diferença total12 não integralmente atribuída. Os12 desta comparação não são os12 do aquecimento do executável de fronteira; são processos e medições diferentes.

LAB continua com evidência histórica71/105/126 e pico amostrado128, administração984/1024. Nenhuma redução real nova no LAB. +26 não atribuídos após SCM continuam sem causa individual demonstrada. Não extrapolar economia8/12/9 para a capacidade positiva.

Projeção condicional anterior preservada: pré-Start129−R+U, primeiro monitor130−R+U; R precisa ser redução real comprovada e U custo interno adicional desconhecido. Nenhum orçamento independente foi concedido.128/1024, quatro papéis,60s/1s, headroom, NONE_GRANTED, H/P3/RAW1/N7 e UNKNOWN históricos intactos. ED11 e Phase6 continuam PARTIAL.

## Decisão necessária, alternativas concretas e próxima ação

A ADR candidata foi atualizada para revisão3 PROPOSED, com a contraprova e alternativas. Nenhuma decisão accepted foi editada ou criada.

A. Coordenar TODAS as criações no mesmo processo: mantém topologia, mas exige cobertura demonstrável de cada criador. Um lock local não basta. Aceitar confiança em componentes não coordenados relaxaria I3 e precisaria de decisão explícita, com exposição potencial de entrada e forja/interferência na saída. Não está aprovado.

B. Helper-processo de transferência, solução oficial para essa janela: tecnicamente separa os transfers herdáveis, porém adiciona processo/lifetimes e viola os quatro papéis/topologia atuais. Não implementado nem recomendado sob estes contratos.

C. Herança FALSE e bootstrap de stdout por endpoint explícito autenticado no MESMO owner; stdin/stderr abertos no console compartilhado após qualificação: evita criar transfers herdáveis no PowerShell, mas altera transporte inicial/N5/B+D. Exigiria vínculo ao processo ORIGINAL+criação/token, endpoint protegido e exclusivo, revalidação, primeira falha, Stop/retention e custos. Nome/PID/nonce isolados não substituem prova. Isso constitui revisão arquitetural, não otimização transparente autorizada nesta tarefa.

Disposição atual: STOP para integrar a variante atual. Se a meta é eliminar tecnicamente a janela preservando quatro papéis, a próxima ação concreta é decidir o escopo normativo de C antes de implementar esse novo bootstrap. Não pedir outra observação genérica de handles nem aceitar outra rodada LAB como prova da ausência dessa janela.

Não há procedimento operacional qualificado para entregar nesta etapa, portanto não foi criado novo pedido de observação nem registro de aprovação. A aprovação operacional separada continua necessária depois de uma solução segura e pins factuais; uma aprovação de execução não deve substituir a decisão arquitetural ausente.

## Hashes e preservação

Todos os SHA256 integrais: artifact-hashes.json. SHA256 da ADR candidata revisão3: 6fc1450eb5204fc1b1765baf2ed87503be120b86aa88f636e424bb58b8db0a44. SHA256 do executável boundary Release: f2e9f9cbd230383f19667cff057fd416fdcfad8b8b183dbcd84762329ca4108b. O helper experimental e os binários novos estão na árvore build/ed11-inheritance-integration-20261025; não substituem o pacote histórico.

Launcher canônico permanece3451d4d5e2b2dd6bfae8ac64fc97293b63a9d7a3658fd0d00cba805be35bf2ab; qualificador operacional anterior permanece2bf1746ae9de3f5d559b4fc989d9da3c3f9dacfe47539f8b0b067e2fe496ae15. Pins derivados operacionais não foram alterados porque a integração foi rejeitada. Aprovações consumidas, arquivos protegidos, intent/imagem/sink, SCM, ACL, histórico e WIP preservados.

CURRENT_TASK_COMPLETED=YES_TECHNICAL_REVIEW_WITH_PROVEN_INTEGRATION_BLOCKER
CONCURRENCY_GATE=FAIL_PROVEN_PRIVATE_TRANSFER_EXPOSURE
CONSOLE_GATE=NATIVE_ISOLATED_PASS_CANONICAL_PENDING
CANONICAL_INTEGRATION=NOT_APPLIED_UNSAFE
DEBUG_RELEASE=PASS_ZERO_WARNINGS
REGRESSIONS=62_PASS
MECHANISM_CASES=36_PASS
BOUNDARY_CASES=6_EXPECTED_OUTCOMES_18_ITERATIONS
ADR_STATUS=PROPOSED_REVISION3
NEW_OPERATION_REQUEST=NOT_PREPARED_UNSAFE
OPERATIONAL_PINS=UNCHANGED
POSITIVE_CAPACITY=NOT_DEMONSTRATED
ED11_TOTAL_STATUS=PARTIAL
PHASE6_TOTAL_STATUS=PARTIAL
PREPARE_EXECUTED=NO
START_EXECUTED=NO
STOP_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=FOCUSED_TRANSPORT_OR_CREATION_ISOLATION_ARCHITECTURAL_DECISION
