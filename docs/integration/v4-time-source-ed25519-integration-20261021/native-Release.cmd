@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
cd /d "C:\Projetos\PapinhoAccelerator"
cmake --build "C:\Projetos\PapinhoAccelerator\build\root08-issuer-factual-transaction-binding-20261020\Selected\Release" --clean-first --target papacc_config_root8_v4_test papacc_v4_backing_test papacc_config_root8_v3_test papacc_config_root8_v2_test papacc_config_root8_test papacc_config_lst1_test papacc_phase6_memory_census_test papacc_time_source_key_test
if errorlevel 1 exit /b %errorlevel%
ctest --test-dir "C:\Projetos\PapinhoAccelerator\build\root08-issuer-factual-transaction-binding-20261020\Selected\Release" --output-on-failure --timeout 180 -R "papacc.time_source.key_data|papacc.config_root8|papacc.config_lst1|papacc.phase6.memory_census|papacc.v4.immutable_backings"
exit /b %errorlevel%
