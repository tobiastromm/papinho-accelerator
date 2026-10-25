# Auditoria C nativa â€” ownership, lifetimes e eficiÃªncia

## ConclusÃ£o e alcance

Auditoria estÃ¡tica concluÃ­da, sem alteraÃ§Ã£o funcional, execuÃ§Ã£o, build, preflight ou experimento. Mesma branch/main HEAD913c90cf2328f7939d8f5b927640601903ac6e91. O foco Ã© nosso C no owner/ciclo/serviÃ§o, nÃ£o otimizar PowerShell/Windows. O inventÃ¡rio de sites de API e hashes cobre os arquivos efetivamente lidos; call-graph-and-boundaries.md, ownership-matrix.csv e phase-ledger.csv compÃµem esta revisÃ£o. NÃ£o Ã© verificaÃ§Ã£o formal nem atestado de ausÃªncia universal de leaks em todas as bibliotecas/produto.

NÃ£o encontrei leak efetivamente demonstrado numa execuÃ§Ã£o, double-close ou use-after-close comprovado no caminho normal revisado. HÃ¡ um ramo defeituoso de retenÃ§Ã£o saturada a corrigir/testar, duas duplicaÃ§Ãµes explÃ­citas de validaÃ§Ã£o, amplificaÃ§Ã£o de verificaÃ§Ãµes no censo e trÃªs referÃªncias de cycle_live cuja vida excede seu Ãºltimo uso funcional. A matriz separa essas descobertas de custos obrigatÃ³rios/UNKNOWN. NÃ£o chamar de leak um snapshot que continua acessÃ­vel ao cleanup ou um owner estacionado por UNKNOWN.

Os artefatos03 atuais sÃ³ permitem opt-in read-only; o positivo continua DENY. O ramo positivo aqui Ã© analisado como cÃ³digo existente, nÃ£o operaÃ§Ã£o autorizada/executada. Qualificador e serviÃ§o tÃªm tabelas de handles distintas; SC_HANDLE/HPSS/EVT/marker/LocalAlloc nÃ£o sÃ£o todos um kernel HANDLE somÃ¡vel. ServiÃ§o fechado de qualificaÃ§Ã£o nÃ£o ativa rede/PST/listeners: main.c.api3.inc â†’ ps_main â†’ psr_prepare â†’ host.run/papacc_server_entry â†’ server_run_root_win32 init_closed/poll_closed â†’ runtime_closed/retained. NÃ£o atribuir custos de um ramo produtivo nÃ£o percorrido ao LAB sem prova.

## A â€” defeitos e caminhos de ownership

**A1 â€” saturaÃ§Ã£o do store de fechamento incerto (defeito estÃ¡tico condicionado, nÃ£o leak observado).** scm-registration-lab.c:15â€“16 declara lab_aux_retained[2] e lab_close_aux(h). Se CloseHandle falha com lab_aux_count>=2, a funÃ§Ã£o retorna0 sem guardar h, sem aumentar contador e sem marcar native.retained. O valor local pode desaparecer no retorno do caller. Duas slots nÃ£o demonstram limite universal de falhas subsequentes.

TraÃ§o de risco no ramo positivo: callback protegido â†’ lab_operator_current abre token temporÃ¡rio â†’ CloseHandle falha e store recebe1 â†’ censo retorna erro; main do ciclo, no loop pÃ³s-Start, conserva primeiro e mas ainda tenta ig_observe e pode voltar ao loop enquanto START_PENDING nÃ£o aceita Stop. Nova revalidaÃ§Ã£o pode repetir aquisiÃ§Ã£o/close incerto, preencher2 e depois entrar no ramo de descarte. NÃ£o demonstramos que esse cenÃ¡rio ocorreu, que todo CloseHandle falho ainda representa objeto vivo ou que o Windows normalmente falharÃ¡ nesses handles. O defeito de rastreabilidade no terceiro erro Ã© factual na fonte; sua ocorrÃªncia e leak fÃ­sico sÃ£o NÃƒO demonstrados. UNKNOWN nÃ£o pode ser convertido em zero, objeto invÃ¡lido presumido ou referÃªncia descartada.

Menor correÃ§Ã£o recomendada A1: bloquear novas aquisiÃ§Ãµes pelo owner apÃ³s a PRIMEIRA falha de fechamento, preservar slot/primeiro erro e propagar retenÃ§Ã£o por todos os callers/loop antes de novas observaÃ§Ãµes; qualificar store cheio e erros mÃºltiplos com API-fault fixture existente. NÃ£o simplesmente aumentar array/quota nem fechar novamente um handle incerto. NÃ£o aplicar neste trabalho.

**A2 â€” retornos NtCreateFile inesperados (revisÃ£o/experimento, nÃ£o defeito nativo atingÃ­vel comprovado).** psr_open e cycle_open_relative usam frames locais e testam status<0; sucesso informacional inesperado/pending nÃ£o tem tratamento tÃ£o explÃ­cito quanto o gate de ausÃªncia, que conserva OA/IOSB/name em storage estÃ¡vel. Chamadas usam SYNCHRONOUS_IO_NONALERT. NÃ£o afirmar STATUS_PENDING atingÃ­vel nessas flags sem qualificar o contrato da API/provedor; tambÃ©m nÃ£o aprovar sucesso sÃ³ porque NTSTATUS>=0 no modelo de fault injection. Preparar teste de retornos e propriedade de armazenamento se esse ramo for ampliado. NÃ£o inventar leak ou usar esse ponto para alterar o acesso necessÃ¡rio.

**ProteÃ§Ãµes confirmadas.** bd_close conhece os12slots inclusive unexpected/actor_check/aux_check; originais do adapter e duplicados tÃªm slots separados e os originais sÃ£o anulados apÃ³s CloseHandle bem-sucedido. ig_cleanup encerra borrows de observer/sample ANTES de bd_close; cycle_process continua com cycleowner e nÃ£o Ã© fechado uma segunda vez pelo observer. ig_n0_close precede finish do mutex. Ps_close usa CloseServiceHandle, runtime fecha sete refs e LocalFree de descriptor/SID. psrd_bind/emit mantÃ©m os dois refs do sink atÃ© escrita/flush/close; falha retorna retained/park, nÃ£o limpa por timeout. O handler SCM e console sÃ³ sinalizam atÃ´micos/Stop; nÃ£o executam cleanup concorrente. SERVICE_STATUS_HANDLE e HMODULE borrowed nÃ£o devem receber CloseHandle. Nenhum novo CreateEvent/CreateThread explÃ­cito nesses bridges.

## B â€” redundÃ¢ncia demonstrada de trabalho

**B1 â€” callback protegido chamado duas vezes em uma revalidaÃ§Ã£o N0.** Em server_service_admission_bd_win32.inc, psa_bd_current chama psa_bd_aux_current para N0 (linha85); este retorna PSA.bd.check(d) (47). Se sucesso, psa_bd_current chama PSA.bd.check(d) novamente no retorno86. Nosso produtor vincula ig_n0_current. Ã‰ duplicaÃ§Ã£o de chamada factual, nÃ£o hipÃ³tese sobre Windows. ConteÃºdo da validaÃ§Ã£o pode incluir arquivos, SDs, token, SCM e hashes; mesmo quando content=0, hÃ¡ trabalho protegido repetido.

Candidato mÃ­nimo: consolidar sÃ³ o retorno da ramificaÃ§Ã£o N0 para usar o resultado de aux_current, mantendo callback Ãºnico no fim do caminho nÃ£o-N0. Aux_current deve continuar chamar o contexto quando invocado diretamente por ps_bd_console. Preservar toda a sequÃªncia de identidade/token/sessÃ£o/arquivo/freshness e o ponto imediatamente anterior ao uso. Testar falhas/alteraÃ§Ãµes entre estÃ¡gios e primeiro erro antes de aprovar patch. Economia estrutural demonstrÃ¡vel: UMA invocaÃ§Ã£o de contexto por psa_bd_current bem-sucedido N0; nÃ£o se atribui milissegundos ou diminuiÃ§Ã£o de handles persistentes.

**B2 â€” censo amplifica validaÃ§Ã£o por entrada.** ig_project_census usa snapshot Ãºnico, mas para cada PROCESSENTRY chama ps_bd_census. Este primeiro chama ps_bd_inventory â†’ ps_bd_console â†’ psa_bd_aux_current â†’ ig_n0_current. Antes do loop, o prÃ³prio censo revalida owner/inventory: no ramo normal N0 sÃ£o2callbacks pela revalidaÃ§Ã£o+B1 e1pelo inventÃ¡rio. Para N entradas aceitas sem erro, hÃ¡ N+3callbacks de contexto nesse censo, antes de demais validaÃ§Ãµes externas; nÃ£o Ã© N snapshots, e sim N verificaÃ§Ãµes protegidas sobre o mesmo snapshot.

Na rota read-only content=1, isso inclui releitura/hash de origem/dependÃªncias/inputs/arquivoWindows/polÃ­ticas, alÃ©m de roots/SD/token/SCM. Na positiva, metric_window pode suprimir hashes de conteÃºdo, mas identidade/SD/SCM e chamadas continuam. read_ops3354861/read_bytes3428560081 histÃ³ricos sÃ£o consistentes com carga alta; nÃ£o provam atribuiÃ§Ã£o exclusiva ou nÃºmero de callbacks, pois nÃ£o houve trace por chamada. NÃ£o inferir custo de handles ou leak a partir de bytes lidos.

Alternativa focal: validar inventÃ¡rio/contexto na abertura/fechamento da mesma geraÃ§Ã£o de censo e executar predicado de membership por entrada com snapshot/inventÃ¡rio limitados ao owner, mantendo identidade/revalidaÃ§Ã£o final antes de efeitos. Ã‰ candidato que exige revisÃ£o de seguranÃ§a/freshness e fixture de mutaÃ§Ã£o/concorrÃªncia durante enumeraÃ§Ã£o; nÃ£o autorizado remover checagens por decisÃ£o de performance ou criar cache stale. Fonte comprova repetiÃ§Ã£o; equivalÃªncia de uma versÃ£o agrupada ainda precisa qualificaÃ§Ã£o.

**B3 â€” revalidaÃ§Ã£o dupla no readiness do serviÃ§o.** papacc_server_service_qualification_managers_ready chama psr_current e depois papacc_server_service_runtime_ready, que chama psr_current de novo. Entre ambos muda somente managers_ready apÃ³s checks dos managers. Candidato: uma validaÃ§Ã£o final no ponto comum sem remover checks de estado/Stop. Uma chamada psr_current a menos nesse boundary Ã© contagem estrutural, nÃ£o ganho de handle garantido. Falhas devem continuar preservar o primeiro NTSTATUS/recibo e nÃ£o anunciar RUNNING antes de todos os gates.

**RepetiÃ§Ãµes que nÃ£o sÃ£o removÃ­veis por mera contagem.** Hash de writer antes de fechar e hash do reader reaberto em cycle_copy protegem duas aquisiÃ§Ãµes e intervalo de substituiÃ§Ã£o; nÃ£o excluir automaticamente como duplicado. Token reaberto compara TokenId/AuthId/ModifiedId atual, nÃ£o Ã© equivalente a reler sÃ³ o token antigo. Duplicates do bind fazem transferÃªncia de ownership sem tirar borrowed refs ao caller; acabam com os originais sendo fechados. SÃ£o quatro cÃ³pias transitÃ³rias reais, nÃ£o quatro handles extras estÃ¡veis apÃ³s handoff. Remover DuplicateHandle exigiria contrato explÃ­cito de move/erro e testes de fechamento, nÃ£o uma chamada API mais moderna.

## C â€” reduÃ§Ãµes possÃ­veis depois do Ãºltimo uso

**C1 â€” trÃªs slots de cycle_live.** cycle_token usa-se para snapshot TokenUser/groups/privileges/elevation e DuplicateToken; cycle_access para AccessCheck; cycle_threads Ã© o snapshot de THREAD32. Os usos funcionais acabam dentro de cycle_live. ApÃ³s retorno limpo, sÃ£o lidos somente pelo cleanup, que hoje fecha os trÃªs. A regra main !cycle_stop_requested impede uma segunda validaÃ§Ã£o live normal: nÃ£o hÃ¡ sobrescrita/novo ciclo iterativo desses slots no caminho limpo.

Fechamento local checado apÃ³s TODOS os checks bem-sucedidos pode retirar3referÃªncias nomeadas do intervalo pÃ³s-cycle_live, sem diminuir a inspeÃ§Ã£o. Em erro/close incerto, manter ownership/retention/firstfailure, sem nova tentativa de fechar nem anunciar quiescÃªncia. NÃ£o aplicar a mesma reduÃ§Ã£o aos tokens nativos do SERVIÃ‡O, porque psr_current/psr_acl voltam a usÃ¡-los. Economia de3 refere-se ao conjunto explicitamente mantido, nÃ£o a um resultado medido na tabela do Windows e nÃ£o ao pico dentro de cycle_live.

A reduÃ§Ã£o sozinha Ã© INSUFICIENTE para viabilidade: intent+imagem+processo sÃ£o3persistentes, dando129 condicionado Ã  base126, antes de cycle_live. Snapshot mÃ³dulo jÃ¡ Ã© fechado antes do snapshot threads; nÃ£o existe um quarto snapshot persistente a economizar. Intent Ã© exigido por admission_effect; imagem Ã© revalidada e usada em AccessCheck; processo sustenta identidade/lifetime/Stop/quiescÃªncia. NÃ£o recomendar fechÃ¡-los antecipadamente sÃ³ para passar128.

**C2 â€” SID fixo em psr_acl.** Cada chamada converte o MESMO SID administrativo fixo com ConvertStringSidToSidW e LocalFree. psr_current chama psr_acl em trÃªs slots. Inicializar representaÃ§Ã£o limitada de SID em buffer do caller ou aproveitar SID esperado com lifetime/proteÃ§Ã£o jÃ¡ comprovados pode evitar3alocaÃ§Ãµes/frees de SID por psr_current. Isso Ã© heap, NÃƒO3handles. NÃ£o usar SID textual como nova autoridade ou trocar descriptor RAW1/owner/ACE. O SD retornado por GetSecurityInfo continua nativo e seu LocalFree obrigatÃ³rio.

**C3 â€” buffers sequenciais de endpoints.** cycle_no_endpoints declara quatro unions de65536bytes para TCP/UDPv4/v6, consulta e descarta cada um sequencialmente. Um union alinhado reutilizado poderia reduzir armazenamento DECLARADO em196608bytes; Release pode jÃ¡ sobrepor lifetimes no frame. NÃ£o declarar reduÃ§Ã£o de committed stack sem medir compilador/perfil/Stop/error. A funÃ§Ã£o nÃ£o abre socket explicitamente; custos das consultas de IP Helper sÃ£o UNKNOWN. Sem relacionar essa economia ao limite de handles.

## D/E/F â€” alternativas de API e garantias

| OperaÃ§Ã£o | AvaliaÃ§Ã£o para Windows10/MSVC Selected e Legacy | Custo/garantias/Stop | DecisÃ£o |
|---|---|---|---|
| NtCreateFile relativo vs CreateFile textual | Interfaces atuais funcionam no alvo; perfis Selected/Legacy sÃ£o budgets/toolchain, nÃ£o outra versÃ£o de Windows | RootDirectory e FILE_CREATE/FILE_OPEN preservam pai original; manter flags/no-reparse/SD desde criaÃ§Ã£o e fechar na API correta | E manter; nenhuma troca por modernidade |
| PSS vs Toolhelp/OpenThread/GetThreadContext | PSS Ã© disponÃ­vel no alvo10; SDK8.1+ existente. Toolhelp nÃ£o fornece contexto RSP/stack equivalente; GetThreadContext requer estado adequado da thread, nÃ£o substitui leitura concorrente segura | Remover context flags perderia mÃ©trica obrigatÃ³ria. Suspender threads interfere no alvo/Stop e exige nova prova | D medir custo isolado de PSS, nÃ£o substituir agora |
| OpenThread apÃ³s PSS | Contexto fornece ThreadId/creation; handle vivo permite conferir processo/creation/liveness antes/depois VirtualQueryEx | Remover economiza1temporÃ¡rio nomeado mas perde proteÃ§Ã£o contra thread/PID reutilizados se nÃ£o houver prova equivalente | E manter |
| DuplicateHandle no bind / pseudo-self | Owner atual requer realhandle e owns seus refs | Move em vez de duplicate pode reduzir overlaptransitÃ³rio, mas precisa contrato e partial-failuretest; selfpseudo=-1 rejeitado por PS_BD_ID | D, nÃ£o seguro assumir ganho estÃ¡vel |
| GetProcessHandleCount | Conta tabela do processo; nÃ£o identifica tipo/owner de cada objeto e nÃ£o mede pico | Ã‰ mais direto que somar nossos slots; nÃ£o substituir por GUIresources ou enumeraÃ§Ã£o Nt* nÃ£o qualificada | E manter; complementar sÃ³ em fixture |
| RAW1 GetSecurityInfo(mask7) | API e perfil aceitos; memÃ³ria devolvida requer LocalFree | Buffer do serviÃ§o via GetKernelObjectSecurity Ã© fronteira distinta. SDDL/normalizaÃ§Ã£o/novoAPI nÃ£o garantem mesmos bytes | E manter RAW1, nÃ£o trocar representaÃ§Ã£o para PASS |
| CopyFileEx/CopyFile2/overlapped vs loopReadFile | APIs mais novas nÃ£o provam menos handles. Loop atual4KiB bounded/Stop/readback/SD/identity/exclusive estÃ¡ auditÃ¡vel | NovaAPI poderia abrir refs/worker/alocar internamente e enfraquecer no-overwrite/Stop/order | E manter atÃ© necessidade demonstrada |
| lab_hash1KiB / BearSSL vs CNG | SHA atual usa capsule BearSSL/caller-span, sem BCryptOpenAlgorithmProvider por hash | Mais buffer reduz potencial de syscalls se leituras cheias, mas aumenta frame/Stoplatency. CNG pode carregar provider e refs; nÃ£o hÃ¡ melhoria provada | D medir em fixture, nÃ£o alterar toolchain/quotas |
| CreateEvent em vez de atÃ´micoStop | NÃ£o existe evento prÃ³prio nesse bridge | Acrescentaria handle/possÃ­velthread sem requisito novo | E nÃ£o introduzir |
| OpenSCManager/OpenService | Correto manter SC_HANDLE e CloseServiceHandle | Reabrir em cada etapa aumentaria refs/TOCTOU. Calls podem usar RPC/kernelinterno nÃ£o medido | E/F preservar; nÃ£o computar2SC como2kernel |
| InitializeSid em buffer limitado | API suportada no alvo e mesmo compilador, sem dependÃªncia nova | Precisa preencher subauthorities/validar tamanho e preservar SID exato/owner, sem ACLnormalizaÃ§Ã£o | C2 candidato de heap; nenhuma economia de handles |

ReferÃªncias primÃ¡rias usadas nesta auditoria: [NtCreateFile](https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntcreatefile), [PssCaptureSnapshot](https://learn.microsoft.com/en-us/windows/win32/api/processsnapshot/nf-processsnapshot-psscapturesnapshot), [GetThreadContext](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getthreadcontext), [DuplicateHandle](https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-duplicatehandle), [GetSecurityInfo](https://learn.microsoft.com/en-us/windows/win32/api/aclapi/nf-aclapi-getsecurityinfo), [InitializeSid](https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-initializesid). Sem download/instalaÃ§Ã£o. Compatibilidade de API nÃ£o certifica custo ou aprovaÃ§Ã£o de uma mudanÃ§a; nenhuma alternativa foi compilada/testada neste audit.

## Ledger e contagem: alcance exato

phase-ledger.csv cobre Aâ€“J, ownership-matrix.csv cobre29grupos e named-reference-counts.json explicita os nomes. No caminho N0 com aquisiÃ§Ãµes bem-sucedidas,27 HANDLEs nomeados estÃ¡veis antes de efeitos:6filescomuns+labtoken/aprovaÃ§Ã£o/mutex3+contexto11+BD7. Durante handoff hÃ¡5originais adicionais e2console, produzindo34refs HANDLE nomeadas simultÃ¢neas. DoisSC_HANDLE e tiposPSS/EVT sÃ£o separados, stdio/pseudo/borrowed e recursosinternos excluÃ­dos.

Depois de efeitos, intent+imagem+processo+token/access/snapshot acrescentam6:33nomeados depois de cycle_live, atÃ©34com um thread temporÃ¡rio ou leitura do diagnÃ³stico. ServiÃ§o tem6HANDLEs prÃ³prios de binding estÃ¡vel, podendo coexistir com2diagnÃ³stico e1tokencheck =9nomeados no prepare. NÃ£o sÃ£o todos os recursos de runtime/managers/Windows, nem pico total medido. NÃ£o converter a diferenÃ§a126âˆ’27 em99handles Windows atribuÃ­dos: nÃ£o hÃ¡ atribuiÃ§Ã£o por objeto e observaÃ§Ãµes sÃ£o fases/instantes diferentes.

KNOWN_NATIVE_HANDLE_PEAK=TOTAL_UNKNOWN; SOURCE_NAMED_SIMULTANEITY_AT_LEAST34_QUALIFIER_AND9_SERVICE_ON_DESCRIBED_SUCCESS_PATH. Essa Ã© anÃ¡lise de slots/lifetimes, nÃ£o certificaÃ§Ã£o de maior pico atingÃ­vel em erro ou API. A observaÃ§Ã£o real126nÃ£o Ã© pico. O modelo condicionado de viabilidade126+2/3/6=128/129/132 permanece uma hipÃ³tese explÃ­cita de base estÃ¡vel, nÃ£o limite universal; eliminando3slots nÃ£o resolve129do primeiro monitoramento. O PSS Ã© criado DEPOIS do GetProcessHandleCount da prÃ³pria mediÃ§Ã£o, logo hÃ¡ cegueira do observador sobre seu overhead nessa amostra.

Em erro, mais slots unexpected/checktoken/transient podem coexistir e o conjunto nÃ£o Ã© limitado pelo nÃºmero do caminho limpo. O owner sÃ³ encerra borrows e referÃªncias quando contrato permite; retained/park nÃ£o Ã© DESTROYED. Arquivos persistentes nÃ£o sÃ£o handles vivos por nome, e Close0 nÃ£o significa quiescÃªncia global. H0â€“H9 nÃ£o dispensa anÃ¡lise de lifetimes controlados novos nem exige teto universal de resÃ­duo opaco histÃ³rico.128/1024/quatro papÃ©is/freshness60s1s/headroom/NONE_GRANTED/Stop/retention/exclusividade preservados.

## Prioridade e menor patch recomendado, ainda NÃƒO aplicado

1. Qualificar/corrigir A1: saturaÃ§Ã£o de retenÃ§Ã£o e supressÃ£o de novas aquisiÃ§Ãµes apÃ³s close incerto. Antes de otimizaÃ§Ã£o, usar fixture de fault injection existente para demonstrar primeiroerro/no-doubleclose/no-lostref e STOP/UNKNOWN. A auditoria nÃ£o autoriza implementar.
2. Patch mÃ­nimo de eficiÃªncia comprovada: consolidar o callback duplicado N0 B1, mantendo aux_current para callers diretos e o contexto de legados; testar mutaÃ§Ã£o/replay/Stop/freshness antes/depois. Pode reduzir trabalho pesado, NÃƒO promete economia de handle estÃ¡vel. B3 do serviÃ§o Ã© outro boundary focal, nÃ£o fundir todas as checagens indiscriminadamente.
3. Encerrar tokens/access/thread snapshot C1 no Ãºltimo uso limpo com retenÃ§Ã£o/closecheck, se revisÃ£o dos contratos aceitar lifetimes mais curtos. Economia estrutural de3apenas no intervalo posterior, nÃ£o do primeiro monitoramento. Deve ser qualificada com APIs simuladas e objeto/lifetime, sem LAB.
4. SÃ³ depois, avaliar B2 de censo com validaÃ§Ã£o limitada Ã  geraÃ§Ã£o e critÃ©rios de mutaÃ§Ã£o antes/depois. Ã‰ o multiplicador C mais evidente da carga de hash/SD/SCM; nÃ£o criar cache global ou confiar em processo prÃ©-existente. NÃ£o aceitar PASS por omitir checagem obrigatÃ³ria.
5. Experimentos-PROPOSED e Csource NOT-RUN estÃ£o preparados, mas nÃ£o compilados/executados; custo de PSS e lifetimes positivos devem ser demonstrados antes de habilitar ciclo. Sem outro recorder/launcher/owner ou novo traceLAB. C2/C3/buffers sÃ£o secundÃ¡rios ao ownership e nÃ£o solucionam automaticamente quota128.

Nenhum pacote positivo executÃ¡vel preparado. Se nÃ£o se demonstrar folga para refs obrigatÃ³rios e overhead, DENY antes de consumir; nÃ£o aumentar limite ou fechar seguranÃ§a sÃ³ para passar. APIs/Windows/Powershell nÃ£o sÃ£o culpados por exclusÃ£o: hÃ¡ redundÃ¢ncia nossa comprovada, mas custo total/delta real ainda requer experimento autorizado.

CURRENT_TASK_COMPLETED=YES
NATIVE_C_OWNERSHIP_AUDIT=STATIC_COMPLETED_WITH_SOURCE_CALLGRAPH_LEDGER
DEMONSTRATED_LEAKS=NONE_OBSERVED_RETENTION_STORE_OVERFLOW_DEFECT_BRANCH_IDENTIFIED
REDUNDANT_HANDLES=NO_PROVEN_UNNECESSARY_STABLE_BD_DUPLICATES;REDUNDANT_CONTEXT_CALLS_PROVEN
SAFE_LIFETIME_REDUCTIONS=3_CYCLE_LIVE_NAMED_REFS_CANDIDATE_REQUIRES_QUALIFICATION
API_ALTERNATIVES=JUSTIFIED_MATRIX_NO_SUBSTITUTION_APPLIED
KNOWN_NATIVE_HANDLE_PEAK=TOTAL_UNKNOWN_NAMED_SUCCESS_SIMULTANEITY34_QUALIFIER_9_SERVICE
WINDOWS_INTERNAL_COSTS=UNKNOWN_UNLESS_MEASURED
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_NO_FUNCTIONAL_SOURCE_CHANGE
FIRST_UNRESOLVED_DIVERGENCE=RETENTION_FAILURE_SATURATION_AND_POSITIVE_CONTROLLED_HEADROOM_NOT_QUALIFIED
NEXT=NATIVE_C_FOCUSED_CORRECTION_OR_POSITIVE_CYCLE_REVIEW
