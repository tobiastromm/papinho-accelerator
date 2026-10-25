#requires -Version 5.1
[CmdletBinding()]
param([ValidateSet('CompileOnly','Verify')][string]$Mode='CompileOnly',[Parameter(Mandatory=$true)][string]$ExpectedScriptSha256)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$root='C:\Projetos\PapinhoAccelerator';$tools=$root+'\tools\qualification'
function Hash([string]$p){$f=[IO.File]::Open($p,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read);$s=[Security.Cryptography.SHA256]::Create();try{return ([BitConverter]::ToString($s.ComputeHash($f))).Replace('-','').ToLowerInvariant()}finally{$s.Dispose();$f.Dispose()}}
if((Hash ($tools+'\n0-protected-input-readonly.ps1'))-cne$ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
if([IntPtr]::Size-ne8 -or $PSVersionTable.PSEdition-ne'Desktop' -or $PSVersionTable.PSVersion.Major-ne5){throw 'WINDOWS_POWERSHELL_5_X64_REQUIRED'}
Import-Module ($PSHOME+'\Modules\Microsoft.PowerShell.Utility\Microsoft.PowerShell.Utility.psd1') -ErrorAction Stop
if(('PaccFileAcl' -as [type]) -or ('PaccN0InputRead' -as [type])){throw 'FRESH_OPERATOR_PROCESS_REQUIRED'}
$pins=@{'scm-file-acl-native.ps1'='f52242fe74932d1539e83a742430ea523eda746ce39d8815b1b977cf6bb6ac75';'n0-protected-input-readonly.cs'='cd3678f0a593c1c9e5de2c81172d9d6534c9655379c3070bc81fac181e07aec3'}
$bytes=@{}
foreach($entry in $pins.GetEnumerator()){
 $f=[IO.File]::Open(($tools+'\'+$entry.Key),[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 try{$m=[IO.MemoryStream]::new();try{$f.CopyTo($m);$b=$m.ToArray()}finally{$m.Dispose()}}finally{$f.Dispose()}
 $sha=[Security.Cryptography.SHA256]::Create();try{$actual=([BitConverter]::ToString($sha.ComputeHash($b))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
 if($actual-cne$entry.Value){throw 'SOURCE_PIN_DENY'};$bytes[$entry.Key]=$b
}
$helper=[Text.UTF8Encoding]::new($false,$true).GetString($bytes['scm-file-acl-native.ps1']).TrimStart([char]0xFEFF)
$match=[regex]::Match($helper,'(?s)Add-Type -TypeDefinition @"\r?\n(.*)\r?\n"@')
if(-not $match.Success){throw 'HELPER_SOURCE_FORMAT'}
$code=$match.Groups[1].Value+"`n"+[Text.UTF8Encoding]::new($false,$true).GetString($bytes['n0-protected-input-readonly.cs'])
$usings=@([regex]::Matches($code,'using [A-Za-z0-9_.]+;')|ForEach-Object{$_.Value}|Sort-Object -Unique)
$code=($usings -join "`n")+"`n"+[regex]::Replace($code,'using [A-Za-z0-9_.]+;','')
$cp=[CodeDom.Compiler.CompilerParameters]::new();$cp.GenerateInMemory=$true;$cp.WarningLevel=4;$cp.TreatWarningsAsErrors=$true
[void]$cp.ReferencedAssemblies.Add('System.dll');[void]$cp.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition $code -CompilerParameters $cp
if($Mode-eq'CompileOnly'){'N0_INPUT_READER_COMPILE=PASS; NO_PROTECTED_READ_NO_PREFLIGHT_NO_CREATE';return}
function Check-OperatorBoot {
 Import-Module ($PSHOME+'\Modules\Microsoft.PowerShell.Diagnostics\Microsoft.PowerShell.Diagnostics.psd1') -ErrorAction Stop
 if([Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'INTERACTIVE_CONSOLE_REQUIRED'}
 $id=[Security.Principal.WindowsIdentity]::GetCurrent()
 try{if($id.User.Value-cne'S-1-5-21-785699550-1440458426-1682014700-1108' -or -not ([Security.Principal.WindowsPrincipal]::new($id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'ELEVATED_OPERATOR_DENY'}}finally{$id.Dispose()}
 $thread=[Security.Principal.WindowsIdentity]::GetCurrent($true)
 if($null-ne$thread){$thread.Dispose();throw 'IMPERSONATION_DENY'}
 $boot=Get-WinEvent -FilterHashtable @{LogName='System';ProviderName='Microsoft-Windows-Kernel-General';Id=12} -MaxEvents 1
 $xml=[xml]$boot.ToXml()
 $node=$xml.SelectSingleNode('/*[local-name()="Event"]/*[local-name()="System"]/*[local-name()="Computer"]')
 if($null-eq$node){throw 'BOOT_COMPUTER_UNKNOWN'}
 $sha=[Security.Cryptography.SHA256]::Create()
 try{$hostHash=([BitConverter]::ToString($sha.ComputeHash([Text.Encoding]::UTF8.GetBytes($node.InnerText)))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
 $start=@($xml.Event.EventData.Data|Where-Object{$_.Name-eq'StartTime'})[0].'#text'
 if($hostHash-cne'2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff' -or $boot.RecordId-ne236441 -or [DateTimeOffset]::Parse($start).UtcDateTime-ne[DateTimeOffset]::Parse('2026-10-23T18:36:34.5000000Z').UtcDateTime){throw 'HOST_BOOT_DENY'}
}
Check-OperatorBoot
if((Hash ($root+'\docs\integration\n0-n9-protected-raw1-input-placement2-20261024\manual-N2P-20261024-02.txt'))-cne'94bbf1efc64cb5cc1b8851ee9461907c401cfd7ca59096e06cbe41ce34b03c38'){throw 'N2P_RECEIPT_PIN_DENY'}
$result=[PaccN0InputRead]::Run()
Write-Host ('N0_INPUT_READ_FIRST_ERROR='+$result+'; RETAINED='+[PaccN0InputRead]::Retained+'; FILES_CREATED=NO; PREFLIGHT=NO; PINS=EMPTY_DENY')
if([PaccN0InputRead]::Retained){Write-Host 'UNKNOWN_PRESERVE_OWNER_NO_RETRY';while($true){[Threading.Thread]::Sleep(1000)}}
$global:LASTEXITCODE=2;if($result-eq'NONE'){$global:LASTEXITCODE=0}
Write-Host ('N0_INPUT_READ_EXIT='+$global:LASTEXITCODE+'; OWN_REFERENCES_CLOSED; NO_RETRY')
