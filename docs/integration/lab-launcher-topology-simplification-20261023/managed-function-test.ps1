$ErrorActionPreference='Stop'
$q='C:\Projetos\_tmp\PapinhoAccelerator\lab-launcher-topology-20261023'
$tokens=$null;$errors=$null
$ast=[Management.Automation.Language.Parser]::ParseFile("$q\launcher-proposed.ps1",[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'AST failed'}
$def=$ast.Find({param($x) $x -is [Management.Automation.Language.FunctionDefinitionAst] -and $x.Name -eq 'Invoke-InheritedConsoleNative'},$true)
if(-not $def){throw 'function absent'}
. ([scriptblock]::Create($def.Extent.Text))
$script:RetainedNativeProcess=$null
$results=@()
foreach($config in @('debug','release')){
 $o=@(Invoke-InheritedConsoleNative "$q\topology-$config.exe" @('--text-child'))
 $line=@($o|Where-Object{$_ -like 'PROBE *'})
 if($line.Count -ne 1 -or $script:LASTEXITCODE -ne 0 -or $null -ne $script:RetainedNativeProcess){throw 'capture/exit/retention mismatch'}
 if($line[0] -notmatch 'count=(\d+) conin=1 conout=1 handler=1 ids=([0-9,]+)'){throw 'console contract missing'}
 if(@($Matches[2].Split(',')|Where-Object{$_ -eq [string]$PID}).Count -ne 1){throw 'PowerShell not in shared console membership'}
 if($o[1] -cne 'FIRST_FAILURE=170; UNCHANGED_FIXTURE_EXIT'){throw 'first error output changed'}
 $results+=@{config=$config;parent_pid=$PID;powershell=$PSVersionTable.PSVersion.ToString();result='PASS';output=$o;scope='same launcher function extracted by AST; no launcher entry or SCM'}
}
try{Invoke-InheritedConsoleNative "$q\does-not-exist.exe" @('--text-child');throw 'unexpected success'}catch{if($null -ne $script:RetainedNativeProcess){throw 'failed start retained a process'}}
try{Invoke-InheritedConsoleNative "$q\topology-debug.exe" @('--text-child bad');throw 'unexpected unsafe arguments accepted'}catch{if($_.Exception.Message -notlike '*NATIVE_ARGUMENT_DENY*'){throw}}
$retained=$false
try{Invoke-InheritedConsoleNative "$q\topology-debug.exe" @('--text-child') | ForEach-Object {throw 'FIXTURE_CAPTURE_STOP'}}catch{if($_.Exception.Message -notlike '*FIXTURE_CAPTURE_STOP*'){throw}}
if($null -eq $script:RetainedNativeProcess){throw 'capture stop incorrectly released owner'}
$retained=$true
# Fixture-only reconciliation AFTER factual exit; no production/native admission refs.
$script:RetainedNativeProcess.WaitForExit()
$script:RetainedNativeProcess.StandardOutput.Dispose()
$script:RetainedNativeProcess.Dispose()
$script:RetainedNativeProcess=$null
$results+=@{case='downstream_capture_stop';retained_before_factual_exit=$retained;release='FIXTURE_ONLY_AFTER_WAIT_FOR_EXIT';result='PASS'}
$results | ConvertTo-Json -Depth 5 | Set-Content "$q\managed-function-results.json" -Encoding UTF8
'INHERITED_CONSOLE_FUNCTION=PASS; WINDOWS_POWERSHELL5; Debug/Release; FIRST_ERROR_CAPTURE; NO_STARTED_PROCESS_ON_CREATE_FAILURE; UNSAFE_ARGUMENT_DENY; NO_LAUNCHER_NO_SCM'
