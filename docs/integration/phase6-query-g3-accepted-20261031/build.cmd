@echo off
call docs\integration\phase6-g0-g9-integration-20261031\build.cmd %1 %2
if errorlevel 1 exit /b 1
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
cmake --build build/windows-virtual-scm-integration-20261022/%1/%2 --parallel 1 --target papacc_scq_query_cold_chain_test
exit /b %errorlevel%
