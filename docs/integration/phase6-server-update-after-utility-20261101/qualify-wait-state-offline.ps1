param([string]$Profile,[string]$Configuration)
$ErrorActionPreference='Stop'
$folder=$PSScriptRoot
& cmd.exe /c (Join-Path $folder 'build-wait-state-offline.cmd') $Profile $Configuration > "$folder/wait-state-$Profile-$Configuration-build.log" 2>&1
if($LASTEXITCODE){throw 'WAIT_STATE_BUILD_FAILED'}
ctest --test-dir "build/windows-virtual-scm-integration-20261022/$Profile/$Configuration" -R '^papacc\.personal\.scq_start_progress$' -V --output-on-failure > "$folder/wait-state-$Profile-$Configuration-tests.log" 2>&1
if($LASTEXITCODE){throw 'WAIT_STATE_TEST_FAILED'}
Write-Output "PASS WAIT_STATE $Profile $Configuration"
