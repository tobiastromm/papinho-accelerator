# Offline SD/flow/model/AST only. No native adapter constructor or Prepare.
$ErrorActionPreference='Stop'
$f=$PSScriptRoot
$m=Get-Content -LiteralPath (Join-Path $f 'P1-READ-PREPARER-ARTIFACT-BINDINGS.json') -Raw|ConvertFrom-Json
foreach($b in $m){
 if((Get-FileHash -LiteralPath (Join-Path $f $b.base)).Hash-ne$b.base_sha256-or(Get-FileHash -LiteralPath (Join-Path $f $b.artifact)).Hash-ne$b.sha256){throw 'PIN_DENY'}
 $t=[IO.File]::ReadAllText((Join-Path $f $b.base))
 foreach($r in $b.rules){if([regex]::Matches($t,[regex]::Escape($r.old)).Count-ne$r.count){throw 'RULE_COUNT_DENY'};$t=$t.Replace($r.old,$r.new)}
 if($t-cne[IO.File]::ReadAllText((Join-Path $f $b.artifact))){throw 'EXACT_DELTA_DENY'}
}
$source=Get-Content -LiteralPath (Join-Path $f 'p1-read-session-originals.cs') -Raw
$model=Get-Content -LiteralPath (Join-Path $f 'p1-read-originals.OFFLINE-MODEL.cs') -Raw
if($model-match'DllImport|extern '){throw 'MODEL_BOUNDARY_DENY'}
$usings=@([regex]::Matches($source+"`n"+$model,'(?m)^using [A-Za-z0-9_.]+;')|ForEach-Object {$_.Value}|Sort-Object -Unique)
$body=[regex]::Replace($source+"`n"+$model,'(?m)^using [A-Za-z0-9_.]+;','')
$c=[CodeDom.Compiler.CompilerParameters]::new();$c.GenerateInMemory=$true;$c.WarningLevel=4;$c.TreatWarningsAsErrors=$true
[void]$c.ReferencedAssemblies.Add('System.dll');[void]$c.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition (($usings-join"`n")+"`n"+$body) -CompilerParameters $c
[P1ReadDocumentsModel]::Run()
# Exercise real PS metadata policy against an in-memory FileSecurity object.
$sessionActor='S-1-5-21-785699550-1440458426-1682014700-1108'
$runtimeSid='S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099'
$policy=Join-Path $f 'p1-read-document-metadata.OFFLINE-QUALIFIED.ps1'
$tokens=$null;$errors=$null;$a=[Management.Automation.Language.Parser]::ParseFile($policy,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'POLICY_AST_DENY'}
$fn=@($a.FindAll({param($n)$n-is[Management.Automation.Language.FunctionDefinitionAst]},$true))
if($fn.Count-ne1){throw 'POLICY_SCOPE_DENY'}
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
$path=Join-Path $f 'prepare-p1-read-originals.MANUAL.CANDIDATE.ps1';$tokens=$null;$errors=$null
$a=[Management.Automation.Language.Parser]::ParseFile($path,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'PREPARER_AST_DENY'}
$guard=@($a.FindAll({param($n)$n-is[Management.Automation.Language.FunctionDefinitionAst]-and$n.Name-ceq'Assert-P1OriginalPins'},$true))
. ([scriptblock]::Create($guard[0].Extent.Text))
foreach($pair in @(@('d1dee4fa8bdc00768c27c9448f6da99955b9b616c1e1523f0f778b65c2ab5e54',('2'*64)),@(('1'*64),'82957fe0833f4e1f8b7496e9b925100a6dc1c63986ca20dccaeb67c40219eabb'))){$denied=$false;try{Assert-P1OriginalPins $pair[0] $pair[1]}catch{$denied=$true};if(-not$denied){throw 'REPLAY_ACCEPTED'}}
'PASS METADATA_POLICY_CASES='+$checks+' REPLAY2 AST EXACT_TRANSFORMS; NO_FILE_OR_NATIVE_ADAPTER_OPERATION'
