@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /W4 /WX /MD /I include /I src/issuer docs/integration/phase6-functional-integration-20261028/census-before.c /Fobuild/phase6-functional-20261028/census/before.obj /Febuild/phase6-functional-20261028/census/before.exe
if errorlevel 1 exit /b %errorlevel%
build\phase6-functional-20261028\census\before.exe
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /W4 /WX /MD /I include /I src/issuer docs/integration/phase6-functional-integration-20261028/census-after.c /Fobuild/phase6-functional-20261028/census/after.obj /Febuild/phase6-functional-20261028/census/after.exe
if errorlevel 1 exit /b %errorlevel%
build\phase6-functional-20261028\census\after.exe
if errorlevel 1 exit /b %errorlevel%
