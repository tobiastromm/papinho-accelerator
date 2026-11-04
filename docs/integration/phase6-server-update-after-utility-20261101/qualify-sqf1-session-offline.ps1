# Offline bindings/model only: never invoke a driver, native constructor,
# Prepare/Create or product. No real originals, SCM, endpoint or Start.
$ErrorActionPreference='Stop'
$folder=$PSScriptRoot
$manifestPath=Join-Path $folder 'SQF1-SESSION-ARTIFACT-BINDINGS.json'
if((Get-FileHash -LiteralPath $manifestPath).Hash-ne'776e9c59f5717b1cf01e780351815fa79cf8057a892756b578bdc88c6a6b9f72'){throw 'BINDINGS_PIN_DENY'}
$manifest=ConvertFrom-Json -InputObject ([IO.File]::ReadAllText($manifestPath))
if($manifest.Count-ne6){throw 'BINDINGS_COUNT_DENY'}
$allowed=@('sqf1-session-originals.cs','SQF1-ACT-ENDPOINT.REQUEST.CANDIDATE.md','SQF1-ACT-SCQ.REQUEST.CANDIDATE.md','SQF1-ACT-START.REQUEST.CANDIDATE.md','prepare-sqf1-originals.MANUAL.CANDIDATE.ps1','run-sqf1-first-failure-diagnostic.MANUAL.CANDIDATE.ps1')
foreach($binding in $manifest){
 if($allowed-cnotcontains$binding.artifact){throw 'BINDING_SCOPE_DENY'}
 $base=Join-Path $folder $binding.base;$next=Join-Path $folder $binding.artifact
 if((Get-FileHash -LiteralPath $base).Hash-ne$binding.base_sha256-or(Get-FileHash -LiteralPath $next).Hash-ne$binding.sha256){throw 'ARTIFACT_PIN_DENY'}
 $expected=[IO.File]::ReadAllText($base)
 foreach($rule in $binding.rules){
  if([regex]::Matches($expected,[regex]::Escape($rule.Old)).Count-ne$rule.Count){throw 'DELTA_MULTIPLICITY_DENY'}
  $expected=$expected.Replace($rule.Old,$rule.New)
 }
 if([IO.File]::ReadAllText($next)-cne$expected){throw 'ARTIFACT_EXACT_DELTA_DENY'}
}
if($manifest[0].rules.Count-ne3-or$manifest[0].rules[0].Count-ne1-or$manifest[0].rules[1].Count-ne1-or$manifest[0].rules[2].Count-ne2){throw 'ADAPTER_NAME_ONLY_DELTA_DENY'}
$source=[IO.File]::ReadAllText((Join-Path $folder 'sqf1-session-originals.cs'))
$modelPath=Join-Path $folder 'first-start-originals.model.cs'
if((Get-FileHash -LiteralPath $modelPath).Hash-ne'a20a8a6173104ce41d3115833a167b003dfb3bba017fcd9c69c3a15f5cae433f'){throw 'MODEL_PIN_DENY'}
$model=[IO.File]::ReadAllText($modelPath)
$usings=@([regex]::Matches($source+"`n"+$model,'(?m)^using [A-Za-z0-9_.]+;')|ForEach-Object {$_.Value}|Sort-Object -Unique)
$body=[regex]::Replace($source+"`n"+$model,'(?m)^using [A-Za-z0-9_.]+;','')
$compiler=[CodeDom.Compiler.CompilerParameters]::new()
$compiler.GenerateInMemory=$true;$compiler.WarningLevel=4;$compiler.TreatWarningsAsErrors=$true
[void]$compiler.ReferencedAssemblies.Add('System.dll');[void]$compiler.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition (($usings-join"`n")+"`n"+$body) -CompilerParameters $compiler
$modelResult=[FirstStartDocumentsModel]::Run()
$asts=@{}
foreach($name in @('prepare-sqf1-originals.MANUAL.CANDIDATE.ps1','run-sqf1-first-failure-diagnostic.MANUAL.CANDIDATE.ps1')){
 $tokens=$null;$errors=$null
 $asts[$name]=[Management.Automation.Language.Parser]::ParseFile((Join-Path $folder $name),[ref]$tokens,[ref]$errors)
 if($errors.Count){throw 'MANUAL_AST_DENY'}
 foreach($call in $asts[$name].FindAll({param($n) $n -is [Management.Automation.Language.CommandAst]},$true)){
  if($call.GetCommandName()-match'^(Start-Service|Stop-Service|Restart-Service|Set-Acl|Copy-Item|Remove-Item|Move-Item|sc|sc.exe|Invoke-Expression|Start-Process)$'){throw 'EXTRA_EFFECT_DENY'}
 }
}
$guard=@($asts['prepare-sqf1-originals.MANUAL.CANDIDATE.ps1'].FindAll({param($n) $n -is [Management.Automation.Language.FunctionDefinitionAst] -and $n.Name-ceq'Assert-P1OriginalPins'},$true))
if($guard.Count-ne1){throw 'GUARD_COUNT_DENY'}
# Only this pure string guard is defined/invoked. Never the operational wrapper.
. ([scriptblock]::Create($guard[0].Extent.Text))
$requestPath=Join-Path $folder 'ONE-SQF1-THREE-ACTS-REQUEST.CANDIDATE.md'
$request=(Get-FileHash -LiteralPath $requestPath).Hash.ToLowerInvariant()
if($request-cne'c66738655b58f16697e40e914cf83f217e73d667f5e08070be75c12406aaa177'){throw 'REQUEST_PIN_DENY'}
$guardCases=0
for($case=0;$case-lt44;$case++){
 $r=$request;$decision='2'*64
 if($case-eq1){$r='312999dd9ba4f701991560e65b4a25e3a13c7914bc75603a6efb47a11536798d'}
 if($case-eq2){$decision='7f6451d59fc0b7015e0ac612ee471782950880fcfc2013137dc9643276af641a'}
 if($case-eq3){$decision='c6c34866050bc0da189acd8724470838909bc7c790b838e23c46cb86ef58210f'}
 if($case-eq4){$decision='ef8b47fa4dba37224bef904488ff0494db4e5804c1e696463ddc0dd864f3e199'}
 if($case-eq5){$r='af8529db7f447fa6af6fc462fcb3cba4583abfc41f2e867c255b801974a67bc9'}
 if($case-eq6){$r='25ea33a28c714952e1b1fb42202e64be7ca933afe0fa7aa37d2a5e654918dff4'}
 if($case-eq7){$decision=$r}
 if($case-eq8){$decision='bad'}
 if($case-eq9){$r='86303aac1bf3b040a5949e8947a2bee2cb157fdbde472f31d5f2830b469a2a34'}
 if($case-eq10){$decision='a7d3544dd19c0639e9799486efe70b976d3ab543de0d11ab15edd2d424148607'}
 if($case-eq11){$r='608b6ca768dd35f35c827ee29498a2bdf5796cc5a3d66843a8568fa002934441'}
 if($case-eq12){$decision='f54d0bf3c6c609d5e7f93d348d4332364fdee0edc1592ea2a30137ba31ead0e1'}
 if($case-eq13){$r='16e0693541eed115886c59092ebac8f5b32dfe68ef2a893b5fda5d49fd7fb99a'}
 if($case-eq14){$decision='019222b9ea2c4e2e0889639098c63ff5187b04de96b8ddc768a0b9fcd437b28b'}
 if($case-eq15){$r='e3c579ff64a62aabedb34bd06d914f50184bdadeca7f4b09a28c893aab595f69'}
 if($case-eq16){$decision='780ac91743c26eff3ac4497b35e582321beff58adc02aae456ff2aa0ac44bac4'}
 if($case-eq17){$r='bd8bd383513415fa21470f8d324f985e0343cf651c53fc3204f665b6d0744447'}
 if($case-eq18){$decision='41824a586fe583d9f26969c01fbaa2e0061f7865a92d2ea0ef4820a042912c4c'}
 if($case-eq19){$r='cce1bb4ce0a0f47ab19d2506cbf5908564949bb9a1107cba61f085017b37cbaa'}
 if($case-eq20){$decision='ebbde1881439fde75d21219130d1e7660e432457e94ac6d268173a014f1962fb'}
 if($case-eq21){$r='59a5a75925b52590d0db90eb0bdb436cc7dd9ffe1a3cb0a81200b21ff53c399a'}
 if($case-eq22){$decision='7f2fff6e3e855d156fa5c14ef0fe1f814e7d81c9cb5c782bd18f3c8be6859f15'}
 if($case-eq23){$r='f87d9281c4f9926fdc04af50006b48ac881d9dc466c392d475e941fe54299d12'}
 if($case-eq24){$decision='719b44fe82316f382b7849e64cf7ae2b5dca22379d39bf91ba1e485ddb144648'}
 if($case-eq25){$r='90b68869c2476b6246ed34de71e8225f5740cbf9f3dcecf3983dacd18188befd'}
 if($case-eq26){$r='2a34a063563cd9fdf12ec5742b6c6d45ede377fa4501b104779c33205e31cfc0'}
 if($case-eq27){$decision='59f2e58d663bea8646f40d8e3a08c0ec5bddea9e5bfe48774464771658c792a5'}
 if($case-eq28){$r='8ee498d23e5c306b746f1e32e246d37a0ee4e50df5081842a15da2274200ae54'}
 if($case-eq29){$decision='ff573574ea963eaa300f4d3aa4b4699ab76db416753118945f5a1742ea2c3ef4'}
 if($case-eq30){$r='b2ae05445cc49e19b56d997a2c248db575796ce5ee7896108c862ffd6617bd7e'}
 if($case-eq31){$decision='f3a03914a670bf6fd265997519a21fc672e89bc12bc9fd552db61a36ee25beca'}
 if($case-eq32){$r='b275975034b33477a90157a0f55bc67e5bd9482abdf21c1c967f384e0cbfb896'}
 if($case-eq33){$decision='af606af323918f1bc9362a1de6ad5eff22fe515315b5b5a0ea45aa524abf5223'}
 if($case-eq34){$r='ed404e4cad579acc4e91cd2fae727f73bbe1629ce9d445d16f3c2035c6f6f018'}
 if($case-eq35){$decision='00539879827aa7593600d4023e1b784f7863d56de692668a0e8b3a98ea7ee68f'}
 if($case-eq36){$r='9249509a3a942554defa8142bb6b78252e1c5ff9cc39db7bb11082548867653b'}
 if($case-eq37){$decision='2161c0f085438c1e217d30063df9b5856d3e01733b2d33a8e29a3c1f6891dd69'}
 if($case-eq38){$r='e7627530423dd073c96e836ba22032862b4da44f965302fff87e11fe7569c0d2'}
 if($case-eq39){$decision='d3c4ce1dc22b8c9b01ec8c0db2dc453c98a8d489e7556407dc41ccb74305b8af'}
 if($case-eq40){$r='5bf09792d603754b94e9117cb6514895081f16414c48482e731108ad6ba29618'}
 if($case-eq41){$decision='a9253f2243c30b4de2f6f480a0c5df79dd48ba7c6deed27243963a8e2893e37b'}
 if($case-eq42){$r='db62ad040b42e49a568fcdd91a816abc2c29a97ed324a93e0935ab75d2587a2f'}
 if($case-eq43){$decision='96442649fdaa821b0c64d970983f56ba3f51fe0ac1b3195fe35167a131fc501d'}
 $denied=$false;try{Assert-P1OriginalPins $r $decision}catch{if($_.Exception.Message-ne'P1_ORIGINAL_SCOPE_DENY'){throw};$denied=$true}
 if($denied-ne($case-ne0)){throw 'GUARD_NEGATIVE_DENY'};$guardCases++
}
$driverPath=Join-Path $folder 'run-sqf1-first-failure-diagnostic.MANUAL.CANDIDATE.ps1'
$driver=[IO.File]::ReadAllText($driverPath)
$calls=@($asts['run-sqf1-first-failure-diagnostic.MANUAL.CANDIDATE.ps1'].FindAll({param($n) $n -is [Management.Automation.Language.CommandAst] -and $n.InvocationOperator-eq'Ampersand'},$true))
if($calls.Count-ne2-or$calls[1].Extent.Text-cne'& $sessionImage @sessionArguments'-or-not$calls[0].Extent.Text.Contains('-File $pairProcedure')){throw 'DRIVER_CALLS_DENY'}
if(-not$driver.Contains("'--owner-manual-first-scq-start-to-existing'")-or-not$driver.Contains("'1994944','3145728','7239172096','3221225472','192','100','60000','240000'")){throw 'ENTRY_G3_DENY'}
if(-not$driver.Contains('417614100:2883584:329728')-or$driver.Contains('417614100:1769472:132345')){throw 'IMAGE_SELECTION_DENY'}
$canonical=(Get-Content -LiteralPath (Join-Path $folder 'SQF1-IMAGE-UPDATE-RESULT.json') -Raw|ConvertFrom-Json)
if($canonical.authorization-ne'USED_NO_RETRY'-or$canonical.canonical_id-ne'417614100:2883584:329728'-or$canonical.canonical_bytes-ne809472-or$canonical.canonical_sha256-ne'81a849dc51bf2e7fd0455c09f30c926329ef4c30196b36747f1429e5fdecee86'){throw 'CANONICAL_BASELINE_DENY'}
if((Get-FileHash -LiteralPath (Join-Path $folder $canonical.evidence)).Hash-ne$canonical.evidence_sha256){throw 'CANONICAL_EVIDENCE_PIN_DENY'}
if(Test-Path -LiteralPath (Join-Path $folder 'OWNER-SQF1-THREE-ACTS-APPROVAL.txt')){throw 'UNEXPECTED_DECISION_EXISTS'}
$requestBytes=(Get-Item -LiteralPath $requestPath).Length
if($requestBytes-gt65536){throw 'DOCUMENT_BOUND_DENY'}
foreach($profile in @('Selected','Legacy')){foreach($config in @('Debug','Release')){
 $build=Join-Path $folder ('sqf1-'+$profile+'-'+$config+'-build.log')
 $test=Join-Path $folder ('sqf1-'+$profile+'-'+$config+'-tests.log')
 if((Select-String -LiteralPath $build -Pattern 'warning C\d|error C\d|FAILED:')-or-not(Select-String -LiteralPath $test -SimpleMatch '100% tests passed, 0 tests failed out of 20')){throw 'IMAGE_QUALIFICATION_EVIDENCE_DENY'}
}}
$result=[ordered]@{
 status='OFFLINE_QUALIFIED_FOR_OWNER_DECISION_NOT_AUTHORIZED';request_sha256=$request;request_bytes=$requestBytes;
 artifact_bindings='PASS_SIX_EXACT_TRANSFORMS';adapter_name_only_delta=4;adapter_model=$modelResult;pure_scope_guard_cases=$guardCases;
 wrappers_ast='PASS';future_driver_invocations=2;g3_p1_accepted_applied=$true;manual_wrappers_invoked=$false;
 native_constructor_or_file_api_invoked=$false;protected_originals_created=0;endpoint_or_scm_invoked=$false;
 owner_decision_emitted=$false;canonical_id=$canonical.canonical_id;canonical_sha256=$canonical.canonical_sha256;
 installed_objects_queried=$false;product_builds_reused=4;product_ctest_pairs_reused=80;baseline_sqb3_ctest_pairs_reused=0;product_builds_rerun=0;product_ctests_rerun=0;
 historical_retention='UNKNOWN';os_quiescence='UNKNOWN';start_admission='UNPROVEN_FRESH_GATES_PENDING';productive_readiness='DENY'
}
[IO.File]::WriteAllText((Join-Path $folder 'SQF1-SESSION-OFFLINE-EVIDENCE.json'),($result|ConvertTo-Json -Depth 5),[Text.UTF8Encoding]::new($false))
$modelResult
'PASS P1_CHAIN_SCOPE cases=44; SIX_BINDINGS_AST_G3_PASS; NO_NATIVE_DOCUMENT_PRODUCT_OR_OPERATION'
