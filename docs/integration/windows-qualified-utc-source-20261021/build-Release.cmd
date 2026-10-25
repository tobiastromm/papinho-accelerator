@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d "C:\Users\tobias\AppData\Local\Temp\pacc-windows-utc-source-20261021\Release"
cl /nologo /TC /W4 /WX /O2 /I"C:\Projetos\PapinhoAccelerator\include" /I"C:\Projetos\PapinhoAccelerator\src\issuer" "C:\Users\tobias\AppData\Local\Temp\pacc-windows-utc-source-20261021\windows_utc_probe.c" C:\Projetos\PapinhoAccelerator\src\issuer\qualified_utc.c /Feutc_probe.exe
if errorlevel 1 exit /b %errorlevel%
utc_probe.exe
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /W4 /WX /O2 /I"C:\Projetos\PapinhoAccelerator\include" /I"C:\Projetos\PapinhoAccelerator\src\issuer" C:\Projetos\PapinhoAccelerator\tests\qualified_utc_test.c C:\Projetos\PapinhoAccelerator\src\issuer\qualified_utc.c /Feutc_math_test.exe
if errorlevel 1 exit /b %errorlevel%
utc_math_test.exe
exit /b %errorlevel%
