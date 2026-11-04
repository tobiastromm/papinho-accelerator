#requires -Version 5.1
[CmdletBinding()]
param([Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedScriptSha256,[Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedRequestSha256)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$folder='C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
if((Get-FileHash -LiteralPath $MyInvocation.MyCommand.Path -Algorithm SHA256).Hash-ne$ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
if((Get-FileHash -LiteralPath (Join-Path $folder 'ONE-READ-TERMINAL-READONLY-STATUS-REQUEST.CANDIDATE.md') -Algorithm SHA256).Hash-ne$ExpectedRequestSha256){throw 'REQUEST_PIN_DENY'}
if([Environment]::MachineName-cne'TCORP-TOBIAS'-or[IntPtr]::Size-ne8-or$PSVersionTable.PSEdition-ne'Desktop'-or[Console]::IsInputRedirected-or[Console]::IsOutputRedirected){throw 'CONTEXT_DENY'}
$actor=[Security.Principal.WindowsIdentity]::GetCurrent()
try{if($actor.User.Value-cne'S-1-5-21-785699550-1440458426-1682014700-1108'){throw 'OPERATOR_DENY'}}finally{$actor.Dispose()}
Write-Host 'ONE_READONLY_RETAINED_DISPOSITION; NO_RETRY_NO_STOP_NO_KILL_NO_E_Q_S; HISTORICAL_CAUSE_UNKNOWN'
if((Read-Host 'Type OBSERVE_READ_TERMINAL_STATUS_ONCE only after specific owner approval')-cne'OBSERVE_READ_TERMINAL_STATUS_ONCE'){throw 'CANCELLED_NO_RETRY'}
$first=0
foreach($verb in @('queryex','qc','qsidtype')){
 Write-Host ('SCM_READONLY_BEGIN '+$verb)
 & 'C:\Windows\System32\sc.exe' $verb 'PapinhoAccelerator'
 $code=$LASTEXITCODE
 Write-Host ('SCM_READONLY_END '+$verb+' exit='+$code)
 if($code-ne0){$first=$code;break}
}
# PID is a locator only: check the original FILETIME before attributing it.
if($first-eq0){
 $proc=$null
 try{
  $proc=Get-Process -Id 5824 -ErrorAction Stop
  $time=$proc.StartTime.ToUniversalTime().ToFileTimeUtc()
  $expected=([long]31288271*4294967296)+[long]406391211
  if($time-ne$expected){Write-Host 'CONTROLLER_IDENTITY=MISMATCH_OR_PID_REUSED; NO_ATTRIBUTION';$first=13}
  else{
   Write-Host ('CONTROLLER_ORIGINAL_CREATION_MATCH pid=5824 filetime='+$time+'; TOKEN_AND_INTERNAL_RETENTION_NOT_ATTESTED')
   Write-Host ('CONTROLLER_OBSERVED has_exited='+$proc.HasExited+'; NO_WAIT_NO_TERMINATION')
  }
 }catch{Write-Host ('CONTROLLER_QUERY=UNKNOWN; exception_type='+$_.Exception.GetType().Name);$first=1}
 finally{if($null-ne$proc){$proc.Dispose()}}
}
Write-Host ('RETAINED_READONLY_RESULT first='+$first+'; NO_RETRY; EXIT_NOT_AUTHORITY; INTERNAL_HANDLES_AND_HISTORICAL_RETENTION_AND_OS_QUIESCENCE=UNKNOWN')
exit $(if($first-eq0){0}else{2})
