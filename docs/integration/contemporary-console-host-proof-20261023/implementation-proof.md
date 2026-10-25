# Captura contemporânea de console — implementação e limites

TASK=PACC-PHASE6-CONTEMPORARY-CONSOLE-HOST-PROOF-1

Instrumentação preparada e qualificada OFFLINE. Nenhum novo fato do console real foi capturado nesta tarefa. Censo e B4.B.4 permanecem inalterados. O ponto de entrada do launcher não foi executado.

## Integração no owner existente

[scm-instrumented-cycle-lab.inc](../../../tools/qualification/scm-instrumented-cycle-lab.inc) conserva o mesmo predicado !participant && (child || project_prefix). Depois de registrar a primeira falha170 e fechar o snapshot de censo, chama uma única vez o [collector diagnóstico](../../../tools/qualification/scm-console-proof.inc), passando PID/parent da rejeição CONTEMPORÂNEA. Não há PID histórico hardcoded, filtro por nome como autorização, retry, segundo censo, worker, serviço, registry ou autoridade. Retorno continua ERROR_BUSY independentemente da captura. Erro diagnóstico é separado do primeiro erro original.

O collector emite papéis0=launcher,1=elevado,2=qualificador com PID/creation já validados. Mantém handle do processo rejeitado enquanto coleta GetProcessTimes, QueryFullProcessImageNameW, ProcessIdToSessionId, GetConsoleProcessList e GetConsoleWindow/GetWindowThreadProcessId. Revalida criação/liveness, caminho/metadados do arquivo e relação de janela/lista após a coleta. Parent e sessão iguais são indícios registrados, não autorização.

A geração inicialmente aberta e mantida pelo collector pode ser correlacionada durante a coleta. A entrada Toolhelp original não contém creation time: uma troca entre o snapshot e OpenProcess NÃO é autenticada retroativamente. A classificação permanece diagnóstica, com AUTHENTICATED_ASSOCIATION=UNKNOWN mesmo quando a composição de evidências é coerente. Não se declara proteção universal contra toda substituição a partir do número PID. Fatos de imagem mapeada/publisher também permanecem distintos.

## Imagem e proveniência

O caminho do processo é registrado em UTF16 hexadecimal, no máximo768 unidades por linha, com TRUNCATED explícito; não expõe bytes de memória, contexto de thread ou material privado. Para leitura de arquivo/proveniência, existe apenas um guard de escopo de I/O: C:\Windows\System32\conhost.exe em drive local fixo e diretórios Windows/System32 sem reparse observado. Outros caminhos não são abertos para hash/trust e ficam UNKNOWN. Esse guard não aceita processo no censo e não transforma nome/caminho em confiança.

O arquivo é OPEN_EXISTING/read-only, sem share de escrita, com file identity nativa/size/atributos, SHA256 incremental em buffer4096 e limite lógico de leitura16MiB. FileID/hash são do ARQUIVO ABERTO ATUAL, não prova de vínculo ao arquivo originalmente mapeado no processo. O collector também não é uma qualificação de segurança contra administrador capaz de substituir componentes de caminho entre consultas.

WinVerifyTrust usa WTD_UI_NONE, HWND INVALID_HANDLE_VALUE, cache-only URL retrieval e verificação de cadeia excluindo root. Registra LONG nativo sem inventar mapeamento. Não busca rede nem altera trust policy por código próprio. Cache/provedor internos continuam opacos. Não há geração de chave, certificado ou assinatura. Falha por cache/catálogo/chain incompleta não é sucesso. Publisher identity e vínculo da imagem mapeada permanecem UNKNOWN: um retorno0 de confiança do arquivo não prova ownership do console nem autoridade. Estado WVT é fechado uma vez; erro de fechamento retém o estado e suas dependências, sem segundo close.

## Associação suportada e limitação

[GetConsoleProcessList](https://learn.microsoft.com/en-us/windows/console/getconsoleprocesslist) enumera clientes do console atual; não fornece identidade autenticada do host. A presença dos clientes elevado e qualificador é checada antes/depois, com buffer fixo32 e UNKNOWN se insuficiente/indisponível, sem realocação/retry.

[GetConsoleWindow](https://learn.microsoft.com/en-us/windows/console/getconsolewindow) e [GetWindowThreadProcessId](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowthreadprocessid) permitem correlação de HWND e processo criador quando disponíveis. A composição produz SOMENTE window_owner_candidate, condicionada a identidade/imagem estáveis, sessão, membros e ausência de Stop/retenção. Pseudoconsole/desktop restrito/sem janela são limites explícitos. HWND nulo não substitui GetConsoleProcessList nem significa sozinho ausência de console.

Não foi identificada uma API pública única que ateste universalmente ownership autenticado de conhost em todas essas topologias. Não se inventa essa garantia. [GetProcessTimes](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getprocesstimes) fornece criação por handle; [WinVerifyTrust](https://learn.microsoft.com/en-us/windows/win32/api/wintrust/nf-wintrust-winverifytrust) verifica confiança do objeto indicado, e [WINTRUST_DATA](https://learn.microsoft.com/en-us/windows/win32/api/wintrust/ns-wintrust-wintrust_data) documenta cache-only. Nenhuma dessas funções isoladamente é autorização.

## Recursos e lifetime

Foi extraído o núcleo da medição JÁ EXISTENTE para ig_measure_process, reutilizado pelos quatro papéis e pelo diagnóstico. Arrays/counters de admissão continuam com4 entradas; o índice diagnóstico4 NÃO é adicionado a ig_observer.count, não ocupa slot do serviço, não passa por promoção de autoridade. Memória/commit/WS/handles/picos observáveis são registrados antes da etapa PSS, para não transformar falha de stack em valores0. PSS observa threads, stack VA comprometida e I/O, sem registrar conteúdo de stack/registers.

A captura também mede os atores0–2 com handles/creation originais, revalidados. São amostras sequenciais de processos coexistentes, com ticks, NÃO um snapshot atômico nem garantia de todos os picos transitórios. Campo indisponível ou falha de métrica fica UNKNOWN. Não há quinto participante autorizado, refunds ou créditos. Limites antigos continuam sendo testados separadamente, sem um guard novo inventado para admitir o auxiliar.

Stop antes/durante coleta impede novas etapas quando observado; I/O/provedor síncrono não tem garantia fabricada de cancelamento ou duração máxima. Não há timeout-as-close. Handle/file/provider/snapshot falhos permanecem retidos no MESMO owner; o native.retained existente impede cleanup dos guards/owner e o launcher preserva a disposição UNKNOWN. Fechamento confirmado das referências não declara término do host de console ou liberação de todo cache kernel.

[Ledger](resource-ledger.md) detalha recursos novos e opacos. ED11 permanece PARTIAL, quotas inalteradas e NONE_GRANTED.

## Qualificação

Quatro builds Selected/Legacy Debug/Release, toolchain/SDK local preservados, /W4 /WX: PASS, warnings0 nos builds finais. Ajustes intermediários de escape C e finais de linha foram corrigidos sem relaxar warnings. [Resumo](evidence/build-summary.json).

28 execuções de teste PASS: dez casos de fatos de console, dez casos de diagnóstico/captura/Stop/retenção/close e34 de política por variante (216 casos nessas três suites), mais16 execuções das regressões runtime/lifecycle. [Saídas](evidence/final-tests.json).

Matriz: candidato coerente de janela; mesmo nome sem relação (nome não é entrada de confiança); identidade/geração divergente; imagem divergente; sessão divergente; janela indisponível/trocada; membros ausentes antes/depois; Stop; retenção. A suite existente testa recursos excedidos/limite/freshness. A suite diagnóstica adiciona OpenProcess negado, captura única sem retry, preservação de LastError/primeira causa170, fechamento único e close falho retido sem repetição. Handles retidos nas falhas injetadas são FAKES de fixture; nenhum owner real antigo é liberado pelo teste.

Esses testes de fatos/API injetada não comprovam relação real ou assinatura do conhost Windows. Nenhum preflight positivo, chamada de entrada do launcher, captura contemporânea real, serviço ou teste de rede executado. A operação futura segue dependente de autorização específica.

## B4.B.4

Não é permitido interpretar silenciosamente outro PROCESSO físico como se fosse apenas memória do processo PowerShell: o texto accepted estabelece até4 processos com papéis explícitos. Identidade/associação comprovada não remove esse requisito. [Correção focal PROPOSED](B4-B4-associated-console-PROPOSED.md) prepara a alternativa de composição do papel elevado, com contagem física explícita, agregados sem aumento e nenhuma aceitação automática. Ela não está aplicada nem implementada. O estado atual continua DENY.
