@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake --build build/root08-worker-storage-strategy-20261020/Release --target papacc_phase6_memory_census_test papacc_d3_worker_test papacc_issuer_root_store_test papacc_issuer_root_guard_test papacc_bootstrap_d3_commit_test papacc_bootstrap_namespace_test
exit /b %errorlevel%
