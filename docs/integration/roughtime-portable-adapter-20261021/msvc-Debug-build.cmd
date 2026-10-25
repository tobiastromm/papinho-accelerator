@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d "C:\Users\tobias\AppData\Local\Temp\pacc-roughtime-portable-adapter-20261021\msvc-Debug"
cl /nologo /TC /Za /W4 /WX /MD /Od /Zi /I"C:\Projetos\PapinhoAccelerator\include" /I"C:\Projetos\PapinhoAccelerator\src\issuer" /I"C:\Users\tobias\AppData\Local\Temp\pacc-roughtime-portable-adapter-20261021" "C:\Projetos\PapinhoAccelerator\src\issuer\roughtime.c" "C:\Projetos\PapinhoAccelerator\tests\roughtime_test.c" C:\Projetos\PapinhoAccelerator\src\issuer\qualified_utc.c /Fert_test.exe /link /INCREMENTAL:NO
if errorlevel 1 exit /b %errorlevel%
rt_test.exe
exit /b %errorlevel%
