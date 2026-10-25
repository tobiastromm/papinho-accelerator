@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d "C:\Users\tobias\AppData\Local\Temp\pacc-ed25519-subset-extraction\msvc-Release"
cl /nologo /TC /Za /W4 /WX /MD /FAs /O2 /I"C:\Projetos\PapinhoAccelerator\include" /I"C:\Projetos\PapinhoAccelerator\src\issuer" "C:\Projetos\PapinhoAccelerator\src\issuer\strict_ed25519.c" "C:\Projetos\PapinhoAccelerator\tests\strict_ed25519_test.c" /Festrict_test.exe /link /INCREMENTAL:NO /MAP:strict_test.map
if errorlevel 1 exit /b %errorlevel%
strict_test.exe
exit /b %errorlevel%
