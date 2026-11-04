# One protected DATA original through the existing administrative materializer.
# CANDIDATE: never invoke automatically. No parent/key/SCM or product call here.
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
if($ExpectedOperationalRequestSha256 -ceq '7dea315e963e2e8d3307f0277ca85a38a7e4e5c64c8f928b963493ff87a6f430' -or $ExpectedDecisionSha256 -ceq '7497d7175c4add7ae7cb351e9a5693d3847f299e5a7be8e108198bb17a895abb'){throw 'CONSUMED_UPDATE_NOT_THIS_ACT'}
if($PSVersionTable.PSEdition -cne 'Desktop' -or $PSVersionTable.PSVersion.Major -ne 5 -or $PSVersionTable.PSVersion.Minor -ne 1 -or -not [Environment]::Is64BitProcess){throw 'WINDOWS_POWERSHELL_51_AMD64_DENY'}
if([Environment]::MachineName -cne 'TCORP-TOBIAS' -or [Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'ORIGINAL_HOST_CONSOLE_DENY'}
$source=Join-Path $PSScriptRoot 'pdp-config-original.cs'
$sourceBytes=[IO.File]::ReadAllBytes($source)
if($sourceBytes.Length -eq 0 -or $sourceBytes.Length -gt 65536){throw 'SOURCE_CODE_BOUND_DENY'}
$sourceHasher=[Security.Cryptography.SHA256]::Create()
try {$sourcePin=([BitConverter]::ToString($sourceHasher.ComputeHash($sourceBytes))).Replace('-','').ToLowerInvariant()}finally{$sourceHasher.Dispose()}
if($sourcePin -cne 'b93d13e4903ea1e17a20e225af04bfcafcbf358abddce994e199cb6f683c3a72'){throw 'HELD_SOURCE_BYTES_PIN_DENY'}
Assert-PdpPin (Join-Path $PSScriptRoot 'FCA-FIRST-CONFIG.DATA.txt') '16c4847a27d32689ad7eb2c0bd29f6f0f698df74092e9df107cc8203e07e0ae7'
if('FirstStartDocumentsNative' -as [type]){throw 'FRESH_PROCESS_REQUIRED_NO_REENTRY'}
$compiler=[CodeDom.Compiler.CompilerParameters]::new();$compiler.GenerateInMemory=$true;$compiler.WarningLevel=4;$compiler.TreatWarningsAsErrors=$true
[void]$compiler.ReferencedAssemblies.Add('System.dll');[void]$compiler.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition ([Text.UTF8Encoding]::new($false,$true).GetString($sourceBytes)) -CompilerParameters $compiler
if(-not [FirstStartDocumentsNative]::NativeLayouts()){throw 'NATIVE_LAYOUT_DENY'}
$expected='OWNER_PDP_CONFIG_ORIGINAL '+$ExpectedOperationalRequestSha256+' 16c4847a27d32689ad7eb2c0bd29f6f0f698df74092e9df107cc8203e07e0ae7'
Write-Host 'ONE_NEW_PROTECTED_CONFIGURATION_DATA_ORIGINAL; NO_PARENT_NO_KEY_NO_START_NO_SCM; NO_RETRY_NO_DELETE; HASHES_NOT_AUTHORITY'
Write-Host 'Target: C:\Program Files\Papinho\Accelerator\FIRST-INSTANCE-PREPARATION.CONFIG.ORIGINAL.conf; exactly 43 UTF8 bytes, no BOM. Owner/operator + SYSTEM + Administrators, protected DACL. One FILE_CREATE/write/flush/readback; existing target DENY.'
Write-Host 'This confirmation covers only the configuration copy. PDP has its own original native plan, nonce, confirmation and consumption. Cancel on any mismatch.'
Write-Host $expected
if([Console]::ReadLine() -cne $expected){Write-Host 'CONFIG_ORIGINAL_CANCELLED consumed=False; NO_RETRY';exit 2}
$native=$null;$steps=$null;$prepared=$false;$closeKnown=$true;$first='NONE';$consumed=$false
try {
 $native=[FirstStartDocumentsNative]::new()
 $native.Prepare();$prepared=$true
 $steps=[FirstStartDocumentSteps]::new();$first=$steps.Run($native);$consumed=$steps.Consumed;$closeKnown=$steps.CloseKnown
 if($first -ceq 'NONE' -and ($native.CreatedMask -ne 1 -or $native.VerifiedMask -ne 1 -or -not $closeKnown)){$first='RESULT_INCOMPLETE'}
}catch{
 $base=$_.Exception.GetBaseException();$first=$base.Message
 if($base -is [ComponentModel.Win32Exception]){$first+='; WIN32='+$base.NativeErrorCode}
 if($null -ne $native -and $null -eq $steps){try{$closeKnown=$native.Close()}catch{$closeKnown=$false}}
 elseif($null -ne $steps){$closeKnown=$false}
}
$created=0;$verified=0
if($null -ne $native){$created=$native.CreatedMask;$verified=$native.VerifiedMask;foreach($line in $native.Evidence){Write-Host $line}}
Write-Host ('CONFIG_ORIGINAL prepared='+$prepared+' first='+$first+' consumed='+$consumed+' created_mask='+$created+' verified_mask='+$verified+' close_known='+$closeKnown+'; DATA_ONLY_NOT_PDP_AUTHORITY; NO_RETRY_NO_DELETE; HISTORICAL_RETENTION_AND_OS_QUIESCENCE_UNKNOWN')
if(-not $closeKnown){$script:retainedConfigurationOriginal=$native;Write-Host 'CONFIG_ORIGINAL_RETENTION_UNKNOWN_NO_RETRY_NO_KILL';[Threading.Thread]::Sleep([Threading.Timeout]::Infinite);exit 2}
if($first -cne 'NONE'){exit 2}
exit 0
