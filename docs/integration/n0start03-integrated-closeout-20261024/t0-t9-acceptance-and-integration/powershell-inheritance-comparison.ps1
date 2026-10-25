param([ValidateSet('Legacy','Private')][string]$Route,[Parameter(Mandatory=$true)][string]$ResultPath)
$ErrorActionPreference='Stop';$root='C:\Projetos\PapinhoAccelerator';$bin=$root+'\build\ed11-private-stdout-20261025\Selected\Release';$result=@{route=$Route;scope='ISOLATED_SAME_CHILD_NO_OPERATION';result='FAIL'}
try {
 [void][Reflection.Assembly]::LoadFrom($bin+'\Pacc.TransportConsole.Test.dll')
 [void][Reflection.Assembly]::LoadFrom($bin+'\Pacc.N0PrivateStdout.Binding.dll')
 $sentinels=[PaccTransportConsoleTest]::CreateSentinels()
 $result.parent_before=[PaccTransportConsoleTest]::Handles()
 if($Route-eq'Legacy'){
  $si=[Diagnostics.ProcessStartInfo]::new();$si.FileName=$bin+'\papacc_noninherited_transport_integration_test.exe';$si.Arguments='--legacy-baseline '+$PID+$sentinels;$si.UseShellExecute=$false;$si.RedirectStandardOutput=$true
  $child=[Diagnostics.Process]::new();$child.StartInfo=$si;if(-not$child.Start()){throw 'CREATE_FAIL'}
  $result.parent_connected=[PaccTransportConsoleTest]::Handles();$result.output=$child.StandardOutput.ReadToEnd();$child.WaitForExit();$result.exit=$child.ExitCode;$child.StandardOutput.Dispose();$child.Dispose()
  if($result.exit-ne0-or$result.output-notmatch'SENTINEL_MATCHES=8'){throw 'LEGACY_EXPECTATION'}
 }else{
  [Threading.Thread]::BeginThreadAffinity()
  $child=[Pacc.N0PrivateStdout.Supervisor]::new($bin+'\papacc_n0_private_stdout.dll');$child.Open();$child.Start($bin+'\papacc_noninherited_transport_integration_test.exe',('--fixture-child clean'+$sentinels));$child.Authenticate()
  $result.parent_connected=[PaccTransportConsoleTest]::Handles();$text=[Text.StringBuilder]::new()
  while($null-ne($line=$child.ReadLine())){[void]$text.AppendLine($line)}
  $result.output=$text.ToString();$result.exit=$child.WaitOriginal();$child.CloseChecked();$result.receipt=$child.Receipt();[Threading.Thread]::EndThreadAffinity()
  if($result.exit-ne0-or$result.output-notmatch'SENTINEL_MATCHES=0'){throw 'PRIVATE_EXPECTATION'}
 }
 [PaccTransportConsoleTest]::CloseSentinels();$result.parent_after=[PaccTransportConsoleTest]::Handles();$result.result='PASS'
}catch{$result.error=$_.Exception.ToString()}
$result|ConvertTo-Json|Set-Content -LiteralPath $ResultPath -Encoding UTF8
if($result.result-ne'PASS'){exit 2}
