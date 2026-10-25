$ErrorActionPreference='Stop'
Add-Type -TypeDefinition @"
using System;
using System.Runtime.InteropServices;
public static class CapacitySyntheticEvents {
 [DllImport("kernel32.dll",SetLastError=true)] public static extern IntPtr CreateEventW(IntPtr sa,bool manual,bool initial,string name);
 [DllImport("kernel32.dll",SetLastError=true)] public static extern bool SetHandleInformation(IntPtr h,uint mask,uint flags);
 [DllImport("kernel32.dll",SetLastError=true)] public static extern bool CloseHandle(IntPtr h);
}
"@
$root='C:\Projetos\PapinhoAccelerator'
$out=$root+'\docs\integration\n0start03-integrated-closeout-20261024\positive-capacity-resolution'
$results=@()
foreach($config in @('Debug','Release')) {
 $exe=$root+'\build\ed11-actual-observer-20261024\Selected\'+$config+'\papacc_service_observer_stack_va_isolated_test.exe'
 foreach($case in @('baseline','noninherit8','inherit8','baseline_after')) {
  $events=@();$child=$null;$reader=$null
  try {
   if($case -eq 'noninherit8' -or $case -eq 'inherit8') {
    for($i=0;$i -lt 8;$i++) {
     $h=[CapacitySyntheticEvents]::CreateEventW([IntPtr]::Zero,$true,$false,$null)
     if($h -eq [IntPtr]::Zero){throw 'CREATE_SYNTHETIC_EVENT_FAILED'}
     $events+=,$h
     $flag=0;if($case -eq 'inherit8'){$flag=1}
     if(-not [CapacitySyntheticEvents]::SetHandleInformation($h,1,$flag)){throw 'SET_SYNTHETIC_FLAG_FAILED'}
    }
   }
   $si=[Diagnostics.ProcessStartInfo]::new();$si.FileName=$exe;$si.Arguments='--isolated-observer-self clean';$si.UseShellExecute=$false;$si.CreateNoWindow=$false;$si.RedirectStandardOutput=$true;$si.RedirectStandardInput=$false;$si.RedirectStandardError=$false
   $child=[Diagnostics.Process]::new();$child.StartInfo=$si
   if(-not $child.Start()){throw 'FIXTURE_START_FAILED'}
   $reader=$child.StandardOutput;$text=$reader.ReadToEnd();$child.WaitForExit();$exit=$child.ExitCode
   [IO.File]::WriteAllText($out+'\inheritance-'+$config+'-'+$case+'.log',$text,[Text.UTF8Encoding]::new($false))
   if($exit -ne 0){throw ('ISOLATED_FIXTURE_FAILED '+$exit)}
   $m=[regex]::Match($text,'OBSERVER_FIXTURE_PASS mode=clean before=(\d+) after=(\d+) sampled_peak=(\d+)')
   if(-not $m.Success){throw 'MISSING_FIXTURE_FINAL'}
   $results+=@{config=$config;case=$case;before=[int]$m.Groups[1].Value;after=[int]$m.Groups[2].Value;sampled_peak=[int]$m.Groups[3].Value;exit=$exit;synthetic_events=$events.Count}
  } finally {
   if($reader){$reader.Dispose()};if($child){$child.Dispose()}
   foreach($h in $events){if(-not [CapacitySyntheticEvents]::CloseHandle($h)){throw 'SYNTHETIC_CLOSE_UNKNOWN_NO_RETRY'}}
  }
 }
}
$results | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath ($out+'\inheritance-results.json') -Encoding UTF8
$results | ForEach-Object { [pscustomobject]$_ } | Format-Table config,case,before,after,sampled_peak,exit
