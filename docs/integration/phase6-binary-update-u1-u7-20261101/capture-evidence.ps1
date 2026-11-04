$ErrorActionPreference = 'Stop'
$updateRoot = 'docs/integration/phase6-binary-update-u1-u7-20261101'
$updateNorm = 'docs/integration/phase6-staged-binary-update-20261101/UPDATE-CONTRACT-AND-ASSURANCE.CANDIDATE.md'
$updateRequest = "$updateRoot/ONE-BINARY-UPDATE-REQUEST.CANDIDATE.md"
$updateProcedure = "$updateRoot/update-binary.MANUAL.CANDIDATE.ps1"
if ((Get-FileHash -LiteralPath $updateNorm).Hash -ne '9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234') { throw 'NORM_HASH_DENY' }
if ((Get-FileHash -LiteralPath $updateRequest).Hash -ne 'baa501ad97fb1c5cbf65c96d2ecfc338ea0f7c55a872ad0443b74580f08f4726') { throw 'REQUEST_HASH_DENY' }
if ((Get-FileHash -LiteralPath $updateProcedure).Hash -ne '85531965578ba126200bfab6d2168226dfdaadd9c4e1b96135411d01d1e6ac55') { throw 'PROCEDURE_HASH_DENY' }
$updateExe = 'build/windows-virtual-scm-integration-20261022/Selected/Release/papacc_server.exe'
if ((Get-FileHash -LiteralPath $updateExe).Hash -ne '42d468faf367447ac8416194cadb5d2b0474c91aa3b7953838b310e65b0f29ca' -or (Get-Item -LiteralPath $updateExe).Length -ne 737280) { throw 'EXE_HASH_SIZE_DENY' }
$updateBuilds = @()
foreach ($updateProfile in @('Selected','Legacy')) {
 foreach ($updateConfiguration in @('Debug','Release')) {
  $updateLabel = "$updateProfile-$updateConfiguration"
  foreach ($updateSuffix in @('build','census-final-build','observer-build')) {
   if ((Get-Content -LiteralPath "$updateRoot/$updateLabel-$updateSuffix.log" -Raw) -match '(?im)\bwarning [A-Z]+\d+|fatal error|FAILED:') { throw "BUILD_NOT_CLEAN_$updateLabel" }
  }
  if ((Get-Content -LiteralPath "$updateRoot/$updateLabel-tests.log" -Raw) -notmatch '100% tests passed, 0 tests failed out of 13') { throw "TEST_DENY_$updateLabel" }
  if ((Get-Content -LiteralPath "$updateRoot/$updateLabel-details.log" -Raw) -notmatch 'cases=77 checks=382 PREPARATION_33_EFFECT_44') { throw "UPDATE_MODEL_DENY_$updateLabel" }
  if ((Get-Content -LiteralPath "$updateRoot/$updateLabel-observer-tests.log" -Raw) -notmatch '100% tests passed, 0 tests failed out of 1') { throw "OBSERVER_DENY_$updateLabel" }
  $updateCensus = Get-Content -LiteralPath "$updateRoot/$updateLabel-census-final-details.log" -Raw
  $updateK = if ($updateProfile -eq 'Selected') { 2025386 } else { 1856818 }
  if ($updateCensus -notmatch "BINARY_UPDATE_K=$updateK;") { throw "CENSUS_DENY_$updateLabel" }
  $updateBuilds += [ordered]@{configuration=$updateLabel;build='PASS';warnings=0;distinct_affected_ctests=14;update_cases=77;update_checks=382;K=$updateK;native_boundaries='MODELED'}
 }
}
$updateAst = Get-Content -LiteralPath "$updateRoot/manual-script-ast.json" -Raw | ConvertFrom-Json
if ($updateAst.parse_errors -ne 0 -or $updateAst.edition -ne 'Desktop' -or -not $updateAst.x64 -or $updateAst.version -notlike '5.1.*') { throw 'SCRIPT_AST_DENY' }
$updatePaths = @('CMakeLists.txt','apps/server/main.c.api3.inc','apps/server/personal_staged_update_win32.h',
 'apps/server/server_launcher_owner_win32.c','apps/server/server_launcher_owner_win32.h',
 'apps/server/server_launcher_staged_update_win32.inc','apps/server/server_launcher_staged_update_effects_win32.inc',
 'apps/server/server_configuration_personal_native_win32.inc','apps/server/server_console_win32.c',
 'tests/staged_binary_update_test.c','tests/phase6_memory_census_test.c','tests/qualification_staging_test.c',
 'docs/adr/DECISION-personal-binary-update-u1-u7-20261101.md',
 'docs/capabilities/phase6-scope-and-closure-20261028.md',$updateNorm,$updateExe,
 'build/private-bootstrap-preserved-20261031/papacc_server-update-u1-u7-42d468faf367447ac8416194cadb5d2b0474c91aa3b7953838b310e65b0f29ca.exe',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/libssl-3-x64.dll',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/libcrypto-3-x64.dll')
$updatePaths += Get-ChildItem -LiteralPath $updateRoot -File | Where-Object Name -ne 'EVIDENCE.json' | ForEach-Object { "$updateRoot/$($_.Name)" }
$updateEvidence = [ordered]@{
 task='U1_U7_ACCEPTANCE_AND_OFFLINE_UPDATE';date='2026-11-01';branch='main';head='f341985924d29870d8ff627ed37c8a0874ad6469';
 normative='B_U1_U7_ACCEPTED_EXACT_9fb5318d';implementation='CANONICAL_OWNER_CLI_ORIGINAL_ACT_EXCLUSIVE_CREATE_COPY_FLUSH_TWO_RENAMES_READBACK_CLOSE';
 operational_authority='DENY';operational_request='READY_FOR_OWNER_SELECTION_AND_SPECIFIC_AUTHORIZATION';
 operational_request_sha256='baa501ad97fb1c5cbf65c96d2ecfc338ea0f7c55a872ad0443b74580f08f4726';
 procedure_sha256='85531965578ba126200bfab6d2168226dfdaadd9c4e1b96135411d01d1e6ac55';
 selected_release=[ordered]@{bytes=737280;sha256='42d468faf367447ac8416194cadb5d2b0474c91aa3b7953838b310e65b0f29ca';owner_operational_selection='PENDING';staged=$false};
 historical_staged=[ordered]@{bytes=571392;sha256='c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf';id=@(417614100,6225920,71237);parent=@(417614100,8257536,71209);fresh_query=$false};
 native_update='NOT_EXECUTED';native_queries='NOT_EXECUTED';real_protected_effects=0;procedure_executed=$false;
 rollback='ABSENT';retry='FORBIDDEN';unknown_retention='PRESERVED';native_bilateral='NOT_DEMONSTRATED';os_quiescence='UNKNOWN';
 E21='PENDING';ED11='PARTIAL';phase6='PARTIAL';production_pst='0.6.4_UNCHANGED';
 distinct_test_configuration_pairs=56;builds=$updateBuilds;script_validation=$updateAst;
 census=[ordered]@{Selected_root=373752;Selected_known=2000800;Selected_K=2025386;Selected_cap=3145728;Legacy_root=205184;Legacy_known=1832232;Legacy_K=1856818;Legacy_cap=2097152;opaque_costs='UNKNOWN_NOT_ZERO'};
 files=@($updatePaths | Sort-Object -Unique | ForEach-Object { [ordered]@{path=$_;bytes=(Get-Item -LiteralPath $_).Length;sha256=(Get-FileHash -LiteralPath $_ -Algorithm SHA256).Hash.ToLowerInvariant()} })
}
[IO.File]::WriteAllText((Join-Path (Get-Location) "$updateRoot/EVIDENCE.json"),($updateEvidence | ConvertTo-Json -Depth 10),(New-Object Text.UTF8Encoding($false)))
Write-Output 'U1_U7_OFFLINE_EVIDENCE_VERIFIED; 4_BUILDS_56_DISTINCT_PAIRS; NO_OPERATIONAL_UPDATE; OWNER_AUTHORIZATION_PENDING'
