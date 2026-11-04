param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedRequestSha256
)
# CANDIDATE: requires separate owner approval of the exact request. Do not run
# as a test. Hash comparisons are DATA, not administrative authority.
$ErrorActionPreference = 'Stop'
$updateInvoked = $false
$updateCloseUncertain = $false
$updateExit = 2
try {
 if ($PSVersionTable.PSEdition -ne 'Desktop' -or $PSVersionTable.PSVersion.Major -ne 5 -or $PSVersionTable.PSVersion.Minor -ne 1 -or -not [Environment]::Is64BitProcess) { throw 'POWERSHELL_51_X64_DENY' }
 if ((Get-FileHash -LiteralPath $PSCommandPath -Algorithm SHA256).Hash -ne $ExpectedScriptSha256) { throw 'SCRIPT_HASH_DENY' }
 $updateRequest = Join-Path $PSScriptRoot 'ONE-BINARY-UPDATE-REQUEST.CANDIDATE.md'
 if ((Get-FileHash -LiteralPath $updateRequest -Algorithm SHA256).Hash -ne $ExpectedRequestSha256) { throw 'REQUEST_HASH_DENY' }
 if ([Environment]::MachineName -cne 'TCORP-TOBIAS') { throw 'HOST_METADATA_DENY' }
 $updateIdentity = [Security.Principal.WindowsIdentity]::GetCurrent()
 try {
  if ($updateIdentity.User.Value -cne 'S-1-5-21-785699550-1440458426-1682014700-1108') { throw 'OPERATOR_DENY' }
  $updatePrincipal = New-Object Security.Principal.WindowsPrincipal($updateIdentity)
  if (-not $updatePrincipal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) { throw 'ADMIN_CONTEXT_DENY' }
 } finally { $updateIdentity.Dispose() }
 if ([Environment]::GetFolderPath([Environment+SpecialFolder]::ProgramFiles) -cne 'C:\Program Files') { throw 'KNOWN_FOLDER_DENY' }
 $updateDirectory = 'C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release'
 $updateTarget = 'C:\Program Files\Papinho\Accelerator'
 $updateExe = Join-Path $updateDirectory 'papacc_server.exe'
 $updateNewPin = '42d468faf367447ac8416194cadb5d2b0474c91aa3b7953838b310e65b0f29ca'
 $updateOldPin = 'c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf'
 $updateSslPin = '3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'
 $updateCryptoPin = '09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'
 $updateFiles = @(
  @{ Path=$updateExe; Bytes=737280; Pin=$updateNewPin },
  @{ Path=(Join-Path $updateTarget 'papacc_server.exe'); Bytes=571392; Pin=$updateOldPin },
  @{ Path=(Join-Path $updateDirectory 'libssl-3-x64.dll'); Bytes=1321472; Pin=$updateSslPin },
  @{ Path=(Join-Path $updateDirectory 'libcrypto-3-x64.dll'); Bytes=7217664; Pin=$updateCryptoPin },
  @{ Path=(Join-Path $updateTarget 'libssl-3-x64.dll'); Bytes=1321472; Pin=$updateSslPin },
  @{ Path=(Join-Path $updateTarget 'libcrypto-3-x64.dll'); Bytes=7217664; Pin=$updateCryptoPin }
 )
 foreach ($updateFile in $updateFiles) {
  $updateItem = Get-Item -LiteralPath $updateFile.Path -ErrorAction Stop
  if ($updateItem.PSIsContainer -or ($updateItem.Attributes -band [IO.FileAttributes]::ReparsePoint) -or $updateItem.Length -ne $updateFile.Bytes) { throw 'FILE_METADATA_DENY' }
  if ((Get-FileHash -LiteralPath $updateFile.Path -Algorithm SHA256).Hash -ne $updateFile.Pin) { throw 'FILE_HASH_DENY' }
 }
 Write-Host 'ONE_MANUAL_BINARY_UPDATE; U1_U7_OPAQUE_RISK; HASHES_NOT_AUTHORITY; NATIVE_ORIGINAL_GATES_AND_CONFIRMATION_REQUIRED; NO_RETRY_NO_ROLLBACK_NO_START; ON_DISK_PINS_NOT_MAPPED_IMAGE_ATTESTATION'
 Write-Host 'Confirm only the exact approved native plan, parent/file IDs, pins, two exclusive leaves, K/R and retained-old effects. Cancel on any difference. No response is supplied automatically.'
 $updateArguments = @('--owner-manual-binary-update',$updateExe,
  '417614100','8257536','71209','417614100','6225920','71237',
  $updateOldPin,$updateNewPin,$updateSslPin,$updateCryptoPin)
 $updateInvoked = $true # authorization attempt is not rearmed by native DENY
 & $updateExe @updateArguments
 $updateExit = $LASTEXITCODE
} catch {
 if ($updateInvoked) { $updateCloseUncertain = $true }
 Write-Host ('BINARY_UPDATE_WRAPPER_DENY: ' + $_.Exception.Message)
} finally {
 Write-Host ('MANUAL_BINARY_UPDATE_EXIT={0}; INVOKED={1}; WRAPPER_CLOSE_UNCERTAIN={2}; OS_QUIESCENCE=UNKNOWN; NO_RETRY_NO_ROLLBACK_NO_START_NO_PRODUCTIVE_PROMOTION' -f $updateExit,$updateInvoked,$updateCloseUncertain)
}
exit $updateExit
