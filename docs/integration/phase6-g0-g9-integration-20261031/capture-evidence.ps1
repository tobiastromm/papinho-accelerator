$ErrorActionPreference = 'Stop'
$evidenceDirectory = 'docs/integration/phase6-g0-g9-integration-20261031'
$utf8Evidence = New-Object System.Text.UTF8Encoding($false)
$buildEvidence = @()
foreach ($evidenceProfile in @('Selected','Legacy')) {
 foreach ($evidenceConfiguration in @('Debug','Release')) {
  $evidenceLabel = "$evidenceProfile-$evidenceConfiguration"
  $buildLog = "$evidenceDirectory/$evidenceLabel-final-build.log"
  $testLog = "$evidenceDirectory/$evidenceLabel-tests.log"
  $buildText = Get-Content -LiteralPath $buildLog -Raw
  $testText = Get-Content -LiteralPath $testLog -Raw
  if ($buildText -match '(?im)\bwarning [A-Z]+\d+|fatal error|FAILED:') { throw "BUILD_NOT_CLEAN_$evidenceLabel" }
  if ($testText -notmatch '100% tests passed, 0 tests failed out of 27') { throw "TEST_RESULT_DENY_$evidenceLabel" }
  $lastTest = "build/windows-virtual-scm-integration-20261022/$evidenceProfile/$evidenceConfiguration/Testing/Temporary/LastTest.log"
  [IO.File]::Copy((Join-Path (Get-Location) $lastTest),(Join-Path (Get-Location) "$evidenceDirectory/$evidenceLabel-details.log"),$true)
  $buildEvidence += [ordered]@{ profile=$evidenceLabel; build='PASS'; warnings=0; test_configuration_pairs=27; tests='PASS'; build_log=$buildLog; test_log=$testLog }
 }
}
$evidencePaths = @(
 'CMakeLists.txt','apps/server/main.c.api3.inc','apps/server/personal_native_win32.h',
 'apps/server/personal_scq_cold_acquisition_win32.inc','apps/server/personal_scq_cold_start_win32.h',
 'apps/server/personal_scq_cold_start_win32.inc','apps/server/personal_scq_start_assurance_win32.h',
 'apps/server/personal_scq_start_assurance_win32.inc','apps/server/server_launcher_scq_cold_start_win32.inc',
 'apps/server/server_launcher_owner_win32.c','apps/server/server_launcher_owner_win32.h',
 'apps/server/server_configuration_personal_native_win32.inc','apps/server/server_configuration_commit_win32.h','apps/server/server_service_win32.h',
 'apps/server/server_service_startup_win32.h','apps/server/server_service_startup_win32.inc','apps/server/server_service_win32.c',
 'tests/scq_start_assurance_test.c','tests/scq_cold_controller_test.c','tests/scq_cold_open_test.c','tests/scq_first_controller_test.c',
 'docs/adr/DECISION-personal-first-diagnostic-start-assurance-g0-g9-20261031.md',
 'docs/integration/phase6-bootstrap-source-assurance-20261031/FIRST-DIAGNOSTIC-START-ASSURANCE.CANDIDATE.md',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/papacc_server.exe',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/libssl-3-x64.dll',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/libcrypto-3-x64.dll'
)
$evidencePaths += Get-ChildItem -LiteralPath $evidenceDirectory -File | Where-Object Name -ne 'EVIDENCE.json' | ForEach-Object { "$evidenceDirectory/$($_.Name)" }
$evidenceHashes = @($evidencePaths | Sort-Object -Unique | ForEach-Object {
 $evidenceFile = Get-Item -LiteralPath $_
 [ordered]@{ path=$_; bytes=$evidenceFile.Length; sha256=(Get-FileHash -LiteralPath $_ -Algorithm SHA256).Hash.ToLowerInvariant() }
})
$evidenceResult = [ordered]@{
 task='PACC-PHASE6-G0-G9-ACCEPTANCE-AND-COLD-CHAIN-COMPLETION'; branch='main'; head='f341985924d29870d8ff627ed37c8a0874ad6469';
 normative='G0_G9_ACCEPTED'; controller='NATIVE_CALLER_INTEGRATED_OFFLINE'; receiver='INDEPENDENT_PRE_ENDPOINT_SOURCE_ABSENT';
 cold_chain='NOT_COMPLETE'; operational_request='NOT_READY'; authority='DENY'; endpoint_created=$false; start_executed=$false; staged_updated=$false;
 G3_operational_values='NOT_SELECTED'; native_bilateral='NOT_DEMONSTRATED'; opaque_costs='UNKNOWN'; os_quiescence='UNKNOWN'; E21='PENDING'; ED11='PARTIAL'; phase6='PARTIAL';
 builds=$buildEvidence; final_test_configuration_pairs=108;
 new_scenarios_per_configuration=[ordered]@{ cold_open=13; cold_controller=17; start_assurance=33 };
 models='Native acquisition, console, RNG, SCM, pipe and sensitive effects modeled; no operational proof'; files=$evidenceHashes
}
[IO.File]::WriteAllText((Join-Path (Get-Location) "$evidenceDirectory/EVIDENCE.json"),($evidenceResult | ConvertTo-Json -Depth 8),$utf8Evidence)
Write-Output 'EVIDENCE_CAPTURED_FROM_EXISTING_LOGS_AND_WORKSPACE_FILES; NO_OPERATIONAL_OBSERVATION'
