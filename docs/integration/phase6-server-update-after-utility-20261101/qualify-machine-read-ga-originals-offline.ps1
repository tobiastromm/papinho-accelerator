# Offline only: compiles the fixed adapter and exercises injected IO in memory.
$ErrorActionPreference='Stop'
$sourceDirectory=Split-Path -Parent $MyInvocation.MyCommand.Path
$code=[IO.File]::ReadAllText((Join-Path $sourceDirectory 'machine-read-ga-originals.cs'))
$model=[IO.File]::ReadAllText((Join-Path $sourceDirectory 'machine-read-ga-originals.OFFLINE-MODEL.cs'))
$usings=@([regex]::Matches($code+"`n"+$model,'(?m)^using [A-Za-z0-9_.]+;') | ForEach-Object {$_.Value} | Sort-Object -Unique)
$body=[regex]::Replace($code+"`n"+$model,'(?m)^using [A-Za-z0-9_.]+;','')
$compiler=[CodeDom.Compiler.CompilerParameters]::new()
$compiler.GenerateInMemory=$true
$compiler.WarningLevel=4
$compiler.TreatWarningsAsErrors=$true
[void]$compiler.ReferencedAssemblies.Add('System.dll')
[void]$compiler.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition (($usings -join "`n")+"`n"+$body) -CompilerParameters $compiler
[P1ReadDocumentsModel]::Run()

$f=$sourceDirectory
# Exercise real PS metadata policy against an in-memory FileSecurity object.
$sessionActor='S-1-5-21-785699550-1440458426-1682014700-1108'
$runtimeSid='S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099'
$policy=Join-Path $f 'run-machine-read-ga-diagnostic.MANUAL.CANDIDATE.ps1'
$tokens=$null;$errors=$null;$a=[Management.Automation.Language.Parser]::ParseFile($policy,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'POLICY_AST_DENY'}
$fn=@($a.FindAll({param($n)$n-is[Management.Automation.Language.FunctionDefinitionAst]},$true))
$fn=@($fn|Where-Object Name -CEQ 'Assert-DocumentMetadata');if($fn.Count-ne1){throw 'POLICY_SCOPE_DENY'}
. ([scriptblock]::Create($fn[0].Extent.Text))
function Get-Acl {param([string]$LiteralPath) return $script:mockAcl}
$checks=0
for($case=0;$case-lt12;$case++){
 $script:mockAcl=[Security.AccessControl.FileSecurity]::new()
 $owner=if($case-eq1){'S-1-5-32-544'}else{$sessionActor}
 $script:mockAcl.SetOwner([Security.Principal.SecurityIdentifier]::new($owner))
 $script:mockAcl.SetAccessRuleProtection(($case-ne2),$false)
 foreach($sid in @($sessionActor,'S-1-5-18','S-1-5-32-544',$runtimeSid)){
  if($case-eq3-and$sid-eq$runtimeSid){continue}
  $who=if($case-eq4-and$sid-eq$runtimeSid){'S-1-5-11'}else{$sid}
  $rights=if($sid-eq$runtimeSid){0x120089}else{0x1f01ff}
  if($sid-eq$runtimeSid){switch($case){5{$rights=0x1f01ff};6{$rights=0x12008b};7{$rights=0x1200a9};8{$rights=0x120088}}}
  if($case-eq9-and$sid-eq$sessionActor){$rights=0x120089}
  $kind=if($case-eq10-and$sid-eq$runtimeSid){[Security.AccessControl.AccessControlType]::Deny}else{[Security.AccessControl.AccessControlType]::Allow}
  $rule=[Security.AccessControl.FileSystemAccessRule]::new([Security.Principal.SecurityIdentifier]::new($who),[Security.AccessControl.FileSystemRights]$rights,$kind)
  $script:mockAcl.AddAccessRule($rule)
 }
 if($case-eq11){$script:mockAcl.AddAccessRule([Security.AccessControl.FileSystemAccessRule]::new([Security.Principal.SecurityIdentifier]::new('S-1-1-0'),[Security.AccessControl.FileSystemRights]0x120089,[Security.AccessControl.AccessControlType]::Allow))}
 $denied=$false;try{Assert-DocumentMetadata 'MODEL_ONLY'}catch{$denied=$true}
 if($denied-ne($case-ne0)){throw ('POLICY_CASE_DENY '+$case)};$checks++
}
Remove-Item Function:\Get-Acl
$path=Join-Path $f 'prepare-machine-read-ga-originals.MANUAL.CANDIDATE.ps1';$tokens=$null;$errors=$null
$a=[Management.Automation.Language.Parser]::ParseFile($path,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'PREPARER_AST_DENY'}
$guard=@($a.FindAll({param($n)$n-is[Management.Automation.Language.FunctionDefinitionAst]-and$n.Name-ceq'Assert-P1OriginalPins'},$true))
. ([scriptblock]::Create($guard[0].Extent.Text))
foreach($pair in @(@('d1dee4fa8bdc00768c27c9448f6da99955b9b616c1e1523f0f778b65c2ab5e54',('2'*64)),@(('1'*64),'82957fe0833f4e1f8b7496e9b925100a6dc1c63986ca20dccaeb67c40219eabb'))){$denied=$false;try{Assert-P1OriginalPins $pair[0] $pair[1]}catch{$denied=$true};if(-not$denied){throw 'REPLAY_ACCEPTED'}}
'PASS METADATA_POLICY_CASES='+$checks+' REPLAY2 AST_POLICY_EXTRACTION; NO_FILE_OR_NATIVE_ADAPTER_OPERATION'

foreach($old in @('f431b5e9c4c68371649d9ee4277d6b8f6b76568c7c8805dbb6f972e8c3b5e579','c18d6abf399ae0e29f12ac79af299469fb43097fa923593006d66df50f4bddb4')){
 $denied=$false;try{Assert-P1OriginalPins ('1'*64) $old}catch{$denied=$true};if(-not $denied){throw 'CONSUMED_DECISION_ACCEPTED'}
}
$run=[IO.File]::ReadAllText((Join-Path $f 'run-machine-read-ga-diagnostic.MANUAL.CANDIDATE.ps1'))
$block=[regex]::Match($run,'(?s)\$sessionArguments=@\(.*??
\)').Value
if(-not $block -or $block -match '[;&|`]' -or $block -match '\$\('){throw 'DATA_BLOCK_SCOPE_DENY'}
$sessionRequest='MODEL_REQUEST';$ExpectedOperationalRequestSha256='1'*64;$sessionDecision='MODEL_DECISION';$ExpectedDecisionSha256='2'*64
. ([scriptblock]::Create($block))
if($sessionArguments.Count-ne16 -or $sessionArguments[0]-cne'--owner-manual-machine-read-scq-start' -or $sessionArguments[6]-cne'1994944' -or $sessionArguments[7]-cne'3145728' -or $sessionArguments[8]-cne'7239172096' -or $sessionArguments[9]-cne'3221225472' -or $sessionArguments[10]-cne'192' -or $sessionArguments[11]-cne'100' -or $sessionArguments[12]-cne'60000' -or $sessionArguments[13]-cne'240000'){throw 'READ_ARGV_BINDING_DENY'}
if($sessionArguments[14]-cne'PapinhoAccelerator.CA.f3f7579f3e90851845264abbb7991500.9b845d6b6c8e909e5f226c65b321987c' -or $sessionArguments[15]-cne'877e3f7d80794459586e6d1b977127cd6537f8287c5087055f508f6e95863278'){throw 'FIXTURE_BINDING_DENY'}
if($run -match 'P1 TRANSPORT_ONLY AMD64|--owner-manual-first-scq-start-to-existing'){throw 'TO_FALLBACK_DENY'}
'PASS READ_ARGV16_FIXED_PURPOSE_LIMITS_FIXTURE REPLAY2_ADDITIONAL NO_INVOCATION'
