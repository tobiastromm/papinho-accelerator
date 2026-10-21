@echo off
call C:\MSVC600-master\VC98\Bin\VCVARS32.BAT
if errorlevel 1 exit /b %errorlevel%
if not exist "C:\Projetos\PapinhoAccelerator\build\evidence-20261021\vc6\Release" mkdir "C:\Projetos\PapinhoAccelerator\build\evidence-20261021\vc6\Release"
pushd "C:\Projetos\PapinhoAccelerator\build\evidence-20261021\vc6\Release"
cl /nologo /TC /Za /W3 /WX /MD /O2 /I"C:\Projetos\PapinhoAccelerator/src/provisioning/shared_structure" "C:\Projetos\PapinhoAccelerator/src/provisioning/shared_structure/m1.c" "C:\Projetos\PapinhoAccelerator/src/provisioning/shared_structure/trust_unavailable.c" "C:\Projetos\PapinhoAccelerator/tests/shared_evidence/evidence_test.c" /Feshared_evidence.exe /link /INCREMENTAL:NO
if errorlevel 1 exit /b %errorlevel%
shared_evidence.exe
exit /b %errorlevel%
