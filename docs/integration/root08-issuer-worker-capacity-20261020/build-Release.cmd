@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake --build build/canonical-issuer-carrier-integration-20261020/Release --target papacc_phase6_memory_census_test papacc_d3_worker_test papacc_issuer_root_store_test
exit /b %errorlevel%
