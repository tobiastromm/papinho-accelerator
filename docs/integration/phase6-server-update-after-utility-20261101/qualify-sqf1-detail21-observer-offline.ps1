# Offline only: no RunFixed, SCM or process query.
$ErrorActionPreference='Stop'
$f=$PSScriptRoot
$m=Get-Content -LiteralPath (Join-Path $f 'SQF1-DETAIL21-OBSERVER-BINDINGS.json') -Raw|ConvertFrom-Json
$b=Join-Path $f $m.base
if((Get-FileHash -LiteralPath $b).Hash-ne$m.base_sha256){throw 'BASE_PIN_DENY'}
$t=[IO.File]::ReadAllText($b)
foreach($r in $m.rules){if([regex]::Matches($t,[regex]::Escape($r.old)).Count-ne$r.count){throw 'DELTA_COUNT_DENY'};$t=$t.Replace($r.old,$r.new)}
$p=Join-Path $f 'sqf1-detail21-participants-readonly.CANDIDATE.cs'
if([IO.File]::ReadAllText($p)-cne$t-or(Get-FileHash -LiteralPath $p).Hash-ne$m.source_sha256){throw 'EXACT_DELTA_DENY'}
if($t-match'TerminateProcess|ControlService|StartService|WriteProcessMemory|ReadProcessMemory|DuplicateToken|AdjustTokenPrivileges'){throw 'MUTATOR_DENY'}
if(([regex]::Matches($t,'\[DllImport\(')).Count-ne12){throw 'EDGE_CENSUS_DENY'}
Add-Type -TypeDefinition $t -Language CSharp -ErrorAction Stop
$tokens=$null;$errors=$null
$path=Join-Path $f 'observe-sqf1-detail21-participants.MANUAL.CANDIDATE.ps1'
$a=[Management.Automation.Language.Parser]::ParseFile($path,[ref]$tokens,[ref]$errors)
if($errors.Count-or(Get-FileHash -LiteralPath $path).Hash-ne$m.wrapper_sha256){throw 'WRAPPER_DENY'}
foreach($c in $a.FindAll({param($n)$n-is[Management.Automation.Language.CommandAst]},$true)){if($c.GetCommandName()-notin@('Add-Type','Assert-Pin','Get-Content','Get-FileHash','Join-Path','Set-StrictMode','Write-Host')){throw 'COMMAND_DENY'}}
if((Get-Content -LiteralPath $path -Raw).Contains('OBSERVE_SQF1DETAIL21_CURRENT_PARTICIPANTS_ONCE')){throw 'AUTO_CONFIRM_DENY'}
if((Get-FileHash -LiteralPath (Join-Path $f 'ONE-SQF1-DETAIL21-PARTICIPANTS-READONLY-REQUEST.CANDIDATE.md')).Hash-ne$m.request_sha256){throw 'REQUEST_PIN_DENY'}
'PASS EXACT_5_BINDINGS SOURCE_COMPILED AST_12_EDGES; NO_NATIVE_RUN; PREVIOUS_30_MODEL_CASES_REUSED'
