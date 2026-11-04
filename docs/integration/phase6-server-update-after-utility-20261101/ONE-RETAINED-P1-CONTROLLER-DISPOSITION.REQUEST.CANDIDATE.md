# Pedido operacional focal — disposição excepcional do controlador P1 retido

CANDIDATE / FOR_OWNER_DECISION / NOT_AUTHORIZED / NOT_EXECUTED.
2026-11-02. Uma única execução MANUAL, sem retry, em outra janela original
Windows PowerShell5.1Desktop x64/elevada no host TCORP-TOBIAS.
Operador original SID S-1-5-21-785699550-1440458426-1682014700-1108.
Não é decisão emitida, ADR accepted, cleanup SCQ nem autorização de UPDATE/Start.

## Decisão exata solicitada

Autorizar excepcionalmente o abandono desta sessão falhada e, somente após todos
os gates abaixo e confirmação atribuída, UMA chamada TerminateProcess no mesmo
handle do controlador original, seguida de prova de término. Não encerrar a
janela inteira ou os PowerShell pais; não atingir filhos ou qualquer serviço.
Aceitar perda do estado volátil do controlador e ausência de clean close SCQ,
sem converter UNKNOWN em PASS. Aprovação precisa vincular este pedido byte-exato;
"continue" ou autorização consumida de E/Q/S não a substitui.

## Por que é a menor ação disponível

Código instalado retém em Sleep(INFINITE), não-alertável, após ERROR5/CLOSE0/
RETAINED1. Não existe comando administrativo de fechamento dessa instância.
O handler Ctrl+C apenas sinaliza Stop e não retoma essa espera. Fechar janela
é menos seletivo e pode afetar os PowerShell pais. Injetar código/debugger,
fechar handles remotos à força ou criar canal de manutenção seria mais amplo.
Reboot afetaria todo o host e não provaria close. Nenhuma dessas alternativas
está autorizada ou será executada. A ação proposta termina apenas este processo.

## Originais e proveniência

Sessão utilizada:2a34a063563cd9fdf12ec5742b6c6d45ede377fa4501b104779c33205e31cfc0.
Decisão59f2e58d663bea8646f40d8e3a08c0ec5bddea9e5bfe48774464771658c792a5.
Tentativa241a0c96391265c83a5482c3f178c4f0367e68908b4d80c10eea6b994caf8082.
Originais417614100:1179648:127916 e417614100:983040:127922 preservados.
Log parcial preservado, consulta read-only559b7944... utilizada sem retry:
SCM STOPPED/PID0, configuração esperada, PID148 criação original ainda vivo.
Não é snapshot fresco futuro. Primeiro erro5 permanece UNKNOWN. Nenhuma nova
sessão pode reutilizar esses atos; este abandono não os devolve ou rearma.

## Gates e sequência fixa

1. Conferir hashes do pedido, procedimento, código C# e decisão REAL nova;
   validar host, Windows PowerShellx64 original, operador/elegibilidade e ausência
   de impersonation. Não autoelevar, alterar ACL ou usar SeDebugPrivilege.
2. Adquirir UMA referência ao PID148 com PROCESS_QUERY_LIMITED_INFORMATION |
   SYNCHRONIZE | PROCESS_TERMINATE (0x00101001), não herdável. Não enumerar
   processos, não procurar outro PID e não reabrir se falhar.
3. GetProcessTimes e GetProcessId no mesmo handle devem coincidir com
   PID148/FILETIME134381137375135845 (high31288046/low3049392229).
   QueryFullProcessImageName deve coincidir com o caminho abaixo.
4. Token original por TOKEN_QUERY: TokenUser igual ao SID do operador;
   TokenStatistics Primary1, groups17, TokenId101453194:0,
   AuthenticationId570063:0, ModifiedId101452861:0. Deltas ou indisponibilidade
   negam; não flexibilizar por conveniência. Fechar referência própria de token.
5. Consultar somente status SCM por CONNECT e SERVICE_QUERY_STATUS: serviço
   PapinhoAccelerator, WIN32_OWN_PROCESS16, STOPPED1, PID0. Sem Start/Stop ou
   consulta/reconfiguração de ACL/configuração. Divergência nega.
6. Abrir imagem em FileAccess.Read/FileShare.Read, sem substituição/overwrite;
   reter o handle durante a submissão. Verificar identidade, atributos sem
   reparse/diretório, um link, tamanho786944 e SHA256, sem alterar seu descritor.
   Imagem: C:\Program Files\Papinho\Accelerator\papacc_server.exe.
   Identidade:417614100:4718592:287758.
   SHA256:bf2f8c67a3f5de3a5a4b88f28b69eb8573aec04db1df8e36c7907f999a6a748a.
   Pin em disco não é atestação das páginas mapeadas; PID/criação/token e
   proveniência anterior são verificações independentes, não grant pelo hash.
7. Apresentar plano e exigir digitação independente e manual:
   DISPOSE_RETAINED_P1_148_134381137375135845_ONCE.
   Nenhuma resposta fornecida automaticamente. Revalidar criação/imagem/token,
   status STOPPED/PID0 e processo ainda vivo pelo MESMO handle antes do efeito.
8. Consumir antes de UMA submissão TerminateProcess, exitcode0xE0000001,
   marca de abandono forçado e nunca sucesso diagnóstico. Retorno falso ou erro:
   não repetir, preservar saída e UNKNOWN. Processo já saído: não agir por PID.
9. UMA WaitForSingleObject por até60000ms no mesmo handle. Retorno deve ser
   WAIT_OBJECT_0; consultar exit code e exigir0xE0000001. Sucesso da submissão
   sozinho, timeout ou consulta falha não prova término. Não reenviar, matar
   pais/filhos ou acionar Stop se a espera não concluir.
10. Consultar novamente status SCM e fechar somente referências do observador.
    Erro de fechamento preserva UNKNOWN. Enviar saída desta janela e também
    a saída final da janela original, caso o wrapper retorne após o abandono.
    O wrapper antigo provavelmente reportará falha por exitcode não zero;
    isso não altera o erro5 original nem permite retry.

Os60000ms são tolerância operacional de espera proposta à decisão, não garantia
nativa, deadline G3 ou prova de capacidade. Queries são sequenciais, sem lease
atômico; a identidade do objeto de processo é fixada pelo handle retido.
Nenhum argumento permite selecionar outro alvo, perfil ou exitcode.

## Efeitos e riscos reconhecidos

Terminar threads do controlador pode cancelar I/O e liberar suas referências
pelo OS APENAS quando a saída for realmente observada. Não haverá handlers de
close SCQ ou detach das DLLs garantidos. Objetos com referências em outros
processos podem continuar vivos; o processo kernel também pode continuar como
objeto referenciado. Filhos não são terminados. Não há certeza global de ausência
de borrowers, I/O ou efeitos externos. Possível falha/indefinição depois da
submissão conserva UNKNOWN, sem rollback. A consulta anterior não atestou estado
interno completo. A condição pré-Start é inferida do código qualificado/log e
corroborada por SCM parado, não prova universal de ausência de efeitos históricos.

A ação não apaga arquivos, originais, .previous, candidatos, dados ou reservas
persistentes; não modifica DLLs/SCM/ACL. Fechamentos automáticos de handles da
instância terminada são efeito residual explícito de abandonar esse processo,
não clean close do protocolo. Não emitir receipt/close ou liberar reserva
contábil de outro owner por inferência. Histórico fica FAILED/ABANDONED, nunca
QUALIFIED. Causa5, retenção histórica e quiescência OS continuam UNKNOWN.

Compilação C# transitória pelo Add-Type e CRT/.NET/SHA/kernel/RPC/SCM têm custos
opacos e podem criar artefatos temporários normais do compilador. Não se instala
utilitário/serviço/owner/root/canal nem distribuição; nenhum formato produtivo
ou persistido muda. Não importar G3/U1/R0 envelopes para este procedimento.
Nenhum ceiling de tempo/handles/memória ou cancelamento garantido é alegado.

## Confronto normativo

G5 accepted prevê decisão posterior específica do proprietário e declara
"não há autoridade de terminar processo, refund, cleanup ou recuperação implícita".
Este pedido solicita APENAS a primeira dessas autoridades, explicitamente, para
este controlador identificado, sem autorização de refund/cleanup/recuperação.
G7 e F6/OHI/SSO-B S5 continuam impedindo declarar fechamento legítimo, devolver
consumo ou substituir prova de quiescência por serviço parado/controlador fechado.
Não altera essas cláusulas nem implementa liberação geral de RETAINED_UNKNOWN.
É disposição excepcional manual do host admitida para decisão em G5, distinta
de sucesso ou retomada/replay da operação. Se o proprietário não aceitar essa
perda de garantias e os riscos residuais delimitados, manter DENY e o processo.
Não registrar ACCEPTED normativo ou inventar aprovação antes da decisão real.

Fontes primárias técnicas:
https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-terminateprocess
https://learn.microsoft.com/en-us/windows/win32/procthread/terminating-a-process
https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle

## Artefatos e qualificação offline

retained-p1-disposition.CANDIDATE.cs SHA256 8e4f4c1929e7acf812f139d1b8eb47490fe5b705f7e6763fe612e5d013df9591.
dispose-retained-p1-controller.MANUAL.CANDIDATE.ps1 SHA256 002497df6d4b4851aa24563399124f8a9ad604bae30256bb58cc1329f62c24d2.
Corpo RunFixed e gates reais qualificados em23 cenários do modelo, com todas
as15 fronteiras nativas substituídas e console/arquivo/hash simulados. Negativos
incluem PID/criação/token/imagem/SCM divergentes, falta de confirmação, serviço
mudando, saída anterior, falha de submissão, timeout, exitcode divergente,
fechamento incerto e replay; no máximo uma submissão em cada tentativa.
Compilação do adaptador real feita SEM chamar RunFixed; wrapper somente AST.
Nenhuma coleta ou TerminateProcess real foi chamada nesta preparação.

## Próximo passo após decisão/saída

Somente após aprovação específica registrar a resposta/escopo e SHA256 real
em OWNER-RETAINED-P1-CONTROLLER-DISPOSITION-APPROVAL.txt, preparar comando com
três pins reais e aguardar execução manual. Não fornecer placeholder executável.
Após saída: distinguir submissão/exit observado/fechamento próprio/SCM,
preservar todos os UNKNOWN e NÃO iniciar UPDATE ou E/Q/S automaticamente.
Término observado desta instância é evidência localizada de disposição, não
quiescência OS ou resolução de outra coorte histórica. UPDATE U1–U7/U2 do
candidato787968/a2626bf759cfe5e10f7add6515be2e2b8523c8cdc86bf6d49773010a44bf8a94
exige seu pedido byte-exato e aprovação próprios após reconciliar impedimentos.
Nova E/Q/S também independente; não reutilizar originais/autorizações usadas.

Sem Start/Stop, endpoint/SCQ, UPDATE/staging, alterações SCM/ACL/CA/CURRENT,
rede operacional/TPM/NV/Q4/limpeza/refund/rollback/restauração ou outro alvo.
Preservar HEAD/WIP, pins, históricos, EXEs e documentos, UNKNOWN e PST0.6.4.
