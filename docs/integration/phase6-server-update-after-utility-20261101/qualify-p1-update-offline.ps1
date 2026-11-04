$ErrorActionPreference='Stop'
# STATIC ONLY: never invoke/dot-source the manual procedure or native helpers.
$base=Join-Path $PSScriptRoot 'update-cold-diagnostic.MANUAL.CANDIDATE.ps1'
$next=Join-Path $PSScriptRoot 'update-p1-image.MANUAL.CANDIDATE.ps1'
if((Get-FileHash -LiteralPath $base).Hash -ne 'e81f1e410cb7c6f87f9ae6a5a166a39994559f7ca622179f95a32cc432e503ae'){throw 'BASE_PIN_DENY'}
$rules=@(
 @{Old='ONE-COLD-DIAGNOSTIC-UPDATE-REQUEST.CANDIDATE.md';New='ONE-P1-IMAGE-UPDATE-REQUEST.CANDIDATE.md'},
 @{Old='7d29d50ca008dc7a92759270388bf62e962d2d343e39390db64bd96f971eca7d';New='54d374b2fddac81e30b4f8bb574fbaf3446b5a52cbdb002769fe5cdcfe1b4a50'},
 @{Old='3d59007bdc0714eb89eb646479e7a23d52fd82c953f1c72deec541a3643bce0b';New='7d29d50ca008dc7a92759270388bf62e962d2d343e39390db64bd96f971eca7d'},
 @{Old='Path=$data;Bytes=763904;Pin=$newPin';New='Path=$data;Bytes=772608;Pin=$newPin'},
 @{Old='Path=$old;Bytes=762880;Pin=$oldPin';New='Path=$old;Bytes=763904;Pin=$oldPin'},
 @{Old='417614100,1900544,539611';New='417614100,1835008,134613'},
 @{Old='''1900544'',''539611''';New='''1835008'',''134613'''},
 @{Old='ONE_MANUAL_COLD_DIAGNOSTIC_IMAGE_UPDATE';New='ONE_MANUAL_P1_IMAGE_UPDATE'}
)
$expected=[IO.File]::ReadAllText($base)
foreach($rule in $rules){
 if([regex]::Matches($expected,[regex]::Escape($rule.Old)).Count -ne 1){throw 'DELTA_RULE_MULTIPLICITY_DENY'}
 $expected=$expected.Replace($rule.Old,$rule.New)
}
if([IO.File]::ReadAllText($next) -cne $expected){throw 'UNAPPROVED_PROCEDURE_DELTA'}
$tokens=$null;$parseErrors=$null
$oldAst=[Management.Automation.Language.Parser]::ParseFile($base,[ref]$tokens,[ref]$parseErrors)
if($parseErrors.Count){throw 'BASE_AST_DENY'}
$newAst=[Management.Automation.Language.Parser]::ParseFile($next,[ref]$tokens,[ref]$parseErrors)
if($parseErrors.Count){throw 'NEW_AST_DENY'}
$oldFunctions=@($oldAst.FindAll({param($n) $n -is [Management.Automation.Language.FunctionDefinitionAst]},$true))
$newFunctions=@($newAst.FindAll({param($n) $n -is [Management.Automation.Language.FunctionDefinitionAst]},$true))
if($oldFunctions.Count -ne $newFunctions.Count){throw 'HELPER_COUNT_DENY'}
for($i=0;$i -lt $oldFunctions.Count;$i++){
 if($oldFunctions[$i].Extent.Text -cne $newFunctions[$i].Extent.Text){throw 'HELPER_CHANGE_DENY'}
}
$calls=@($newAst.FindAll({param($n) $n -is [Management.Automation.Language.CommandAst] -and $n.InvocationOperator -eq 'Ampersand'},$true))
if($calls.Count -ne 1 -or $calls[0].Extent.Text -cne '& $executor @arguments'){throw 'NATIVE_CALL_DENY'}
foreach($command in $newAst.FindAll({param($n) $n -is [Management.Automation.Language.CommandAst]},$true)){
 if($command.GetCommandName() -match '^(Copy-Item|Move-Item|Remove-Item|Start-Process|Invoke-Expression|Start-Service|Stop-Service|Set-Acl|sc|sc.exe)$'){throw 'EXTRA_EFFECT_DENY'}
}
$payload='C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe'
if((Get-Item -LiteralPath $payload).Length -ne 772608 -or (Get-FileHash -LiteralPath $payload).Hash -ne '54d374b2fddac81e30b4f8bb574fbaf3446b5a52cbdb002769fe5cdcfe1b4a50'){throw 'PAYLOAD_PIN_DENY'}
$request=Join-Path $PSScriptRoot 'ONE-P1-IMAGE-UPDATE-REQUEST.CANDIDATE.md'
if((Get-FileHash -LiteralPath $request).Hash -ne '9a2d514469efb1c1ccc02847ac02e60e54de740ac72aae5c9c84c39f93306e24'){throw 'REQUEST_PIN_DENY'}
$result=[ordered]@{
 task='P1_QUALIFIED_IMAGE_UPDATE_PREPARATION';procedure_ast='PASS';exact_delta_rules=$rules.Count;
 unchanged_helpers=$oldFunctions.Count;native_invocations_in_future_procedure=$calls.Count;
 procedure_executed=$false;native_helpers_executed=$false;installed_objects_queried=$false;
 payload_bytes=772608;payload_sha256=(Get-FileHash -LiteralPath $payload).Hash.ToLowerInvariant();
 procedure_sha256=(Get-FileHash -LiteralPath $next).Hash.ToLowerInvariant();
 request_sha256=(Get-FileHash -LiteralPath $request).Hash.ToLowerInvariant();
 source_builds_reused=4;source_ctest_pairs_reused=60;builds_rerun=0;ctests_rerun=0;
 executor_K_historical=2027314;R_example_cluster4096=774144;
 installed_state='HISTORICAL_EXPECTATIONS_FRESH_GATES_PENDING';historical_retention='UNKNOWN';os_quiescence='UNKNOWN';
 operational_authority='DENY';protected_effects=0
}
[IO.File]::WriteAllText((Join-Path $PSScriptRoot 'P1-IMAGE-UPDATE-OFFLINE-EVIDENCE.json'),($result|ConvertTo-Json -Depth 5),[Text.UTF8Encoding]::new($false))
'P1_UPDATE_STATIC_QUALIFICATION_PASS; NO_PROCEDURE_OR_NATIVE_HELPER_EXECUTION'
