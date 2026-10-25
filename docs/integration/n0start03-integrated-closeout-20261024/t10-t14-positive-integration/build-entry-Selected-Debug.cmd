@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /Od /Zi /MDd  /Fobuild/ed11-positive-entry-tests-20261025/Selected/Debug/default.obj /Fdbuild/ed11-positive-entry-tests-20261025/Selected/Debug/default.pdb /Febuild/ed11-positive-entry-tests-20261025/Selected/Debug/default.exe tests/noninherited_positive_profile_test.c /link advapi32.lib
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /Od /Zi /MDd /DPAPACC_N0START03_READONLY_PINS=1 /Fobuild/ed11-positive-entry-tests-20261025/Selected/Debug/readonly.obj /Fdbuild/ed11-positive-entry-tests-20261025/Selected/Debug/readonly.pdb /Febuild/ed11-positive-entry-tests-20261025/Selected/Debug/readonly.exe tests/noninherited_positive_profile_test.c /link advapi32.lib
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /Od /Zi /MDd /DPAPACC_N0START03_PRIVATE_POSITIVE=1 /DPAPACC_N0_PRIVATE_STDOUT=1 /DPAPACC_N0_POSITIVE_PINNED_LAB=1 /Fobuild/ed11-positive-entry-tests-20261025/Selected/Debug/positive.obj /Fdbuild/ed11-positive-entry-tests-20261025/Selected/Debug/positive.pdb /Febuild/ed11-positive-entry-tests-20261025/Selected/Debug/positive.exe tests/noninherited_positive_profile_test.c /link advapi32.lib
if errorlevel 1 exit /b %errorlevel%
exit /b 0
