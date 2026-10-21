@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
if not exist "build\portable-issuer-core-20261019\Release" mkdir "build\portable-issuer-core-20261019\Release"
cl /nologo /TC /W4 /WX /O2 /MD /showIncludes /Iinclude /Isrc\issuer /DPAPACC_ISSUER_OFFLINE_TEST /Fo"build\portable-issuer-core-20261019\Release\\" /Fd"build\portable-issuer-core-20261019\Release\offline.pdb" src\issuer\issuer_core.c tests\issuer_core_test.c /Fe"build\portable-issuer-core-20261019\Release\issuer_offline.exe"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /W4 /WX /O2 /MD /Iinclude /Isrc\issuer /Fo"build\portable-issuer-core-20261019\Release\\" /Fd"build\portable-issuer-core-20261019\Release\production.pdb" src\issuer\issuer_core.c tests\issuer_production_gate_test.c /Fe"build\portable-issuer-core-20261019\Release\issuer_production_gate.exe"
exit /b %errorlevel%
