#requires -Version 5.1
[CmdletBinding()]
param([ValidateSet('CompileOnly','Model','NativeFixture','Verify')][string]$Mode='CompileOnly',[Parameter(Mandatory=$true)][string]$ExpectedScriptSha256,[string]$Confirmation='')
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$root='C:\Projetos\PapinhoAccelerator';$tools=$root+'\tools\qualification';$package=[IO.Path]::GetDirectoryName($PSCommandPath)
function Hash([string]$p){$f=[IO.File]::Open($p,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read);$s=[Security.Cryptography.SHA256]::Create();try{return ([BitConverter]::ToString($s.ComputeHash($f))).Replace('-','').ToLowerInvariant()}finally{$s.Dispose();$f.Dispose()}}
if((Hash $PSCommandPath)-cne$ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
if([IntPtr]::Size-ne8 -or $PSVersionTable.PSEdition-ne'Desktop' -or $PSVersionTable.PSVersion.Major-ne5){throw 'WINDOWS_POWERSHELL_5_X64_REQUIRED'}
Import-Module ($PSHOME+'\Modules\Microsoft.PowerShell.Utility\Microsoft.PowerShell.Utility.psd1') -ErrorAction Stop
if(('PaccFileAcl' -as [type]) -or ('PaccQ2Read' -as [type]) -or ('PaccN0InputRead' -as [type])){throw 'FRESH_OPERATOR_PROCESS_REQUIRED'}
$pins=@{'scm-file-acl-native.ps1'='f52242fe74932d1539e83a742430ea523eda746ce39d8815b1b977cf6bb6ac75';'q2-protected-readback.cs'='6fe1d262291f0343be4d316fcafa9c70898c676263d3a37d4ab4074d9ea5d117'}
$pins['successor-input-readonly.cs']='1ec26dc9bd04a468543836b42c574a8b872515b5f596398a1901613edf8b9cc0'
$bytes=@{}
foreach($entry in $pins.GetEnumerator()){
 $base=$tools;if($entry.Key-eq'q2-protected-readback.cs'){$base=$package};if($entry.Key-eq'successor-input-readonly.cs'){$base=$root+'\docs\integration\n0start03-protected-receipt-and-pins-20261024'}
 $f=[IO.File]::Open(($base+'\'+$entry.Key),[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 try{$m=[IO.MemoryStream]::new();try{$f.CopyTo($m);$b=$m.ToArray()}finally{$m.Dispose()}}finally{$f.Dispose()}
 $sha=[Security.Cryptography.SHA256]::Create();try{$actual=([BitConverter]::ToString($sha.ComputeHash($b))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
 if($actual-cne$entry.Value){throw 'SOURCE_PIN_DENY'};$bytes[$entry.Key]=$b
}
$helper=[Text.UTF8Encoding]::new($false,$true).GetString($bytes['scm-file-acl-native.ps1']).TrimStart([char]0xFEFF)
$match=[regex]::Match($helper,'(?s)Add-Type -TypeDefinition @"\r?\n(.*)\r?\n"@')
if(-not $match.Success){throw 'HELPER_SOURCE_FORMAT'}
$code=$match.Groups[1].Value+"`n"+[Text.UTF8Encoding]::new($false,$true).GetString($bytes['q2-protected-readback.cs'])+"`n"+[Text.UTF8Encoding]::new($false,$true).GetString($bytes['successor-input-readonly.cs'])
$usings=@([regex]::Matches($code,'using [A-Za-z0-9_.]+;')|ForEach-Object{$_.Value}|Sort-Object -Unique)
$code=($usings -join "`n")+"`n"+[regex]::Replace($code,'using [A-Za-z0-9_.]+;','')
$cp=[CodeDom.Compiler.CompilerParameters]::new();$cp.GenerateInMemory=$true;$cp.WarningLevel=4;$cp.TreatWarningsAsErrors=$true
[void]$cp.ReferencedAssemblies.Add('System.dll');[void]$cp.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition $code -CompilerParameters $cp
if($Mode-eq'CompileOnly'){'Q2_READBACK_COMPILE=PASS; NO_PROTECTED_READ_NO_PREFLIGHT_NO_CREATE';return}
if($Mode-eq'Model'){[PaccQ2Read]::Model();return}
if($Mode-eq'NativeFixture'){[PaccQ2Read]::NativeFixture();return}
function Check-OperatorBoot {
 Import-Module ($PSHOME+'\Modules\Microsoft.PowerShell.Diagnostics\Microsoft.PowerShell.Diagnostics.psd1') -ErrorAction Stop
 if([Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'INTERACTIVE_CONSOLE_REQUIRED'}
 $id=[Security.Principal.WindowsIdentity]::GetCurrent()
 try{$kind=0;$elevation=0;$used=0;if(-not [PaccFileAcl]::GetTokenInformation($id.Token,8,[ref]$kind,4,[ref]$used) -or $used-ne4 -or $kind-ne1){throw 'PRIMARY_TOKEN_DENY'};if(-not [PaccFileAcl]::GetTokenInformation($id.Token,20,[ref]$elevation,4,[ref]$used) -or $used-ne4 -or $elevation-ne1){throw 'TOKEN_ELEVATION_DENY'};if($id.User.Value-cne'S-1-5-21-785699550-1440458426-1682014700-1108' -or -not ([Security.Principal.WindowsPrincipal]::new($id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'ELEVATED_OPERATOR_DENY'}}finally{$id.Dispose()}
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
if($Confirmation-cne'READ_ONLY_Q2 N0START-20261024-03 0b1de869e11493b7cb08fb329578f6ce'){throw 'EXACT_READONLY_CONFIRMATION_DENY'}
Check-OperatorBoot
if((Hash ($root+'\docs\integration\n0start03-protected-input-placement-preparation-20261024\manual-N2P-20261024-03.txt'))-cne'92574a6f1966358d054daa50484c7bd855d8a2a4fb4d5ed42edca938d231a1e9'){throw 'N2P_RECEIPT_PIN_DENY'}
if((Hash ($root+'\docs\adr\DECISION-n0start03-poststart-disposition-and-one-token-observation-20261025.md'))-cne'97763395f521d0a8d15ee0afe22b61705897d6d97c2c1f8905ad9114e8b1b9ca'){throw 'Q_DECISION_PIN_DENY'}
$result=[PaccQ2Read]::Run()
if($result-eq'NONE'){$dossier=[PaccN0InputRead]::Run();if($dossier-cne'NONE'){$result='DOSSIER_'+$dossier}}
$retained=[PaccQ2Read]::Retained -or [PaccN0InputRead]::Retained
[Console]::Out.WriteLine('Q2_READBACK_FIRST_ERROR='+$result+'; RETAINED='+$retained+'; TRANSITION=NOT_APPLIED; NO_START_STOP_SCM_ACL_CREATE_RETRY')
if($retained){[Console]::Out.WriteLine('UNKNOWN_PRESERVE_OWNER_NO_RETRY');while($true){[Threading.Thread]::Sleep(1000)}}
$global:LASTEXITCODE=2;if($result-eq'NONE'){$global:LASTEXITCODE=0}
[Console]::Out.WriteLine('Q2_READBACK_EXIT='+$global:LASTEXITCODE+'; OBSERVATIONS_ONLY_REVIEW_REQUIRED; Q2_NOT_APPLIED; NO_ADMISSION')
