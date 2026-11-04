$ErrorActionPreference = 'Stop'
$updateEvidenceRoot = 'docs/integration/phase6-staged-binary-update-20261101'
$updateTests = '^papacc\.(phase6\.memory_census|personal\.(staged_binary_update|qualification_staging|staging_resources|anchor_child_guard|staging_close_guard|scm_original_registration|scm_registration_files|scq_query_cold_chain)|service\.(scm_query_model|closed_empty_model|host_product_model))$'
foreach ($updateProfile in @('Selected','Legacy')) {
 foreach ($updateConfiguration in @('Debug','Release')) {
  $updateLabel = "$updateProfile-$updateConfiguration"
  & cmd /c "docs\integration\phase6-staged-binary-update-20261101\build.cmd $updateProfile $updateConfiguration" *> "$updateEvidenceRoot/$updateLabel-build.log"
  if ($LASTEXITCODE -ne 0) { throw "BUILD_FAILED_$updateLabel" }
  & ctest --test-dir "build/windows-virtual-scm-integration-20261022/$updateProfile/$updateConfiguration" -R $updateTests --output-on-failure *> "$updateEvidenceRoot/$updateLabel-tests.log"
  if ($LASTEXITCODE -ne 0) { throw "TEST_FAILED_$updateLabel" }
  Copy-Item -LiteralPath "build/windows-virtual-scm-integration-20261022/$updateProfile/$updateConfiguration/Testing/Temporary/LastTest.log" -Destination "$updateEvidenceRoot/$updateLabel-details.log"
 }
}
