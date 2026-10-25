#requires -Version 5.1
[CmdletBinding()]
param([ValidateSet('Check','Plan')][string]$Mode='Check')
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
# Deliberately unarmed: this preparation cannot start/reconfigure a service.
$Root='C:\Projetos\PapinhoAccelerator'
$ScriptPath=$Root+'\tools\qualification\scm-lab-runtime-preflight.ps1'
$Candidate=$Root+'\build\windows-virtual-service-lab-runtime-20261023\Selected\Release\papacc_server_lab_host.exe'
$CandidateHash='f101a0a6276cb4683f2aed2985c4d22613b78ffdc32d2c277cd3e88f2d538449'
$Registered=$Root+'\build\scm-protected-installation-binding-20261022\Selected\Release\papacc_server.exe'
$Config=$Root+'\build\scm-protected-installation-binding-20261022\Selected\Release\papacc.conf'
function Open-Pinned([string]$Path,[string]$Hash){
 if([IO.Path]::GetFullPath($Path) -cne $Path){throw 'NONCANONICAL_PATH'}
 $p=$Path
 while($p){if(([IO.File]::GetAttributes($p) -band [IO.FileAttributes]::ReparsePoint) -ne 0){throw 'REPARSE_PATH'};$p=[IO.Path]::GetDirectoryName($p)}
 $f=[IO.File]::Open($Path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 try{
  $sha=[Security.Cryptography.SHA256]::Create()
  try{$actual=([BitConverter]::ToString($sha.ComputeHash($f))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
  if($actual -cne $Hash){throw 'HASH_PIN_MISMATCH'}
  return $f
 }catch{$f.Dispose();throw}
}
$pins=@();$closed=$true
try{
 if($PSCommandPath -cne $ScriptPath){throw 'SCRIPT_PATH'}
 $pins+=Open-Pinned $Candidate $CandidateHash
 $pins+=Open-Pinned $Registered '3737e83be95613a2616b9fb764fe95d72796912746e9440e37b3ed2a77e01abb'
 $pins+=Open-Pinned $Config 'a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44'
 Write-Output 'ARTIFACT_PINS=PASS; SERVICE=PapinhoAcceleratorLab20261023; ACCOUNT=NT SERVICE\PapinhoAcceleratorLab20261023'
 Write-Output ('CANDIDATE='+$Candidate+'; SHA256='+$CandidateHash)
 Write-Output 'SCM_CURRENT_STATE=NOT_QUERIED; EXPECTED_REGISTERED_IMAGE_IS_DIFFERENT; SCM_MUTATIONS=NONE'
 if($Mode -eq 'Plan'){
  Write-Output 'FUTURE_ONLY: exact protected image/config rebinding approval; token/ACL/CA-denial producer; listenerless shared-runtime readiness and ED11 gate; manual UAC/full plan confirmation; START -> factual STATUS -> STOP -> factual quiescence.'
 }
 Write-Output 'START_STOP_LAUNCHER=UNARMED; PROTECTED_RUNTIME_PRODUCER=ABSENT; NATIVE_TOKEN_ACL=NOT_QUALIFIED; RUNTIME_START=DENY'
}catch{Write-Output ('PREFLIGHT=DENY; REASON='+$_.Exception.Message)}
finally{
 for($i=$pins.Count-1;$i -ge 0;$i--){try{$pins[$i].Dispose()}catch{$closed=$false}}
 if(-not $closed){Write-Output 'PIN_CLOSE=UNKNOWN; NO_RETRY'}
}
# Artifact checks never authorize runtime or lift any product gate.
exit 2
