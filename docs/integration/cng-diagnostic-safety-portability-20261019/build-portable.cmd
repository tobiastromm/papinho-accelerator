@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /W4 /WX /showIncludes /Od /MDd /Iinclude /Isrc\capability /Fo"build\cng-diagnostic-safety-portability-20261019\Debug\portable.obj" tests\ca_portability_boundary_test.c /Fe"build\cng-diagnostic-safety-portability-20261019\Debug\portable.exe"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /W4 /WX /showIncludes /O2 /MD /Iinclude /Isrc\capability /Fo"build\cng-diagnostic-safety-portability-20261019\Release\portable.obj" tests\ca_portability_boundary_test.c /Fe"build\cng-diagnostic-safety-portability-20261019\Release\portable.exe"
if errorlevel 1 exit /b %errorlevel%
exit /b 0
