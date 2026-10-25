@echo off
call "C:\MSVC600-master\VC98\Bin\VCVARS32.BAT"
if errorlevel 1 exit /b %errorlevel%
cd /d "C:\Users\tobias\AppData\Local\Temp\pacc-roughtime-full-transcript-ed11-20261021\vc6-Debug"
cl /nologo /TC /DRT_LAB_TRANSCRIPT /Za /W4 /WX /FAs /MD /Od /Zi /I"C:\Projetos\PapinhoAccelerator\include" /I"C:\Projetos\PapinhoAccelerator\src\issuer" /I"C:\Projetos\PapinhoAccelerator\tests" "C:\Projetos\PapinhoAccelerator\src\issuer\roughtime.c" "C:\Projetos\PapinhoAccelerator\tests\roughtime_capacity_test.c" C:\Projetos\PapinhoAccelerator\src\issuer\qualified_utc.c /Fert_test.exe /link /INCREMENTAL:NO
if errorlevel 1 exit /b %errorlevel%
rt_test.exe "C:\Projetos\PapinhoAccelerator\tests\fixtures\roughtime-lab-20261021"
exit /b %errorlevel%
