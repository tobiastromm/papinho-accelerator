# Pure gate tests: functions/guards extracted from canonical source. No launcher invocation.
$ErrorActionPreference='Stop';Set-StrictMode -Version Latest
$root='C:\Projetos\PapinhoAccelerator';$p=$root+'\docs\integration\n0start03-integrated-closeout-20261024\t10-t14-positive-integration'
$source=[IO.File]::ReadAllText($root+'\tools\qualification\scm-n0-protected-single-powershell.ps1')
$tokens=$null;$errors=$null;$ast=[Management.Automation.Language.Parser]::ParseInput($source,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'AST_PARSE_DENY'}
$f=$ast.FindAll({param($node)$node-is[Management.Automation.Language.FunctionDefinitionAst]-and$node.Name-eq'Assert-PositiveApproval'},$true)
if($f.Count-ne1){throw 'APPROVAL_FUNCTION_BOUNDARY'}
. ([scriptblock]::Create($f[0].Extent.Text))
$begin=$source.IndexOf(' if($Profile-cne');$end=$source.IndexOf(' if($Mode-eq''Prepare''',$begin)
if($begin-lt0-or$end-le$begin){throw 'PROFILE_BOUNDARY'}
$gate=[scriptblock]::Create($source.Substring($begin,$end-$begin));$cases=0
foreach($Profile in @('EMPTY_DENY','N0START03ReadOnly','N0START03Positive')){foreach($Mode in @('Review','Prepare','Cycle')){
 $denied=$false;try{. $gate}catch{$denied=$true}
 $wanted=($Profile-eq'EMPTY_DENY')-or($Profile-eq'N0START03ReadOnly'-and$Mode-eq'Cycle')-or($Profile-eq'N0START03Positive'-and$Mode-eq'Prepare')
 if($denied-ne$wanted){throw ('PROFILE_GATE_FAIL '+$Profile+' '+$Mode)};$cases++
}}
$Operation='N0START-20261024-03';$Attempt='0b1de869e11493b7cb08fb329578f6ce';$T10DecisionHash='1f293812e9fca107c7fa8ba20abe20fd8ffbca3cdf6e95e58465a95dadbe4dd5'
$ExpectedPath='fixture-script';$Qualifier='fixture-qualifier';$TransportNative='fixture-native';$TransportBinding='fixture-binding'
$ExpectedScriptSha256='a'*64;$QualifierHash='b'*64;$TransportNativeHash='c'*64;$TransportBindingHash='d'*64
function Record {
 return [ordered]@{status='EXACT_MANUAL_POSITIVE_OWNER_APPROVED';operation=$Operation;attempt=$Attempt;mode='Cycle';manual_only=$true;automatic_execution=$false;request_sha256=('e'*64);T10_T14_sha256=$T10DecisionHash;artifacts=@{launcher=@{path=$ExpectedPath;sha256=$ExpectedScriptSha256};qualifier=@{path=$Qualifier;sha256=$QualifierHash};native_resource=@{path=$TransportNative;sha256=$TransportNativeHash};framework_binding=@{path=$TransportBinding;sha256=$TransportBindingHash}}}
}
foreach($case in @('valid','readonly','old-attempt','operation','mode','manual','automatic','policy','request','script','qualifier','native','binding','path','missing','array','bool-string')) {
 $r=Record
 switch($case){'readonly'{$r.status='EXACT_MANUAL_READONLY_OWNER_APPROVED'}'old-attempt'{$r.attempt='75330d11cc7e4123ae62eadc9a59c2f5'}'operation'{$r.operation='N0START-20261024-02'}'mode'{$r.mode='Prepare'}'manual'{$r.manual_only=$false}'automatic'{$r.automatic_execution=$true}'policy'{$r.T10_T14_sha256='f'*64}'request'{$r.request_sha256=''}'script'{$r.artifacts.launcher.sha256='f'*64}'qualifier'{$r.artifacts.qualifier.sha256='f'*64}'native'{$r.artifacts.native_resource.sha256='f'*64}'binding'{$r.artifacts.framework_binding.sha256='f'*64}'path'{$r.artifacts.qualifier.path='other'}'missing'{$r.Remove('mode')}'array'{$r.status=@('EXACT_MANUAL_POSITIVE_OWNER_APPROVED','EXACT_MANUAL_POSITIVE_OWNER_APPROVED')}'bool-string'{$r.manual_only='True'}}
 $bytes=[Text.UTF8Encoding]::new($false,$true).GetBytes(($r|ConvertTo-Json -Depth 8));$memory=[IO.MemoryStream]::new($bytes,$false);$denied=$false
 try{Assert-PositiveApproval $memory}catch{$denied=$true}finally{$memory.Dispose()}
 if($denied-ne($case-ne'valid')){throw ('RECORD_GATE_FAIL '+$case)};$cases++
}
$empty=[IO.MemoryStream]::new();try{$denied=$false;try{Assert-PositiveApproval $empty}catch{$denied=$true};if(-not$denied){throw 'EMPTY_ACCEPTED'};$cases++}finally{$empty.Dispose()}
Write-Output ('POSITIVE_LAUNCHER_GATES=PASS cases='+$cases+'; NO_NATIVE_ENTRY_NO_PROTECTED_QUERY_NO_APPROVAL_FILE')
