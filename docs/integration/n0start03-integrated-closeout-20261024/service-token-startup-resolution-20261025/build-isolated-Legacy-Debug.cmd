@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /Od /Zi /MDd /Fobuild/ed11-positive-private-20261025/Legacy/Debug/census-fault-test.obj /Fdbuild/ed11-positive-private-20261025/Legacy/Debug/census-fault-test.pdb /Febuild/ed11-positive-private-20261025/Legacy/Debug/census-fault-test.exe docs/integration/n0start03-integrated-closeout-20261024/service-token-startup-resolution-20261025/census-fault-test.c /link advapi32.lib
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /Od /Zi /MDd /Fobuild/ed11-positive-private-20261025/Legacy/Debug/restricted-token-native-test.obj /Fdbuild/ed11-positive-private-20261025/Legacy/Debug/restricted-token-native-test.pdb /Febuild/ed11-positive-private-20261025/Legacy/Debug/restricted-token-native-test.exe docs/integration/n0start03-integrated-closeout-20261024/service-token-startup-resolution-20261025/restricted-token-native-test.c /link advapi32.lib
if errorlevel 1 exit /b %errorlevel%
