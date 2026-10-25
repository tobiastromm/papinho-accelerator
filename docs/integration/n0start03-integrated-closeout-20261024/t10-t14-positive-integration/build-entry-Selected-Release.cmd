@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /O2 /DNDEBUG /MD  /Fobuild/ed11-positive-entry-tests-20261025/Selected/Release/default.obj /Fdbuild/ed11-positive-entry-tests-20261025/Selected/Release/default.pdb /Febuild/ed11-positive-entry-tests-20261025/Selected/Release/default.exe tests/noninherited_positive_profile_test.c /link advapi32.lib
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /O2 /DNDEBUG /MD /DPAPACC_N0START03_READONLY_PINS=1 /Fobuild/ed11-positive-entry-tests-20261025/Selected/Release/readonly.obj /Fdbuild/ed11-positive-entry-tests-20261025/Selected/Release/readonly.pdb /Febuild/ed11-positive-entry-tests-20261025/Selected/Release/readonly.exe tests/noninherited_positive_profile_test.c /link advapi32.lib
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /O2 /DNDEBUG /MD /DPAPACC_N0START03_PRIVATE_POSITIVE=1 /DPAPACC_N0_PRIVATE_STDOUT=1 /DPAPACC_N0_POSITIVE_PINNED_LAB=1 /Fobuild/ed11-positive-entry-tests-20261025/Selected/Release/positive.obj /Fdbuild/ed11-positive-entry-tests-20261025/Selected/Release/positive.pdb /Febuild/ed11-positive-entry-tests-20261025/Selected/Release/positive.exe tests/noninherited_positive_profile_test.c /link advapi32.lib
if errorlevel 1 exit /b %errorlevel%
exit /b 0
