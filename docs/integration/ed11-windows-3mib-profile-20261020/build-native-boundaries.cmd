@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake --build build/ed11-windows-3mib-profile-20261020/Selected/Debug --target papacc_d3_worker_test
if errorlevel 1 exit /b %errorlevel%
cmake --build build/ed11-windows-3mib-profile-20261020/Selected/Release --target papacc_d3_worker_test
if errorlevel 1 exit /b %errorlevel%
cmake --build build/ed11-windows-3mib-profile-20261020/Legacy/Debug --target papacc_d3_worker_test
if errorlevel 1 exit /b %errorlevel%
cmake --build build/ed11-windows-3mib-profile-20261020/Legacy/Release --target papacc_d3_worker_test
if errorlevel 1 exit /b %errorlevel%
exit /b 0
