# Prova C nativa — reconciliação de PARTICIPANT_TOPOLOGY_DENY

## Fatos da execução manual

Pedido2f225db7 / aprovação4a5f2e00, boot237900, PID10252/criação134376273119221101. [Saída fornecida pelo proprietário](OWNER-MANUAL-OUTPUT.json), [API counts](NATIVE-API-COUNTS.txt), [reconciliação](RECONCILIATION.json) e [ledger](PHASE-LEDGER.csv).

A consulta de handles foi válida:94, erro0. Passou pelo pré-observador124, com30 de diferença naquela observação;34 até128. Pico amostrado94. Entrada80, dependências83, metadata90, arquivos91/92/93/94. Isso comprova somente capacidade nessas fases pré-TBS; não demonstra pico futuro, custo TBS/RPC ou capacidade total da prova. Não inferir economia operacional exata249-94 entre processos/composições diferentes. Os10 ticks iguais não medem duração total do processo ou pico universal.

Primeira falha PARTICIPANT_TOPOLOGY_DENY, INITIAL_ADMIN, native0 (negação lógica). Clientes2 = [10252,17204], pai17204, PID associado ao HWND17204. A única condição reprovada que esses campos demonstram é `console==parent`; count/membership/nonzero/distinção de self estão satisfeitos. Não se trata de erro de consulta ou negação por contagem.

TBS open/submit/close=0/0/0. Contexto NOT_OPENED: context_close=false não é falha de fechamento de um contexto aberto. Proof=false, Name=null, sessão não criada, receipt não criado. Nenhum comando TPM/NV/Q4, novo intent, Start/Stop, SCM/ACL ou autoridade produtiva. Amostras administrativas vazias: o código negou antes de OpenProcess dos observadores;1024 NÃO foi aferido. Sem aferição, não se presume margem administrativa.

Final controlled_refs=0, retained=false, observer_refs_retained=false, observer_close_error0. Fechamento controlado reportado sem retenção; não comprova quiescência global/fechamento de internos Windows/CRT/caches ou efeitos históricos Q4. Revogação UNKNOWN; geração e posse nativa não demonstradas.

Os quatro hashes e os IDs nativos impressos foram preservados como evidência daquele instante; os hashes conferem com o pacote. Não se inferiram IDs por hash nem foi feita uma nova leitura protegida dos objetos Q4. [Pins originais conferidos](HISTORICAL-PACKAGE-PINS-PRESERVED.json).

## Autorização consumida

A aprovação4a5f2e001feb29a0607333c40dd46e413b3e75a3957b1c527d7090b2eab1376c foi usada por UMA invocação. Não editada, reutilizada ou substituída. Ausência de receipt não significa autorização disponível. NÃO executar novamente o comando anterior. Nenhum novo pedido/aprovação/comando operacional foi criado neste trabalho.

## Causa e correção permitida

O contrato do pedido exige processo associado à janela distinto de self e pai. `host_platform.c:hp_admin` implementa exatamente isso. O mesmo teste existia no host C#; portanto não há defeito de portabilidade C demonstrado nessa condição. A migração resolveu a pressão observada nas fases iniciais, mas não alterou a restrição de topologia já conhecida nos testes de console.

O pacote anterior já documentava negações de HWND alias e usava papéis modelados no fluxo positivo. Logo não era correto tratar esses ensaios como garantia de admissão do console real. O resultado atual torna concreta a variante pai==janela no ambiente do proprietário. Não é justificável gastar outra aprovação com a mesma hipótese.

Segundo a Microsoft, [GetConsoleWindow](https://learn.microsoft.com/en-us/windows/console/getconsolewindow) pode devolver em pseudoconsole um HWND destinado apenas a mensagens; [GetWindowThreadProcessId](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowthreadprocessid) identifica o criador da janela. Isso não autentica o broker real nem autoriza excluir custos. O recibo não permite afirmar imagem/identidade real de conhost nem classificar conclusivamente o terminal como pseudoconsole.

Não removemos a condição, não somamos pai duas vezes e não omitimos auxiliar para passar. [Proposta focal de resolução](TOPOLOGY-RESOLUTION-PROPOSED.md): ambiente distinto comprovado sem TPM OU binding explícito canônico do auxiliar real com contabilização completa e revisão do contrato antes de outro pedido. Nenhuma nova autoridade inferida.

## Testes efetivamente acrescentados

Única alteração: fixture `platform_test.c`, dois negativos explícitos HWND==parent e HWND==self. Dez novas assertions: erro/stage/code exatos, count94/2clientes, metadados e ZERO aquisição/fechamento dos observadores, nenhuma retenção. Corpos funcionais e binário manual não modificados.

Debug/Release do único target afetado: PASS, /W4 /WX, zero warnings.68 assertions por configuração (58 anteriores+10 novas), dois CTests PASS. [Debug build](Debug-build.log), [Debug testes](Debug-tests.log), [Release build](Release-build.log), [Release testes](Release-tests.log). Não repetimos protocolo/criptografia/produto ou uma prova TPM. Referências incertas modeladas da suíte antiga mantêm semântica UNKNOWN; fim de processo fixture não é prova de fechamento operacional.

[Preimagem](platform_test.PREIMAGE.c), [alterações/pins](SOURCE-CHANGES.json). Hash atual de fixture é novo; hashes históricos do pedido continuam apontando à sua preimagem preservada, não foram atualizados retroativamente. Executável, pedido, aprovação, certificado, guard funcional e launcher históricos intactos.

## Próximo gate e ED11

Primeira propriedade ausente: associação da topologia real aos papéis administrativos contabilizados, compatível com o contrato da prova. A decisão deve escolher manter e qualificar ambiente de três processos distintos, ou revisar estritamente o binding para identificar/cobrar o auxiliar real sem tratar HWND como autenticação. Nenhuma próxima execução TPM pronta/autorizada; primeiro resolver essa propriedade com observação não TPM e testes apropriados ao contrato escolhido.

E21=PENDING: posse TPM não alcançada, custódia/NV/ledger durável e Q4 continuam independentes. E17/E18 serviço/lifecycle e E19/E20 política/composição real não dependem desta prova; não foram promovidos.4demonstrados/12limitados/6pendentes, E22 derivado. ED11/Phase6 PARTIAL. UNKNOWN, reservas, WIP e HEAD/main preservados; nenhum commit/worktree/retry/limpeza.

OWNER_APPROVAL=USED_NOT_REUSABLE
OWN_HANDLE_QUERY=PASS94_AT_OBSERVATION
TOPOLOGY=DENY_PARENT_WINDOW_ALIAS
ADMIN_BUDGET=NOT_OBSERVED
TPM_CONTEXT=NOT_OPENED
TPM_COMMANDS=0
PROOF=NOT_REACHED
CONTROLLED_REFS=0
RETAINED=false
NATIVE_HOST_GUARD=UNCHANGED
TESTS=DEBUG_RELEASE_68_ASSERTIONS_EACH_PASS
NEXT=REAL_CONSOLE_ROLE_BINDING_AND_ACCOUNTING_CONTRACT_RESOLUTION
