# Prepared procedure only. No execution is authorized by this file's presence.
# Creates exactly ONE new decision copy; preserves the historical pair; NEVER starts the product.
#requires -Version 5.1
[CmdletBinding()]
param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedRequestSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedDecisionSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedOperationalRequestSha256
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
$sourceStream=[IO.File]::Open((Join-Path $sourceDirectory 'machine-fixture-r0-r4-decision.cs'),[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
$sourceMemory=[IO.MemoryStream]::new();$sourceHash=[Security.Cryptography.SHA256]::Create()
try {
 $sourceStream.CopyTo($sourceMemory);$sourceBytes=$sourceMemory.ToArray()
 $sourcePin=([BitConverter]::ToString($sourceHash.ComputeHash($sourceBytes))).Replace('-','').ToLowerInvariant()
 if($sourcePin -cne '5adcd77a1f849904584cfdfa70be0146dd292ccfacaf5497cf96086bba80c38a'){throw 'SOURCE_PIN_DENY'}
} finally {$sourceHash.Dispose();$sourceMemory.Dispose();$sourceStream.Dispose()}
if((Get-DocumentPin (Join-Path $sourceDirectory 'ONE-MACHINE-FIXTURE-PREPARATION-REQUEST.CANDIDATE.md')) -cne $ExpectedRequestSha256){throw 'REQUEST_PIN_DENY'}
if((Get-DocumentPin (Join-Path $sourceDirectory 'OWNER-MACHINE-FIXTURE-R0-R4-PREPARATION-APPROVAL.txt')) -cne $ExpectedDecisionSha256){throw 'DECISION_PIN_DENY'}
if((Get-DocumentPin (Join-Path $sourceDirectory 'MACHINE-FIXTURE-PREPARATION-ASSURANCE.CANDIDATE.md')) -cne '471c2289c4473516eec1f067dbe8b5c1d3ceddf9f6e8c2a6d7ee7840f7d28f09' -or (Get-DocumentPin (Join-Path $sourceDirectory '..\..\adr\DECISION-personal-machine-fixture-preparation-assurance-t0-t6-20261103.md')) -cne 'd6a77ec6135cc0d43c6a2e29db5535d72be8b8ce251b6a33f8cd22bbb6434e1e'){throw 'T0_T6_NORM_DENY'}
if((Get-DocumentPin (Join-Path $sourceDirectory 'ONE-MACHINE-FIXTURE-R0-R4-PREPARATION-REQUEST.CANDIDATE.md')) -cne $ExpectedOperationalRequestSha256){throw 'OPERATIONAL_REQUEST_PIN_DENY'}
if((Get-DocumentPin (Join-Path $sourceDirectory 'MACHINE-FIXTURE-RESOURCE-ENVELOPE.CANDIDATE.md')) -cne '611c1ad8a78de7cf7c07aa8773906875c64aebaf25b72c6ed2d80e754ef5feda'){throw 'F0_F5_NORM_DENY'}
if((Get-DocumentPin (Join-Path $sourceDirectory 'MACHINE-FIXTURE-GROWTH-ENVELOPE.CANDIDATE.md')) -cne 'fa443a1cc70011ccd60ed40a602df45c13acb59d4ae1c0fe48cd2667436f8000'){throw 'R0_R4_NORM_DENY'}
if('FirstStartDocumentsNative' -as [type]){throw 'NO_REENTRY_NO_RETRY'}
$compiler=[CodeDom.Compiler.CompilerParameters]::new()
$compiler.GenerateInMemory=$true;$compiler.WarningLevel=4;$compiler.TreatWarningsAsErrors=$true
[void]$compiler.ReferencedAssemblies.Add('System.dll');[void]$compiler.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition ([Text.UTF8Encoding]::new($false,$true).GetString($sourceBytes)) -CompilerParameters $compiler
$native=$null;$steps=$null;$prepared=$false;$first='NONE';$closed=$false
try {
 $native=[FirstStartDocumentsNative]::new($ExpectedRequestSha256,$ExpectedDecisionSha256)
 $native.Prepare();$prepared=$true
 Write-Host 'ONE_MACHINE_FIXTURE_NEW_DECISION_ONLY_PRESERVE_HISTORICAL_PAIR; NO_START_NO_ENDPOINT_NO_SCM; HASHES_NOT_AUTHORITY; NO_RETRY_NO_DELETE'
 Write-Host ('Request SHA256: '+$ExpectedRequestSha256)
 Write-Host ('Decision SHA256: '+$ExpectedDecisionSha256)
 Write-Host 'FIXTURE_DECISION_COPY_ONLY: confirm ONE new copy of the actual fresh decision; retain the historical request DATA and old decision untouched. This copy act does not create a key or perform E/Q/S/P. Hashes and copied DATA are never authority.'
 $confirmation='OWNER_MACHINE_FIXTURE_NEW_DECISION '+$ExpectedOperationalRequestSha256+' '+$ExpectedRequestSha256+' '+$ExpectedDecisionSha256
 Write-Host $confirmation
 if([Console]::ReadLine() -cne $confirmation){throw 'CANCELLED'}
 $steps=[FirstStartDocumentSteps]::new();$first=$steps.Run($native);$closed=$steps.CloseKnown
} catch {
 $first=$_.Exception.Message
 # The steps actor owns its one close attempt once Run starts. Never retry it.
 if($null-eq$steps -and $null-ne$native){try {$closed=$native.Close()}catch {$closed=$false}}
} finally {
 if($null-ne$native){foreach($line in $native.Evidence){Write-Host $line}}
 $consumed=$false;if($null-ne$steps){$consumed=$steps.Consumed}
 $createdMask=0;$verifiedMask=0;if($null-ne$native){$createdMask=$native.CreatedMask;$verifiedMask=$native.VerifiedMask}
 Write-Host ('MACHINE_FIXTURE_NEW_DECISION prepared='+$prepared+' first='+$first+' consumed='+$consumed+' created_mask='+$createdMask+' verified_mask='+$verifiedMask+' close_known='+$closed+'; NO_START_NO_ENDPOINT_NO_SCM_NO_RETRY; OS_QUIESCENCE=UNKNOWN; COPY_ACT_SCOPE=ONE_NEW_DECISION_ONLY; ENDPOINT_SCQ_START=DENY; AUTHORITY_NOT_IMPLIED')
 if(-not $closed -and $null-ne$native){$script:machineFixtureOriginalRetained=$native;Write-Host "DOCUMENT_RETENTION_UNKNOWN_NO_RETRY_NO_DELETE";[Threading.Thread]::Sleep([Threading.Timeout]::Infinite)}
}
if($first-ne'NONE' -or -not $closed){exit 2}
exit 0
