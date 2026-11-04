#requires -Version 5.1
#requires -RunAsAdministrator
# MANUAL ONLY. No execution approval is stored or inferred here.
param([Parameter(Mandatory=$true)][ValidatePattern('^[a-fA-F0-9]{64}$')][string]$ExpectedScriptSha256)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$nativeExit=2
try {
 if ($PSVersionTable.PSVersion.Major -ne 5 -or $PSVersionTable.PSEdition -ne 'Desktop' -or -not [Environment]::Is64BitProcess) { throw 'POWERSHELL_51_X64_REQUIRED' }
 if ((Get-FileHash -LiteralPath $PSCommandPath -Algorithm SHA256).Hash.ToLowerInvariant() -ne $ExpectedScriptSha256.ToLowerInvariant()) { throw 'SCRIPT_HASH_DENY' }
 $base='C:\Projetos\PapinhoAccelerator\build\phase6-functional-20261028\Selected\Release'
 $files=@(
  @{name='papacc_server.exe';bytes=552960;sha256='74f430e0bd86529396bd267c8ca40548218dc83611ef126c24e72da70388e8ad'},
  @{name='libssl-3-x64.dll';bytes=1321472;sha256='3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'},
  @{name='libcrypto-3-x64.dll';bytes=7217664;sha256='09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'}
 )
 foreach ($file in $files) {
  $path=Join-Path $base $file.name
  $item=Get-Item -LiteralPath $path -Force
  if ($item.PSIsContainer -or ($item.Attributes -band [IO.FileAttributes]::ReparsePoint) -or $item.Length -ne $file.bytes) { throw 'PACKAGE_SHAPE_DENY' }
  if ((Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash.ToLowerInvariant() -ne $file.sha256) { throw 'PACKAGE_HASH_DENY' }
 }
 Write-Output 'MANUAL_STAGING_RESOURCE_OBSERVATION; ON_DISK_PINS_CHECKED_NOT_MAPPED_IMAGE_ATTESTATION; NO_RNG_NO_CONFIRMATION_NO_STAGING_NO_SCM_NO_RETRY; AUTHORITY_DENY'
 $executable=Join-Path $base $files[0].name
 & $executable '--qualification-staging-observe'
 $nativeExit=$LASTEXITCODE
 Write-Output ('STAGING_OBSERVATION_NATIVE_EXIT='+$nativeExit+'; EXIT_ZERO_MEANS_COLLECTION_ONLY; AUTHORITY_DENY; NO_RETRY')
} catch {
 Write-Output ('STAGING_OBSERVATION_WRAPPER_FIRST='+$_.Exception.GetType().FullName+'; AUTHORITY_DENY; NO_RETRY')
 $nativeExit=2
}
exit $nativeExit
