# Topologia real de console â€” evidÃªncia nativa e binding proposto

TASK=PACC-PHASE6-ED11-REAL-CONSOLE-ROLE-BINDING
Pacote iniciado2026-10-27; a pasta preserva a identidade documental da investigaÃ§Ã£o. NÃ£o Ã© nova operaÃ§Ã£o TPM/LAB.

## Resultado tÃ©cnico

Foi encontrado um auxiliar REAL distinto do pai/cliente HWND: conhost.exe PID20144, criaÃ§Ã£o134376272949369467, referenciado por consulta nativa ao handle original do PowerShell17204. O bloqueio nÃ£o demonstra ausÃªncia desse auxiliar: o locator HWND nÃ£o o identifica neste ambiente. Foram implementados um observador nativo e testes reais de console clÃ¡ssico/ConPTY, concorrÃªncia, identidade, fechamento e UNKNOWN. Nenhum cÃ³digo funcional da prova, launcher, contrato accepted ou pin histÃ³rico foi modificado.

O alvo ainda NÃƒO estÃ¡ qualificado para outra prova. AssociaÃ§Ã£o autenticada completa/mapped image permanecem UNPROVEN; a lista de clientes atual do console elevado nÃ£o pÃ´de ser obtida pelo observador nÃ£o elevado. Uma coleta herdada manual SEM TPM estÃ¡ pronta. [ADR focal C0â€“C9 PROPOSED](../../../adr/ADR-CANDIDATE-tpm-proof-explicit-console-host-binding-20261027.md), SHA256 `7dee709121cc9b56633efb6d968bd0935b81bc6c18b000226ed35cf0a179f985`; nÃ£o aceita, sem autoridade operacional.

## Topologia factual nova

[Dados por participante](PARTICIPANT-IDENTITIES.csv), [leituras concorrentes do alvo](FACTUAL-ROLE-AND-ACCOUNTING.json), [saÃ­da1](OWNER-FINAL-CONCURRENT-1.json) e [saÃ­da2](OWNER-FINAL-CONCURRENT-2.json).

| Papel | PID / criaÃ§Ã£o FILETIME | EvidÃªncia atual |
|---|---|---|
| Operador, processo PowerShell seed |17204 /134376272948693901|Handle original, criaÃ§Ã£o antes/depois, sessÃ£o1, TokenUser SID autorizado...1108, elevado, arquivo original e mÃ©tricas|
| Processo associado ao HWND histÃ³rico |17204|O recibo anterior o iguala ao pai; nÃ£o o confundimos com host real. HWND atual do console elevado ainda nÃ£o observado por este coletor|
| Auxiliar candidato real |20144 /134376272949369467|Handle original estÃ¡vel, sessÃ£o1, SID...1108/elevado, parent metadata17204, conhost.exe, arquivo Windows volume417614100/FileID327680:1591236/hashb02ee54f...|
| Ancestral desktop observado |7200 /134375241536646020|explorer.exe, mesmo usuÃ¡rio, nÃ£o elevado; observado separadamente como ancestral/baseline, nÃ£o host do console|
| Prova anterior |10252 /134376273119221101 no recibo histÃ³rico|Processo indisponÃ­vel agora; nÃ£o reconstruÃ­mos seu token/associaÃ§Ã£o ou custo por PID reutilizÃ¡vel|
| Observadores desta investigaÃ§Ã£o |22036 e15780, criaÃ§Ãµes prÃ³prias nos JSONs|Conta do proprietÃ¡rio, nÃ£o elevados; originais distintos e refs fechadas. NÃ£o sÃ£o a prova nem debitados no orÃ§amento administrativo da futura prova|

Os nomes sÃ£o descriÃ§Ãµes, nÃ£o autenticaÃ§Ã£o. Volume/ID/tamanho/hash do arquivo aberto, token/sessÃ£o, referÃªncias nativas e continuidade foram observados. Igualdade de arquivo nÃ£o prova imagem mapeada; assinatura nÃ£o Ã© presumida. O seed esperado foi fixado por criaÃ§Ã£o, nÃ£o somente PID. O seed nÃ£o ganhou autoridade por ser parÃ¢metro do coletor.

NtQueryInformationProcess49 no seed retornou NTSTATUS0,8bytes, raw20145=0x4eb1. MÃ¡scara dos dois lowbits leva a20144, flag1. O handle original do candidato retorna raw17206=0x4336/flag2, metadado reverso ao pai; isso nÃ£o Ã© outro host. NÃ£o seguimos todo PID mascarado indiscriminadamente. Explorer raw6484/flag0 tambÃ©m nÃ£o Ã© host desse console. Raw/status/tamanho foram repetidos sobre os mesmos handles, sem mudanÃ§a de criaÃ§Ã£o ou tÃ©rmino observado.

Essa informaÃ§Ã£o Ã© DISCOVERY METADATA, nÃ£o autenticaÃ§Ã£o. [phnt](https://github.com/winsiderss/phnt/blob/master/ntpsapi.h) identifica a classe e tipo; [libuv](https://github.com/libuv/libuv/blob/v1.x/src/win/tty.c) consulta a classe e separa os lowbits para descobrir conhost. [Microsoft](https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntqueryinformationprocess) adverte que APIs nativas podem mudar (URL correta nos sources abaixo). NÃ£o usamos setter, debug injection, SeDebugPrivilege, substituiÃ§Ã£o de token ou UAC bypass. O campo Ã© metadado configurÃ¡vel, nÃ£o vÃ­nculo criptogrÃ¡fico.

## ContabilizaÃ§Ã£o sem duplicaÃ§Ã£o

Primeira leitura parcial: PowerShell711 + conhost266 =977handles. Duas leituras concorrentes finais: PowerShell618 + conhost266 =884, observadas sequencialmente mantendo handles de ambos vivos.140 atÃ©1024 naquele intervalo, nÃ£o garantia futura. NÃ£o atribuÃ­mos711â†’618 a este patch; o processo alvo permanece dinÃ¢mico. NÃ£o somamos mÃ¡ximos histÃ³ricos ou leituras de relÃ³gios diferentes para criar teto.

Conta-se o operador uma vez mesmo quando tambÃ©m Ã© pai e HWND owner. Conta-se o auxiliar REAL20144 separadamente. O observador/prÃ³xima prova tem orÃ§amento prÃ³prio128 e guard124; seu custo nÃ£o Ã© transferido para administraÃ§Ã£o nem subtraÃ­do da tabela do processo. Os leitores concorrentes fazem observaÃ§Ãµes independentes; nÃ£o se somam884+884 como se fossem quatro administradores.

Explorer3297handles foi medido e aparece no relatÃ³rio, nÃ£o ocultado nem considerado zero. Pela composiÃ§Ã£o B+D, baseline OS/desktop externo Ã© distinto de processos relacionados Ã  operaÃ§Ã£o. Ser ancestral nÃ£o o torna automaticamente actor administrativo; se uma futura seleÃ§Ã£o depender de processo adicional UI/broker como participante, deve identificÃ¡-lo e cobri-lo ou negar. NÃ£o afirmamos ter feito censo completo do desktop ou provado ausÃªncia de todo terminal/broker. NÃ£o excluÃ­mos processo necessÃ¡rio para fabricar PASS. ClassificaÃ§Ã£o/abrangÃªncia do orÃ§amento completo ainda precisa do binding e lista de clientes reais.

Private Bytes e WorkingSet por processo estÃ£o nos JSONs. Threads/stack-VA/custos kernel/transitivos/unsampled peak continuam fora desta prova e nÃ£o recebem crÃ©dito.884 Ã© SUBTOTAL DOS DOIS OBJETOS DESCOBERTOS, nÃ£o orÃ§amento/admissÃ£o ED11 PASS. N7, recursos e freshness aplicÃ¡veis nÃ£o foram relaxados.

## Observador e limites de contexto

[ConsoleRoles.c](ConsoleRoles.c) abre referÃªncias originais, coleta criaÃ§Ã£o/token/sessÃ£o/arquivo por handle, hash streaming, mÃ©tricas e revalidaÃ§Ã£o de associaÃ§Ã£o/lifetime. Slots bounded, reserva de temporÃ¡rio antes da aquisiÃ§Ã£o; nenhuma aquisiÃ§Ã£o apÃ³s freeze; primeiro erro e referÃªncia incerta retidos. ContabilizaÃ§Ã£o por identidade Ãºnica; alias de papel no mesmo objeto reutiliza slot, PID/criaÃ§Ã£o divergente nega. Arquivo pÃºblico current-on-disk nÃ£o Ã© mapped-image attestation. Fonte de autorizaÃ§Ã£o nÃ£o Ã© o JSON; todos os recibos dizem authority=DENY.

O sandbox executa sob outro SID (...1003), explicitamente distinto do operador. ExecuÃ§Ã£o fora dele usa SID...1108, porÃ©m sem elevaÃ§Ã£o; nÃ£o Ã© o token elevado da prova. NÃ£o se autentica operador usando a identidade da conta restrita. Nenhum processo-alvo teve token, handles globais, memÃ³ria ou flags alterados.

Na tentativa de associaÃ§Ã£o externa, antes havia2clientes no console do observador; FreeConsole retornou TRUE/erro0 e depois0clientes. AttachConsole(17204) falhou5; o observador continuou sem janela/clientes desse alvo. [Discriminante integral](OWNER-FINAL-ATTACH-DISCRIMINANT.json). NÃ£o atribuÃ­mos conclusivamente o5 Ã  elevaÃ§Ã£o: a [documentaÃ§Ã£o AttachConsole](https://learn.microsoft.com/en-us/windows/console/attachconsole) tambÃ©m descreve esse cÃ³digo para jÃ¡-associado, e os nossos retornos nÃ£o estabelecem toda a causa interna. NÃ£o contornamos UAC ou tentamos repetidamente anexar o mesmo processo.

O comando final usa console HERDADO, sem FreeConsole/AttachConsole. Precisa ser invocado pelo proprietÃ¡rio no PowerShell elevado jÃ¡ existente para fornecer a evidÃªncia que o contexto do agente nÃ£o alcanÃ§a. NÃ£o hÃ¡ nova aprovaÃ§Ã£o TPM a consumir. A criaÃ§Ã£o esperada17204 impede adoÃ§Ã£o silenciosa de PID reutilizado. Esse Ã© o limite real de observaÃ§Ã£o atual.

## CorreÃ§Ãµes e custos efetivamente encontrados

A verificaÃ§Ã£o opcional Authenticode/offline, aplicada inicialmente ao ancestral Explorer, carregou dependÃªncias e fez o observador atingir213handles. O guard negou; resultado preservado em ACTUAL-ROLE-OBSERVATION-OWNER-CONTEXT.json, nenhuma reduÃ§Ã£o de quota ou atribuiÃ§Ã£o de zero. Essa verificaÃ§Ã£o foi separada da coleta de topologia, que nÃ£o concede autoridade de assinatura: o recibo final marca trust_queried=false/trust=null e mapped image UNPROVEN. NÃ£o foi removida uma verificaÃ§Ã£o obrigatÃ³ria do host de prova â€” esse host e seus pins nÃ£o mudaram. QualificaÃ§Ã£o de proveniÃªncia obrigatÃ³ria permanece gate antes de futura admissÃ£o.

Outro ajuste focal do protÃ³tipo: preservar e interpretar lowbits como metadados tipados em vez de tratar backlink/ancestral como segundo auxiliar. Um host de console do processo ancestral observado Ã© BASELINE_HOST64, nÃ£o auxiliar do seed. A descoberta de hosts de outros contextos fica explÃ­cita, sem duplicar o subtotal do alvo.

Pico prÃ³prio final de coleta fora do sandbox85handles; leitores concorrentes87. Ambos abaixo de124/128 nesse escopo sem TPM. Primeiro protÃ³tipo103 com outros providers e213 no ramo Authenticode nÃ£o sÃ£o apagados. NÃ£o comparar diretamente com94 do host de prova nem declarar economia operacional. Refs originais finais0/retainedfalse; internos/caches/globais permanecem UNKNOWN. AquisiÃ§Ãµes apÃ³s freeze bloqueadas. Falha de fechamento real reteria o processo coletor vivo, sem retry/fechamento forÃ§ado.

## Testes nativos novos

[ConsoleFixtures.c](ConsoleFixtures.c), [Debug](Debug-FINAL-v3-tests.log), [Release](Release-FINAL-v3-tests.log), [resultados estruturados](FINAL-FIXTURE-RESULTS.json).

Quatro builds finais: observador+fixture x Debug/Release /W4 /WX /GS /sdl, zero warnings, MSVC/x64 moderno. NÃ£o reconstruÃ­mos produto/launcher/prova TPM, nem repetimos os antigos testes de alias-denial.

40checks por configuraÃ§Ã£o, dos quais26 do novo modelo de identidade/ownership: identidade Ãºnica/dedup, geraÃ§Ã£o divergente, flags/width/status/overflow da classe49, reserva BUSY, capacidade saturada, Stop antes de adquirir, fechamento incerto injetado sem chamar CloseHandle invÃ¡lido e sem segundo fechamento. O original sintÃ©tico incerto fica retido atÃ© perda do processo fixture; nÃ£o se declara fechamento ou leak-free global por essa perda.

Oito filhos nativos finitos no total (quatro por configuraÃ§Ã£o): CREATE_NEW_CONSOLE, CreatePseudoConsole real e dois leitores/console distintos concorrentes. HÃ¡ referÃªncia original do processo e tÃ©rmino aguardado; nenhum TerminateProcess/kill. Processo encerrado nÃ£o Ã© live, original conhecido fechado. Todos os JSONs finais first=NONE/refcount0/retainedfalse. Pico de cada observador-fixture77, controller87. Console clÃ¡ssico apresentou HWND do prÃ³prio cliente; ConPTY apresentou HWND do host. Isso Ã© evidÃªncia experimental dos ambientes isolados, nÃ£o reclassificaÃ§Ã£o automÃ¡tica do terminal do proprietÃ¡rio.

ClosePseudoConsole retornou; seu retorno Ã© void e fechamento interno nÃ£o Ã© totalmente observado. Pipes/process/thread refs controladas foram conferidas. Dois testes executÃ¡veis adicionais com criaÃ§Ã£o errada negaram2/SEED_CREATION_MISMATCH, refs0: [resultados](NEGATIVE-EXIT-RESULTS.json). Falhas iniciais de build (quoting /Fo e nome bind colidindo com declaraÃ§Ã£o WinSock) corrigidas; builds finais sem supressÃ£o de warnings ou troca de toolchain. Logs de etapas anteriores de coleta e fixtures preservados.

## Arquitetura e prÃ³ximo gate

NÃ£o precisamos criar um auxiliar artificial para substituir um inexistente:20144 existe e foi medido. A alteraÃ§Ã£o necessÃ¡ria Ã© o locator/binding; HWND continua metadado, nÃ£o o identificador do auxiliar. C0â€“C9 propÃµe binding explÃ­cito por originais e descoberta nativa repetida, proteÃ§Ã£o por token/arquivo/sessÃ£o/lifetime, soma Ãºnica de operador+auxiliar, lista exata de clientes e aceitaÃ§Ã£o residual NOVA e restrita ao experimento matemÃ¡tico. NÃ£o se reutiliza a N7 histÃ³rica para uma identidade nÃ£o coberta. Se esse risco nÃ£o for aceito, DENY atÃ© vÃ­nculo autenticado suficiente.

O guard funcional e pedido consumido nÃ£o mudaram. ADR C0â€“C9 PROPOSED SHA256 `7dee709121cc9b56633efb6d968bd0935b81bc6c18b000226ed35cf0a179f985`. Antes de integrar ao host da prova: decisÃ£o explÃ­cita, evidÃªncia de clientes/contexto herdado real, qualificaÃ§Ã£o de todos os recursos exigidos e atualizaÃ§Ã£o prospectiva dos pins. NÃƒO foi preparado novo pedido TPM: topologia real integral ainda nÃ£o qualificada. AprovaÃ§Ã£o4a5f permanece usada.

[Coleta manual read-only preparada](MANUAL-READONLY-HANDOFF.md), executÃ¡vel `438735bb3ea0286fbc8f6b67ca04663f7e990d93cc6db159cb744efc70c20dc8`. O comando usa somente o observador e PID/criaÃ§Ã£o jÃ¡ conhecidos; sem TPM/TBS/NV/Start/Stop/SCM/ACL, sem intent/Q4, sem aprovaÃ§Ã£o consumida. Depois do JSON do console elevado, os testes e a integraÃ§Ã£o tÃ©cnica podem continuar dentro da decisÃ£o normativa efetivamente autorizada, e sÃ³ entÃ£o avaliar um novo pedido TPM.

E21=PENDING, E17/E18 serviÃ§o/lifecycle e E19/E20 alvo composto/D4/D5 independentes, E22 derivado.4demonstrados/12limitados/6pendentes. ED11/Phase6 PARTIAL. HistÃ³ricos/pins/WIP e reservas/UNKNOWN preservados; nada de commit/worktree/limpeza/refund/CURRENT/assinatura/recuperaÃ§Ã£o.

Sources: [Microsoft NtQueryInformationProcess](https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntqueryinformationprocess), [GetConsoleWindow](https://learn.microsoft.com/en-us/windows/console/getconsolewindow), [GetWindowThreadProcessId](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowthreadprocessid), [FreeConsole](https://learn.microsoft.com/en-us/windows/console/freeconsole). A ABI interna e limites de descoberta nÃ£o foram transformados em promessa de autenticaÃ§Ã£o.

CURRENT_TASK_COMPLETED=YES_TECHNICAL_INVESTIGATION_AND_QUALIFICATION_WITH_REMAINING_BOUNDARY_EXPLICIT
REAL_AUXILIARY=DISCOVERED_ORIGINAL_IDENTITY_OBSERVED_NOT_ADMITTED
ADMIN_PAIR_HANDLES=884_OBSERVED_977_EARLIER_NOT_FRESH_BUDGET
ROLE_ACCOUNTING=UNIQUE_PROCESS_OBJECTS_QUALIFIED_IN_FIXTURE
REAL_ELEVATED_CLIENT_LIST=NOT_OBSERVED
CONTRACT_REVISION=C0_C9_PROPOSED_NOT_ACCEPTED
BUILDS=4_PASS_ZERO_WARNINGS
TESTS=40_CHECKS_PER_CONFIGURATION_PLUS_CREATION_NEGATIVES_PASS
TPM_Q4_START_STOP=NOT_EXECUTED
SCM_ACL_MUTATIONS=NONE
NEXT=OWNER_INHERITED_READONLY_EVIDENCE_AND_EXPLICIT_CONSOLE_BINDING_DECISION

Revisão final do fixture: a referência original de evento sob CloseHandle simulado incerto agora permanece em storage estático do modelo até perda do processo, sem reinitialização do slot nem descarte de rastreabilidade. Somente o fixture foi corrigido;40checks/config e os cenários nativos afetados foram repetidos (logs v3). Nenhum objeto operacional ou guard da prova alterado.
