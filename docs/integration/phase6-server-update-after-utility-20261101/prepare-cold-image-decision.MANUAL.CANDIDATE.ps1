# Prepared procedure only. No execution is authorized by this file's presence.
# Creates ONE operational-decision DATA original; existing pair remains unchanged. NEVER starts the product.
#requires -Version 5.1
[CmdletBinding()]
param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedRequestSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedDecisionSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedOperationalRequestSha256
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$sourceDirectory='C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
function Get-DocumentPin([string]$Path) {
 $stream=[IO.File]::Open($Path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 $hash=[Security.Cryptography.SHA256]::Create()
 try { return ([BitConverter]::ToString($hash.ComputeHash($stream))).Replace('-','').ToLowerInvariant() }
 finally {$hash.Dispose();$stream.Dispose()}
}
if((Get-DocumentPin $MyInvocation.MyCommand.Path) -cne $ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
if([IntPtr]::Size-ne8 -or $PSVersionTable.PSEdition-ne'Desktop' -or $PSVersionTable.PSVersion.Major-ne5){throw 'WINDOWS_POWERSHELL_5_X64_REQUIRED'}
if([Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'ORIGINAL_CONSOLE_REQUIRED'}
if([Environment]::MachineName -cne 'TCORP-TOBIAS'){throw 'HOST_CONTEXT_DENY'} # context only, never authority
$sourceStream=[IO.File]::Open((Join-Path $sourceDirectory 'cold-image-operational-decision.cs'),[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
$sourceMemory=[IO.MemoryStream]::new();$sourceHash=[Security.Cryptography.SHA256]::Create()
try {
 $sourceStream.CopyTo($sourceMemory);$sourceBytes=$sourceMemory.ToArray()
 $sourcePin=([BitConverter]::ToString($sourceHash.ComputeHash($sourceBytes))).Replace('-','').ToLowerInvariant()
 if($sourcePin -cne '95b5df17c83fc646b4aff2b29b2c7d4d96c5581549c695235f8c73b6eaf77f5e'){throw 'SOURCE_PIN_DENY'}
} finally {$sourceHash.Dispose();$sourceMemory.Dispose();$sourceStream.Dispose()}
if((Get-DocumentPin (Join-Path $sourceDirectory 'FIRST-START-SESSION-REQUEST.ORIGINAL.md')) -cne $ExpectedRequestSha256){throw 'REQUEST_PIN_DENY'}
if((Get-DocumentPin (Join-Path $sourceDirectory 'OWNER-COLD-IMAGE-THREE-ACTS-APPROVAL.txt')) -cne $ExpectedDecisionSha256){throw 'DECISION_PIN_DENY'}
if('FirstStartDocumentsNative' -as [type]){throw 'NO_REENTRY_NO_RETRY'}
$compiler=[CodeDom.Compiler.CompilerParameters]::new()
$compiler.GenerateInMemory=$true;$compiler.WarningLevel=4;$compiler.TreatWarningsAsErrors=$true
[void]$compiler.ReferencedAssemblies.Add('System.dll');[void]$compiler.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition ([Text.UTF8Encoding]::new($false,$true).GetString($sourceBytes)) -CompilerParameters $compiler
if($ExpectedRequestSha256 -cne '312999dd9ba4f701991560e65b4a25e3a13c7914bc75603a6efb47a11536798d' -or $ExpectedDecisionSha256 -eq '7f6451d59fc0b7015e0ac612ee471782950880fcfc2013137dc9643276af641a' -or $ExpectedDecisionSha256 -eq 'c6c34866050bc0da189acd8724470838909bc7c790b838e23c46cb86ef58210f' -or $ExpectedDecisionSha256 -eq $ExpectedRequestSha256){throw 'OPERATIONAL_DECISION_DATA_SCOPE_DENY'}
if((Get-DocumentPin (Join-Path $sourceDirectory 'ONE-COLD-IMAGE-THREE-ACTS-REQUEST.CANDIDATE.md')) -cne $ExpectedOperationalRequestSha256){throw 'OPERATIONAL_REQUEST_PIN_DENY'}
$native=$null;$steps=$null;$prepared=$false;$first='NONE';$closed=$false
try {
 $native=[FirstStartDocumentsNative]::OperationalDecisionData($ExpectedRequestSha256,$ExpectedDecisionSha256)
 $native.Prepare();$prepared=$true
 Write-Host 'ONE_NEW_OPERATIONAL_DECISION_DATA_ONLY; NO_START_NO_ENDPOINT_NO_SCM; HASHES_NOT_AUTHORITY; NO_RETRY_NO_DELETE'
 Write-Host ('Request SHA256: '+$ExpectedRequestSha256)
 Write-Host ('Decision SHA256: '+$ExpectedDecisionSha256)
 Write-Host 'Confirm only this ONE decision copy after actual owner approval of the three acts and auxiliary copy. No authority is inferred from DATA; the three independent native confirmations remain required. Cancel if scope is missing.'
 $confirmation='OWNER_OPERATIONAL_DECISION_DATA '+$ExpectedOperationalRequestSha256+' '+$ExpectedRequestSha256+' '+$ExpectedDecisionSha256
 Write-Host $confirmation
 if([Console]::ReadLine() -cne $confirmation){throw 'CANCELLED'}
 $steps=[FirstStartDocumentSteps]::new();$first=$steps.RunOperationalDecision($native);$closed=$steps.CloseKnown
} catch {
 $first=$_.Exception.Message
 # The steps actor owns its one close attempt once Run starts. Never retry it.
 if($null-eq$steps -and $null-ne$native){try {$closed=$native.Close()}catch {$closed=$false}}
} finally {
 if($null-ne$native){foreach($line in $native.Evidence){Write-Host $line}}
 $consumed=$false;if($null-ne$steps){$consumed=$steps.Consumed}
 $createdMask=0;$verifiedMask=0;if($null-ne$native){$createdMask=$native.CreatedMask;$verifiedMask=$native.VerifiedMask}
 Write-Host ('OPERATIONAL_DECISION_DATA prepared='+$prepared+' first='+$first+' consumed='+$consumed+' created_mask='+$createdMask+' verified_mask='+$verifiedMask+' close_known='+$closed+'; NO_START_NO_ENDPOINT_NO_SCM_NO_RETRY; OS_QUIESCENCE=UNKNOWN; DATA_ONLY_SCOPE_NOT_INFERRED; NATIVE_THREE_ACTS_NOT_CONFIRMED_BY_THIS_SCRIPT; AUTHORITY_NOT_IMPLIED')
 if(-not $closed -and $null-ne$native){$script:firstStartOriginalRetained=$native}
}
if($first-ne'NONE' -or -not $closed){exit 2}
exit 0
