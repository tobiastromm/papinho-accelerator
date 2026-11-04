@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
dumpbin /DEPENDENTS build\phase6-functional-20261028\Selected\Release\papacc_server.exe > docs\integration\phase6-registration-resources-20261031\PE-server.log
exit /b %errorlevel%
