@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d C:\Projetos\PapinhoAccelerator
cmake --build build/root08-issuer-factual-transaction-binding-20261020/Selected/Debug --target papacc_v3_update_cohort_test papacc_v4_backing_test papacc_config_root8_v4_test papacc_v3_transaction_worker_test papacc_phase6_memory_census_test papacc_store_hash_owner_test papacc_store_hash_lifetime_test
exit /b %errorlevel%
