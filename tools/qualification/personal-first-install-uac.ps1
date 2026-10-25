#requires -Version 5.1
[CmdletBinding()]
param(
 [ValidateSet('Check','Launch','Elevated')][string]$Mode='Check',
 [ValidatePattern('^$|^S-1-[0-9-]+$')][string]$OriginSid='',
 [ValidatePattern('^$|^[0-9a-f]{32}$')][string]$Attempt='',
 [ValidatePattern('^$|^[0-9a-f]{64}$')][string]$ExpectedScriptSha256=''
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
# Qualification-only constants. No external executable/config/command arguments.
$Root='C:\Projetos\PapinhoAccelerator'
$ScriptPath=$Root+'\tools\qualification\personal-first-install-uac.ps1'
$Exe=$Root+'\build\local-bootstrap-config-cli-20261019\Release\papacc_server.exe'
$Parent=[IO.Path]::GetDirectoryName($Exe)
$Source=$Root+'\tools\qualification\personal-first-install-readonly.conf'
$Leaf='personal-readonly-target-not-created'
$Child=$Parent+'\'+$Leaf
$ReceiptRoot=$Root+'\docs\integration\personal-first-install-uac-launcher-20261022\manual-results'
$PowerShellExe='C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe'
$ExeHash='bb618deffcd975060b2f860833f14cbbc50d61e49b595ad1f57181556961925a'
$Pins=[ordered]@{}
$Pins[$Exe]=$ExeHash
$Pins[$Parent+'\libcrypto-3-x64.dll']='09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'
$Pins[$Parent+'\libssl-3-x64.dll']='3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'
$Pins[$Source]='a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44'
function Assert-PlainPath([string]$Path) {
 if(-not [IO.Path]::IsPathRooted($Path) -or [IO.Path]::GetFullPath($Path) -cne $Path){throw 'NONCANONICAL_PATH'}
 $p=$Path
 while($p){
  if(([IO.File]::GetAttributes($p) -band [IO.FileAttributes]::ReparsePoint) -ne 0){throw 'REPARSE_PATH'}
  $p=[IO.Path]::GetDirectoryName($p)
 }
}
function Open-Pinned([string]$Path,[string]$Hash) {
 Assert-PlainPath $Path
 if($Hash -cnotmatch '^[0-9a-f]{64}$'){throw 'INVALID_PIN'}
 # Held across hash/use/UAC: deny write/delete sharing; never modify a binary.
 $s=[IO.File]::Open($Path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 try {
  $sha=[Security.Cryptography.SHA256]::Create()
  try {$actual=([BitConverter]::ToString($sha.ComputeHash($s))).Replace('-','').ToLowerInvariant()} finally {$sha.Dispose()}
  if($actual -cne $Hash){throw 'PIN_MISMATCH'}
  $s.Position=0
  return $s
 } catch {$s.Dispose();throw}
}
function Get-Hash([IO.Stream]$Stream) {
 $Stream.Position=0;$sha=[Security.Cryptography.SHA256]::Create()
 try {return ([BitConverter]::ToString($sha.ComputeHash($Stream))).Replace('-','').ToLowerInvariant()} finally {$sha.Dispose();$Stream.Position=0}
}
function Assert-ConsoleExecutable([IO.Stream]$Stream) {
 $b=[IO.BinaryReader]::new($Stream,[Text.Encoding]::UTF8,$true)
 try {
  $Stream.Position=0;if($b.ReadUInt16() -ne 0x5a4d){throw 'INVALID_PE'}
  $Stream.Position=0x3c;$offset=$b.ReadUInt32()
  if($offset -gt $Stream.Length-96){throw 'INVALID_PE'}
  $Stream.Position=$offset;if($b.ReadUInt32() -ne 0x4550 -or $b.ReadUInt16() -ne 0x8664){throw 'WRONG_EXECUTABLE_TARGET'}
  $Stream.Position=$offset+24;if($b.ReadUInt16() -ne 0x20b){throw 'WRONG_EXECUTABLE_TARGET'}
  $Stream.Position=$offset+24+68;if($b.ReadUInt16() -ne 3){throw 'NOT_CONSOLE_EXECUTABLE'}
 } finally {$b.Dispose();$Stream.Position=0}
}
function Initialize-NativeProbe {
 Add-Type -TypeDefinition @"
using System;
using System.Runtime.InteropServices;
public static class PaccUacQualificationNative {
 [DllImport("kernel32.dll")] public static extern IntPtr GetStdHandle(int n);
 [DllImport("kernel32.dll",SetLastError=true)] public static extern bool FlushConsoleInputBuffer(IntPtr handle);
 [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)] public static extern uint GetFileAttributesW(string path);
 [DllImport("kernel32.dll",SetLastError=true)] public static extern bool GetConsoleMode(IntPtr h,out uint mode);
 [DllImport("kernel32.dll")] public static extern uint WTSGetActiveConsoleSessionId();
 [DllImport("advapi32.dll",SetLastError=true)] public static extern bool GetTokenInformation(IntPtr h,int cls,out int value,int size,out int returned);
 [DllImport("advapi32.dll",SetLastError=true)] public static extern bool GetTokenInformation(IntPtr h,int cls,byte[] value,int size,out int returned);
}
"@
}
function Get-ChildAbsent {
 $attributes=[PaccUacQualificationNative]::GetFileAttributesW($Child)
 if($attributes -ne [uint32]::MaxValue){return $false}
 $errorCode=[Runtime.InteropServices.Marshal]::GetLastWin32Error()
 if($errorCode -eq 2 -or $errorCode -eq 3){return $true}
 throw 'TARGET_ABSENCE_UNKNOWN'
}
function Get-Operator {
 $identity=[Security.Principal.WindowsIdentity]::GetCurrent()
 try {
  $principal=[Security.Principal.WindowsPrincipal]::new($identity)
  $thread=[Security.Principal.WindowsIdentity]::GetCurrent($true);$impersonation=$null -ne $thread
  if($impersonation){$thread.Dispose()}
  $elev=0;$type=0;$size=0;$im=0;$om=0
  $eq=[PaccUacQualificationNative]::GetTokenInformation($identity.Token,20,[ref]$elev,4,[ref]$size)
  $tq=[PaccUacQualificationNative]::GetTokenInformation($identity.Token,18,[ref]$type,4,[ref]$size)
  $stats=[byte[]]::new(128)
  $sq=[PaccUacQualificationNative]::GetTokenInformation($identity.Token,10,$stats,$stats.Length,[ref]$size)
  if(-not $sq -or $size -gt $stats.Length -or $size -le 0){throw 'TOKEN_QUERY_DENIED'}
  # Memory-only equality witness; never included in receipt or exported.
  $witness=[Convert]::ToBase64String($stats,0,$size)
  $admin=$principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
  $interactive=$principal.IsInRole([Security.Principal.SecurityIdentifier]::new('S-1-5-4'))
  $service=$principal.IsInRole([Security.Principal.SecurityIdentifier]::new('S-1-5-6'))
  $session=[Diagnostics.Process]::GetCurrentProcess().SessionId
  $cin=[PaccUacQualificationNative]::GetConsoleMode([PaccUacQualificationNative]::GetStdHandle(-10),[ref]$im)
  $cout=[PaccUacQualificationNative]::GetConsoleMode([PaccUacQualificationNative]::GetStdHandle(-11),[ref]$om)
  return [pscustomobject]@{sid=$identity.User.Value;name=$identity.Name;witness=$witness;console=($cin -and $cout);eligible=($eq -and $tq -and $elev -eq 1 -and $type -ne 3 -and $admin -and $interactive -and -not $service -and -not $impersonation -and $session -ne 0 -and $session -eq [PaccUacQualificationNative]::WTSGetActiveConsoleSessionId())}
 } finally {$identity.Dispose()}
}
function Assert-SameOperator($Before,$After) {
 if($Before.sid -cne $After.sid -or $Before.witness -cne $After.witness -or -not $After.eligible -or -not $After.console){throw 'OPERATOR_OR_CONTEXT_CHANGED'}
}
function New-Receipt($Operator,[string]$Result,$Code,$Absent) {
 return [ordered]@{operator_sid=$Operator.sid;operator_name=$Operator.name;eligible=$Operator.eligible;interactive_console=$Operator.console;binary_sha256=$ExeHash;confirmation_result=$Result;exit_code=$Code;target_child_absent=$Absent;bootstrap='NOT_RUN';ca_custody='DENY';current_publication='DENY';signature_authority='DENY';ed11='PARTIAL'}
}
function Save-Receipt($Receipt,[string]$Id) {
 if($Id -cnotmatch '^[0-9a-f]{32}$'){throw 'INVALID_ATTEMPT'}
 Assert-PlainPath ([IO.Path]::GetDirectoryName($ReceiptRoot))
 if(-not [IO.Directory]::Exists($ReceiptRoot)){[IO.Directory]::CreateDirectory($ReceiptRoot)|Out-Null}
 Assert-PlainPath $ReceiptRoot
 $path=$ReceiptRoot+'\'+$Id+'.json'
 $s=[IO.File]::Open($path,[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::Read)
 try {$b=[Text.Encoding]::UTF8.GetBytes(($Receipt|ConvertTo-Json));$s.Write($b,0,$b.Length);$s.Flush()} finally {$s.Dispose()}
}
function Parse-Result([string]$Diagnostic,[int]$Code) {
 # Whitelist exact status only; never persist raw console/diagnostic text.
 if($Code -ne 2){return 'UNEXPECTED_EXIT_DENY'}
 $pattern='(?m)^FIRST_INSTALL=(EXACT_CONFIRMATION_DATA_ONLY|DENY_OR_CANCELLED); RESOURCES=(CLOSED|RETAINED_UNKNOWN); CA_CUSTODY=ABSENT; ED11=PARTIAL; BOOTSTRAP=DENY; CURRENT_PUBLICATION=DENY\r?$'
 $lines=@($Diagnostic -split "\r?\n" | Where-Object {$_ -cne ''})
 if($lines.Count -ne 2 -or $lines[0] -cnotmatch '^CUSTODY_METADATA_ONLY: target=[0-3] ca=0 effect_denied=1$'){return 'UNVERIFIED_DIAGNOSTIC_DENY'}
 $m=[regex]::Matches($lines[1],$pattern)
 if($m.Count -ne 1){return 'UNVERIFIED_DIAGNOSTIC_DENY'}
 if($m[0].Groups[2].Value -eq 'RETAINED_UNKNOWN'){return 'RETAINED_UNKNOWN_NO_RETRY'}
 if($m[0].Groups[1].Value -eq 'EXACT_CONFIRMATION_DATA_ONLY' -and $lines[0] -ceq 'CUSTODY_METADATA_ONLY: target=0 ca=0 effect_denied=1'){return 'READ_ONLY_WORKER_VERIFIED_FINAL_DENY'}
 return 'DENIED_OR_CANCELLED'
}
function Wait-ForManualClose {
 # Never let the ceremony Enter/paste/queued keys dismiss the result window.
 # No pause in redirected negative tests; no confirmation or authority here.
 if([Console]::IsInputRedirected -or [Console]::IsOutputRedirected){return}
 try {
  if('PaccUacQualificationNative' -as [type]){
   [void][PaccUacQualificationNative]::FlushConsoleInputBuffer([PaccUacQualificationNative]::GetStdHandle(-10))
  }
  [Console]::WriteLine('Resultado preservado. Pressione F10 para fechar esta janela (Enter nao fecha).')
  do {$closeKey=[Console]::ReadKey($true)} while($closeKey.Key -ne [ConsoleKey]::F10)
 } catch {[Console]::WriteLine('RESULT_WINDOW_INPUT_UNAVAILABLE')}
}
function Is-UacCancellation($Exception) {
 while($null -ne $Exception){
  if($Exception -is [ComponentModel.Win32Exception] -and $Exception.NativeErrorCode -eq 1223){return $true}
  $Exception=$Exception.InnerException
 }
 return $false
}
$held=[Collections.Generic.List[IO.FileStream]]::new()
$exitCode=2;$op=$null;$receiptSaved=$false;$code=$null;$absent=$null
try {
 if($PSHOME -ine 'C:\Windows\System32\WindowsPowerShell\v1.0'){throw 'UNSUPPORTED_POWERSHELL_HOST'}
 # Only OS-shipped modules by absolute path; never resolve user module folders.
 Import-Module -Name ($PSHOME+'\Modules\Microsoft.PowerShell.Management\Microsoft.PowerShell.Management.psd1') -ErrorAction Stop
 Import-Module -Name ($PSHOME+'\Modules\Microsoft.PowerShell.Utility\Microsoft.PowerShell.Utility.psd1') -ErrorAction Stop
 if($PSCommandPath -cne $ScriptPath){throw 'UNTRUSTED_SCRIPT_LOCATION'}
 if($Mode -eq 'Elevated'){
  if(-not $OriginSid -or -not $Attempt -or -not $ExpectedScriptSha256){throw 'MISSING_ELEVATED_BINDING'}
 } elseif($OriginSid -or $Attempt -or $ExpectedScriptSha256){throw 'UNEXPECTED_INTERNAL_ARGUMENTS'}
 Assert-PlainPath $ScriptPath
 $self=[IO.File]::Open($ScriptPath,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read);$held.Add($self)
 $selfHash=Get-Hash $self
 if($Mode -eq 'Elevated' -and $selfHash -cne $ExpectedScriptSha256){throw 'SCRIPT_CHANGED_ACROSS_UAC'}
 foreach($p in $Pins.Keys){$s=Open-Pinned $p $Pins[$p];$held.Add($s);if($p -ceq $Exe){Assert-ConsoleExecutable $s}}
 # Do not run from a user-controlled current directory or custom module path.
 Initialize-NativeProbe
 $op=Get-Operator
 if($Mode -eq 'Check'){
  New-Receipt $op 'CHECK_ONLY_NO_EXECUTION' $null (Get-ChildAbsent) | ConvertTo-Json
  $exitCode=2 # Check never qualifies the ceremony, even on an eligible host.
 } elseif($Mode -eq 'Launch'){
  if(-not $op.console){throw 'NO_INTERACTIVE_CONSOLE_NO_UAC'}
  Assert-PlainPath $PowerShellExe
  $id=[Guid]::NewGuid().ToString('N') # receipt correlation only; NOT ceremony nonce
  # All tokens below are fixed paths or strictly validated SID/hash/hex IDs.
  $argv=@('-NoProfile','-ExecutionPolicy','Bypass','-File',$ScriptPath,'-Mode','Elevated','-OriginSid',$op.sid,'-Attempt',$id,'-ExpectedScriptSha256',$selfHash)
  foreach($v in $argv){if($v -cmatch '[\s"`;$&|<>]'){throw 'UNSAFE_LAUNCH_ARGUMENT'}}
  try {
   # OWNER MANUAL ONLY. Owner authorized per-process ExecutionPolicy Bypass.
   # No persistent policy changes; no credentials/password variables. UAC owns its prompt.
   $process=Start-Process -FilePath $PowerShellExe -ArgumentList $argv -Verb RunAs -WindowStyle Normal -WorkingDirectory $Parent -PassThru -Wait
   try {$process.Refresh();$exitCode=$process.ExitCode;[Console]::WriteLine('ELEVATED_QUALIFICATION_EXIT='+$exitCode)} finally {$process.Dispose()}
  } catch {
   if(Is-UacCancellation $_.Exception){Save-Receipt (New-Receipt $op 'UAC_CANCELLED_DENY' 1223 (Get-ChildAbsent)) $id;$exitCode=2;[Console]::WriteLine('UAC_CANCELLED_DENY')}
   else {throw}
  }
 } else {
  if(-not $op.console -or -not $op.eligible){throw 'ELEVATED_CONTEXT_INELIGIBLE'}
  if(-not (Get-ChildAbsent)){throw 'TARGET_PREEXISTS_DENY'}
  [Console]::WriteLine('READ-ONLY qualification. Effective operator: '+$op.name+' ('+$op.sid+')')
  if($op.sid -cne $OriginSid){
   [Console]::WriteLine('UAC authenticated a DIFFERENT operator. No equivalence is assumed.')
   [Console]::WriteLine('To use this effective operator for this read-only test, type USE_EFFECTIVE_OPERATOR; otherwise cancel.')
   if([Console]::ReadLine() -cne 'USE_EFFECTIVE_OPERATOR'){throw 'EFFECTIVE_OPERATOR_NOT_SELECTED'}
  }
  Assert-SameOperator $op (Get-Operator)
  # Fixed grammar/source/parent/name. No passwords, CA_INITIALIZE or arbitrary command.
  $argv=@('config','first-install','--declare','NEW_INSTALL','--source',$Source,'--parent',$Parent,'--container',$Leaf)
  $info=[Diagnostics.ProcessStartInfo]::new();$info.FileName=$Exe;$info.WorkingDirectory=$Parent
  $info.UseShellExecute=$false;$info.CreateNoWindow=$false
  $info.RedirectStandardInput=$false;$info.RedirectStandardOutput=$false;$info.RedirectStandardError=$true
  # Fixed canonical arguments contain no whitespace/quotes/shell metacharacters.
  foreach($v in $argv){if($v -cmatch '[\s"`;$&|<>]'){throw 'UNSAFE_CHILD_ARGUMENT'}}
  $info.Arguments=$argv -join ' '
  $process=[Diagnostics.Process]::new();$process.StartInfo=$info
  try {
   if(-not $process.Start()){throw 'CHILD_START_FAILED'}
   $read=$process.StandardError.ReadToEndAsync()
   $process.WaitForExit();$diagnostic=$read.GetAwaiter().GetResult();$code=$process.ExitCode
  } finally {$process.Dispose()}
  $after=Get-Operator;Assert-SameOperator $op $after
  $absent=Get-ChildAbsent
  $result=Parse-Result $diagnostic $code
  if(-not $absent){$result='UNEXPECTED_PERSISTENT_EFFECT_STOP'}
  Save-Receipt (New-Receipt $op $result $code $absent) $Attempt;$receiptSaved=$true
  [Console]::WriteLine('QUALIFICATION='+$result+'; CLI_EXIT='+$code+'; CHILD_ABSENT='+$absent)
  [Console]::WriteLine('CA/CURRENT/signature remain DENY. UNKNOWN is not rollback; never retry it automatically.')
  $exitCode=2
  if($result -eq 'READ_ONLY_WORKER_VERIFIED_FINAL_DENY' -and $absent){$exitCode=0}
  # Result window is held by Wait-ForManualClose in finally (F10 only).
 }
} catch {
 # Codes only: no raw stderr, plan, source text, token bytes or secret logs.
 $reason='PRECONDITION_OR_IO_DENY'
 if($_.Exception.Message -cmatch '^[A-Z][A-Z0-9_]+$'){$reason=$_.Exception.Message}
 if($Mode -eq 'Elevated' -and $null -ne $op -and $Attempt -cmatch '^[0-9a-f]{32}$' -and -not $receiptSaved){
  try {Save-Receipt (New-Receipt $op ('DENY_'+$reason) $code $absent) $Attempt;$receiptSaved=$true} catch {[Console]::WriteLine('RECEIPT_NOT_SAVED_DENY')}
 }
 [Console]::WriteLine('QUALIFICATION=DENY; REASON='+$reason)
 $exitCode=2
} finally {
 try {foreach($s in $held){$s.Dispose()}}
 finally {if($Mode -eq 'Elevated'){Wait-ForManualClose}}
}
exit $exitCode
