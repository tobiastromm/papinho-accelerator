param([string]$Profile,[string]$Configuration)
$ErrorActionPreference='Stop'
$folder='docs/integration/phase6-server-update-after-utility-20261101'
& cmd.exe /c (Join-Path (Get-Location) 'docs\integration\phase6-server-update-after-utility-20261101\build-working-growth-offline.cmd') $Profile $Configuration > "$folder/working-growth-$Profile-$Configuration-build.log" 2>&1
if($LASTEXITCODE){throw "BUILD_FAILED $Profile $Configuration"}
ctest --test-dir "build/windows-virtual-scm-integration-20261022/$Profile/$Configuration" -R '^(papacc\.personal\.scq_transport_continuation|papacc\.personal\.scq_pre_readiness_chain|papacc\.personal\.scq_recipient_close|papacc\.personal\.scq_native_adapter|papacc\.personal\.scq_session_driver|papacc\.personal\.scq_query_cold_chain|papacc\.personal\.scq_bootstrap_capture|papacc\.personal\.scq_p1_entry|papacc\.personal\.scq_start_assurance|papacc\.personal\.scq_start_progress|papacc\.personal\.scq_start_take_image|papacc\.personal\.scq_cold_open|papacc\.personal\.scq_cold_controller|papacc\.personal\.scq_native_selection_capture|papacc\.service\.closed_empty_model)$' --output-on-failure -V > "$folder/working-growth-$Profile-$Configuration-tests.log" 2>&1
if($LASTEXITCODE){throw "TEST_FAILED $Profile $Configuration"}
Write-Output "PASS $Profile $Configuration"
