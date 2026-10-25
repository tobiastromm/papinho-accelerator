@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /std:c11 /Od /Zi /MDd /Fobuild\premonitor-self-token-lifetime-20261024\native\Legacy\Debug/fixture.obj /Fdbuild\premonitor-self-token-lifetime-20261024\native\Legacy\Debug/fixture.pdb /Febuild\premonitor-self-token-lifetime-20261024\native\Legacy\Debug/fixture.exe docs\integration\premonitor-self-token-lifetime-20261024/self-token-native-fixture.c /link advapi32.lib
exit /b %errorlevel%
