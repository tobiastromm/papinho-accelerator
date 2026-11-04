# Pedido — uma observação read-only das fronteiras do controlador frio

Status: READY_FOR_OWNER_SELECTION_AND_READONLY_AUTHORIZATION / NÃO AUTORIZADO.
Uma execução MANUAL futura, sem retries. Não autoriza executar este pedido agora.
Não é outra sessão E/Q/S e não reutilizaaf8529...67bc9 ou decisãoef8b...e199.

## Finalidade e alcance demonstrável

Distinguir recursos presentes no primeiro despacho frio após CRT daqueles
adquiridos durante owner/console/token/knownfolders, até o ponto equivalente à
consulta que negou136. Não provar retroativamente a origem dos136 históricos.
Não há evento pre-CRT já gravado: o primeiro corte agrupa herdados/ambiente,
loader/CRT/DLLs e console já fornecidos antes de main. Esta observação NÃO pode
separá-los individualmente nem autorizar fechamento por diferença aritmética.
Não substituir entrypoint CRT ou instrumentar globalmente o Windows para fazê-lo.

## Artefato diagnóstico exclusivo proposto para seleção

C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe,
765952bytes, SHA256
714fedbff069af841766d158bd4f833eab6d066e2ebe37c8995aa161d6c3d546.
Uma chamada com SOMENTE --observe-cold-handle-boundaries, nenhum path de request,
decisão, hash de autorização SCQ, limite escolhido, token ou nonce como argumento.
No mesmo owner e executável, não outro utilitário/serviço/canal/root permanente.
Esse artefato do workspace é observador DATA selecionado para diagnóstico,
não executor administrativo confiável para UPDATE/E/Q/S. Não instalar/copiar
para ProgramFiles nem executar qualquer outro entrypoint desta imagem.

DLLs do mesmo diretório, pins reconferidos e retidos por leitura no wrapper:
libssl-3-x64.dll1321472bytes,
3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed;
libcrypto-3-x64.dll7217664bytes,
09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.
Nenhum código administrativo ou aprovação é adquirido do workspace por esse
modo. Hash/lock em disco não é atestação de páginas mapeadas nem de intervalo
anterior ao lock. Alteração do artefato invalida esta seleção, sem reconstrução
silenciosa. A imagem instalada7d29...ca7d/763904bytes permanece intocada.

## Operador e contexto

TCORP-TOBIAS; SID S-1-5-21-785699550-1440458426-1682014700-1108, console
original, Windows PowerShell5.1 Desktop x64 administrativo legítimo. Sem
impersonação, autoelevação, redirecionamento ou alteração de ACL/GPO. Esses
metadados delimitam contexto, não criam autoridade produtiva. O contexto do
token é verificado pela função original personal_token(capture=1), sem promovê-lo
a autorização E/Q/S. Somente aprovação específica deste pedido autoriza a coleta.

## Cinco cortes finitos, sem output entre eles

| Slot | Ponto | Interpretação permitida |
| --- | --- | --- |
|0| Primeira entrada no modo, antes de owner_acquire | Baseline POST-CRT, não handles no instante de criação/pre-loader |
|1| Após owner_acquire | Controle do custo do root/reserva/inicializadores existentes |
|2| Após SetConsoleCtrlHandler | Diferença observada do bridge, não ownership dos internos |
|3| Após personal_token/capture e verificações de grupos | Um TOKEN_QUERY explicitamente próprio; restantes internos não atribuídos individualmente |
|4| Após ProgramFiles/ProgramData DONT_VERIFY, frees dos PWSTR e GetDriveTypeW | Mesmo grupo de preparação imediatamente anterior ao guard original |

Cada corte usa uma única GetProcessHandleCount(GetCurrentProcess()). São cinco
consultas no total em êxito; parar na primeira falha/Stop. O modo NÃO chama
scqc_open_raw nem o guard operacional: sua última consulta equivale à fronteira
pré-open, não é a consulta histórica e não é seguida de aquisição de referência.
Não acrescenta outra query final ou repete uma consulta falha. Nenhum número
observado configura capacidade ou dispensa124/128. Valores são totais completos.
known0/error não é contagem zero. Output só após amostragem e fechamento.

Depois do slot4, somente fechamento do token próprio e unregister do bridge,
owner_finish e evidência escalar. Não fechar stdin/stdout/stderr, pseudo-handles,
handles herdados/loader ou internos por conveniência. Buffer knownfolder é memória,
CoTaskMemFree não prova liberação de handles internos do shell.

## Quantitativos e custo do observador

Array de5 registros conhecidos/count/error DWORD =60bytes fixos, sem heap,
enumeração de tabela de handles ou duplicação por registro. O mesmo root cobra
esses60bytes adicionais à reserva conhecida, sem elevar3MiB/2MiB. Censo estático
Selected2001440/Legacy1832872 permanece inalterado; array é automático separado,
não custo zero. Frames/CRT/stack transitiva/provider/kernel são distintos de
sizeof e permanecem sem máximo nativo demonstrado. Nenhum teto novo inventado.

Amostragem não adquire HANDLEs próprios: usa pseudo-handle atual. O caminho
observado adquire um TOKEN_QUERY real, retido até close factual. Console APIs,
grupos e knownfolders podem inicializar estado nativo opaco. Memória/handles
internos e efeitos de cache/loader/console do próprio observador não têm ceiling
demonstrado. O wrapper mantém3 streams no processo pai; não subtrair esses ou
outros handles de nenhuma contagem do filho. Criação do processo/CRT altera
o cenário: não tratar diferenças temporais como uma decomposição exata de136.

Risco read-only a reconhecer na aprovação: custos nativos opacos, pressão do
host entre amostras, latência de APIs e cancelamento não garantidos; console/
token/path queries não são efeito filesystem autorizado. Não importar reduções
de garantia de UPDATE/staging/SCM/Start como capacidade demonstrada desta coleta.
Não medir memória/host/objetos globais ou impor outros números por inferência.

## Stop, encerramento e proibições

Não abrir originais/arquivos administrativos, CSPRNG, SCM, processos de peers,
endpoint/evento SCQ, KSP, rede ou serviço. Não chamar qualquer presenter de ato.
Sem escrita de arquivo de resultado: evidência somente console, enviada pelo
proprietário. Nenhuma criação protegida, staging/UPDATE, rename/delete/restore,
SCM/ACL, Start/Stop, CA/CURRENT, TPM/NV/Q4, limpeza/refund ou recuperação.

Stop preserva novos passos conforme bridge existente; falha de query não é
insuficiência presumida. Fechar somente referências próprias uma vez. Em close
incerto reter owner, imprimir close_known0 e estacionar; não matar/limpar/retry.
OS_QUIESCENCE e coortes históricas permanecem UNKNOWN mesmo se close_known1.
Nenhum limite de prazo de retorno/cancelamento é garantido.

## Procedimento preparado e evidência esperada

observe-cold-boundaries.MANUAL.CANDIDATE.ps1, SHA256
b68ba77cce2144bf1755ac48032c733604b0bfb181e9781994c89cc94f93a87c.
Parsing AST PASS; NÃO EXECUTADO. Verifica hashes do pedido, de si, exe e DLLs,
retém arquivos apenas leitura, invoca o modo exato uma vez e fecha streams.
Após autorização, preparar comando manual literal com os hashes reais.
Não emitir comando operacional antes da decisão específica.

Cinco COLD_BOUNDARY slot/known/handles/error e COLD_BOUNDARY_RESULT first/
close_known/sample_bytes; exit0 só significa coleta encerrada, nunca capacidade.
Reconciliar campos e primeira falha/fechamento, sem inferir contagem em slots
não alcançados. Se slot0 já exceder124, não fechar recursos com base apenas nisso:
o grupo anterior a main permanece sem ownership individual. Se crescer numa fase,
isso localiza fase provável, não prova vazamento ou dispensabilidade. Variações
assíncronas e custos do observador impedem causalidade absoluta por simples delta.

Qualificação offline:4builds Selected/Legacy Debug/Release PASS/zero warnings,
12pares CTest afetados PASS, cold_open32casos/115checks inclui observador136,
falhas de console/token/path/query/Stop e ausência de opens/atos. Nenhuma prova
Windows nova foi executada. Não há quota/contrato alterado ou fechamento seguro
demonstrado pelos modelos. Autorização futura é SOMENTE desta coleta read-only.
