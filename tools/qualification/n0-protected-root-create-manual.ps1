#requires -Version 5.1
[CmdletBinding()]
param([ValidateSet('Review','Create')][string]$Mode='Review',
 [Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedScriptSha256)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$root='C:\Projetos\PapinhoAccelerator'
$scriptFile=$root+'\tools\qualification\n0-protected-root-create-manual.ps1'
$exe=$root+'\build\n0-protected-root-creator-20261024\Selected\Release\papacc_n0_root_creator.exe'
$exeHash='29226a2ff79f6bcc3f11374aba1383e3fd0e9b9850e4ae648e0d8f54fb233324'
$helper=$root+'\tools\qualification\scm-file-acl-native.ps1'
$helperHash='f52242fe74932d1539e83a742430ea523eda746ce39d8815b1b977cf6bb6ac75'
function BytesHash([byte[]]$b){$h=[Security.Cryptography.SHA256]::Create();try{return ([BitConverter]::ToString($h.ComputeHash($b))).Replace('-','').ToLowerInvariant()}finally{$h.Dispose()}}
function FileHash([string]$p){$f=[IO.File]::Open($p,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read);$h=[Security.Cryptography.SHA256]::Create();try{return ([BitConverter]::ToString($h.ComputeHash($f))).Replace('-','').ToLowerInvariant()}finally{$h.Dispose();$f.Dispose()}}
if([IntPtr]::Size-ne8 -or $PSVersionTable.PSEdition-ne'Desktop' -or $PSVersionTable.PSVersion.Major-ne5){throw 'WINDOWS_POWERSHELL_5_X64_REQUIRED'}
if((FileHash $scriptFile)-cne$ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
if((FileHash $exe)-cne$exeHash -or (FileHash $helper)-cne$helperHash){throw 'BINARY_OR_HELPER_PIN_DENY'}
if($Mode-eq'Review'){'N0_ROOT_SCRIPT_REVIEW=PASS; NO_NATIVE_CREATE_NO_PREFLIGHT';return}
Import-Module ($PSHOME+'\Modules\Microsoft.PowerShell.Utility\Microsoft.PowerShell.Utility.psd1') -ErrorAction Stop
Import-Module ($PSHOME+'\Modules\Microsoft.PowerShell.Management\Microsoft.PowerShell.Management.psd1') -ErrorAction Stop
Import-Module ($PSHOME+'\Modules\Microsoft.PowerShell.Diagnostics\Microsoft.PowerShell.Diagnostics.psd1') -ErrorAction Stop
if([Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'INTERACTIVE_OPERATOR_CONSOLE_REQUIRED'}
$id=[Security.Principal.WindowsIdentity]::GetCurrent()
try{if($id.User.Value-cne'S-1-5-21-785699550-1440458426-1682014700-1108' -or -not ([Security.Principal.WindowsPrincipal]::new($id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'ELEVATED_OPERATOR_DENY'}}finally{$id.Dispose()}
$thread=[Security.Principal.WindowsIdentity]::GetCurrent($true)
if($null-ne$thread){$thread.Dispose();throw 'IMPERSONATION_DENY'}
if([Environment]::GetFolderPath([Environment+SpecialFolder]::CommonApplicationData)-cne'C:\ProgramData'){throw 'KNOWN_FOLDER_DENY'}
$boot=Get-WinEvent -FilterHashtable @{LogName='System';ProviderName='Microsoft-Windows-Kernel-General';Id=12} -MaxEvents 1
$xml=[xml]$boot.ToXml();$node=$xml.SelectSingleNode('/*[local-name()="Event"]/*[local-name()="System"]/*[local-name()="Computer"]')
if($null-eq$node){throw 'BOOT_COMPUTER_UNKNOWN'}
$hostHash=BytesHash ([Text.Encoding]::UTF8.GetBytes($node.InnerText))
$start=@($xml.Event.EventData.Data|Where-Object{$_.Name-eq'StartTime'})[0].'#text'
if($hostHash-cne'2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff' -or $boot.RecordId-ne236441 -or [DateTimeOffset]::Parse($start).UtcDateTime-ne[DateTimeOffset]::Parse('2026-10-23T18:36:34.5000000Z').UtcDateTime){throw 'HOST_BOOT_DENY'}
if('PaccFileAcl' -as [type]){throw 'FRESH_OPERATOR_PROCESS_REQUIRED'}
$files=[Collections.Generic.List[IO.FileStream]]::new();$locks=[Collections.Generic.List[IntPtr]]::new()
$process=$null;$writer=$null;$first='NONE';$unknown=$false;$nativeExit=$null
try{
 # Load ONLY original held, hash-checked helper bytes, not a second named-path read.
 $f=[IO.File]::Open($helper,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read);$files.Add($f)
 $m=[IO.MemoryStream]::new();try{$f.CopyTo($m);$hb=$m.ToArray()}finally{$m.Dispose()}
 if((BytesHash $hb)-cne$helperHash){throw 'HELD_HELPER_HASH_DENY'}
 & ([scriptblock]::Create([Text.UTF8Encoding]::new($false,$true).GetString($hb).TrimStart([char]0xFEFF)))
 # Preserve the executable's entire named path against non-admin substitution.
 $chain=[Collections.Generic.Stack[string]]::new();$p=[IO.Path]::GetDirectoryName($exe)
 while($p){$chain.Push($p);$parent=[IO.Directory]::GetParent($p);if($null-eq$parent){$p=$null}else{$p=$parent.FullName}}
 while($chain.Count){$p=$chain.Pop();$h=[PaccFileAcl]::Open($p,$false,$true);$locks.Add($h);$item=Get-Item -LiteralPath $p -Force;if(-not $item.PSIsContainer -or ($item.Attributes-band[IO.FileAttributes]::ReparsePoint)-ne0){throw 'EXECUTABLE_ANCESTOR_REPARSE_DENY'}}
 $f=[IO.File]::Open($exe,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read);$files.Add($f)
 $h=[Security.Cryptography.SHA256]::Create();try{$actual=([BitConverter]::ToString($h.ComputeHash($f))).Replace('-','').ToLowerInvariant()}finally{$h.Dispose()}
 if($actual-cne$exeHash -or ((Get-Item -LiteralPath $exe -Force).Attributes-band[IO.FileAttributes]::ReparsePoint)-ne0){throw 'HELD_EXECUTABLE_PIN_DENY'}
 $policyPins=@{
 'DECISION-n0-protected-root-descriptor-text-errata-20261024.md'='00392057174d7f039b65c64fd877d73af15ec8967655696c1e396fb2519bec1b';
 'DECISION-n0-n9-security-descriptor-digest-profile-20261024.md'='8b042e2bbf6500aa90aa3407fcd3423f45d96cf6b16568ffa95b98f3d081c972';
 'DECISION-first-native-lab-diagnostic-boundary-20261024.md'='bec9d0c275d5ee8bd68de4d42fe173a08f124a9900c119e7a08d23757d9d7941'
 }
 foreach($entry in $policyPins.GetEnumerator()){
  $f=[IO.File]::Open(($root+'\docs\adr\'+$entry.Key),[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read);$files.Add($f)
  $h=[Security.Cryptography.SHA256]::Create();try{$actual=([BitConverter]::ToString($h.ComputeHash($f))).Replace('-','').ToLowerInvariant()}finally{$h.Dispose()}
  if($actual-cne$entry.Value){throw 'HELD_POLICY_PIN_DENY'}
 }
 $receipt=$root+'\docs\integration\n0-protected-root-creator-implementation-20261024\manual-N0R-20261024-01.txt'
 $f=[IO.File]::Open($receipt,[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::Read)
 $writer=[IO.StreamWriter]::new($f,[Text.UTF8Encoding]::new($false));$writer.AutoFlush=$true
 $writer.WriteLine('N0R-20261024-01; SCRIPT='+$ExpectedScriptSha256+'; CREATOR='+$exeHash+'; HOST_BOOT_VERIFIED; OPERATIONAL_ADMISSION=NONE')
 Write-Host 'N0R-20261024-01: Three directories only, no inputs/SCM/preflight. Native confirmation follows. Escape requests Stop. No retry.'
 $psi=[Diagnostics.ProcessStartInfo]::new();$psi.FileName=$exe;$psi.Arguments='--create';$psi.UseShellExecute=$false;$psi.CreateNoWindow=$false;$psi.RedirectStandardOutput=$true
 $process=[Diagnostics.Process]::new();$process.StartInfo=$psi
 if(-not $process.Start()){throw 'NATIVE_PROCESS_START_FAILED'}
 # Native stdin remains this console. This relay has no confirmation authority.
 while(-not $process.StandardOutput.EndOfStream){$line=$process.StandardOutput.ReadLine();Write-Host $line;$writer.WriteLine($line)}
 $process.WaitForExit();$nativeExit=$process.ExitCode;$writer.WriteLine('NATIVE_EXIT='+$nativeExit)
}catch{
 $first=$_.Exception.Message;Write-Host ('ROOT_LAUNCHER_FIRST_ERROR='+$first)
 # Broken receipt relay must not leave an output-unaware creator proceeding.
 # Closing the pipe makes subsequent checked native stdout writes fail; never kill.
 if($null-ne$process){try{if(-not $process.HasExited){$process.StandardOutput.Dispose();Write-Host 'Do not confirm after relay failure. Preserve this console; no retry.';while(-not $process.WaitForExit(1000)){};$nativeExit=$process.ExitCode}}catch{$unknown=$true}}
}finally{
 if($null-ne$process){try{if(-not $process.HasExited){$unknown=$true}}catch{$unknown=$true}}
 if(-not $unknown){
  if($null-ne$writer){try{$writer.Dispose()}catch{$unknown=$true}}
  if(-not $unknown){foreach($f in $files){try{$f.Dispose()}catch{$unknown=$true;break}}}
  if(-not $unknown){for($i=$locks.Count-1;$i-ge0;$i--){if(-not [PaccFileAcl]::CloseHandle($locks[$i])){$unknown=$true;break};$locks[$i]=[IntPtr]::Zero}}
 }
}
if($unknown){$script:RetainedCreator=$process;$script:RetainedLocks=$locks;$script:RetainedFiles=$files;$script:RetainedReceipt=$writer;Write-Host 'ROOT_LAUNCHER_RETAINED_UNKNOWN; DO_NOT_CLOSE_KILL_OR_RETRY';while($true){[Threading.Thread]::Sleep(1000)}}
$global:LASTEXITCODE=2
if($first-eq'NONE' -and $nativeExit-eq0){$global:LASTEXITCODE=0}
Write-Host ('N0_ROOT_LAUNCHER_EXIT='+$global:LASTEXITCODE+'; ORIGINAL_PROCESS_EXIT_CONFIRMED; PINS=EMPTY_DENY; ED11=PARTIAL; NO_RETRY')
