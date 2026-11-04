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
$request=Join-Path $PSScriptRoot 'ONE-FCA-DATA-PARENT-READONLY-REQUEST.CANDIDATE.md'
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
$codePath=Join-Path $PSScriptRoot 'fca-parent-readonly.cs'
if((Get-FileHash -LiteralPath $codePath -Algorithm SHA256).Hash -ne 'daa881d7b9ac370371d4d1bf714ab6ca1ab3bace389ef685c78cdb1ec5fbcfa2'){throw 'SOURCE_PIN_DENY'}
if('FcaParentReadNative' -as [type]){throw 'NO_REENTRY_NO_RETRY'}
$compiler=[CodeDom.Compiler.CompilerParameters]::new();$compiler.GenerateInMemory=$true;$compiler.WarningLevel=4;$compiler.TreatWarningsAsErrors=$true
[void]$compiler.ReferencedAssemblies.Add('System.dll');[void]$compiler.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition ([IO.File]::ReadAllText($codePath)) -CompilerParameters $compiler
if(-not [FcaParentReadNative]::Layouts()){throw 'NATIVE_LAYOUT_DENY'}
Write-Host 'ONE_READONLY_FCA_DATA_PARENT; NEW_PROCESS_NOT_FCA_OWNER; NO_RETRY_NO_CREATE_NO_ACL_NO_KSP_NO_SCM_NO_START_NO_UPDATE'
Write-Host 'ProgramData KnownFolder DONT_VERIFY plus fixed Papinho; direct Device root and relative component opens with OBJ_DONT_REPARSE. Read-only access differs from future parent ADD_SUBDIRECTORY acquisition. No child Accelerator open. Policy projection is DATA, not admission or future original continuity.'
Write-Host 'Type OBSERVE_FCA_DATA_PARENT_ONCE only after specific owner approval:'
if([Console]::ReadLine() -cne 'OBSERVE_FCA_DATA_PARENT_ONCE'){Write-Host 'FCA_PARENT_CANCELLED consumed=False; NO_RETRY';exit 2}
$native=[FcaParentReadNative]::new();$steps=[FcaParentReadSteps]::new()
$steps.Run($native)
foreach($line in $steps.Evidence){Write-Host $line}
Write-Host ('FCA_PARENT_READ_RESULT first='+$steps.First+' consumed='+$steps.Consumed+' complete='+$steps.Complete+' observer_close_known='+$steps.CloseKnown+'; POLICY_MATCH_NOT_ADMISSION; FUTURE_CONTINUITY_UNPROVEN; HISTORICAL_RETENTION_AND_OS_QUIESCENCE_UNKNOWN; NO_RETRY')
if(-not $steps.CloseKnown){$script:fcaParentObserverRetained=$native;Write-Host 'OBSERVER_RETENTION_UNKNOWN_NO_RETRY_NO_KILL';[Threading.Thread]::Sleep([Threading.Timeout]::Infinite);exit 2}
if($steps.First -cne 'NONE'){exit 2}
exit 0