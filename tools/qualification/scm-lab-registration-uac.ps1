#requires -Version 5.1
[CmdletBinding()]
param([ValidateSet('Check','Launch','Elevated')][string]$Mode='Check',[ValidatePattern('^$|^[0-9a-f]{64}$')][string]$ExpectedScriptSha256='')
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$Root='C:\Projetos\PapinhoAccelerator'
$ScriptPath=$Root+'\tools\qualification\scm-lab-registration-uac.ps1'
$PowerShellExe='C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe'
$ExpectedSid='S-1-5-21-785699550-1440458426-1682014700-1108'
$ReceiptRoot=$Root+'\docs\integration\windows-virtual-service-native-lab-registration-20261023'
$Exe=$Root+'\build\windows-virtual-service-native-lab-registration-20261023\Selected\Release\papacc_scm_registration_lab.exe'
$ExeHash='cb1e3eb35901652e51400e797f7f137b168660e9ddb9253176e92726756b83d7'
$DataReceipt=$Root+'\docs\integration\scm-protected-binding-positive-20261023\manual-1b80dd8837dd49a2bc507faeef989526.json'
function Assert-Plain([string]$Path){
 if([IO.Path]::GetFullPath($Path) -cne $Path){throw 'NONCANONICAL_PATH'}
 $p=$Path;while($p){if(([IO.File]::GetAttributes($p) -band [IO.FileAttributes]::ReparsePoint) -ne 0){throw 'REPARSE_PATH'};$p=[IO.Path]::GetDirectoryName($p)}
}
function Open-Pinned([string]$Path,[string]$Hash){
 Assert-Plain $Path
 $f=[IO.File]::Open($Path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 try {$sha=[Security.Cryptography.SHA256]::Create();try{$v=([BitConverter]::ToString($sha.ComputeHash($f))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()};if($v -cne $Hash){throw 'HASH_PIN_MISMATCH'};return $f}catch{$f.Dispose();throw}
}
Add-Type -TypeDefinition @"
using System;using System.Runtime.InteropServices;
public static class PaccScmRegistrationContext {
 [DllImport("kernel32.dll")] public static extern IntPtr GetStdHandle(int n);
 [DllImport("kernel32.dll")] public static extern bool GetConsoleMode(IntPtr h,out uint mode);
 [DllImport("advapi32.dll",SetLastError=true)] public static extern bool GetTokenInformation(IntPtr h,int cls,out int value,int size,out int returned);
}
"@
function Observe-Context {
 $id=[Security.Principal.WindowsIdentity]::GetCurrent()
 try {
  $thread=[Security.Principal.WindowsIdentity]::GetCurrent($true);$impersonating=$null -ne $thread;if($thread){$thread.Dispose()}
  $e=0;$t=0;$n=0;$im=0;$om=0
  $eq=[PaccScmRegistrationContext]::GetTokenInformation($id.Token,20,[ref]$e,4,[ref]$n)
  $tq=[PaccScmRegistrationContext]::GetTokenInformation($id.Token,18,[ref]$t,4,[ref]$n)
  $console=[PaccScmRegistrationContext]::GetConsoleMode([PaccScmRegistrationContext]::GetStdHandle(-10),[ref]$im) -and [PaccScmRegistrationContext]::GetConsoleMode([PaccScmRegistrationContext]::GetStdHandle(-11),[ref]$om)
  $admin=([Security.Principal.WindowsPrincipal]::new($id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
  return [pscustomobject]@{sid=$id.User.Value;query_ok=($eq -and $tq);elevated=($e -eq 1);elevation_type=$t;administrators_enabled=$admin;console=$console;impersonating=$impersonating}
 }finally{$id.Dispose()}
}

$pins=@();$pause=$false;$result=2
try{
 if($PSCommandPath -cne $ScriptPath){throw 'SCRIPT_PATH'};Assert-Plain $ScriptPath
 $selfHash=(Get-FileHash -LiteralPath $ScriptPath -Algorithm SHA256).Hash.ToLowerInvariant()
 if($Mode -eq 'Elevated' -and ($ExpectedScriptSha256 -eq '' -or $selfHash -cne $ExpectedScriptSha256)){throw 'SCRIPT_PIN'}
 $pins+=Open-Pinned $ScriptPath $selfHash
 $pins+=Open-Pinned $Exe $ExeHash
 $pins+=Open-Pinned ($Root+'\build\scm-protected-installation-binding-20261022\Selected\Release\papacc_server.exe') '3737e83be95613a2616b9fb764fe95d72796912746e9440e37b3ed2a77e01abb'
 $pins+=Open-Pinned ($Root+'\build\scm-protected-installation-binding-20261022\Selected\Release\papacc.conf') 'a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44'
 $pins+=Open-Pinned $DataReceipt 'c7ea3e90e82389d47c8fb1f746c104db484a4cc3ec6f4c3b4364e6a037d8cbf2'
 $data=Get-Content -LiteralPath $DataReceipt -Raw|ConvertFrom-Json
 if(-not $data.read_only_data_pass -or -not $data.native_bound_closed -or $data.native_exit -ne 0 -or $data.operator.sid -cne $ExpectedSid){throw 'PRIOR_DATA_EVIDENCE_DENY'}
 $operator=Observe-Context
 if($Mode -eq 'Check'){
  $operator|ConvertTo-Json
  Write-Output 'CHECK_ONLY; LAB=PapinhoAcceleratorLab20261023; ACCOUNT=NT SERVICE\PapinhoAcceleratorLab20261023; STARTUP=DISABLED; NO_NATIVE_EFFECT'
  if($operator.elevated -and $operator.administrators_enabled -and $operator.console -and -not $operator.impersonating -and $operator.sid -ceq $ExpectedSid){$result=0}
 }elseif($Mode -eq 'Launch'){
  if(-not $operator.console){throw 'INTERACTIVE_CONSOLE_REQUIRED'}
  if(Test-Path -LiteralPath ($ReceiptRoot+'\manual-native-intent.txt')){throw 'PREVIOUS_ATTEMPT_NO_RETRY'}
  $a='-NoProfile -ExecutionPolicy Bypass -File "'+$ScriptPath+'" -Mode Elevated -ExpectedScriptSha256 '+$selfHash
  $child=Start-Process -FilePath $PowerShellExe -Verb RunAs -ArgumentList $a -WorkingDirectory $Root -WindowStyle Normal -Wait -PassThru
  $result=$child.ExitCode;Write-Output ('SCM_LAB_REGISTRATION_EXIT='+$result)
 }else{
  $pause=$operator.console
  if(-not $operator.query_ok -or -not $operator.elevated -or $operator.elevation_type -eq 3 -or -not $operator.administrators_enabled -or -not $operator.console -or $operator.impersonating -or $operator.sid -cne $ExpectedSid){throw 'ELIGIBLE_OPERATOR_CONSOLE_DENY'}
  if(Test-Path -LiteralPath ($ReceiptRoot+'\manual-native-intent.txt')){throw 'PREVIOUS_NATIVE_ATTEMPT_NO_RETRY'}
  Write-Output 'LAB registration ONLY, DISABLED; no Start, runtime, listener, CA or CURRENT. Native plan/confirmation follows.'
  Write-Output ('QUALIFICATION_SHA256='+$ExeHash+'; OPERATOR='+$operator.sid)
  $attempt=[Guid]::NewGuid().ToString('N');$saved=$ErrorActionPreference
  try{$ErrorActionPreference='Continue';$lines=@(& $Exe --register-lab $attempt 2>&1|ForEach-Object{$_.ToString()});$nativeExit=$LASTEXITCODE}finally{$ErrorActionPreference=$saved}
  $lines|ForEach-Object{Write-Output $_}
  $good=@($lines|Where-Object{$_ -match '^LAB_REGISTRATION=CREATED_CONFIGURED_READBACK_PASS; NAME=PapinhoAcceleratorLab20261023; ACCOUNT=NT SERVICE\\PapinhoAcceleratorLab20261023; SID=S-1-5-80(-[0-9]+){5}; SID_BYTES=32; STARTUP=DISABLED; STATE=STOPPED; PID=0; SERVICE_TOKEN=NOT_QUALIFIED; START=NOT_CALLED$'}).Count -eq 1
  $closed=@($lines|Where-Object{$_ -ceq 'LAB_FINAL; STAGE=FILES_REVALIDATED; ERROR=0; CLOSE=0; RETAINED=0; AUX_RETAINED=0; EFFECT_REQUESTED=1; DISPOSITION=4; READBACK=1; NO_START_NO_DELETE_NO_RETRY; CA_CURRENT_AUTHORITY=DENY'}).Count -eq 1
  $pass=$nativeExit -eq 0 -and $good -and $closed
  $result=2;if($pass){$result=0}
  Assert-Plain $ReceiptRoot
  $receipt=@{attempt=$attempt;operator=$operator;qualification_sha256=$ExeHash;script_sha256=$selfHash;service_name='PapinhoAcceleratorLab20261023';virtual_account='NT SERVICE\PapinhoAcceleratorLab20261023';native_exit=$nativeExit;output=$lines;registration_pass=$pass;runtime='NOT_STARTED';token='NOT_QUALIFIED';retry='DENY';deletion='NOT_RUN';acl_mutations='NONE';current='DENY';ed11='PARTIAL'}|ConvertTo-Json -Depth 6
  $path=$ReceiptRoot+'\manual-'+$attempt+'.json';$f=[IO.File]::Open($path,[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::None)
  try{$b=[Text.Encoding]::UTF8.GetBytes($receipt);$f.Write($b,0,$b.Length);$f.Flush($true)}finally{$f.Dispose()}
  Write-Output ('RECEIPT='+$path+'; REGISTRATION_PASS='+$pass+'; NATIVE_EXIT='+$nativeExit)
  Write-Output 'Never retry/delete automatically. Registration/config readback is NOT a running token or admission. CA/CURRENT/signature remain DENY.'
 }
}catch{Write-Output ('STOP: '+$_.Exception.Message);$result=2}
finally{foreach($pin in $pins){$pin.Dispose()}}
if($pause){Write-Output 'Resultado preservado. F10 fecha. Nunca repetir automaticamente.';try{while([Console]::ReadKey($true).Key -ne [ConsoleKey]::F10){}}catch{$result=2}}
exit $result
