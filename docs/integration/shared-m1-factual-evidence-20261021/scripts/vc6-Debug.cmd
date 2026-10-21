@echo off
call C:\MSVC600-master\VC98\Bin\VCVARS32.BAT
if errorlevel 1 exit /b %errorlevel%
if not exist "C:\Projetos\PapinhoAccelerator\build\sm1-20261021\vc6\Debug" mkdir "C:\Projetos\PapinhoAccelerator\build\sm1-20261021\vc6\Debug"
pushd "C:\Projetos\PapinhoAccelerator\build\sm1-20261021\vc6\Debug"
cl /nologo /TC /Za /W3 /WX /MD /Od /Zi /I"C:\Projetos\PapinhoAccelerator/src/provisioning/shared_structure" "C:\Projetos\PapinhoAccelerator/src/provisioning/shared_structure/m1.c" "C:\Projetos\PapinhoAccelerator/tests/shared_m1/m1_test.c" /Feshared_m1.exe /link /INCREMENTAL:NO
if errorlevel 1 exit /b %errorlevel%
shared_m1.exe
exit /b %errorlevel%
