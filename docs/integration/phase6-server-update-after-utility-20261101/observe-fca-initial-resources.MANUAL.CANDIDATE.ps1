#requires -Version 5.1
param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedRequestSha256
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
if($PSVersionTable.PSEdition -cne 'Desktop' -or -not [Environment]::Is64BitProcess -or [Environment]::MachineName -cne 'TCORP-TOBIAS'){throw 'HOST_DESKTOP_AMD64_DENY'}
if([Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'CONSOLE_REQUIRED'}
if((Get-FileHash -LiteralPath $PSCommandPath).Hash -ne $ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
if((Get-FileHash -LiteralPath (Join-Path $PSScriptRoot 'ONE-FCA-INITIAL-RESOURCE-OBSERVATION-REQUEST.CANDIDATE.md')).Hash -ne $ExpectedRequestSha256){throw 'REQUEST_PIN_DENY'}
$exe=Join-Path $PSScriptRoot 'fca-initial-resource-observer.exe'
$lock=$null
try{
 $lock=[IO.File]::Open($exe,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 $hash=[Security.Cryptography.SHA256]::Create()
 try{$digest=([BitConverter]::ToString($hash.ComputeHash($lock))).Replace('-','').ToLowerInvariant()}finally{$hash.Dispose()}
 if($digest -cne '5c4c293d8c25da16e8d19f0e296f6bfb6baa3c02c566fbf5385122cb2b6503db'){throw 'OBSERVER_PIN_DENY'}
 Write-Host 'ONE_PROSPECTIVE_READONLY_RESOURCE_SAMPLE; DIFFERENT_PROCESS_AND_TIME; NOT_HISTORICAL_CAUSE_NOT_FCA_ADMISSION; NO_RETRY_NO_UPDATE_NO_FCA'
 & $exe
 $code=$LASTEXITCODE
 Write-Host ('MANUAL_RESOURCE_OBSERVATION_EXIT='+$code+'; EXIT_NOT_AUTHORITY; HISTORICAL_RETENTION_OS_QUIESCENCE_UNKNOWN; NO_RETRY')
}finally{if($lock){$lock.Dispose()}}
exit $code