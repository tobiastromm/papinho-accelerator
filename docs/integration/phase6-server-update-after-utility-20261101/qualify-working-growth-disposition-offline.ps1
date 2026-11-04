# Offline only; never call the real disposition.
$ErrorActionPreference='Stop'
$folder=$PSScriptRoot
$real=Get-Content -Raw -Encoding UTF8 (Join-Path $folder 'working-growth-controller-disposition.CANDIDATE.cs')
$model=Get-Content -Raw -Encoding UTF8 (Join-Path $folder 'working-growth-controller-disposition.OFFLINE-MODEL.cs')
if($model.Contains('DllImport')-or$model.Contains('extern ')){throw 'MODEL_NATIVE_EDGE_DENY'}
if(([regex]::Matches($real,'\[DllImport\(')).Count-ne15){throw 'NATIVE_EDGE_CENSUS_DENY'}
Add-Type -TypeDefinition $real -Language CSharp -ErrorAction Stop
Write-Output 'REAL_CANDIDATE_COMPILED_ONLY_NO_RUNFIXED'
Add-Type -TypeDefinition $model -Language CSharp -ErrorAction Stop
Write-Output ([DispositionOfflineQualification]::Run())
$tokens=$null;$errors=$null
$ast=[Management.Automation.Language.Parser]::ParseFile((Join-Path $folder 'dispose-working-growth-controller.MANUAL.CANDIDATE.ps1'),[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'WRAPPER_AST_DENY'}
$manual=$ast.Extent.Text
if(([regex]::Matches($manual,'\[RetainedP1Disposition\]::RunFixed\(\)')).Count-ne1-or$manual.Contains('DISPOSE_RETAINED_P1_21484_134381297590479169_ONCE')){throw 'WRAPPER_CALL_OR_AUTO_CONFIRM_DENY'}
Write-Output 'WRAPPER_AST_PASS_ONE_FUTURE_NATIVE_INVOCATION_NO_AUTOMATIC_REPLY_NO_OPERATION'
