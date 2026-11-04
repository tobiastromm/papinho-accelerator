$ErrorActionPreference='Stop'
# STATIC ONLY. Never invoke/dot-source the manual driver, helpers or updater.
$base=Join-Path $PSScriptRoot 'update-p1-image.MANUAL.CANDIDATE.ps1'
$next=Join-Path $PSScriptRoot 'update-p1-closed-chain.MANUAL.CANDIDATE.ps1'
if((Get-FileHash -LiteralPath $base).Hash -ne '89cb7ef92a5cc7e8d22affe6c0ea366ba69e280865b3b95dd969a29e886b9ad9'){throw 'BASE_PIN_DENY'}
$rules=@(
 @{Old='ONE-P1-IMAGE-UPDATE-REQUEST.CANDIDATE.md';New='ONE-P1-CLOSED-CHAIN-UPDATE-REQUEST.CANDIDATE.md'},
 @{Old='54d374b2fddac81e30b4f8bb574fbaf3446b5a52cbdb002769fe5cdcfe1b4a50';New='9816585b3d240ce59da70769245204378f8e08e67c67f5d5a0b16b729b8d0220'},
 @{Old='7d29d50ca008dc7a92759270388bf62e962d2d343e39390db64bd96f971eca7d';New='54d374b2fddac81e30b4f8bb574fbaf3446b5a52cbdb002769fe5cdcfe1b4a50'},
 @{Old='Path=$data;Bytes=772608;Pin=$newPin';New='Path=$data;Bytes=774656;Pin=$newPin'},
 @{Old='Path=$old;Bytes=763904;Pin=$oldPin';New='Path=$old;Bytes=772608;Pin=$oldPin'},
 @{Old='417614100,1835008,134613';New='417614100,1769472,132345'},
 @{Old='''1835008'',''134613''';New='''1769472'',''132345'''},
 @{Old='ONE_MANUAL_P1_IMAGE_UPDATE';New='ONE_MANUAL_P1_CLOSED_CHAIN_IMAGE_UPDATE'}
)
$expected=[IO.File]::ReadAllText($base)
foreach($rule in $rules){
 if([regex]::Matches($expected,[regex]::Escape($rule.Old)).Count -ne 1){throw 'DELTA_MULTIPLICITY_DENY'}
 $expected=$expected.Replace($rule.Old,$rule.New)
}
if([IO.File]::ReadAllText($next) -cne $expected){throw 'UNAPPROVED_PROCEDURE_DELTA'}
if((Get-FileHash -LiteralPath $next).Hash -ne 'cbfc1290ed5cffaedfacea70442931fe450ed36197b61a21b68872f75283077e'){throw 'PROCEDURE_PIN_DENY'}
$tokens=$null;$errors=$null
$oldAst=[Management.Automation.Language.Parser]::ParseFile($base,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'BASE_AST_DENY'}
$ast=[Management.Automation.Language.Parser]::ParseFile($next,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'NEW_AST_DENY'}
$oldFunctions=@($oldAst.FindAll({param($n) $n -is [Management.Automation.Language.FunctionDefinitionAst]},$true))
$newFunctions=@($ast.FindAll({param($n) $n -is [Management.Automation.Language.FunctionDefinitionAst]},$true))
if($newFunctions.Count -ne 6 -or $oldFunctions.Count -ne $newFunctions.Count){throw 'HELPERS_COUNT_DENY'}
for($i=0;$i-lt$newFunctions.Count;$i++){if($oldFunctions[$i].Extent.Text-cne$newFunctions[$i].Extent.Text){throw 'HELPER_CHANGE_DENY'}}
$calls=@($ast.FindAll({param($n) $n -is [Management.Automation.Language.CommandAst] -and $n.InvocationOperator -eq 'Ampersand'},$true))
if($calls.Count-ne1 -or $calls[0].Extent.Text-cne'& $executor @arguments'){throw 'NATIVE_CALL_DENY'}
foreach($command in $ast.FindAll({param($n) $n -is [Management.Automation.Language.CommandAst]},$true)){
 if($command.GetCommandName()-match'^(Copy-Item|Move-Item|Remove-Item|Start-Process|Invoke-Expression|Start-Service|Stop-Service|Set-Acl|sc|sc.exe)$'){throw 'EXTRA_EFFECT_DENY'}
}
$request=Join-Path $PSScriptRoot 'ONE-P1-CLOSED-CHAIN-UPDATE-REQUEST.CANDIDATE.md'
if((Get-FileHash -LiteralPath $request).Hash-ne'28fee308a5c509e72d842a85d8cd45ebffc028f1bb65c5a878d9cd46cf5670a6'){throw 'REQUEST_PIN_DENY'}
$source='C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release'
$files=@(
 @{Name='papacc_server.exe';Bytes=774656;Pin='9816585b3d240ce59da70769245204378f8e08e67c67f5d5a0b16b729b8d0220'},
 @{Name='libssl-3-x64.dll';Bytes=1321472;Pin='3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'},
 @{Name='libcrypto-3-x64.dll';Bytes=7217664;Pin='09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'}
)
foreach($file in $files){$p=Join-Path $source $file.Name;if((Get-Item -LiteralPath $p).Length-ne$file.Bytes -or (Get-FileHash -LiteralPath $p).Hash-ne$file.Pin){throw ('SOURCE_PIN_DENY '+$file.Name)}}
foreach($profile in @('Selected','Legacy')){foreach($config in @('Debug','Release')){
 $build=Join-Path $PSScriptRoot ('p1-chain-namespace-'+$profile+'-'+$config+'-build.log')
 $test=Join-Path $PSScriptRoot ('p1-chain-namespace-'+$profile+'-'+$config+'-tests.log')
 if(-not(Test-Path -LiteralPath $build)-or(Select-String -LiteralPath $build -Pattern 'warning C\d|error C\d|FAILED:')){throw 'BUILD_EVIDENCE_DENY'}
 if(-not(Select-String -LiteralPath $test -SimpleMatch '100% tests passed, 0 tests failed out of 15')){throw 'TEST_EVIDENCE_DENY'}
}}
if((Get-FileHash -LiteralPath (Join-Path $PSScriptRoot 'OWNER-MANUAL-P1-IMAGE-UPDATE-OUTPUT.txt')).Hash-ne'fa9ab01b30b600590b3b6c33db54a70197628f993bb723af9dd9b6d122289230'){throw 'HISTORICAL_EVIDENCE_DENY'}
$prior=Get-Content -LiteralPath (Join-Path $PSScriptRoot 'P1-IMAGE-UPDATE-RESULT.json') -Raw | ConvertFrom-Json
if($prior.canonical_id-ne'417614100:1769472:132345'-or$prior.canonical_bytes-ne772608-or$prior.K-ne2027314-or$prior.authorization-ne'USED_NO_RETRY'){throw 'HISTORICAL_BINDING_DENY'}
$rounded=[long]([Math]::Ceiling(774656.0/4096.0))*4096
if($rounded-ne778240){throw 'R_ARITHMETIC_DENY'}
$evidence=[ordered]@{
 task='P1_CLOSED_CHAIN_UPDATE_PREPARATION';status='READY_FOR_OWNER_SELECTION_AND_OPERATIONAL_AUTHORIZATION';
 procedure_ast='PASS';exact_delta_rules=$rules.Count;unchanged_helpers=$newFunctions.Count;future_native_calls=$calls.Count;
 procedure_executed=$false;native_helpers_executed=$false;installed_objects_queried=$false;protected_effects=0;
 request_sha256=(Get-FileHash -LiteralPath $request).Hash.ToLowerInvariant();
 procedure_sha256=(Get-FileHash -LiteralPath $next).Hash.ToLowerInvariant();
 payload_bytes=774656;payload_sha256=$files[0].Pin;DLLs='UNCHANGED_SOURCE_PINS_CONFIRMED';
 prior_canonical_id=$prior.canonical_id;prior_canonical_sha256=$prior.canonical_sha256;
 executor_id=$prior.executor_id;executor_sha256=$prior.executor_sha256;executor_K_historical=$prior.K;
 R_example_cluster4096=$rounded;source_builds_reused=4;source_ctest_pairs_reused=60;builds_rerun=0;ctests_rerun=0;
 installed_state='HISTORICAL_EXPECTATIONS_FRESH_GATES_PENDING';historical_retention='UNKNOWN';os_quiescence='UNKNOWN';
 operational_authority='DENY';native_P1_chain='NOT_OBSERVED';Start='NOT_AUTHORIZED'
}
[IO.File]::WriteAllText((Join-Path $PSScriptRoot 'P1-CLOSED-CHAIN-UPDATE-OFFLINE-EVIDENCE.json'),($evidence|ConvertTo-Json -Depth 5),[Text.UTF8Encoding]::new($false))
'P1_CLOSED_CHAIN_UPDATE_STATIC_PASS; 8_DELTA_RULES_6_UNCHANGED_HELPERS_1_FUTURE_NATIVE_CALL; NO_PROCEDURE_EXECUTED'
