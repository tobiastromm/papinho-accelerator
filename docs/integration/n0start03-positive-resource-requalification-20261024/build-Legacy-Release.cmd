@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /std:c11 /DFIXTURE_SELECTED=0 /O2 /DNDEBUG /MD /Fobuild/n0start03-positive-resource-requalification-20261024/final/Legacy/Release/fixture.obj /Fdbuild/n0start03-positive-resource-requalification-20261024/final/Legacy/Release/fixture.pdb /Febuild/n0start03-positive-resource-requalification-20261024/final/Legacy/Release/fixture.exe docs/integration/n0start03-positive-resource-requalification-20261024/positive-resource-fixture.c /link advapi32.lib psapi.lib
exit /b %errorlevel%
