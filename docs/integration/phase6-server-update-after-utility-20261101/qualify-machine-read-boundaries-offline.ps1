param([string]$Profile,[string]$Configuration)
$ErrorActionPreference='Stop'
$f='docs/integration/phase6-server-update-after-utility-20261101'
& cmd.exe /c (Join-Path (Get-Location) "$f/build-read-runtime-offline.cmd") $Profile $Configuration > "$f/machine-read-boundary-$Profile-$Configuration-build.log" 2>&1
if($LASTEXITCODE){throw 'MACHINE_READ_BOUNDARY_BUILD_FAILED'}
$filter='^(papacc\.personal\.(install_paths|scq_admin_source|scq_first_controller|scq_endpoint_act|scq_start_selection|scq_native_first_ingress_boundary|scq_start_runtime_diagnostic)|papacc\.scq\.runtime_token_shape)$'
ctest --test-dir "build/windows-virtual-scm-integration-20261022/$Profile/$Configuration" -R $filter --output-on-failure -V > "$f/machine-read-boundary-$Profile-$Configuration-tests.log" 2>&1
if($LASTEXITCODE){throw 'MACHINE_READ_BOUNDARY_TEST_FAILED'}
"PASS MACHINE_READ_BOUNDARY $Profile $Configuration"
