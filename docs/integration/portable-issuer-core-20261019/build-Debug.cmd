@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
if not exist "build\portable-issuer-core-20261019\Debug" mkdir "build\portable-issuer-core-20261019\Debug"
cl /nologo /TC /W4 /WX /Od /Zi /MDd /showIncludes /Iinclude /Isrc\issuer /DPAPACC_ISSUER_OFFLINE_TEST /Fo"build\portable-issuer-core-20261019\Debug\\" /Fd"build\portable-issuer-core-20261019\Debug\offline.pdb" src\issuer\issuer_core.c tests\issuer_core_test.c /Fe"build\portable-issuer-core-20261019\Debug\issuer_offline.exe"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /W4 /WX /Od /Zi /MDd /Iinclude /Isrc\issuer /Fo"build\portable-issuer-core-20261019\Debug\\" /Fd"build\portable-issuer-core-20261019\Debug\production.pdb" src\issuer\issuer_core.c tests\issuer_production_gate_test.c /Fe"build\portable-issuer-core-20261019\Debug\issuer_production_gate.exe"
exit /b %errorlevel%
