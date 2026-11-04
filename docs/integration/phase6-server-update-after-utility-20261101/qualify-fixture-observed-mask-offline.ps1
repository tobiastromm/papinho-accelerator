param([string]$Profile,[string]$Configuration)
$ErrorActionPreference='Stop'
$f='docs/integration/phase6-server-update-after-utility-20261101'
& cmd.exe /c (Join-Path (Get-Location) "$f/build-fixture-sd-boundary-offline.cmd") $Profile $Configuration > "$f/fixture-observed-mask-final-$Profile-$Configuration-build.log" 2>&1
if($LASTEXITCODE){throw 'FIXTURE_SD_BOUNDARY_BUILD_FAILED'}
ctest --test-dir "build/windows-virtual-scm-integration-20261022/$Profile/$Configuration" -R '^(papacc\.personal\.(machine_fixture_source|scq_machine_fixture))$' --output-on-failure -V > "$f/fixture-observed-mask-final-$Profile-$Configuration-tests.log" 2>&1
if($LASTEXITCODE){throw 'FIXTURE_SD_BOUNDARY_TEST_FAILED'}
"PASS FIXTURE_SD_BOUNDARY $Profile $Configuration"
