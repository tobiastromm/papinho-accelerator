param(
 [Parameter(Mandatory=$true)][ValidateSet('Selected','Legacy')][string]$Profile,
 [Parameter(Mandatory=$true)][ValidateSet('Debug','Release')][string]$Configuration,
 [ValidatePattern('^[a-z0-9-]+$')][string]$Round='final4'
)
$ErrorActionPreference='Stop'
$folder='docs/integration/phase6-server-update-after-utility-20261101'
& cmd.exe /c (Join-Path (Get-Location) "$folder/build-fca-offline.cmd") $Profile $Configuration > "$folder/fca-$Round-$Profile-$Configuration-build.log" 2>&1
if($LASTEXITCODE){throw 'FCA_OFFLINE_BUILD_DENY'}
$tests='^(papacc\.personal\.(scq_transport_continue_wrapper|scq_offer_candidate|scq_originals_candidate|machine_fixture_source|machine_fixture_source_observed_sd|scq_machine_fixture|scq_first_controller|scq_machine_read_chain|scq_machine_read_chain_observed_sd|scq_read_runtime|cng_probe|scq_start_assurance|scq_cold_controller|scq_private_profile|scq_start_selection|scq_pre_readiness_chain|scq_recipient_close|scq_start_progress|scq_session_driver|scq_dispatch|token_predicates|scq_cold_open|scq_native_sd)|papacc\.personal_ca\.(lifetime|native|model)|papacc\.phase6\.memory_census|papacc\.personal_first_prepare\.(model|entry)|papacc\.personal_effect_plan\.data)$'
ctest --test-dir "build/windows-virtual-scm-integration-20261022/$Profile/$Configuration" -R $tests --output-on-failure -V > "$folder/fca-$Round-$Profile-$Configuration-tests.log" 2>&1
if($LASTEXITCODE){throw 'FCA_OFFLINE_MODEL_DENY'}
"PASS FCA OFFLINE $Profile $Configuration; NO_OPERATIONAL_COMMAND_INVOKED"
