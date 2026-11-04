$ErrorActionPreference = 'Stop'
$queryEvidence = 'docs/integration/phase6-query-g3-accepted-20261031'
$queryPattern = '^papacc\.(phase6\.memory_census|personal\.scq_(query_cold_chain|cold_open|cold_controller|start_assurance|endpoint_act|first_controller|start_selection|native_selection_capture|pre_readiness_chain|native_first_ingress_boundary|bootstrap_capture|bootstrap_carrier|recipient_close|native_adapter|session_driver|admin_source|transport_scope|transport_continuation)|service\.(console_original_wait_model|lifecycle_data|scm_query_model|scm_virtual_model|scm_acl_model|closed_empty_model|host_product_model|host_lab_model|runtime_model))$'
foreach ($queryProfile in @('Selected','Legacy')) {
 foreach ($queryConfiguration in @('Debug','Release')) {
  $queryLabel = "$queryProfile-$queryConfiguration"
  & cmd /c "docs\integration\phase6-query-g3-accepted-20261031\build.cmd $queryProfile $queryConfiguration" *> "$queryEvidence/$queryLabel-final-build.log"
  if ($LASTEXITCODE -ne 0) { throw "BUILD_FAILED_$queryLabel" }
  & ctest --test-dir "build/windows-virtual-scm-integration-20261022/$queryProfile/$queryConfiguration" -R $queryPattern --output-on-failure *> "$queryEvidence/$queryLabel-tests.log"
  Copy-Item -LiteralPath "build/windows-virtual-scm-integration-20261022/$queryProfile/$queryConfiguration/Testing/Temporary/LastTest.log" -Destination "$queryEvidence/$queryLabel-details.log"
  if ($LASTEXITCODE -ne 0) { throw "TEST_FAILED_$queryLabel" }
 }
}
