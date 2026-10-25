@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /Od /Zi /MDd /Fobuild/ed11-positive-entry-tests-20261025/Legacy/Debug/approval.obj /Fdbuild/ed11-positive-entry-tests-20261025/Legacy/Debug/approval.pdb /Febuild/ed11-positive-entry-tests-20261025/Legacy/Debug/approval-current.exe docs/integration/n0start03-integrated-closeout-20261024/pre-metric-timeout-resolution-20261025/approval-reader-test.c
exit /b %errorlevel%
