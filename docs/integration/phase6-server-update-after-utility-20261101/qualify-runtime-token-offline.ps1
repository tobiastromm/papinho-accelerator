param([string]$Profile,[string]$Configuration)
$ErrorActionPreference='Stop'
$folder='docs/integration/phase6-server-update-after-utility-20261101'
& cmd.exe /c (Join-Path (Get-Location) 'docs\integration\phase6-server-update-after-utility-20261101\build-runtime-token-offline.cmd') $Profile $Configuration > "$folder/runtime-token-$Profile-$Configuration-build.log" 2>&1
if($LASTEXITCODE){throw "BUILD_FAILED $Profile $Configuration"}
ctest --test-dir "build/windows-virtual-scm-integration-20261022/$Profile/$Configuration" -R '^(papacc\.phase6\.memory_census|papacc\.personal\.runtime_identity|papacc\.personal\.scq_private_profile|papacc\.personal\.scq_dispatch|papacc\.personal\.scq_transport_continuation|papacc\.personal\.scq_pre_readiness_chain|papacc\.personal\.scq_recipient_close|papacc\.personal\.scq_native_adapter|papacc\.personal\.scq_session_driver|papacc\.personal\.scq_query_cold_chain|papacc\.personal\.scq_bootstrap_capture|papacc\.personal\.scq_admin_source|papacc\.personal\.scq_p1_entry|papacc\.personal\.scq_start_assurance|papacc\.personal\.scq_start_progress|papacc\.personal\.scq_start_take_image|papacc\.personal\.scq_start_runtime_diagnostic|papacc\.scq\.runtime_token_shape|papacc\.service\.closed_empty_model|papacc\.service\.host_product_model|papacc\.service\.host_lab_model)$' --output-on-failure -V > "$folder/runtime-token-$Profile-$Configuration-tests.log" 2>&1
if($LASTEXITCODE){throw "TEST_FAILED $Profile $Configuration"}
Write-Output "PASS $Profile $Configuration"
