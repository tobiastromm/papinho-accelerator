#requires -Version 5.1
# Manual engineering input placement. NOT an Accelerator admission/preflight launcher.
[CmdletBinding()]
param([ValidateSet('ModelTest','VerifyRoot','Place')][string]$Mode='ModelTest',
 [Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedScriptSha256)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$root='C:\Projetos\PapinhoAccelerator'
$tools=$root+'\tools\qualification'
function Hash([string]$path){
 $file=[IO.File]::Open($path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 $sha=[Security.Cryptography.SHA256]::Create()
 try{return ([BitConverter]::ToString($sha.ComputeHash($file))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose();$file.Dispose()}
}
if((Hash ($tools+'\n0start03-raw1-input-placement.ps1')) -cne $ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
if([IntPtr]::Size-ne8 -or $PSVersionTable.PSEdition-ne'Desktop' -or $PSVersionTable.PSVersion.Major-ne5){throw 'WINDOWS_POWERSHELL_5_X64_REQUIRED'}
Import-Module ($PSHOME+'\Modules\Microsoft.PowerShell.Utility\Microsoft.PowerShell.Utility.psd1') -ErrorAction Stop
$pins=@{
 'scm-file-acl-native.ps1'='f52242fe74932d1539e83a742430ea523eda746ce39d8815b1b977cf6bb6ac75';
 'n0start03-raw1-input-native.cs'='f6c4f7b5403d71e1e63a5d41483712a66e0dd83ef6df4ec4bb3888d044bd6db9';
 'n0-raw1-input-model.cs'='cf36dd132b71da337ec260de2739bafda993fe3da827430ed28c20da19c6ed00'
}
$sourceBytes=@{}
foreach($entry in $pins.GetEnumerator()){
 $f=[IO.File]::Open(($tools+'\'+$entry.Key),[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 try{$m=[IO.MemoryStream]::new();try{$f.CopyTo($m);$b=$m.ToArray()}finally{$m.Dispose()}}finally{$f.Dispose()}
 $sha=[Security.Cryptography.SHA256]::Create();try{$actual=([BitConverter]::ToString($sha.ComputeHash($b))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
 if($actual-cne$entry.Value){throw ('SOURCE_PIN_DENY '+$entry.Key)};$sourceBytes[$entry.Key]=$b
}
if(('PaccFileAcl' -as [type]) -or ('PaccN2PlacementNative' -as [type])){throw 'ALREADY_LOADED_NO_REENTRY'}
# Compile the existing native ACL helper together with a bounded placement adapter.
# No code is evaluated from ordinary unpinned inputs.
$helper=[Text.UTF8Encoding]::new($false,$true).GetString($sourceBytes['scm-file-acl-native.ps1']).TrimStart([char]0xFEFF)
$match=[regex]::Match($helper,'(?s)Add-Type -TypeDefinition @"\r?\n(.*)\r?\n"@')
if(-not $match.Success){throw 'HELPER_SOURCE_FORMAT'}
$code=$match.Groups[1].Value+"`n"+[Text.UTF8Encoding]::new($false,$true).GetString($sourceBytes['n0start03-raw1-input-native.cs'])
if($Mode-eq'ModelTest'){$code+="`n"+[Text.UTF8Encoding]::new($false,$true).GetString($sourceBytes['n0-raw1-input-model.cs'])}
$usings=@([regex]::Matches($code,'using [A-Za-z0-9_.]+;') | ForEach-Object{$_.Value} | Sort-Object -Unique)
$code=($usings -join "`n")+"`n"+[regex]::Replace($code,'using [A-Za-z0-9_.]+;','')
$parameters=[CodeDom.Compiler.CompilerParameters]::new()
$parameters.GenerateInMemory=$true
$parameters.WarningLevel=4
$parameters.TreatWarningsAsErrors=$true
[void]$parameters.ReferencedAssemblies.Add('System.dll')
[void]$parameters.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition $code -CompilerParameters $parameters
$descriptor=$root+'\docs\integration\n0-n9-protected-input-placement-20261024\proposed-new-file-creation.raw'
if((Hash $descriptor)-cne'0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814'){throw 'DESCRIPTOR_PIN_DENY'}
if($Mode-eq'ModelTest'){
 [PaccN2PlacementModelTests]::Run([IO.File]::ReadAllBytes($descriptor))
 return
}
# Place is prepared only. Must not be invoked before separate exact-operation approval.
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
if($Mode-eq'VerifyRoot'){
 $query=[PaccN2PlacementNative]::new();$result=$query.VerifyRootReadOnly()
 foreach($line in $query.Evidence){Write-Host $line}
 Write-Host ('ROOT_READONLY_FIRST_ERROR='+$result+'; RETAINED='+$query.Retained+'; FILES_PLACED=NO')
 if($query.Retained){$script:RetainedRootQuery=$query;while($true){[Threading.Thread]::Sleep(1000)}}
 $global:LASTEXITCODE=2;if($result-eq'NONE'){$global:LASTEXITCODE=0};return
}
Write-Host 'N2P-20261024-03; ATTEMPT=0b1de869e11493b7cb08fb329578f6ce; BOOT=236441'
Write-Host 'Create TWO NEW protected files only, 7573/397 bytes. Parent and existing files unchanged.'
Write-Host 'No Start/Stop/SCM/ACL remediation/admission pins/intent. No overwrite/delete/retry.'
Write-Host 'Exact descriptor SHA256=0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814'
Write-Host 'Escape requests Stop between synchronous native calls. Abrupt host loss can leave UNKNOWN.'
Write-Host 'Run only after separate exact-operation owner approval. This prompt grants no authority.'
$expected='PLACE_N2_INPUTS N2P-20261024-03 0b1de869e11493b7cb08fb329578f6ce'
$confirmation=Read-Host ('Type exactly: '+$expected)
if($confirmation-cne$expected){throw 'CONFIRMATION_DENY_NO_EFFECT'}
Check-OperatorBoot
$receipt=$root+'\docs\integration\n0start03-protected-input-placement-preparation-20261024\manual-N2P-20261024-03.txt'
# Exclusive administrative evidence file. Not a registry or operational intent.
$stream=[IO.File]::Open($receipt,[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::Read)
$writer=[IO.StreamWriter]::new($stream,[Text.UTF8Encoding]::new($false))
$owner=[PaccN2PlacementNative]::new();$first='NONE';$closed=$false;$receiptRetained=$false
try {
 $writer.WriteLine('N2P-20261024-03; SCRIPT='+$ExpectedScriptSha256+'; CONFIRMED; OPERATIONAL_AUTHORITY=NONE');$writer.Flush()
 try{$owner.Prepare()}catch{$first=[PaccN2PlacementSteps]::Describe($_.Exception)}
 if($first-eq'NONE'){$first=[PaccN2PlacementSteps]::Run($owner);$closed=-not $owner.Retained}
 else{$closed=$owner.Close()}
 foreach($line in $owner.Evidence){Write-Host $line;$writer.WriteLine($line)}
 $final='NATIVE_FIRST_ERROR='+$first+'; NATIVE_OWNED_CLOSED='+$closed+'; RETAINED='+$owner.Retained+'; ADMIN_RECEIPT_CLOSE=PENDING; PINS=EMPTY_DENY; ED11=PARTIAL; NO_RETRY'
 Write-Host $final;$writer.WriteLine($final);$writer.Flush()
} catch {
 if($first-eq'NONE'){$first=[PaccN2PlacementSteps]::Describe($_.Exception)}
 if(-not $closed -and -not $owner.Retained){$closed=$owner.Close()}
 Write-Host ('FIRST_ERROR='+$first+'; RECEIPT_OR_WORKFLOW_FAILED; NO_RETRY')
} finally {
 try{$writer.Dispose()}catch{$receiptRetained=$true;if($first-eq'NONE'){$first='ADMINISTRATIVE_RECEIPT_CLOSE_UNKNOWN'};Write-Host 'ADMINISTRATIVE_RECEIPT_CLOSE_UNKNOWN; PRESERVE_EVIDENCE'}
}
if($owner.Retained -or $receiptRetained){
 $script:RetainedPlacement=$owner;$script:RetainedAdministrativeWriter=$writer
 Write-Host 'RETAINED_UNKNOWN: original references remain in this process. Do not close/retry/kill.'
 while($true){[Threading.Thread]::Sleep(1000)}
}
Write-Host 'Native placement routine returned; own reference closure checked. No automatic retry.'
Write-Host ('FIRST_ERROR='+$first+'; ADMIN_RECEIPT_CLOSE=CONFIRMED')
$global:LASTEXITCODE=2
if($first-eq'NONE' -and $closed){$global:LASTEXITCODE=0}
Write-Host ('N2_PLACEMENT_EXIT='+$global:LASTEXITCODE)
# Returns to this existing PowerShell. No F10/second shell/process exit handler.
