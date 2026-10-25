@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d C:\Projetos\PapinhoAccelerator
cmake --build build/root08-issuer-factual-transaction-binding-20261020/Selected/Release --clean-first --target papacc_config_root8_test papacc_config_root8_v2_test papacc_config_root8_v3_test papacc_config_root8_v4_test papacc_config_lst1_test papacc_phase6_memory_census_test papacc_store_hash_owner_test papacc_store_hash_lifetime_test papacc_prepared_storage_model_test papacc_prepared_storage_native_test papacc_v3_update_cohort_test papacc_v3_backing_test papacc_v4_backing_test papacc_kind8_dispatch_model_test papacc_kind8_dispatch_native_test papacc_v3_current_negative_test papacc_v3_transaction_worker_test papacc_root8_kind8_model_test papacc_root8_kind8_native_gate_test
if errorlevel 1 exit /b %errorlevel%
ctest --test-dir build/root08-issuer-factual-transaction-binding-20261020/Selected/Release --output-on-failure --timeout 180 -R "papacc.config_root8|papacc.config_lst1|papacc.phase6.memory_census|papacc.store_hash|papacc.prepared_storage|papacc.v3.update_cohort|papacc.v[34].immutable_backings|papacc.kind8.dispatch|papacc.v3.current_negative|papacc.root8.kind8"
exit /b %errorlevel%
