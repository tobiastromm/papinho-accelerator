# Isolated comparison of exact Pin/Lock source; no native admission/SCM/protected files.
param([ValidateSet('Current','Patched')][string]$Variant,[string]$ResultPath)
$ErrorActionPreference='Stop';Set-StrictMode -Version Latest
trap {[IO.File]::WriteAllText($ResultPath,(@{result='FAIL';error=$_.Exception.ToString();pid=$PID}|ConvertTo-Json));exit 2}
$Root='C:\Projetos\PapinhoAccelerator';$Qualifier=$Root+'\build\ed11-positive-private-20261025\Selected\Release\papacc_scm_instrumented_lab.exe'
$TransportBinding=if($Variant-ne'Offline'){$Root+'\build\ed11-private-stdout-20261025\Selected\Release\Pacc.N0PrivateStdout.Binding.dll'}else{$Root+'\build\ed11-positive-private-20261025\Selected\Release\Pacc.N0PrivateStdout.Budget.Binding.dll'}
[void][Reflection.Assembly]::LoadFrom($Root+'\build\ed11-private-stdout-20261025\Selected\Release\Pacc.TransportConsole.Test.dll')
[void][Reflection.Assembly]::LoadFrom($Root+'\build\ed11-positive-private-20261025\Selected\Release\Pacc.AdminProbe.Test.dll')
[PaccAdminProbe]::ConfigureFixtureErrors()
$rows=[Collections.Generic.List[object]]::new();$pins=[Collections.Generic.List[IDisposable]]::new();$locks=[Collections.Generic.List[IntPtr]]::new()
function Sample($phase){
 $row=@{phase=$phase;handles=[PaccTransportConsoleTest]::Handles();tick=[Environment]::TickCount}
 if($phase-in@('PINS_COMPLETE','CONFIRMATION_COMPLETE','CHILD_AUTHENTICATED_PRE_CENSUS_SUBSTITUTE','PINS_LOCKS_CLOSED')){
  $table=[PaccAdminProbe]::Capture();[IO.File]::WriteAllText(($ResultPath+'.'+$phase+'.handles.csv'),$table)
  $row.after_snapshot=[PaccTransportConsoleTest]::Handles()
  $row.tokens=@(($table-split"`n")|Where-Object{$_-match ';Token;'}).Count
  if($phase-ne'PINS_LOCKS_CLOSED'){$row.pin_handles=@($pins|ForEach-Object{if($_-is[IO.FileStream]){$_.SafeFileHandle.DangerousGetHandle().ToInt64()}})}
 }
 if($null-ne(Get-Variable fixtureAux -ErrorAction SilentlyContinue)){$row.auxiliary_pid=$fixtureAux.Id;$row.auxiliary_creation=$fixtureAux.Creation;$row.auxiliary_handles=[PaccAdminProbe]::Count($fixtureAux.Id);$row.administrative_handles=$row.handles+$row.auxiliary_handles}
 $rows.Add($row)
}
$source=if($Variant-eq'Current'){[IO.File]::ReadAllText($Root+'\docs\integration\n0start03-integrated-closeout-20261024\admin-handle-budget-resolution-20261025\scm-n0-protected-single-powershell.ps1.preimage')}else{[IO.File]::ReadAllText($Root+'\tools\qualification\scm-n0-protected-single-powershell.ps1')}
$tokens=$null;$errors=$null;$ast=[Management.Automation.Language.Parser]::ParseInput($source,[ref]$tokens,[ref]$errors)
foreach($name in @('Hash-Stream','Pin','Lock-QualifierPath','Select-AuxiliaryCandidate')){
 $node=$ast.FindAll({param($node)$node-is[Management.Automation.Language.FunctionDefinitionAst]-and$node.Name-eq$name},$true)[0];$text=$node.Extent.Text
 if($false){
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
$self=[Diagnostics.Process]::GetCurrentProcess();try{$fixtureBirth=[uint64]$self.StartTime.ToUniversalTime().ToFileTimeUtc();$fixtureSession=$self.SessionId}finally{$self.Dispose()}
$fixtureAux=Select-AuxiliaryCandidate $fixtureBirth $fixtureSession
Sample 'PINS_COMPLETE'
$Mode='Cycle';$Operation='ISOLATED_FIXTURE_NO_OPERATION';$Attempt='0b1de869e11493b7cb08fb329578f6ce';$AuxiliaryPid=1;$AuxiliaryCreation=[uint64]1
# Exact administrative-message/Read-Host block; no native owner dispatch.
$begin=$source.IndexOf(" Write-Host ('OPERATION='")
if($begin-lt0){$begin=$source.IndexOf(' if($Mode-eq''Cycle''){[Console]::Out.WriteLine(''OPERATION=''')}
$end=$source.IndexOf(' [Threading.Thread]::BeginThreadAffinity()',$begin)
if($begin-lt0-or$end-le$begin){throw 'ADMIN_SOURCE_FRAGMENT_BOUNDARY_DENY'}
[PaccTransportConsoleTest]::Text("START_N0_SINGLE_LAB $Operation $Attempt $AuxiliaryPid $AuxiliaryCreation`r")
. ([scriptblock]::Create($source.Substring($begin,$end-$begin)))
Sample 'CONFIRMATION_COMPLETE'
[Threading.Thread]::BeginThreadAffinity()
$process=[Pacc.N0PrivateStdout.Supervisor]::new($Root+'\build\ed11-positive-private-20261025\Selected\Release\papacc_n0_private_stdout.dll')
$process.Open();Sample 'TRANSPORT_OPEN'
$process.Start($Root+'\build\ed11-positive-private-20261025\Selected\Release\papacc_noninherited_transport_integration_test.exe','--fixture-child console')
$process.Authenticate();Sample 'CHILD_AUTHENTICATED_PRE_CENSUS_SUBSTITUTE'
Write-Host 'N0_OUTPUT_TRUNCATED; DRAIN_CONTINUES_UNTIL_FACTUAL_RETURN; NO_RETRY';Sample 'LATE_HOST_WARNING'
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
