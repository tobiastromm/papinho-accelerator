@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
if not exist "build\cng-failure-origin-instrumentation-20261019\Debug" mkdir "build\cng-failure-origin-instrumentation-20261019\Debug"
cl /nologo /TC /W4 /WX /Od /Zi /MDd /Iapps\server /Fo"build\cng-failure-origin-instrumentation-20261019\Debug\\" /Fd"build\cng-failure-origin-instrumentation-20261019\Debug\synthetic.pdb" tests\ca_failure_instrumentation_test.c tests\ca_failure_baseline_bridge.c /Fe"build\cng-failure-origin-instrumentation-20261019\Debug\ca_failure_instrumentation_test.exe" /link advapi32.lib
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /W4 /WX /Od /Zi /MDd /Iapps\server /Fo"build\cng-failure-origin-instrumentation-20261019\Debug\\" /Fd"build\cng-failure-origin-instrumentation-20261019\Debug\backend.pdb" apps\server\server_ca_key_win32.c tests\ca_key_offline_test.c /Fe"build\cng-failure-origin-instrumentation-20261019\Debug\ca_key_offline_test.exe" /link ncrypt.lib bcrypt.lib advapi32.lib
exit /b %errorlevel%