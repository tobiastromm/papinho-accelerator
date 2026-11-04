# One DATA original followed by one independently confirmed PDP parent act.
# CANDIDATE / no automatic execution, responses, retry, cleanup or UPDATE.
#requires -Version 5.1
[CmdletBinding()]
param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedOperationalRequestSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedDecisionSha256
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
function Assert-PdpPin([string]$Path,[string]$Hash){if((Get-FileHash -LiteralPath $Path -Algorithm SHA256 -ErrorAction Stop).Hash -ne $Hash){throw ('PIN_DENY: '+$Path)}}
Assert-PdpPin $PSCommandPath $ExpectedScriptSha256
Assert-PdpPin (Join-Path $PSScriptRoot 'ONE-PDP-PARENT-PREPARATION-REQUEST.CANDIDATE.md') $ExpectedOperationalRequestSha256
Assert-PdpPin (Join-Path $PSScriptRoot 'OWNER-PDP-PARENT-PREPARATION-APPROVAL.txt') $ExpectedDecisionSha256
Assert-PdpPin (Join-Path $PSScriptRoot 'FCA-DATA-PARENT-PDP0-PDP7.CANDIDATE.md') 'e68969bed2a8d84ab366378cedc991bc1f105964cfcfff4c7e099d56cd8acd14'
Assert-PdpPin (Join-Path $PSScriptRoot '..\..\adr\DECISION-personal-data-parent-pdp0-pdp7-20261103.md') 'b8c9ae9573a9c010fde25f8856f6efb78575f0a45b500eeefae0dd42e93d4065'
if($ExpectedOperationalRequestSha256 -ceq '7dea315e963e2e8d3307f0277ca85a38a7e4e5c64c8f928b963493ff87a6f430' -or $ExpectedDecisionSha256 -ceq '7497d7175c4add7ae7cb351e9a5693d3847f299e5a7be8e108198bb17a895abb'){throw 'CONSUMED_UPDATE_NOT_THIS_ACT'}
if($PSVersionTable.PSEdition -cne 'Desktop' -or $PSVersionTable.PSVersion.Major -ne 5 -or $PSVersionTable.PSVersion.Minor -ne 1 -or -not [Environment]::Is64BitProcess){throw 'WINDOWS_POWERSHELL_51_AMD64_DENY'}
if([Environment]::MachineName -cne 'TCORP-TOBIAS' -or [Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'ORIGINAL_HOST_CONSOLE_DENY'}
$impersonated=[Security.Principal.WindowsIdentity]::GetCurrent($true)
try {if($null -ne $impersonated){throw 'IMPERSONATION_DENY'}}finally{if($null -ne $impersonated){$impersonated.Dispose()}}
$actor=[Security.Principal.WindowsIdentity]::GetCurrent()
try {if($actor.User.Value -cne 'S-1-5-21-785699550-1440458426-1682014700-1108' -or -not ([Security.Principal.WindowsPrincipal]::new($actor)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'OPERATOR_CONTEXT_DENY'}}finally{$actor.Dispose()}
$copy=Join-Path $PSScriptRoot 'prepare-pdp-config-original.MANUAL.CANDIDATE.ps1'
$copyPin='40402ec375429f2e03328d1ebf101d4f0bc1886bb38c4de232a9be6caf0eac60'
Assert-PdpPin $copy $copyPin
$sourceBytes=[IO.File]::ReadAllBytes((Join-Path $PSScriptRoot 'pdp-config-original.cs'))
if($sourceBytes.Length -eq 0 -or $sourceBytes.Length -gt 65536){throw 'SOURCE_CODE_BOUND_DENY'}
$hasher=[Security.Cryptography.SHA256]::Create()
try {$sourcePin=([BitConverter]::ToString($hasher.ComputeHash($sourceBytes))).Replace('-','').ToLowerInvariant()}finally{$hasher.Dispose()}
if($sourcePin -cne 'b93d13e4903ea1e17a20e225af04bfcafcbf358abddce994e199cb6f683c3a72'){throw 'SOURCE_CODE_PIN_DENY'}
if('FirstStartDocumentsNative' -as [type]){throw 'FRESH_PROCESS_REQUIRED_NO_REENTRY'}
$compiler=[CodeDom.Compiler.CompilerParameters]::new();$compiler.GenerateInMemory=$true;$compiler.WarningLevel=4;$compiler.TreatWarningsAsErrors=$true
[void]$compiler.ReferencedAssemblies.Add('System.dll');[void]$compiler.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition ([Text.UTF8Encoding]::new($false,$true).GetString($sourceBytes)) -CompilerParameters $compiler
if(-not [FirstStartDocumentsNative]::NativeLayouts()){throw 'NATIVE_LAYOUT_DENY'}
$image='C:\Program Files\Papinho\Accelerator\papacc_server.exe'
$config='C:\Program Files\Papinho\Accelerator\FIRST-INSTANCE-PREPARATION.CONFIG.ORIGINAL.conf'
$imageReference=$null;$configReference=$null;$copyExit=$null;$nativeExit=$null
$dllReferences=[Collections.Generic.List[IO.FileStream]]::new()
function Assert-LockedData([IO.FileStream]$Stream,[long]$Bytes,[string]$Pin){
 if($Stream.Length -ne $Bytes){throw 'LOCKED_SIZE_DENY'};$Stream.Position=0;$hash=[Security.Cryptography.SHA256]::Create()
 try {$observed=([BitConverter]::ToString($hash.ComputeHash($Stream))).Replace('-','').ToLowerInvariant()}finally{$hash.Dispose()}
 if($observed -cne $Pin){throw 'LOCKED_HASH_DENY'}
}
try {
 $imageReference=[IO.File]::Open($image,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 Assert-LockedData $imageReference 911360 'c14dc2a0e2f3e51d19588c9930e354d512c9f3d3c41f0eaff7957dae0734f474'
 if(-not [FirstStartDocumentsNative]::ExpectedExecutor($imageReference.SafeFileHandle.DangerousGetHandle())){throw 'CURRENT_IMAGE_ID_DENY'}
 foreach($dll in @(
  @{Name='libssl-3-x64.dll';Bytes=1321472;Pin='3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'},
  @{Name='libcrypto-3-x64.dll';Bytes=7217664;Pin='09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'})){
  $reference=[IO.File]::Open((Join-Path 'C:\Program Files\Papinho\Accelerator' $dll.Name),[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
  $dllReferences.Add($reference);Assert-LockedData $reference $dll.Bytes $dll.Pin
 }
 Write-Host 'ONE_MANUAL_CONFIGURATION_COPY_THEN_PDP_PARENT; SEPARATE_CONFIRMATIONS_CONSUMPTIONS; NO_RETRY_NO_UPDATE_NO_START_NO_CA; ON_DISK_PINS_NOT_MAPPED_IMAGE_ATTESTATION'
 & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $copy -ExpectedScriptSha256 $copyPin -ExpectedOperationalRequestSha256 $ExpectedOperationalRequestSha256 -ExpectedDecisionSha256 $ExpectedDecisionSha256
 $copyExit=$LASTEXITCODE
 Write-Host ('MANUAL_CONFIG_ORIGINAL_EXIT='+$copyExit+'; EXIT_ALONE_NOT_AUTHORITY; NO_RETRY')
 if($copyExit -ne 0){throw 'CONFIG_ORIGINAL_FAILED_OR_UNKNOWN_NO_PDP_NO_RETRY'}
 $configReference=[IO.File]::Open($config,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 Assert-LockedData $configReference 43 '16c4847a27d32689ad7eb2c0bd29f6f0f698df74092e9df107cc8203e07e0ae7'
 Assert-LockedData $imageReference 911360 'c14dc2a0e2f3e51d19588c9930e354d512c9f3d3c41f0eaff7957dae0734f474'
 if(-not [FirstStartDocumentsNative]::ExpectedExecutor($imageReference.SafeFileHandle.DangerousGetHandle())){throw 'CURRENT_IMAGE_CHANGED_DENY'}
 Write-Host 'PDP native plan follows. Compare source ID with the CONFIG_ORIGINAL creation output, operator, retained ProgramData ID, exact Papinho leaf, purpose and risks. Cancel on mismatch. No response supplied automatically.'
 Write-Host 'Only ONE parent Papinho, no Accelerator/key/CA/SCM. Entry128/guard188/cap192, private1994944/working3145728, physical7239172096/commit3221225472, freshness100ms/window240000/global60000 may DENY; no capacity inferred.'
 $nativeArguments=@('config','first-install','--declare','NEW_INSTALL_PARENT_PREPARE','--source',$config,'--parent','C:\ProgramData','--container','Papinho')
 if($nativeArguments.Count -ne 10){throw 'EXACT_PDP_ARGV_DENY'}
 & $image @nativeArguments
 $nativeExit=$LASTEXITCODE
 Write-Host ('MANUAL_PDP_PARENT_EXIT='+$nativeExit+'; NO_RETRY_NO_CA_NO_START; EXIT_NOT_READINESS; HISTORICAL_RETENTION_AND_OS_QUIESCENCE_UNKNOWN')
 if($nativeExit -ne 0){throw 'PDP_PARENT_FAILED_OR_UNKNOWN_NO_RETRY'}
}finally{
 if($null -ne $configReference){$configReference.Dispose()}
 foreach($reference in $dllReferences){$reference.Dispose()}
 if($null -ne $imageReference){$imageReference.Dispose()}
}
