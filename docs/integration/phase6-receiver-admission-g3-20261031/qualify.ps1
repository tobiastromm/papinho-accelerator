$ErrorActionPreference = 'Stop'
$g3Evidence = 'docs/integration/phase6-receiver-admission-g3-20261031'
$g3Tests = '^papacc\.(phase6\.memory_census|personal\.scq_(cold_open|cold_controller|start_assurance|first_controller|native_selection_capture|start_selection|pre_readiness_chain)|service\.(closed_empty_model|host_product_model))$'
foreach ($g3Profile in @('Selected','Legacy')) {
 foreach ($g3Configuration in @('Debug','Release')) {
  $g3Label = "$g3Profile-$g3Configuration"
  & cmd /c "docs\integration\phase6-g0-g9-integration-20261031\build.cmd $g3Profile $g3Configuration" *> "$g3Evidence/$g3Label-build.log"
  if ($LASTEXITCODE -ne 0) { throw "BUILD_FAILED_$g3Label" }
  & ctest --test-dir "build/windows-virtual-scm-integration-20261022/$g3Profile/$g3Configuration" -R $g3Tests --output-on-failure *> "$g3Evidence/$g3Label-tests.log"
  if ($LASTEXITCODE -ne 0) { throw "TEST_FAILED_$g3Label" }
  Copy-Item -LiteralPath "build/windows-virtual-scm-integration-20261022/$g3Profile/$g3Configuration/Testing/Temporary/LastTest.log" -Destination "$g3Evidence/$g3Label-details.log"
 }
}
