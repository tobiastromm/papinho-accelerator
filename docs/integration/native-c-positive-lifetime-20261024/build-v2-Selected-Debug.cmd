@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /std:c11 /DFIXTURE_SELECTED=1 /Od /Zi /MDd /Fobuild/native-c-positive-lifetime-20261024/qualified/Selected/Debug/fixture.obj /Fdbuild/native-c-positive-lifetime-20261024/qualified/Selected/Debug/fixture.pdb /Febuild/native-c-positive-lifetime-20261024/qualified/Selected/Debug/fixture.exe docs/integration/native-c-positive-lifetime-20261024/native-positive-lifetime-fixture.c /link advapi32.lib
exit /b %errorlevel%
