@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
cmake --build build/windows-virtual-scm-integration-20261022/%1/%2 --parallel 1 --target papacc_server papacc_staged_binary_update_test papacc_qualification_staging_test papacc_staging_close_guard_test papacc_phase6_memory_census_test papacc_scm_original_registration_test papacc_scm_registration_files_test papacc_scq_query_cold_chain_test papacc_service_closed_empty_model papacc_service_host_product_model papacc_service_scm_query_model_test
exit /b %errorlevel%
