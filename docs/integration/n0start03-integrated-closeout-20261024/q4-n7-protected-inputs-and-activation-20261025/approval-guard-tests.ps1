# Pure extracted approval guards; synthetic in-memory records, never an approval file.
$ErrorActionPreference='Stop';Set-StrictMode -Version Latest
$root='C:\Projetos\PapinhoAccelerator';$pkg=$root+'\docs\integration\n0start03-integrated-closeout-20261024\q4-n7-protected-inputs-and-activation-20261025'
$scriptPath=$pkg+'\q4-input-placement.CANDIDATE.ps1';$ExpectedScriptSha256=(Get-FileHash -LiteralPath $scriptPath -Algorithm SHA256).Hash.ToLowerInvariant()
$t=$null;$e=$null;$ast=[Management.Automation.Language.Parser]::ParseFile($scriptPath,[ref]$t,[ref]$e);if($e.Count){throw 'AST_FAIL'}
$f=$ast.FindAll({param($n)$n-is[Management.Automation.Language.FunctionDefinitionAst]-and$n.Name-eq'Assert-Q4PlacementApproval'},$true);if($f.Count-ne1){throw 'APPROVAL_GUARD_BOUNDARY'}
. ([scriptblock]::Create($f[0].Extent.Text))
$inputs=Get-Content -LiteralPath ($pkg+'\candidate-input-hashes.json') -Raw|ConvertFrom-Json
function SyntheticRecord {
 return [ordered]@{status='EXACT_MANUAL_Q4_INPUT_PLACEMENT_OWNER_APPROVED';operation='Q4P-20261025-01';attempt='10584f02e11f92c92b43cddf0d9d316f';manual_only=$true;automatic_execution=$false;operational_admission=$false;script_sha256=$ExpectedScriptSha256;evidence_sha256=$inputs.'windows-origin-evidence.CANDIDATE.json'.sha256;record_sha256=$inputs.'OPENED-WINDOWS-FILE.CANDIDATE.txt'.sha256;creation_descriptor_sha256='0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814';N7_proposed_sha256='a5bcb78615b059d8f3c9508641442d20663a204c8ad19563c03953c87b4d3aba';N7_decision_sha256=('a'*64)}
}
$cases=0
foreach($case in @('valid','old03','old02','wrong-operation','status','script','evidence','record','descriptor','missing-N7','N7-source','N7-decision','auto','manual','admission','bool-string','array-operation','array-attempt')){
 $record=SyntheticRecord
 switch($case){'old03'{$record.attempt='0b1de869e11493b7cb08fb329578f6ce'}'old02'{$record.attempt='75330d11cc7e4123ae62eadc9a59c2f5'}'wrong-operation'{$record.operation='N0START-20261024-03'}'status'{$record.status='DRAFT'}'script'{$record.script_sha256='f'*64}'evidence'{$record.evidence_sha256='f'*64}'record'{$record.record_sha256='f'*64}'descriptor'{$record.creation_descriptor_sha256='f'*64}'missing-N7'{$record.Remove('N7_decision_sha256')}'N7-source'{$record.N7_proposed_sha256='f'*64}'N7-decision'{$record.N7_decision_sha256=''}'auto'{$record.automatic_execution=$true}'manual'{$record.manual_only=$false}'admission'{$record.operational_admission=$true}'bool-string'{$record.manual_only='True'}'array-operation'{$record.operation=@('Q4P-20261025-01')}'array-attempt'{$record.attempt=@('10584f02e11f92c92b43cddf0d9d316f')}}
 $denied=$false;try{Assert-Q4PlacementApproval $record}catch{$denied=$true}
 if($denied-ne($case-ne'valid')){throw ('APPROVAL_FIXTURE_FAIL '+$case)};$cases++
}
$noApproval=$ast.FindAll({param($n)$n-is[Management.Automation.Language.IfStatementAst]-and$n.Extent.Text.StartsWith('if(-not$ExactApprovalSha256)')},$true)
if($noApproval.Count-ne1){throw 'MISSING_AUTHORITY_GATE_BOUNDARY'}
$ExactApprovalSha256=$null;$denied=$false;try{. ([scriptblock]::Create($noApproval[0].Extent.Text))}catch{$denied=$true};if(-not$denied){throw 'MISSING_AUTHORITY_ACCEPTED'};$cases++
Write-Output ('Q4_APPROVAL_EXTRACTED_GATES=PASS cases='+$cases+'; NO_APPROVAL_CREATED_NO_PLACE_NO_PROTECTED_READ')
