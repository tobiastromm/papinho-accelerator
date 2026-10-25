param([Parameter(Mandatory=$true)][string]$LauncherHash)
$ErrorActionPreference='Stop';$r='C:\Projetos\PapinhoAccelerator';$path=$r+'\tools\qualification\scm-n0-protected-single-powershell.ps1'
$tokens=$null;$errors=$null;[void][Management.Automation.Language.Parser]::ParseFile($path,[ref]$tokens,[ref]$errors);if($errors.Count){throw 'PARSER_DENY'}
$source=[IO.File]::ReadAllText($path)
foreach($v in @("`$Profile='EMPTY_DENY'",'POSITIVE_ROUTE_DENY_PIN_SLICE_READONLY_ONLY','EXACT-TRANSPORT-READONLY-APPROVED.json','CloseUnstartedIfKnown','CloseChecked','RESOURCE_RETENTION=UNKNOWN')){if(-not$source.Contains($v)){throw ('SOURCE_GATE '+$v)}}
if($source.Contains('[Diagnostics.ProcessStartInfo]')-or$source.Contains('GetProcessById')){throw 'OLD_SUPERVISOR_REINTRODUCED'}
if(Test-Path -LiteralPath ($r+'\docs\integration\n0start03-integrated-closeout-20261024\t0-t9-acceptance-and-integration\EXACT-TRANSPORT-READONLY-APPROVED.json')){throw 'UNEXPECTED_EXECUTABLE_APPROVAL'}
& $path -Mode Review -Profile N0START03ReadOnly -ExpectedScriptSha256 $LauncherHash
if($global:LASTEXITCODE-ne0){throw 'STATIC_REVIEW_FAILED'}
'STATIC_SOURCE_GATES=PASS; NO_PREPARE_NO_OWNER_NO_OPERATIONAL_EFFECT'
