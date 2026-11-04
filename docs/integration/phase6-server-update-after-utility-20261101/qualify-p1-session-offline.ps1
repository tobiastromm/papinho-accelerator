# Offline adapter model + static wrappers only. NEVER invoke a manual wrapper,
# native constructor/Prepare/Create, product, endpoint or SCM.
$ErrorActionPreference='Stop'
$folder=$PSScriptRoot
$base=[IO.File]::ReadAllText((Join-Path $folder 'first-start-originals.cs'))
if((Get-FileHash (Join-Path $folder 'first-start-originals.cs')).Hash -ne '18719507fafe6ea6fb81f73dc688410ce7531dfb6074c331de123e0441878f5f'){throw 'BASE_ADAPTER_PIN_DENY'}
$expected=$base.Replace('FIRST-START-SESSION-REQUEST.DRAFT.md','ONE-P1-THREE-ACTS-REQUEST.CANDIDATE.md').Replace('OWNER-FIRST-START-APPROVAL.txt','OWNER-P1-THREE-ACTS-APPROVAL.txt').Replace('FIRST-START-SESSION-REQUEST.ORIGINAL.md','FIRST-P1-SESSION-REQUEST.ORIGINAL.md').Replace('FIRST-START-SESSION-DECISION.ORIGINAL.txt','FIRST-P1-OPERATIONAL-DECISION.ORIGINAL.txt')
$source=[IO.File]::ReadAllText((Join-Path $folder 'p1-session-originals.cs'))
if($source -cne $expected){throw 'ADAPTER_EXACT_NAME_DELTA_DENY'}
$model=[IO.File]::ReadAllText((Join-Path $folder 'first-start-originals.model.cs'))
$usings=@([regex]::Matches($source+"`n"+$model,'(?m)^using [A-Za-z0-9_.]+;')|ForEach-Object {$_.Value}|Sort-Object -Unique)
$body=[regex]::Replace($source+"`n"+$model,'(?m)^using [A-Za-z0-9_.]+;','')
$compiler=[CodeDom.Compiler.CompilerParameters]::new()
$compiler.GenerateInMemory=$true;$compiler.WarningLevel=4;$compiler.TreatWarningsAsErrors=$true
[void]$compiler.ReferencedAssemblies.Add('System.dll');[void]$compiler.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition (($usings -join "`n")+"`n"+$body) -CompilerParameters $compiler
$modelResult=[FirstStartDocumentsModel]::Run()
$asts=@{}
foreach($name in @('prepare-p1-originals.MANUAL.CANDIDATE.ps1','run-p1-diagnostic.MANUAL.CANDIDATE.ps1')){
 $tokens=$null;$errors=$null
 $asts[$name]=[Management.Automation.Language.Parser]::ParseFile((Join-Path $folder $name),[ref]$tokens,[ref]$errors)
 if($errors.Count){throw 'MANUAL_AST_DENY'}
 foreach($call in $asts[$name].FindAll({param($n) $n -is [Management.Automation.Language.CommandAst]},$true)){
  if($call.GetCommandName() -match '^(Start-Service|Stop-Service|Restart-Service|Set-Acl|Copy-Item|Remove-Item|Move-Item|sc|sc.exe|Invoke-Expression|Start-Process)$'){throw 'EXTRA_OPERATION_DENY'}
 }
}
$guard=@($asts['prepare-p1-originals.MANUAL.CANDIDATE.ps1'].FindAll({param($n) $n -is [Management.Automation.Language.FunctionDefinitionAst] -and $n.Name -ceq 'Assert-P1OriginalPins'},$true))
if($guard.Count-ne1){throw 'UNIQUE_P1_GUARD_DENY'}
# Define/invoke ONLY the pure DATA-pin guard, never its wrapper.
. ([scriptblock]::Create($guard[0].Extent.Text))
$request=(Get-FileHash (Join-Path $folder 'ONE-P1-THREE-ACTS-REQUEST.CANDIDATE.md')).Hash.ToLowerInvariant()
$guardCases=0
for($case=0;$case-lt9;$case++){
 $requestPin=$request;$decisionPin='2'*64
 if($case-eq1){$requestPin='312999dd9ba4f701991560e65b4a25e3a13c7914bc75603a6efb47a11536798d'}
 if($case-eq2){$decisionPin='7f6451d59fc0b7015e0ac612ee471782950880fcfc2013137dc9643276af641a'}
 if($case-eq3){$decisionPin='c6c34866050bc0da189acd8724470838909bc7c790b838e23c46cb86ef58210f'}
 if($case-eq4){$decisionPin='ef8b47fa4dba37224bef904488ff0494db4e5804c1e696463ddc0dd864f3e199'}
 if($case-eq5){$requestPin='af8529db7f447fa6af6fc462fcb3cba4583abfc41f2e867c255b801974a67bc9'}
 if($case-eq6){$requestPin='25ea33a28c714952e1b1fb42202e64be7ca933afe0fa7aa37d2a5e654918dff4'}
 if($case-eq7){$decisionPin=$requestPin}
 if($case-eq8){$decisionPin='bad'}
 $denied=$false;try{Assert-P1OriginalPins $requestPin $decisionPin}catch{if($_.Exception.Message-ne'P1_ORIGINAL_SCOPE_DENY'){throw};$denied=$true}
 if($denied-ne($case-ne0)){throw 'P1_SCOPE_MODEL_DENY'};$guardCases++
}
$driver=[IO.File]::ReadAllText((Join-Path $folder 'run-p1-diagnostic.MANUAL.CANDIDATE.ps1'))
$calls=@($asts['run-p1-diagnostic.MANUAL.CANDIDATE.ps1'].FindAll({param($n) $n -is [Management.Automation.Language.CommandAst] -and $n.InvocationOperator -eq 'Ampersand'},$true))
if($calls.Count-ne2 -or $calls[1].Extent.Text-cne'& $sessionImage @sessionArguments' -or -not $calls[0].Extent.Text.Contains('-File $pairProcedure')){throw 'DRIVER_INVOCATION_SCOPE_DENY'}
if(-not $driver.Contains("'--owner-manual-first-scq-start-to-existing'") -or -not $driver.Contains("'1994944','1994944','7239172096','9823326208','128','100','60000','240000'")){throw 'P1_ENTRY_OR_G3_DENY'}
$pins=@{
 'p1-session-originals.cs'='07d1feb413be1685763f1ea5fc5cedfdfce3570604ae7860d1c46796909efc6b'
 'prepare-p1-originals.MANUAL.CANDIDATE.ps1'='4c4110d8d36f92d3871c9e87b15c6d6a7bd0c7690f16df4f0a1d33d38824dd9d'
 'run-p1-diagnostic.MANUAL.CANDIDATE.ps1'='c4242f434730df652ec7043fcef8313e0e4a8ebbd961690bcd65a6d85cf3d79b'
 'P1-ACT-ENDPOINT.REQUEST.CANDIDATE.md'='89343c75847d37734e582abc30656ccbdd37055d7667cad90b521c75ee85b04f'
 'P1-ACT-SCQ.REQUEST.CANDIDATE.md'='842c9d862a116e5d5ff1990d10927341b8441b799639cfb906d4a52fd2c2a7f7'
 'P1-ACT-START.REQUEST.CANDIDATE.md'='b1877bd5c5ecc397fee5e927ed19b32567d0986d383c2ebf4d539fd9bad76c01'
}
foreach($name in $pins.Keys){if((Get-FileHash (Join-Path $folder $name)).Hash-ne$pins[$name]){throw 'PACKAGE_PIN_DENY'}}
$requestBytes=(Get-Item (Join-Path $folder 'ONE-P1-THREE-ACTS-REQUEST.CANDIDATE.md')).Length
if($requestBytes-gt65536){throw 'REQUEST_DOCUMENT_BOUND_DENY'}
$result=[ordered]@{
 status='OFFLINE_QUALIFIED_FOR_OWNER_DECISION_NOT_AUTHORIZED';request_sha256=$request;request_bytes=$requestBytes;
 adapter_model=$modelResult;adapter_name_only_delta='PASS_FOUR_FIXED_NAMES';pure_scope_guard_cases=$guardCases;
 wrappers_ast='PASS';future_driver_invocations=2;g3_unchanged=$true;manual_wrappers_invoked=$false;
 native_constructor_or_file_api_invoked=$false;protected_originals_created=0;endpoint_or_scm_invoked=$false;
 owner_decision_emitted=$false;product_builds_reused=4;product_ctest_pairs_reused=60;product_builds_rerun=0;
 historical_retention='UNKNOWN';os_quiescence='UNKNOWN';start_admission='UNPROVEN_GATES_PENDING';productive_readiness='DENY'
}
[IO.File]::WriteAllText((Join-Path $folder 'P1-SESSION-OFFLINE-EVIDENCE.json'),($result|ConvertTo-Json -Depth 5),[Text.UTF8Encoding]::new($false))
$modelResult
'PASS P1_SCOPE_GUARD cases=9; AST=PASS; DRIVER_NOT_EXECUTED; NO_NATIVE_DOCUMENT_OR_PRODUCT_OPERATION'
