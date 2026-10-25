# Authorized manual capture WPRH-20261023-01

2026-10-23. Owner autorizou UMA captura externa Handle.Light.File e UMA execução manual Prepare; nenhuma execução pelo agente. A aceitação de crescimento ETL sem teto nativo limita-se a esta janela supervisionada30s. Não é admissão operacional nem alteração B4. Segue manual-handoff.md; este documento registra a autorização posterior sem apagar a preparação histórica.

Consulta preparatória atual: três pins WPR/launcher/qualificador coincidem com o handoff; diretório externo ainda ausente; volume C com10926161920bytes livres na consulta. Espaço é observação temporal, não reserva ou crédito; reconsultar imediatamente antes da captura. Nenhum limite mínimo de disco novo foi aprovado. Se o owner não puder supervisionar crescimento/espaço, STOP.

## Organização manual

Preparar dois consoles ANTES de iniciar ETW. A: Windows PowerShell5.1 elevado para controlar WPR; B: console do owner para o comando pinado Prepare, cujo launcher fará UAC conforme topologia existente. Não executar o preflight até A confirmar aquisição factual do trace. Não executar ambos em um bloco automático. B pode ficar aguardando F10 depois do retorno factual; A encerra o trace independentemente da janela B. Não fechar processo/referências incertos.

O limite30s é operacional a partir do sucesso de start, medido monotonicamente; se o estímulo/UAC atrasar, encerrar somente a sessão própria e guardar coleta incompleta. Não repetir. Nenhum watchdog foi criado. Perfil Light pode não fornecer stacks/herança; atribuição continua pendente de ETL.

## A — pré-condições, sem iniciar trace

Executar por etapas, preservar saída e primeiro erro. Não prosseguir depois de falha. Não utilizar estas consultas para reconstruir identidades antigas por PID.

```powershell
$ErrorActionPreference = 'Stop'
$wprHandleRoot = 'C:\Projetos\_tmp\PapinhoAccelerator\wpr-handle-attribution-20261023-01'
$wprHandleExe = 'C:\Windows\System32\wpr.exe'
$wprHandleIdentity = [Security.Principal.WindowsIdentity]::GetCurrent()
try {
  if ($wprHandleIdentity.User.Value -ne 'S-1-5-21-785699550-1440458426-1682014700-1108') { throw 'OPERATOR_STOP' }
  if (-not ([Security.Principal.WindowsPrincipal]::new($wprHandleIdentity)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) { throw 'ELEVATION_STOP' }
} finally { $wprHandleIdentity.Dispose() }
if ([Console]::IsInputRedirected -or [Console]::IsOutputRedirected -or [IntPtr]::Size -ne 8) { throw 'CONSOLE_ARCH_STOP' }
$wprHandleThreadIdentity = [Security.Principal.WindowsIdentity]::GetCurrent($true)
if ($null -ne $wprHandleThreadIdentity) { $wprHandleThreadIdentity.Dispose(); throw 'IMPERSONATION_STOP' }
$wprHandleComputer = [Environment]::MachineName
$wprHandleSha = [Security.Cryptography.SHA256]::Create()
try { $wprHandleComputerHash = ([BitConverter]::ToString($wprHandleSha.ComputeHash([Text.Encoding]::UTF8.GetBytes($wprHandleComputer)))).Replace('-','').ToLowerInvariant() } finally { $wprHandleSha.Dispose() }
if ($wprHandleComputerHash -cne '2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff') { throw 'HOST_STOP' }
$wprHandleBoot = Get-WinEvent -FilterHashtable @{LogName='System';ProviderName='Microsoft-Windows-Kernel-General';Id=12} -MaxEvents 1
$wprHandleBootXml = [xml]$wprHandleBoot.ToXml()
$wprHandleBootTime = @($wprHandleBootXml.Event.EventData.Data | Where-Object {$_.Name -eq 'StartTime'})[0].'#text'
$wprHandleBoot | Select-Object RecordId,TimeCreated,ProviderName,Id
$wprHandleBootTime
if ($wprHandleBoot.RecordId -ne 236441 -or ([DateTimeOffset]::Parse($wprHandleBootTime).UtcDateTime -ne [DateTimeOffset]::Parse('2026-10-23T18:36:34.5000000Z').UtcDateTime)) { throw 'BOOT_STOP' }
$wprHandlePins = @{
 'C:\Windows\System32\wpr.exe'='8c3d49a83ebd8138ffa15da93d25ba1fc44acdec3cb9d729b6f43b55a5ef7ca5'
 'C:\Projetos\PapinhoAccelerator\tools\qualification\scm-instrumented-start-uac.ps1'='60a6dd761fee00143cdf88d7fcf629ad1a47986f41875ab1e8020c641b10bca0'
 'C:\Projetos\PapinhoAccelerator\build\console-proof-20261023\Selected\Release\papacc_scm_instrumented_lab.exe'='e76df9084e62e761bd24e818074c4aed1dcff520b63aa38660275400ed153a51'
 'C:\Projetos\PapinhoAccelerator\build\session0-diagnostic-runtime-integration-20261023\Selected\Release\papacc_server_lab_host.exe'='eda84c0cf91ad1266cee462e62c89b84ffa5873853a0122215d15d666701361b'
 'C:\Projetos\PapinhoAccelerator\docs\adr\DECISION-ed11-single-lab-diagnostic-native-boundary-20261023.md'='e7031bd29c9bd174359ffcc3c9a021a6ad25a0256a5cf03b40c0866d3a5776b7'
}
foreach ($wprHandlePin in $wprHandlePins.GetEnumerator()) {
  $wprHandleHash = Get-FileHash -LiteralPath $wprHandlePin.Key -Algorithm SHA256
  $wprHandleHash | Select-Object Path,Hash
  if ($wprHandleHash.Hash.ToLowerInvariant() -cne $wprHandlePin.Value) { throw 'PIN_STOP' }
}
Get-Item -LiteralPath $wprHandleExe | Select-Object FullName,Length,VersionInfo
$wprHandleSignature = Get-AuthenticodeSignature -LiteralPath $wprHandleExe
$wprHandleSignature | Select-Object Status,SignatureType,SignerCertificate
if ($wprHandleSignature.Status -ne 'Valid' -or $wprHandleSignature.SignerCertificate.Thumbprint -ne '3B77DB29AC72AA6B5880ECB2ED5EC1EC6601D847') { throw 'WPR_SIGNATURE_STOP' }
Get-PSDrive -Name C | Select-Object Used,Free
if (Test-Path -LiteralPath $wprHandleRoot) { throw 'EVIDENCE_EXISTS_STOP' }
$wprHandleAncestor = Split-Path -Parent $wprHandleRoot
while ($wprHandleAncestor) {
  if (Test-Path -LiteralPath $wprHandleAncestor) {
    if (((Get-Item -LiteralPath $wprHandleAncestor -Force).Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0) { throw 'REPARSE_STOP' }
  }
  $wprHandleAncestor = Split-Path -Parent $wprHandleAncestor
}
& $wprHandleExe -profiledetails Handle.Light -filemode
& $wprHandleExe -status
& 'C:\Windows\System32\logman.exe' query -ets
```

Revisar os resultados: perfil deve coincidir com o handoff e coletores pretendidos. Não encerrar sessões de outros usuários; se houver sessão WPR ou possível conflito de collector kernel, STOP para avaliação. Sessões normais do Windows não são por si conflito; erro/acesso negado que impeça determinar compatibilidade mantém UNKNOWN/STOP. Não alterar política para obter acesso. Esta revisão humana é necessária: comandos de listagem não são teste automático universal de conflito.

## A — confirmação, diretório exclusivo e único início

Só depois de todas as pré-condições conferidas:

```powershell
$wprHandleConfirmation = Read-Host 'Digite CAPTURE_WPR_HANDLES WPRH-20261023-01 ONE_TRACE ONE_READONLY_PREPARE NO_SERVICE_EFFECTS'
if ($wprHandleConfirmation -cne 'CAPTURE_WPR_HANDLES WPRH-20261023-01 ONE_TRACE ONE_READONLY_PREPARE NO_SERVICE_EFFECTS') { throw 'CONFIRMATION_STOP' }
if (Test-Path -LiteralPath $wprHandleRoot) { throw 'EVIDENCE_EXISTS_STOP' }
New-Item -ItemType Directory -Path $wprHandleRoot -ErrorAction Stop | Out-Null
Get-Item -LiteralPath $wprHandleRoot | Select-Object FullName,CreationTimeUtc,Attributes
& $wprHandleExe -start Handle.Light -filemode -recordtempto $wprHandleRoot -instancename WPRH-20261023-01
$wprHandleStartCode = $LASTEXITCODE
$wprHandleStartCode
```

Não usar Force, cancel, segundo start ou retry. Se start falhar, preservar código; aquisição parcial deve ser determinada por consulta read-only e ownership antes de qualquer encerramento. Não executar B. Se sucesso factual, iniciar referência monotônica imediatamente:

```powershell
$wprHandleClock = [Diagnostics.Stopwatch]::StartNew()
```

## B — UMA execução read-only

Executar somente após confirmação factual de A e sem atraso que inviabilize a janela:

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\tools\qualification\scm-instrumented-start-uac.ps1' -Mode Prepare -ExpectedScriptSha256 60a6dd761fee00143cdf88d7fcf629ad1a47986f41875ab1e8020c641b10bca0
```

Aceitar UAC. Não usar Launch/ElevatedRun. Guardar primeira falha, IG_QUALIFIER_INSTANCE e caminho do recibo. Win32170 pode continuar ocorrendo e não autoriza contorno/repetição. A criação dos recibos administrativos existentes é permitida no fluxo read-only; arquivo diagnóstico operacional não é criado. Não esperar F10 para encerrar trace em A.

## A — encerramento somente da sessão adquirida

Ao término do estímulo OU ao atingir30s, o que vier primeiro; sem esperar retorno da janela B se isso ultrapassar o intervalo. Uma solicitação de stop pode demorar além da janela e não deve ser repetida.

```powershell
$wprHandleClock.Elapsed
& $wprHandleExe -status collectors -details -instancename WPRH-20261023-01
if (Test-Path -LiteralPath ($wprHandleRoot + '\handles.etl')) { throw 'OUTPUT_EXISTS_STOP_NO_OVERWRITE' }
& $wprHandleExe -stop ($wprHandleRoot + '\handles.etl') 'PACC WPRH-20261023-01 read-only handle diagnosis' -skipPdbGen -instancename WPRH-20261023-01
$wprHandleStopCode = $LASTEXITCODE
$wprHandleStopCode
$wprHandleClock.Stop()
$wprHandleClock.Elapsed
& $wprHandleExe -status -instancename WPRH-20261023-01
Get-Item -LiteralPath ($wprHandleRoot + '\handles.etl') | Select-Object FullName,Length,CreationTimeUtc,LastWriteTimeUtc
Get-FileHash -LiteralPath ($wprHandleRoot + '\handles.etl') -Algorithm SHA256
Get-PSDrive -Name C | Select-Object Used,Free
```

Não prosseguir para hash/arquivo se stop falhar sem resultado factual. Preservar erro original e estado UNKNOWN; não cancelar/repetir nem apagar evidências. Saída existente inesperada mantém sessão própria sob investigação, não autoriza overwrite/abandono. Encerramento ETW não prova liberação universal do kernel nem cleanup do qualificador.

## Evidências e análise

Preservar saídas/códigos, identidade WPR/host/boot, duração real, status/perdas, ETL/hash e recibos do launcher. Nenhum número de handles será fabricado como147. Análise WPA segue o handoff: instância exata, baseline/rundown, create/duplicate/close por dono/tipo/tempo, cobertura e perdas; herança/stacks ausentes continuam UNKNOWN. WPA não instalado por esta tarefa; escolher instalação existente compatível para análise offline. Não retirar limites ou promover admissão antes do resultado.

MANUAL_CAPTURE_AUTHORIZATION=OWNER_APPROVED_SINGLE_EXTERNAL_TRACE_AND_SINGLE_PREPARE
TRACE_EXECUTED_BY_AGENT=NO
PREFLIGHT_EXECUTED_BY_AGENT=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
HANDLE_LIMIT=128_UNCHANGED
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=MANUAL_WPR_HANDLE_CAPTURE_AND_WPA_ANALYSIS

## Falha manual de início reportada pelo owner

Owner executou start Handle.Light/filemode/recordtempto para WPRH-20261023-01 e recebeu `Invalid temporary trace directory`,0xc5586004. Consulta read-only posterior: destino C:\Projetos\_tmp\PapinhoAccelerator\wpr-handle-attribution-20261023-01 inexistente; parent C:\Projetos\_tmp\PapinhoAccelerator existe como Directory. WPR -status da instância e status padrão responderam `WPR is not recording`. Não foi observada gravação ativa nessas consultas; não é prova universal de liberação kernel nem de sessões ETW alheias.

Causa concreta compatível com a rejeição: diretório temporário exigido não foi criado antes de start. O procedimento completo continha New-Item, mas o resumo final de três comandos omitiu essa etapa e podia ser executado isoladamente; corrigir essa instrução antes de eventual nova tentativa. Nenhum diretório criado pelo agente, cancel/stop/start/retry ou preflight executado. Preservar STOP; não interpretar a falha como autorização automática de repetição. Antes de eventual nova execução manual, revalidar gates e criar destino exclusivo conforme roteiro completo. Erro original preservado; ETL/atribuição permanecem pendentes.

## Correção da sequência após instrução do owner

Owner solicitou corrigir e apresentar o que fazer e como, após falha de preparação. Preparar nova execução MANUAL com criação do diretório antes de start; não há repetição automática pelo agente. Manter gates e escopo da autorização externa original; nenhuma autorização operacional, SCM/ACL ou mudança B4. Nesta retomada, o destino permanece ausente e a consulta da instância responde WPR is not recording; pins WPR/launcher coincidem. A afirmação anterior de necessidade de nova autorização não deve provocar pedido circular: a direção atual permite entregar a sequência corrigida para execução manual, permanecendo obrigatório STOP se qualquer gate falhar.

Sequência corrigida: A elevado executa pré-condições do roteiro completo, confirma destino ausente, cria diretório exclusivo e verifica sucesso; só então inicia UMA sessão própria. B executa UMA vez Prepare pinado apenas depois do sucesso de A. A encerra sessão própria ao fim do estímulo ou30s, sem esperar F10. Se criação/início falhar, não executar B nem repetir; devolver saída. Não criar diretório automaticamente pelo agente nem apagar/reutilizar evidências. A criação do diretório é efeito diagnóstico externo explicitamente autorizado, não arquivo operacional.

## Encerramento e descarte solicitado pelo owner

Owner confirmou em console elevado `WPR is not recording` após stop da sessão01. ETL salvo117440512bytes, SHA2568aa6ad6255861b68a2e2c1f6243b5ef0aea54d3d5e8c30d83f2fa824eb34668e. Prepare não executado durante essa coleta segundo relato; não é evidência de handles do qualificador. Owner autorizou explicitamente excluir esta captura e preparar repetição manual. Excluir somente esse ETL após conferir caminho absoluto, tipo e hash; preservar diretório e registros. Nova identidade externa WPRH-20261023-02, sem consumo da tentativa operacional e sem efeitos de serviço. Nenhum retry automático ou início de trace pelo agente.
