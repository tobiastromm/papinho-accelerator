# Actual candidate artifact variables and inert record in memory only. No launcher call.
$ErrorActionPreference='Stop';Set-StrictMode -Version Latest
function File-Sha([string]$path){$sha=[Security.Cryptography.SHA256]::Create();try{return ([BitConverter]::ToString($sha.ComputeHash([IO.File]::ReadAllBytes($path)))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}}
$root='C:\Projetos\PapinhoAccelerator';$p=$root+'\docs\integration\n0start03-integrated-closeout-20261024\q7-source-image-pin-correction-20261025'
$tokens=$null;$errors=$null;$ast=[Management.Automation.Language.Parser]::ParseFile($root+'\tools\qualification\scm-n0-protected-single-powershell.ps1',[ref]$tokens,[ref]$errors);if($errors.Count){throw 'PARSE_DENY'}
$block=$ast.FindAll({param($n)$n-is[Management.Automation.Language.IfStatementAst]-and$n.Extent.Text.StartsWith("if(`$Profile-ceq'Q4TokenDiagnostic'){")-and$n.Extent.Text.Contains('$QualifierHash=')},$true);if($block.Count-ne1){throw 'PROFILE_ASSIGNMENT_BOUNDARY'}
$Root=$root;$Profile='Q4TokenDiagnostic';. ([scriptblock]::Create($block[0].Extent.Text))
$ExpectedPath=$root+'\tools\qualification\scm-n0-protected-single-powershell.ps1';$ExpectedScriptSha256=(File-Sha $ExpectedPath);$TransportBinding=$root+'\build\ed11-private-stdout-20261025\Selected\Release\Pacc.N0PrivateStdout.Binding.dll';$TransportBindingHash='513107355c46b8282ac17eb9763a1278bd201c0d4f9f9a925950676fba14d9f4';$T10DecisionHash='1f293812e9fca107c7fa8ba20abe20fd8ffbca3cdf6e95e58465a95dadbe4dd5'
$f=$ast.FindAll({param($n)$n-is[Management.Automation.Language.FunctionDefinitionAst]-and$n.Name-eq'Assert-PositiveApproval'},$true);. ([scriptblock]::Create($f[0].Extent.Text))
foreach($pair in @(@($Qualifier,$QualifierHash),@($TransportNative,$TransportNativeHash),@($TransportBinding,$TransportBindingHash))){if((File-Sha $pair[0])-cne$pair[1]){throw 'ACTUAL_ARTIFACT_PIN_DIVERGENCE'}}

$record=([IO.File]::ReadAllText($p+'\CORRECTED-APPROVAL.INERT-TEMPLATE.json')|ConvertFrom-Json)
$cases=0
foreach($kind in @('inert','hypothetical-current-memory-only','old-used-approval')){
 $r=$record|ConvertTo-Json -Depth 10|ConvertFrom-Json
 if($kind-eq'hypothetical-current-memory-only'){$r.status='EXACT_MANUAL_POSITIVE_OWNER_APPROVED';$r.q7_activation_authorized=$true}
 if($kind-eq'old-used-approval'){$r=([IO.File]::ReadAllText($root+'\docs\integration\n0start03-integrated-closeout-20261024\q4-final-pins-and-q7-readiness-20261025\EXACT-Q7-APPROVED.json')|ConvertFrom-Json)}
 $f=[IO.MemoryStream]::new([Text.Encoding]::UTF8.GetBytes(($r|ConvertTo-Json -Depth 10)),$false);$denied=$false
 try{Assert-PositiveApproval $f}catch{$denied=$true}finally{$f.Dispose()}
 if($denied-ne($kind-ne'hypothetical-current-memory-only')){throw 'POSTFAILURE_APPROVAL_GATE'};$cases++
}
if([IO.File]::Exists($ApprovalPath)){throw 'UNEXPECTED_FUTURE_APPROVAL'}
Write-Output ('CORRECTED_ARTIFACT_APPROVAL_GATES=PASS cases='+$cases+'; OLD_USED_APPROVAL_DENIED; NEW_RECORD_ABSENT; NO_LAUNCHER')
