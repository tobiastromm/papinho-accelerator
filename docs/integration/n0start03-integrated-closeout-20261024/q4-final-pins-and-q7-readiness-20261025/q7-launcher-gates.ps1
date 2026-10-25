# Only AST-extracted pure functions/guards; no canonical launcher/native invocation.
$ErrorActionPreference='Stop';Set-StrictMode -Version Latest
$root='C:\Projetos\PapinhoAccelerator';$source=[IO.File]::ReadAllText($root+'\tools\qualification\scm-n0-protected-single-powershell.ps1')
$tokens=$null;$errors=$null;$ast=[Management.Automation.Language.Parser]::ParseInput($source,[ref]$tokens,[ref]$errors);if($errors.Count){throw ($errors|Out-String)}
$f=$ast.FindAll({param($n)$n-is[Management.Automation.Language.FunctionDefinitionAst]-and$n.Name-eq'Assert-PositiveApproval'},$true);. ([scriptblock]::Create($f[0].Extent.Text))
$a=$source.IndexOf(' if($Profile-cne');$b=$source.IndexOf(' if($Mode-eq''Prepare''',$a);$gate=[scriptblock]::Create($source.Substring($a,$b-$a));$cases=0
foreach($Profile in @('EMPTY_DENY','N0START03ReadOnly','N0START03Positive','Q4TokenDiagnostic')){foreach($Mode in @('Review','Prepare','Cycle')){
 $deny=$false;try{. $gate}catch{$deny=$true};$want=($Profile-eq'EMPTY_DENY')-or($Profile-eq'N0START03ReadOnly'-and$Mode-eq'Cycle')-or($Profile-ne'EMPTY_DENY'-and$Profile-ne'N0START03ReadOnly'-and$Mode-eq'Prepare');if($deny-ne$want){throw 'PROFILE_GATE_FAIL'};$cases++
}}
$Profile='Q4TokenDiagnostic';$Operation='N0TOKEN-20261025-01';$Attempt='10584f02e11f92c92b43cddf0d9d316f';$T10DecisionHash='1f293812e9fca107c7fa8ba20abe20fd8ffbca3cdf6e95e58465a95dadbe4dd5';$ExpectedScriptSha256='a'*64;$QualifierHash='b'*64;$TransportNativeHash='c'*64;$TransportBindingHash='d'*64
$ExpectedPath='fixture-script';$Qualifier='fixture-qualifier';$TransportNative='fixture-native';$TransportBinding='fixture-binding'
function Record {return [ordered]@{status='EXACT_MANUAL_POSITIVE_OWNER_APPROVED';operation=$Operation;attempt=$Attempt;mode='Cycle';manual_only=$true;automatic_execution=$false;request_sha256=('e'*64);T10_T14_sha256=$T10DecisionHash;Q0_Q9_sha256='97763395f521d0a8d15ee0afe22b61705897d6d97c2c1f8905ad9114e8b1b9ca';Q4_annex_sha256='590aa1c695adce55d6ba17e9ce508d747090c05c2d61cd44a7ced603cf6c4db4';Q2_record_sha256='5794e17265af6e34d335591b0fed42e5d9048aec81e751578fd9f4132cf524e1';N7_sha256='1d6bca5386e3520034570d17cf6f526fdef4c19b52aee633113e1bd8e5b40fe7';evaluation_plan_sha256='d93cff9c596ad68529fb836f1dc45280af9686828efcf939ac48621a01b4e9e6';q7_activation_authorized=$true;artifacts=@{launcher=@{path=$ExpectedPath;sha256=$ExpectedScriptSha256};qualifier=@{path=$Qualifier;sha256=$QualifierHash};native_resource=@{path=$TransportNative;sha256=$TransportNativeHash};framework_binding=@{path=$TransportBinding;sha256=$TransportBindingHash}}}}
$names=@('valid','Q0_Q9_sha256','Q4_annex_sha256','Q2_record_sha256','N7_sha256','evaluation_plan_sha256','old-attempt','s2','activation','activation-string','activation-array','fresh-approval-absent','wrong-binary','wrong-path','hash-array','boolean','truncated','closed-stream','repeat-call')
foreach($name in $names){$r=Record
 switch($name){
 'Q0_Q9_sha256'{$r.Q0_Q9_sha256='f'*64}'Q4_annex_sha256'{$r.Q4_annex_sha256='f'*64}'Q2_record_sha256'{$r.Q2_record_sha256='f'*64}'N7_sha256'{$r.N7_sha256='f'*64}'evaluation_plan_sha256'{$r.evaluation_plan_sha256='f'*64}
 'old-attempt'{$r.attempt='0b1de869e11493b7cb08fb329578f6ce'}'s2'{$r.operation='Q4P-20261025-01';$r.status='EXACT_MANUAL_PLACEMENT_OWNER_APPROVED'}'activation'{$r.q7_activation_authorized=$false}'activation-string'{$r.q7_activation_authorized='True'}'activation-array'{$r.q7_activation_authorized=@($true,$true)}'fresh-approval-absent'{$r.Remove('request_sha256')}'wrong-binary'{$r.artifacts.qualifier.sha256='f'*64}'wrong-path'{$r.artifacts.qualifier.path='historical'}'hash-array'{$r.N7_sha256=@($r.N7_sha256,$r.N7_sha256)}'boolean'{$r.manual_only='True'}
 }
 $text=$r|ConvertTo-Json -Depth 9;if($name-eq'truncated'){$text=$text.Substring(0,50)};$m=[IO.MemoryStream]::new([Text.Encoding]::UTF8.GetBytes($text),$false);if($name-eq'closed-stream'){$m.Dispose()};$deny=$false
 try{Assert-PositiveApproval $m;if($name-eq'repeat-call'){Assert-PositiveApproval $m;if($m.Position-ne0){throw 'STREAM_POSITION_LOST'}}}catch{$deny=$true}finally{$m.Dispose()}
 if($deny-ne($name-notin@('valid','repeat-call'))){throw ('Q7_APPROVAL_GATE_FAIL '+$name)};$cases++
}
# Inspect that native approval path is independent and original transport/cleanup paths remain.
if($source-notmatch'EXACT-Q7-APPROVED.json'-or$source-match'Q4_BINDING_INCOMPLETE_DENY'){throw 'INTEGRATION_INCOMPLETE'}
Write-Output ('Q7_EXTRACTED_GATES=PASS cases='+$cases+'; NO_LAUNCHER_NO_PREFLIGHT_NO_PROTECTED_READ_NO_APPROVAL_FILE')
