@echo off
call "C:\MSVC600-master\VC98\Bin\VCVARS32.BAT"
if errorlevel 1 exit /b %errorlevel%
cd /d "C:\Users\tobias\AppData\Local\Temp\pacc-v4-ed25519-integration\vc6-Debug"
cl /nologo /TC /Za /W4 /WX /MD /FAs /Od /Zi /I"C:\Projetos\PapinhoAccelerator\include" /I"C:\Projetos\PapinhoAccelerator\src\config" /I"C:\Projetos\PapinhoAccelerator\src\issuer" "C:\Projetos\PapinhoAccelerator\src\issuer\strict_ed25519.c" "C:\Projetos\PapinhoAccelerator\src\config\config_root8_time_source_key.c" "C:\Projetos\PapinhoAccelerator\tests\config_root8_time_source_key_test.c" /Fetest.exe /link /INCREMENTAL:NO /MAP:test.map
if errorlevel 1 exit /b %errorlevel%
test.exe
exit /b %errorlevel%
