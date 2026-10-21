@echo off
call "C:\MSVC600-master\VC98\Bin\VCVARS32.BAT"
if errorlevel 1 exit /b %errorlevel%
if not exist "C:\Projetos\PapinhoAccelerator\build\shared-provisioning-api-migration-20261020\vc6\Debug" mkdir "C:\Projetos\PapinhoAccelerator\build\shared-provisioning-api-migration-20261020\vc6\Debug"
pushd "C:\Projetos\PapinhoAccelerator\build\shared-provisioning-api-migration-20261020\vc6\Debug"
cl /nologo /TC /Za /W3 /WX /MD /Od /Zi /I"C:\Projetos\PapinhoAccelerator\src\provisioning\shared_structure" /I"C:\Projetos\PapinhoAccelerator\tests\shared_provisioning\oracles" /I"C:\Projetos\PapinhoAccelerator\tests\shared_provisioning" C:\Projetos\PapinhoAccelerator\src\provisioning\shared_structure\structure.c C:\Projetos\PapinhoAccelerator\tests\shared_provisioning\differential_test.c C:\Projetos\PapinhoAccelerator\tests\shared_provisioning\oracles\browser_content.c C:\Projetos\PapinhoAccelerator\tests\shared_provisioning\oracles\papinho_papacc_frame.c C:\Projetos\PapinhoAccelerator\tests\shared_provisioning\oracles\browser_password.c C:\Projetos\PapinhoAccelerator\tests\shared_provisioning\oracles\browser_wipe.c /Festructure_differential.exe /link /INCREMENTAL:NO
if errorlevel 1 exit /b %errorlevel%
structure_differential.exe
exit /b %errorlevel%
