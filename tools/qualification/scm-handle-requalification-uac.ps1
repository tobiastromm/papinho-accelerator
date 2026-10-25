#requires -Version 5.1
[CmdletBinding()]
param(
 [ValidateSet('Check','Prepare','ElevatedCheck')][string]$Mode='Check',
 [ValidatePattern('^[0-9a-f]{64}$')][Parameter(Mandatory=$true)][string]$ExpectedScriptSha256,
 [uint32]$LaunchProcessId=0,[uint64]$LaunchCreation=0
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$Root='C:\Projetos\PapinhoAccelerator'
$Script=$Root+'\tools\qualification\scm-handle-requalification-uac.ps1'
$PSExe='C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe'
$PSHash='9785001b0dcf755eddb8af294a373c0b87b2498660f724e76c4d53f9c217c7a3'
$Exe='C:\Projetos\_tmp\PapinhoAccelerator\ed11-handle-native-requalification-20261023\Selected\Release\papacc_scm_instrumented_lab.exe'
$ExeHash='42af729e154426dc7cbdd7e79f42e0c0d29490d0df2937cd3995ea4b0960763d'
$Candidate=$Root+'\build\session0-diagnostic-runtime-integration-20261023\Selected\Release\papacc_server_lab_host.exe'
$CandidateHash='eda84c0cf91ad1266cee462e62c89b84ffa5873853a0122215d15d666701361b'
$Policy=$Root+'\docs\adr\DECISION-ed11-single-lab-diagnostic-native-boundary-20261023.md'
$PolicyHash='e7031bd29c9bd174359ffcc3c9a021a6ad25a0256a5cf03b40c0866d3a5776b7'
$ReceiptRoot=$Root+'\docs\integration\ed11-handle-native-requalification-20261023'
$Attempt='75330d11cc7e4123ae62eadc9a59c2f5'
$Operator='S-1-5-21-785699550-1440458426-1682014700-1108'
function Assert-Plain([string]$Path) {
 if([IO.Path]::GetFullPath($Path) -cne $Path){throw 'NONCANONICAL_PATH'}
 $x=$Path;while($x){if(([IO.File]::GetAttributes($x) -band [IO.FileAttributes]::ReparsePoint) -ne 0){throw 'REPARSE_PATH'};$x=[IO.Path]::GetDirectoryName($x)}
}
function Stream-Hash($f) {$s=[Security.Cryptography.SHA256]::Create();try{return ([BitConverter]::ToString($s.ComputeHash($f))).Replace('-','').ToLowerInvariant()}finally{$s.Dispose()}}
function Path-Hash([string]$Path) {$f=[IO.File]::Open($Path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read);try{return Stream-Hash $f}finally{$f.Dispose()}}
function Pin([string]$Path,[string]$Hash) {Assert-Plain $Path;$f=[IO.File]::Open($Path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read);try{if((Stream-Hash $f) -cne $Hash){throw 'PIN_MISMATCH'};return $f}catch{$f.Dispose();throw}}
function Creation([uint32]$ProcessId) {$p=[Diagnostics.Process]::GetProcessById([int]$ProcessId);try{return [uint64]$p.StartTime.ToUniversalTime().ToFileTimeUtc()}finally{$p.Dispose()}}
function Context {
 $id=[Security.Principal.WindowsIdentity]::GetCurrent();try{$sid=$id.User.Value;$admin=([Security.Principal.WindowsPrincipal]::new($id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)}finally{$id.Dispose()}
 $thread=[Security.Principal.WindowsIdentity]::GetCurrent($true);$imp=$null -ne $thread;if($thread){$thread.Dispose()}
 $console=$false;try{$console=-not [Console]::IsInputRedirected -and -not [Console]::IsOutputRedirected}catch{}
 return [pscustomobject]@{sid=$sid;administrators=$admin;console=$console;impersonating=$imp;token_elevation='VERIFIED_BY_NATIVE_OWNER'}
}
function Save-New([string]$Path,$Value) {
 $f=[IO.File]::Open($Path,[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::Read)
 try{$bytes=[Text.Encoding]::UTF8.GetBytes(($Value|ConvertTo-Json -Depth 8));$f.Write($bytes,0,$bytes.Length);$f.Flush($true)}finally{$f.Dispose()}
}
# No PowerShell native-command launcher: inherit the already-associated console.
# All governed diagnostic records are stdout; stderr remains on the operator console.
function Invoke-InheritedConsoleNative([string]$NativePath,[string[]]$NativeArguments) {
 foreach($argument in $NativeArguments){if($argument -cnotmatch '^[A-Za-z0-9_-]+$'){throw 'NATIVE_ARGUMENT_DENY'}}
 $start=[Diagnostics.ProcessStartInfo]::new()
 $start.FileName=$NativePath;$start.Arguments=$NativeArguments -join ' '
 $start.UseShellExecute=$false;$start.CreateNoWindow=$false
 $start.RedirectStandardOutput=$true;$start.RedirectStandardError=$false;$start.RedirectStandardInput=$false
 $process=[Diagnostics.Process]::new();$process.StartInfo=$start
 $started=$false;$exited=$false;$reader=$null
 try {
  if(-not $process.Start()){throw 'NATIVE_CREATE_FAILED'};$started=$true
   $reader=$process.StandardOutput
  while($null -ne ($nativeLine=$reader.ReadLine())){Write-Output $nativeLine}
  $process.WaitForExit();$script:LASTEXITCODE=$process.ExitCode;$exited=$true
 } finally {
  if(-not $started){$process.Dispose()}
  elseif($exited){
   try{$reader.Dispose();$process.Dispose()}catch{$script:RetainedNativeProcess=$process;throw}
  }else{$script:RetainedNativeProcess=$process}
 }
}

function Read-RequalificationResult([string[]]$OutputLines) {
  $observed='UNKNOWN';$gate='NOT_REACHED';$stage='UNKNOWN';$predicate='UNKNOWN';$census='NOT_REACHED';$exclusive='NOT_PROVED';$retention='UNKNOWN'
  foreach($line in $OutputLines){
   if($line -match '^IG_HANDLE_REQUALIFICATION observed=1 handles=(\d+) '){$observed=[uint32]$Matches[1]}
   if($line -match '^IG_PROCESS role=2 .* handles=(\d+) '){$observed=[uint32]$Matches[1]}
   if($line -match '^IG_CENSUS_REJECT .* child=(\d+) project_prefix=(\d+)'){
    if($Matches[1]-eq'1'){$predicate='!participant && child'}else{$predicate='!participant && project_prefix'};$census='DENY'
   }
   if($line -match '^IG_DIAGNOSTIC stage=([^ ]+) '){$stage=$Matches[1]}
   if($line -match '^IG_FINAL stage=([^ ]+) first=(\d+) close=0 '){
    if($Matches[2]-eq'0'){$stage='NONE';$gate='PASS';$census='PASS'}
    $retention='NATIVE_OWNED_REFERENCES_CLOSED'
    if($Matches[1] -in @('SOURCE_IMAGE','SCM_READBACK','CONFIG_BYTE_LOCK','PREFLIGHT_CENSUS','PREFLIGHT_METRICS')){$exclusive='ACQUIRED_AND_CLOSED'}
   }
  }
 return @{HANDLE_COUNT_OBSERVED=$observed;HANDLE_LIMIT=128;HANDLE_GATE=$gate;FIRST_FAILED_STAGE=$stage;FIRST_FAILED_PREDICATE=$predicate;CONSOLE_CENSUS=$census;CANONICAL_EXCLUSIVITY=$exclusive;RESOURCE_RETENTION=$retention}
}
$Capture='ED11H-20261023-01'
$Confirmation='REQUALIFY_ED11_HANDLES_READ_ONLY ED11H-20261023-01'
$script:RetainedNativeProcess=$null
$pins=@();$log=$null;$code=2;$pause=$false;$lines=[Collections.Generic.List[string]]::new();$chars=0;$logFailed=$false
try {
 if($PSCommandPath -cne $Script -or [IntPtr]::Size -ne 8){throw 'SCRIPT_OR_ARCHITECTURE'}
 if((Path-Hash $Script) -cne $ExpectedScriptSha256){throw 'SCRIPT_PIN'}
 $pins+=Pin $Script $ExpectedScriptSha256;$pins+=Pin $Exe $ExeHash;$pins+=Pin $Candidate $CandidateHash;$pins+=Pin $Policy $PolicyHash;$pins+=Pin $PSExe $PSHash
 if($Mode -eq 'Check'){Write-Output 'READ_ONLY_LAUNCHER_PINS=PASS; NO_NATIVE_ENTRY_NO_UAC_NO_PREFLIGHT';$code=0}
 elseif($Mode -eq 'Prepare'){
  $ctx=Context;if(-not $ctx.console -or $ctx.impersonating -or $ctx.sid -cne $Operator){throw 'INTERACTIVE_OWNER_REQUIRED'}
  $argsText='-NoProfile -ExecutionPolicy Bypass -File "'+$Script+'" -Mode ElevatedCheck -ExpectedScriptSha256 '+$ExpectedScriptSha256+' -LaunchProcessId '+$PID+' -LaunchCreation '+(Creation $PID)
  $child=Start-Process -FilePath $PSExe -Verb RunAs -ArgumentList $argsText -WorkingDirectory $Root -WindowStyle Normal -Wait -PassThru
  try{$code=$child.ExitCode}finally{$child.Dispose()}
  Write-Output ('HANDLE_REQUALIFICATION_EXIT='+$code)
 }else{
  $ctx=Context;$pause=$ctx.console
  if(-not $ctx.console -or -not $ctx.administrators -or $ctx.impersonating -or $ctx.sid -cne $Operator -or -not $LaunchProcessId -or -not $LaunchCreation){throw 'ELEVATED_INTERACTIVE_OWNER_REQUIRED'}
  Assert-Plain $ReceiptRoot
  Write-Host ('READ-ONLY '+$Capture+'; LAB=PapinhoAcceleratorLab20261023; ATTEMPT='+$Attempt)
  Write-Host ('QUALIFIER='+$Exe+'; SHA256='+$ExeHash)
  Write-Host 'One --prepare only. No Start/Stop/rebind/ACL/operational diagnostic file or intent consumption. Existing boot236441 and protected SCM/config identities must match; mismatch STOPS.128 unchanged; credits NONE_GRANTED.'
  Write-Host 'Post-census self handle sample is diagnostic only; rejected census means HANDLE_GATE=NOT_REACHED. No PSS/trust supplemental capture. No retry.'
  Write-Host ('Type exactly: '+$Confirmation)
  if((Read-Host) -cne $Confirmation){throw 'CONFIRMATION_DENY'}
  # Administrative one-shot evidence only, never an operational attempt intent.
  Save-New ($ReceiptRoot+'\'+$Capture+'-claim.json') @{capture=$Capture;operator=$ctx;script_sha256=$ExpectedScriptSha256;qualifier_sha256=$ExeHash;created_utc=[DateTime]::UtcNow.ToString('o');retry='DENY';operational_intent_consumed=$false}
  $logPath=$ReceiptRoot+'\'+$Capture+'.log'
  $log=[IO.File]::Open($logPath,[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::Read)
  Invoke-InheritedConsoleNative $Exe @('--prepare',$Attempt,[string]$LaunchProcessId,[string]$LaunchCreation,[string]$PID,[string](Creation $PID)) | ForEach-Object {
   $line=$_.ToString();Write-Host $line
   if($line.Length -gt 4096 -or $lines.Count -ge 4096 -or $chars+$line.Length -gt 4194304){$logFailed=$true}else{$lines.Add($line);$chars+=$line.Length}
   if(-not $logFailed){try{$b=[Text.Encoding]::UTF8.GetBytes($line+[Environment]::NewLine);$log.Write($b,0,$b.Length);$log.Flush($true)}catch{$logFailed=$true}}
   if($line.StartsWith('IG_RETAINED')){
    Save-New ($ReceiptRoot+'\'+$Capture+'-retained.json') @{capture=$Capture;output=@($lines.ToArray());release='UNKNOWN';retry='DENY';ed11='PARTIAL';authority='DENY'}
    Write-Host 'UNKNOWN: preserve this window and references; no retry/kill/forced close.'
   }
  }
  $nativeExit=$LASTEXITCODE
  $parsed=Read-RequalificationResult @($lines.ToArray())
  $result=@{capture=$Capture;attempt=$Attempt;script_sha256=$ExpectedScriptSha256;qualifier_sha256=$ExeHash;candidate_sha256=$CandidateHash;policy_sha256=$PolicyHash;operator=$ctx;native_exit=$nativeExit;HANDLE_COUNT_OBSERVED=$parsed.HANDLE_COUNT_OBSERVED;HANDLE_LIMIT=128;HANDLE_GATE=$parsed.HANDLE_GATE;FIRST_FAILED_STAGE=$parsed.FIRST_FAILED_STAGE;FIRST_FAILED_PREDICATE=$parsed.FIRST_FAILED_PREDICATE;CONSOLE_CENSUS=$parsed.CONSOLE_CENSUS;CANONICAL_EXCLUSIVITY=$parsed.CANONICAL_EXCLUSIVITY;RESOURCE_RETENTION=$parsed.RESOURCE_RETENTION;log_failed=$logFailed;output=@($lines.ToArray());START_EXECUTED='NO';SCM_MUTATIONS='NONE';ACL_MUTATIONS='NONE';intent_consumed=$false;retry='DENY';ed11='PARTIAL';current='DENY';signature='DENY';recovery='DENY'}
  Save-New ($ReceiptRoot+'\'+$Capture+'-final.json') $result
  Write-Output ($result|ConvertTo-Json -Depth 5)
  if($nativeExit-eq0 -and -not $logFailed){$code=0}
 }
}catch{
 $failure=$_.Exception.Message;Write-Output ('STOP: '+$failure);$code=2
 if($Mode -eq 'ElevatedCheck'){
  try{Save-New ($ReceiptRoot+'\'+$Capture+'-failure-'+[DateTime]::UtcNow.ToFileTimeUtc()+'.json') @{capture=$Capture;failure=$failure;output=@($lines.ToArray());native_completion='NOT_PROVED_BY_EXCEPTION';resource_retention='UNKNOWN_UNTIL_FINAL_NATIVE_RECEIPT';retry='DENY';authority='DENY'}}catch{Write-Host 'FAILURE_RECEIPT_UNAVAILABLE; PRESERVE_CONSOLE'}
 }
}
finally{
 if($null-ne$script:RetainedNativeProcess){Write-Host 'NATIVE_EXIT_UNKNOWN; PINS_AND_LOG_RETAINED; NO_RETRY_NO_KILL';while($true){[Threading.Thread]::Sleep(1000)}}
 if($log){$log.Dispose()}
 foreach($pin in $pins){try{$pin.Dispose()}catch{Write-Output 'PIN_CLOSE_UNCERTAIN';$code=2}}
}
if($pause){Write-Output 'Qualifier returned. Evidence preserved. F10 closes this operator window; no retry.';try{while([Console]::ReadKey($true).Key-ne[ConsoleKey]::F10){}}catch{$code=2}}
exit $code
