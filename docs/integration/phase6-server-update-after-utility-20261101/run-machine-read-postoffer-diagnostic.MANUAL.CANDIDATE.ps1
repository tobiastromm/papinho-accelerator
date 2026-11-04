# One candidate manual sequence. NEVER execute before specific owner approval.
# Two new originals, then the existing explicit MACHINE_READ E/Q/S/P controller. No retry.
#requires -Version 5.1
[CmdletBinding()]
param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedOperationalRequestSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedDecisionSha256
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$sessionFolder='C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
$sessionImage='C:\Program Files\Papinho\Accelerator\papacc_server.exe'
$sessionRequest=Join-Path 'C:\Program Files\Papinho\Accelerator' 'MACHINE-READ-POSTOFFER-SESSION-REQUEST.ORIGINAL.md'
$sessionDecision=Join-Path 'C:\Program Files\Papinho\Accelerator' 'MACHINE-READ-POSTOFFER-SESSION-DECISION.ORIGINAL.txt'
$sessionActor='S-1-5-21-785699550-1440458426-1682014700-1108'
function Assert-SessionPin([string]$Path,[string]$Pin) {
 if((Get-FileHash -LiteralPath $Path -Algorithm SHA256 -ErrorAction Stop).Hash -ne $Pin){throw ('SESSION_PIN_DENY: '+$Path)}
}
function Assert-DocumentMetadata([string]$Path) {
 $acl=Get-Acl -LiteralPath $Path -ErrorAction Stop
 if($acl.GetOwner([Security.Principal.SecurityIdentifier]).Value -cne $sessionActor -or -not $acl.AreAccessRulesProtected){throw 'DOCUMENT_OWNER_PROTECTION_DENY'}
 $rules=@($acl.GetAccessRules($true,$true,[Security.Principal.SecurityIdentifier]))
 if($rules.Count-ne4){throw 'DOCUMENT_DACL_DENY'}
 foreach($sid in @($sessionActor,'S-1-5-18','S-1-5-32-544','S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099')){
  $matched=@($rules|Where-Object {$_.IdentityReference.Value -ceq $sid})
  if($matched.Count-ne1){throw 'DOCUMENT_DACL_SID_DENY'};$r=$matched[0]
  $expectedRights=if($sid-ceq'S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099'){0x00120089}else{0x001f01ff}
  if($r.AccessControlType-ne[Security.AccessControl.AccessControlType]::Allow -or [int]$r.FileSystemRights-ne$expectedRights -or $r.IsInherited -or [int]$r.InheritanceFlags-ne0 -or [int]$r.PropagationFlags-ne0){throw 'DOCUMENT_DACL_RIGHTS_DENY'}
 }
}
Assert-SessionPin $MyInvocation.MyCommand.Path $ExpectedScriptSha256
if([IntPtr]::Size-ne8 -or $PSVersionTable.PSEdition-ne'Desktop' -or $PSVersionTable.PSVersion.Major-ne5){throw 'WINDOWS_POWERSHELL_5_X64_REQUIRED'}
if([Environment]::MachineName -cne 'TCORP-TOBIAS' -or [Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'ORIGINAL_HOST_CONSOLE_DENY'}
$impersonated=[Security.Principal.WindowsIdentity]::GetCurrent($true)
if($null-ne$impersonated){$impersonated.Dispose();throw 'IMPERSONATION_DENY'}
$identity=[Security.Principal.WindowsIdentity]::GetCurrent()
try {
 if($identity.User.Value -cne $sessionActor -or -not ([Security.Principal.WindowsPrincipal]::new($identity)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'ORIGINAL_ADMIN_OPERATOR_DENY'}
} finally {$identity.Dispose()}
$sessionPins=@{
 'ONE-MACHINE-READ-POSTOFFER-SESSION-REQUEST.CANDIDATE.md'=$ExpectedOperationalRequestSha256
 'OWNER-MACHINE-READ-POSTOFFER-SESSION-APPROVAL.txt'=$ExpectedDecisionSha256
 'machine-read-postoffer-originals.cs'='ea441da2541b320527b659d8c8143d5badd8d2c14f37dac3fb687929edb6afcb'
 'prepare-machine-read-postoffer-originals.MANUAL.CANDIDATE.ps1'='9b59bc4d4bb2224fd57d3f05923829c66f07d0079662fa72373135b4f11095c7'
}
$sessionPins['READ-MACHINE-DIAGNOSTIC-SCOPE.CANDIDATE.md']='8489c009dfa62df0835415d4544c0632abd850924a35217b2ec451718147ad38'
$sessionPins['MACHINE-FIXTURE-GA-READBACK.CANDIDATE.md']='3b090974b4abbf5db2543e8ef223cebf78ec8a4b08581d9fa3d7e7beb9b53d47'
$sessionPins['..\..\adr\DECISION-personal-scq-read-machine-k0-k9-20261103.md']='e4c9fdaff1c4f4ddedbe8c7373be81eab31fb594c6683ba87ea823ac154b82c0'
$sessionPins['..\..\adr\DECISION-personal-machine-fixture-ga-readback-s0-s5-20261103.md']='8eeb6da08d28c9f22d3df75cd840758db7e5f57ceda7db3f6f4db2e428ac1186'
foreach($name in $sessionPins.Keys){Assert-SessionPin (Join-Path $sessionFolder $name) $sessionPins[$name]}
if(Test-Path -LiteralPath $sessionRequest){throw 'P1_REQUEST_EXISTS_NO_ADOPTION_NO_RETRY'}
if(Test-Path -LiteralPath $sessionDecision){throw 'P1_DECISION_EXISTS_NO_ADOPTION_NO_RETRY'}
Assert-SessionPin $sessionImage '486d7971097087cf31dbe8ff43b7edc0b05ad2efdf5c06a6975dc56d0fb61932'
Assert-SessionPin 'C:\Program Files\Papinho\Accelerator\libssl-3-x64.dll' '3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'
Assert-SessionPin 'C:\Program Files\Papinho\Accelerator\libcrypto-3-x64.dll' '09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'
Write-Host 'ONE_MANUAL_MACHINE_READ_POSTOFFER_SESSION; TWO_NEW_ORIGINALS_THEN_E_Q_S_P_SEPARATE; NO_RETRY_NO_STOP_NO_PRODUCTION; ON_DISK_PINS_NOT_MAPPED_IMAGE_ATTESTATION'
$pairProcedure=Join-Path $sessionFolder 'prepare-machine-read-postoffer-originals.MANUAL.CANDIDATE.ps1'
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $pairProcedure -ExpectedScriptSha256 '9b59bc4d4bb2224fd57d3f05923829c66f07d0079662fa72373135b4f11095c7' -ExpectedOperationalRequestSha256 $ExpectedOperationalRequestSha256 -ExpectedDecisionSha256 $ExpectedDecisionSha256
$pairExit=$LASTEXITCODE
Write-Host ('MANUAL_P1_ORIGINALS_EXIT='+$pairExit+'; NO_RETRY; EXIT_ALONE_NOT_AUTHORITY')
if($pairExit-ne0){throw 'P1_ORIGINALS_FAILED_OR_UNKNOWN_NO_SESSION_NO_RETRY'}
Assert-SessionPin $sessionRequest $ExpectedOperationalRequestSha256
Assert-SessionPin $sessionDecision $ExpectedDecisionSha256
Assert-DocumentMetadata $sessionRequest
Assert-DocumentMetadata $sessionDecision
Assert-SessionPin $sessionImage '486d7971097087cf31dbe8ff43b7edc0b05ad2efdf5c06a6975dc56d0fb61932'
Assert-SessionPin 'C:\Program Files\Papinho\Accelerator\libssl-3-x64.dll' '3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'
Assert-SessionPin 'C:\Program Files\Papinho\Accelerator\libcrypto-3-x64.dll' '09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'
Write-Host 'Compare request/decision IDs with the pair output and every native plan. Cancel on any mismatch. No response is supplied automatically.'
Write-Host 'At the native P1 DATA prompt, manually enter ONLY the approved tuple below. DATA is not E/Q/S/P confirmation or authority:'
Write-Host 'P1 MACHINE_READ AMD64 417614100:131072:226316 417614100:8257536:71209 417614100:3080192:866236 43fb6712be30ce5b7afca8111512bd284672efa4bb4b8678b21a04eb7b36dd87'
Write-Host 'Runtime PID/token, bilateral, receipt/close and finite shutdown are pending native proofs. P1_READ/entry128/guard188/cap192 may deny; no capacity is inferred.'
$sessionArguments=@(
 '--owner-manual-machine-read-scq-start',
 $sessionRequest,$ExpectedOperationalRequestSha256,$sessionDecision,$ExpectedDecisionSha256,
 '486d7971097087cf31dbe8ff43b7edc0b05ad2efdf5c06a6975dc56d0fb61932',
 '1994944','3145728','7239172096','3221225472','192','100','60000','240000',
 'PapinhoAccelerator.CA.f3f7579f3e90851845264abbb7991500.9b845d6b6c8e909e5f226c65b321987c',
 '877e3f7d80794459586e6d1b977127cd6537f8287c5087055f508f6e95863278'
)
if($sessionArguments.Count-ne16){throw 'EXACT_NATIVE_ARGUMENT_COUNT_DENY'}
& $sessionImage @sessionArguments
$nativeExit=$LASTEXITCODE
Write-Host ('MANUAL_P1_DIAGNOSTIC_NATIVE_EXIT='+$nativeExit+'; NO_RETRY; NO_STOP; NO_PRODUCTIVE_PROMOTION; OS_QUIESCENCE=UNKNOWN')
if($nativeExit-ne0){throw 'P1_SESSION_FAILED_OR_UNKNOWN_NO_RETRY'}
