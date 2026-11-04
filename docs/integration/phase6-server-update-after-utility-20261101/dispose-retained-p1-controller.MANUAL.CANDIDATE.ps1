#requires -Version 5.1
[CmdletBinding()]
param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedRequestSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedDecisionSha256
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$folder='C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
function Assert-Pin([string]$path,[string]$pin){if((Get-FileHash -LiteralPath $path -Algorithm SHA256 -ErrorAction Stop).Hash-ne$pin){throw 'DISPOSITION_PIN_DENY'}}
Assert-Pin $MyInvocation.MyCommand.Path $ExpectedScriptSha256
Assert-Pin (Join-Path $folder 'ONE-RETAINED-P1-CONTROLLER-DISPOSITION.REQUEST.CANDIDATE.md') $ExpectedRequestSha256
Assert-Pin (Join-Path $folder 'OWNER-RETAINED-P1-CONTROLLER-DISPOSITION-APPROVAL.txt') $ExpectedDecisionSha256
$source=Join-Path $folder 'retained-p1-disposition.CANDIDATE.cs'
Assert-Pin $source '8e4f4c1929e7acf812f139d1b8eb47490fe5b705f7e6763fe612e5d013df9591'
if([Environment]::MachineName-cne'TCORP-TOBIAS'-or[IntPtr]::Size-ne8-or$PSVersionTable.PSEdition-ne'Desktop'-or$PSVersionTable.PSVersion.Major-ne5-or[Console]::IsInputRedirected-or[Console]::IsOutputRedirected){throw 'ORIGINAL_CONTEXT_DENY'}
$impersonated=[Security.Principal.WindowsIdentity]::GetCurrent($true)
if($null-ne$impersonated){$impersonated.Dispose();throw 'IMPERSONATION_DENY'}
$actor=[Security.Principal.WindowsIdentity]::GetCurrent()
try{if($actor.User.Value-cne'S-1-5-21-785699550-1440458426-1682014700-1108'-or-not([Security.Principal.WindowsPrincipal]::new($actor)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'ORIGINAL_ADMIN_DENY'}}finally{$actor.Dispose()}
Write-Host 'ONE_EXCEPTIONAL_RETAINED_CONTROLLER_DISPOSITION; NOT_SCQ_CLOSE; NO_RETRY_NO_UPDATE_NO_START_NO_STOP_NO_DELETE; ON_DISK_PINS_NOT_MAPPED_IMAGE_ATTESTATION'
Add-Type -TypeDefinition (Get-Content -LiteralPath $source -Raw -Encoding UTF8) -Language CSharp -ErrorAction Stop
$dispositionExit=[RetainedP1Disposition]::RunFixed()
Write-Host ('MANUAL_RETAINED_DISPOSITION_EXIT='+$dispositionExit+'; NO_RETRY; APPLICATION_CLOSE_AND_HISTORICAL_RETENTION_AND_OS_QUIESCENCE_UNKNOWN')
exit $dispositionExit
