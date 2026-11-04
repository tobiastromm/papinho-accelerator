# M1 â€” observaÃ§Ã£o nativa de recursos por referÃªncia SCM, somente leitura

Status: CANDIDATE_NOT_APPROVED_NOT_EXECUTED. Pedido especÃ­fico de UMA coleta
manual de metadados pÃºblicos, nÃ£o sessÃ£o SCQ, admissÃ£o ou decisÃ£o normativa.
NÃ£o executar antes de aprovaÃ§Ã£o explÃ­cita deste texto byte-exato. O prompt atual
proÃ­be SCM: somente o modo self e APIs simuladas foram executados.

## Prova mÃ­nima e fonte

Usar a modalidade --service-resource-observation do censo jÃ¡ existente para
observar o PID reportado pelo serviÃ§o exato PapinhoAccelerator, sem criar ou
iniciar processo. Se serviÃ§o/processo nÃ£o estiver disponÃ­vel, o resultado Ã©
NOT_AVAILABLE/UNKNOWN e a coleta encerra. NÃ£o obter PASS criando participante,
instalando serviÃ§o, promovendo fixture ou executando o servidor manualmente.

Uma consulta sc.exe queryex isolada foi rascunhada anteriormente. Ela nÃ£o mede
recursos. O rascunho QOC-M1-SCM-AVAILABILITY.CANDIDATE.md fica histÃ³rico, nÃ£o Ã© a
seleÃ§Ã£o desta prova. Este pedido reÃºne disponibilidade, aquisiÃ§Ã£o de referÃªncia,
continuidade, contadores e fechamento na MESMA invocaÃ§Ã£o do censo existente.

## Artefato da qualificaÃ§Ã£o, distinto do servidor operacional

Coletor Selected/Release: `C:\Projetos\PapinhoAccelerator\build\phase6-functional-20261028\Selected\Release\papacc_phase6_memory_census_test.exe`.
SHA256: `0bdb2b9e0a906ac8255955afc4ee59d4fc4b82c24b504e5783cccb2ba5f7e987`.
Esse pin identifica bytes do coletor em disco, nÃ£o imagem mapeada atestada ou
instalaÃ§Ã£o publicada. NÃ£o seleciona o papacc_server.exe operacional, que segue
nÃ£o congelado e sem seleÃ§Ã£o solicitada. NÃ£o existe wrapper de grants, novo owner,
serviÃ§o, endpoint, root, ledger ou autoridade paralela.

## Efeitos exatos autorizÃ¡veis

Uma execuÃ§Ã£o manual em Windows PowerShell5.1 x64 jÃ¡ elevado, no equipamento
administrado legitimamente pelo proprietÃ¡rio, com confirmaÃ§Ã£o do hash ANTES da
chamada. NÃ£o contornar UAC. ElevaÃ§Ã£o nÃ£o Ã© custÃ³dia nem aprovaÃ§Ã£o SCQ.

1. Consultar SCM local com SC_MANAGER_CONNECT e abrir SOMENTE PapinhoAccelerator
   com SERVICE_QUERY_STATUS. Sem QUERY_CONFIG, configuraÃ§Ãµes/command lines de
   serviÃ§o, registros ou arquivos potencialmente secretos.
2. Obter status/PID via QueryServiceStatusEx. Exigir own-process e um processo
   vivo em RUNNING ou START_PENDING, sem aguardar outra condiÃ§Ã£o. Ausente, parado,
   compartilhado, PID0 ou consulta negada: primeira falha e encerramento.
3. Abrir a referÃªncia de processo com PROCESS_QUERY_LIMITED_INFORMATION |
   SYNCHRONIZE, sem VM_READ, DUP_HANDLE ou direitos de controle. Reter o handle e
   comparar PID/criaÃ§Ã£o/liveness e status antes/depois de cada coleta. NÃ£o existe
   PID/token futuro fixado no pedido. PID do SCM e handle retido limitam a coleta,
   mas NÃƒO autenticam o runtime: identidade de conta/token/grupos/imagem e a
   associaÃ§Ã£o original SCQ continuam nÃ£o demonstradas por estes metadados.
4. Fazer exatamente8 amostras planejadas de handles/private commit/working set,
   coletor separado e fÃ­sico/commit globais. Registrar QPC/frequÃªncia, intervalo
   total da coleta, custo agrupado das queries e preparaÃ§Ã£o da referÃªncia.
   NÃ£o dormir, fazer polling de readiness, reconectar ou repetir apÃ³s erro.
   O8 Ã© cardinalidade finita da coleta, nÃ£o limite operacional de sessÃ£o.
5. Tentar fechar cada uma das3 referÃªncias controladas uma Ãºnica vez. Close
   incerto mantÃ©m referÃªncias restantes e informa UNKNOWN, sem retry ou liberaÃ§Ã£o
   presumida. NÃ£o afirmar quiescÃªncia do serviÃ§o nem usar tÃ©rmino do coletor como
   prova de fechamento de qualquer operaÃ§Ã£o SCQ.

APIs e direitos mÃ­nimos tÃªm base na documentaÃ§Ã£o Microsoft de
[GetProcessMemoryInfo](https://learn.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-getprocessmemoryinfo),
[GetProcessHandleCount](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getprocesshandlecount)
e [SCM rights](https://learn.microsoft.com/en-us/windows/win32/services/service-security-and-access-rights).
O modo usa Windows moderno observado, nÃ£o prova NT4 pelo nome Legacy do build.

## EvidÃªncia e limites

Somente JSON de contadores/status/PID/criaÃ§Ã£o/relÃ³gios/primeira falha/close.
Sem raw file content, token impersonÃ¡vel, credencial, configuraÃ§Ã£o de serviÃ§o,
ownerAuth, key, material privado ou assinatura. NÃ£o consulta TokenUser/grupos,
certificados ou CA. Sem KSP/CNG/hash de conteÃºdo durante a coleta.

A origem aqui Ã© uma observaÃ§Ã£o Windows sobre referÃªncias de consulta, NÃƒO uma
origem de autoridade SCQ. A saÃ­da nÃ£o preenche os3 seletores NULL nem fabrica
pedido/decisÃ£o operacionais. 15 casos de APIs simuladas/configuraÃ§Ã£o passaram;
a consulta SCM NATIVA nÃ£o foi demonstrada. As mediÃ§Ãµes self nÃ£o sÃ£o mediÃ§Ãµes do
serviÃ§o. NÃºmero de refs conhecido nÃ£o inclui todo custo transitivo Windows.

Validade da aprovaÃ§Ã£o futura: mesma invocaÃ§Ã£o/contexto/pin/alvo/finalidade; sem
reutilizaÃ§Ã£o como autorizaÃ§Ã£o SCQ. NÃ£o foi inventado prazo global. A chamada
Windows sÃ­ncrona nÃ£o tem duraÃ§Ã£o mÃ¡xima demonstrada; uma ausÃªncia de retorno
ou interrupÃ§Ã£o deixa fechamento UNKNOWN. NÃ£o forÃ§ar encerramento, repetir ou
controlar serviÃ§o para obter evidÃªncia. NÃ£o existe exceÃ§Ã£o de envelope para SCQ.

## Procedimento candidato â€” NÃƒO EXECUTAR SEM APROVAÃ‡ÃƒO

```powershell
if ($PSVersionTable.PSVersion.Major -ne 5 -or $PSVersionTable.PSVersion.Minor -ne 1 -or -not [Environment]::Is64BitProcess) { throw 'M1_CONTEXT_DENY' }
$scqM1Identity = [Security.Principal.WindowsIdentity]::GetCurrent()
try {
    $scqM1Principal = New-Object Security.Principal.WindowsPrincipal($scqM1Identity)
    if (-not $scqM1Principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) { throw 'M1_ADMIN_CONTEXT_DENY' }
} finally { $scqM1Identity.Dispose() }
$scqM1Collector = 'C:\Projetos\PapinhoAccelerator\build\phase6-functional-20261028\Selected\Release\papacc_phase6_memory_census_test.exe'
if ((Get-FileHash -LiteralPath $scqM1Collector -Algorithm SHA256).Hash -ne '0bdb2b9e0a906ac8255955afc4ee59d4fc4b82c24b504e5783cccb2ba5f7e987') { throw 'M1_COLLECTOR_HASH_DENY' }
& $scqM1Collector --service-resource-observation
$scqM1Exit = $LASTEXITCODE
'M1_EXIT={0}; SCQ_AUTHORITY=DENY; NO_RETRY' -f $scqM1Exit
```

## STOP e autorizaÃ§Ãµes excluÃ­das

STOP na primeira divergÃªncia de hash/contexto/direitos/status/processo/criaÃ§Ã£o,
consulta incompleta/negada ou close incerto. Erro1060 (serviÃ§o inexistente),1062
(sem estado/processo elegÃ­vel),5 (acesso negado) e falhas de continuidade nÃ£o sÃ£o
intercambiÃ¡veis e nÃ£o significam zero custo. Sem esperas/retry automÃ¡ticos.

NÃ£o autoriza endpoint real, sessÃ£o SCQ, instalaÃ§Ã£o, Start/Stop/control/create/
config/delete, ACL, parent/conta/CA, CURRENT, rede operacional, TPM/NV/Q4,
limpeza/refund, recuperaÃ§Ã£o ou capacidade produtiva. A biblioteca do Windows
consulta seu SCM local existente; nenhum IPC privado novo Ã© criado.

## O que a prova pode e nÃ£o pode fechar

Se houver processo disponÃ­vel, entrega baseline observado da referÃªncia candidata
e do observador, latÃªncia da coleta e disponibilidade global. NÃ£o fornece teto
universal, margem operacional escolhida, custos ativos de endpoint/IO/RNG/hash,
Stop/retidos ou duraÃ§Ã£o global da sessÃ£o. Esses termos precisam de evidÃªncia ou
fundamentaÃ§Ã£o prÃ³pria, sem usar o pico observado como limite.

Se nÃ£o houver processo, nÃ£o haverÃ¡ medida do runtime: o prÃ³ximo fato Ã© sua
indisponibilidade atual, nÃ£o autorizaÃ§Ã£o para criar/iniciar serviÃ§o. A decisÃ£o
de disponibilizaÃ§Ã£o legÃ­tima continua separada. A prova nÃ£o Ã© bootstrap nem
admissÃ£o SCQ. Sem envelope/limites/originais legÃ­timos: pedido SCQ NOT_READY;
E21 PENDING, ED11/Phase6 PARTIAL, autoridade DENY.
