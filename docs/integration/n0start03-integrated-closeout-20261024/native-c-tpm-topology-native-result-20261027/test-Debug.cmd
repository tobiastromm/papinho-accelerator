@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
cmake --build build/v3-backings-20261021/Selected/Debug --target papacc_tpm_native_platform_test > "docs\integration\n0start03-integrated-closeout-20261024\native-c-tpm-topology-native-result-20261027\Debug-build.log" 2>&1
if errorlevel 1 exit /b 1
ctest --test-dir build/v3-backings-20261021/Selected/Debug -R papacc.tpm_native.guards_offline -V > "docs\integration\n0start03-integrated-closeout-20261024\native-c-tpm-topology-native-result-20261027\Debug-tests.log" 2>&1
exit /b %errorlevel%
