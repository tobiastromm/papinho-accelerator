@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /std:c11 /DFIXTURE_SELECTED=1 /O2 /DNDEBUG /MD /Fobuild/native-c-premonitor-handle-pressure-20261024/Selected/Release/fixture.obj /Fdbuild/native-c-premonitor-handle-pressure-20261024/Selected/Release/fixture.pdb /Febuild/native-c-premonitor-handle-pressure-20261024/Selected/Release/fixture.exe docs/integration/native-c-premonitor-handle-pressure-20261024/premonitor-fixture.c /link advapi32.lib
exit /b %errorlevel%
