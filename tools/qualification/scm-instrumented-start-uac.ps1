#requires -Version 5.1
[CmdletBinding()]
param(
 [ValidateSet('Check','Prepare','Launch','ElevatedCheck','ElevatedRun')][string]$Mode='Check',
 [ValidatePattern('^$|^[0-9a-f]{64}$')][string]$ExpectedScriptSha256='',
 [ValidatePattern('^$|^[0-9a-f]{64}$')][string]$ExpectedOperationApprovalSha256='',
 [uint32]$LaunchProcessId=0,[uint64]$LaunchCreation=0
)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$Root='C:\Projetos\PapinhoAccelerator'
$Script=$Root+'\tools\qualification\scm-instrumented-start-uac.ps1'
$PSExe='C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe'
$PSHash='9785001b0dcf755eddb8af294a373c0b87b2498660f724e76c4d53f9c217c7a3'
$Exe=$Root+'\build\console-proof-20261023\Selected\Release\papacc_scm_instrumented_lab.exe'
$ExeHash='e76df9084e62e761bd24e818074c4aed1dcff520b63aa38660275400ed153a51'
$Candidate=$Root+'\build\session0-diagnostic-runtime-integration-20261023\Selected\Release\papacc_server_lab_host.exe'
$CandidateHash='eda84c0cf91ad1266cee462e62c89b84ffa5873853a0122215d15d666701361b'
$Policy=$Root+'\docs\adr\DECISION-ed11-single-lab-diagnostic-native-boundary-20261023.md'
$PolicyHash='e7031bd29c9bd174359ffcc3c9a021a6ad25a0256a5cf03b40c0866d3a5776b7'
$ReceiptRoot=$Root+'\docs\integration\canonical-preflight-busy-diagnosis-20261023'
$Approval=$ReceiptRoot+'\EXACT-OPERATION-APPROVED.json'
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
$script:RetainedNativeProcess=$null
$pins=@();$code=2;$pause=$false;$log=$null;$logFailed=$false;$retentionSaved=$false;$lineChars=0
$lines=[Collections.Generic.List[string]]::new()
try {
 if($PSCommandPath -cne $Script -or [IntPtr]::Size -ne 8){throw 'SCRIPT_OR_ARCHITECTURE'}
 Assert-Plain $Script;$self=Path-Hash $Script
 if($ExpectedScriptSha256 -ne '' -and $self -cne $ExpectedScriptSha256){throw 'SCRIPT_PIN'}
 if($Mode -like 'Elevated*' -and $ExpectedScriptSha256 -eq ''){throw 'SCRIPT_PIN_REQUIRED'}
 $pins+=Pin $Script $self;$pins+=Pin $Exe $ExeHash;$pins+=Pin $Candidate $CandidateHash;$pins+=Pin $Policy $PolicyHash;$pins+=Pin $PSExe $PSHash
 if($Mode -in @('Launch','ElevatedRun')){
  if($ExpectedOperationApprovalSha256 -eq ''){throw 'SEPARATE_EXACT_OPERATION_APPROVAL_REQUIRED'}
  $pins+=Pin $Approval $ExpectedOperationApprovalSha256
  $approvalRecord=Get-Content -LiteralPath $Approval -Raw|ConvertFrom-Json
  if($approvalRecord.approved -ne $true -or $approvalRecord.attempt -cne $Attempt -or $approvalRecord.service -cne 'PapinhoAcceleratorLab20261023' -or $approvalRecord.policy_sha256 -cne $PolicyHash -or $approvalRecord.qualifier_sha256 -cne $ExeHash -or $approvalRecord.script_sha256 -cne $self -or $approvalRecord.candidate_sha256 -cne $CandidateHash -or $approvalRecord.start_limit -ne 1 -or $approvalRecord.stop_limit -ne 1){throw 'EXACT_OPERATION_APPROVAL_MISMATCH'}
 }
 if($Mode -eq 'Check'){
  Write-Output 'INSTRUMENTED_LAUNCHER_PINS=PASS; NO_NATIVE_ENTRY_NO_UAC_NO_FILE_CREATE_NO_REBIND_NO_START; OPERATION_APPROVAL=SEPARATE';$code=0
 }elseif($Mode -in @('Prepare','Launch')){
  $ctx=Context;if(-not $ctx.console -or $ctx.impersonating -or $ctx.sid -cne $Operator){throw 'INTERACTIVE_OWNER_REQUIRED'}
  $childMode='ElevatedCheck';if($Mode -eq 'Launch'){$childMode='ElevatedRun'}
  $a='-NoProfile -ExecutionPolicy Bypass -File "'+$Script+'" -Mode '+$childMode+' -ExpectedScriptSha256 '+$self+' -LaunchProcessId '+$PID+' -LaunchCreation '+(Creation $PID)
  if($ExpectedOperationApprovalSha256 -ne ''){$a+=' -ExpectedOperationApprovalSha256 '+$ExpectedOperationApprovalSha256}
  $child=Start-Process -FilePath $PSExe -Verb RunAs -ArgumentList $a -WorkingDirectory $Root -WindowStyle Normal -Wait -PassThru
  try{$code=$child.ExitCode}finally{$child.Dispose()};Write-Output ('INSTRUMENTED_LAB_EXIT='+$code)
 }else{
  $ctx=Context;$pause=$ctx.console
  if(-not $ctx.console -or -not $ctx.administrators -or $ctx.impersonating -or $ctx.sid -cne $Operator -or -not $LaunchProcessId -or -not $LaunchCreation){throw 'ELEVATED_INTERACTIVE_OWNER_REQUIRED'}
  Assert-Plain $ReceiptRoot
  $positive=$Mode -eq 'ElevatedRun';$nativeMode='--prepare';if($positive){$nativeMode='--cycle-lab'}
  $nativeArgs=@($nativeMode,$Attempt,[string]$LaunchProcessId,[string]$LaunchCreation,[string]$PID,[string](Creation $PID))
  if($positive){$nativeArgs+=@($ExpectedOperationApprovalSha256)}
  $suffix='prepare-'+(Creation $PID);if($positive){$suffix=$Attempt}
  if($positive){Assert-Plain ($Root+'\build\scm-protected-installation-binding-20261022\Selected\Release')}
  $logPath=$ReceiptRoot+'\manual-'+$suffix+'.log';if($positive){$logPath=$Root+'\build\scm-protected-installation-binding-20261022\Selected\Release\papacc-psr-'+$Attempt+'.launch.log'};$log=[IO.File]::Open($logPath,[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::Read)
  if($positive){$header='ADMISSION='+$Attempt+'; OWNER='+$PID+'; CREATION='+(Creation $PID)+[char]10;$b=[Text.Encoding]::ASCII.GetBytes($header);$log.Write($b,0,$b.Length);$log.Flush($true)}
  $saved=$ErrorActionPreference
  try{
   $ErrorActionPreference='Continue'
   Invoke-InheritedConsoleNative $Exe $nativeArgs | ForEach-Object {
    $line=$_.ToString();Write-Host $line
    if($line.Length -gt 4096 -or $lines.Count -ge 4096 -or $lineChars+$line.Length -gt 4194304){$logFailed=$true}else{$lines.Add($line);$lineChars+=$line.Length}
    if(-not $logFailed){try{$b=[Text.Encoding]::UTF8.GetBytes($line+[Environment]::NewLine);$log.Write($b,0,$b.Length);$log.Flush($true)}catch{$logFailed=$true}}
    if($line.StartsWith('IG_RETAINED') -and -not $retentionSaved){
     try{Save-New ($ReceiptRoot+'\manual-'+$Attempt+'-retained.json') @{attempt=$Attempt;operator=$ctx;qualifier_sha256=$ExeHash;candidate_sha256=$CandidateHash;policy_sha256=$PolicyHash;script_sha256=$self;disposition='UNKNOWN_REFERENCES_RETAINED';native_line=$line;log=$logPath;log_failed=$logFailed;output=@($lines.ToArray());retry='DENY';release='NOT_CONFIRMED';ed11='PARTIAL';authority='DENY'};$retentionSaved=$true}catch{$logFailed=$true}
     Write-Host 'RETAINED OWNER: do not close/retry/kill. Durable disposition is UNKNOWN, not a release receipt.'
    }
   }
   $nativeExit=$LASTEXITCODE
  }finally{
   $ErrorActionPreference=$saved
   if($null -ne $script:RetainedNativeProcess){
    Write-Host 'IG_RETAINED_NATIVE_CAPTURE; PROCESS_EXIT=UNKNOWN; PINS_AND_LOG_RETAINED; NO_CLOSE_NO_RETRY_NO_KILL'
    try{Save-New ($ReceiptRoot+'\manual-'+$suffix+'-capture-retained.json') @{attempt=$Attempt;disposition='UNKNOWN_NATIVE_CAPTURE';script_sha256=$self;qualifier_sha256=$ExeHash;log=$logPath;retry='DENY';release='NOT_CONFIRMED';authority='DENY'}}catch{Write-Host 'RETAINED_RECEIPT_FAILED; UNKNOWN_PRESERVED'}
    while($true){[Threading.Thread]::Sleep(1000)}
   }
   $log.Dispose();$log=$null
  }
  $captured=@($lines|Where-Object{$_ -like 'IG_DIAGNOSTIC_RECEIPT *'}).Count -eq 1
  $pass=-not $logFailed -and $nativeExit -eq 0
  if($positive){$pass=$pass -and $lines.Contains('IG_CYCLE_QUALIFIED; READINESS_TOKEN_STOP_QUIESCENCE_CAPTURE_FACTUAL; NO_AUTHORITY') -and $captured}
  $receipt=@{attempt=$Attempt;operator=$ctx;policy_sha256=$PolicyHash;script_sha256=$self;qualifier_sha256=$ExeHash;candidate_sha256=$CandidateHash;native_exit=$nativeExit;diagnostic_captured=$captured;cycle_pass=$pass;positive=$positive;log_failed=$logFailed;output=@($lines.ToArray());log=$logPath;retry='DENY';rollback='NOT_CLAIMED';deletion='NOT_RUN';host_credits='NONE_GRANTED';ed11='PARTIAL';ca='DENY';current='DENY';signature='DENY';recovery='DENY'}
  Save-New ($ReceiptRoot+'\manual-'+$suffix+'-final.json') $receipt
  Write-Output ('RECEIPT='+$ReceiptRoot+'\manual-'+$suffix+'-final.json; DIAGNOSTIC_CAPTURED='+$captured+'; CYCLE_PASS='+$pass)
  if($pass){$code=0}
 }
}catch{Write-Output ('STOP: '+$_.Exception.Message);$code=2}
finally{if($log){$log.Dispose()};foreach($pin in $pins){try{$pin.Dispose()}catch{Write-Output 'PIN_CLOSE_UNCERTAIN';$code=2}}}
if($pause){Write-Output 'Resultado preservado. F10 fecha somente após retorno factual do qualificador. Nunca repetir automaticamente.';try{while([Console]::ReadKey($true).Key -ne [ConsoleKey]::F10){}}catch{$code=2}}
exit $code
