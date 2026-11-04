# Offline compiler/model/parser/guard qualification; never invokes the manual procedure.
$ErrorActionPreference='Stop'
$folder=Split-Path -Parent $MyInvocation.MyCommand.Path
$source=[IO.File]::ReadAllText((Join-Path $folder 'first-start-originals.cs'))
$model=[IO.File]::ReadAllText((Join-Path $folder 'first-start-preparation-scope.model.cs'))
$usings=@([regex]::Matches($source+"`n"+$model,'(?m)^using [A-Za-z0-9_.]+;') | ForEach-Object {$_.Value} | Sort-Object -Unique)
$body=[regex]::Replace($source+"`n"+$model,'(?m)^using [A-Za-z0-9_.]+;','')
$compiler=[CodeDom.Compiler.CompilerParameters]::new()
$compiler.GenerateInMemory=$true;$compiler.WarningLevel=4;$compiler.TreatWarningsAsErrors=$true
[void]$compiler.ReferencedAssemblies.Add('System.dll');[void]$compiler.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition (($usings -join "`n")+"`n"+$body) -CompilerParameters $compiler
[FirstStartPreparationScopeModel]::Run()
$tokens=$null;$errors=$null
$ast=[Management.Automation.Language.Parser]::ParseFile((Join-Path $folder 'prepare-first-start-originals.PREPARATION-ONLY.MANUAL.ps1'),[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'MANUAL_PARSE_DENY'}
$guard=@($ast.FindAll({param($node) $node -is [Management.Automation.Language.IfStatementAst] -and $node.Extent.Text.Contains("throw 'PREPARATION_SCOPE_PIN_DENY'")},$true))
if($guard.Count-ne1){throw 'UNIQUE_SCOPE_GUARD_REQUIRED'}
# Execute ONLY the isolated, fixed two-pin guard AST; no wrapper/native constructor.
$guardOnly=[ScriptBlock]::Create($guard[0].Extent.Text)
$request='312999dd9ba4f701991560e65b4a25e3a13c7914bc75603a6efb47a11536798d'
$decision='7f6451d59fc0b7015e0ac612ee471782950880fcfc2013137dc9643276af641a'
foreach($case in 0..2){
 $ExpectedRequestSha256=$request;$ExpectedDecisionSha256=$decision
 if($case-eq1){$ExpectedRequestSha256='0'*64};if($case-eq2){$ExpectedDecisionSha256='0'*64}
 $denied=$false;try{& $guardOnly}catch{if($_.Exception.Message-notmatch'PREPARATION_SCOPE_PIN_DENY'){throw};$denied=$true}
 if($denied-ne($case-ne0)){throw 'SCOPE_GUARD_MODEL_DENY'}
}
'PASS PREPARATION_GUARD cases=3 MANUAL_AST_PARSE=PASS; ONLY_GUARD_EXECUTED; MANUAL_WRAPPER_NOT_INVOKED'
