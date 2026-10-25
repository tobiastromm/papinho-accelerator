# Offline fixture only. Does not invoke launcher Prepare or any admission owner.
param([string]$Case='clean',[string]$Config='Release',[string]$Profile='Selected',[Parameter(Mandatory=$true)][string]$ResultPath)
$ErrorActionPreference='Stop';Set-StrictMode -Version Latest
$root='C:\Projetos\PapinhoAccelerator'
$release=$root+'\build\ed11-private-stdout-20261025\Selected\Release'
$bin=$root+'\build\ed11-positive-private-20261025\'+$Profile+'\'+$Config
$state=@{case=$Case;config=$Config;profile=$Profile;result='FAIL';operational=$false}
try {
 [void][Reflection.Assembly]::LoadFrom($release+'\Pacc.N0PrivateStdout.Binding.dll')
 [void][Reflection.Assembly]::LoadFrom($release+'\Pacc.TransportConsole.Test.dll')
 $state.parent_before=[PaccTransportConsoleTest]::Handles()
 if($Case-eq'console-loss'){
  if([Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'FIXTURE_NOT_INTERACTIVE'}
  [PaccTransportConsoleTest]::DetachOwnTestConsole()
  try {[void][Console]::ReadKey($true)}catch{$state.console_api_error=$_.Exception.Message;throw 'ISOLATED_CONSOLE_LOSS_DENIED_BEFORE_CREATE'}
  throw 'CONSOLE_LOSS_WAS_NOT_DENIED'
 }

 if($Case-eq'console'-or$Case-eq'positive-console'){
  if([Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'FIXTURE_NOT_INTERACTIVE'}
  [PaccTransportConsoleTest]::Text("ADMIN_CONFIRM`r")
  if((Read-Host 'Fixture administrative confirmation')-cne'ADMIN_CONFIRM'){throw 'ADMIN_CONFIRM_DENY'}
  [PaccTransportConsoleTest]::Text("CONFIRM_TRANSPORT`r")
  [PaccTransportConsoleTest]::Key(27,[char]27)
  [PaccTransportConsoleTest]::Key(121,[char]0)
 }
 [Threading.Thread]::BeginThreadAffinity()
 if($Case-eq'preloaded-native'){[PaccTransportConsoleTest]::Preload($bin+'\papacc_n0_private_stdout.dll')}
 $process=[Pacc.N0PrivateStdout.Supervisor]::new($bin+'\papacc_n0_private_stdout.dll')
 $process.Open();$process.Start($bin+'\papacc_noninherited_transport_integration_test.exe',('--fixture-child '+$Case));$process.Authenticate()
 $state.parent_connected=[PaccTransportConsoleTest]::Handles()
 $reader=$process;$Mode=if($Case.StartsWith('positive')){'Cycle'}else{'Prepare'};$final=$false;$returned=$false;$outputTruncated=$false;$exitCode=2
 # Exercise byte-for-byte the canonical forwarding/receipt/Wait block, not a copy.
 $source=[IO.File]::ReadAllText($root+'\tools\qualification\scm-n0-protected-single-powershell.ps1')
 $begin=$source.IndexOf('  $chars=0');$end=$source.IndexOf(' } finally {# Affinity remains',$begin)
 if($begin-lt0-or$end-lt$begin){throw 'FORWARDING_BOUNDARY_DENY'}
 if($Case-eq'forward-error'){$savedOut=[Console]::Out;[Console]::SetOut([PaccThrowWriter]::new())}
 . ([scriptblock]::Create($source.Substring($begin,$end-$begin)))
 $state.final=$final;$state.returned=$returned;$state.truncated=$outputTruncated;$state.exit=$exitCode
 $process.CloseChecked();$state.native_receipt=$process.Receipt()
 $state.parent_after=[PaccTransportConsoleTest]::Handles()
 if($Case-eq'large'){if(-not$outputTruncated-or$exitCode-ne2){throw 'TRUNCATION_GATE'}}elseif($Case-eq'positive-partial'){if($exitCode-ne2-or-not$final){throw 'PARTIAL_RECEIPT_GATE'}}elseif($exitCode-ne0){throw 'CHILD_FAILED'}
 if($Case-eq'console'-or$Case-eq'positive-console'){do{$key=[Console]::ReadKey($true)}while($key.Key-ne[ConsoleKey]::F10);$state.f10=$true;$state.escape_and_native_readconsole=$true}
 [Threading.Thread]::EndThreadAffinity()
 $state.result='PASS'
}catch{
 $state.error=$_.Exception.ToString()
 if($Case-eq'preloaded-native' -and $state.error.Contains('NATIVE_MODULE_ALREADY_PRESENT_DENY')){$state.result='PASS';$state.no_child_or_endpoint_created=$true}
 if($Case-eq'console-loss' -and $state.error.Contains('ISOLATED_CONSOLE_LOSS_DENIED_BEFORE_CREATE')){$state.result='PASS';$state.no_child_or_endpoint_created=$true}
 if($Case-eq'forward-error'){[Console]::SetOut($savedOut)}
 if(($Case-eq'forward-error' -and $state.error.Contains('SYNTHETIC_STDOUT_WRITE_ERROR')) -or ($Case-in@('missing-final','positive-unknown','positive-inconsistent') -and $state.error.Contains('FACTUAL_FINAL_RECEIPT_MISSING_RETAIN'))){
  $state.result='PASS';$state.expected_denial=$true;$state.retention_required=(-not$returned -or -not$final);$state.fixture_containment_exit_not_product_cleanup=$true
 }
}
$state|ConvertTo-Json -Depth 5|Set-Content -LiteralPath $ResultPath -Encoding UTF8
if($state.result-ne'PASS'){exit 2}
