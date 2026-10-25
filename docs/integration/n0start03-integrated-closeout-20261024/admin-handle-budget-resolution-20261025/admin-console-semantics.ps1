# Isolated exact console fragment semantics. No owner/native process launch.
param([string]$Case,[string]$ResultPath)
$ErrorActionPreference='Stop';Set-StrictMode -Version Latest
$Root='C:\Projetos\PapinhoAccelerator';[void][Reflection.Assembly]::LoadFrom($Root+'\build\ed11-private-stdout-20261025\Selected\Release\Pacc.TransportConsole.Test.dll')
$source=[IO.File]::ReadAllText($Root+'\tools\qualification\scm-n0-protected-single-powershell.ps1')
$begin=$source.IndexOf(' if($Mode-eq''Cycle''){[Console]::Out.WriteLine(''OPERATION=''');$end=$source.IndexOf(' [Threading.Thread]::BeginThreadAffinity()',$begin)
if($begin-lt0-or$end-le$begin){throw 'SOURCE_BOUNDARY'}
$fragment=$source.Substring($begin,$end-$begin);$Mode=if($Case-eq'ReadOnly'){'Prepare'}else{'Cycle'}
$Operation='ISOLATED_FIXTURE';$Attempt='0b1de869e11493b7cb08fb329578f6ce';$AuxiliaryPid=1;$AuxiliaryCreation=[uint64]1
$expected=if($Mode-eq'Cycle'){'START_N0_SINGLE_LAB '+$Operation+' '+$Attempt+' 1 1'}else{'PREPARE_N0_READ_ONLY '+$Attempt+' 1 1'}
$state=@{case=$Case;result='FAIL';native_child_started=$false;operational=$false};$old=[Console]::Out;$capture=[IO.StringWriter]::new()
try {
 if($Case-eq'Redirected'){
  $guard=($source-split"`n"|Where-Object{$_-match '^ if\(\[Console\]::IsInputRedirected'})
  if(@($guard).Count-ne1){throw 'GUARD_SOURCE_BOUNDARY'}
  . ([scriptblock]::Create($guard));throw 'REDIRECT_NOT_DENIED'
 }
 if($Case-eq'Mismatch'){$expected='MISMATCH'}
 [PaccTransportConsoleTest]::Text($expected+"`r")
 if($Case-eq'WriteError'){[Console]::SetOut([PaccThrowWriter]::new())}else{[Console]::SetOut($capture)}
 $information=@(. ([scriptblock]::Create($fragment)) 6>&1)
 if($Case-in@('WriteError','Mismatch')){throw 'NEGATIVE_ACCEPTED'}
 $n=@($information|Where-Object{$_-is[Management.Automation.InformationRecord]}).Count
 if($Mode-eq'Cycle'){
  if($n-ne0-or-not$capture.ToString().Contains('Type exactly: '+$expected)-or-not$capture.ToString().Contains('OPERATION='+$Operation)){throw 'POSITIVE_CONSOLE_PROTOCOL'}
 }elseif($n-ne3){throw 'READONLY_INFORMATION_STREAM_CHANGED'}
 $state.result='PASS';$state.information_records=$n;$state.output=$capture.ToString()
}catch{
 $state.error=$_.Exception.ToString()
 if(($Case-eq'WriteError'-and$state.error.Contains('SYNTHETIC_STDOUT_WRITE_ERROR'))-or($Case-eq'Mismatch'-and$state.error.Contains('CONFIRMATION_DIVERGENCE'))-or($Case-eq'Redirected'-and$state.error.Contains('INTERACTIVE_CONSOLE_REQUIRED'))){$state.result='PASS';$state.expected_denial=$true}
}finally{[Console]::SetOut($old);$capture.Dispose()}
[IO.File]::WriteAllText($ResultPath,($state|ConvertTo-Json -Depth 5));if($state.result-ne'PASS'){exit 2}
