# Offline-only qualification; native candidate is compiled, NEVER invoked.
$ErrorActionPreference='Stop'
$folder='C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
$source=Get-Content -LiteralPath (Join-Path $folder 'sqf1-detail21-retained-participants-disposition.CANDIDATE.cs') -Raw -Encoding UTF8
if(([regex]::Matches($source,'\[DllImport\(')).Count-ne17){throw 'NATIVE_EDGE_CENSUS_DENY'}
if($source-match 'ControlService|StartService|ChangeServiceConfig|WriteProcessMemory|ReadProcessMemory|DuplicateToken|AdjustTokenPrivileges|FileMode.Create|FileAccess.Write'){throw 'UNAUTHORIZED_OPERATION_DENY'}
if(([regex]::Matches($source,'Native\(TerminateProcess\(')).Count-ne2){throw 'TERMINATION_CALL_CENSUS_DENY'}
Add-Type -TypeDefinition $source -Language CSharp -ErrorAction Stop
Write-Output 'REAL_CANDIDATE_COMPILED_ONLY; NO_DISPOSITION_METHOD_INVOKED'
Write-Output 'PREVIOUS_52_MODEL_CASES_REUSED; NEW_BINDINGS_ONLY'
$path=Join-Path $folder 'dispose-sqf1-detail21-retained-participants.MANUAL.CANDIDATE.ps1'
$tokens=$null;$errors=$null
$ast=[System.Management.Automation.Language.Parser]::ParseFile($path,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'WRAPPER_AST_DENY'}
$commands=@($ast.FindAll({param($n) $n -is [System.Management.Automation.Language.CommandAst]},$true) | ForEach-Object {$_.GetCommandName()} | Sort-Object -Unique)
foreach($name in $commands){if($name-notin@('Add-Type','Assert-Pin','Get-Content','Get-FileHash','Join-Path','Set-StrictMode','Write-Host')){throw ('WRAPPER_COMMAND_DENY '+$name)}}
$manual=Get-Content -LiteralPath $path -Raw
if(([regex]::Matches($manual,'\[SQF1DETAIL21RetainedParticipantsDisposition\]::RunFixed\(\)')).Count-ne1){throw 'WRAPPER_INVOCATION_CENSUS_DENY'}
if($manual.Contains('DISPOSE_SQF1DETAIL21_11760_20768_ONCE')){throw 'AUTOMATIC_CONFIRMATION_DENY'}
Write-Output 'WRAPPER_AST_PASS; ONE_FUTURE_FIXED_INVOCATION; NO_AUTOMATIC_REPLY; NO_SCM_PROCESS_TOKEN_QUERY_OR_TERMINATION'
