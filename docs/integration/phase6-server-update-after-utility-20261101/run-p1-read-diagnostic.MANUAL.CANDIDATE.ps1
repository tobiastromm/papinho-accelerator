# One candidate manual sequence. NEVER execute before specific owner approval.
# Two new originals, then the existing native TO/P1 E/Q/S controller. No retry.
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
$sessionRequest=Join-Path $sessionFolder 'FIRST-P1-READ-SESSION-REQUEST.ORIGINAL.md'
$sessionDecision=Join-Path $sessionFolder 'FIRST-P1-READ-OPERATIONAL-DECISION.ORIGINAL.txt'
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
 'ONE-P1-READ-THREE-ACTS-REQUEST.CANDIDATE.md'=$ExpectedOperationalRequestSha256
 'OWNER-P1-READ-THREE-ACTS-APPROVAL.txt'=$ExpectedDecisionSha256
 'P1-READ-ACT-ENDPOINT.REQUEST.CANDIDATE.md'='50c81a21bced66a94362d0b489bc1493185747ed5bde010b36f4a9ac3f43ba52'
 'P1-READ-ACT-SCQ.REQUEST.CANDIDATE.md'='f31f36987e3a947631d4dc310e01f031b43913321e31925e69730423c6b81e08'
 'P1-READ-ACT-START.REQUEST.CANDIDATE.md'='70f16b1a5bd3d5195cab98108dae988051b72e91045e0f95a0f2419469080086'
 'p1-read-session-originals.cs'='169a58a38580ac8a67ad27481ef55d949780f79905a928b1164baa9622a3fc3c'
 'prepare-p1-read-originals.MANUAL.CANDIDATE.ps1'='8c6911f8ca1f0cad8aa2c36b440609f6db760e7feab6e29de88733ac992dca9f'
}
foreach($name in $sessionPins.Keys){Assert-SessionPin (Join-Path $sessionFolder $name) $sessionPins[$name]}
if(Test-Path -LiteralPath $sessionRequest){throw 'P1_REQUEST_EXISTS_NO_ADOPTION_NO_RETRY'}
if(Test-Path -LiteralPath $sessionDecision){throw 'P1_DECISION_EXISTS_NO_ADOPTION_NO_RETRY'}
Assert-SessionPin $sessionImage '78886f53119af1875dd7f326221b8f154e9528ef02f2f61f6a7bd9d23fab3de5'
Assert-SessionPin 'C:\Program Files\Papinho\Accelerator\libssl-3-x64.dll' '3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'
Assert-SessionPin 'C:\Program Files\Papinho\Accelerator\libcrypto-3-x64.dll' '09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'
Write-Host 'ONE_MANUAL_P1_READ_SESSION; TWO_NEW_ORIGINALS_THEN_E_Q_S_SEPARATE; NO_RETRY_NO_STOP_NO_PRODUCTION; ON_DISK_PINS_NOT_MAPPED_IMAGE_ATTESTATION'
$pairProcedure=Join-Path $sessionFolder 'prepare-p1-read-originals.MANUAL.CANDIDATE.ps1'
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $pairProcedure -ExpectedScriptSha256 '8c6911f8ca1f0cad8aa2c36b440609f6db760e7feab6e29de88733ac992dca9f' -ExpectedOperationalRequestSha256 $ExpectedOperationalRequestSha256 -ExpectedDecisionSha256 $ExpectedDecisionSha256
$pairExit=$LASTEXITCODE
Write-Host ('MANUAL_P1_ORIGINALS_EXIT='+$pairExit+'; NO_RETRY; EXIT_ALONE_NOT_AUTHORITY')
if($pairExit-ne0){throw 'P1_ORIGINALS_FAILED_OR_UNKNOWN_NO_SESSION_NO_RETRY'}
Assert-SessionPin $sessionRequest $ExpectedOperationalRequestSha256
Assert-SessionPin $sessionDecision $ExpectedDecisionSha256
Assert-DocumentMetadata $sessionRequest
Assert-DocumentMetadata $sessionDecision
Assert-SessionPin $sessionImage '78886f53119af1875dd7f326221b8f154e9528ef02f2f61f6a7bd9d23fab3de5'
Assert-SessionPin 'C:\Program Files\Papinho\Accelerator\libssl-3-x64.dll' '3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'
Assert-SessionPin 'C:\Program Files\Papinho\Accelerator\libcrypto-3-x64.dll' '09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'
Write-Host 'Compare request/decision IDs with the pair output and every native plan. Cancel on any mismatch. No response is supplied automatically.'
Write-Host 'At the native P1 DATA prompt, manually enter ONLY the approved tuple below. DATA is not E/Q/S confirmation or authority:'
Write-Host 'P1 TRANSPORT_ONLY AMD64 417614100:131072:226316 417614100:8257536:71209 417614100:1638400:439388 43fb6712be30ce5b7afca8111512bd284672efa4bb4b8678b21a04eb7b36dd87'
Write-Host 'Runtime PID/token, bilateral, receipt/close and finite shutdown are pending native proofs. P1_READ/entry128/guard188/cap192 may deny; no capacity is inferred.'
$sessionArguments=@(
 '--owner-manual-first-scq-start-to-existing',
 $sessionRequest,$ExpectedOperationalRequestSha256,$sessionDecision,$ExpectedDecisionSha256,
 '78886f53119af1875dd7f326221b8f154e9528ef02f2f61f6a7bd9d23fab3de5',
 '1994944','3145728','7239172096','3221225472','192','100','60000','240000'
)
if($sessionArguments.Count-ne14){throw 'EXACT_NATIVE_ARGUMENT_COUNT_DENY'}
& $sessionImage @sessionArguments
$nativeExit=$LASTEXITCODE
Write-Host ('MANUAL_P1_DIAGNOSTIC_NATIVE_EXIT='+$nativeExit+'; NO_RETRY; NO_STOP; NO_PRODUCTIVE_PROMOTION; OS_QUIESCENCE=UNKNOWN')
if($nativeExit-ne0){throw 'P1_SESSION_FAILED_OR_UNKNOWN_NO_RETRY'}
