#requires -Version 5.1
[CmdletBinding()]
param([ValidateSet('Check','Launch','Elevated')][string]$Mode='Check',[ValidatePattern('^$|^[0-9a-f]{64}$')][string]$ExpectedScriptSha256='')
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$Root='C:\Projetos\PapinhoAccelerator'
$Script=$Root+'\tools\qualification\scm-lab-first-cycle-uac.ps1'
$PSExe='C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe'
$Exe=$Root+'\build\windows-service-protected-runtime-20261023\Selected\Release\papacc_scm_runtime_cycle_lab.exe'
$ExeHash='219d13960ac3bfdc98cb94c5e3baa8e834d8f5b52afe731006cc47ec1e02387d'
$Candidate=$Root+'\build\windows-service-protected-runtime-20261023\Selected\Release\papacc_server_lab_host.exe'
$CandidateHash='62087dbe13712c468dc92c5848de684cbc7cba6f921d15259f895fa8664d2716'
$ReceiptRoot=$Root+'\docs\integration\first-native-lab-service-start-stop-20261023'
$Prior=$Root+'\docs\integration\scm-lab-dependency-acl-correction-20261023\manual-result.json'
$ExpectedSid='S-1-5-21-785699550-1440458426-1682014700-1108'
function Assert-Plain([string]$Path){if([IO.Path]::GetFullPath($Path) -cne $Path){throw 'NONCANONICAL_PATH'};$p=$Path;while($p){if(([IO.File]::GetAttributes($p) -band [IO.FileAttributes]::ReparsePoint) -ne 0){throw 'REPARSE_PATH'};$p=[IO.Path]::GetDirectoryName($p)}}
function Hash-Stream($f){$sha=[Security.Cryptography.SHA256]::Create();try{return ([BitConverter]::ToString($sha.ComputeHash($f))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}}
function Path-Hash([string]$Path){$f=[IO.File]::Open($Path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read);try{return Hash-Stream $f}finally{$f.Dispose()}}
function Pin([string]$Path,[string]$Hash){Assert-Plain $Path;$f=[IO.File]::Open($Path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read);try{if((Hash-Stream $f) -cne $Hash){throw 'PIN_MISMATCH'};return $f}catch{$f.Dispose();throw}}
Add-Type -TypeDefinition @"
using System;using System.Runtime.InteropServices;
public static class PaccFirstCycleContext {
 [DllImport("kernel32.dll")] public static extern IntPtr GetStdHandle(int n);
 [DllImport("kernel32.dll")] public static extern bool GetConsoleMode(IntPtr h,out uint mode);
 [DllImport("advapi32.dll",SetLastError=true)] public static extern bool GetTokenInformation(IntPtr h,int cls,out int value,int size,out int n);
}
"@
function Context {
 $id=[Security.Principal.WindowsIdentity]::GetCurrent()
 try{$e=0;$t=0;$n=0;$im=0;$om=0;$a=[PaccFirstCycleContext]::GetTokenInformation($id.Token,20,[ref]$e,4,[ref]$n);$b=[PaccFirstCycleContext]::GetTokenInformation($id.Token,18,[ref]$t,4,[ref]$n);$admin=([Security.Principal.WindowsPrincipal]::new($id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator);$sid=$id.User.Value}finally{$id.Dispose()}
 $th=[Security.Principal.WindowsIdentity]::GetCurrent($true);$imp=$null -ne $th;if($th){$th.Dispose()}
 $console=[PaccFirstCycleContext]::GetConsoleMode([PaccFirstCycleContext]::GetStdHandle(-10),[ref]$im) -and [PaccFirstCycleContext]::GetConsoleMode([PaccFirstCycleContext]::GetStdHandle(-11),[ref]$om)
 return [pscustomobject]@{sid=$sid;elevated=($a -and $b -and $e -eq 1 -and $t -ne 3);administrators=$admin;console=$console;impersonating=$imp}
}
$pins=@();$pause=$false;$code=2;$log=$null;$logFailed=$false
try {
 if($PSCommandPath -cne $Script){throw 'SCRIPT_PATH'};Assert-Plain $Script;$self=Path-Hash $Script
 if($Mode -eq 'Elevated' -and ($ExpectedScriptSha256 -eq '' -or $self -cne $ExpectedScriptSha256)){throw 'SCRIPT_PIN'}
 $pins+=Pin $Script $self;$pins+=Pin $Exe $ExeHash;$pins+=Pin $Candidate $CandidateHash;$pins+=Pin $Prior '02d8e02fc20b03018cc7abe1256e1e490a4edd6c43f3fddb605d8908a6f0b2ff'
 $priorResult=Get-Content -LiteralPath $Prior -Raw|ConvertFrom-Json
 if($priorResult.exit -ne 0 -or $priorResult.descriptor_retained -or -not $priorResult.parent_closed -or @($priorResult.files|Where-Object{-not $_.closed}).Count){throw 'PRIOR_DLL_RESULT_DENY'}
 $ctx=Context
 if($Mode -eq 'Check'){
  $saved=$ErrorActionPreference;try{$ErrorActionPreference='Continue';& $Exe --check;$nativeExit=$LASTEXITCODE}finally{$ErrorActionPreference=$saved}
  if($nativeExit -ne 0){throw 'NATIVE_PREFLIGHT_DENY'}
  Write-Output 'CHECK_PASS; NO_IMAGE_PLACEMENT_NO_REBIND_NO_START; PREPARATION_ONLY';$code=0
 }elseif($Mode -eq 'Launch'){
  if(-not $ctx.console){throw 'CONSOLE_REQUIRED'}
  if(Test-Path -LiteralPath ($ReceiptRoot+'\manual-native-intent.txt')){throw 'PRIOR_ATTEMPT_NO_RETRY'}
  $a='-NoProfile -ExecutionPolicy Bypass -File "'+$Script+'" -Mode Elevated -ExpectedScriptSha256 '+$self
  $child=Start-Process -FilePath $PSExe -Verb RunAs -ArgumentList $a -WorkingDirectory $Root -WindowStyle Normal -Wait -PassThru
  $code=$child.ExitCode;Write-Output ('SCM_FIRST_CYCLE_EXIT='+$code)
 }else{
  $pause=$ctx.console
  if(-not $ctx.elevated -or -not $ctx.administrators -or -not $ctx.console -or $ctx.impersonating -or $ctx.sid -cne $ExpectedSid){throw 'OPERATOR_OR_CONSOLE_DENY'}
  if(Test-Path -LiteralPath ($ReceiptRoot+'\manual-native-intent.txt')){throw 'PRIOR_ATTEMPT_NO_RETRY'}
  Assert-Plain $ReceiptRoot
  $attempt=[Guid]::NewGuid().ToString('N');$lines=[Collections.Generic.List[string]]::new();$logPath=$ReceiptRoot+'\manual-'+$attempt+'.log'
  $log=[IO.File]::Open($logPath,[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::Read)
  Write-Output ('LAB_ONLY; OPERATOR='+$ctx.sid+'; QUALIFIER_SHA256='+$ExeHash+'; CANDIDATE_SHA256='+$CandidateHash)
  Write-Output 'Native full plan/confirmation follows. Unknown live status or close failure retains qualifier process and pinned refs. Do not close/retry/kill automatically.'
  $saved=$ErrorActionPreference
  try {
   $ErrorActionPreference='Continue'
   & $Exe --cycle-lab $attempt 2>&1|ForEach-Object{
    $line=$_.ToString();$lines.Add($line);Write-Host $line
    try{$b=[Text.Encoding]::UTF8.GetBytes($line+[Environment]::NewLine);$log.Write($b,0,$b.Length);$log.Flush($true)}catch{$logFailed=$true;Write-Host 'LOG_WRITE_UNKNOWN; continuing approved safety Stop, preserve first failure'}
   }
   $nativeExit=$LASTEXITCODE
  }finally{$ErrorActionPreference=$saved;$log.Dispose();$log=$null}
  $pass=-not $logFailed -and $nativeExit -eq 0 -and $lines.Contains('LAB_CYCLE=RUNNING_TOKEN_ACCESS_STOP_QUIESCENCE_PASS; STOPPED_EXIT0_PROCESS_SIGNALED; NO_CA_NO_CURRENT_NO_AUTHORITY; KSP_ACCESS_NOT_TESTED') -and $lines.Contains('LAB_CYCLE_FINAL STAGE=STOP_ONCE ERROR=0 CLOSE=0 CLOSED=1 EFFECT=2 START_REQUESTED=1 STOP_REQUESTED=1; NO_RETRY_NO_DELETE_NO_ROLLBACK; ED11_PARTIAL')
  $code=2;if($pass){$code=0}
  $receipt=@{attempt=$attempt;operator=$ctx;script_sha256=$self;qualifier_sha256=$ExeHash;candidate_sha256=$CandidateHash;native_exit=$nativeExit;log_failed=$logFailed;cycle_pass=$pass;service='PapinhoAcceleratorLab20261023';account='NT SERVICE\PapinhoAcceleratorLab20261023';output=@($lines.ToArray());log=$logPath;retry='DENY';rollback='NOT_CLAIMED';deletion='NOT_RUN';ca='DENY';current='DENY';ed11='PARTIAL'}|ConvertTo-Json -Depth 8
  $receiptPath=$ReceiptRoot+'\manual-'+$attempt+'.json';$o=[IO.File]::Open($receiptPath,[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::None)
  try{$b=[Text.Encoding]::UTF8.GetBytes($receipt);$o.Write($b,0,$b.Length);$o.Flush($true)}finally{$o.Dispose()}
  Write-Output ('RECEIPT='+$receiptPath+'; CYCLE_PASS='+$pass+'; NATIVE_EXIT='+$nativeExit)
 }
}catch{Write-Output ('STOP: '+$_.Exception.Message);$code=2}
finally{if($log){$log.Dispose()};foreach($pin in $pins){try{$pin.Dispose()}catch{Write-Output 'PIN_CLOSE_UNCERTAIN';$code=2}}}
if($pause){Write-Output 'Resultado preservado. F10 fecha. Nunca repetir automaticamente.';try{while([Console]::ReadKey($true).Key -ne [ConsoleKey]::F10){}}catch{$code=2}}
exit $code
