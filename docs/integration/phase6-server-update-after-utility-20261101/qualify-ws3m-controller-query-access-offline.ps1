# Offline-only compiler/model/AST qualification. Never calls native RunFixed.
$ErrorActionPreference='Stop'
$folder='C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
$source=Get-Content -LiteralPath (Join-Path $folder 'ws3m-controller-query-access.CANDIDATE.cs') -Raw -Encoding UTF8
$model=Get-Content -LiteralPath (Join-Path $folder 'ws3m-controller-query-access.OFFLINE-MODEL.cs') -Raw -Encoding UTF8
if($model.Contains('DllImport')-or$model.Contains('extern ')){throw 'MODEL_NATIVE_BOUNDARY_DENY'}
if(([regex]::Matches($source,'\[DllImport\(')).Count-ne14){throw 'NATIVE_EDGE_CENSUS_DENY'}
if($source-match 'TerminateProcess|ControlService|StartService|WriteProcessMemory|ReadProcessMemory|DuplicateToken|AdjustTokenPrivileges|OpenProcess\(.*0x00101001'){throw 'MUTATOR_OR_EXCESS_RIGHTS_DENY'}
Add-Type -TypeDefinition $source -Language CSharp -ErrorAction Stop
Write-Output 'REAL_SOURCE_COMPILED_ONLY; NATIVE_RUNFIXED_NOT_CALLED'
Add-Type -TypeDefinition $model -Language CSharp -ErrorAction Stop
Write-Output ([RuntimeQueryAccessReadOnlyModel]::OfflineCases())
$path=Join-Path $folder 'observe-ws3m-controller-query-access.MANUAL.CANDIDATE.ps1'
$tokens=$null;$errors=$null
$ast=[System.Management.Automation.Language.Parser]::ParseFile($path,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'WRAPPER_AST_DENY'}
$commands=@($ast.FindAll({param($n) $n -is [System.Management.Automation.Language.CommandAst]},$true) | ForEach-Object {$_.GetCommandName()} | Sort-Object -Unique)
foreach($name in $commands){if($name-notin@('Add-Type','Assert-Pin','Get-Content','Get-FileHash','Join-Path','Set-StrictMode','Write-Host')){throw ('WRAPPER_COMMAND_DENY '+$name)}}
$manual=Get-Content -LiteralPath $path -Raw
if(([regex]::Matches($manual,'\[RuntimeQueryAccessReadOnly\]::RunFixed\(\)')).Count-ne1){throw 'WRAPPER_INVOCATION_CENSUS_DENY'}
if($manual.Contains('OBSERVE_WS3M_CONTROLLER_QUERY_ACCESS_ONCE')){throw 'AUTOMATIC_CONFIRMATION_DENY'}
Write-Output 'WRAPPER_AST_PASS; ONE_FUTURE_FIXED_NATIVE_INVOCATION; NO_AUTOMATIC_REPLY; NO_NATIVE_QUERY_EXECUTED'
