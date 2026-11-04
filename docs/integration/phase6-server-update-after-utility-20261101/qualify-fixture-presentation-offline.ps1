param([string]$Profile,[string]$Configuration)
$ErrorActionPreference='Stop'
$f='docs/integration/phase6-server-update-after-utility-20261101'
& cmd.exe /c (Join-Path (Get-Location) "$f/build-fixture-presentation-offline.cmd") $Profile $Configuration > "$f/fixture-presentation-final-$Profile-$Configuration-build.log" 2>&1
if($LASTEXITCODE){throw 'FIXTURE_PRESENTATION_BUILD_FAILED'}
ctest --test-dir "build/windows-virtual-scm-integration-20261022/$Profile/$Configuration" -R '^(papacc\.personal\.(machine_fixture_source|scq_first_controller|scq_machine_read_chain)|papacc\.phase6\.memory_census)$' --output-on-failure -V > "$f/fixture-presentation-final-$Profile-$Configuration-tests.log" 2>&1
if($LASTEXITCODE){throw 'FIXTURE_PRESENTATION_TEST_FAILED'}
"PASS FIXTURE_PRESENTATION $Profile $Configuration"
