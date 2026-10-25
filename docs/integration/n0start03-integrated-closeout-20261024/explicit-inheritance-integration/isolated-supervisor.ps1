param([string]$Configuration='Release',[string]$Strategy='limited',[int]$Fault=0,[string]$Child='handle-map',[string]$Probe='none')
$ErrorActionPreference='Stop'
$root=(Get-Location).Path
$bin=Join-Path $root "build/ed11-inheritance-integration-20261025/Selected/$Configuration"
$dll=Join-Path $bin 'papacc_explicit_inheritance_fixture.dll'
$exe=Join-Path $bin 'papacc_service_observer_stack_va_isolated_test.exe'
$pin1=[IO.File]::Open($dll,'Open','Read','Read');$pin2=[IO.File]::Open($exe,'Open','Read','Read')
Add-Type @"
using System;using System.Runtime.InteropServices;
public static class NX {
[DllImport(@"$dll")] public static extern uint NX_Bytes();
[DllImport(@"$dll")] public static extern uint NX_Init(IntPtr c,uint b,uint f);
[DllImport(@"$dll")] public static extern ulong NX_Info(IntPtr c,uint k);
[DllImport(@"$dll",CharSet=CharSet.Unicode)] public static extern uint NX_Start(IntPtr c,string e,string a,IntPtr i,IntPtr x);
[DllImport(@"$dll")] public static extern IntPtr NX_BorrowReader(IntPtr c,out uint t);
[DllImport(@"$dll")] public static extern uint NX_EndBorrow(IntPtr c,uint t,uint e);
[DllImport(@"$dll")] public static extern uint NX_Wait(IntPtr c,uint t);
[DllImport(@"$dll")] public static extern uint NX_Close(IntPtr c);
[DllImport("kernel32",SetLastError=true)] public static extern IntPtr CreateEventW(IntPtr a,bool m,bool i,IntPtr n);
[DllImport("kernel32",SetLastError=true)] public static extern bool SetHandleInformation(IntPtr h,uint m,uint f);
[DllImport("kernel32",SetLastError=true)] public static extern bool CloseHandle(IntPtr h);
[DllImport("kernel32")] public static extern IntPtr GetStdHandle(int x);
}
"@
function Sample([string]$phase) {$v=[Diagnostics.Process]::GetCurrentProcess();try {[Console]::Out.WriteLine("PARENT_SAMPLE phase=$phase handles=$($v.HandleCount) private=$($v.PrivateMemorySize64) threads=$($v.Threads.Count); PARENT_TABLE_ONLY")} finally {$v.Dispose()}}
$events=@();$parent=[Diagnostics.Process]::GetCurrentProcess()
try {
 for($i=0;$i -lt 8;$i++) {$h=[NX]::CreateEventW([IntPtr]::Zero,$true,$false,[IntPtr]::Zero);if($h -eq [IntPtr]::Zero){throw 'event'};$events+= $h;if(![NX]::SetHandleInformation($h,1,1)){throw 'inherit flag'}}
 $expected=if($Strategy -eq 'legacy'){8}else{0}
 $env:PACC_FIXTURE_PARENT_EVENTS=(@($PID,$parent.StartTime.ToFileTimeUtc(),$expected)+@($events|ForEach-Object {$_.ToInt64()})) -join ' '
 Remove-Item Env:PACC_FIXTURE_INHERITED_EVENTS -ErrorAction SilentlyContinue
 Sample 'BEFORE_START'
 $argsText="--isolated-observer-self $Child"
 if($Strategy -eq 'legacy') {
  $si=New-Object Diagnostics.ProcessStartInfo;$si.FileName=$exe;$si.Arguments=$argsText;$si.UseShellExecute=$false;$si.CreateNoWindow=$false;$si.RedirectStandardOutput=$true
  $process=New-Object Diagnostics.Process;$process.StartInfo=$si
  if(!$process.Start()){throw 'start'};$text=$process.StandardOutput.ReadToEnd();$process.WaitForExit();$exit=$process.ExitCode;$process.Dispose();[Console]::Out.Write($text);if($exit){throw "child exit $exit"}
 } else {
  [Threading.Thread]::BeginThreadAffinity()
  $bytes=[NX]::NX_Bytes();$ctx=[Runtime.InteropServices.Marshal]::AllocHGlobal([int]$bytes);for($i=0;$i -lt $bytes;$i++){[Runtime.InteropServices.Marshal]::WriteByte($ctx,$i,0)}
  if([NX]::NX_Init($ctx,$bytes,$Fault)){throw 'init'}
  $rc=[NX]::NX_Start($ctx,$exe,$argsText,[NX]::GetStdHandle(-10),[NX]::GetStdHandle(-12))
  if($Probe -eq 'duplicate' -and !$rc){$n=[NX]::NX_Start($ctx,$exe,$argsText,[NX]::GetStdHandle(-10),[NX]::GetStdHandle(-12));if($n -ne 183){throw 'duplicate'}}
  Sample 'AFTER_NATIVE_START'
  if(!$rc){
   [uint32]$ticket=0;$h=[NX]::NX_BorrowReader($ctx,[ref]$ticket);if($h -eq [IntPtr]::Zero){throw 'borrow'}
   $safe=New-Object Microsoft.Win32.SafeHandles.SafeFileHandle($h,$false)
   $stream=New-Object IO.FileStream($safe,[IO.FileAccess]::Read)
   $reader=New-Object IO.StreamReader($stream,[Text.Encoding]::ASCII)
   $text=$reader.ReadToEnd();$reader.Dispose();$safe.Dispose();[Console]::Out.Write($text)
   if($Probe -eq 'badlease'){$ticket++}
   $ioerror=if($Probe -eq 'write-error'){29}else{0}
   $end=[NX]::NX_EndBorrow($ctx,$ticket,$ioerror)
   if($end -and $Probe -eq 'none'){throw 'end borrow'}
  }
  if([NX]::NX_Info($ctx,3)){if([NX]::NX_Wait($ctx,30000)){throw 'wait'}}
  $before=[NX]::NX_Info($ctx,6);$frozen=[NX]::NX_Info($ctx,1)
  if($frozen){$again=[NX]::NX_Start($ctx,$exe,$argsText,[NX]::GetStdHandle(-10),[NX]::GetStdHandle(-12));if(!$again -or [NX]::NX_Info($ctx,6) -ne $before){throw 'freeze violated'}}
  $closed=[NX]::NX_Close($ctx)
  $row=@{fault=$Fault;start=$rc;close=$closed;first=[NX]::NX_Info($ctx,0);frozen=$frozen;owned=[NX]::NX_Info($ctx,2);creates=[NX]::NX_Info($ctx,8);peak_owned=[NX]::NX_Info($ctx,9);state_bytes=$bytes;attribute_peak_bytes=[NX]::NX_Info($ctx,15);child_exit=[NX]::NX_Info($ctx,13)}
  [Console]::Out.WriteLine('NX_RESULT '+($row|ConvertTo-Json -Compress))
  if(!$frozen){if($closed -or $row.owned){throw 'cleanup'};[Runtime.InteropServices.Marshal]::FreeHGlobal($ctx)}
  [Threading.Thread]::EndThreadAffinity()
  if($Fault -eq 0 -and ($rc -or $row.child_exit)){throw 'native path'}
  if($frozen){[Console]::Out.WriteLine('UNKNOWN_RETAINED_NO_RETRY; ISOLATED_PROCESS_EXIT_CONTAINMENT');exit 2}
 }
} finally {
 foreach($h in $events){if(![NX]::CloseHandle($h)){throw 'event close uncertain'}}
 Sample 'AFTER_EVENTS_CLOSED'
 $pin2.Dispose();$pin1.Dispose();$parent.Dispose()
}
