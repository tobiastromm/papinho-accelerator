#requires -Version 5.1
[CmdletBinding()]
param([ValidateSet('Check','Test','Launch','Elevated')][string]$Mode='Check',
 [ValidatePattern('^$|^[0-9a-f]{64}$')][string]$ExpectedScriptSha256='')
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$Root='C:\Projetos\PapinhoAccelerator'
$ScriptPath=$Root+'\tools\qualification\ca-custody-elevated-lab-uac.ps1'
$LabExe=$Root+'\build\ca-lab-finalization-launcher-20261022\Release\next-lab.exe'
$LabHash='91613b6b94eea8052f81920a2fe518891c769e5ddcdcb2ef11bd903285947117'
$ExpectedSid='S-1-5-21-785699550-1440458426-1682014700-1108'
$KeyName='PapinhoAccelerator.CA.2345e07adf714045a20d1c8b6fdf5aaf.a2bcd6d2ed3043d083e7e50342d6fab0'
$PowerShellExe='C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe'
$ReceiptRoot=$Root+'\docs\integration\ca-custody-elevated-lab-finalization-20261022\manual-results'
# Same fixed paths preserve already-consumed observations as STOP, never reset.
$AttemptPath=$ReceiptRoot+'\new-identity-observation.started.json'
$ResultPath=$ReceiptRoot+'\new-identity-observation.result.json'
function Write-NewReceipt([string]$Path,$Value) {
 $f=[IO.File]::Open($Path,[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::None)
 try {$w=[IO.StreamWriter]::new($f,[Text.UTF8Encoding]::new($false));try {$w.Write(($Value | ConvertTo-Json -Depth 8));$w.Flush()} finally {$w.Dispose()}} finally {$f.Dispose()}
}
function Assert-Plain([string]$Path) {
 if(-not [IO.Path]::IsPathRooted($Path) -or [IO.Path]::GetFullPath($Path) -cne $Path){throw 'NONCANONICAL_PATH'}
 $p=$Path
 while($p){if(([IO.File]::GetAttributes($p) -band [IO.FileAttributes]::ReparsePoint) -ne 0){throw 'REPARSE_PATH'};$p=[IO.Path]::GetDirectoryName($p)}
}
function Stream-Hash([IO.Stream]$Stream) {
 $Stream.Position=0;$sha=[Security.Cryptography.SHA256]::Create()
 try {return ([BitConverter]::ToString($sha.ComputeHash($Stream))).Replace('-','').ToLowerInvariant()}
 finally {$sha.Dispose();$Stream.Position=0}
}
function Open-Pinned([string]$Path,[string]$Hash) {
 Assert-Plain $Path
 $s=[IO.File]::Open($Path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 try {if((Stream-Hash $s) -cne $Hash){throw 'PIN_MISMATCH'};return $s} catch {$s.Dispose();throw}
}
Add-Type -TypeDefinition @"
using System;
using System.Runtime.InteropServices;
public static class PaccCaLabGateNative {
 [DllImport("kernel32.dll")] public static extern IntPtr GetStdHandle(int n);
 [DllImport("kernel32.dll",SetLastError=true)] public static extern bool GetConsoleMode(IntPtr h,out uint mode);
 [DllImport("advapi32.dll",SetLastError=true)] public static extern bool GetTokenInformation(IntPtr h,int cls,out int value,int size,out int returned);
}
"@
function Observe-Operator {
 $id=[Security.Principal.WindowsIdentity]::GetCurrent()
 try {
  $thread=[Security.Principal.WindowsIdentity]::GetCurrent($true);$impersonating=$null -ne $thread
  if($impersonating){$thread.Dispose()}
  $e=0;$t=0;$n=0;$im=0;$om=0
  $eq=[PaccCaLabGateNative]::GetTokenInformation($id.Token,20,[ref]$e,4,[ref]$n)
  $tq=[PaccCaLabGateNative]::GetTokenInformation($id.Token,18,[ref]$t,4,[ref]$n)
  $admin=([Security.Principal.WindowsPrincipal]::new($id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
  $console=[PaccCaLabGateNative]::GetConsoleMode([PaccCaLabGateNative]::GetStdHandle(-10),[ref]$im) -and [PaccCaLabGateNative]::GetConsoleMode([PaccCaLabGateNative]::GetStdHandle(-11),[ref]$om)
  return [pscustomobject]@{sid=$id.User.Value;elevated=($eq -and $e -eq 1);elevation_type=$t;query_ok=($eq -and $tq);administrators_enabled=$admin;impersonating=$impersonating;console=$console}
 } finally {$id.Dispose()}
}

function Test-Eligible($o) {
 return ($o.query_ok -and $o.elevated -and $o.elevation_type -ne 3 -and $o.administrators_enabled -and -not $o.impersonating -and $o.console -and $o.sid -ceq $ExpectedSid)
}
function Test-Unconsumed([bool]$Started,[bool]$Finished) {return (-not $Started -and -not $Finished)}
function Test-Confirmation([string]$Actual,[string]$Required) {return ($Actual -cne '' -and $Actual -ceq $Required)}
function Read-ExactConfirmation {
 $value='';Write-Host '> ' -NoNewline
 while($true){
  $k=[Console]::ReadKey($true)
  if($k.Key -eq [ConsoleKey]::Escape){throw 'CANCELLED_BEFORE_CONSUMPTION'}
  if($k.Key -eq [ConsoleKey]::Enter){[Console]::WriteLine();return $value}
  if($k.Key -eq [ConsoleKey]::Backspace){if($value.Length){$value=$value.Substring(0,$value.Length-1);[Console]::Write("`b `b")};continue}
  $n=[int]$k.KeyChar
  if($n -ge 32 -and $n -le 126){if($value.Length -ge 200){throw 'CONFIRMATION_TOO_LONG'};$value+=$k.KeyChar;[Console]::Write($k.KeyChar)}
 }
}
function Test-NativePass([string[]]$Lines,[int]$Code,[bool]$Overflow) {
 if($Code -ne 0 -or $Overflow){return $false}
 $expected=@('CREATE','LENGTH','EXPORT_POLICY','USAGE','DACL','FINALIZE')
 $phases=@($Lines | Where-Object {$_ -cmatch '^LAB_NATIVE '})
 if($phases.Count -ne $expected.Count){return $false}
 for($i=0;$i -lt $expected.Count;$i++){if($phases[$i] -cne ('LAB_NATIVE phase='+$expected[$i]+' status=00000000')){return $false}}
 return (@($Lines | Where-Object {$_ -cmatch '^LAB_RESULT phase=FINAL_CLOSE status=00000000 state=3 created=1 finalized=2 key_owned=0 provider_owned=0 close_confirmed=1 '}).Count -eq 1 -and
  @($Lines | Where-Object {$_ -ceq 'LAB_PERSISTED_KEY=PRESENT_FINALIZED_LAB_ONLY; HANDLES_CLOSED; NO_DELETION'}).Count -eq 1 -and
  @($Lines | Where-Object {$_ -ceq 'LAB_CUSTODY_PASS_NO_CERT_NO_SIGNATURE_NO_CURRENT_NO_AUTHORITY'}).Count -eq 1)
}
function Test-Gates {
 $o=[pscustomobject]@{query_ok=$true;elevated=$true;elevation_type=2;administrators_enabled=$true;impersonating=$false;console=$true;sid=$ExpectedSid}
 if(-not (Test-Eligible $o)){throw 'MODEL_BASELINE_FAILED'}
 foreach($field in @('query_ok','elevated','administrators_enabled','console')){$o.$field=$false;if(Test-Eligible $o){throw ('MODEL_GATE_FAILED_'+$field)};$o.$field=$true}
 $o.sid='DIVERGENT';if(Test-Eligible $o){throw 'MODEL_IDENTITY_FAILED'};$o.sid=$ExpectedSid
 $o.impersonating=$true;if(Test-Eligible $o){throw 'MODEL_IMPERSONATION_FAILED'}
 if((Test-Unconsumed $true $false) -or (Test-Unconsumed $false $true)){throw 'MODEL_REPLAY_FAILED'}
 if((Test-Confirmation '' 'EXPECTED') -or (Test-Confirmation 'OTHER' 'EXPECTED')){throw 'MODEL_CONFIRMATION_FAILED'}
 if(Test-NativePass @('LAB_NATIVE phase=CREATE status=8009000f') 3 $false){throw 'MODEL_COLLISION_FAILED'}
 if(Test-NativePass @('LAB_NATIVE phase=FINALIZE status=80090010') 3 $false){throw 'MODEL_UNCERTAIN_FAILED'}
 if(Test-NativePass @() 0 $false){throw 'MODEL_EMPTY_SUCCESS_FAILED'}
 # Synthetic parser-only success baseline; never a key/authority fixture.
 $ok=@('CREATE','LENGTH','EXPORT_POLICY','USAGE','DACL','FINALIZE') | ForEach-Object {'LAB_NATIVE phase='+$_+' status=00000000'}
 $ok+=@('LAB_RESULT phase=FINAL_CLOSE status=00000000 state=3 created=1 finalized=2 key_owned=0 provider_owned=0 close_confirmed=1 first_function=4',
 'LAB_PERSISTED_KEY=PRESENT_FINALIZED_LAB_ONLY; HANDLES_CLOSED; NO_DELETION',
 'LAB_CUSTODY_PASS_NO_CERT_NO_SIGNATURE_NO_CURRENT_NO_AUTHORITY')
 if(-not (Test-NativePass $ok 0 $false)){throw 'MODEL_PARSER_BASELINE_FAILED'}
 if((Test-NativePass $ok 3 $false) -or (Test-NativePass $ok 0 $true)){throw 'MODEL_EXIT_OR_OVERFLOW_FAILED'}
 foreach($bad in @('8009000f','80090010','80090036')){
  $corrupt=@($ok);$corrupt[0]='LAB_NATIVE phase=CREATE status='+$bad
  if(Test-NativePass $corrupt 0 $false){throw 'MODEL_NATIVE_ERROR_AS_SUCCESS_FAILED'}
 }
 $uncertain=@($ok);$uncertain[6]=$uncertain[6].Replace('close_confirmed=1','close_confirmed=0')
 if(Test-NativePass $uncertain 0 $false){throw 'MODEL_CLEANUP_UNKNOWN_FAILED'}
 if(Test-NativePass (@($ok)+@($ok[0])) 0 $false){throw 'MODEL_DUPLICATE_PHASE_FAILED'}
 Write-Output 'LAUNCHER_NEGATIVE_MODEL_PASS; NATIVE_KEY_OPERATIONS=NOT_RUN; UAC=NOT_RUN'
}
if($Mode -eq 'Test'){Test-Gates;exit 0}
$scriptStream=$null;$labStream=$null;$pause=$false;$result=2
try {
 if($PSCommandPath -cne $ScriptPath){throw 'SCRIPT_PATH_MISMATCH'}
 Assert-Plain $ScriptPath
 $scriptStream=[IO.File]::Open($ScriptPath,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 $selfHash=Stream-Hash $scriptStream
 if($Mode -eq 'Elevated' -and ($ExpectedScriptSha256 -eq '' -or $selfHash -cne $ExpectedScriptSha256)){throw 'SCRIPT_PIN_MISMATCH'}
 $labStream=Open-Pinned $LabExe $LabHash
 $operator=Observe-Operator
 if($Mode -eq 'Check') {
  $operator | ConvertTo-Json
  Write-Output ('CHECK_ONLY; NEW_LAB_NAME='+$KeyName+'; SHA256='+$LabHash+'; NONOVERWRITE=REQUIRED; NO_AUTOMATIC_DELETE; KEY_CREATION=NOT_RUN; CURRENT=DENY')
  $result=0
 } elseif($Mode -eq 'Launch') {
  if(-not $operator.console){throw 'NO_INTERACTIVE_CONSOLE'}
  $arguments='-NoProfile -ExecutionPolicy Bypass -File "'+$ScriptPath+'" -Mode Elevated -ExpectedScriptSha256 '+$selfHash
  try {$child=Start-Process -FilePath $PowerShellExe -Verb RunAs -ArgumentList $arguments -WorkingDirectory $Root -WindowStyle Normal -Wait -PassThru}
  catch {throw 'UAC_CANCELLED_OR_LAUNCH_FAILED_DENY'}
  Write-Output ('UAC_CA_LAB_EXIT='+$child.ExitCode);$result=$child.ExitCode
 } else {
  $pause=$operator.console
  if(-not (Test-Eligible $operator)){throw 'ELEVATED_OPERATOR_OR_CONSOLE_DENY'}
  Assert-Plain $ReceiptRoot
  if(-not (Test-Unconsumed ([IO.File]::Exists($AttemptPath)) ([IO.File]::Exists($ResultPath)))){throw 'ATTEMPT_ALREADY_CONSUMED_NO_RETRY'}
  $random=New-Object byte[] 32;$rng=[Security.Cryptography.RandomNumberGenerator]::Create()
  try {$rng.GetBytes($random)} finally {$rng.Dispose()}
  $attempt=([BitConverter]::ToString($random)).Replace('-','').ToLowerInvariant()
  $required='LAB_CA_CREATE_FINALIZE '+$attempt
  Write-Output ('LAB ONLY; effective operator='+$operator.sid)
  Write-Output ('Microsoft Software Key Storage Provider; MACHINE; exact key='+$KeyName)
  Write-Output ('BINARY_SHA256='+$LabHash+'; ATTEMPT='+$attempt)
  Write-Output 'RSA2048; export policy0; signing-only; DACL operator/SYSTEM, LocalService LAB exclusion.'
  Write-Output 'ONE exclusive no-overwrite creation, required properties, Finalize and public validation.'
  Write-Output 'Success leaves a persisted LAB key and closes handles. NO automatic deletion.'
  Write-Output 'Any collision/error/uncertainty STOP; no retry. Previous LAB remains UNKNOWN.'
  Write-Output 'No certificate, signing, bootstrap, root, CURRENT or production authority. ED11 PARTIAL.'
  Write-Output 'Type the ENTIRE exact line; visible input; Esc cancels before consumption:'
  Write-Output $required
  $answer=Read-ExactConfirmation
  if(-not (Test-Confirmation $answer $required)){throw 'CONFIRMATION_MISMATCH_NO_SUBMISSION'}
  $again=Observe-Operator
  if(-not (Test-Eligible $again) -or $again.sid -cne $operator.sid){throw 'OPERATOR_CHANGED_NO_SUBMISSION'}
  if((Stream-Hash $labStream) -cne $LabHash -or (Stream-Hash $scriptStream) -cne $selfHash){throw 'PIN_CHANGED_NO_SUBMISSION'}
  Write-NewReceipt $AttemptPath ([ordered]@{purpose='ONE_SHOT_LAB_CREATE_FINALIZE_NO_DELETE';attempt=$attempt;operator=$again;new_name=$KeyName;binary_sha256=$LabHash;script_sha256=$selfHash;confirmation='EXACT_PLAN_CONFIRMED';consumed=$true;previous_disposition='UNKNOWN';disposition='UNKNOWN_UNTIL_RESULT';current='DENY';ed11='PARTIAL'})
  $capture=[Collections.Generic.List[string]]::new();$script:overflow=$false
  & $LabExe '--execute-new-lab' 2>&1 | ForEach-Object {
   $line=$_.ToString();Write-Output $line
   if($capture.Count -lt 128 -and $line.Length -le 1024){$capture.Add($line)}else{$script:overflow=$true}
  }
  $code=$LASTEXITCODE
  $passed=Test-NativePass ($capture.ToArray()) $code $script:overflow
  $disposition='UNKNOWN';if($passed){$disposition='PRESENT_FINALIZED_LAB_ONLY_HANDLES_CLOSED'}
  Write-NewReceipt $ResultPath ([ordered]@{purpose='LAB_CREATE_FINALIZE_NO_DELETE_RESULT';attempt=$attempt;operator=$again;new_name=$KeyName;binary_sha256=$LabHash;script_sha256=$selfHash;native_exit=$code;phase_output=$capture.ToArray();capture_overflow=$script:overflow;qualified_lab_flow=$passed;disposition=$disposition;key_deletion='NOT_RUN';previous_disposition='UNKNOWN';current='DENY';ed11='PARTIAL'})
  Write-Output ('RECEIPT='+$ResultPath+'; LAB_PASS='+$passed+'; DISPOSITION='+$disposition)
  if($passed){$result=0}else{$result=3}
 }
} catch {Write-Output ('DENY: '+$_.Exception.Message);$result=2}
finally {if($labStream){$labStream.Dispose()};if($scriptStream){$scriptStream.Dispose()}}
if($pause){
 Write-Output 'Resultado preservado. Pressione F10 para fechar. Nunca repetir automaticamente.'
 try {while([Console]::ReadKey($true).Key -ne [ConsoleKey]::F10){}} catch {Write-Output 'RESULT_WINDOW_INPUT_UNAVAILABLE';$result=2}
}
exit $result
