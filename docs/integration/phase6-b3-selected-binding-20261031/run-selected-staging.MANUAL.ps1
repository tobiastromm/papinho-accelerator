#requires -Version 5.1
#requires -RunAsAdministrator
[CmdletBinding()]
param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-fA-F0-9]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-fA-F0-9]{64}$')][string]$ExpectedApprovalSha256
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$held=[Collections.Generic.List[IO.FileStream]]::new()
$sha=$null
$exitCode=2
$invoked=$false
$closeFailed=$false
try {
 if($PSVersionTable.PSVersion.Major-ne5-or$PSVersionTable.PSEdition-ne'Desktop'-or-not[Environment]::Is64BitProcess){throw 'POWERSHELL_51_X64_DENY'}
 if([Console]::IsInputRedirected-or[Console]::IsOutputRedirected){throw 'ORIGINAL_CONSOLE_DENY'}
 if((Get-FileHash -LiteralPath $PSCommandPath -Algorithm SHA256).Hash-ne$ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
 $record=Join-Path $PSScriptRoot 'OWNER-STAGING-EXECUTION-AUTHORIZATION.json'
 if((Get-FileHash -LiteralPath $record -Algorithm SHA256).Hash-ne$ExpectedApprovalSha256){throw 'OWNER_APPROVAL_PIN_DENY'}
 $approval=Get-Content -LiteralPath $record -Raw|ConvertFrom-Json
 if($approval.decision-ne'AUTHORIZE_ONE_OWNER_MANUAL_SELECTED_STAGING'-or$approval.maximum_manual_invocations-ne1-or$approval.automatic_execution-ne$false){throw 'OWNER_APPROVAL_SCOPE_DENY'}
 $request=Join-Path $PSScriptRoot 'ONE-SELECTED-STAGING-REQUEST.CANDIDATE.md'
 if((Get-FileHash -LiteralPath $request -Algorithm SHA256).Hash-ne$approval.request_sha256){throw 'REQUEST_PIN_DENY'}
 $manifestPath=Join-Path $PSScriptRoot 'FINAL-PACKAGE.json'
 if((Get-FileHash -LiteralPath $manifestPath -Algorithm SHA256).Hash-ne$approval.package_manifest_sha256){throw 'PACKAGE_MANIFEST_DENY'}
 $manifest=Get-Content -LiteralPath $manifestPath -Raw|ConvertFrom-Json
 if(@($manifest.files).Count-ne3){throw 'PACKAGE_COUNT_DENY'}
 $base='C:\Projetos\PapinhoAccelerator\build\phase6-functional-20261028\Selected\Release'
 $names=@('papacc_server.exe','libssl-3-x64.dll','libcrypto-3-x64.dll')
 $pins=[Collections.Generic.List[string]]::new()
 $sha=[Security.Cryptography.SHA256]::Create()
 for($i=0;$i-lt3;$i++){
  $file=$manifest.files[$i]
  if($file.name-cne$names[$i]-or$file.source-cne(Join-Path $base $names[$i])){throw 'PACKAGE_PATH_DENY'}
  $ancestor=$file.source
  while($ancestor){
   $item=Get-Item -LiteralPath $ancestor -Force
   if($item.Attributes-band[IO.FileAttributes]::ReparsePoint){throw 'SOURCE_REPARSE_DENY'}
   $ancestor=[IO.Path]::GetDirectoryName($ancestor)
  }
  $stream=[IO.File]::Open($file.source,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
  $held.Add($stream)
  if($stream.Length-ne$file.bytes){throw 'PACKAGE_LENGTH_DENY'}
  $pin=([BitConverter]::ToString($sha.ComputeHash($stream))).Replace('-','').ToLowerInvariant()
  if($pin-cne$file.sha256){throw 'PACKAGE_CONTENT_DENY'}
  $pins.Add($pin)
 }
 $boot=Get-WinEvent -FilterHashtable @{LogName='System';ProviderName='Microsoft-Windows-Kernel-General';Id=12} -MaxEvents 1
 $identity=[Security.Principal.WindowsIdentity]::GetCurrent()
 try{Write-Host ('OWNER_CONTEXT_METADATA_NOT_AUTHORITY sid='+$identity.User.Value+' host='+[Environment]::MachineName+' boot_record='+$boot.RecordId+' boot_event_utc='+$boot.TimeCreated.ToUniversalTime().ToString('o'))}finally{$identity.Dispose()}
 Write-Host 'ONE_OWNER_MANUAL_SELECTED_STAGING; OPAQUE_RISK_ACCEPTED; NATIVE_CONFIRMATION_REQUIRED; NO_RETRY; NOT_PRODUCTION; ON_DISK_PINS_NOT_MAPPED_IMAGE_ATTESTATION'
 # Exactly one native invocation. DATA pins are authenticated by the original
 # console/token/plan confirmation inside the same canonical native owner.
 $invoked=$true
 & (Join-Path $base $names[0]) '--owner-manual-selected-staging' $pins[0] $pins[1] $pins[2]
 $exitCode=$LASTEXITCODE
} catch {
 Write-Host ('MANUAL_SELECTED_STAGING_FIRST='+$_.Exception.GetType().FullName+'; MESSAGE='+$_.Exception.Message+'; NO_RETRY')
 $exitCode=2
} finally {
 if($null-ne$sha){try{$sha.Dispose()}catch{$closeFailed=$true}}
 for($i=$held.Count-1;$i-ge0;$i--){try{$held[$i].Dispose()}catch{$closeFailed=$true}}
 if($closeFailed){$exitCode=2}
 Write-Host ('MANUAL_SELECTED_STAGING_EXIT='+$exitCode+'; INVOKED='+$invoked+'; WRAPPER_CLOSE_UNCERTAIN='+$closeFailed+'; OS_QUIESCENCE=UNKNOWN; NO_RETRY; NO_PRODUCTIVE_PROMOTION')
}
exit $exitCode
