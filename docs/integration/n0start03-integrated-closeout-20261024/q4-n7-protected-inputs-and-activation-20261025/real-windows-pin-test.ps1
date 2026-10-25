param([ValidateSet('Debug','Release')][string]$FixtureConfig='Release')
$ErrorActionPreference='Stop';Set-StrictMode -Version Latest
$pkg='C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\q4-n7-protected-inputs-and-activation-20261025'
$scriptPath=$pkg+'\q4-input-placement.CANDIDATE.ps1';$sha=(Get-FileHash -LiteralPath $scriptPath -Algorithm SHA256).Hash.ToLowerInvariant()
. $scriptPath -Mode ModelTest -FixtureConfig $FixtureConfig -ExpectedScriptSha256 $sha
$owner=[PaccN2PlacementNative]::new()
$flags=[Reflection.BindingFlags]::Instance-bor[Reflection.BindingFlags]::NonPublic
$pin=[PaccN2PlacementNative].GetMethod('PinWindowsFile',$flags)
if($null-eq$pin){throw 'PIN_METHOD_BOUNDARY'}
[void]$pin.Invoke($owner,@())
$owned=[PaccN2PlacementNative].GetField('owned',$flags).GetValue($owner)
if($owned.Count-ne1-or$owner.EffectsCreated-or$owner.Retained){throw 'READONLY_OWNERSHIP_EXPECTED_ONE'}
$closed=$owner.Close()
if(-not$closed-or$owner.Retained-or$owned.Count-ne0){$script:RetainedReadonlyFixture=$owner;throw 'READONLY_WINDOWS_CLOSE_UNKNOWN_NO_RETRY'}
[Console]::Out.WriteLine('Q4_NATIVE_WINDOWS_PIN=PASS; OWNED1_TO0_CHECKED_CLOSE; PROTECTED_ROOT_NOT_OPENED; NO_PLACEMENT_NO_OPERATOR_QUALIFICATION')
