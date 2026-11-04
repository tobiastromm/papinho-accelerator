#requires -Version 5.1
[CmdletBinding()]
param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedRequestSha256
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$folder='C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
function Assert-Pin([string]$path,[string]$pin){if((Get-FileHash -LiteralPath $path -Algorithm SHA256 -ErrorAction Stop).Hash-ne$pin){throw 'PARTICIPANTS_PIN_DENY'}}
Assert-Pin $MyInvocation.MyCommand.Path $ExpectedScriptSha256
Assert-Pin (Join-Path $folder 'ONE-READ-FIRSTCAUSE-PARTICIPANTS-READONLY-REQUEST.CANDIDATE.md') $ExpectedRequestSha256
$source=Join-Path $folder 'read-firstcause-participants-readonly.CANDIDATE.cs'
Assert-Pin $source '0e3f5a783bc399b0e0f620087b34ab596af1ef607fa87900b81aab7dc0fbcb66'
if([Environment]::MachineName-cne'TCORP-TOBIAS'-or[IntPtr]::Size-ne8-or$PSVersionTable.PSEdition-ne'Desktop'-or$PSVersionTable.PSVersion.Major-ne5-or[Console]::IsInputRedirected-or[Console]::IsOutputRedirected){throw 'ORIGINAL_CONTEXT_DENY'}
$impersonated=[Security.Principal.WindowsIdentity]::GetCurrent($true)
if($null-ne$impersonated){$impersonated.Dispose();throw 'IMPERSONATION_DENY'}
$actor=[Security.Principal.WindowsIdentity]::GetCurrent()
try{if($actor.User.Value-cne'S-1-5-21-785699550-1440458426-1682014700-1108'-or-not([Security.Principal.WindowsPrincipal]::new($actor)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'ORIGINAL_ADMIN_DENY'}}finally{$actor.Dispose()}
Write-Host 'ONE_READONLY_READFIRSTCAUSE_CORRELATION; NO_RETRY_NO_STOP_NO_KILL_NO_E_Q_S_NO_UPDATE; NOT_ORIGINAL_RUNTIME_ATTESTATION'
Add-Type -TypeDefinition (Get-Content -LiteralPath $source -Raw -Encoding UTF8) -Language CSharp -ErrorAction Stop
$observationExit=[READFIRSTCAUSEParticipantsReadOnly]::RunFixed()
Write-Host ('MANUAL_READFIRSTCAUSE_PARTICIPANTS_EXIT='+$observationExit+'; OBSERVER_EXIT_NOT_APPLICATION_CLOSE; HISTORICAL_RETENTION_AND_OS_QUIESCENCE_UNKNOWN; NO_RETRY')
exit $observationExit
