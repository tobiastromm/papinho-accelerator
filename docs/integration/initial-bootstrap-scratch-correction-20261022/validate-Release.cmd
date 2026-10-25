@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d C:\Projetos\PapinhoAccelerator
cmake --build build/root08-issuer-factual-transaction-binding-20261020/Selected/Release --target papacc_bootstrap_scratch_test papacc_config_binding_native_test papacc_config_binding_test papacc_phase6_memory_census_test papacc_store_hash_owner_test papacc_store_hash_lifetime_test papacc_prepared_storage_model_test papacc_prepared_storage_native_test
if errorlevel 1 exit /b %errorlevel%
ctest --test-dir build/root08-issuer-factual-transaction-binding-20261020/Selected/Release --output-on-failure --timeout 180 -R "papacc.bootstrap.scratch|papacc.config_binding|papacc.phase6.memory_census|papacc.store_hash|papacc.prepared_storage"
exit /b %errorlevel%
