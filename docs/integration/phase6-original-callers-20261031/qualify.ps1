$ErrorActionPreference = 'Stop'
$evidenceRoot = 'docs/integration/phase6-original-callers-20261031'
$affectedPattern = '^papacc\.(phase6\.memory_census|personal\.scq_(first_controller|start_selection|native_selection_capture|pre_readiness_chain|native_first_ingress_boundary|bootstrap_capture|bootstrap_carrier|recipient_close|native_adapter|session_driver|admin_source|transport_scope|transport_continuation)|service\.(console_original_wait_model|lifecycle_data|scm_query_model|scm_virtual_model|scm_acl_model|closed_empty_model|host_product_model|host_lab_model|runtime_model))$'
foreach ($qualificationProfile in @('Selected','Legacy')) {
 foreach ($qualificationConfiguration in @('Debug','Release')) {
  $qualificationLabel = "$qualificationProfile-$qualificationConfiguration"
  & cmd /c "docs\integration\phase6-original-callers-20261031\build.cmd $qualificationProfile $qualificationConfiguration" *> "$evidenceRoot/$qualificationLabel-final-build.log"
  if ($LASTEXITCODE -ne 0) { throw "BUILD_FAILED_$qualificationLabel" }
  & ctest --test-dir "build/windows-virtual-scm-integration-20261022/$qualificationProfile/$qualificationConfiguration" -R $affectedPattern --output-on-failure *> "$evidenceRoot/$qualificationLabel-tests.log"
  if ($LASTEXITCODE -ne 0) { throw "TEST_FAILED_$qualificationLabel" }
 }
}

