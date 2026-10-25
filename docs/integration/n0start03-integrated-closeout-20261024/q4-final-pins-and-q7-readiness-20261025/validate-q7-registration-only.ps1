# Actual candidate artifact variables and inert record in memory only. No launcher call.
$ErrorActionPreference='Stop';Set-StrictMode -Version Latest
function File-Sha([string]$path){$sha=[Security.Cryptography.SHA256]::Create();try{return ([BitConverter]::ToString($sha.ComputeHash([IO.File]::ReadAllBytes($path)))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}}
$root='C:\Projetos\PapinhoAccelerator';$p=$root+'\docs\integration\n0start03-integrated-closeout-20261024\q4-final-pins-and-q7-readiness-20261025'
$tokens=$null;$errors=$null;$ast=[Management.Automation.Language.Parser]::ParseFile($root+'\tools\qualification\scm-n0-protected-single-powershell.ps1',[ref]$tokens,[ref]$errors);if($errors.Count){throw 'PARSE_DENY'}
$block=$ast.FindAll({param($n)$n-is[Management.Automation.Language.IfStatementAst]-and$n.Extent.Text.StartsWith("if(`$Profile-ceq'Q4TokenDiagnostic'){")-and$n.Extent.Text.Contains('$QualifierHash=')},$true);if($block.Count-ne1){throw 'PROFILE_ASSIGNMENT_BOUNDARY'}
$Root=$root;$Profile='Q4TokenDiagnostic';. ([scriptblock]::Create($block[0].Extent.Text))
$ExpectedPath=$root+'\tools\qualification\scm-n0-protected-single-powershell.ps1';$ExpectedScriptSha256=(File-Sha $ExpectedPath);$TransportBinding=$root+'\build\ed11-private-stdout-20261025\Selected\Release\Pacc.N0PrivateStdout.Binding.dll';$TransportBindingHash='513107355c46b8282ac17eb9763a1278bd201c0d4f9f9a925950676fba14d9f4';$T10DecisionHash='1f293812e9fca107c7fa8ba20abe20fd8ffbca3cdf6e95e58465a95dadbe4dd5'
$f=$ast.FindAll({param($n)$n-is[Management.Automation.Language.FunctionDefinitionAst]-and$n.Name-eq'Assert-PositiveApproval'},$true);. ([scriptblock]::Create($f[0].Extent.Text))
foreach($pair in @(@($Qualifier,$QualifierHash),@($TransportNative,$TransportNativeHash),@($TransportBinding,$TransportBindingHash))){if((File-Sha $pair[0])-cne$pair[1]){throw 'ACTUAL_ARTIFACT_PIN_DIVERGENCE'}}

$approval=$p+'\EXACT-Q7-APPROVED.json'
if((File-Sha $approval)-cne'c07384b26a57a438c10a382948ab2be5ffe8ab80fab56138881e605c8c6f4c46'){throw 'EXACT_APPROVAL_HASH_DIVERGENCE'}
$f=[IO.File]::Open($approval,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
try{Assert-PositiveApproval $f;if($f.Position-ne0){throw 'STREAM_POSITION_LOST'}}finally{$f.Dispose()}
Write-Output 'Q7_REGISTERED_APPROVAL=PASS; EXACT_ARTIFACTS_AND_SAME_STREAM_TYPE_GATE; NO_LAUNCHER_NO_PREFLIGHT_NO_EFFECT'
