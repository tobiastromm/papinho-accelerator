$ErrorActionPreference = 'Stop'
# Repository evidence only; never invokes the installed EXE or native owner.
$evidenceDirectory = $PSScriptRoot
$configurationRows = @()
foreach ($profileName in @('Selected','Legacy')) {
 foreach ($configurationName in @('Debug','Release')) {
  $configurationLabel = "$profileName-$configurationName"
  $detailText = Get-Content -Raw -LiteralPath (Join-Path $evidenceDirectory "$configurationLabel-details.log")
  $buildText = Get-Content -Raw -LiteralPath (Join-Path $evidenceDirectory "$configurationLabel-build.log")
  $testText = Get-Content -Raw -LiteralPath (Join-Path $evidenceDirectory "$configurationLabel-tests.log")
  if ($buildText -match '(?im)warning C|error C|FAILED:') { throw "BUILD_DIAGNOSTIC_$configurationLabel" }
  if ($testText -notmatch '100% tests passed, 0 tests failed out of 14') { throw "TEST_COUNT_$configurationLabel" }
  if ($detailText -notmatch 'cases=102 checks=602') { throw "UPDATE_CASES_$configurationLabel" }
  $executablePath = "build/windows-virtual-scm-integration-20261022/$profileName/$configurationName/papacc_server.exe"
  $configurationRows += [ordered]@{
   configuration=$configurationLabel
   exe_bytes=(Get-Item -LiteralPath $executablePath).Length
   exe_sha256=(Get-FileHash -LiteralPath $executablePath -Algorithm SHA256).Hash.ToLowerInvariant()
   build='PASS_ZERO_WARNINGS'; affected_tests=14; update_scenarios=102; update_checks=602
   known=[int]([regex]::Match($detailText,'(?m)^KNOWN=(\d+)').Groups[1].Value)
   K=[int]([regex]::Match($detailText,'BINARY_UPDATE_K=(\d+)').Groups[1].Value)
  }
 }
}
$configurationRows | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath (Join-Path $evidenceDirectory 'BUILD-TEST-SUMMARY.json') -Encoding UTF8
$evidencePaths = @(
 'docs/adr/DECISION-personal-update-executor-data-u2-20261101.md',
 'docs/integration/phase6-binary-update-u1-u7-20261101/U2-EXECUTOR-AND-CANDIDATE-DATA.CANDIDATE.md',
 'apps/server/personal_staged_update_win32.h',
 'apps/server/server_launcher_staged_update_win32.inc',
 'apps/server/server_launcher_staged_update_effects_win32.inc',
 'tests/staged_binary_update_test.c',
 'docs/capabilities/phase6-scope-and-closure-20261028.md',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/papacc_server.exe',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/libssl-3-x64.dll',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/libcrypto-3-x64.dll'
)
$evidencePaths += Get-ChildItem -LiteralPath $evidenceDirectory -File | Where-Object Name -ne 'EVIDENCE.json' | ForEach-Object FullName
$manifest = [ordered]@{
 task='PACC-PHASE6-U2-EXECUTOR-DATA-ACCEPTANCE-AND-INTEGRATION'
 normative='ACCEPTED_EXACT_58493c93'; implementation='EXECUTOR_ORIGINAL_GATE_AND_DATA_POLICY_IN_EXISTING_OWNER'
 executor_operationally_qualified=$false; operational_request='NOT_READY_MISSING_INDEPENDENT_PROTECTED_CAPABLE_EXECUTOR'
 candidate_as_executor='DENIED'; previous_attempt='USED_NO_RETRY'
 real_update_effects=0; real_start_stop=0; real_scm_queries=0
 static_installed_artifact_read=$true; mapped_image_attestation='NOT_DEMONSTRATED'
 distinct_test_configuration_pairs=56; builds=$configurationRows
 data_policy='ONLY_AFTER_EXECUTOR_WITH_FULL_OWNER_DACL_ACE_INSPECTION_AND_ORIGINAL_HANDLE_HASH_CONTINUITY'
 os_quiescence='UNKNOWN'; E21='PENDING'; ED11='PARTIAL'; phase6='PARTIAL'; production_pst='0.6.4_UNCHANGED'
 files=@($evidencePaths | Sort-Object -Unique | ForEach-Object {
  [ordered]@{path=$_;bytes=(Get-Item -LiteralPath $_).Length;sha256=(Get-FileHash -LiteralPath $_ -Algorithm SHA256).Hash.ToLowerInvariant()}
 })
}
$manifest | ConvertTo-Json -Depth 10 | Set-Content -LiteralPath (Join-Path $evidenceDirectory 'EVIDENCE.json') -Encoding UTF8
