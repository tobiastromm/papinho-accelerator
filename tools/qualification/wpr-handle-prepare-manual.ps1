#requires -Version 5.1
# External diagnostic coordinator ONLY. Not an Accelerator admission owner.
[CmdletBinding()]
param([Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedScriptSha256)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$diagRoot='C:\Projetos\_tmp\PapinhoAccelerator\wpr-handle-attribution-20261023-03'
$instance='WPRH-20261023-03'
$wpr='C:\Windows\System32\wpr.exe'
$launcher='C:\Projetos\PapinhoAccelerator\tools\qualification\scm-instrumented-start-uac.ps1'
$psExe='C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe'
$launcherHash='60a6dd761fee00143cdf88d7fcf629ad1a47986f41875ab1e8020c641b10bca0'
$log=$null;$subject=$null;$owned=$false;$stopAttempted=$false;$firstError=$null;$clock=$null
function Note([string]$message){Write-Host $message;if($null -ne $script:log){$script:log.WriteLine(([DateTime]::UtcNow.ToString('o')+' '+$message));$script:log.Flush()}}
function WprQuery([string[]]$arguments){$result=@(& $script:wpr @arguments 2>&1);$nativeCode=$LASTEXITCODE;foreach($line in $result){Note ([string]$line)};return [pscustomobject]@{code=$nativeCode;text=($result -join "`n")}}
function Plain([string]$path){$part=$path;while($part){if(Test-Path -LiteralPath $part){if(((Get-Item -LiteralPath $part -Force).Attributes -band [IO.FileAttributes]::ReparsePoint)-ne 0){throw 'REPARSE_STOP'}};$part=[IO.Path]::GetDirectoryName($part)}}
function Get-BootComputerHash([xml]$eventXml){
 $computerNode=$eventXml.SelectSingleNode('/*[local-name()="Event"]/*[local-name()="System"]/*[local-name()="Computer"]')
 if($null-eq$computerNode -or [string]::IsNullOrEmpty($computerNode.InnerText)){throw 'EVENT_COMPUTER_MISSING'}
 $computerSha=[Security.Cryptography.SHA256]::Create()
 try{return ([BitConverter]::ToString($computerSha.ComputeHash([Text.Encoding]::UTF8.GetBytes($computerNode.InnerText)))).Replace('-','').ToLowerInvariant()}finally{$computerSha.Dispose()}
}
try {
 if([IntPtr]::Size -ne 8 -or [Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'INTERACTIVE_X64_REQUIRED'}
 $identity=[Security.Principal.WindowsIdentity]::GetCurrent()
 try{if($identity.User.Value -ne 'S-1-5-21-785699550-1440458426-1682014700-1108' -or -not ([Security.Principal.WindowsPrincipal]::new($identity)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'ELEVATED_OWNER_REQUIRED'}}finally{$identity.Dispose()}
 $threadIdentity=[Security.Principal.WindowsIdentity]::GetCurrent($true)
 if($null-ne $threadIdentity){$threadIdentity.Dispose();throw 'IMPERSONATION_STOP'}
 $boot=Get-WinEvent -FilterHashtable @{LogName='System';ProviderName='Microsoft-Windows-Kernel-General';Id=12} -MaxEvents 1
 $bootXml=[xml]$boot.ToXml()
 $machineHash=Get-BootComputerHash $bootXml
 if($machineHash -cne '2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff'){throw 'HOST_STOP_EVENT_COMPUTER_MISMATCH'}
 $startTime=@($bootXml.Event.EventData.Data|Where-Object{$_.Name-eq'StartTime'})[0].'#text'
 if($boot.RecordId-ne236441 -or [DateTimeOffset]::Parse($startTime).UtcDateTime-ne[DateTimeOffset]::Parse('2026-10-23T18:36:34.5000000Z').UtcDateTime){throw 'BOOT_STOP'}
 $pins=@{}
 $pins[$PSCommandPath]=$ExpectedScriptSha256
 $pins[$wpr]='8c3d49a83ebd8138ffa15da93d25ba1fc44acdec3cb9d729b6f43b55a5ef7ca5'
 $pins[$launcher]=$launcherHash
 $pins[$psExe]='9785001b0dcf755eddb8af294a373c0b87b2498660f724e76c4d53f9c217c7a3'
 $pins['C:\Projetos\PapinhoAccelerator\build\console-proof-20261023\Selected\Release\papacc_scm_instrumented_lab.exe']='e76df9084e62e761bd24e818074c4aed1dcff520b63aa38660275400ed153a51'
 $pins['C:\Projetos\PapinhoAccelerator\build\session0-diagnostic-runtime-integration-20261023\Selected\Release\papacc_server_lab_host.exe']='eda84c0cf91ad1266cee462e62c89b84ffa5873853a0122215d15d666701361b'
 $pins['C:\Projetos\PapinhoAccelerator\docs\adr\DECISION-ed11-single-lab-diagnostic-native-boundary-20261023.md']='e7031bd29c9bd174359ffcc3c9a021a6ad25a0256a5cf03b40c0866d3a5776b7'
 foreach($pin in $pins.GetEnumerator()){Plain $pin.Key;if((Get-FileHash -LiteralPath $pin.Key -Algorithm SHA256).Hash.ToLowerInvariant()-cne$pin.Value){throw ('PIN_STOP '+$pin.Key)}}
 Plain $diagRoot;if(Test-Path -LiteralPath $diagRoot){throw 'DIRECTORY_EXISTS_NO_REUSE'}
 $sig=Get-AuthenticodeSignature -LiteralPath $wpr
 if($sig.Status-ne'Valid' -or $sig.SignerCertificate.Thumbprint-ne'3B77DB29AC72AA6B5880ECB2ED5EC1EC6601D847'){throw 'WPR_SIGNATURE_STOP'}
 Note ('HOST_BOOT='+$machineHash+' / '+$boot.RecordId+' / '+$startTime)
 Note ('DISK_FREE_BYTES='+[string](Get-PSDrive C).Free)
 $profile=WprQuery @('-profiledetails','Handle.Light','-filemode')
 if($profile.code-ne0 -or $profile.text-notmatch'Handle.Light.File'){throw 'PROFILE_STOP'}
 $existing=WprQuery @('-status')
 if($existing.text-notmatch'WPR is not recording'){throw 'EXISTING_OR_UNKNOWN_WPR_STOP'}
 $sessions=@(& 'C:\Windows\System32\logman.exe' query -ets 2>&1);$sessionCode=$LASTEXITCODE
 foreach($line in $sessions){Note ([string]$line)}
 if($sessionCode-ne0){throw 'COLLECTOR_VISIBILITY_STOP'}
 Note 'External observer adds this supervisor PowerShell and transient WPR processes; NOT B4 admission. No credits. Review existing collectors above.'
 Note 'One trace, one read-only Prepare, maximum30s collection before one Stop request. ETL size/kernel costs UNKNOWN. No service effects.'
 $confirmation=Read-Host 'If collectors do not conflict and disk/conditions are acceptable, type CAPTURE_WPRH_03'
 if($confirmation-cne'CAPTURE_WPRH_03'){throw 'CONFIRMATION_STOP'}
 New-Item -ItemType Directory -Path $diagRoot -ErrorAction Stop|Out-Null
 $logFile=[IO.File]::Open(($diagRoot+'\coordinator.log'),[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::Read)
 $log=[IO.StreamWriter]::new($logFile);$log.AutoFlush=$true
 Note ('SUPERVISOR_PID='+$PID+' CREATION='+[Diagnostics.Process]::GetCurrentProcess().StartTime.ToUniversalTime().ToFileTimeUtc())
 Note ('BOOT_RECORD='+$boot.RecordId+' START='+$startTime+' DISK_FREE_BYTES='+[string](Get-PSDrive C).Free)
 foreach($pin in $pins.GetEnumerator()){Note ('PIN='+$pin.Key+' SHA256='+$pin.Value)}
 $started=WprQuery @('-start','Handle.Light','-filemode','-recordtempto',$diagRoot,'-instancename',$instance)
 Note ('WPR_START_EXIT='+$started.code)
 if($started.code-ne0){throw 'WPR_START_FAILED_PARTIAL_ACQUISITION_UNKNOWN_NO_RETRY'}
 $owned=$true;$clock=[Diagnostics.Stopwatch]::StartNew()
 $status=WprQuery @('-status','-instancename',$instance)
 if($status.code-ne0 -or $status.text-notmatch'WPR recording is in progress'){throw 'WPR_START_NOT_CONFIRMED'}
 if($clock.Elapsed.TotalSeconds-ge30){throw 'WINDOW_EXPIRED_BEFORE_PREPARE'}
 Note 'WPR_RECORDING_CONFIRMED. Opening exactly ONE Prepare. Do not run commands in the child window.'
 $subject=Start-Process -FilePath $psExe -ArgumentList ('-NoProfile -ExecutionPolicy Bypass -File "'+$launcher+'" -Mode Prepare -ExpectedScriptSha256 '+$launcherHash) -WorkingDirectory 'C:\Projetos\PapinhoAccelerator' -WindowStyle Normal -PassThru
 Note ('PREPARE_LAUNCHER_PID='+$subject.Id+' CREATION='+$subject.StartTime.ToUniversalTime().ToFileTimeUtc())
 while($clock.Elapsed.TotalSeconds-lt30 -and -not$subject.HasExited){
  Write-Progress -Activity 'WPR recording; Prepare read-only' -Status ('Elapsed '+[int]$clock.Elapsed.TotalSeconds+' /30s') -PercentComplete ([Math]::Min(100,[int]($clock.Elapsed.TotalSeconds/30*100)))
  [void]$subject.WaitForExit(250)
 }
 Write-Progress -Activity 'WPR recording; Prepare read-only' -Completed
} catch {$firstError=$_.Exception.Message;Note ('FIRST_ERROR='+$firstError)}
finally {
 if($owned -and -not$stopAttempted){
  $stopAttempted=$true
  try{
   Note ('STOP_REQUEST elapsed_ms='+$clock.ElapsedMilliseconds+'; only own ETW session, NOT StopService')
   if(Test-Path -LiteralPath ($diagRoot+'\handles.etl')){throw 'ETL_EXISTS_NO_OVERWRITE_SESSION_UNKNOWN'}
   $stopped=WprQuery @('-stop',($diagRoot+'\handles.etl'),'PACC WPRH-20261023-03 read-only handles','-skipPdbGen','-instancename',$instance)
   Note ('WPR_STOP_EXIT='+$stopped.code+' elapsed_ms='+$clock.ElapsedMilliseconds)
   if($stopped.code-ne0){throw 'WPR_STOP_UNKNOWN_NO_RETRY'}
   $end=WprQuery @('-status','-instancename',$instance)
   if($end.text-notmatch'WPR is not recording'){throw 'WPR_FINAL_DISPOSITION_UNKNOWN'}
   $etl=Get-Item -LiteralPath ($diagRoot+'\handles.etl');$etlHash=Get-FileHash -LiteralPath $etl.FullName -Algorithm SHA256
   Note ('ETL='+$etl.FullName+' BYTES='+$etl.Length+' SHA256='+$etlHash.Hash)
  }catch{if($null-eq$firstError){$firstError=$_.Exception.Message};Note ('TRACE_DISPOSITION_ERROR='+$_.Exception.Message+'; do not retry/cancel/kill')}
 }
 if($null-ne$subject){
  if(-not$subject.HasExited){Note 'TRACE STOP REQUEST FINISHED. Child may await F10 or retain references. Do not close/kill it. F10 only after factual qualifier return. Supervisor keeps process reference.';$subject.WaitForExit()}
  Note ('PREPARE_LAUNCHER_EXIT='+$subject.ExitCode);$subject.Dispose()
 }
 Note ('FINAL FIRST_ERROR='+$firstError+'; START_SERVICE=NOT_RUN; ED11=PARTIAL; AUTHORITY=DENY; RETRY=DENY')
 if($null-ne$log){$log.Dispose()}
}
if($null-ne$firstError){exit 2}
exit 0
