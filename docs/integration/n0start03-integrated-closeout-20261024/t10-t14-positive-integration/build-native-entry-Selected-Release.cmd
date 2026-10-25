@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /D_WIN32_WINNT=0x0603 /O2 /DNDEBUG /MD /Fobuild/ed11-positive-entry-tests-20261025/Selected/Release/ /Fdbuild/ed11-positive-entry-tests-20261025/Selected/Release/entry-native.pdb /Febuild/ed11-positive-entry-tests-20261025/Selected/Release/entry-native.exe tests/noninherited_positive_entry_native_test.c tools/qualification/scm-n0-private-stdout-native.c /link advapi32.lib bcrypt.lib
exit /b %errorlevel%
