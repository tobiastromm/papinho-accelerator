# Candidate only. One future explicitly authorized read-only observation.
#requires -Version 5.1
[CmdletBinding()]
param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedRequestSha256
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
if([IntPtr]::Size-ne8 -or $PSVersionTable.PSEdition-ne'Desktop'){throw 'X64_DESKTOP_REQUIRED'}
if([Environment]::MachineName-cne'TCORP-TOBIAS'){throw 'HOST_DENY'}
if([Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'INTERACTIVE_CONSOLE_REQUIRED'}
$folder='C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
$pins=@{
 'ONE-P1-RAW-OPEN-OBSERVATION-REQUEST.CANDIDATE.md'=$ExpectedRequestSha256
 'p1-raw-open-observer.c'='224e017b0fbc6ab0bd2b4e2a2afa7ea11edec3cdc8a001630a6ca476a0f805c3'
 'p1-raw-open-observer.exe'='7f1f8bddad246144803406a97f6b849c84d4a556d334fbee4448ee680638cf1d'
}
if((Get-FileHash -LiteralPath $MyInvocation.MyCommand.Path -Algorithm SHA256).Hash.ToLowerInvariant()-cne$ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
foreach($name in $pins.Keys){
 if((Get-FileHash -LiteralPath (Join-Path $folder $name) -Algorithm SHA256).Hash.ToLowerInvariant()-cne$pins[$name]){throw ('PIN_DENY: '+$name)}
}
# Do not pre-open/hash the target: that would be another target acquisition.
# The observer performs the one exact NtCreateFile, verifies the returned object
# only on accepted success, and never retries or invokes installed controller.
Write-Host 'ONE_PROSPECTIVE_READONLY_OBSERVATION; DIFFERENT_PROCESS; HISTORICAL_CAUSE_UNKNOWN; NO_RETRY_NO_E_Q_S_NO_UPDATE'
& (Join-Path $folder 'p1-raw-open-observer.exe')
$observationExit=$LASTEXITCODE
Write-Host ('P1_RAW_OPEN_MANUAL_EXIT='+$observationExit+'; INVOKED=True; EXIT_NOT_AUTHORITY; HISTORICAL_RETENTION_AND_OS_QUIESCENCE_UNKNOWN; NO_RETRY')
exit $observationExit
