@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /std:c11 /DWIN32_LEAN_AND_MEAN /D_WIN32_WINNT=0x0A00 /DA1_SELECTED=0 /O2 /DNDEBUG /MD /Fobuild/a1-ownership-reservation-feasibility-20261024/Legacy/Release/a1-model.obj /Fdbuild/a1-ownership-reservation-feasibility-20261024/Legacy/Release/a1-model.pdb /Febuild/a1-ownership-reservation-feasibility-20261024/Legacy/Release/a1-model.exe docs/integration/a1-ownership-reservation-feasibility-20261024/a1-reservation-fixture.c
exit /b %errorlevel%
