@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake --build build/phase6-functional-20261028/Selected/Release --parallel 2 --target papacc_server papacc_personal_scq_dispatch_test papacc_scq_private_profile_test papacc_scq_admin_source_test papacc_scq_session_driver_test papacc_scq_native_adapter_test papacc_scq_recipient_close_test papacc_scq_pre_readiness_chain_test papacc_service_host_product_model papacc_service_host_lab_model papacc_service_closed_empty_model papacc_canonical_root_layout_test papacc_phase6_memory_census_test
exit /b %errorlevel%
