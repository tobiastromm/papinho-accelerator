@echo off
call "C:\MSVC600-master\VC98\Bin\VCVARS32.BAT"
if errorlevel 1 exit /b %errorlevel%
cd /d "C:\Users\tobias\AppData\Local\Temp\pacc-qualified-utc-source-20261021\vc6-Debug"
cl /nologo /TC /Za /W4 /WX /MD /Od /Zi /I"C:\Projetos\PapinhoAccelerator\include" /I"C:\Projetos\PapinhoAccelerator\src\issuer" "C:\Projetos\PapinhoAccelerator\src\issuer\qualified_utc.c" "C:\Projetos\PapinhoAccelerator\tests\qualified_utc_test.c" /Fequalified_utc_test.exe /link /INCREMENTAL:NO
if errorlevel 1 exit /b %errorlevel%
qualified_utc_test.exe
exit /b %errorlevel%
