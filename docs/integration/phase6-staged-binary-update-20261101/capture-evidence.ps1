$ErrorActionPreference = 'Stop'
$updateRoot = 'docs/integration/phase6-staged-binary-update-20261101'
$updateBuilds = @()
foreach ($updateProfile in @('Selected','Legacy')) {
 foreach ($updateConfiguration in @('Debug','Release')) {
  $updateLabel = "$updateProfile-$updateConfiguration"
  $updateBuild = Get-Content -LiteralPath "$updateRoot/$updateLabel-build.log" -Raw
  if ($updateBuild -match '(?im)\bwarning [A-Z]+\d+|fatal error|FAILED:') { throw "BUILD_NOT_CLEAN_$updateLabel" }
  if ((Get-Content -LiteralPath "$updateRoot/$updateLabel-tests.log" -Raw) -notmatch '100% tests passed, 0 tests failed out of 10') { throw "TEST_DENY_$updateLabel" }
  if ((Get-Content -LiteralPath "$updateRoot/$updateLabel-details.log" -Raw) -notmatch 'cases=33 checks=81 READONLY_ORIGINALS_SCM_MODELED') { throw "MODEL_DENY_$updateLabel" }
  if ((Get-Content -LiteralPath "$updateRoot/$updateLabel-final-tests.log" -Raw) -notmatch '100% tests passed, 0 tests failed out of 4') { throw "DOMAIN_DENY_$updateLabel" }
  $updateBuilds += [ordered]@{configuration=$updateLabel;build='PASS';warnings=0;affected_ctests=13;update_cases=33;update_checks=81;native_APIs='MODELED'}
 }
}
$updateOriginalRequest = 'docs/integration/phase6-sqb2-deadline-20261101/PRESTART-CONTINUATION-BINDING.REQUEST.md'
if ((Get-FileHash -LiteralPath $updateOriginalRequest -Algorithm SHA256).Hash -ne '9637246ad9ad6de34ff9eb5cb9440e628a14909ea7f95cb6a2ba2b7265287f68') { throw 'ORIGINAL_REQUEST_HASH_DENY' }
$updateCandidate = "$updateRoot/UPDATE-CONTRACT-AND-ASSURANCE.CANDIDATE.md"
$updatePaths = @('CMakeLists.txt','apps/server/personal_staged_update_win32.h','apps/server/personal_native_win32.h',
 'apps/server/server_launcher_staged_update_win32.inc','apps/server/server_launcher_owner_win32.c','apps/server/server_launcher_owner_win32.h',
 'apps/server/server_configuration_personal_native_win32.inc','apps/server/server_launcher_qualification_staging_win32.inc',
 'tests/staged_binary_update_test.c','tests/qualification_staging_test.c','tests/qualification_staging_close_guard_test.c',
 'docs/adr/DECISION-personal-first-install-authorization-f0-f9-20261022.md',
 'docs/adr/DECISION-v3-update-approval-u0-u7-u6-correction-20261021.md',
 'docs/adr/DECISION-ed11-preserve-incremental-resource-envelopes-20261012.md',
 'docs/adr/DECISION-ed11-windows-3mib-process-profile-20261020.md',
 'docs/capabilities/phase6-scope-and-closure-20261028.md',$updateOriginalRequest,
 'build/windows-virtual-scm-integration-20261022/Selected/Release/papacc_server.exe',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/libssl-3-x64.dll',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/libcrypto-3-x64.dll')
$updatePaths += Get-ChildItem -LiteralPath $updateRoot -File | Where-Object Name -ne 'EVIDENCE.json' | ForEach-Object { "$updateRoot/$($_.Name)" }
$updateEvidence = [ordered]@{
 task='GOVERNED_STAGED_BINARY_UPDATE';date='2026-11-01';branch='main';head='f341985924d29870d8ff627ed37c8a0874ad6469';
 native_preparation='INTEGRATED_PRIVATE_OWNER_ENTRY_NO_CLI';native_queries_executed=$false;replacement='NOT_IMPLEMENTED';
 authority='DENY';admission_missing=15;operational_request='NOT_READY';staged_updated=$false;protected_effects=0;
 first_gap='SPECIFIC_BINARY_UPDATE_ACT_AND_ASSURANCE_CONTRACT';normative_candidate_status='CANDIDATE_NOT_ACCEPTED';
 normative_candidate_sha256=(Get-FileHash -LiteralPath $updateCandidate -Algorithm SHA256).Hash.ToLowerInvariant();
 native_bilateral='NOT_DEMONSTRATED';os_quiescence='UNKNOWN';E21='PENDING';ED11='PARTIAL';phase6='PARTIAL';
 builds=$updateBuilds;distinct_test_configuration_pairs=52;main_plus_domain_test_executions=56;
 mutation_partial_update_tests='NOT_QUALIFIED_WRITER_ABSENT';
 census=[ordered]@{new_packet_bytes=224;Selected_root=368168;Selected_known=1995216;Selected_limit=3145728;Legacy_root=199600;Legacy_known=1826648;Legacy_limit=2097152;opaque_native='UNKNOWN'};
 staged_historical=[ordered]@{bytes=571392;sha256='c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf';id=@(417614100,6225920,71237);freshly_observed=$false};
 files=@($updatePaths | Sort-Object -Unique | ForEach-Object { [ordered]@{path=$_;bytes=(Get-Item -LiteralPath $_).Length;sha256=(Get-FileHash -LiteralPath $_ -Algorithm SHA256).Hash.ToLowerInvariant()} })
}
[IO.File]::WriteAllText((Join-Path (Get-Location) "$updateRoot/EVIDENCE.json"),($updateEvidence | ConvertTo-Json -Depth 10),(New-Object Text.UTF8Encoding($false)))
Write-Output 'UPDATE_PREPARATION_EVIDENCE_CAPTURED; MUTATION_NOT_IMPLEMENTED; DENY; NO_NATIVE_QUERY_OR_EFFECT'
