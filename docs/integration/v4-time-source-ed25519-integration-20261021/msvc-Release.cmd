@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d "C:\Users\tobias\AppData\Local\Temp\pacc-v4-ed25519-integration\msvc-Release"
cl /nologo /TC /Za /W4 /WX /MD /FAs /O2 /I"C:\Projetos\PapinhoAccelerator\include" /I"C:\Projetos\PapinhoAccelerator\src\config" /I"C:\Projetos\PapinhoAccelerator\src\issuer" "C:\Projetos\PapinhoAccelerator\src\issuer\strict_ed25519.c" "C:\Projetos\PapinhoAccelerator\src\config\config_root8_time_source_key.c" "C:\Projetos\PapinhoAccelerator\tests\config_root8_time_source_key_test.c" /Fetest.exe /link /INCREMENTAL:NO /MAP:test.map
if errorlevel 1 exit /b %errorlevel%
test.exe
exit /b %errorlevel%
