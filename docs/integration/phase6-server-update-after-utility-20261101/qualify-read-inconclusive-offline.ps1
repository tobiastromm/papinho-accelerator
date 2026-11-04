param([string]$Profile,[string]$Configuration)
$ErrorActionPreference='Stop'
$f='docs/integration/phase6-server-update-after-utility-20261101'
& cmd.exe /c (Join-Path (Get-Location) "$f/build-read-inconclusive-offline.cmd") $Profile $Configuration > "$f/read-inconclusive-final6-$Profile-$Configuration-build.log" 2>&1
if($LASTEXITCODE){throw 'READ_TRANSPORT_CAUSE_BUILD_FAILED'}
ctest --test-dir "build/windows-virtual-scm-integration-20261022/$Profile/$Configuration" -R '^(papacc\.personal\.(scq_transport_continue_wrapper|scq_offer_candidate|scq_originals_candidate|machine_fixture_source|machine_fixture_source_observed_sd|scq_machine_fixture|scq_first_controller|scq_machine_read_chain|scq_machine_read_chain_observed_sd|scq_read_runtime|cng_probe|scq_start_assurance|scq_cold_controller|scq_private_profile|scq_start_selection|scq_pre_readiness_chain|scq_recipient_close|scq_start_progress|scq_session_driver|scq_dispatch)|papacc\.personal_ca\.lifetime|papacc\.phase6\.memory_census)$' --output-on-failure -V > "$f/read-inconclusive-final6-$Profile-$Configuration-tests.log" 2>&1
if($LASTEXITCODE){throw 'READ_TRANSPORT_CAUSE_TEST_FAILED'}
"PASS READ_TRANSPORT_CAUSE $Profile $Configuration"
