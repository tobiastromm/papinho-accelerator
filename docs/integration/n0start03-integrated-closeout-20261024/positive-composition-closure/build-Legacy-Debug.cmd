@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /std:c11 /D_WIN32_WINNT=0x0603 /DFIXTURE_SELECTED=0 /Od /Zi /MDd /Fobuild/ed11-positive-composition-20261025/Legacy/Debug/ /Fdbuild/ed11-positive-composition-20261025/Legacy/Debug/fixture.pdb /Febuild/ed11-positive-composition-20261025/Legacy/Debug/fixture.exe docs/integration/n0start03-integrated-closeout-20261024/positive-composition-closure/positive-composition.c tools/qualification/scm-n0-private-stdout-native.c /link advapi32.lib psapi.lib bcrypt.lib iphlpapi.lib
exit /b %errorlevel%
