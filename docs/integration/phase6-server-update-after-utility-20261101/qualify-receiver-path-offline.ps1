param([string]$Profile,[string]$Configuration)
$ErrorActionPreference='Stop'
$folder='docs/integration/phase6-server-update-after-utility-20261101'
& cmd.exe /c (Join-Path (Get-Location) "$folder/build-working-growth-offline.cmd") $Profile $Configuration > "$folder/receiver-path-$Profile-$Configuration-build.log" 2>&1
if($LASTEXITCODE){throw 'RECEIVER_PATH_BUILD_FAILED'}
$filter='^(papacc\.personal\.(scq_transport_continuation|scq_pre_readiness_chain|scq_recipient_close|scq_native_adapter|scq_session_driver|scq_query_cold_chain|scq_bootstrap_capture|scq_p1_entry|scq_start_assurance|scq_start_progress|scq_start_take_image|scq_cold_open|scq_cold_controller|scq_native_selection_capture|scq_private_profile|scq_dispatch|runtime_identity)|papacc\.service\.closed_empty_model|papacc\.phase6\.memory_census)$'
ctest --test-dir "build/windows-virtual-scm-integration-20261022/$Profile/$Configuration" -R $filter --output-on-failure -V > "$folder/receiver-path-$Profile-$Configuration-tests.log" 2>&1
if($LASTEXITCODE){throw 'RECEIVER_PATH_TEST_FAILED'}
Write-Output "PASS RECEIVER_PATH $Profile $Configuration"
