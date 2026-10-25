#requires -Version 5.1
[CmdletBinding()]
param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidateRange(1,4294967295)][uint32]$AuxiliaryPid,
 [Parameter(Mandatory=$true)][uint64]$AuxiliaryCreation
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$Root='C:\Projetos\PapinhoAccelerator'
$ExpectedPath=$Root+'\tools\qualification\scm-n0-protected-single-powershell.ps1'
$Qualifier='C:\Projetos\PapinhoAccelerator\build\n0-n9-protected-dossier-readonly-binding-20261024\Selected\Release\papacc_scm_instrumented_lab.exe'
$QualifierHash='9abce35a948ec1f7dfaee1e0256c11c8d2c56bb72bae0d6d695d8bcf6fad66f0'
# Empty until independently reviewed factual evidence exists; no bypass parameter.
$DossierHash=''
$EvidenceHash=''
$Attempt='75330d11cc7e4123ae62eadc9a59c2f5'
$Operator='S-1-5-21-785699550-1440458426-1682014700-1108'
$script:N0RetainedProcess=$null
$script:N0RetainedPins=$null
$pins=[Collections.Generic.List[IDisposable]]::new()
$process=$null;$reader=$null;$started=$false;$returned=$false;$final=$false;$closeFailed=$false;$exitCode=2
function Hash-Stream($f) {
 $sha=[Security.Cryptography.SHA256]::Create()
 try {$f.Position=0;$v=([BitConverter]::ToString($sha.ComputeHash($f))).Replace('-','').ToLowerInvariant();$f.Position=0;return $v}finally{$sha.Dispose()}
}
function Pin([string]$path,[string]$hash) {
 if($hash -cnotmatch '^[0-9a-f]{64}$'){throw 'MISSING_QUALIFIED_PIN'}
 if([IO.Path]::GetFullPath($path) -cne $path){throw 'NONCANONICAL_PIN_PATH'}
 $walk=$path
 while($walk){if(([IO.File]::GetAttributes($walk) -band [IO.FileAttributes]::ReparsePoint) -ne 0){throw 'REPARSE_PIN_PATH'};$walk=[IO.Path]::GetDirectoryName($walk)}
 $file=[IO.File]::Open($path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 $pins.Add($file)
 if((Hash-Stream $file) -cne $hash){throw 'HASH_DIVERGENCE'}
}
try {
 if($PSCommandPath -cne $ExpectedPath -or [IntPtr]::Size -ne 8 -or $PSVersionTable.PSEdition -cne 'Desktop'){throw 'SCRIPT_OR_RUNTIME_DIVERGENCE'}
 Pin $ExpectedPath $ExpectedScriptSha256
 $identity=[Security.Principal.WindowsIdentity]::GetCurrent()
 try {if($identity.User.Value -cne $Operator -or -not ([Security.Principal.WindowsPrincipal]::new($identity)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'OPERATOR_OR_ELEVATION_DENY'}}finally{$identity.Dispose()}
 $imp=[Security.Principal.WindowsIdentity]::GetCurrent($true)
 if($null -ne $imp){$imp.Dispose();throw 'IMPERSONATION_DENY'}
 if([Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'INTERACTIVE_CONSOLE_REQUIRED'}
 if($DossierHash -cnotmatch '^[0-9a-f]{64}$' -or $EvidenceHash -cnotmatch '^[0-9a-f]{64}$'){throw 'WINDOWS_PROVENANCE_UNKNOWN_DENY'}
 Pin 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' '9785001b0dcf755eddb8af294a373c0b87b2498660f724e76c4d53f9c217c7a3'
 Pin $Qualifier $QualifierHash
 Pin 'C:\ProgramData\PapinhoAccelerator\Protected\N0\papacc-n2-75330d11cc7e4123ae62eadc9a59c2f5.approved' $DossierHash
 Pin 'C:\ProgramData\PapinhoAccelerator\Protected\N0\papacc-n2-75330d11cc7e4123ae62eadc9a59c2f5.evidence' $EvidenceHash
 Pin ($Root+'\docs\adr\DECISION-first-native-lab-diagnostic-boundary-20261024.md') 'bec9d0c275d5ee8bd68de4d42fe173a08f124a9900c119e7a08d23757d9d7941'
 Pin ($Root+'\docs\adr\DECISION-canonical-lab-console-topology-bd-20261023.md') 'eb93002cf3f6d863a060fb97b7380389aa66b6fbe68e66c1af4fb17b988d3290'
 Pin ($Root+'\docs\adr\DECISION-ed11-single-lab-diagnostic-native-boundary-20261023.md') 'e7031bd29c9bd174359ffcc3c9a021a6ad25a0256a5cf03b40c0866d3a5776b7'
 Pin ($Root+'\docs\adr\DECISION-n0-n9-security-descriptor-digest-profile-20261024.md') '8b042e2bbf6500aa90aa3407fcd3423f45d96cf6b16568ffa95b98f3d081c972'
 $self=[Diagnostics.Process]::GetCurrentProcess()
 try {$birth=[uint64]$self.StartTime.ToUniversalTime().ToFileTimeUtc();if($self.MainModule.FileName -cne 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe'){throw 'ACTOR_IMAGE_DENY'}}finally{$self.Dispose()}
 Write-Host ('READ-ONLY N0-N9; ATTEMPT='+$Attempt+'; auxiliary PID='+$AuxiliaryPid+' creation='+$AuxiliaryCreation)
 Write-Host 'Existing owner/mutex/census/metrics only. No Start/Stop/rebind/ACL/intent or operational diagnostic file. Missing facts DENY. No retry.'
 $confirmation='PREPARE_N0_READ_ONLY '+$Attempt+' '+$AuxiliaryPid+' '+$AuxiliaryCreation
 Write-Host ('Type exactly: '+$confirmation)
 if((Read-Host) -cne $confirmation){throw 'CONFIRMATION_DIVERGENCE'}
 [Threading.Thread]::BeginThreadAffinity()
 try {
  # Framework OS thread ID is a candidate only; native owner rechecks original
  # thread/process identity, lifetime, no impersonation. Never ManagedThreadId.
  $tid=[uint32][AppDomain]::GetCurrentThreadId()
  $start=[Diagnostics.ProcessStartInfo]::new();$start.FileName=$Qualifier
  $start.Arguments='--prepare-n0 '+$Attempt+' '+$PID+' '+$birth+' '+$tid+' '+$AuxiliaryPid+' '+$AuxiliaryCreation
  $start.UseShellExecute=$false;$start.CreateNoWindow=$false
  $start.RedirectStandardOutput=$true;$start.RedirectStandardError=$false;$start.RedirectStandardInput=$false
  $process=[Diagnostics.Process]::new();$process.StartInfo=$start
  if(-not $process.Start()){throw 'NATIVE_CREATE_FAILED'};$started=$true;$reader=$process.StandardOutput
  $chars=0
  while($null -ne ($line=$reader.ReadLine())) {
   $chars+=$line.Length
   if($line.Length -gt 4096 -or $chars -gt 131072){throw 'OUTPUT_BOUND_UNKNOWN_RETAIN'}
   Write-Host $line
   if($line -cmatch '^IG_FINAL stage=\S+ first=\d+ close=0 starts=0 stops=0 quiescent=0 intent_consumed=0 effect=0 diagnostic_created=0;'){$final=$true}
  }
  $process.WaitForExit();$exitCode=$process.ExitCode;$returned=$true
  if(-not $final){throw 'FACTUAL_FINAL_RECEIPT_MISSING_RETAIN'}
 } finally {[Threading.Thread]::EndThreadAffinity()}
} catch {$exitCode=2;Write-Host ('N0_READ_ONLY_ERROR='+$_.Exception.Message)} finally {
 if($started -and (-not $returned -or -not $final)) {
  $script:N0RetainedProcess=$process;$script:N0RetainedPins=$pins
  Write-Host 'RESOURCE_RETENTION=UNKNOWN; NO_CLOSE_NO_RETRY_NO_KILL; DO_NOT_CLOSE_OPERATOR'
  while($true){Start-Sleep -Seconds 1}
 } else {
  try {if($reader){$reader.Dispose()};if($process){$process.Dispose()};foreach($pin in $pins){$pin.Dispose()}}catch{$exitCode=2;$closeFailed=$true;$script:N0RetainedPins=$pins;Write-Host 'CLOSE_UNKNOWN_NO_RETRY';while($true){Start-Sleep -Seconds 1}}
  if(-not $closeFailed -and $started -and $returned -and $final) {
   Write-Host 'Qualifier returned with factual final close receipt. F10 returns to this existing operator console; no retry.'
   do {$key=[Console]::ReadKey($true)}while($key.Key -ne [ConsoleKey]::F10)
  }
 }
}
$global:LASTEXITCODE=$exitCode
Write-Host ('N0_READ_ONLY_EXIT='+$exitCode+'; START_EXECUTED=NO; ED11=PARTIAL; AUTHORITY=DENY')
# Invocation is & script in THIS already elevated PowerShell, never a new powershell.exe.
