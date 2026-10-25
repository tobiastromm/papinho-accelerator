#requires -Version 5.1
# Manual engineering input placement. NOT an Accelerator admission/preflight launcher.
[CmdletBinding()]
param([ValidateSet('ModelTest','VerifyRoot','Place')][string]$Mode='ModelTest',
 [Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedScriptSha256,
 [ValidatePattern('^[0-9a-f]{64}$')][string]$ExactApprovalSha256,
 [ValidateSet('Debug','Release')][string]$FixtureConfig='Release')
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$script:Q4ApprovalStream=$null
$script:Q4N7Stream=$null
$root='C:\Projetos\PapinhoAccelerator'
$tools=$root+'\tools\qualification'
$pkg=$root+'\docs\integration\n0start03-integrated-closeout-20261024\q4-n7-protected-inputs-and-activation-20261025'
function Hash([string]$path){
 $file=[IO.File]::Open($path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 $sha=[Security.Cryptography.SHA256]::Create()
 try{return ([BitConverter]::ToString($sha.ComputeHash($file))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose();$file.Dispose()}
}
if((Hash ($pkg+'\q4-input-placement.CANDIDATE.ps1')) -cne $ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
if([IntPtr]::Size-ne8 -or $PSVersionTable.PSEdition-ne'Desktop' -or $PSVersionTable.PSVersion.Major-ne5){throw 'WINDOWS_POWERSHELL_5_X64_REQUIRED'}
Import-Module ($PSHOME+'\Modules\Microsoft.PowerShell.Utility\Microsoft.PowerShell.Utility.psd1') -ErrorAction Stop
$pins=@{
 'scm-file-acl-native.ps1'='f52242fe74932d1539e83a742430ea523eda746ce39d8815b1b977cf6bb6ac75';
 'q4-input-placement-native.CANDIDATE.cs'='360e12354c23044a3653be823ff2fb31d67efbbad49544cad4743f76cb6fd120';
 'n0-raw1-input-model.cs'='cf36dd132b71da337ec260de2739bafda993fe3da827430ed28c20da19c6ed00'
}

function Assert-Q4PlacementApproval($record){
 foreach($field in @('status','operation','attempt','script_sha256','evidence_sha256','record_sha256','creation_descriptor_sha256','N7_proposed_sha256','N7_decision_sha256')){if($record.$field-isnot[string]){throw 'PLACEMENT_APPROVAL_SCALAR_DENY'}}
 if($record.status-isnot[string]-or$record.status-cne'EXACT_MANUAL_Q4_INPUT_PLACEMENT_OWNER_APPROVED'-or$record.operation-cne'Q4P-20261025-01'-or$record.attempt-cne'10584f02e11f92c92b43cddf0d9d316f'){throw 'PLACEMENT_APPROVAL_IDENTITY_DENY'}
 if($record.manual_only-isnot[bool]-or$record.manual_only-ne$true-or$record.automatic_execution-isnot[bool]-or$record.automatic_execution-ne$false-or$record.operational_admission-isnot[bool]-or$record.operational_admission-ne$false){throw 'PLACEMENT_APPROVAL_SCOPE_DENY'}
 if($record.script_sha256-cne$ExpectedScriptSha256-or$record.evidence_sha256-cne'bb548b4f2a866d33445d39e2bb72e0093b60b04380cc4da6fd70c4855a0038fa'-or$record.record_sha256-cne'c63209c7591f404eead7ca8e3e4b91b5b3c8db13e47ad0e637c6e2c3a36a46e6'-or$record.creation_descriptor_sha256-cne'0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814'){throw 'PLACEMENT_APPROVAL_PIN_DENY'}
 if($record.N7_proposed_sha256-cne'a5bcb78615b059d8f3c9508641442d20663a204c8ad19563c03953c87b4d3aba'-or$record.N7_decision_sha256-isnot[string]-or$record.N7_decision_sha256-cnotmatch'^[0-9a-f]{64}$'){throw 'SPECIFIC_N7_MISSING_DENY'}
}
if($Mode-cne'ModelTest'){
 if(-not$ExactApprovalSha256){throw 'EXACT_MANUAL_PLACEMENT_APPROVAL_REQUIRED_NO_EFFECT'}
 $script:Q4ApprovalStream=[IO.File]::Open(($pkg+'\EXACT-PROTECTED-PLACEMENT-APPROVED.json'),[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 if($script:Q4ApprovalStream.Length-le0-or$script:Q4ApprovalStream.Length-gt16384){throw 'APPROVAL_SIZE_DENY'}
 $bytes=[byte[]]::new([int]$script:Q4ApprovalStream.Length);if($script:Q4ApprovalStream.Read($bytes,0,$bytes.Length)-ne$bytes.Length){throw 'APPROVAL_READ_DENY'}
 $sha=[Security.Cryptography.SHA256]::Create();try{$actual=([BitConverter]::ToString($sha.ComputeHash($bytes))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
 if($actual-cne$ExactApprovalSha256){throw 'APPROVAL_SHA_DENY'}
 $approved=([Text.UTF8Encoding]::new($false,$true).GetString($bytes))|ConvertFrom-Json
 Assert-Q4PlacementApproval $approved
 $N7Path=$root+'\docs\adr\DECISION-q4-specific-n7-and-independent-evaluation-20261025.md'
 $script:Q4N7Stream=[IO.File]::Open($N7Path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 if($script:Q4N7Stream.Length-le0-or$script:Q4N7Stream.Length-gt65536){throw 'N7_DECISION_SIZE_DENY'}
 $N7Bytes=[byte[]]::new([int]$script:Q4N7Stream.Length);if($script:Q4N7Stream.Read($N7Bytes,0,$N7Bytes.Length)-ne$N7Bytes.Length){throw 'N7_DECISION_READ_DENY'}
 $sha=[Security.Cryptography.SHA256]::Create();try{$N7Hash=([BitConverter]::ToString($sha.ComputeHash($N7Bytes))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
 if($N7Hash-cne$approved.N7_decision_sha256){throw 'ACCEPTED_N7_DECISION_PIN_DENY'}
 $N7Text=[Text.UTF8Encoding]::new($false,$true).GetString($N7Bytes)
 if($N7Text-cnotmatch'(?m)^status: accepted\r?$'-or-not$N7Text.Contains('10584f02e11f92c92b43cddf0d9d316f')-or-not$N7Text.Contains('a5bcb78615b059d8f3c9508641442d20663a204c8ad19563c03953c87b4d3aba')){throw 'ACCEPTED_SPECIFIC_N7_REQUIRED'}
}

$sourceBytes=@{}
foreach($entry in $pins.GetEnumerator()){
 $directory=$tools;if($entry.Key-ceq'q4-input-placement-native.CANDIDATE.cs'){$directory=$pkg}
 $f=[IO.File]::Open(($directory+'\'+$entry.Key),[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
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
$code=$match.Groups[1].Value+"`n"+[Text.UTF8Encoding]::new($false,$true).GetString($sourceBytes['q4-input-placement-native.CANDIDATE.cs'])
if($Mode-eq'ModelTest'){$code+="`n"+[Text.UTF8Encoding]::new($false,$true).GetString($sourceBytes['n0-raw1-input-model.cs'])}
$usings=@([regex]::Matches($code,'using [A-Za-z0-9_.]+;') | ForEach-Object{$_.Value} | Sort-Object -Unique)
$code=($usings -join "`n")+"`n"+[regex]::Replace($code,'using [A-Za-z0-9_.]+;','')
$parameters=[CodeDom.Compiler.CompilerParameters]::new()
$parameters.GenerateInMemory=$true
$parameters.WarningLevel=4
$parameters.CompilerOptions=if($FixtureConfig-ceq'Debug'){'/debug+ /optimize-'}else{'/optimize+'}
$parameters.TreatWarningsAsErrors=$true
[void]$parameters.ReferencedAssemblies.Add('System.dll')
[void]$parameters.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition $code -CompilerParameters $parameters
$descriptor=$root+'\docs\integration\n0-n9-protected-input-placement-20261024\proposed-new-file-creation.raw'
if((Hash $descriptor)-cne'0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814'){throw 'DESCRIPTOR_PIN_DENY'}
if($Mode-eq'ModelTest'){
 [PaccN2PlacementModelTests]::Run([IO.File]::ReadAllBytes($descriptor))
 $absenceCases=@(
  @([uint32]3221225524,[uint32]3221225473,$false,'ABSENT'),
  @([uint32]3221225524,[uint32]259,$false,'PENDING_UNKNOWN'),
  @([uint32]259,[uint32]3221225473,$false,'PENDING_UNKNOWN'),
  @([uint32]0,[uint32]0,$true,'COLLISION'),
  @([uint32]3221225506,[uint32]3221225473,$false,'DENY'),
  @([uint32]3221225539,[uint32]3221225473,$false,'DENY'),
  @([uint32]3221225524,[uint32]3221225473,$true,'DENY'),
  @([uint32]0,[uint32]0,$false,'DENY'))
 foreach($case in $absenceCases){if([PaccN2PlacementNative]::AbsenceDecision($case[0],$case[1],$case[2])-cne$case[3]){throw 'ABSENCE_MODEL_FAIL'}}
 [Console]::Out.WriteLine('Q4_ABSENCE_MODELS=PASS cases=8; NO_NATIVE_QUERY')
 $stat=[byte[]]::new(56);$stat[24]=1
 if(-not[PaccN2PlacementNative]::PrimaryStatistics($stat)){throw 'PRIMARY_STATISTICS_VALID_FAIL'}
 $stat[24]=2;if([PaccN2PlacementNative]::PrimaryStatistics($stat)){throw 'IMPERSONATION_TOKEN_ACCEPTED'}
 $stat[24]=0;if([PaccN2PlacementNative]::PrimaryStatistics($stat)){throw 'TOKEN_KIND_UNKNOWN_ACCEPTED'}
 foreach($bad in @($null,[byte[]]::new(55),[byte[]]::new(57))){if([PaccN2PlacementNative]::PrimaryStatistics($bad)){throw 'TOKEN_STATISTICS_WIDTH_ACCEPTED'}}
 [Console]::Out.WriteLine('Q4_PRIMARY_STATISTICS_MODELS=PASS cases=6; NO_TOKEN_QUERY')
 $hash='b02ee54fb2ec69673386d41119ee8ed083a6eab3bfca6aa2155d20ce68ef8963';$raw='7852b616c03c57528c4f85f843fd2f671d49dbd66a965f227c2ffbf663a177e0'
 $facts=@(417614100,327680,1591236,0,867840,2,$hash,$raw)
 if(-not[PaccN2PlacementNative]::WindowsFactsMatch($facts[0],$facts[1],$facts[2],$facts[3],$facts[4],$facts[5],$facts[6],$facts[7])){throw 'WINDOWS_FACTS_VALID_FAIL'}
 for($i=0;$i-lt8;$i++){$bad=$facts.Clone();if($i-lt6){$bad[$i]=[uint32]1}else{$bad[$i]='f'*64};if([PaccN2PlacementNative]::WindowsFactsMatch($bad[0],$bad[1],$bad[2],$bad[3],$bad[4],$bad[5],$bad[6],$bad[7])){throw 'WINDOWS_FACTS_DIVERGENCE_ACCEPTED'}}
 [Console]::Out.WriteLine('Q4_WINDOWS_FACTS_MODELS=PASS cases=9; NO_NATIVE_QUERY')
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
 $script:Q4N7Stream.Dispose();$script:Q4N7Stream=$null;$script:Q4ApprovalStream.Dispose();$script:Q4ApprovalStream=$null
 $global:LASTEXITCODE=2;if($result-eq'NONE'){$global:LASTEXITCODE=0};return
}
$receipt=$pkg+'\manual-Q4P-20261025-01.txt'
$owner=[PaccN2PlacementNative]::new();$first='NONE';$closed=$false;$receiptRetained=$false;$writer=$null;$stream=$null;$receiptStatus='NOT_CREATED'
try {
 $owner.Prepare()
Write-Host 'Q4P-20261025-01; ATTEMPT=10584f02e11f92c92b43cddf0d9d316f; BOOT=236441'
Write-Host 'Create TWO NEW protected files only, 8432/397 bytes. Parent and existing files unchanged.'
Write-Host 'No Start/Stop/SCM/ACL remediation/admission pins/intent. No overwrite/delete/retry.'
Write-Host 'Exact descriptor SHA256=0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814'
Write-Host 'Escape requests Stop between synchronous native calls. Abrupt host loss can leave UNKNOWN.'
Write-Host 'Run only after separate exact-operation owner approval. This prompt grants no authority.'
$expected='PLACE_N2_INPUTS Q4P-20261025-01 10584f02e11f92c92b43cddf0d9d316f'
$confirmation=Read-Host ('Type exactly: '+$expected)
if($confirmation-cne$expected){throw 'CONFIRMATION_DENY_NO_EFFECT'}
Check-OperatorBoot

 $owner.RevalidateBeforeEffects()
 $first=[PaccN2PlacementSteps]::Run($owner);$closed=-not $owner.Retained
} catch {
 if($first-eq'NONE'){$first=[PaccN2PlacementSteps]::Describe($_.Exception)}
} finally {
 if(-not $closed -and -not $owner.Retained){$closed=$owner.Close()}
}
# Console evidence first; receipt is attempted only AFTER actual input creation.
foreach($line in $owner.Evidence){Write-Host $line}
$final='NATIVE_FIRST_ERROR='+$first+'; NATIVE_OWNED_CLOSED='+$closed+'; RETAINED='+$owner.Retained+'; PINS=EMPTY_DENY; ED11=PARTIAL; NO_RETRY'
Write-Host $final
if($owner.EffectsCreated){
 try {
  $stream=[IO.File]::Open($receipt,[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::Read)
  $writer=[IO.StreamWriter]::new($stream,[Text.UTF8Encoding]::new($false))
  $writer.WriteLine('Q4P-20261025-01; SCRIPT='+$ExpectedScriptSha256+'; OPERATIONAL_AUTHORITY=NONE')
  foreach($line in $owner.Evidence){$writer.WriteLine($line)}
  $writer.WriteLine($final);$writer.Flush();$receiptStatus='WRITTEN_CLOSE_PENDING'
 }catch{
  if($first-eq'NONE'){$first='ADMIN_RECEIPT_FAILED '+[PaccN2PlacementSteps]::Describe($_.Exception)}
  $receiptStatus='FAILED_OR_PARTIAL';Write-Host ('ADMIN_RECEIPT_FAILED; FIRST_ERROR='+$first+'; PRESERVE_INPUTS_NO_RETRY')
 }finally{
  try{if($null-ne$writer){$writer.Dispose()}elseif($null-ne$stream){$stream.Dispose()};if($receiptStatus-eq'WRITTEN_CLOSE_PENDING'){$receiptStatus='CONFIRMED'}}catch{$receiptRetained=$true;$receiptStatus='CLOSE_UNKNOWN';if($first-eq'NONE'){$first='ADMIN_RECEIPT_CLOSE_UNKNOWN'}}
 }
}
Write-Host ('FIRST_ERROR='+$first+'; ADMIN_RECEIPT_CLOSE='+$receiptStatus)
# Preserve both authorization pins when native/receipt retention is uncertain.
if(-not$owner.Retained-and-not$receiptRetained){
 foreach($name in @('Q4N7Stream','Q4ApprovalStream')){
  try{$pin=Get-Variable -Scope Script -Name $name -ValueOnly;if($null-ne$pin){$pin.Dispose();Set-Variable -Scope Script -Name $name -Value $null}}
  catch{$receiptRetained=$true;if($first-eq'NONE'){$first='AUTHORITY_PIN_CLOSE_UNKNOWN'};break}
 }
}
[Console]::Out.WriteLine('AUTHORITY_PINS_CLOSED='+(-not$receiptRetained-and-not$owner.Retained)+'; FIRST_ERROR='+$first)
if($owner.Retained -or $receiptRetained){
 $script:RetainedPlacement=$owner;$script:RetainedAdministrativeWriter=$writer;$script:RetainedAdministrativeStream=$stream
 Write-Host 'RETAINED_UNKNOWN: preserve owner and evidence; no close/kill/retry.'
 while($true){[Threading.Thread]::Sleep(1000)}
}
$global:LASTEXITCODE=2
if($first-eq'NONE' -and $closed -and $receiptStatus-eq'CONFIRMED'){$global:LASTEXITCODE=0}
Write-Host ('N2_PLACEMENT_EXIT='+$global:LASTEXITCODE)


