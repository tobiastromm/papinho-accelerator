$ErrorActionPreference = 'Stop'
$g3EvidenceRoot = 'docs/integration/phase6-receiver-admission-g3-20261031'
$g3Utf8 = New-Object System.Text.UTF8Encoding($false)
$g3ProposalPath = "$g3EvidenceRoot/INITIAL-QUERY-AND-G3.CANDIDATE.md"
$g3ProposalHash = (Get-FileHash -LiteralPath $g3ProposalPath -Algorithm SHA256).Hash.ToLowerInvariant()
if ($g3ProposalHash -ne '5b77ddae089ba496c511a562575e22642a29555e536fd4563e5e5ebb8faefeca') { throw 'PROPOSAL_HASH_DENY' }
$g3Builds = @()
foreach ($g3Profile in @('Selected','Legacy')) {
 foreach ($g3Configuration in @('Debug','Release')) {
  $g3Label = "$g3Profile-$g3Configuration"
  $g3BuildLog = "$g3EvidenceRoot/$g3Label-build.log"
  $g3TestLog = "$g3EvidenceRoot/$g3Label-tests.log"
  if ((Get-Content -LiteralPath $g3BuildLog -Raw) -match '(?im)\bwarning [A-Z]+\d+|fatal error|FAILED:') { throw "BUILD_NOT_CLEAN_$g3Label" }
  if ((Get-Content -LiteralPath $g3TestLog -Raw) -notmatch '100% tests passed, 0 tests failed out of 10') { throw "TEST_DENY_$g3Label" }
  $g3Builds += [ordered]@{profile=$g3Label;build='PASS';warnings=0;tests='PASS';test_configuration_pairs=10;build_log=$g3BuildLog;test_log=$g3TestLog}
 }
}
$g3HistoricalPath = 'docs/integration/phase6-scm-preplan-first-cause-20261031/OWNER-CORRECTED-SCM-RESULT.txt'
if ((Get-FileHash -LiteralPath $g3HistoricalPath -Algorithm SHA256).Hash -ne '2b7cd87e6931907b81a7b49df455eb45c6738c6867b72e5e0e6e9c518d3ab83d') { throw 'HISTORICAL_SOURCE_HASH_DENY' }
$g3Historical = Get-Content -LiteralPath $g3HistoricalPath -Raw
$g3Queries = @([regex]::Matches($g3Historical,'qpc_begin=(\d+) qpc_end=(\d+) frequency=(\d+)') | ForEach-Object {
 1000.0 * ([double]$_.Groups[2].Value - [double]$_.Groups[1].Value) / [double]$_.Groups[3].Value
})
$g3QuerySummary = $g3Queries | Measure-Object -Minimum -Maximum
$g3Paths = @(
 'apps/server/personal_scq_start_plan_data_win32.inc',
 'apps/server/personal_scq_cold_acquisition_win32.inc','tests/scq_start_assurance_test.c',
 'docs/capabilities/phase6-scope-and-closure-20261028.md',
 'docs/adr/DECISION-personal-first-diagnostic-start-assurance-g0-g9-20261031.md',
 'docs/integration/phase6-bootstrap-source-assurance-20261031/FIRST-DIAGNOSTIC-START-ASSURANCE.CANDIDATE.md',
 $g3HistoricalPath,'docs/integration/phase6-scm-preplan-first-cause-20261031/CORRECTED-SCM-RESOURCE-RECONCILIATION.json',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/papacc_server.exe',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/libssl-3-x64.dll',
 'build/windows-virtual-scm-integration-20261022/Selected/Release/libcrypto-3-x64.dll'
)
$g3Paths += Get-ChildItem -LiteralPath $g3EvidenceRoot -File | Where-Object Name -ne 'EVIDENCE.json' | ForEach-Object { "$g3EvidenceRoot/$($_.Name)" }
$g3Files = @($g3Paths | Sort-Object -Unique | ForEach-Object {
 [ordered]@{path=$_;bytes=(Get-Item -LiteralPath $_).Length;sha256=(Get-FileHash -LiteralPath $_ -Algorithm SHA256).Hash.ToLowerInvariant()}
})
$g3Result = [ordered]@{
 task='RECEIVER_ORIGINAL_ADMISSION_AND_G3';branch='main';head='f341985924d29870d8ff627ed37c8a0874ad6469';
 G0_G9='ACCEPTED_UNCHANGED';query_specialization='PROPOSED_NOT_ACCEPTED';G3_values='PROPOSED_NOT_SELECTED';
 proposal_sha256=$g3ProposalHash;receiver='INDEPENDENT_PRE_CONNECTION_AUTHORITY_NOT_AVAILABLE';cold_chain='NOT_COMPLETE';
 operational_request='NOT_READY';authority='DENY';E21='PENDING';ED11='PARTIAL';phase6='PARTIAL';os_quiescence='UNKNOWN';
 operational_effects=0;native_bilateral='NOT_DEMONSTRATED';staged_updated=$false;
 implementation='G3_exact_values_bound_into_cold_source_policy_digest';preimage_bytes=84;G3_field_mutations=8;
 builds=$g3Builds;test_configuration_pairs=40;
 history=[ordered]@{scope='OWNER_SUPPLIED_HISTORICAL_WINDOWS_OBSERVATION_NOT_FRESH_CAPACITY';samples=$g3QuerySummary.Count;min_query_ms=$g3QuerySummary.Minimum;max_query_ms=$g3QuerySummary.Maximum;physical_floor_proposed=7239172096;commit_floor_proposed=9823326208};
 files=$g3Files
}
[IO.File]::WriteAllText((Join-Path (Get-Location) "$g3EvidenceRoot/EVIDENCE.json"),($g3Result | ConvertTo-Json -Depth 9),$g3Utf8)
Write-Output 'EVIDENCE_CAPTURED_FROM_WORKSPACE_AND_HISTORICAL_TEXT; NO_OPERATIONAL_QUERY'
