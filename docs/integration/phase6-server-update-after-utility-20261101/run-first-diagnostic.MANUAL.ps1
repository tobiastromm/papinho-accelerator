# One OWNER-MANUAL authorized sequence only. Prepared, never executed by the agent.
# No retry, automated answers, service-start shortcut, Stop, cleanup or recovery.
# On-disk hashes are not mapped-image attestation or administrative authority.
#requires -Version 5.1
[CmdletBinding()]
param([Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedScriptSha256)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$sessionFolder='C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
$sessionImage='C:\Program Files\Papinho\Accelerator\papacc_server.exe'
$sessionRequest=Join-Path $sessionFolder 'FIRST-START-SESSION-REQUEST.ORIGINAL.md'
$sessionDecision=Join-Path $sessionFolder 'FIRST-DIAGNOSTIC-OPERATIONAL-DECISION.ORIGINAL.txt'
$sessionActor='S-1-5-21-785699550-1440458426-1682014700-1108'
function Assert-SessionPin([string]$Path,[string]$Pin) {
    if((Get-FileHash -LiteralPath $Path -Algorithm SHA256 -ErrorAction Stop).Hash -ne $Pin){throw ('SESSION_PIN_DENY: '+$Path)}
}
function Assert-DocumentMetadata([string]$Path) {
    $documentAcl=Get-Acl -LiteralPath $Path -ErrorAction Stop
    if($documentAcl.GetOwner([Security.Principal.SecurityIdentifier]).Value -cne $sessionActor -or -not $documentAcl.AreAccessRulesProtected){throw 'DOCUMENT_OWNER_PROTECTION_DENY'}
    $documentRules=@($documentAcl.GetAccessRules($true,$true,[Security.Principal.SecurityIdentifier]))
    $documentSids=@($sessionActor,'S-1-5-18','S-1-5-32-544')
    if($documentRules.Count-ne3){throw 'DOCUMENT_DACL_DENY'}
    foreach($documentSid in $documentSids){
        $matchingRules=@($documentRules | Where-Object {$_.IdentityReference.Value -ceq $documentSid})
        if($matchingRules.Count-ne1){throw 'DOCUMENT_DACL_SID_DENY'}
        $r=$matchingRules[0]
        if($r.AccessControlType-ne[Security.AccessControl.AccessControlType]::Allow -or [int]$r.FileSystemRights-ne0x001f01ff -or $r.IsInherited -or [int]$r.InheritanceFlags-ne0 -or [int]$r.PropagationFlags-ne0){throw 'DOCUMENT_DACL_RIGHTS_DENY'}
    }
}
Assert-SessionPin $MyInvocation.MyCommand.Path $ExpectedScriptSha256
if([IntPtr]::Size-ne8 -or $PSVersionTable.PSEdition-ne'Desktop' -or $PSVersionTable.PSVersion.Major-ne5){throw 'WINDOWS_POWERSHELL_5_X64_REQUIRED'}
if([Environment]::MachineName -cne 'TCORP-TOBIAS' -or [Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'ORIGINAL_HOST_CONSOLE_DENY'}
$sessionImpersonated=[Security.Principal.WindowsIdentity]::GetCurrent($true)
if($null-ne$sessionImpersonated){$sessionImpersonated.Dispose();throw 'IMPERSONATION_DENY'}
$sessionIdentity=[Security.Principal.WindowsIdentity]::GetCurrent()
try {
    if($sessionIdentity.User.Value -cne $sessionActor -or -not ([Security.Principal.WindowsPrincipal]::new($sessionIdentity)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'ELEVATED_ORIGINAL_OPERATOR_DENY'}
} finally {$sessionIdentity.Dispose()}
$sessionPins=@{
    'ONE-FIRST-DIAGNOSTIC-THREE-ACTS-REQUEST.CANDIDATE.md'='25ea33a28c714952e1b1fb42202e64be7ca933afe0fa7aa37d2a5e654918dff4'
    'ACT-ENDPOINT.REQUEST.CANDIDATE.md'='27067878da3b0f8ce299ef76bf64facd343bd49d362fb0fa4b1c8262f7986160'
    'ACT-SCQ.REQUEST.CANDIDATE.md'='38027b4ce84623e2283d7300ad52ebfb57806163336c3ee24180e7a17a1a8d3f'
    'ACT-START.REQUEST.CANDIDATE.md'='e5c1559d4c9574800d588122241f1db473ac3bb6da9c223d90e8543b4f8c7ca8'
    'FIRST-START-SESSION-REQUEST.ORIGINAL.md'='312999dd9ba4f701991560e65b4a25e3a13c7914bc75603a6efb47a11536798d'
    'FIRST-START-SESSION-DECISION.ORIGINAL.txt'='7f6451d59fc0b7015e0ac612ee471782950880fcfc2013137dc9643276af641a'
    'OWNER-THREE-ACTS-OPERATIONAL-APPROVAL.txt'='c6c34866050bc0da189acd8724470838909bc7c790b838e23c46cb86ef58210f'
    'first-start-operational-decision.cs'='196ac1504eef1ce1ed7ec53f0523c1d126030be4d6f027ff93fefc44b080ec5e'
    'prepare-operational-decision.MANUAL.ps1'='8cafa124744c43a7b4160561bd175a07d4423be3a38c19be1eccc94caa15d289'
}
foreach($sessionName in $sessionPins.Keys){Assert-SessionPin (Join-Path $sessionFolder $sessionName) $sessionPins[$sessionName]}
Assert-DocumentMetadata $sessionRequest
Assert-DocumentMetadata (Join-Path $sessionFolder 'FIRST-START-SESSION-DECISION.ORIGINAL.txt')
Assert-SessionPin $sessionImage '3d59007bdc0714eb89eb646479e7a23d52fd82c953f1c72deec541a3643bce0b'
Assert-SessionPin 'C:\Program Files\Papinho\Accelerator\libssl-3-x64.dll' '3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'
Assert-SessionPin 'C:\Program Files\Papinho\Accelerator\libcrypto-3-x64.dll' '09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'
Write-Host 'ONE_MANUAL_FIRST_DIAGNOSTIC; E_Q_S_SEPARATE; NO_RETRY_NO_STOP_NO_PRODUCTION; ON_DISK_PINS_NOT_MAPPED_IMAGE_ATTESTATION'
Write-Host 'Phase 1: one auxiliary decision copy. Existing request and preparation decision are preserved.'
$decisionProcedure=Join-Path $sessionFolder 'prepare-operational-decision.MANUAL.ps1'
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $decisionProcedure -ExpectedScriptSha256 '8cafa124744c43a7b4160561bd175a07d4423be3a38c19be1eccc94caa15d289' -ExpectedRequestSha256 '312999dd9ba4f701991560e65b4a25e3a13c7914bc75603a6efb47a11536798d' -ExpectedDecisionSha256 'c6c34866050bc0da189acd8724470838909bc7c790b838e23c46cb86ef58210f' -ExpectedOperationalRequestSha256 '25ea33a28c714952e1b1fb42202e64be7ca933afe0fa7aa37d2a5e654918dff4'
$decisionNativeExit=$LASTEXITCODE
Write-Host ('MANUAL_DECISION_COPY_EXIT='+$decisionNativeExit+'; NO_RETRY; EXIT_ALONE_NOT_AUTHORITY')
if($decisionNativeExit-ne0){throw 'AUXILIARY_COPY_FAILED_OR_UNKNOWN_NO_SESSION_NO_RETRY'}
Assert-SessionPin $sessionDecision 'c6c34866050bc0da189acd8724470838909bc7c790b838e23c46cb86ef58210f'
Assert-DocumentMetadata $sessionDecision
Assert-SessionPin $sessionRequest '312999dd9ba4f701991560e65b4a25e3a13c7914bc75603a6efb47a11536798d'
Assert-SessionPin $sessionImage '3d59007bdc0714eb89eb646479e7a23d52fd82c953f1c72deec541a3643bce0b'
Assert-SessionPin 'C:\Program Files\Papinho\Accelerator\libssl-3-x64.dll' '3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'
Assert-SessionPin 'C:\Program Files\Papinho\Accelerator\libcrypto-3-x64.dll' '09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'
Write-Host 'Phase 2: original installed controller. Check every native plan and ID; cancel on mismatch. No automatic replies.'
Write-Host 'Request ID expected 417614100:1310720:120518; image ID expected 417614100:1900544:539611; new decision ID must match the copy result.'
Write-Host 'Pre-Start runtime PID/token remain unbound. Real identity, bilateral admission, receipt/close and finite shutdown are native gates, never presumed PASS.'
$sessionArguments=@(
    '--owner-manual-first-scq-start',
    $sessionRequest,
    '312999dd9ba4f701991560e65b4a25e3a13c7914bc75603a6efb47a11536798d',
    $sessionDecision,
    'c6c34866050bc0da189acd8724470838909bc7c790b838e23c46cb86ef58210f',
    '3d59007bdc0714eb89eb646479e7a23d52fd82c953f1c72deec541a3643bce0b',
    '1994944','1994944','7239172096','9823326208','128','100','60000','240000'
)
if($sessionArguments.Count-ne14){throw 'EXACT_NATIVE_ARGUMENT_COUNT_DENY'}
& $sessionImage @sessionArguments
$sessionNativeExit=$LASTEXITCODE
Write-Host ('MANUAL_FIRST_DIAGNOSTIC_NATIVE_EXIT='+$sessionNativeExit+'; NO_RETRY; NO_PRODUCTIVE_PROMOTION; OS_QUIESCENCE=UNKNOWN')
if($sessionNativeExit-ne0){throw 'FIRST_DIAGNOSTIC_FAILED_OR_UNKNOWN_NO_RETRY'}
