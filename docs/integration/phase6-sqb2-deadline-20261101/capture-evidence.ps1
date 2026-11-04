$ErrorActionPreference = 'Stop'
$sqbRoot = 'docs/integration/phase6-sqb2-deadline-20261101'
$sqbUtf8 = New-Object System.Text.UTF8Encoding($false)
$sqbBuilds = @()
foreach ($sqbProfile in @('Selected','Legacy')) {
 foreach ($sqbConfiguration in @('Debug','Release')) {
  $sqbLabel = "$sqbProfile-$sqbConfiguration"
  $sqbBuildLog = "$sqbRoot/$sqbLabel-final-build.log"
  $sqbTestLog = "$sqbRoot/$sqbLabel-tests.log"
  if ((Get-Content -LiteralPath $sqbBuildLog -Raw) -match '(?im)\bwarning [A-Z]+\d+|fatal error|FAILED:') { throw "BUILD_NOT_CLEAN_$sqbLabel" }
  if ((Get-Content -LiteralPath $sqbTestLog -Raw) -notmatch '100% tests passed, 0 tests failed out of 28') { throw "TEST_DENY_$sqbLabel" }
  $sqbBuilds += [ordered]@{profile=$sqbLabel;build='PASS';warnings=0;tests='PASS';test_configuration_pairs=28;build_log=$sqbBuildLog;test_log=$sqbTestLog}
 }
}
$sqbApproved = 'docs/integration/phase6-query-g3-accepted-20261031/PRIVATE-DEADLINE-DELTA.REQUEST.md'
if ((Get-FileHash -LiteralPath $sqbApproved -Algorithm SHA256).Hash -ne '62548c758282dd6b828b451a01a92bc4a369843ac55f7cc9c52f8c84a25ae11c') { throw 'SQB2_APPROVED_HASH_DENY' }
$sqbBridge = "$sqbRoot/PRESTART-CONTINUATION-BINDING.REQUEST.md"
if ((Get-FileHash -LiteralPath $sqbBridge -Algorithm SHA256).Hash -ne '9637246ad9ad6de34ff9eb5cb9440e628a14909ea7f95cb6a2ba2b7265287f68') { throw 'BRIDGE_REQUEST_HASH_DENY' }
$sqbPaths = @(
 'CMakeLists.txt','apps/server/personal_scq_bootstrap_win32.inc',
 'apps/server/personal_scq_transport_win32.h','apps/server/personal_scq_transport_win32.inc',
 'apps/server/personal_scq_session_win32.h','apps/server/personal_scq_start_assurance_win32.h',
 'apps/server/personal_scq_start_assurance_win32.inc','apps/server/server_service_data.h','apps/server/server_service_data.c',
 'apps/server/server_service_win32.c','apps/server/server_service_startup_win32.inc','apps/server/server_launcher_owner_win32.c',
 'apps/server/server_configuration_personal_native_win32.inc','tests/scq_query_cold_chain_test.c',
 'tests/scq_bootstrap_carrier_test.c','tests/scq_native_adapter_test.c',
 'docs/adr/DECISION-personal-scq-private-deadline-sqb2-20261101.md',
 'docs/adr/DECISION-personal-scq-initial-query-g3-20261031.md','docs/capabilities/phase6-scope-and-closure-20261028.md',$sqbApproved,
 'build/windows-virtual-scm-integration-20261022/Selected/Release/papacc_server.exe',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/libssl-3-x64.dll',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/libcrypto-3-x64.dll'
)
$sqbPaths += Get-ChildItem -LiteralPath $sqbRoot -File | Where-Object Name -ne 'EVIDENCE.json' | ForEach-Object { "$sqbRoot/$($_.Name)" }
$sqbFiles = @($sqbPaths | Sort-Object -Unique | ForEach-Object {
 [ordered]@{path=$_;bytes=(Get-Item -LiteralPath $_).Length;sha256=(Get-FileHash -LiteralPath $_ -Algorithm SHA256).Hash.ToLowerInvariant()}
})
$sqbResult = [ordered]@{
 task='QUERY_G3_SQB2_COLD_CHAIN';date='2026-11-01';branch='main';head='f341985924d29870d8ff627ed37c8a0874ad6469';
 normative='QUERY_G3_AND_SQB2_ACCEPTED';G3_values='SELECTED_UNCHANGED';SQB2='INTEGRATED_OFFLINE';
 global_deadline_binding='PASS_MODELED_GENERAL_CASE';canonical_prestart='DENY_GUARD_UNCHANGED';
 prestart_approval='ATTRIBUTED_REPLY_APPROVED_ORIGINAL_BUT_SUBSEQUENT_EXPLICIT_HASH_DIVERGES_RECONCILIATION_PENDING';
 prestart_actual_sha256='9637246ad9ad6de34ff9eb5cb9440e628a14909ea7f95cb6a2ba2b7265287f68';
 prestart_subsequent_written_sha256='9637246ad9ad6e34ff9eb5cb9440e628a14909ea7f95cb6a2ba2b7265287f687';
 cold_chain='CORE_MODEL_PASS_CANONICAL_PRESTART_LINK_BLOCKED';native_bilateral='NOT_DEMONSTRATED';
 authority='DENY';operational_request='NOT_READY';E21='PENDING';ED11='PARTIAL';phase6='PARTIAL';os_quiescence='UNKNOWN';
 operational_effects=0;staged_updated=$false;builds=$sqbBuilds;test_configuration_pairs=112;
 formats=[ordered]@{SCQ1_bytes=308;SCQ1_chars=622;record_bytes=1048;SQB1_bytes=1076;SQR1_bytes=1052;SQC1_bytes=1076;SQB2_bytes=1084;extra_bootstrap_buffer_bytes=8;public_persisted_formats='UNCHANGED'};
 selected_G3=[ordered]@{handles=128;guard=124;private_growth_bytes=1994944;working_growth_bytes=1994944;physical_floor_bytes=7239172096;commit_floor_bytes=9823326208;freshness_ms=100;authorization_ms=240000;global_duration_ms=60000};
 model=[ordered]@{query_cases=27;query_checks=1430;readiness_before_injected_fault=6;known_final_close=5;carrier_metadata_cases=7;carrier_checks=97;assurance_checks=555;protected_APIs='MODELED'};
 census=[ordered]@{selected_known_bytes=1994992;selected_process_limit_bytes=3145728;legacy_known_bytes=1826424;legacy_process_limit_bytes=2097152;increment_SQB2_known_bytes=16;G3_growth_unchanged=1994944;opaque_costs='UNKNOWN'};
 files=$sqbFiles
}
[IO.File]::WriteAllText((Join-Path (Get-Location) "$sqbRoot/EVIDENCE.json"),($sqbResult | ConvertTo-Json -Depth 10),$sqbUtf8)
Write-Output 'SQB2_OFFLINE_EVIDENCE_CAPTURED; PRESTART_DENY; NO_OPERATIONAL_QUERY_OR_EXECUTION'
