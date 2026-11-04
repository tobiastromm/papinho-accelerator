param([Parameter(Mandatory=$true)][string]$ExpectedScriptSha256)
$ErrorActionPreference='Stop'
if ((Get-FileHash -LiteralPath $PSCommandPath -Algorithm SHA256).Hash.ToLowerInvariant() -ne $ExpectedScriptSha256.ToLowerInvariant()) { throw 'SCRIPT_HASH_DENY' }
if ($PSVersionTable.PSVersion.Major -ne 5 -or $PSVersionTable.PSVersion.Minor -ne 1 -or ![Environment]::Is64BitProcess) { throw 'WINDOWS_POWERSHELL_51_X64_REQUIRED' }
$identity=[Security.Principal.WindowsIdentity]::GetCurrent()
try {
 $principal=New-Object Security.Principal.WindowsPrincipal($identity)
 if (!$principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) { throw 'ALREADY_ELEVATED_OWNER_CONSOLE_REQUIRED_NO_AUTOMATIC_ELEVATION' }
} finally { $identity.Dispose() }
$collector='C:\Projetos\PapinhoAccelerator\build\phase6-functional-20261028\Selected\Release\papacc_personal_native_test.exe'
if ((Get-FileHash -LiteralPath $collector -Algorithm SHA256).Hash -ne '0CF471896A5F40CC4319054EE35F6B980201AE77C29843470906BCBB35EF31DC') { throw 'COLLECTOR_HASH_DENY' }
Write-Output 'READ_ONLY_CONTEXT_DIAGNOSTIC; TEMP_PARENT_PROBE; NO_INSTALLATION_OR_CA; NO_RETRY_AUTOMATION; AUTHORITY_DENY'
& $collector
$collectorExit=$LASTEXITCODE
$boot=[ordered]@{scope='HOST_BOOT_METADATA_NOT_AUTHORITY';host=[Environment]::MachineName;record_id=$null;boot_start=$null;event_time=$null;error=$null}
try {
 $event=Get-WinEvent -FilterHashtable @{LogName='System';ProviderName='Microsoft-Windows-Kernel-General';Id=12} -MaxEvents 1
 [xml]$xml=$event.ToXml();$start=$xml.Event.EventData.Data|Where-Object {$_.Name -eq 'StartTime'}
 $boot.record_id=$event.RecordId;$boot.event_time=$event.TimeCreated.ToUniversalTime().ToString('o');if($start){$boot.boot_start=[string]$start.'#text'}
} catch {$boot.error=$_.Exception.GetType().FullName}
$boot|ConvertTo-Json
if ($collectorExit -ne 0) { throw "COLLECTOR_EXIT_$collectorExit; PRESERVE_OUTPUT; NO_RETRY" }
