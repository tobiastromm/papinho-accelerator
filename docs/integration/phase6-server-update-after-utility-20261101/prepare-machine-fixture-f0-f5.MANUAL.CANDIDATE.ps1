# CANDIDATE: one owner-approved MANUAL preparation; never run automatically.
#requires -Version 5.1
[CmdletBinding()]
param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedOperationalRequestSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedDecisionSha256
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$folder='C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
if($PSVersionTable.PSEdition -ne 'Desktop' -or $PSVersionTable.PSVersion.Major -ne 5 -or -not [Environment]::Is64BitProcess){throw 'POWERSHELL_51_AMD64_DENY'}
if([Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'ORIGINAL_CONSOLE_DENY'}
if((Get-FileHash -LiteralPath $PSCommandPath -Algorithm SHA256).Hash -ne $ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
$request=Join-Path $folder 'ONE-MACHINE-FIXTURE-F0-F5-PREPARATION-REQUEST.CANDIDATE.md'
$decision=Join-Path $folder 'OWNER-MACHINE-FIXTURE-F0-F5-PREPARATION-APPROVAL.txt'
if((Get-FileHash -LiteralPath $request -Algorithm SHA256).Hash -ne $ExpectedOperationalRequestSha256 -or (Get-FileHash -LiteralPath $decision -Algorithm SHA256).Hash -ne $ExpectedDecisionSha256){throw 'ORIGINAL_DOCUMENT_PIN_DENY'}
$copy=Join-Path $folder 'prepare-machine-fixture-f0-f5-decision.MANUAL.CANDIDATE.ps1'
if((Get-FileHash -LiteralPath $copy -Algorithm SHA256).Hash -ne '4232f25e1735d4dcae3d3c19d23de3cbd3f52052339196d38f7d47482e4f53e2'){throw 'COPY_PROCEDURE_PIN_DENY'}
Write-Host 'ONE_MANUAL_MACHINE_FIXTURE_PREPARATION; ONE_NEW_DECISION_THEN_ONE_FIXTURE_PRESERVE_HISTORICAL_PAIR; SEPARATE_CONFIRMATIONS_CONSUMPTIONS; NO_RETRY_NO_DELETE_NO_E_Q_S_P_NO_START_NO_UPDATE'
Write-Host 'Opaque costs UNKNOWN under accepted T0-T6/F0-F5. Fixture entry128/guard188/cap192, commit3221225472, physical7239172096, growth1994944. No inherited K7 or readiness. A denial can occur after ONE decision copy; preserve it and do not rerun.'
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $copy -ExpectedScriptSha256 '4232f25e1735d4dcae3d3c19d23de3cbd3f52052339196d38f7d47482e4f53e2' -ExpectedRequestSha256 'fc91b7bce523b014c28ce677b60ea889e32c8bc50c39c83b264096d6f923ab5c' -ExpectedDecisionSha256 $ExpectedDecisionSha256 -ExpectedOperationalRequestSha256 $ExpectedOperationalRequestSha256
$copyExit=$LASTEXITCODE
Write-Host ('MACHINE_FIXTURE_DOCUMENTS_MANUAL_EXIT='+$copyExit+'; EXIT_NOT_AUTHORITY; NO_RETRY')
if($copyExit -ne 0){throw 'NEW_DECISION_COPY_FAILED_OR_UNKNOWN_NO_RETRY'}
$target='C:\Program Files\Papinho\Accelerator'
$exe=Join-Path $target 'papacc_server.exe'
$originalRequest=Join-Path $target 'MACHINE-TEST-ONLY-FIXTURE-REQUEST.ORIGINAL.md'
$originalDecision=Join-Path $target 'MACHINE-TEST-ONLY-FIXTURE-F0-F5-DECISION.ORIGINAL.txt'
$streams=New-Object 'System.Collections.Generic.List[IO.FileStream]'
$hashes=@('e4a36490737eb7e1896997f873e5e69511a7a6a65291b0f44399b4ed18f3b676','3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed','09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044')
$names=@('papacc_server.exe','libssl-3-x64.dll','libcrypto-3-x64.dll')
$lengths=@(871936,1321472,7217664)
$invoked=$false;$nativeExit=2
try {
 for($i=0;$i -lt 3;$i++){
  $stream=[IO.File]::Open((Join-Path $target $names[$i]),[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
  $streams.Add($stream);$hash=[Security.Cryptography.SHA256]::Create()
  try {$pin=([BitConverter]::ToString($hash.ComputeHash($stream))).Replace('-','').ToLowerInvariant()}finally{$hash.Dispose()}
  if($stream.Length -ne $lengths[$i] -or $pin -cne $hashes[$i]){throw 'PROTECTED_BINARY_PIN_DENY'}
 }
 Write-Host 'Compare original request ID417614100:6815744:9313, image ID417614100:5373952:164063 and the NEW decision ID/hash with copy output and native plan. No response supplied automatically. Fixture confirmation is independent. Name/public SHA are future native results, never prefilled.'
 $arguments=@('--owner-manual-machine-fixture-prepare',$originalRequest,'fc91b7bce523b014c28ce677b60ea889e32c8bc50c39c83b264096d6f923ab5c',$originalDecision,$ExpectedDecisionSha256,$hashes[0],$hashes[1],$hashes[2])
 if($arguments.Count -ne 8){throw 'EXACT_FIXTURE_ARGV_DENY'}
 $invoked=$true
 & $exe @arguments
 $nativeExit=$LASTEXITCODE
}finally{
 for($i=$streams.Count-1;$i -ge 0;$i--){$streams[$i].Dispose()}
 Write-Host ('MANUAL_MACHINE_FIXTURE_EXIT='+$nativeExit+'; INVOKED='+$invoked+'; EXIT_NOT_AUTHORITY; NO_RETRY_NO_DELETE_NO_E_Q_S_P_NO_START; HISTORICAL_RETENTION_AND_OS_QUIESCENCE_UNKNOWN')
}
if($nativeExit -ne 0){throw 'MACHINE_FIXTURE_FAILED_OR_UNKNOWN_NO_RETRY'}
