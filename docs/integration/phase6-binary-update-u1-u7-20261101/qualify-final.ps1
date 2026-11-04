$ErrorActionPreference = 'Stop'
$updateRoot = 'docs/integration/phase6-binary-update-u1-u7-20261101'
foreach ($updateLabel in @('Selected-Release','Legacy-Debug','Legacy-Release')) {
 $updateProfile,$updateConfiguration = $updateLabel.Split('-')
 & cmd /c "docs\integration\phase6-binary-update-u1-u7-20261101\build.cmd $updateProfile $updateConfiguration" *> "$updateRoot/$updateLabel-final-build.log"
 if ($LASTEXITCODE -ne 0) { throw "BUILD_FAILED_$updateLabel" }
 & ctest --test-dir "build/windows-virtual-scm-integration-20261022/$updateProfile/$updateConfiguration" -R '^papacc\.personal\.staged_binary_update$' --output-on-failure *> "$updateRoot/$updateLabel-final-tests.log"
 if ($LASTEXITCODE -ne 0) { throw "TEST_FAILED_$updateLabel" }
 Copy-Item -LiteralPath "build/windows-virtual-scm-integration-20261022/$updateProfile/$updateConfiguration/Testing/Temporary/LastTest.log" -Destination "$updateRoot/$updateLabel-final-details.log"
}
