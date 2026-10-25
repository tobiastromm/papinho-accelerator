$ErrorActionPreference='Stop'
Import-Module -Name ($PSHOME+'\Modules\Microsoft.PowerShell.Utility\Microsoft.PowerShell.Utility.psd1') -ErrorAction Stop
$path='C:\Projetos\PapinhoAccelerator\tools\qualification\personal-first-install-uac.ps1'
$tokens=$null;$errors=$null
$ast=[Management.Automation.Language.Parser]::ParseFile($path,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'SCRIPT_PARSE_FAILED'}
foreach($name in @('Assert-PlainPath','Open-Pinned','Get-Hash','Assert-ConsoleExecutable','Assert-SameOperator','Parse-Result','Is-UacCancellation')){
 $f=$ast.FindAll({param($n) $n -is [Management.Automation.Language.FunctionDefinitionAst]},$false)|Where-Object {$_.Name -ceq $name}
 if(@($f).Count -ne 1){throw 'FUNCTION_NOT_UNIQUE'}
 . ([ScriptBlock]::Create($f.Extent.Text))
}
$count=0
function Check([bool]$Value){if(-not $Value){throw ('CHECK_FAILED_'+$script:count)};$script:count++}
function Denied([scriptblock]$Action){$failed=$false;try {& $Action|Out-Null} catch {$failed=$true};Check $failed}
$prefix='CUSTODY_METADATA_ONLY: target=0 ca=0 effect_denied=1'+"`n"
$tail='; RESOURCES=CLOSED; CA_CUSTODY=ABSENT; ED11=PARTIAL; BOOTSTRAP=DENY; CURRENT_PUBLICATION=DENY'
Check ((Parse-Result ($prefix+'FIRST_INSTALL=EXACT_CONFIRMATION_DATA_ONLY'+$tail) 2) -ceq 'READ_ONLY_WORKER_VERIFIED_FINAL_DENY')
Check ((Parse-Result ($prefix+'FIRST_INSTALL=DENY_OR_CANCELLED'+$tail) 2) -ceq 'DENIED_OR_CANCELLED')
Check ((Parse-Result ($prefix+'FIRST_INSTALL=EXACT_CONFIRMATION_DATA_ONLY'+$tail.Replace('CLOSED','RETAINED_UNKNOWN')) 2) -ceq 'RETAINED_UNKNOWN_NO_RETRY')
Check ((Parse-Result ($prefix+'FIRST_INSTALL=EXACT_CONFIRMATION_DATA_ONLY'+$tail) 0) -ceq 'UNEXPECTED_EXIT_DENY')
Check ((Parse-Result 'arbitrary output' 2) -ceq 'UNVERIFIED_DIAGNOSTIC_DENY')
Check ((Parse-Result (('FIRST_INSTALL=EXACT_CONFIRMATION_DATA_ONLY'+$tail+"`n")*2) 2) -ceq 'UNVERIFIED_DIAGNOSTIC_DENY')
# MODEL equality only; never passed to the launcher/native ceremony.
$a=[pscustomobject]@{sid='S-1-5-1';witness='a';eligible=$true;console=$true}
$b=[pscustomobject]@{sid='S-1-5-2';witness='a';eligible=$true;console=$true}
Denied {Assert-SameOperator $a $b}
$b.sid=$a.sid;$b.witness='b';Denied {Assert-SameOperator $a $b}
$b.witness='a';$b.console=$false;Denied {Assert-SameOperator $a $b}
$b.console=$true;$b.eligible=$false;Denied {Assert-SameOperator $a $b}
Check (Is-UacCancellation ([ComponentModel.Win32Exception]::new(1223)))
Check (-not (Is-UacCancellation ([ComponentModel.Win32Exception]::new(5))))
$fixture=Join-Path $env:TEMP 'pacc-uac-launcher-20261022\pin-fixture.txt'
[IO.Directory]::CreateDirectory([IO.Path]::GetDirectoryName($fixture))|Out-Null
Assert-PlainPath ([IO.Path]::GetDirectoryName($fixture))
[IO.File]::WriteAllText($fixture,'isolated hash and sharing test')
$h='97db72512b79cbbc535f80bf419791695963b86f07e803300666ae61ed20bb10'
Denied {Open-Pinned $fixture ('0'*64)}
$s=Open-Pinned $fixture $h
try {
 Check ((Get-Hash $s) -ceq $h)
 Denied {$w=[IO.File]::Open($fixture,[IO.FileMode]::Open,[IO.FileAccess]::Write,[IO.FileShare]::ReadWrite);$w.Dispose()}
 Denied {Assert-ConsoleExecutable $s}
} finally {$s.Dispose()}
Denied {Assert-PlainPath 'relative-path'}
[Console]::WriteLine('PURE_HELPER_NEGATIVE_CHECKS_PASS='+$count+'; NO_UAC_NO_CHILD_NO_NATIVE_AUTHORITY')
