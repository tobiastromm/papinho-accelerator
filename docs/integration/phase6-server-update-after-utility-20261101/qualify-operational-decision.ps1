# Compiles the reused document adapter and exercises injected IO only.
$ErrorActionPreference='Stop'
$folder=Split-Path -Parent $MyInvocation.MyCommand.Path
$source=[IO.File]::ReadAllText((Join-Path $folder 'first-start-operational-decision.cs'))
$model=[IO.File]::ReadAllText((Join-Path $folder 'operational-decision.model.cs'))
$usings=@([regex]::Matches($source+"`n"+$model,'(?m)^using [A-Za-z0-9_.]+;') | ForEach-Object {$_.Value} | Sort-Object -Unique)
$body=[regex]::Replace($source+"`n"+$model,'(?m)^using [A-Za-z0-9_.]+;','')
$compiler=[CodeDom.Compiler.CompilerParameters]::new()
$compiler.GenerateInMemory=$true;$compiler.WarningLevel=4;$compiler.TreatWarningsAsErrors=$true
[void]$compiler.ReferencedAssemblies.Add('System.dll');[void]$compiler.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition (($usings -join "`n")+"`n"+$body) -CompilerParameters $compiler
[OperationalDecisionModel]::Run()
$tokens=$null;$errors=$null
$ast=[Management.Automation.Language.Parser]::ParseFile((Join-Path $folder 'prepare-operational-decision.MANUAL.ps1'),[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'OP_MANUAL_PARSE_DENY'}
$guard=@($ast.FindAll({param($node) $node -is [Management.Automation.Language.IfStatementAst] -and $node.Extent.Text.Contains("throw 'OPERATIONAL_DECISION_DATA_SCOPE_DENY'")},$true))
if($guard.Count-ne1){throw 'UNIQUE_OP_DATA_GUARD_REQUIRED'}
$guardOnly=[ScriptBlock]::Create($guard[0].Extent.Text)
$request='312999dd9ba4f701991560e65b4a25e3a13c7914bc75603a6efb47a11536798d'
foreach($case in 0..3){
 $ExpectedRequestSha256=$request;$ExpectedDecisionSha256='2'*64
 if($case-eq1){$ExpectedRequestSha256='0'*64}
 if($case-eq2){$ExpectedDecisionSha256='7f6451d59fc0b7015e0ac612ee471782950880fcfc2013137dc9643276af641a'}
 if($case-eq3){$ExpectedDecisionSha256=$request}
 $denied=$false;try{& $guardOnly}catch{if($_.Exception.Message-notmatch'OPERATIONAL_DECISION_DATA_SCOPE_DENY'){throw};$denied=$true}
 if($denied-ne($case-ne0)){throw 'OP_DATA_GUARD_MODEL_DENY'}
}
'PASS OP_DECISION_DATA_GUARD cases=4 MANUAL_AST_PARSE=PASS; ONLY_GUARD_EXECUTED; NOT_PROOF_OF_OWNER_APPROVAL; WRAPPER_NOT_INVOKED'
