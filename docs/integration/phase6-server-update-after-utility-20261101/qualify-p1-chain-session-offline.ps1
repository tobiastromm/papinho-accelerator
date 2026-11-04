# Offline bindings/model only: never invoke a driver, native constructor,
# Prepare/Create or product. No real originals, SCM, endpoint or Start.
$ErrorActionPreference='Stop'
$folder=$PSScriptRoot
$manifestPath=Join-Path $folder 'P1-CHAIN-SESSION-ARTIFACT-BINDINGS.json'
if((Get-FileHash -LiteralPath $manifestPath).Hash-ne'784a2af93b990491eb3dd02a99c25584dca036e507ff80f190c5a803ddfe4426'){throw 'BINDINGS_PIN_DENY'}
$manifest=ConvertFrom-Json -InputObject ([IO.File]::ReadAllText($manifestPath))
if($manifest.Count-ne6){throw 'BINDINGS_COUNT_DENY'}
$allowed=@('p1-chain-session-originals.cs','P1-CHAIN-ACT-ENDPOINT.REQUEST.CANDIDATE.md','P1-CHAIN-ACT-SCQ.REQUEST.CANDIDATE.md','P1-CHAIN-ACT-START.REQUEST.CANDIDATE.md','prepare-p1-chain-originals.MANUAL.CANDIDATE.ps1','run-p1-chain-diagnostic.MANUAL.CANDIDATE.ps1')
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
if($manifest[0].rules.Count-ne4){throw 'ADAPTER_NAME_ONLY_DELTA_DENY'}
$source=[IO.File]::ReadAllText((Join-Path $folder 'p1-chain-session-originals.cs'))
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
foreach($name in @('prepare-p1-chain-originals.MANUAL.CANDIDATE.ps1','run-p1-chain-diagnostic.MANUAL.CANDIDATE.ps1')){
 $tokens=$null;$errors=$null
 $asts[$name]=[Management.Automation.Language.Parser]::ParseFile((Join-Path $folder $name),[ref]$tokens,[ref]$errors)
 if($errors.Count){throw 'MANUAL_AST_DENY'}
 foreach($call in $asts[$name].FindAll({param($n) $n -is [Management.Automation.Language.CommandAst]},$true)){
  if($call.GetCommandName()-match'^(Start-Service|Stop-Service|Restart-Service|Set-Acl|Copy-Item|Remove-Item|Move-Item|sc|sc.exe|Invoke-Expression|Start-Process)$'){throw 'EXTRA_EFFECT_DENY'}
 }
}
$guard=@($asts['prepare-p1-chain-originals.MANUAL.CANDIDATE.ps1'].FindAll({param($n) $n -is [Management.Automation.Language.FunctionDefinitionAst] -and $n.Name-ceq'Assert-P1OriginalPins'},$true))
if($guard.Count-ne1){throw 'GUARD_COUNT_DENY'}
# Only this pure string guard is defined/invoked. Never the operational wrapper.
. ([scriptblock]::Create($guard[0].Extent.Text))
$requestPath=Join-Path $folder 'ONE-P1-CHAIN-THREE-ACTS-REQUEST.CANDIDATE.md'
$request=(Get-FileHash -LiteralPath $requestPath).Hash.ToLowerInvariant()
if($request-cne'608b6ca768dd35f35c827ee29498a2bdf5796cc5a3d66843a8568fa002934441'){throw 'REQUEST_PIN_DENY'}
$guardCases=0
for($case=0;$case-lt11;$case++){
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
 $denied=$false;try{Assert-P1OriginalPins $r $decision}catch{if($_.Exception.Message-ne'P1_ORIGINAL_SCOPE_DENY'){throw};$denied=$true}
 if($denied-ne($case-ne0)){throw 'GUARD_NEGATIVE_DENY'};$guardCases++
}
$driverPath=Join-Path $folder 'run-p1-chain-diagnostic.MANUAL.CANDIDATE.ps1'
$driver=[IO.File]::ReadAllText($driverPath)
$calls=@($asts['run-p1-chain-diagnostic.MANUAL.CANDIDATE.ps1'].FindAll({param($n) $n -is [Management.Automation.Language.CommandAst] -and $n.InvocationOperator-eq'Ampersand'},$true))
if($calls.Count-ne2-or$calls[1].Extent.Text-cne'& $sessionImage @sessionArguments'-or-not$calls[0].Extent.Text.Contains('-File $pairProcedure')){throw 'DRIVER_CALLS_DENY'}
if(-not$driver.Contains("'--owner-manual-first-scq-start-to-existing'")-or-not$driver.Contains("'1994944','1994944','7239172096','9823326208','128','100','60000','240000'")){throw 'ENTRY_G3_DENY'}
if(-not$driver.Contains('417614100:1638400:813992')-or$driver.Contains('417614100:1769472:132345')){throw 'IMAGE_SELECTION_DENY'}
$canonical=(Get-Content -LiteralPath (Join-Path $folder 'P1-CLOSED-CHAIN-UPDATE-RESULT.json') -Raw|ConvertFrom-Json)
if($canonical.authorization-ne'USED_NO_RETRY'-or$canonical.canonical_id-ne'417614100:1638400:813992'-or$canonical.canonical_bytes-ne774656-or$canonical.canonical_sha256-ne'9816585b3d240ce59da70769245204378f8e08e67c67f5d5a0b16b729b8d0220'){throw 'CANONICAL_BASELINE_DENY'}
if((Get-FileHash -LiteralPath (Join-Path $folder $canonical.evidence)).Hash-ne$canonical.evidence_sha256){throw 'CANONICAL_EVIDENCE_PIN_DENY'}
$requestBytes=(Get-Item -LiteralPath $requestPath).Length
if($requestBytes-gt65536){throw 'DOCUMENT_BOUND_DENY'}
foreach($profile in @('Selected','Legacy')){foreach($config in @('Debug','Release')){
 $build=Join-Path $folder ('p1-chain-namespace-'+$profile+'-'+$config+'-build.log')
 $test=Join-Path $folder ('p1-chain-namespace-'+$profile+'-'+$config+'-tests.log')
 if((Select-String -LiteralPath $build -Pattern 'warning C\d|error C\d|FAILED:')-or-not(Select-String -LiteralPath $test -SimpleMatch '100% tests passed, 0 tests failed out of 15')){throw 'IMAGE_QUALIFICATION_EVIDENCE_DENY'}
}}
$result=[ordered]@{
 status='OFFLINE_QUALIFIED_FOR_OWNER_DECISION_NOT_AUTHORIZED';request_sha256=$request;request_bytes=$requestBytes;
 artifact_bindings='PASS_SIX_EXACT_TRANSFORMS';adapter_name_only_delta=4;adapter_model=$modelResult;pure_scope_guard_cases=$guardCases;
 wrappers_ast='PASS';future_driver_invocations=2;g3_unchanged=$true;manual_wrappers_invoked=$false;
 native_constructor_or_file_api_invoked=$false;protected_originals_created=0;endpoint_or_scm_invoked=$false;
 owner_decision_emitted=$false;canonical_id=$canonical.canonical_id;canonical_sha256=$canonical.canonical_sha256;
 installed_objects_queried=$false;product_builds_reused=4;product_ctest_pairs_reused=60;product_builds_rerun=0;product_ctests_rerun=0;
 historical_retention='UNKNOWN';os_quiescence='UNKNOWN';start_admission='UNPROVEN_FRESH_GATES_PENDING';productive_readiness='DENY'
}
[IO.File]::WriteAllText((Join-Path $folder 'P1-CHAIN-SESSION-OFFLINE-EVIDENCE.json'),($result|ConvertTo-Json -Depth 5),[Text.UTF8Encoding]::new($false))
$modelResult
'PASS P1_CHAIN_SCOPE cases=11; SIX_BINDINGS_AST_G3_PASS; NO_NATIVE_DOCUMENT_PRODUCT_OR_OPERATION'
