@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
dumpbin /disasm build\ed11-positive-composition-20261025\Selected\Release\positive-composition.obj
exit /b %errorlevel%
