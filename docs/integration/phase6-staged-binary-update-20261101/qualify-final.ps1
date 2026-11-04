$ErrorActionPreference = 'Stop'
$updateEvidenceRoot = 'docs/integration/phase6-staged-binary-update-20261101'
foreach ($updateProfile in @('Selected','Legacy')) {
 foreach ($updateConfiguration in @('Debug','Release')) {
  $updateLabel = "$updateProfile-$updateConfiguration"
  & cmd /c "docs\integration\phase6-staged-binary-update-20261101\qualify-final.cmd $updateProfile $updateConfiguration" *> "$updateEvidenceRoot/$updateLabel-refinement-build.log"
  if ($LASTEXITCODE -ne 0) { throw "BUILD_FAILED_$updateLabel" }
  & ctest --test-dir "build/windows-virtual-scm-integration-20261022/$updateProfile/$updateConfiguration" -R '^papacc\.(personal\.staged_binary_update|service\.(original_registration|registration_files|registration_resource_observations))$' --output-on-failure *> "$updateEvidenceRoot/$updateLabel-final-tests.log"
  if ($LASTEXITCODE -ne 0) { throw "TEST_FAILED_$updateLabel" }
  Copy-Item -LiteralPath "build/windows-virtual-scm-integration-20261022/$updateProfile/$updateConfiguration/Testing/Temporary/LastTest.log" -Destination "$updateEvidenceRoot/$updateLabel-final-details.log"
 }
}
