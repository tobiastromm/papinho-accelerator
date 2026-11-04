$ErrorActionPreference = 'Stop'
$queryRoot = 'docs/integration/phase6-query-g3-accepted-20261031'
$queryUtf8 = New-Object System.Text.UTF8Encoding($false)
$queryBuilds = @()
foreach ($queryProfile in @('Selected','Legacy')) {
 foreach ($queryConfiguration in @('Debug','Release')) {
  $queryLabel = "$queryProfile-$queryConfiguration"
  $queryBuildLog = "$queryRoot/$queryLabel-final-build.log"
  $queryTestLog = "$queryRoot/$queryLabel-tests.log"
  if ((Get-Content -LiteralPath $queryBuildLog -Raw) -match '(?im)\bwarning [A-Z]+\d+|fatal error|FAILED:') { throw "BUILD_NOT_CLEAN_$queryLabel" }
  if ((Get-Content -LiteralPath $queryTestLog -Raw) -notmatch '100% tests passed, 0 tests failed out of 28') { throw "TEST_DENY_$queryLabel" }
  $queryBuilds += [ordered]@{profile=$queryLabel;build='PASS';warnings=0;tests='PASS';test_configuration_pairs=28;build_log=$queryBuildLog;test_log=$queryTestLog}
 }
}
$queryOriginal = 'docs/integration/phase6-receiver-admission-g3-20261031/INITIAL-QUERY-AND-G3.CANDIDATE.md'
if ((Get-FileHash -LiteralPath $queryOriginal -Algorithm SHA256).Hash -ne '5b77ddae089ba496c511a562575e22642a29555e536fd4563e5e5ebb8faefeca') { throw 'APPROVED_PROPOSAL_HASH_DENY' }
$queryDelta = "$queryRoot/PRIVATE-DEADLINE-DELTA.REQUEST.md"
if ((Get-FileHash -LiteralPath $queryDelta -Algorithm SHA256).Hash -ne '62548c758282dd6b828b451a01a92bc4a369843ac55f7cc9c52f8c84a25ae11c') { throw 'DELTA_REQUEST_HASH_DENY' }
$queryPaths = @(
 'CMakeLists.txt','apps/server/personal_scq_bootstrap_win32.inc',
 'apps/server/personal_scq_start_assurance_win32.h','apps/server/personal_scq_start_assurance_win32.inc',
 'apps/server/personal_scq_transport_win32.h','apps/server/personal_scq_transport_win32.inc',
 'apps/server/personal_scq_cold_acquisition_win32.inc','apps/server/server_service_data.h',
 'apps/server/server_service_win32.c','apps/server/server_service_startup_win32.inc',
 'apps/server/server_configuration_personal_native_win32.inc',
 'tests/scq_query_cold_chain_test.c','tests/scq_start_assurance_test.c','tests/scq_bootstrap_capture_test.c',
 'docs/adr/DECISION-personal-scq-initial-query-g3-20261031.md',
 'docs/capabilities/phase6-scope-and-closure-20261028.md',$queryOriginal,
 'build/windows-virtual-scm-integration-20261022/Selected/Release/papacc_server.exe',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/libssl-3-x64.dll',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/libcrypto-3-x64.dll'
)
$queryPaths += Get-ChildItem -LiteralPath $queryRoot -File | Where-Object Name -ne 'EVIDENCE.json' | ForEach-Object { "$queryRoot/$($_.Name)" }
$queryFiles = @($queryPaths | Sort-Object -Unique | ForEach-Object {
 [ordered]@{path=$_;bytes=(Get-Item -LiteralPath $_).Length;sha256=(Get-FileHash -LiteralPath $_ -Algorithm SHA256).Hash.ToLowerInvariant()}
})
$queryResult = [ordered]@{
 task='PACC-PHASE6-QUERY-G3-ACCEPTANCE-AND-COLD-CHAIN';date='2026-11-01';branch='main';head='f341985924d29870d8ff627ed37c8a0874ad6469';
 normative='QUERY_G3_ACCEPTED';values='SELECTED_UNCHANGED';query='NATIVE_CALLER_INTEGRATED_OFFLINE';
 G3_binding='NORMAL_GLOBAL_DEADLINE_AUTHENTICITY_INCOMPLETE';cold_chain='GENERAL_CANONICAL_CHAIN_NOT_COMPLETE';
 canonical_preStart_guard='DENY_PRESERVED';coexpiry_component='PASS_MODELED_ONLY_NOT_OPERATIONAL_STRATEGY';
 deadline_delta='CANDIDATE_NOT_APPROVED';deadline_delta_sha256='62548c758282dd6b828b451a01a92bc4a369843ac55f7cc9c52f8c84a25ae11c';
 auto_review_rejections=@('Direct initial-receiver-source guard removal rejected; unchanged','SQB2 and buffer growth rejected twice including B3/SCQ1 scope evidence; not applied');
 operational_request='NOT_READY';authority='DENY';E21='PENDING';ED11='PARTIAL';phase6='PARTIAL';os_quiescence='UNKNOWN';
 operational_effects=0;native_bilateral='NOT_DEMONSTRATED';staged_updated=$false;builds=$queryBuilds;test_configuration_pairs=112;
 selected_G3=[ordered]@{handles=128;guard=124;private_growth_bytes=1994944;working_growth_bytes=1994944;physical_floor_bytes=7239172096;commit_floor_bytes=9823326208;freshness_ms=100;authorization_ms=240000;global_duration_ms=60000};
 model=[ordered]@{query_cases=20;query_checks=1017;coexpiry_readiness_before_fault=3;coexpiry_known_final_close=2;query_resource_cases=15;assurance_cases=33;assurance_checks=555;quantitative_field_mutations=8;protected_APIs='MODELED'};
 census=[ordered]@{selected_known_bytes=1994976;selected_process_limit_bytes=3145728;increment_known_bytes=32;G3_growth_unchanged=1994944;opaque_costs='UNKNOWN'};
 files=$queryFiles
}
[IO.File]::WriteAllText((Join-Path (Get-Location) "$queryRoot/EVIDENCE.json"),($queryResult | ConvertTo-Json -Depth 10),$queryUtf8)
Write-Output 'OFFLINE_EVIDENCE_CAPTURED; NO_NATIVE_SESSION_NO_PROTECTED_OBSERVATION'
