@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /c /DPAPACC_N0START03_READONLY_PINS=1 /DPAPACC_N0_POSITIVE_PINNED_LAB=1 "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-operational-pin-integration-20261024\forbidden-profile.c" /Fo"C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-operational-pin-integration-20261024\forbidden-profile.obj"
exit /b %errorlevel%
