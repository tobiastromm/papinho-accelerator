# Isolated comparison of exact Pin/Lock source; no native admission/SCM/protected files.
param([ValidateSet('Current','Offline','Console')][string]$Variant,[string]$ResultPath)
$ErrorActionPreference='Stop';Set-StrictMode -Version Latest
trap {[IO.File]::WriteAllText($ResultPath,(@{result='FAIL';error=$_.Exception.ToString();pid=$PID}|ConvertTo-Json));exit 2}
$Root='C:\Projetos\PapinhoAccelerator';$Qualifier=$Root+'\build\ed11-positive-private-20261025\Selected\Release\papacc_scm_instrumented_lab.exe'
$TransportBinding=if($Variant-ne'Offline'){$Root+'\build\ed11-private-stdout-20261025\Selected\Release\Pacc.N0PrivateStdout.Binding.dll'}else{$Root+'\build\ed11-positive-private-20261025\Selected\Release\Pacc.N0PrivateStdout.Budget.Binding.dll'}
[void][Reflection.Assembly]::LoadFrom($Root+'\build\ed11-private-stdout-20261025\Selected\Release\Pacc.TransportConsole.Test.dll')
$rows=[Collections.Generic.List[object]]::new();$pins=[Collections.Generic.List[IDisposable]]::new();$locks=[Collections.Generic.List[IntPtr]]::new()
function Sample($phase){$rows.Add(@{phase=$phase;handles=[PaccTransportConsoleTest]::Handles();tick=[Environment]::TickCount})}
$source=[IO.File]::ReadAllText($Root+'\tools\qualification\scm-n0-protected-single-powershell.ps1')
$tokens=$null;$errors=$null;$ast=[Management.Automation.Language.Parser]::ParseInput($source,[ref]$tokens,[ref]$errors)
foreach($name in @('Hash-Stream','Pin','Lock-QualifierPath')){
 $node=$ast.FindAll({param($node)$node-is[Management.Automation.Language.FunctionDefinitionAst]-and$node.Name-eq$name},$true)[0];$text=$node.Extent.Text
 if($Variant-eq'Offline'-and$name-eq'Lock-QualifierPath'){
  $line=($text-split"`n"|Where-Object{$_-match '^ & \(\[scriptblock\]::Create'})
  if(@($line).Count-ne1){throw 'EXACT_HELPER_LOAD_BOUNDARY_DENY'}
  $text=$text.Replace($line,' [void][Reflection.Assembly]::LoadFrom($TransportBinding)')
 }
 . ([scriptblock]::Create($text))
}
function Digest($path){$f=[IO.File]::OpenRead($path);try{return Hash-Stream $f}finally{$f.Dispose()}}
Sample 'ENTRY'
$identity=[Security.Principal.WindowsIdentity]::GetCurrent();$identity.Dispose()
$imp=[Security.Principal.WindowsIdentity]::GetCurrent($true);if($null-ne$imp){$imp.Dispose();throw 'IMPERSONATION_FIXTURE_DENY'}
Sample 'IDENTITY_PROBES'
# Match Utility JSON initialization, but no approval file or authority.
$null='{"fixture":true}'|ConvertFrom-Json
Pin ($Root+'\tools\qualification\scm-n0-protected-single-powershell.ps1') (Digest ($Root+'\tools\qualification\scm-n0-protected-single-powershell.ps1'))
Pin $TransportBinding (Digest $TransportBinding)
foreach($name in @('DECISION-n0start03-positive-private-transport-composition-20261025.md','DECISION-n0start03-n7-and-ed11-evaluation-plan-20261024.md')){$path=$Root+'\docs\adr\'+$name;Pin $path (Digest $path)}
Sample 'PRE_LOCK'
Lock-QualifierPath
Sample 'PATH_LOCKS'
[void][Reflection.Assembly]::LoadFrom($TransportBinding)
Sample 'BINDING_LOADED'
foreach($name in @('DECISION-n0start03-noninherited-local-transport-20261025.md','DECISION-first-native-lab-diagnostic-boundary-20261024.md','DECISION-canonical-lab-console-topology-bd-20261023.md','DECISION-ed11-single-lab-diagnostic-native-boundary-20261023.md','DECISION-n0-n9-security-descriptor-digest-profile-20261024.md')){$path=$Root+'\docs\adr\'+$name;Pin $path (Digest $path)}
Sample 'PINS_COMPLETE'
foreach($message in @('FIXTURE OPERATION: no admission/SCM/protected files','Fixture three original administrative messages','Type exactly: FIXTURE_CONFIRM')){
 if($Variant-eq'Console'){[Console]::Out.WriteLine($message)}else{Write-Host $message}
 Sample ('MESSAGE_'+$message.Length)
}
[PaccTransportConsoleTest]::Text("FIXTURE_CONFIRM`r");Sample 'INPUT_INJECTED'
if((Read-Host)-cne'FIXTURE_CONFIRM'){throw 'FIXTURE_CONFIRM_DENY'}
Sample 'CONFIRMATION_COMPLETE'
[Threading.Thread]::BeginThreadAffinity()
$process=[Pacc.N0PrivateStdout.Supervisor]::new($Root+'\build\ed11-positive-private-20261025\Selected\Release\papacc_n0_private_stdout.dll')
$process.Open();Sample 'TRANSPORT_OPEN'
$process.Start($Root+'\build\ed11-positive-private-20261025\Selected\Release\papacc_noninherited_transport_integration_test.exe','--fixture-child console')
$process.Authenticate();Sample 'CHILD_AUTHENTICATED_PRE_CENSUS_SUBSTITUTE'
[PaccTransportConsoleTest]::Text("CONFIRM_TRANSPORT`r");[PaccTransportConsoleTest]::Key(27,[char]27)
while($null-ne($line=$process.ReadLine())){[Console]::Out.WriteLine($line)}
$exit=[int]$process.WaitOriginal();if($exit-ne0){throw 'FIXTURE_CHILD_EXIT'}
Sample 'CHILD_RETURNED'
$process.CloseChecked();Sample 'TRANSPORT_CLOSED'
foreach($pin in $pins){$pin.Dispose()}
for($i=$locks.Count-1;$i-ge0;$i--){if(-not[PaccFileAcl]::CloseHandle($locks[$i])){throw 'FIXTURE_CLOSE_UNKNOWN_NO_RETRY'};$locks[$i]=[IntPtr]::Zero}
Sample 'PINS_LOCKS_CLOSED'
[Threading.Thread]::EndThreadAffinity()
@{variant=$Variant;pid=$PID;phases=$rows;owned_pin_count=$pins.Count;owned_lock_count=$locks.Count;result='PASS';substitutions='no protected inputs or native canonical owner/SCM; child inert console fixture; no valid operation approval'}|ConvertTo-Json -Depth 6|Set-Content -LiteralPath $ResultPath -Encoding UTF8
