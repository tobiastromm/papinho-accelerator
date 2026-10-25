$ErrorActionPreference='Stop'
$path='C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1'
$tokens=$null;$errors=$null;$ast=[Management.Automation.Language.Parser]::ParseFile($path,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'PARSE'}
$loops=$ast.FindAll({param($x)$x-is[Management.Automation.Language.WhileStatementAst]-and$x.Extent.Text.Contains('$reader.ReadLine()')},$true)
if($loops.Count-ne1){throw 'LOOP'}
$loop=[scriptblock]::Create($loops[0].Extent.Text)
Add-Type -TypeDefinition @"
using System;using System.IO;using System.Text;
public sealed class ForwardingFailWriter:TextWriter {public override Encoding Encoding{get{return Encoding.UTF8;}}public override void WriteLine(string s){throw new IOException("FIXTURE_STDOUT_BROKEN");}}
"@
$script:hostMessages=[Collections.Generic.List[string]]::new()
function Write-Host {param($Object)$script:hostMessages.Add([string]$Object)}
$results=[Collections.Generic.List[object]]::new()
$good='IG_FINAL stage=MODEL first=0 close=0 starts=1 stops=1 quiescent=1 intent_consumed=1 effect=2 diagnostic_created=1;'
$cases=@(
 @{Name='short';Body="first`nsecond`n";Mode='Cycle';Final=$false},
 @{Name='empty_unicode_order';Body="`nação 漢字 😀`n`nlast`n";Mode='Cycle';Final=$false},
 @{Name='long_truncation_drain';Body=(('x'*100+"`n")*1500)+$good+"`n";Mode='Cycle';Final=$true;Truncated=$true},
 @{Name='single_overlong_line';Body=('x'*4097)+"`n"+$good+"`n";Mode='Cycle';Final=$true;Truncated=$true},
 @{Name='final_quiescent';Body=$good+"`n";Mode='Cycle';Final=$true},
 @{Name='unknown_quiescence';Body=$good.Replace('quiescent=1','quiescent=0')+"`n";Mode='Cycle';Final=$false},
 @{Name='retained_record';Body="IG_RETAINED first=5; UNKNOWN`n";Mode='Cycle';Final=$false},
 @{Name='stop_before_start';Body='IG_FINAL stage=STOP first=995 close=0 starts=0 stops=0 quiescent=0 intent_consumed=0 effect=0 diagnostic_created=0;' +"`n";Mode='Cycle';Final=$true},
 @{Name='readonly_receipt';Body='IG_FINAL stage=MODEL first=0 close=0 starts=0 stops=0 quiescent=0 intent_consumed=0 effect=0 diagnostic_created=0;' +"`n";Mode='Prepare';Final=$true}
)
$saved=[Console]::Out
foreach($case in $cases){
 $Mode=$case.Mode;$chars=0;$final=$false;$outputTruncated=$false;$hostMessages.Clear();$reader=[IO.StringReader]::new($case.Body);$sink=[IO.StringWriter]::new()
 try{[Console]::SetOut($sink);. $loop;$observed=$sink.ToString();if($reader.ReadLine()-ne$null){throw 'NOT_DRAINED'}}finally{[Console]::SetOut($saved);$reader.Dispose();$sink.Dispose()}
 if($final-ne$case.Final){throw ('FINAL_'+$case.Name)}
 if($case.Truncated){if(-not$outputTruncated-or-not$observed.Contains($good)-or$hostMessages.Count-ne1){throw 'TRUNCATION'}}elseif($observed-cne$case.Body.Replace("`n",[Environment]::NewLine)){throw ('CONTENT_'+$case.Name)}
 $results.Add(@{name=$case.Name;pass=$true;chars=$chars;final=$final;truncated=$outputTruncated})
}
foreach($fault in @('IOException','DisposedWriter')){
 $Mode='Cycle';$chars=0;$final=$false;$outputTruncated=$false;$reader=[IO.StringReader]::new($good+"`n");$sink=if($fault-eq'IOException'){[ForwardingFailWriter]::new()}else{[IO.StringWriter]::new()};if($fault-eq'DisposedWriter'){$sink.Dispose()};$caught=$false
 try{[Console]::SetOut($sink);. $loop}catch{$caught=$true;$message=$_.Exception.ToString()}finally{[Console]::SetOut($saved);$reader.Dispose();$sink.Dispose()}
 if(-not$caught-or$final){throw 'WRITE_ERROR_SUPPRESSED'}
 $results.Add(@{name=$fault;pass=$true;error=$message;receipt_accepted=$final})
}
$retain=$ast.FindAll({param($x)$x-is[Management.Automation.Language.IfStatementAst]-and$x.Extent.Text.StartsWith('if(($launchAttempted -and -not $started)')},$true)
if($retain.Count-ne1){throw 'RETENTION_NOT_UNIQUE'}
$condition=[scriptblock]::Create($retain[0].Clauses[0].Item1.Extent.Text)
foreach($state in @(@{la=$true;st=$false;re=$false;fi=$false;want=$true},@{la=$true;st=$true;re=$false;fi=$false;want=$true},@{la=$true;st=$true;re=$false;fi=$true;want=$true},@{la=$true;st=$true;re=$true;fi=$false;want=$true},@{la=$true;st=$true;re=$true;fi=$true;want=$false})){
 $launchAttempted=$state.la;$started=$state.st;$returned=$state.re;$final=$state.fi;if((& $condition)-ne$state.want){throw 'RETENTION_CONDITION'}
}
$branch=$retain[0].Clauses[0].Item2.Extent.Text
if($branch-notmatch'\$script:N0RetainedProcess=\$process'-or$branch-match'\.Dispose\(|CloseHandle\('){throw 'RETENTION_DESTRUCTIVE'}
$results.Add(@{name='actual_retention_predicate_five_states_and_no_close_branch';pass=$true})
$results | ConvertTo-Json -Depth 5 | Set-Content -Encoding UTF8 (Join-Path $PSScriptRoot 'loop-test-results.json')
[Console]::Out.WriteLine('LOOP_TESTS_PASS; 11_IO_CASES_PLUS_RETENTION_5_STATES; EXTRACTED_CODE_ONLY_NO_OPERATIONAL_ENTRY')
