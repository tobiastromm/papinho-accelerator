# Offline-only: parse the wrapper, evaluate its literal argv construction only.
# Never dot-source/invoke the wrapper or its file/process/native helpers.
$ErrorActionPreference='Stop'
$folder='C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
$path=Join-Path $folder 'update-wait-state-image.MANUAL.CANDIDATE.ps1'
$tokens=$null;$errors=$null
$ast=[System.Management.Automation.Language.Parser]::ParseFile($path,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'WRAPPER_AST_DENY'}
$assign=@($ast.FindAll({param($n) $n -is [System.Management.Automation.Language.AssignmentStatementAst] -and $n.Left.Extent.Text -ceq '$arguments'},$true))
if($assign.Count-ne1){throw 'ARGV_ASSIGNMENT_CENSUS_DENY'}
if(@($assign[0].Right.FindAll({param($n) $n -is [System.Management.Automation.Language.CommandAst] -or $n -is [System.Management.Automation.Language.InvokeMemberExpressionAst]},$true)).Count){throw 'ARGV_EFFECT_DENY'}
foreach($v in @($assign[0].FindAll({param($n) $n -is [System.Management.Automation.Language.VariableExpressionAst]},$true))){if($v.VariablePath.UserPath-notin@('arguments','data','oldPin','newPin','sslPin','cryptoPin','executorPin')){throw 'ARGV_VARIABLE_DENY'}}
$data='C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe'
$oldPin='11cbef6508b1b06fb56031b22b93dccc6ae3e05df22528190f3cf7215636d815'
$newPin='044f10de34010b83075976ba39599a92381bce483ade9f780960b0fcc73010b1'
$sslPin='3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'
$executorPin='084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a'
$cryptoPin='09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'
. ([scriptblock]::Create($assign[0].Extent.Text))
$expected=@('--owner-manual-binary-update',$data,'417614100','8257536','71209','417614100','5242880','349380',$oldPin,$newPin,$sslPin,$cryptoPin,'417614100','30670848','57859','084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a')
function Matches-ExactBinding([object[]]$v){if($v.Count-ne16){return $false};for($i=0;$i-lt16;$i++){if([string]$v[$i]-cne[string]$expected[$i]){return $false}};return $true}
if(-not(Matches-ExactBinding $arguments)){throw 'EXACT_NATIVE_ARGV_DENY'}
$checks=1
for($i=0;$i-lt16;$i++){$bad=$arguments.Clone();$bad[$i]='WRONG_BINDING';if(Matches-ExactBinding $bad){throw 'MUTATED_ARG_ACCEPTED'};$checks++}
if(Matches-ExactBinding @($arguments[0..14])){throw 'TRUNCATED_ARGV_ACCEPTED'};$checks++
$bad=$arguments+@('EXTRA');if(Matches-ExactBinding $bad){throw 'EXTRA_ARGV_ACCEPTED'};$checks++
$bad=$arguments.Clone();$bad[8]=$newPin;$bad[9]=$oldPin;if(Matches-ExactBinding $bad){throw 'SWAPPED_PINS_ACCEPTED'};$checks++
$bad=$arguments.Clone();$bad[6]='1638400';$bad[7]='326399';if(Matches-ExactBinding $bad){throw 'PREVIOUS_CANONICAL_ACCEPTED'};$checks++
$bad=$arguments.Clone();$bad[1]='C:\Program Files\Papinho\Accelerator\papacc_server.exe';if(Matches-ExactBinding $bad){throw 'INSTALLED_AS_DATA_ACCEPTED'};$checks++
$commands=@($ast.FindAll({param($n) $n -is [System.Management.Automation.Language.CommandAst] -and $n.Extent.Text -ceq '& $executor @arguments'},$true))
if($commands.Count-ne1){throw 'NATIVE_INVOCATION_CENSUS_DENY'}
$manual=Get-Content -LiteralPath $path -Raw
if($manual.Contains('OWNER_BINARY_UPDATE ')-or$manual.Contains('DISPOSE_STAGE212_7580_18000_ONCE')){throw 'AUTOMATIC_CONFIRMATION_OR_DISPOSITION_DENY'}
if(-not$manual.Contains('Bytes=805376;Pin=$newPin')-or-not$manual.Contains('Bytes=804352;Pin=$oldPin')){throw 'SIZE_SCOPE_DENY'}
Write-Output ('PASS_WAIT_STATE_UPDATE_WRAPPER AST=PASS native_argv=16 binding_checks='+$checks+' future_native_invocations=1 NO_WRAPPER_OR_NATIVE_HELPER_EXECUTION')
