$ErrorActionPreference='Stop'
$path=Join-Path $PSScriptRoot 'launcher-console-forwarding-PROPOSED.ps1'
$tokens=$null;$errors=$null;$ast=[Management.Automation.Language.Parser]::ParseFile($path,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'PARSE'}
$loops=$ast.FindAll({param($x)$x -is [Management.Automation.Language.WhileStatementAst] -and $x.Extent.Text.Contains('$reader.ReadLine()')},$true)
if($loops.Count-ne1){throw 'LOOP'}
function Write-Host {param($Object) $script:hostMessage=[string]$Object}
$Mode='Cycle';$chars=0;$final=$false;$outputTruncated=$false
$record='IG_FINAL stage=MODEL first=0 close=0 starts=1 stops=1 quiescent=1 intent_consumed=1 effect=2 diagnostic_created=1;'
$reader=[IO.StringReader]::new((('x'*100+"`n")*1500)+$record+"`n")
$sink=[IO.StringWriter]::new();$saved=[Console]::Out
try{[Console]::SetOut($sink);. ([scriptblock]::Create($loops[0].Extent.Text))}finally{[Console]::SetOut($saved);$reader.Dispose()}
if(-not $final-or-not$outputTruncated-or$chars-lt150000-or-not$sink.ToString().Contains($record)){throw 'DRAIN'}
$chars=0;$final=$false;$outputTruncated=$false;$reader=[IO.StringReader]::new($record.Replace('quiescent=1','quiescent=0')+"`n")
try{[Console]::SetOut($sink);. ([scriptblock]::Create($loops[0].Extent.Text))}finally{[Console]::SetOut($saved);$reader.Dispose();$sink.Dispose()}
if($final){throw 'UNKNOWN_FALSE_PASS'}
[Console]::Out.WriteLine('PROPOSED_OUTPUT_LOOP=PASS; DRAIN_TRUNCATION_FINAL_RECEIPT_UNKNOWN_PRESERVED; NO_NATIVE_ENTRY')
