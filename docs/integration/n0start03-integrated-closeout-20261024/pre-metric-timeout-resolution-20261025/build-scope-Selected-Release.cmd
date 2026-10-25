@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
cl /nologo /W4 /WX /TC /O2 /DNDEBUG /MD /Fo"C:\Projetos\PapinhoAccelerator\build\ed11-positive-private-20261025\Selected\Release\census-content-test.obj" /Fe"C:\Projetos\PapinhoAccelerator\build\ed11-positive-private-20261025\Selected\Release\census-content-test.exe" tests/service_n0_census_content_test.c
exit /b %errorlevel%
