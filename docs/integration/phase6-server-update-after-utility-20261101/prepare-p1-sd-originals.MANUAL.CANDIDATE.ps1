# Two new documentary originals only, after actual approval of the P1 package.
# Candidate; never execute from its presence. No product/SCM/endpoint call.
#requires -Version 5.1
[CmdletBinding()]
param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedOperationalRequestSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedDecisionSha256
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$sourceDirectory='C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
function Get-DocumentPin([string]$Path) {
 $stream=[IO.File]::Open($Path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 $hash=[Security.Cryptography.SHA256]::Create()
 try {return ([BitConverter]::ToString($hash.ComputeHash($stream))).Replace('-','').ToLowerInvariant()}
 finally {$hash.Dispose();$stream.Dispose()}
}
function Assert-P1OriginalPins([string]$RequestPin,[string]$DecisionPin) {
 $oldRequests=@('312999dd9ba4f701991560e65b4a25e3a13c7914bc75603a6efb47a11536798d','25ea33a28c714952e1b1fb42202e64be7ca933afe0fa7aa37d2a5e654918dff4','af8529db7f447fa6af6fc462fcb3cba4583abfc41f2e867c255b801974a67bc9','86303aac1bf3b040a5949e8947a2bee2cb157fdbde472f31d5f2830b469a2a34','608b6ca768dd35f35c827ee29498a2bdf5796cc5a3d66843a8568fa002934441')
 $oldDecisions=@('7f6451d59fc0b7015e0ac612ee471782950880fcfc2013137dc9643276af641a','c6c34866050bc0da189acd8724470838909bc7c790b838e23c46cb86ef58210f','ef8b47fa4dba37224bef904488ff0494db4e5804c1e696463ddc0dd864f3e199','a7d3544dd19c0639e9799486efe70b976d3ab543de0d11ab15edd2d424148607','f54d0bf3c6c609d5e7f93d348d4332364fdee0edc1592ea2a30137ba31ead0e1')
 if($RequestPin -notmatch '^[a-f0-9]{64}$' -or $DecisionPin -notmatch '^[a-f0-9]{64}$' -or $RequestPin -ceq $DecisionPin -or $oldRequests -contains $RequestPin -or $oldDecisions -contains $DecisionPin){throw 'P1_ORIGINAL_SCOPE_DENY'}
}
Assert-P1OriginalPins $ExpectedOperationalRequestSha256 $ExpectedDecisionSha256
if((Get-DocumentPin $MyInvocation.MyCommand.Path) -cne $ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
if([IntPtr]::Size-ne8 -or $PSVersionTable.PSEdition-ne'Desktop' -or $PSVersionTable.PSVersion.Major-ne5){throw 'WINDOWS_POWERSHELL_5_X64_REQUIRED'}
if([Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'ORIGINAL_CONSOLE_REQUIRED'}
if([Environment]::MachineName -cne 'TCORP-TOBIAS'){throw 'HOST_CONTEXT_DENY'}
if((Get-DocumentPin (Join-Path $sourceDirectory 'ONE-P1-SD-THREE-ACTS-REQUEST.CANDIDATE.md')) -cne $ExpectedOperationalRequestSha256){throw 'REQUEST_PIN_DENY'}
if((Get-DocumentPin (Join-Path $sourceDirectory 'OWNER-P1-SD-THREE-ACTS-APPROVAL.txt')) -cne $ExpectedDecisionSha256){throw 'DECISION_PIN_DENY'}
$sourceStream=[IO.File]::Open((Join-Path $sourceDirectory 'p1-sd-session-originals.cs'),[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
$sourceMemory=[IO.MemoryStream]::new();$sourceHash=[Security.Cryptography.SHA256]::Create()
try {
 $sourceStream.CopyTo($sourceMemory);$sourceBytes=$sourceMemory.ToArray()
 if(([BitConverter]::ToString($sourceHash.ComputeHash($sourceBytes))).Replace('-','').ToLowerInvariant() -cne '86a225bf6c5fea5539cde89e1f3e20b874b04cfec51efa42a17339c0086e617c'){throw 'SOURCE_PIN_DENY'}
} finally {$sourceHash.Dispose();$sourceMemory.Dispose();$sourceStream.Dispose()}
if('FirstStartDocumentsNative' -as [type]){throw 'NO_REENTRY_NO_RETRY'}
$compiler=[CodeDom.Compiler.CompilerParameters]::new()
$compiler.GenerateInMemory=$true;$compiler.WarningLevel=4;$compiler.TreatWarningsAsErrors=$true
[void]$compiler.ReferencedAssemblies.Add('System.dll');[void]$compiler.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition ([Text.UTF8Encoding]::new($false,$true).GetString($sourceBytes)) -CompilerParameters $compiler
$native=$null;$steps=$null;$prepared=$false;$first='NONE';$closed=$false
try {
 $native=[FirstStartDocumentsNative]::new($ExpectedOperationalRequestSha256,$ExpectedDecisionSha256)
 $native.Prepare();$prepared=$true
 Write-Host 'TWO_NEW_P1_SD_ORIGINALS_DATA_ONLY; NO_START_NO_ENDPOINT_NO_SCM; NO_RETRY_NO_DELETE; HASHES_NOT_AUTHORITY'
 Write-Host ('Request SHA256: '+$ExpectedOperationalRequestSha256)
 Write-Host ('Actual owner decision SHA256: '+$ExpectedDecisionSha256)
 Write-Host 'Confirm ONLY the two documentary copies after actual approval of this package. This does not perform the three native E/Q/S confirmations.'
 $confirmation='OWNER_P1_SD_SESSION_DOCUMENTS '+$ExpectedOperationalRequestSha256+' '+$ExpectedDecisionSha256
 Write-Host $confirmation
 if([Console]::ReadLine() -cne $confirmation){throw 'CANCELLED'}
 $steps=[FirstStartDocumentSteps]::new();$first=$steps.Run($native);$closed=$steps.CloseKnown
} catch {
 $first=$_.Exception.Message
 if($null-eq$steps -and $null-ne$native){try {$closed=$native.Close()}catch {$closed=$false}}
} finally {
 if($null-ne$native){foreach($line in $native.Evidence){Write-Host $line}}
 $consumed=$false;if($null-ne$steps){$consumed=$steps.Consumed}
 $createdMask=0;$verifiedMask=0;if($null-ne$native){$createdMask=$native.CreatedMask;$verifiedMask=$native.VerifiedMask}
 Write-Host ('P1_ORIGINALS prepared='+$prepared+' first='+$first+' consumed='+$consumed+' created_mask='+$createdMask+' verified_mask='+$verifiedMask+' close_known='+$closed+'; DATA_COPIES_ONLY; E_Q_S_NOT_CONFIRMED; OS_QUIESCENCE=UNKNOWN; NO_RETRY_NO_DELETE')
 if(-not $closed -and $null-ne$native){$script:firstStartOriginalRetained=$native}
}
if($first-ne'NONE' -or -not $closed -or $createdMask-ne3 -or $verifiedMask-ne3){exit 2}
exit 0
