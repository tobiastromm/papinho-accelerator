$ErrorActionPreference='Stop'
$path='C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1'
$tokens=$null;$parseErrors=$null;$ast=[Management.Automation.Language.Parser]::ParseFile($path,[ref]$tokens,[ref]$parseErrors)
if($parseErrors.Count){throw ($parseErrors|Out-String)}
$commands=$ast.FindAll({param($x)$x -is [Management.Automation.Language.CommandAst]},$true)
if(@($commands|Where-Object {$_.GetCommandName() -match '^(powershell|pwsh)(\.exe)?$'}).Count){throw 'EXTRA_POWERSHELL'}
Write-Output 'AST=PASS; SINGLE_POWERSHELL; NO_NATIVE_PREPARE_OR_CYCLE_INVOKED'
& $path -Mode Review -ExpectedScriptSha256 'e5144e031f0d93aba16a22a8dba0b92b1eeb6b0bb4c03c0ca4ec5a4dec24b1cc'
if($LASTEXITCODE-ne0){throw 'REVIEW_FAILED'}
& $path -Mode Review -ExpectedScriptSha256 ('0'*64)
if($LASTEXITCODE-ne2){throw 'WRONG_PIN_NOT_DENIED'}
& $path -Mode Cycle -ExpectedScriptSha256 'e5144e031f0d93aba16a22a8dba0b92b1eeb6b0bb4c03c0ca4ec5a4dec24b1cc'
if($LASTEXITCODE-ne2){throw 'MISSING_APPROVAL_NOT_DENIED'}
Write-Output 'LAUNCHER_REVIEW_NEGATIVES=PASS; NO_OPERATOR_OR_NATIVE_ENTRY'

# Execute the ACTUAL output loop against a StringReader fixture only.
$loop=$ast.FindAll({param($x)$x -is [Management.Automation.Language.WhileStatementAst] -and $x.Extent.Text.Contains('$reader.ReadLine()')},$true)
if($loop.Count-ne1){throw 'OUTPUT_LOOP_NOT_UNIQUE'}
function Write-Host {param($Object) $script:displayCount++;if([string]$Object -like 'N0_OUTPUT_TRUNCATED*'){$script:sawTruncation=$true}}
$Mode='Cycle';$chars=0;$final=$false;$outputTruncated=$false;$script:displayCount=0;$script:sawTruncation=$false
$record='IG_FINAL stage=MODEL first=0 close=0 starts=1 stops=1 quiescent=1 intent_consumed=1 effect=2 diagnostic_created=1;'
$body=(('x'*100+"`n")*1500)+$record+"`n"
$reader=[IO.StringReader]::new($body)
try {. ([scriptblock]::Create($loop[0].Extent.Text))}finally{$reader.Dispose()}
if(-not $outputTruncated -or -not $final -or -not $script:sawTruncation -or $chars-lt150000){throw 'OUTPUT_DRAIN_RECEIPT_FAILED'}
$chars=0;$final=$false;$outputTruncated=$false
$reader=[IO.StringReader]::new($record.Replace('quiescent=1','quiescent=0')+"`n")
try {. ([scriptblock]::Create($loop[0].Extent.Text))}finally{$reader.Dispose()}
if($final){throw 'UNCERTAIN_RECEIPT_ACCEPTED'}
Write-Output 'ACTUAL_OUTPUT_LOOP=PASS; BOUNDED_DISPLAY_CONTINUED_DRAIN_FACTUAL_QUIESCENCE_RECEIPT; SYNTHETIC_NO_PROCESS_NO_SCM'
