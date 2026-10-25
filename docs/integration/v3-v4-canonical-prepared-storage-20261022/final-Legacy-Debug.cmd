@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d C:\Projetos\PapinhoAccelerator
cmake --build build/root08-issuer-factual-transaction-binding-20261020/Legacy/Debug --clean-first --target papacc_config_root8_test papacc_config_root8_v2_test papacc_config_root8_v3_test papacc_config_root8_v4_test papacc_config_lst1_test papacc_phase6_memory_census_test papacc_store_hash_owner_test papacc_store_hash_lifetime_test
if errorlevel 1 exit /b %errorlevel%
ctest --test-dir build/root08-issuer-factual-transaction-binding-20261020/Legacy/Debug --output-on-failure --timeout 180 -R "papacc.config_root8|papacc.config_lst1|papacc.phase6.memory_census|papacc.store_hash|papacc.prepared_storage|papacc.v3.update_cohort|papacc.v[34].immutable_backings|papacc.kind8.dispatch|papacc.v3.current_negative|papacc.root8.kind8"
exit /b %errorlevel%
