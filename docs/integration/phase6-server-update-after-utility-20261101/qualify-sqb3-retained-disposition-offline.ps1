# Offline-only qualification; native candidate is compiled, NEVER invoked.
$ErrorActionPreference='Stop'
$folder='C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
$source=Get-Content -LiteralPath (Join-Path $folder 'sqb3-retained-participants-disposition.CANDIDATE.cs') -Raw -Encoding UTF8
$model=Get-Content -LiteralPath (Join-Path $folder 'sqb3-retained-participants-disposition.OFFLINE-MODEL.cs') -Raw -Encoding UTF8
if($model.Contains('DllImport')-or$model.Contains('extern ')){throw 'MODEL_NATIVE_BOUNDARY_DENY'}
if(([regex]::Matches($source,'\[DllImport\(')).Count-ne17){throw 'NATIVE_EDGE_CENSUS_DENY'}
if($source-match 'ControlService|StartService|ChangeServiceConfig|WriteProcessMemory|ReadProcessMemory|DuplicateToken|AdjustTokenPrivileges|FileMode.Create|FileAccess.Write'){throw 'UNAUTHORIZED_OPERATION_DENY'}
if(([regex]::Matches($source,'Native\(TerminateProcess\(')).Count-ne2){throw 'TERMINATION_CALL_CENSUS_DENY'}
Add-Type -TypeDefinition $source -Language CSharp -ErrorAction Stop
Write-Output 'REAL_CANDIDATE_COMPILED_ONLY; NO_DISPOSITION_METHOD_INVOKED'
Add-Type -TypeDefinition $model -Language CSharp -ErrorAction Stop
Write-Output ([SQB3RetainedParticipantsDispositionModel]::OfflineCases())
$path=Join-Path $folder 'dispose-sqb3-retained-participants.MANUAL.CANDIDATE.ps1'
$tokens=$null;$errors=$null
$ast=[System.Management.Automation.Language.Parser]::ParseFile($path,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'WRAPPER_AST_DENY'}
$commands=@($ast.FindAll({param($n) $n -is [System.Management.Automation.Language.CommandAst]},$true) | ForEach-Object {$_.GetCommandName()} | Sort-Object -Unique)
foreach($name in $commands){if($name-notin@('Add-Type','Assert-Pin','Get-Content','Get-FileHash','Join-Path','Set-StrictMode','Write-Host')){throw ('WRAPPER_COMMAND_DENY '+$name)}}
$manual=Get-Content -LiteralPath $path -Raw
if(([regex]::Matches($manual,'\[SQB3RetainedParticipantsDisposition\]::RunFixed\(\)')).Count-ne1){throw 'WRAPPER_INVOCATION_CENSUS_DENY'}
if($manual.Contains('DISPOSE_SQB3_4184_21260_ONCE')){throw 'AUTOMATIC_CONFIRMATION_DENY'}
Write-Output 'WRAPPER_AST_PASS; ONE_FUTURE_FIXED_INVOCATION; NO_AUTOMATIC_REPLY; NO_SCM_PROCESS_TOKEN_QUERY_OR_TERMINATION'
