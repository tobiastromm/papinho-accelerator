# Pedido — consulta focal do requisito de acesso ao controlador WS3M

CANDIDATE / NOT_AUTHORIZED / NOT_EXECUTED. Uma única observação MANUAL read-only, sem retry, sem repetir E/Q/S e sem encerrar participantes. A consulta anterior aba0cba004b31a231c2b3ac68c4ca1431300d95b84be86f13e609203141e0525 está consumida: mostrou runtime19836 START_PENDING, controlador19860 ainda vivo e configuração SCM esperada. Esta é uma consulta distinta, com SDs e token atuais que não foram coletados naquela observação.

## Pergunta factual e limites da conclusão

scqb_open_mode em personal_scq_bootstrap_win32.inc, antes de ler o primeiro pacote, abre processo do administrador por PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE e seu token por TOKEN_QUERY. No Windows o direito de abrir token é confrontado com a DACL; a conta virtual não recebe tais direitos por estar no mesmo serviço/pipe ou conhecer PID. A API não garante acesso entre contas. Os modelos não demonstraram esse direito sob as DACLs reais. Uma negação ali é compatível com runtime retido e timeout do controlador, mas NÃO foi observada diretamente; não declará-la causa histórica.

Observar o token atual do runtime e SOMENTE owner/DACL do objeto de processo e do token do controlador, para confrontar esses dois direitos necessários. Não executar AccessCheck/impersonação, simular caller real como prova nativa, abrir recurso sob token virtual, alterar DACL, habilitar SeDebugPrivilege ou promover ausência de Administrators/descritor/hash/DATA a autoridade. SDDL/valores observados são dados atuais, não prova completa de acesso efetivo ou reexecução da abertura original. DACL atual pode não ser idêntica à da falha. A causa1460 é timeout da espera; primeira causa interna do receptor continua UNKNOWN.

## Artefatos

Pasta C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101.
Fonte ws3m-controller-query-access.CANDIDATE.cs SHA25684c53131253556c47124f799b2395ea6431b0f4f0c42402dde5db8585b752999.
Procedimento observe-ws3m-controller-query-access.MANUAL.CANDIDATE.ps1 SHA256572a8f36c9b23816b1b22f21f5b17e76122fa398950e90999c3f59002cbcec1d.
O wrapper verifica seus bytes, este pedido e a fonte, contexto original e ausência de impersonação antes de chamar o observador. Não executado nesta preparação. Reutiliza o observador V3 corrigido; acresce apenas leitura de owner/DACL dos dois objetos, direitos READ_CONTROL necessários para isso e os alvos fixos atuais. Sem novo serviço/owner/root/canal/protocolo/instalação.

## Contexto e alvos fechados

Host TCORP-TOBIAS, Windows PowerShell5.1Desktop AMD64 elevado, console não redirecionado, operador S-1-5-21-785699550-1440458426-1682014700-1108, sem impersonação/autoelevação. Confirmar uma única vez OBSERVE_WS3M_CONTROLLER_QUERY_ACCESS_ONCE após autorização específica, sem resposta automática.

Controlador fixo19860/criação134381311017119087; TokenPrimary/groups17/TokenUser do operador/token164979794:0/auth570063:0/modified164979462:0, conforme plano original. Runtime somente o PID19836 retornado por QueryServiceStatusEx para o serviço fixo PapinhoAccelerator em START_PENDING2/OWN_PROCESS16/códigos0. PID diferente, STOPPED/RUNNING ou divergência => negar sem perseguir outro objeto. Não reutilizar6120/10080/21484/18000. Nome de imagem dos dois deve coincidir com C:\Program Files\Papinho\Accelerator\papacc_server.exe. Imagem instalada contextual798720/ID417614100:4128768:327703/pin6ad6e378ea5ec074ad6af44327b6fd1a22880a8cf18a8f02a3ecc28baf0edb9e; o observador não reabre o EXE nem atesta páginas mapeadas.

Runtime por TokenUser virtual selecionado S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099, tipo primário, PID/criação/liveness e identidade atual obtidos por handles retidos. Criação deve ser posterior ou igual à do controlador. PID só localiza; estas são referências do observador, não as originais internas do owner. Revalidar criação e TokenId/AuthenticationId/ModifiedId/grupos pelos mesmos handles e exigir segundo status SCM com mesmo estado/PID. Divergência => primeira falha/UNKNOWN, sem fallback.

## Direitos e consultas exatos

SC_MANAGER_CONNECT1/SERVICE_QUERY_STATUS4; sem alteração SCM. Controlador: OpenProcess0x00121000 (QUERY_LIMITED|SYNCHRONIZE|READ_CONTROL), OpenProcessToken0x00020008 (TOKEN_QUERY|READ_CONTROL). Runtime: OpenProcess0x00101000 (QUERY_LIMITED|SYNCHRONIZE), token TOKEN_QUERY8. READ_CONTROL é usado apenas no observador administrativo para ler descritores do controlador; não é concedido ao serviço. Não há PROCESS_DUP_HANDLE/TERMINATE/VM_READ/VM_WRITE ou TOKEN_DUPLICATE/IMPERSONATE/ADJUST.

GetKernelObjectSecurity(controller.process,5,buffer16384) e idem controller.token: owner1+DACL4, duas chamadas fixas sem probe/retry/redimensionamento; sem SACL e sem ACCESS_SYSTEM_SECURITY. Sucesso/comprimento20..16384/self-relative são exigidos; RawSecurityDescriptor valida representação, ACL até256ACEs ou null registrada como null, output owner/flags/SDDL owner+DACL. Comprimento inválido, formato inválido, acesso negado ou outro erro => primeira falha/UNKNOWN, sem alterar permissões. Não transforma DACL null/ACE/SDDL em aprovação.

Reutiliza TokenUser/Statistics e GetProcessId/Times/QueryFullProcessImageName/Wait0 para correlação. No token do runtime: TokenElevation20/Type18, Groups2/RestrictedSids11, HasRestrictions21, Session12, Privileges3 com nomes locais, Integrity25. Escalares20/18/12 capacidade4/retorno4;21 capacidade4/endereço alinhado4/retorno1/leitura só byte0. Demais classes buffer8192, verificações de comprimento/count/pointer/SID, sem truncar ou ampliar. Controller consultas básicas buffer512. Não interpreta tipoDefault como ausência de elevação, não assume bit0 ou privilégio habilitado/desabilitado por nome.

Capturas escalares provisórias são marcadas como tais; detalhes/descritores só são publicados após a revalidação e segundo SCM. Mesmas garantias V3 de primeira falha/captura de comprimento original preservadas. Falha de impressão não mascara falha nativa. Nenhum ack/query/SD/token torna SCQ admitido, bilateral ou pronto.

## Custos, efeitos e proibições

Até6handles explícitos (SCM/serviço/2processos/2tokens), buffers512/8192 e um transitório SD16384 sequencial, strings/SDDL/compilador/hash/RPC/kernel/alocador opacos. Não importar G3/U/B/R ou alegar teto nativo/capacidade. Não consulta recursos de memória/handles do serviço. Lookup de privilégio é local. Add-Type pode usar temporários do compilador, sem arquivo protegido instalado. Libera buffers e tenta fechar referências próprias uma vez; close do observador não é close SCQ/application ou quiescência OS.

Proibidos: Start/Stop/kill, nova E/Q/S, endpoint/pipe, UPDATE, SCM/ACL mutantes, leitura/escrita de memória/debug/impersonação/token duplicado ou ajustado, CA/CURRENT/KSP, rede operacional, TPM/NV/Q4, cleanup/refund/rollback/restauração/recuperação ou produção. Preservar EXEs/originais/pins/históricos/UNKNOWN/reservas/PST0.6.4. Os participantes atuais não podem ser terminados por disposições antigas.

## Qualificação e reconciliação

Fonte real apenas compilada, RunFixed não chamado.50cenários modelados,20verificações de capacidade escalar,1alinhamento,3preservações de erro e14APIs modeladas PASS, inclusive SD nativefail/oversize/representação inválida, READ_CONTROL mínimo somente no controlador, SID/PID/token/SCM divergentes, cancelamento/replay/closeUNKNOWN e campos escalares preservados. Wrapper AST PASS, nenhuma coleta real. Evidência WS3M-CONTROLLER-QUERY-ACCESS-OFFLINE-QUALIFICATION.json.

Após uma execução manual autorizada, exigir saída integral/consumo/primeira falha/completude/close e confrontar DACLs com o token atual e máscaras concretas das duas aberturas. Não inferir primeira causa histórica apenas por incompatibilidade atual, nem declarar acesso suficiente a partir de uma allowACE. Resultado positivo do observador não permite UPDATE/Start/kill. Se houver correção coberta pelos accepted, qualificá-la offline; se uma mudança de acesso/protocolo for realmente necessária, apresentar somente a especialização focal indispensável antes de implementá-la. Sem relaxar guards ou criar infraestrutura.

Fontes primárias: https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocesstoken (direitos do processo e DACL do token); https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-getkernelobjectsecurity (owner/DACL/self-relative/READ_CONTROL). Nenhuma fonte pública comprova a falha histórica específica desta sessão.
