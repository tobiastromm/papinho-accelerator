@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d "C:\Projetos\PapinhoAccelerator"
cmake --build build/v3-current-e0-20261021/Selected/Debug --target papacc_qualified_utc_test papacc_issuer_offline_test papacc_issuer_production_gate_test papacc_issuer_checkpoint_test papacc_phase6_memory_census_test papacc_v3_update_cohort_test
if errorlevel 1 exit /b %errorlevel%
ctest --test-dir build/v3-current-e0-20261021/Selected/Debug --output-on-failure --timeout 60 -R "papacc.issuer.qualified_utc_data|papacc.issuer.offline|papacc.issuer.production_gate|papacc.issuer.checkpoint|papacc.ed11|papacc.phase6.memory_census|papacc.v3.update_cohort"
exit /b %errorlevel%
