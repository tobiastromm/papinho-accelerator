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
 $oldRequests=@('ed404e4cad579acc4e91cd2fae727f73bbe1629ce9d445d16f3c2035c6f6f018','b275975034b33477a90157a0f55bc67e5bd9482abdf21c1c967f384e0cbfb896','312999dd9ba4f701991560e65b4a25e3a13c7914bc75603a6efb47a11536798d','25ea33a28c714952e1b1fb42202e64be7ca933afe0fa7aa37d2a5e654918dff4','af8529db7f447fa6af6fc462fcb3cba4583abfc41f2e867c255b801974a67bc9','86303aac1bf3b040a5949e8947a2bee2cb157fdbde472f31d5f2830b469a2a34','608b6ca768dd35f35c827ee29498a2bdf5796cc5a3d66843a8568fa002934441','16e0693541eed115886c59092ebac8f5b32dfe68ef2a893b5fda5d49fd7fb99a','e3c579ff64a62aabedb34bd06d914f50184bdadeca7f4b09a28c893aab595f69','bd8bd383513415fa21470f8d324f985e0343cf651c53fc3204f665b6d0744447','cce1bb4ce0a0f47ab19d2506cbf5908564949bb9a1107cba61f085017b37cbaa','59a5a75925b52590d0db90eb0bdb436cc7dd9ffe1a3cb0a81200b21ff53c399a','f87d9281c4f9926fdc04af50006b48ac881d9dc466c392d475e941fe54299d12','90b68869c2476b6246ed34de71e8225f5740cbf9f3dcecf3983dacd18188befd','2a34a063563cd9fdf12ec5742b6c6d45ede377fa4501b104779c33205e31cfc0','8ee498d23e5c306b746f1e32e246d37a0ee4e50df5081842a15da2274200ae54','b2ae05445cc49e19b56d997a2c248db575796ce5ee7896108c862ffd6617bd7e')
 $oldDecisions=@('00539879827aa7593600d4023e1b784f7863d56de692668a0e8b3a98ea7ee68f','af606af323918f1bc9362a1de6ad5eff22fe515315b5b5a0ea45aa524abf5223','7f6451d59fc0b7015e0ac612ee471782950880fcfc2013137dc9643276af641a','c6c34866050bc0da189acd8724470838909bc7c790b838e23c46cb86ef58210f','ef8b47fa4dba37224bef904488ff0494db4e5804c1e696463ddc0dd864f3e199','a7d3544dd19c0639e9799486efe70b976d3ab543de0d11ab15edd2d424148607','f54d0bf3c6c609d5e7f93d348d4332364fdee0edc1592ea2a30137ba31ead0e1','019222b9ea2c4e2e0889639098c63ff5187b04de96b8ddc768a0b9fcd437b28b','780ac91743c26eff3ac4497b35e582321beff58adc02aae456ff2aa0ac44bac4','41824a586fe583d9f26969c01fbaa2e0061f7865a92d2ea0ef4820a042912c4c','ebbde1881439fde75d21219130d1e7660e432457e94ac6d268173a014f1962fb','7f2fff6e3e855d156fa5c14ef0fe1f814e7d81c9cb5c782bd18f3c8be6859f15','719b44fe82316f382b7849e64cf7ae2b5dca22379d39bf91ba1e485ddb144648','59f2e58d663bea8646f40d8e3a08c0ec5bddea9e5bfe48774464771658c792a5','ff573574ea963eaa300f4d3aa4b4699ab76db416753118945f5a1742ea2c3ef4','f3a03914a670bf6fd265997519a21fc672e89bc12bc9fd552db61a36ee25beca')
 if($RequestPin -notmatch '^[a-f0-9]{64}$' -or $DecisionPin -notmatch '^[a-f0-9]{64}$' -or $RequestPin -ceq $DecisionPin -or $oldRequests -contains $RequestPin -or $oldDecisions -contains $DecisionPin){throw 'P1_ORIGINAL_SCOPE_DENY'}
}
Assert-P1OriginalPins $ExpectedOperationalRequestSha256 $ExpectedDecisionSha256
if((Get-DocumentPin $MyInvocation.MyCommand.Path) -cne $ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
if([IntPtr]::Size-ne8 -or $PSVersionTable.PSEdition-ne'Desktop' -or $PSVersionTable.PSVersion.Major-ne5){throw 'WINDOWS_POWERSHELL_5_X64_REQUIRED'}
if([Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'ORIGINAL_CONSOLE_REQUIRED'}
if([Environment]::MachineName -cne 'TCORP-TOBIAS'){throw 'HOST_CONTEXT_DENY'}
if((Get-DocumentPin (Join-Path $sourceDirectory 'ONE-WS3M-THREE-ACTS-REQUEST.CANDIDATE.md')) -cne $ExpectedOperationalRequestSha256){throw 'REQUEST_PIN_DENY'}
if((Get-DocumentPin (Join-Path $sourceDirectory 'OWNER-WS3M-THREE-ACTS-APPROVAL.txt')) -cne $ExpectedDecisionSha256){throw 'DECISION_PIN_DENY'}
$sourceStream=[IO.File]::Open((Join-Path $sourceDirectory 'ws3m-session-originals.cs'),[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
$sourceMemory=[IO.MemoryStream]::new();$sourceHash=[Security.Cryptography.SHA256]::Create()
try {
 $sourceStream.CopyTo($sourceMemory);$sourceBytes=$sourceMemory.ToArray()
 if(([BitConverter]::ToString($sourceHash.ComputeHash($sourceBytes))).Replace('-','').ToLowerInvariant() -cne '14c658038f0d9be6e4c9697c2008bc3cdfd75193a779c21414a2cd80c514712d'){throw 'SOURCE_PIN_DENY'}
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
 Write-Host 'TWO_NEW_WS3M_ORIGINALS_DATA_ONLY; NO_START_NO_ENDPOINT_NO_SCM; NO_RETRY_NO_DELETE; HASHES_NOT_AUTHORITY'
 Write-Host ('Request SHA256: '+$ExpectedOperationalRequestSha256)
 Write-Host ('Actual owner decision SHA256: '+$ExpectedDecisionSha256)
 Write-Host 'Confirm ONLY the two documentary copies after actual approval of this package. This does not perform the three native E/Q/S confirmations.'
 $confirmation='OWNER_WS3M_SESSION_DOCUMENTS '+$ExpectedOperationalRequestSha256+' '+$ExpectedDecisionSha256
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
