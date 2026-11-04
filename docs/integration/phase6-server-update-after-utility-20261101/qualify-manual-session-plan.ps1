# Isolated in-memory ACL/data-argument qualification, NOT invocation of the driver.
$ErrorActionPreference='Stop'
$folder=Split-Path -Parent $MyInvocation.MyCommand.Path
$tokens=$null;$errors=$null
$ast=[Management.Automation.Language.Parser]::ParseFile((Join-Path $folder 'run-first-diagnostic.MANUAL.ps1'),[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'MANUAL_PLAN_PARSE_DENY'}
$functionAst=@($ast.FindAll({param($node) $node -is [Management.Automation.Language.FunctionDefinitionAst] -and $node.Name-eq'Assert-DocumentMetadata'},$true))
if($functionAst.Count-ne1){throw 'METADATA_FUNCTION_REQUIRED'}
# Shadow ONLY Get-Acl with an in-memory FileSecurity object. No filesystem query.
function Get-Acl {param($LiteralPath,$ErrorAction) return $script:modelDocumentAcl}
. ([ScriptBlock]::Create($functionAst[0].Extent.Text))
$sessionActor='S-1-5-21-785699550-1440458426-1682014700-1108'
$sddls=@(
 ('O:'+$sessionActor+'G:SYD:P(A;;FA;;;'+$sessionActor+')(A;;FA;;;SY)(A;;FA;;;BA)'),
 ('O:BAG:SYD:P(A;;FA;;;'+$sessionActor+')(A;;FA;;;SY)(A;;FA;;;BA)'),
 ('O:'+$sessionActor+'G:SYD:(A;;FA;;;'+$sessionActor+')(A;;FA;;;SY)(A;;FA;;;BA)'),
 ('O:'+$sessionActor+'G:SYD:P(A;;FR;;;'+$sessionActor+')(A;;FA;;;SY)(A;;FA;;;BA)'),
 ('O:'+$sessionActor+'G:SYD:P(A;;FA;;;'+$sessionActor+')(A;;FA;;;SY)(A;;FA;;;BA)(A;;FA;;;AU)'),
 ('O:'+$sessionActor+'G:SYD:P(A;ID;FA;;;'+$sessionActor+')(A;;FA;;;SY)(A;;FA;;;BA)')
)
for($i=0;$i-lt$sddls.Count;$i++){
 $script:modelDocumentAcl=[Security.AccessControl.FileSecurity]::new()
 $script:modelDocumentAcl.SetSecurityDescriptorSddlForm($sddls[$i])
 $denied=$false
 try{Assert-DocumentMetadata 'MODEL_NO_FILE'}catch{$denied=$true}
 if($denied-ne($i-ne0)){throw ('ACL_MODEL_DENY_'+$i)}
}
$argumentsAst=@($ast.FindAll({param($node) $node -is [Management.Automation.Language.AssignmentStatementAst] -and $node.Left.Extent.Text-eq'$sessionArguments'},$true))
if($argumentsAst.Count-ne1){throw 'ARGUMENT_PLAN_REQUIRED'}
if(@($argumentsAst[0].Right.FindAll({param($node) $node -is [Management.Automation.Language.CommandAst]},$true)).Count){throw 'ARGUMENT_DATA_MUST_CONTAIN_NO_COMMANDS'}
$sessionRequest='MODEL_REQUEST';$sessionDecision='MODEL_DECISION'
$arguments=@(& ([ScriptBlock]::Create($argumentsAst[0].Right.Extent.Text)))
if($arguments.Count-ne14 -or $arguments[0]-cne'--owner-manual-first-scq-start' -or $arguments[1]-cne'MODEL_REQUEST' -or $arguments[3]-cne'MODEL_DECISION'){throw 'ARGUMENT_LAYOUT_DENY'}
if($arguments[2]-cne'312999dd9ba4f701991560e65b4a25e3a13c7914bc75603a6efb47a11536798d' -or $arguments[4]-cne'c6c34866050bc0da189acd8724470838909bc7c790b838e23c46cb86ef58210f' -or $arguments[5]-cne'3d59007bdc0714eb89eb646479e7a23d52fd82c953f1c72deec541a3643bce0b'){throw 'ARGUMENT_PINS_DENY'}
if(($arguments[6..13]-join',')-cne'1994944,1994944,7239172096,9823326208,128,100,60000,240000'){throw 'G3_ARGUMENT_ORDER_DENY'}
'PASS MANUAL_SESSION_PLAN ACL_CASES=6 ARGUMENTS=14 G3_AND_REAL_DECISION_PIN_EXACT; ACL_IN_MEMORY; ONLY_FUNCTION_AND_DATA_AST_EXECUTED; DRIVER_NOT_INVOKED; NO_FILE_QUERY_NO_SCM_ENDPOINT_START'
