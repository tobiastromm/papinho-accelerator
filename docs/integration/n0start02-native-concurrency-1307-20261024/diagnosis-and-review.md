# 1307 — preparação do fixture e owner do token

## Conclusão e limites

A origem reproduzida é a combinação do owner LAB fixo no descritor com um token de execução do fixture que não pode atribuir esse SID. Não é erro de concorrência: CreateMutexExW falha antes de WaitForSingleObject e antes de PSA.owned/fase1. A mesma implementação canônica passa quando o fixture isolado usa sua identidade própria, mantendo proteção e três ACEs. Não foi identificada necessidade de patch funcional.

O recibo histórico1307 não capturou seu token. Não reconstituímos retroativamente esse token como fato. A reprodução controlada atual registra TOKEN_USER, ausência de impersonation e elevação; whoami /user, /groups e /priv complementam grupos/privilégios. Ela reproduz1307 com o descritor original, e passa alterando somente o SID de owner e sua ACE no escopo do fixture. A inferência causal histórica fica limitada a essa preparação incompatível e reprodução, sem inventar recibo anterior.

## API e argumentos reais

apps/server/server_service_admission_win32.inc: psa_sd (:21) produz o descritor válido O:S-1-5-21-785699550-1440458426-1682014700-1108G:BAD:P(A;;GA;;;S-1-5-21-785699550-1440458426-1682014700-1108)(A;;GA;;;SY)(A;;GA;;;BA).

Em papacc_server_service_admission_enter, CreateMutexExW(:86) recebe SECURITY_ATTRIBUTES(nLength=sizeof,lpSecurityDescriptor=PSA.pending,bInheritHandle=FALSE),nome Local\PACC.Preconsume.Offline.<uuid> no teste,flags0,DesiredAccess=SYNCHRONIZE|MUTEX_MODIFY_STATE|READ_CONTROL(0x00120001). A conversão do descritor passou; a chamada retorna NULL/GetLastError1307 (ERROR_INVALID_OWNER). Primeira falha preservada em IG_DIAGNOSTIC; não há aquisição ou timeout concorrente. Não foi usado o Global mutex operacional.

Token factual da reprodução: S-1-5-21-896877160-3262526087-2538948522-1003 (codexsandboxoffline),não elevado,sem impersonation. Grupos não incluem o SID LAB1108; privilégio disponível registrado é SeChangeNotifyPrivilege. Não houve enable de privilégios,impersonation,alteração de token ou elevação. GetTokenInformation(TokenUser/TokenElevation) e OpenThreadToken(ERROR_NO_TOKEN) foram consultados no fixture,com CloseHandle do token verificado.

Na falha1307: configuração-fixture foi aberta somente para leitura e fechada; descriptor alocado foi liberado por psa_free_sd; mutexNULL,owned0,sem intent/launch criado no modoordinary. papacc_server_service_admission_finish(1) retornou0. Não há recibo de aquisição nem prova de concorrência nesse caso.

A documentação Microsoft descreve a restrição de owner em [Owner of a New Object](https://learn.microsoft.com/en-us/windows/win32/secauthz/owner-of-a-new-object) e [TOKEN_OWNER](https://learn.microsoft.com/windows/win32/api/winnt/ns-winnt-token_owner). A identidade de owner é distinta da posse de sincronização obtida por WaitForSingleObject. Não confundir também a política do operador elevado do LAB com os requisitos deste teste isolado de sincronização.

## Correção exclusiva do fixture

Somente tests/service_cross_admission_win32_test.c mudou em código. O modo opcional isolated-current-owner é permitido apenas com namespace Local\PACC.Preconsume.Offline.,paths absolutos sob isolated deste pacote,sem ..,modoordinary. Fora desse modo, o comportamento histórico é preservado. O adaptador da função de conversão do descritor exige o texto canônico exato conhecido; produz o equivalente de teste com SID do TokenUser real no owner e na ACE do ator. DACL protegida,grupoBA,três ACEs ator/SYSTEM/Administrators,GENERIC_ALL,sem herança,sem principals extras. A conversão é oficial e o código canônico efetua GetSecurityInfo/verificação nativa de owner/ACEs e WaitForSingleObject reais. A máscaraGA aparece mapeada para0x1f0001 no readback,aceita pela verificação existente.

Isso não autoriza o ator do sandbox no LAB e não altera o descritor canônico,binários operacionais,launcher,pins da operação ou contratos accepted. O override só existe na unidade de tradução test-only,que já definia PS_ADMISSION_TEST. Não se desativou verificação nem se substituiu o mutex por mock. Não se criaram arquivos de intent ou launch; CREATE_NEW de dados operacionais não foi executado.

Negativos: isolated-canonical-owner-negative usa o descritor original sem substituição e reproduz1307. isolated-token-divergent injeta expectativa incompatível no adaptador de teste e retorna5 antes da criação; é falha injetada,não troca real de token. A divergência real entre o token sandbox e o owner1108 está documentada e exercitada pelo negativo1307. Não alegamos teste com segundo usuário efetivamente logado.

## Qualificação

Selected/Legacy Debug/Release: quatro builds do fixture com toolchain e flags do pacote anterior,/W4/WX,zero warnings.32 registros de teste,em quatro configurações: aquisição0,peer170/WAIT_TIMEOUT258,fechamento factual0/fase2/mutex0,reaquisição0; Stop0/consume5023/fechamento0; owner inválido1307/cleanup0; divergência injetada5/cleanup0; UNKNOWN preserva fase3/mutex1/owned1,finish5023 e peer170.

Para UNKNOWN, o comando r apenas consulta a tentativa de finish canônica; nenhuma liberação forçada. Após evidência de retenção,o comando x termina naturalmente o processo-fixture como já previsto historicamente pelo teste. Esse fim de lifetime não é quiescência ou cleanup semântico e o nome não é reutilizado/recuperado. Nenhum kill,ReleaseMutex externo,adoção ou tratamento de abandono como PASS. Não restaram processos-fixture na consulta posterior. Os nomes isolados e todos os resultados estão em native-results.json.

O supervisor usa biblioteca padrão Python apenas para conduzir fixtures nativos isolados; não inicia qualificador operacional nem cria autoridade. Uma advertência sintática na primeira versão do supervisor foi corrigida para string raw; compilação sintática sem warnings verificada sem repetir os testes nativos. whoami /all não conseguiu consultar claims; isso permanece limitação do comando agregado,com consultas separadas /user,/groups,/priv e consultas de token nativas preservadas. Não foi tratado como zero claims.

## Pacote e próxima decisão

Pacote operacional anterior,request SHA2569f7443f233c4c08a052186a779e7e60f51b79d68594edab7f4d964ccd8a48f65,script573bf1ca9a95dc052c992a0893b47f06f9dd96a92f53d3aa41cdf5bcc25951c2 e qualificadorb482b959400b41e6e64ecb86858dfd00522a0aa988b36300dc7d70f4d3c33eae permanecem byte-exatos. O bloqueio histórico1307 permanece no closeout original; este relatório reconcilia a nova qualificação sem reescrever história. Nova aprovação específica ainda é necessária. EXACT-OPERATION-APPROVED.json ausente; aprovação condicional antiga preservada e não executável. Não houve LAB positivo,preflight,Start/StopService,SCM/ACL,ETW,rede ou revisão ED11. Limites60s/1s,128/1024,quatro papéis,NONE_GRANTED e autoridades DENY preservados.
