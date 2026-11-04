@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
cmake --build build/windows-virtual-scm-integration-20261022/%1/%2 --parallel 1 --target papacc_server papacc_service_host_product_model papacc_service_host_lab_model papacc_service_closed_empty_model papacc_scq_admin_source_test papacc_scq_session_driver_test papacc_scq_native_adapter_test papacc_scq_recipient_close_test papacc_scq_transport_scope_test papacc_scq_transport_continuation_test papacc_scq_pre_readiness_chain_test papacc_phase6_memory_census_test
exit /b %errorlevel%
