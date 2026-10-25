$ErrorActionPreference='Stop'
Add-Type -TypeDefinition @"
using System;
using System.Runtime.InteropServices;
public static class AttributionSyntheticEvents {
 [DllImport("kernel32.dll",SetLastError=true)] public static extern IntPtr CreateEventW(IntPtr sa,bool manual,bool initial,string name);
 [DllImport("kernel32.dll",SetLastError=true)] public static extern bool SetHandleInformation(IntPtr h,uint mask,uint flags);
 [DllImport("kernel32.dll",SetLastError=true)] public static extern bool CloseHandle(IntPtr h);
 [DllImport("kernel32.dll",SetLastError=true)] public static extern uint WaitForSingleObject(IntPtr h,uint timeout);
}
"@
$root='C:\Projetos\PapinhoAccelerator'
$out=$root+'\docs\integration\n0start03-integrated-closeout-20261024\handle-attribution'
$results=@()
foreach($config in @('Debug','Release')) {
 $exe=$root+'\build\ed11-actual-observer-20261024\Selected\'+$config+'\papacc_service_observer_stack_va_isolated_test.exe'
 foreach($case in @('baseline','noninherit8','inherit8','baseline_after')) {
  $events=@();$child=$null;$reader=$null
  try {
   if($case -eq 'noninherit8' -or $case -eq 'inherit8') {
    for($i=0;$i -lt 8;$i++) {
     $h=[AttributionSyntheticEvents]::CreateEventW([IntPtr]::Zero,$true,$false,$null)
     if($h -eq [IntPtr]::Zero){throw 'CREATE_SYNTHETIC_EVENT_FAILED'}
     $events+=,$h
     $flag=0;if($case -eq 'inherit8'){$flag=1}
     if(-not [AttributionSyntheticEvents]::SetHandleInformation($h,1,$flag)){throw 'SET_SYNTHETIC_FLAG_FAILED'}
    }
   }
   $si=[Diagnostics.ProcessStartInfo]::new();$si.FileName=$exe;$si.Arguments='--isolated-observer-self handle-map';$si.UseShellExecute=$false;$si.CreateNoWindow=$false;$si.RedirectStandardOutput=$true;$si.RedirectStandardInput=$false;$si.RedirectStandardError=$false
   if($case -eq 'inherit8'){$si.EnvironmentVariables['PACC_FIXTURE_INHERITED_EVENTS']=(($events | ForEach-Object { $_.ToInt64().ToString() }) -join ',')}else{$si.EnvironmentVariables.Remove('PACC_FIXTURE_INHERITED_EVENTS')}
   $child=[Diagnostics.Process]::new();$child.StartInfo=$si
   if(-not $child.Start()){throw 'FIXTURE_START_FAILED'}
   $reader=$child.StandardOutput;$text=$reader.ReadToEnd();$child.WaitForExit();$exit=$child.ExitCode
   [IO.File]::WriteAllText($out+'\inheritance-'+$config+'-'+$case+'.log',$text,[Text.UTF8Encoding]::new($false))
   if($exit -ne 0){throw ('ISOLATED_FIXTURE_FAILED '+$exit)}
   if($text -notmatch 'TABLE_FIXTURE_PASS'){throw 'MISSING_TABLE_FIXTURE_FINAL'}
   $waits=@();foreach($h in $events){$wait=[AttributionSyntheticEvents]::WaitForSingleObject($h,0);$waits+=,$wait;if($case -eq 'inherit8'){if($wait -ne 0){throw 'SAME_OBJECT_SIGNAL_NOT_PROVEN'}}elseif($wait -ne 258){throw 'NONINHERIT_EVENT_UNEXPECTED_SIGNAL'}}
   $results+=@{config=$config;case=$case;exit=$exit;synthetic_events=$events.Count;parent_waits=$waits;child_pid=$child.Id;child_creation=$child.StartTime.ToUniversalTime().ToFileTimeUtc();object_proof='Parent retained original unsignaled event; child SetEvent; original parent WaitForSingleObject signaled only in inherit8'}
  } finally {
   if($reader){$reader.Dispose()};if($child){$child.Dispose()}
   foreach($h in $events){if(-not [AttributionSyntheticEvents]::CloseHandle($h)){throw 'SYNTHETIC_CLOSE_UNKNOWN_NO_RETRY'}}
  }
 }
}
$results | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath ($out+'\inheritance-results.json') -Encoding UTF8
$results | ForEach-Object { [pscustomobject]$_ } | Format-Table config,case,synthetic_events,child_pid,exit
