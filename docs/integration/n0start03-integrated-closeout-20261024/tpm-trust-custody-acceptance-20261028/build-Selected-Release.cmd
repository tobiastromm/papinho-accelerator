@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
cmake --build build/v3-backings-20261021/Selected/Release --target papacc_server papacc_q4_root_binding_data_test --parallel 2 > "docs\integration\n0start03-integrated-closeout-20261024\tpm-trust-custody-acceptance-20261028\Selected-Release-build.log" 2>&1
exit /b %errorlevel%
