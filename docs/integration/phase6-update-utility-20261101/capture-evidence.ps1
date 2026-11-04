$ErrorActionPreference='Stop'
# Build/test/static-analysis evidence only. Does not invoke papacc_update.exe.
$utilityEvidenceDirectory=$PSScriptRoot
$utilityBaseline=Get-Content -Raw -LiteralPath 'docs/integration/phase6-u2-executor-data-20261101/EVIDENCE.json' | ConvertFrom-Json
$utilityBaselineBuilds=Get-Content -Raw -LiteralPath 'docs/integration/phase6-u2-executor-data-20261101/BUILD-TEST-SUMMARY.json' | ConvertFrom-Json
foreach($utilityOwnerPath in @('apps/server/personal_staged_update_win32.h','apps/server/server_launcher_staged_update_win32.inc','apps/server/server_launcher_staged_update_effects_win32.inc')) {
 $utilityOwnerBefore=@($utilityBaseline.files | Where-Object path -eq $utilityOwnerPath)
 if($utilityOwnerBefore.Count -ne 1 -or (Get-FileHash -LiteralPath $utilityOwnerPath -Algorithm SHA256).Hash -ne $utilityOwnerBefore[0].sha256) { throw "OWNER_CHANGED_$utilityOwnerPath" }
}
$utilityRows=@()
foreach($utilityProfile in @('Selected','Legacy')) {
 foreach($utilityConfiguration in @('Debug','Release')) {
  $utilityLabel="$utilityProfile-$utilityConfiguration"
  $utilityBuildText=Get-Content -Raw -LiteralPath (Join-Path $utilityEvidenceDirectory "$utilityLabel-build.log")
  $utilityTestText=Get-Content -Raw -LiteralPath (Join-Path $utilityEvidenceDirectory "$utilityLabel-tests.log")
  $utilityDetails=Get-Content -Raw -LiteralPath (Join-Path $utilityEvidenceDirectory "$utilityLabel-details.log")
  if($utilityBuildText -match '(?im)warning C|error C|FAILED:') { throw "BUILD_DIAGNOSTIC_$utilityLabel" }
  if($utilityTestText -notmatch '100% tests passed, 0 tests failed out of 2') { throw "TEST_COUNT_$utilityLabel" }
  if($utilityDetails -notmatch 'PASS UPDATE_ENTRY checks=19' -or $utilityDetails -notmatch 'cases=102 checks=602') { throw "TEST_EVIDENCE_$utilityLabel" }
  $utilityBuildDirectory="build/windows-virtual-scm-integration-20261022/$utilityProfile/$utilityConfiguration"
  $utilityBinary=Join-Path $utilityBuildDirectory 'papacc_update.exe'
  $utilityServer=Join-Path $utilityBuildDirectory 'papacc_server.exe'
  $utilityServerPin=(Get-FileHash -LiteralPath $utilityServer -Algorithm SHA256).Hash.ToLowerInvariant()
  $utilityServerBefore=@($utilityBaselineBuilds | Where-Object configuration -eq $utilityLabel)
  if($utilityServerBefore.Count -ne 1 -or $utilityServerPin -ne $utilityServerBefore[0].exe_sha256) { throw "SERVER_CHANGED_$utilityLabel" }
  $utilityRows += [ordered]@{
   configuration=$utilityLabel; build='PASS_ZERO_WARNINGS'; tests=2; entry_checks=19; update_scenarios=102; update_checks=602
   utility_bytes=(Get-Item -LiteralPath $utilityBinary).Length
   utility_sha256=(Get-FileHash -LiteralPath $utilityBinary -Algorithm SHA256).Hash.ToLowerInvariant()
   server_sha256_unchanged=$utilityServerPin
  }
 }
}
$utilityRows | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath (Join-Path $utilityEvidenceDirectory 'BUILD-TEST-SUMMARY.json') -Encoding UTF8
$utilityEvidencePaths=@('CMakeLists.txt','apps/server/update_main.c','tests/update_entry_test.c','tests/staged_binary_update_test.c',
 'apps/server/personal_staged_update_win32.h','apps/server/server_launcher_staged_update_win32.inc','apps/server/server_launcher_staged_update_effects_win32.inc',
 'docs/adr/DECISION-personal-update-executor-data-u2-20261101.md','docs/adr/DECISION-personal-binary-update-u1-u7-20261101.md',
 'docs/integration/phase6-binary-update-u1-u7-20261101/U2-EXECUTOR-AND-CANDIDATE-DATA.CANDIDATE.md',
 'docs/capabilities/phase6-scope-and-closure-20261028.md','docs/capabilities/canonical-phase6-consolidation.md',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/papacc_update.exe',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/papacc_server.exe',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/libssl-3-x64.dll',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/libcrypto-3-x64.dll')
$utilityEvidencePaths += Get-ChildItem -LiteralPath $utilityEvidenceDirectory -File | Where-Object Name -ne 'EVIDENCE.json' | ForEach-Object FullName
[ordered]@{
 task='INDEPENDENT_UPDATE_EXECUTOR_AVAILABILITY'; branch='main'; head='f341985924d29870d8ff627ed37c8a0874ad6469'
 existing_qualified_executor_found=$false; implementation='MINIMAL_UPDATE_ENTRY_LINKING_UNCHANGED_CANONICAL_OWNER'
 utility_implemented=$true; utility_protected=$false; utility_invoked=$false; installed_scm_or_files_modified=$false
 operational_request='NOT_READY_REQUIRES_SEPARATE_EXCLUSIVE_ONE_FILE_PREPARATION'
 owner_sources_unchanged=$true; server_binaries_unchanged=$true; normative_changes=0
 builds=$utilityRows; distinct_affected_test_configuration_pairs=8
 evidence_scope='ENTRY_AND_REAL_OWNER_OVER_MODELED_NATIVE_APIS_PLUS_STATIC_PE_WORKSPACE_ACL_READS'
 os_quiescence='UNKNOWN'; authority='DENY'; E21='PENDING'; ED11='PARTIAL'; phase6='PARTIAL'; production_pst='0.6.4_UNCHANGED'
 files=@($utilityEvidencePaths | Sort-Object -Unique | ForEach-Object {[ordered]@{path=$_;bytes=(Get-Item -LiteralPath $_).Length;sha256=(Get-FileHash -LiteralPath $_ -Algorithm SHA256).Hash.ToLowerInvariant()}})
} | ConvertTo-Json -Depth 10 | Set-Content -LiteralPath (Join-Path $utilityEvidenceDirectory 'EVIDENCE.json') -Encoding UTF8
