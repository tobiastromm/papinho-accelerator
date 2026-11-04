# CANDIDATE: one owner-approved MANUAL preparation; never run automatically.
#requires -Version 5.1
[CmdletBinding()]
param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedRequestSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedDecisionSha256
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$folder='C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
if($PSVersionTable.PSEdition -ne 'Desktop' -or $PSVersionTable.PSVersion.Major -ne 5 -or -not [Environment]::Is64BitProcess){throw 'POWERSHELL_51_AMD64_DENY'}
if([Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'ORIGINAL_CONSOLE_DENY'}
if((Get-FileHash -LiteralPath $PSCommandPath -Algorithm SHA256).Hash -ne $ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
$request=Join-Path $folder 'ONE-MACHINE-FIXTURE-PREPARATION-REQUEST.CANDIDATE.md'
$decision=Join-Path $folder 'OWNER-MACHINE-FIXTURE-PREPARATION-APPROVAL.txt'
if((Get-FileHash -LiteralPath $request -Algorithm SHA256).Hash -ne $ExpectedRequestSha256 -or (Get-FileHash -LiteralPath $decision -Algorithm SHA256).Hash -ne $ExpectedDecisionSha256){throw 'ORIGINAL_DOCUMENT_PIN_DENY'}
$copy=Join-Path $folder 'prepare-machine-fixture-originals.MANUAL.CANDIDATE.ps1'
if((Get-FileHash -LiteralPath $copy -Algorithm SHA256).Hash -ne '7adb76b47324e88d0d21de5a53cabf1b99b107dbd70bf216f862b12be97e9fcc'){throw 'COPY_PROCEDURE_PIN_DENY'}
Write-Host 'ONE_MANUAL_MACHINE_FIXTURE_PREPARATION; TWO_NEW_ORIGINALS_THEN_ONE_FIXTURE; SEPARATE_CONFIRMATIONS_CONSUMPTIONS; NO_RETRY_NO_DELETE_NO_E_Q_S_P_NO_START_NO_UPDATE'
Write-Host 'Opaque costs UNKNOWN under accepted T0-T6 only. Native fixture guards remain124/128, commit9823326208, physical7239172096, growth1994944; no K7 fallback. A denial can occur after documentary copies; preserve them and do not rerun.'
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $copy -ExpectedScriptSha256 '7adb76b47324e88d0d21de5a53cabf1b99b107dbd70bf216f862b12be97e9fcc' -ExpectedRequestSha256 $ExpectedRequestSha256 -ExpectedDecisionSha256 $ExpectedDecisionSha256
$copyExit=$LASTEXITCODE
Write-Host ('MACHINE_FIXTURE_DOCUMENTS_MANUAL_EXIT='+$copyExit+'; EXIT_NOT_AUTHORITY; NO_RETRY')
if($copyExit -ne 0){throw 'DOCUMENT_PAIR_FAILED_OR_UNKNOWN_NO_RETRY'}
$target='C:\Program Files\Papinho\Accelerator'
$exe=Join-Path $target 'papacc_server.exe'
$originalRequest=Join-Path $target 'MACHINE-TEST-ONLY-FIXTURE-REQUEST.ORIGINAL.md'
$originalDecision=Join-Path $target 'MACHINE-TEST-ONLY-FIXTURE-DECISION.ORIGINAL.txt'
$streams=New-Object 'System.Collections.Generic.List[IO.FileStream]'
$hashes=@('6063d3692f22cfd3baa7e5128b5b787ce6ce33ddb362e805897323f39884599a','3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed','09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044')
$names=@('papacc_server.exe','libssl-3-x64.dll','libcrypto-3-x64.dll')
$lengths=@(871424,1321472,7217664)
$invoked=$false;$nativeExit=2
try {
 for($i=0;$i -lt 3;$i++){
  $stream=[IO.File]::Open((Join-Path $target $names[$i]),[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
  $streams.Add($stream);$hash=[Security.Cryptography.SHA256]::Create()
  try {$pin=([BitConverter]::ToString($hash.ComputeHash($stream))).Replace('-','').ToLowerInvariant()}finally{$hash.Dispose()}
  if($stream.Length -ne $lengths[$i] -or $pin -cne $hashes[$i]){throw 'PROTECTED_BINARY_PIN_DENY'}
 }
 Write-Host 'Compare every new document ID/hash with copy output and native plan. No response supplied automatically. Fixture confirmation is independent. Name/public SHA are future native results, never prefilled.'
 $arguments=@('--owner-manual-machine-fixture-prepare',$originalRequest,$ExpectedRequestSha256,$originalDecision,$ExpectedDecisionSha256,$hashes[0],$hashes[1],$hashes[2])
 if($arguments.Count -ne 8){throw 'EXACT_FIXTURE_ARGV_DENY'}
 $invoked=$true
 & $exe @arguments
 $nativeExit=$LASTEXITCODE
}finally{
 for($i=$streams.Count-1;$i -ge 0;$i--){$streams[$i].Dispose()}
 Write-Host ('MANUAL_MACHINE_FIXTURE_EXIT='+$nativeExit+'; INVOKED='+$invoked+'; EXIT_NOT_AUTHORITY; NO_RETRY_NO_DELETE_NO_E_Q_S_P_NO_START; HISTORICAL_RETENTION_AND_OS_QUIESCENCE_UNKNOWN')
}
if($nativeExit -ne 0){throw 'MACHINE_FIXTURE_FAILED_OR_UNKNOWN_NO_RETRY'}
