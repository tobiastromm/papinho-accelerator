@echo off
call "C:\MSVC600-master\VC98\Bin\VCVARS32.BAT"
if errorlevel 1 exit /b %errorlevel%
if not exist "C:\Projetos\PapinhoAccelerator\build\sct-20261021\vc6\Release" mkdir "C:\Projetos\PapinhoAccelerator\build\sct-20261021\vc6\Release"
pushd "C:\Projetos\PapinhoAccelerator\build\sct-20261021\vc6\Release"
cl /nologo /TC /Za /W3 /WX /MD /O2 /I"C:\Projetos\PapinhoAccelerator/src/provisioning/shared_structure" /I"C:\Projetos\PapinhoAccelerator/tests/shared_provisioning/oracles" "C:\Projetos\PapinhoAccelerator/src/provisioning/shared_structure/suite.c" "C:\Projetos\PapinhoAccelerator/src/provisioning/shared_structure/structure.c" "C:\Projetos\PapinhoAccelerator/tests/shared_provisioning/oracles/papinho_papacc_frame.c" "C:\Projetos\PapinhoAccelerator/tests/shared_crypto_boundary/suite_test.c" /Feshared_suite.exe /link /INCREMENTAL:NO
if errorlevel 1 exit /b %errorlevel%
shared_suite.exe
exit /b %errorlevel%
