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
$template=[IO.File]::ReadAllText($p+'\Q7-APPROVAL-RECORD.INERT-TEMPLATE.json');$count=0
foreach($name in @('inert-denied','hypothetical-valid-in-memory','old-placement-record','unknown-close','wrong-actual-binary')){
 $record=$template|ConvertFrom-Json
 if($name-ne'inert-denied'){$record.status='EXACT_MANUAL_POSITIVE_OWNER_APPROVED';$record.q7_activation_authorized=$true}
 if($name-eq'old-placement-record'){$record=([IO.File]::ReadAllText($root+'\docs\integration\n0start03-integrated-closeout-20261024\q4-n7-protected-inputs-and-activation-20261025\EXACT-PROTECTED-PLACEMENT-APPROVED.json')|ConvertFrom-Json)}
 if($name-eq'unknown-close'){$record.q7_activation_authorized=$null}
 if($name-eq'wrong-actual-binary'){$record.artifacts.qualifier.sha256='f'*64}
 $m=[IO.MemoryStream]::new([Text.Encoding]::UTF8.GetBytes(($record|ConvertTo-Json -Depth 12)),$false);$denied=$false;try{Assert-PositiveApproval $m}catch{$denied=$true}finally{$m.Dispose()}
 if($denied-ne($name-ne'hypothetical-valid-in-memory')){throw ('ACTUAL_GATE_FAIL '+$name)};$count++
}
if([IO.File]::Exists($ApprovalPath)){throw 'UNEXPECTED_OWNER_APPROVAL_RECORD'}
Write-Output ('ACTUAL_CANDIDATE_PINS_AND_INERT_APPROVAL=PASS cases='+$count+'; HYPOTHETICAL_RECORD_MEMORY_ONLY; NO_AUTHORITY_CREATED_NO_LAUNCHER')
