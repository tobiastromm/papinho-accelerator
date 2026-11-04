# Fixed read-only observer; CANDIDATE. Never invoke automatically.
#requires -Version 5.1
[CmdletBinding()]
param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedRequestSha256
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
if((Get-FileHash -LiteralPath $PSCommandPath -Algorithm SHA256).Hash -ne $ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
$request=Join-Path $PSScriptRoot 'ONE-FIXTURE-SECURITY-DESCRIPTOR-OBSERVATION-REQUEST.CANDIDATE.md'
if((Get-FileHash -LiteralPath $request -Algorithm SHA256).Hash -ne $ExpectedRequestSha256){throw 'REQUEST_PIN_DENY'}
if($PSVersionTable.PSEdition -ne 'Desktop' -or $PSVersionTable.PSVersion.Major -ne 5 -or -not [Environment]::Is64BitProcess){throw 'WINDOWS_POWERSHELL_51_AMD64_DENY'}
if([Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'ORIGINAL_CONSOLE_REQUIRED'}
if([Environment]::MachineName -cne 'TCORP-TOBIAS'){throw 'HOST_CONTEXT_DENY'}
$impersonated=[Security.Principal.WindowsIdentity]::GetCurrent($true)
try {if($null -ne $impersonated){throw 'IMPERSONATION_DENY'}} finally {if($null -ne $impersonated){$impersonated.Dispose()}}
$identity=[Security.Principal.WindowsIdentity]::GetCurrent()
try {
 if($identity.User.Value -cne 'S-1-5-21-785699550-1440458426-1682014700-1108' -or -not ([Security.Principal.WindowsPrincipal]::new($identity)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'ORIGINAL_ADMIN_CONTEXT_DENY'}
}finally {$identity.Dispose()}
$bytes=[IO.File]::ReadAllBytes((Join-Path $PSScriptRoot 'fixture-security-descriptor-observer.cs'))
$hasher=[Security.Cryptography.SHA256]::Create()
try {$sourcePin=([BitConverter]::ToString($hasher.ComputeHash($bytes))).Replace('-','').ToLowerInvariant()}finally{$hasher.Dispose()}
if($sourcePin -cne '79dc77c154229e51188ad5c04b4a3122ecfb1409749338f737c6a2b192119fa7'){throw 'SOURCE_PIN_DENY'}
if('FixtureSdReadNative' -as [type]){throw 'NO_REENTRY_NO_RETRY'}
$compiler=[CodeDom.Compiler.CompilerParameters]::new();$compiler.GenerateInMemory=$true;$compiler.WarningLevel=4;$compiler.TreatWarningsAsErrors=$true
[void]$compiler.ReferencedAssemblies.Add('System.dll');[void]$compiler.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition ([Text.UTF8Encoding]::new($false,$true).GetString($bytes)) -CompilerParameters $compiler
Write-Host 'ONE_PROSPECTIVE_READONLY_FIXTURE_SD; DIFFERENT_PROCESS_AND_HANDLE; HISTORICAL_CAUSE_NOT_PROVEN; NO_RETRY_NO_CREATE_NO_SET_NO_DELETE_NO_E_Q_S_P_NO_START_NO_UPDATE'
Write-Host ('Fixed MACHINE locator: '+[FixtureSdReadNative]::Name)
Write-Host 'One Software KSP open, one MACHINE|SILENT key open and one Security Descr read: flags5/buffer1024. No fallback, resize or raw/private-key output. Current SD data is not fixture qualification, positive control or original-handle continuity.'
Write-Host 'Type OBSERVE_FIXTURE_SD_DEA86487_ONCE only after specific owner approval:'
if([Console]::ReadLine() -cne 'OBSERVE_FIXTURE_SD_DEA86487_ONCE'){Write-Host 'OBSERVATION_CANCELLED consumed=False; NO_RETRY_NO_AUTHORITY';exit 2}
$native=[FixtureSdReadNative]::new();$steps=[FixtureSdReadSteps]::new()
$steps.Run($native)
foreach($line in $steps.Evidence){Write-Host $line}
Write-Host ('FIXTURE_SD_READ_RESULT first='+[FixtureSdReadSteps]::Hex($steps.First)+' stage='+$steps.Stage+' consumed='+$steps.Consumed+' complete='+$steps.Complete+' observer_close_known='+$steps.CloseKnown+'; CURRENT_ONLY_NOT_HISTORICAL_PROOF_NO_POSITIVE_CONTROL_NO_READINESS; NO_RETRY; HISTORICAL_RETENTION_AND_OS_QUIESCENCE_UNKNOWN')
if(-not $steps.CloseKnown){$script:fixedFixtureSdObserverRetained=$native;Write-Host 'OBSERVER_RETENTION_UNKNOWN_NO_RETRY_NO_KILL';[Threading.Thread]::Sleep([Threading.Timeout]::Infinite);exit 2}
if($steps.First -ne 0){exit 2}
exit 0
