# Candidate read-only collection. Never invokes the operational SCQ entrypoint.
#requires -Version 5.1
param([Parameter(Mandatory=$true)][string]$ExpectedScriptSha256,[Parameter(Mandatory=$true)][string]$ExpectedRequestSha256)
$ErrorActionPreference='Stop'
if([IntPtr]::Size-ne8 -or $PSVersionTable.PSEdition-ne'Desktop' -or $PSVersionTable.PSVersion.Major-ne5){throw 'POWERSHELL_51_X64_DENY'}
if((Get-FileHash -LiteralPath $PSCommandPath).Hash-ne$ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
if((Get-FileHash -LiteralPath (Join-Path $PSScriptRoot 'ONE-COLD-BOUNDARY-OBSERVATION-REQUEST.CANDIDATE.md')).Hash-ne$ExpectedRequestSha256){throw 'REQUEST_PIN_DENY'}
if([Environment]::MachineName-cne'TCORP-TOBIAS' -or [Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'ORIGINAL_CONTEXT_DENY'}
$id=[Security.Principal.WindowsIdentity]::GetCurrent()
try {if($id.User.Value-cne'S-1-5-21-785699550-1440458426-1682014700-1108' -or -not ([Security.Principal.WindowsPrincipal]::new($id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'ADMIN_CONTEXT_DENY'}}finally{$id.Dispose()}
$imp=[Security.Principal.WindowsIdentity]::GetCurrent($true)
if($null-ne$imp){$imp.Dispose();throw 'IMPERSONATION_DENY'}
$folder='C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release'
$pins=@{
 'papacc_server.exe'='714fedbff069af841766d158bd4f833eab6d066e2ebe37c8995aa161d6c3d546'
 'libssl-3-x64.dll'='3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'
 'libcrypto-3-x64.dll'='09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'
}
$held=[Collections.Generic.List[IO.FileStream]]::new();$closed=$true;$invoked=$false;$result=2
try {
 foreach($name in $pins.Keys){
  $file=[IO.File]::Open((Join-Path $folder $name),[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read);$held.Add($file)
  $sha=[Security.Cryptography.SHA256]::Create()
  try{$actual=([BitConverter]::ToString($sha.ComputeHash($file))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
  if($actual-cne$pins[$name]){throw 'OBSERVATION_IMAGE_PIN_DENY'}
  if($name-ceq'papacc_server.exe' -and $file.Length-ne765952){throw 'OBSERVATION_SIZE_DENY'}
 }
 Write-Host 'ONE_READONLY_COLD_BOUNDARY_OBSERVATION; WORKSPACE_DIAGNOSTIC_NOT_ADMIN_AUTHORITY; NO_RETRY_NO_SCM_NO_ENDPOINT_NO_START; ON_DISK_PINS_NOT_MAPPED_IMAGE_ATTESTATION'
 $invoked=$true
 & (Join-Path $folder 'papacc_server.exe') '--observe-cold-handle-boundaries'
 $result=$LASTEXITCODE
} finally {
 for($i=$held.Count-1;$i-ge0;$i--){try{$held[$i].Dispose()}catch{$closed=$false}}
 Write-Host ('COLD_BOUNDARY_MANUAL_EXIT={0}; INVOKED={1}; WRAPPER_CLOSE_KNOWN={2}; NOT_CAPACITY; OS_QUIESCENCE=UNKNOWN; NO_RETRY' -f $result,$invoked,$closed)
}
if(-not$closed){exit 2}
exit $result