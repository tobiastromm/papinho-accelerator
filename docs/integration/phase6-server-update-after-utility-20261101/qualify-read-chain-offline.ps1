param([string]$Profile,[string]$Configuration)
$ErrorActionPreference='Stop'
$f='docs/integration/phase6-server-update-after-utility-20261101'
& cmd.exe /c (Join-Path (Get-Location) "$f/build-read-chain-all-offline.cmd") $Profile $Configuration > "$f/read-chain-final-$Profile-$Configuration-build.log" 2>&1
if($LASTEXITCODE){throw 'READ_RUNTIME_BUILD_FAILED'}
$filter='^(papacc\.personal\.(install_paths|scq_admin_source|scq_first_controller|scq_endpoint_act|scq_start_selection|scq_native_first_ingress_boundary|scq_start_runtime_diagnostic|scq_machine_read_chain|machine_fixture_source|scq_read_runtime|scq_read_diagnostic|scq_machine_fixture|cng_probe|scq_transport_continuation|scq_pre_readiness_chain|scq_recipient_close|scq_native_adapter|scq_session_driver|scq_query_cold_chain|scq_failure_ingress|scq_inputs_metadata|scq_staged_owner_boundary|scq_to_image_owner|scq_existing_target|scq_bootstrap_capture|scq_p1_entry|scq_start_assurance|scq_start_progress|scq_start_take_image|scq_cold_open|scq_cold_controller|scq_native_selection_capture|scq_private_profile|scq_dispatch|runtime_identity)|papacc\.scq\.runtime_token_shape|papacc\.personal_ca\.lifetime|papacc\.service\.closed_empty_model|papacc\.phase6\.memory_census)$'
ctest --test-dir "build/windows-virtual-scm-integration-20261022/$Profile/$Configuration" -R $filter --output-on-failure -V > "$f/read-chain-final-$Profile-$Configuration-tests.log" 2>&1
if($LASTEXITCODE){throw 'READ_RUNTIME_TEST_FAILED'}
"PASS READ_CHAIN_FINAL $Profile $Configuration"
