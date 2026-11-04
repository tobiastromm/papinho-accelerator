@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake --build build/phase6-functional-20261028/Legacy/Debug --parallel 2 --target papacc_server papacc_issuer_offline_test papacc_issuer_production_gate_test papacc_issuer_checkpoint_test papacc_enrollment_test papacc_enrollment_pst_test papacc_issuer_root_store_test papacc_issuer_root_guard_test
exit /b %errorlevel%
